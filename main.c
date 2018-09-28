
/*********************************************************
	적외선 센서를 이용한 라인트레이서 예제 프로그램
**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "uart_api.h"
#include "robot_protocol.h"
#include "keyboard.h"

#define M 99999
#define MAX 15
char route[15];
int x=0;
int Dist[15];
char cross[15]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o'};
int weight[15][15]= // road weight
{     // a  b  c  d  e  f  g  h  i  j  k  l  m  n  o
   /*a*/{M, 3, M, M, M, M, M, M, M, M, M, M, M, M, M},
   /*b*/{3, M, 3, M, M, M, M, M, M, M, M, M, M, M, M},
   /*c*/{M, 3, M, 4, M, 2, M, M, M, M, M, M, M, M, M},
   /*d*/{M, M, 4, M, M, M, M, M, M, M, 7, M, M, M, M},
   /*e*/{M, M, M, M, M, 3, M, 5, M, M, M, M, M, M, M},
   /*f*/{M, M, 2, M, 3, M, 2, M, M, M, M, M, M, M, M},
   /*g*/{M, M, M, M, M, 2, M, M, M, 5, M, M, M, M, M},
   /*h*/{M, M, M, M, 5, M, M, M, 2, M, M, M, M, M, M},
   /*i*/{M, M, M, M, M, M, M, 2, M, 3, M, 2, M, M, M},
   /*j*/{M, M, M, M, M, M, 5, M, 3, M, 2, M, M, 4, M},
   /*k*/{M, M, M, 7, M, M, M, M, M, 2, M, M, M, M, 4},
   /*l*/{M, M, M, M, M, M, M, M, 2, M, M, M, 2, M, M},
   /*m*/{M, M, M, M, M, M, M, M, M, M, M, 2, M, 5, M},
   /*n*/{M, M, M, M, M, M, M, M, M, 4, M, M, 5, M, 2},
   /*o*/{M, M, M, M, M, M, M, M, M, M, 4, M, M, 2, M}
};

void Dijakstra(int start, int end){
	int i, j, count, check, position;
	int Min;
	long int Stack[9999];
	int load[MAX];
	int FastWay[MAX];
	int Fastcount;
	int howmany;

	x=0;
	howmany = 0;
	Fastcount = 0;
	count = 0;
	Min = M;

	for(i=0; i<MAX; i++){
		Dist[i]=weight[start][i];
	}
	for(i=0; i<MAX; i++){
		Stack[i]=0;
	}
	while(count!=MAX){
		count++;
		for(i=0; i<MAX; i++){
			for(j=0; j<MAX; j++){
				if(Stack[j]==i){
					check=0;
					break;
				}
				check=1;
			}
			if(check){
				if(Dist[i]<Min){
					Min = Dist[i];
					position = i;
				}
			}
		}
		
		for(i=0; i<MAX; i++){
			if(weight[position][i] + Dist[position] < Dist[i]){
				Dist[i] = weight[position][i] + Dist[position];
				load[i]=position;
			}
		}
		Min=M;
		Stack[count]=position;
	}

	for(i=0; i<MAX; i++){
		if(load[i]<0){
			load[i]=start;
		}
	}

	i=end;
	while(1){
		FastWay[Fastcount++]=i;
		if(i==start)
			break;
		i = load[i];
	}
	howmany++;
	for(i=Fastcount-2; i>0; i--){
		howmany++;
	}
	howmany++;

	printf("최단경로\n");
	printf("%c->", cross[FastWay[Fastcount -1]]);
	route[x++]=cross[FastWay[Fastcount -1]];
	for(i=Fastcount-2; i>0; i--){
		printf("%c", cross[FastWay[i]]);
		printf("->");
		route[x++]=cross[FastWay[i]];
	}
	printf("%c",cross[FastWay[0]]);
	route[x]=cross[FastWay[0]];
	printf("\n소요 거리 : %d", Dist[end]);
	printf("\n거치는 포인트 : %d \n", howmany);
}
//route와 x확인 가능
int main()
{
	int check = 0;
	Dijakstra(0,14);
	int ret;

	unsigned char Line_Value,tmp_line=0xff;

	ret = user_uart1_open("SAC1"); 	
	
	if (ret < 0)
	{
		printf("\n UART1 Open error!");
		exit(0);
	}	

	user_uart1_config(115200, 8, UART_PARNONE, 1);

	init_keyboard();
	 
	printf("**************************\n");
	printf(" RoboCAR Linetracer exam\n");
	printf("**************************\n");
	
	RoboCAR_AllMotor_Control(STOP,0);

	while(1)
	{

		if(kbhit()) break;
		
		usleep(10*1000);

		Line_Value= RoboCAR_Get_InfraredRay_Data();
		
		if(tmp_line == Line_Value) continue;
		
		tmp_line = Line_Value;

		switch(Line_Value){
			
			//전진
			case 0xE7:	// 1110 0111
			case 0xEF:	// 1110 1111
			case 0xF7:	// 1111 0111
			case 0x81:	// 1000 0001
			case 0xC3:	// 1100 0011
				RoboCAR_AllMotor_Control(FORWARD,55);				
			break;
			

			//우회전
			case 0xCF: // 1100 1111
			case 0xDF: // 1101 1111
			case 0x9F: // 1001 1111
			case 0xC7: // 1100 0111
			case 0x87: // 1000 0111
			case 0x83: // 1000 0111
				RoboCAR_LeftMotor_Control(FORWARD,90);
				RoboCAR_RightMotor_Control(BACKWARD,20);
			break;

			//회전 90도
			case 0x00: //0000 0000
			case 0x01: //0000 0001
			case 0x07: //0000 0111
			case 0x03: //0000 0011
			case 0x0F: //0000 1111 
				RoboCAR_LeftMotor_Control(FORWARD,100);
				RoboCAR_RightMotor_Control(BACKWARD,80);
			break;

			//죄회전 90
			case 0x80: //1000 0000
			case 0xC0: //1100 0000
			case 0xE0: //1110 0000 
			case 0xF0: //1111 1001
				RoboCAR_LeftMotor_Control(BACKWARD,80);
				RoboCAR_RightMotor_Control(FORWARD,100);
			break;
			
			//우측으로 후진
			case 0xFC: // 1111 1100
			case 0xFD: // 1111 1101
			case 0xFE: // 1111 1110
			case 0XF1: // 1111 0001
				RoboCAR_LeftMotor_Control(BACKWARD,90);
				RoboCAR_RightMotor_Control(BACKWARD,20);
			break;
			
			//죄회전
			case 0xF3: //1111 0011
			case 0xFB: //1111 1011
			case 0xF9: //1111 1001 
			case 0xE3: //1110 0011
			case 0xE1: //1110 0001
			case 0xC1: //1100 0001
				RoboCAR_LeftMotor_Control(BACKWARD,20);
				RoboCAR_RightMotor_Control(FORWARD,90);
			break;
			
			//좌측으로 후진	
			case 0x3F: //0011 1111
			case 0xBF: //1011 1111
			case 0x7F: //0111 1111
			case 0x8F: //1000 1111
				RoboCAR_LeftMotor_Control(FORWARD,20);
				RoboCAR_RightMotor_Control(BACKWARD,90);		
			break;

			// 정지
			case 0xFF:			
				RoboCAR_AllMotor_Control(BACKWARD,40);
				RoboCAR_Move_Angle(4, 100, 180);
			break;	
		}// end switch

					
	}//end while		
	
	RoboCAR_AllMotor_Control(STOP,0);
	
	user_uart1_close();
	close_keyboard();

	return 0;
}


