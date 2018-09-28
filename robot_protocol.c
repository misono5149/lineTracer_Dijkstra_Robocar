#include <stdio.h>
#include <string.h>
#include "robot_protocol.h"
#include "uart_api.h"

/*
	Send a Packet to RoboCAR 
*/
char RoboCAR_Send_Packet(unsigned char* data)
{
	unsigned char CheckSum=0;	
	
	CheckSum = ((unsigned char)(data[1]+data[2]+data[3]+data[4]))&0xFE;

	data[5] = CheckSum;

	user_uart1_write(data,7);
	usleep(1000);

	return 0;
}


void RoboCAR_FrontLED_Control(unsigned char led_brightness)
{
	unsigned char packet[7]={0xFF,0,0,0,0,0,0xAA};  
	packet[1]	=	FRONT_LED;
	packet[2]	=	led_brightness;

	RoboCAR_Send_Packet(packet);
}	

void RoboCAR_BackLED_Control(unsigned char led_brightness)
{
	unsigned char packet[7]={0xFF,0,0,0,0,0,0xAA};  
	packet[1]	=	BACK_LED;
	packet[2]	=	led_brightness;

	RoboCAR_Send_Packet(packet);
}	

void RoboCAR_Buzzer_Control(unsigned char buzzer_data)
{
	unsigned char packet[7]={0xFF,0,0,0,0,0,0xAA};  
	packet[1]	=	ROBOCAR_BUZZER;
	packet[2]	=	buzzer_data;

	RoboCAR_Send_Packet(packet);
}	

void RoboCAR_AllMotor_Control(char direction,char MotorSpeed)
{
	unsigned char packet[7]={0xFF,0,0,0,0,0,0xAA};  
	packet[1] = ROBOCAR_ALLMotor;	
	packet[2] = MotorSpeed;
      packet[3] = direction; 
	
	RoboCAR_Send_Packet(packet);
}

void RoboCAR_LeftMotor_Control(char direction,char LeftMotorSpeed)
{
	unsigned char packet[7]={0xFF,0,0,0,0,0,0xAA};  
	packet[1] = ROBOCAR_LEFTMotor;	
	packet[2] = LeftMotorSpeed;
	packet[3] = direction; 

	packet[5] = ((unsigned char)(packet[1]+packet[2]+packet[3]+packet[4]))&0xFE; 
	
	RoboCAR_Send_Packet(packet);

}

void RoboCAR_RightMotor_Control(char direction,char RightMotorSpeed)
{
	unsigned char packet[7]={0xFF,0,0,0,0,0,0xAA};  
	packet[1] = ROBOCAR_RIGHTMotor;	
	packet[2] = RightMotorSpeed;
	packet[3] = direction; 

	packet[5] = ((unsigned char)(packet[1]+packet[2]+packet[3]+packet[4]))&0xFE; 
	
	RoboCAR_Send_Packet(packet);
}
void RoboCAR_Get_Packet(unsigned char* data)
{
	user_uart1_read(data,7);
}

void RoboCAR_Get_DegreeXY(char* X_deg,char* Y_deg)
{
	unsigned char packet[7]={0xFF,0,0,0,0,0,0xAA};  
	unsigned char Recv_data[7];
	
	packet[1]	=	XY_Degree;
		
	RoboCAR_Send_Packet(packet);

	RoboCAR_Get_Packet(Recv_data);  	

	if(Recv_data[1]==XY_Degree)
	{
		*X_deg = Recv_data[2];
		*Y_deg = Recv_data[3];
	}

}


void RoboCAR_Move_WheelCount(char direction,char MotorSpeed,unsigned char wheel_cnt)
{
	unsigned char packet[7]={0xFF,0x00,0,0,0,0,0xAA};  

	packet[1] = GOAL_WHEEL_CNT;
      packet[2] = wheel_cnt; 

	RoboCAR_Send_Packet(packet);

	RoboCAR_AllMotor_Control(direction,MotorSpeed);	

}
void RoboCAR_Move_Angle(char direction,char MotorSpeed,unsigned short angle)
{
	unsigned char packet[7]={0xFF,0x00,0,0,0,0,0xAA};  
	
	packet[1] = GOAL_ANGLE;
      packet[2] = (angle>>8);
      packet[3] = angle&0xff;  

	RoboCAR_Send_Packet(packet);
	
	if(direction == RIGHT_ROTATION)
	{
		RoboCAR_LeftMotor_Control(FORWARD,MotorSpeed);
		RoboCAR_RightMotor_Control(BACKWARD,MotorSpeed);
	}
	else{
		RoboCAR_LeftMotor_Control(BACKWARD,MotorSpeed);
		RoboCAR_RightMotor_Control(FORWARD,MotorSpeed);
	}
}

unsigned char RoboCAR_Get_PSD(void)
{
	unsigned char packet[7]={0xFF,0,0,0,0,0,0xAA};  
	unsigned char Recv_data[7];
	
	packet[1] = PSD_DISTANCE;
	
	RoboCAR_Send_Packet(packet);

	RoboCAR_Get_Packet(Recv_data);  	

	if(Recv_data[1]==PSD_DISTANCE)
	{
		return Recv_data[2];
	}
	return 0;
  	
}


unsigned char RoboCAR_Get_Left_UltraSonic(void)
{
	unsigned char packet[7]={0xFF,0,0,0,0,0,0xAA};  
	unsigned char Recv_data[7];
	
	packet[1]	=	LEFT_US ;
	
	RoboCAR_Send_Packet(packet);

	RoboCAR_Get_Packet(Recv_data);  	

	if(Recv_data[1]==LEFT_US)
	{
		return Recv_data[2];
	}
	return 0;
}
unsigned char RoboCAR_Get_Right_UltraSonic(void)
{
	unsigned char packet[7]={0xFF,0,0,0,0,0,0xAA};  
	unsigned char Recv_data[7];
	
	packet[1]	=	RIGHT_US;
	
	RoboCAR_Send_Packet(packet);

	RoboCAR_Get_Packet(Recv_data);  	

	if(Recv_data[1]==RIGHT_US)
	{
		return Recv_data[2];
	}
	return 0;
}

unsigned char RoboCAR_Get_Back_UltraSonic(void)
{
	unsigned char packet[7]={0xFF,0,0,0,0,0,0xAA};  
	unsigned char Recv_data[7];
	
	packet[1]	=	BACK_US;
	
	RoboCAR_Send_Packet(packet);

	RoboCAR_Get_Packet(Recv_data);  	

	if(Recv_data[1]==BACK_US)
	{
		return Recv_data[2];
	}
	return 0;
}


unsigned char RoboCAR_Get_InfraredRay_Data(void)
{
	unsigned char packet[7]={0xFF,0,0,0,0,0,0xAA};  
	unsigned char Recv_data[7];
	
	packet[1]	=	INFRAREDRAY_DATA;

	RoboCAR_Send_Packet(packet);

	RoboCAR_Get_Packet(Recv_data);  	


	if(Recv_data[1]==INFRAREDRAY_DATA)
	{
		return (signed char)Recv_data[2];
	}
	return 0;
}


