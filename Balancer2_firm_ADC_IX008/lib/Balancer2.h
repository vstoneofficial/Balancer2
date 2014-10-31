#ifndef __BALANCER2_H__
#define __BALANCER2_H__

/*
 * include
 */
#include <stdint.h>
#include "lpc13xx.h"

/*
 * define
 */
#define MAIN_CYCLE (500)		//メイン制御サイクル　Hz
#define CURRENT_CYCLE (1000)	//電流制御サイクル　Hz
#define MODE_CYCLE (100)		//VS－C3など優先順位の低い制御サイクル　Hz
#define ENCODER_CYCLE (100000) 	//エンコーダ更新サイクル	Hz
#define PWM_MAX_VALUE (1000)	//モータPWM最大値

#define m_PI (3.14159265358979324)

#define MODE_CURRENT_CTL_EN (0x01)	//電流制御モード
#define MODE_VSC3_DIS 		(0x80)	//VS-C3　無効取得フラグ

/*
 * Memory Map
 */
#define MEMMAP_SIZE 352

union TYPE_MEMMAP{
	uint8_t u8[MEMMAP_SIZE];
	uint16_t u16[MEMMAP_SIZE/sizeof(uint16_t)];
	uint32_t u32[MEMMAP_SIZE/sizeof(uint32_t)];
	uint64_t u64[MEMMAP_SIZE/sizeof(uint64_t)];
	int16_t i16[MEMMAP_SIZE/sizeof(int16_t)];
	int32_t i32[MEMMAP_SIZE/sizeof(int32_t)];
	int64_t i64[MEMMAP_SIZE/sizeof(int64_t)];
	double d[MEMMAP_SIZE/sizeof(double)];
	struct MAP{
		unsigned short Product_ID;
		unsigned char Version;
		unsigned char MODE;
		short M_CURRENT_L;
		short M_CURRENT_R;
		short T_CURRENT_L;
		short T_CURRENT_R;
		short CURRENT_OFFSET_L;
		short CURRENT_OFFSET_R;
		double BODY_ANGULAR_SPD;
		double BODY_ANGLE;
		double BODY_ANGULAR_SPD_OFFSET;
		double WHEEL_ANGULAR_SPD_L;
		double WHEEL_ANGULAR_SPD_R;
		double WHEEL_ANGLE_L;
		double WHEEL_ANGLE_R;
		double T_SPD_L;
		double T_SPD_R;
		long long ENC_L;
		long long ENC_R;
		short GYRO_DATA;
		unsigned short ADC_C_LA;
		unsigned short ADC_C_LB;
		unsigned short ADC_C_RA;
		unsigned short ADC_C_RB;
		unsigned short PAD_BTN;
		char PAD_AN_RX;
		char PAD_AN_RY;
		char PAD_AN_LX;
		char PAD_AN_LY;
		double USER_ARIA1;
		double USER_ARIA2;
		double USER_ARIA3;
		double USER_ARIA4;
		double USER_ARIA5;
		double USER_ARIA6;
		double USER_ARIA7;
		double USER_ARIA8;
		unsigned short CALIBRATISON_CNT;
		unsigned char CALIBRATISON_TIME;
		unsigned char LOG_MODE;
		unsigned short LOG_DIV;
		unsigned short LOG_CNT;
		unsigned long long LOG_FLG_1;
		unsigned long long LOG_FLG_2;
		unsigned long long LOG_FLG_3;
		double GAIN_BODY;
		double GAIN_WHEEL;
		double GAIN_WHEEL_CORRELATION;
		double GAIN_BODY_ANGLE;
		double GAIN_BODY_ANGULAR_SPD;
		double GAIN_WHEEL_ANGLE;
		double GAIN_WHEEL_ANGULAR_SPD;
		double GAIN_WHEEL_CORRELATION_ANGLE;
		double GAIN_WHEEL_CORRELATION_ANGULAR_SPD;
		double GAIN_CURRENT_P;
		double GAIN_CURRENT_I;
		double GAIN_GYRO_HPF_COF;
		double GAIN_OPTION1;
		double GAIN_OPTION2;
		double GAIN_OPTION3;
		double GAIN_OPTION4;
		double GAIN_OPTION5;
		double GAIN_OPTION6;
	}values;
}memmap;

/*
 * Common
 */
/*
 * 関数		Balancer2Init	電流制御有効
 * 引数		なし
 * 戻値		なし
 */
void Balancer2Init();

/*
 * 関数		setLED	電流制御有効
 * 引数		value	　bit0　オレンジLED　,　bit1　赤LED
 * 戻値		なし
 */
void setLED(unsigned char value);

/*
 * 関数		CycleProc	電流制御有効
 * 引数		なし
 * 戻値		0：停止中	1：取得中
 */
unsigned char CycleProc();

/*
 * 関数		getSW	スイッチ状態取得
 * 引数		なし
 * 戻値		0：	1：押下
 */
