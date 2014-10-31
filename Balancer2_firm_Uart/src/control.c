#include "Balancer2.h"

volatile static double Tspd_L_i = 0.0,Tspd_R_i = 0.0;	//移動速度積分用変数

/*
 * 制御用編雨数の初期化
 */
void ClearVariables(){
	ClearGyroVariables();
	ClearEncVariables();
	ClearCurrentVariables();
	Tspd_L_i = 0.0;
	Tspd_R_i = 0.0;
}

/*
 * キャリブレーション
 */
unsigned char calibrarion(){
	static int64_t currenttemp[2] = {0L,0L};
	static int64_t gyrosum = 0L;

	//変数初期化
	if(memmap.values.CALIBRATISON_CNT == 0){
		memmap.values.BODY_ANGULAR_SPD_OFFSET = 0;
		memmap.values.CURRENT_OFFSET_L = 0;
		memmap.values.CURRENT_OFFSET_R = 0;

		gyrosum = 0L;
		currenttemp[0] = 0L;
		currenttemp[1] = 0L;
	}

	//加算
	{
		gyrosum += (int64_t)memmap.values.GYRO_DATA;
		currenttemp[0] += (int64_t)memmap.values.M_CURRENT_L;
		currenttemp[1] += (int64_t)memmap.values.M_CURRENT_R;
	}
	memmap.values.CALIBRATISON_CNT++;

	//終了処理
	if((int)memmap.values.CALIBRATISON_CNT >= (int)(memmap.values.CALIBRATISON_TIME * MAIN_CYCLE)){
		uint32_t num = memmap.values.CALIBRATISON_TIME * MAIN_CYCLE;
		memmap.values.BODY_ANGULAR_SPD_OFFSET = (double)gyrosum/num;
		memmap.values.CURRENT_OFFSET_L = (short)(currenttemp[0]/num);
		memmap.values.CURRENT_OFFSET_R = (short)(currenttemp[1]/num);

		gyrosum = 0L;
		currenttemp[0] = 0L;
		currenttemp[1] = 0L;
		memmap.values.CALIBRATISON_CNT = 0;
		return 1;
	}
	return 0;
}


/*
 * 倒立制御
 */
