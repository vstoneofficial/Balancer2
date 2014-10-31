#include "Balancer2.h"
#include "control.h"
#include <stdio.h>

void LedBlink(unsigned char light,unsigned char blink , int msec,int callcycle);

/*
 * main関数
 */
int main (void)
{
	Balancer2Init();
	UARTInit(CBR_115200,non,1);
	UARTTrace("Power on");					//UARTで送信

	CurrentControlDisable();				//モータの電流制御を無効
	setLED(3);								//LED1,2を点灯

	while(getSW() == 0){					//ボタンが押されるのを待つ
		CycleProc();
	}
	while(getSW() == 1){					//ボタンが離されるのを待つ
		__NOP();
	}

	setLED(2);								//LED2を点灯
	Wait(1000);								//ボタンを押した振動が収まるまで待つ

	//キャリブレーション
	UARTTrace("calibrarion start");			//UARTで送信
	while(calibrarion() == 0){
		CycleProc();			//制御プロセス　MAIN_CYCLE「Hz］でループを同期。センサの取得、モータの電流制御などを行う
		LedBlink(0,2,150,MAIN_CYCLE);		//LED2を150msec周期で点滅
	}
	ClearVariables();						//制御用変数の初期化
	UARTTrace("calibrarion end");			//UARTで送信

	while(getSW() == 0){					//ボタンが押されるのを待つ
		unsigned char data = 0;
		CycleProc();						//制御プロセス　MAIN_CYCLE「Hz］でループを同期
		LedBlink(2,1,1000,MAIN_CYCLE);		//LED2を点灯、LED1を1000msec周期で点滅
		UARTReadByte(&data);				//1バイト受信待ち
		if(data == 's'){					//sを受信したら動作開始
			break;
		}
	}
	while(getSW() == 1){					//ボタンが離されるのを待つ
		__NOP();
	}

	UARTTrace("control start");
	setLED(2);							//LED2を点灯
	ClearVariables();					//制御用変数の初期化
	CurrentControlEnable();				//モータの電流制御を有効

	while(1){
		unsigned char updateVS_C3 = CycleProc();		//制御プロセス　MAIN_CYCLE[Hz]でループを同期。MODE_CYCLE[Hz]で戻り値が１になるので、その際に更新頻度の少なくて良い処理を実行する
		Control();						//ボタンが押されるのを待つ
		if(updateVS_C3 == 1){					//MODE_CYCLE[Hz]で実行
			int length = 0;
			char str[30];
			length = sprintf(str,"bodyangle:%.5lf\r\n",&memmap.values.BODY_ANGLE);	//本体角度の文字列は配列に格納
			UARTSend((unsigned char*)str,length);									//UARTで送信
			if(getSW() == 1){			//ボタンが押されたら停止
				while(getSW() == 1){	//ボタンが離されるのを待つ
					__NOP();
				}
				break;
			}
		}
	}
	CurrentControlDisable();			//モータの電流制御を無効

	UARTTrace("control end");			//UARTで送信
	while(1){
		CycleProc();		//制御プロセス　MAIN_CYCLE[Hz]でループを同期。MODE_CYCLE[Hz]で戻り値が１になるので、その際に更新頻度の少なくて良い処理を実行する
	}
}

/*
 * LEDの点滅制御関数
 * light　		点灯させるLED
 * blink		点滅させるLED
 * cycle		点滅周期(msec）
 * callcycle	関数の呼ばれる周波数（Hz)
 */
void LedBlink(unsigned char light,unsigned char blink , int msec,int callcycle){
	static int cnt = 0;
	volatile int c = (msec * callcycle/1000);
	cnt ++;

	if(cnt <  c/2){
		setLED(light | blink);
	}
	else{
		setLED(light);
	}
	if(cnt > c){
		cnt = 0;
	}
}