unsigned char getSW();

/*
 * 関数		Wait	時間待ち
 * 引数		msec	待ち時間をmsec単位で指定
 * 戻値		なし
 */
void Wait(unsigned int msec);

/*
 * Senser
 */
/*
 * 関数		CurrentControlEnable	電流制御有効
 * 引数		なし
 * 戻値		なし
 */
void CurrentControlEnable();

/*
 * 関数		CurrentControlDisable	電流制御無効
 * 引数		なし
 * 戻値		なし
 */
void CurrentControlDisable();

/*
 * 関数		ClearEncVariables	エンコーダ用変数初期化
 * 引数		なし
 * 戻値		なし
 */
void ClearEncVariables();

/*
 * 関数		ClearCurrentVariables	電流制御用変数初期化
 * 引数		なし
 * 戻値		なし
 */
void ClearCurrentVariables();

/*
 * 関数		ClearGyroVariables	ジャイロセンサ用変数初期化
 * 引数		なし
 * 戻値		なし
 */
void ClearGyroVariables();


/*
 * Log
 */
/*
 * 関数		LogStart	ログ取得開始
 * 引数		なし
 * 戻値		なし
 */
void LogStart();

/*
 * 関数		LogStop		ログ取得終了
 * 引数		なし
 * 戻値		なし
 */
void LogStop();

/*
 * 関数		isLogRecoding	ログ取得中か確認
 * 引数		なし
 * 戻値		0：停止中	1：取得中
 */
uint8_t isLogRecoding();

/*
 * 関数		isLogEnable		ログモードが有効か確認
 * 引数		なし
 * 戻値		0：無効	1：有効
 */
unsigned char isLogEnable();

/*
 * 関数		setLogEnable	ログモードを有効に設定
 * 引数		なし
 * 戻値		なし
 */
void setLogEnable();

/*
 * 関数		setLogDisable	ログモードを無効に設定
 * 引数		なし
 * 戻値		なし
 */
void setLogDisable();

/*
 * 関数		setLogFlag	ログフラグを設定
 * 引数		*val		設定する変数をメモリマップ変数を（memmap.values.BODY_ANGLE）な形式で指定。
 * 			size		変数のバイト数
 * 戻値		なし
 */
void setLogFlag(uint8_t * val , uint16_t size);

/*
 * 関数		clearLogFlagAll	ログフラグをクリア
 * 引数		なし
 * 戻値		なし
 */
void clearLogFlagAll();



/*
 * ADC VS-IX008B
 */
/*
 * 関数		getIX008	VS-IX008からADCデータ取得
 * 引数		num			ポート番号		［0～7］
 * 戻値		12bitADCデータ		［0～4095］
 */
uint16_t getIX008(uint8_t num);


/*
 * UART
 */
enum eBoardRate{
	CBR_2400 = 2400,
	CBR_4800 = 4800,
	CBR_9600 = 9600,
	CBR_14400 = 14400,
	CBR_19200 = 19200,
	CBR_31250 = 31250,
	CBR_38400 = 38400,
	CBR_57600 = 57600,
	CBR_115200 = 115200
};

enum eParity{
	odd,
	even,
	non
};

/*
 * 関数		UARTInit	UART初期設定
 * 引数		baudrate	ボーレートを設定		［列挙子eBoardRateから選択］
 * 			parity		パリティ設定		［列挙子eParityから選択］
 * 			stop		ストップビット設定		［1or2］
 * 戻値		なし
 */
void UARTInit(uint32_t baudrate,uint8_t parity ,uint8_t stop);

/*
 * 関数		UartSendByte	1バイト送信
 * 引数		data			バイトデータ
 * 戻値		なし
 */
void UARTSendByte(unsigned char data);

/*
 * 関数		UARTReadByte	1バイト送信
 * 引数		*data			受信データ格納用変数のポインタ
 * 戻値		0：受信データなし	1：受信データあり
 */
unsigned char UARTReadByte(unsigned char *data);

/*
 * 関数		UARTSend		複数バイト送信
 * 引数		*BufferPtr		送信データ配列のポインタ
 * 			Length		送信データ配列の長さ
 * 戻値		なし
 */
void UARTSend(uint8_t *BufferPtr, uint32_t Length);

/*
 * 関数		UARTTrace		トレース送信（CR+LFコードを自動負荷）
 * 引数		*BufferPtr		送信文字列のポインタ
 * 戻値		なし
 * 備考		文字列最後の'\0'までを自動送信し、改行コードを最後に送信
 */
void UARTTrace(char *BufferPtr);


/*
 * モータ制御
 */
/*
 * 関数		SetMotorPwm		モータへのPWM指令値設定
 * 引数		m1,m2			モータへのPWM指令（-1000～1000）
 * 戻値		なし
 */
void SetMotorPwm(int m1, int m2);


#endif
