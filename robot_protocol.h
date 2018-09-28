#ifndef __ROBOT_PROTOCOL_H__
#define __ROBOT_PROTOCOL_H__

/*
    로봇 명령어 코드 정의
*/
//RoboCAR LED
#define FRONT_LED			0x07	
#define BACK_LED			0x08

//RoboCAR Buzzer
#define ROBOCAR_BUZZER		0x06

//RoboCAR  Moving
#define STOP    			0x00	//정지
#define FORWARD			0x01	//전진
#define BACKWARD			0x02	// 후진

// RoboCAR Motor control
#define ROBOCAR_LEFTMotor	0x00
#define ROBOCAR_RIGHTMotor	0x01
#define ROBOCAR_ALLMotor	0x03

#define LEFT_ROTATION		0x03
#define RIGHT_ROTATION		0x04

// 각속도 센서(기울기)
#define XY_Degree           0x85

// 이동할 모터 회전수 지정
#define GOAL_WHEEL_CNT		0xC0	

// 로보카 제자리 회전시 이동할 각도지정 명령
#define GOAL_ANGLE		0xC1	
// PSD 
#define PSD_DISTANCE		0x80

//ultrasonic 
#define RIGHT_US            0x81  
#define LEFT_US             0x82  
#define BACK_US             0x83

// 적외선 센서 
#define INFRAREDRAY_DATA	0x84


void RoboCAR_FrontLED_Control(unsigned char led_brightness);
void RoboCAR_BackLED_Control(unsigned char led_brightness);

void RoboCAR_Buzzer_Control(unsigned char buzzer_data);

char RoboCAR_Send_Packet(unsigned char* data);

void RoboCAR_AllMotor_Control(char direction,char MotorSpeed);
void RoboCAR_LeftMotor_Control(char direction,char LeftMotorSpeed);
void RoboCAR_RightMotor_Control(char direction,char RightMotorSpeed);

void RoboCAR_Get_Packet(unsigned char* data);
void RoboCAR_Get_DegreeXY(char* X_deg,char* Y_deg);

void RoboCAR_Move_WheelCount(char direction,char MotorSpeed,unsigned char wheel_cnt);
void RoboCAR_Move_Angle(char direction,char MotorSpeed,unsigned short angle);

unsigned char RoboCAR_Get_PSD(void);
unsigned char RoboCAR_Get_Left_UltraSonic(void);
unsigned char RoboCAR_Get_Right_UltraSonic(void);
unsigned char RoboCAR_Get_Back_UltraSonic(void);

unsigned char RoboCAR_Get_InfraredRay_Data(void);

#endif