void Control(){
	volatile double wheel_L_angle,wheel_R_angle;
	volatile double wheel_L_angular_spd,wheel_R_angular_spd;
	volatile double outL = 0.0,outR = 0.0;

	//本体角度フィードバック
	outL += memmap.values.GAIN_BODY * memmap.values.GAIN_BODY_ANGLE * memmap.values.BODY_ANGLE;
	outR += memmap.values.GAIN_BODY * memmap.values.GAIN_BODY_ANGLE * memmap.values.BODY_ANGLE;

	//本体角速度フィードバック
	outL += memmap.values.GAIN_BODY * memmap.values.GAIN_BODY_ANGULAR_SPD * memmap.values.BODY_ANGULAR_SPD;
	outR += memmap.values.GAIN_BODY * memmap.values.GAIN_BODY_ANGULAR_SPD * memmap.values.BODY_ANGULAR_SPD;

	//ホイール目標回転角速度→目標回転角度
	Tspd_L_i += (memmap.values.T_SPD_L/MAIN_CYCLE);
	Tspd_R_i += (memmap.values.T_SPD_R/MAIN_CYCLE);

	//ホイール角度、角速度算出
	wheel_L_angle = memmap.values.WHEEL_ANGLE_L - Tspd_L_i;
	wheel_R_angle = memmap.values.WHEEL_ANGLE_R - Tspd_R_i;
	wheel_L_angular_spd = memmap.values.WHEEL_ANGULAR_SPD_L - (memmap.values.T_SPD_L/MAIN_CYCLE);
	wheel_R_angular_spd = memmap.values.WHEEL_ANGULAR_SPD_R - (memmap.values.T_SPD_R/MAIN_CYCLE);

	//ホイール角度フィードバック
	outL += memmap.values.GAIN_WHEEL * memmap.values.GAIN_WHEEL_ANGLE * wheel_L_angle;
	outR += memmap.values.GAIN_WHEEL * memmap.values.GAIN_WHEEL_ANGLE * wheel_R_angle;

	//ホイール角速度フィードバック
	outL += memmap.values.GAIN_WHEEL * memmap.values.GAIN_WHEEL_ANGULAR_SPD * wheel_L_angular_spd;
	outR += memmap.values.GAIN_WHEEL * memmap.values.GAIN_WHEEL_ANGULAR_SPD * wheel_R_angular_spd;

	//左右ホイール間の角度差算出
	double correlation = wheel_L_angle - wheel_R_angle;

	//左右ホイール間の角度差フィードバック
	outL -= memmap.values.GAIN_WHEEL_CORRELATION * memmap.values.GAIN_WHEEL_CORRELATION_ANGLE * correlation;
	outR += memmap.values.GAIN_WHEEL_CORRELATION * memmap.values.GAIN_WHEEL_CORRELATION_ANGLE * correlation;

	//左右ホイール間の角速度差算出
	correlation = wheel_L_angular_spd - wheel_R_angular_spd;

	//左右ホイール間の角速度差フィードバック
	outL -= memmap.values.GAIN_WHEEL_CORRELATION * memmap.values.GAIN_WHEEL_CORRELATION_ANGULAR_SPD * correlation;
	outR += memmap.values.GAIN_WHEEL_CORRELATION * memmap.values.GAIN_WHEEL_CORRELATION_ANGULAR_SPD * correlation;

	//最大指令値規制
	if(outL > 32767.0){
		outL = 32767.0;
	}
	if(outR > 32767.0){
		outR = 32767.0;
	}
	if(outL < -32767.0){
		outL = -32767.0;
	}
	if(outR < -32767.0){
		outR = -32767.0;
	}

	//電流指令値設定
	memmap.values.T_CURRENT_L = (short)outL;
	memmap.values.T_CURRENT_R = (short)outR;
}

//VS-C3操縦の最大速度
#define VS_C3_MOVE_MAX_SPEED 9.42477796076937972 //rad/sec = 540deg/sec

//VS-C3操縦の速度設定
void VS_C3Control(){
	double out_L = 0.0,out_R = 0.0;

	out_L = -(double)(signed char)(memmap.values.PAD_AN_LY) * VS_C3_MOVE_MAX_SPEED/128.0;
	out_R = -(double)(signed char)(memmap.values.PAD_AN_LY) * VS_C3_MOVE_MAX_SPEED/128.0;
	out_L += (double)(signed char)(memmap.values.PAD_AN_RX) * VS_C3_MOVE_MAX_SPEED/128.0;
	out_R -= (double)(signed char)(memmap.values.PAD_AN_RX) * VS_C3_MOVE_MAX_SPEED/128.0;

	memmap.values.T_SPD_L = out_L;
	memmap.values.T_SPD_R = out_R;
}

//最大速度規制値
#define WHEELANGURARPSD_MAX (20.0*m_PI)	//3600deg/sec

//最大速度超過していないか確認（転倒検出）
unsigned char isWhleeOverSpeed(){
	unsigned char overspd = 0;
	static double old_L = 0.0,old_R = 0.0;
	if((memmap.values.WHEEL_ANGLE_L - old_L) > WHEELANGURARPSD_MAX/MODE_CYCLE){
		overspd++;
	}
	else if((memmap.values.WHEEL_ANGLE_L - old_L) < -WHEELANGURARPSD_MAX/MODE_CYCLE){
		overspd++;
	}
	if((memmap.values.WHEEL_ANGLE_R - old_R) > WHEELANGURARPSD_MAX/MODE_CYCLE){
		overspd++;
	}
	else if((memmap.values.WHEEL_ANGLE_R - old_R) < -WHEELANGURARPSD_MAX/MODE_CYCLE){
		overspd++;
	}
	if(overspd != 0){
		old_L = 0.0;
		old_R = 0.0;
		return 1;
	}
	old_L = memmap.values.WHEEL_ANGLE_L;
	old_R = memmap.values.WHEEL_ANGLE_R;
	return 0;
}
