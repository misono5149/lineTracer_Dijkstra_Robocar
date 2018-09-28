/*
 * Copyright (C) 2007 Hanback Electronics Inc. (http://www.hanback.co.kr)
 * UART1_API.C
 *
 * API for HBE-KROBO Non Console UART
 *
 * You can use these API for your uart based driver
 *
 *
 * Revision
 * V1.0: 2007/06/25
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <termios.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include "uart_api.h"

#define BAUDRATE    B57600
#define MODEMDEVICE "/dev/ttyST0"

volatile int STOP=FALSE;

void signal_handler_IO(void);   /* signal handler 함수 정의 */
void signal_handler_IO2(void);   /* signal handler 함수 정의 */

int wait_flag=TRUE;		/* signal을 받지 않은 동안은 TRUE */
int wait_flag2=TRUE;		/* signal을 받지 않은 동안은 TRUE */

static struct termios uart_oldtio, uart_newtio;
static struct termios uart_oldtio2, uart_newtio2;

static int uart_dev1 = -1;
static int uart_dev2 = -1;

/*
    시리얼 디바이스 Open 함수 
*/
int user_uart1_open(char *device)
{
    char opendevice[20];
    sprintf(opendevice, "/dev/tty%s", device);
    uart_dev1 = open(opendevice, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (uart_dev1 <0) {perror(opendevice); return -1; }
    return 0;
}

int user_uart2_open(void)
{
    uart_dev2 = open("/dev/ttySAC3", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (uart_dev2 <0) {perror("/dev/ttySAC3"); return -1; }
    return 0;
}


/*
    시리얼 디바이스 Close 함수
*/
void user_uart1_close (void)
{
    tcsetattr(uart_dev1,TCSANOW,&uart_oldtio);
    close(uart_dev1);
}
void user_uart2_close (void)
{
    tcsetattr(uart_dev2,TCSANOW,&uart_oldtio2);
    close(uart_dev2);
}
/*
    터미널 속성을 설정하는 함수
*/
void user_uart1_config(int baud, int bits, int parity, int stops)
{
    struct sigaction saio;           /* signal action의 정의 */

    /* install the signal handler before making the device asynchronous */
    /* 장치를 비동기 모드로 만들기 전에 signal handler */
    saio.sa_handler = signal_handler_IO;
    //saio.sa_mask = 0;
    saio.sa_flags = 0;
    saio.sa_restorer = NULL;
    sigaction(SIGIO,&saio,NULL);

    /* SIGIO signal을 받을 수 있도록 한다. */
    fcntl(uart_dev1, F_SETOWN, getpid());
    /* file descriptor를 비동기로 만든다. (manual page를 보면
	O_APPEND 와 O_NONBLOCK만이 F_SETFL에 사용할 수 있다고 되어 있다.) */
    fcntl(uart_dev1, F_SETFL, FASYNC);

    tcgetattr(uart_dev1,&uart_oldtio); /* save current port settings */

    /* Non canonical 입력처리를 위한 포트 세팅 */
    switch(baud)
    {
	case 9600:    uart_newtio.c_cflag |= B9600; break;
	case 19200:    uart_newtio.c_cflag |= B19200; break;
	case 38400:    uart_newtio.c_cflag |= B38400; break;
	case 57600:    uart_newtio.c_cflag |= B57600; break;
	case 115200:    uart_newtio.c_cflag |= B115200; break;
	default:    uart_newtio.c_cflag |= B0; break;
    }

    switch(bits)
    {
	case 5:  uart_newtio.c_cflag |= CS5; break;
	case 6:  uart_newtio.c_cflag |= CS6; break;
	case 7:  uart_newtio.c_cflag |= CS7; break;
	case 8:  uart_newtio.c_cflag |= CS8; break;
	default: uart_newtio.c_cflag |= CS8; break;
    }

    switch(parity)
    {
	case UART_PAREVEN:
	    uart_newtio.c_cflag |= PARENB;
	    break;
	case UART_PARODD:
	    uart_newtio.c_cflag |= PARENB|PARODD;
	    break;
	default:
	    break;
    }

    switch(stops)
    {
	case UART_STOP_BITS_2:
	    uart_newtio.c_cflag |= CSTOPB;
	    break;
	default:
	    break;
    }

    uart_newtio.c_cflag |= CRTSCTS | CLOCAL | CREAD;
    uart_newtio.c_iflag = IGNPAR;
    uart_newtio.c_oflag = 0;
    uart_newtio.c_lflag = 0;
    //uart_newtio.c_cc[VMIN]=4;
    uart_newtio.c_cc[VMIN]=1;
    uart_newtio.c_cc[VTIME]=0;
    tcflush(uart_dev1, TCIFLUSH);
    tcsetattr(uart_dev1,TCSANOW,&uart_newtio);
}

void user_uart2_config(void)
{
	struct sigaction saio;
	saio.sa_handler = signal_handler_IO2;
	saio.sa_flags = 0;
	saio.sa_restorer = NULL;
	sigaction(SIGIO,&saio,NULL);
	fcntl(uart_dev2, F_SETOWN, getpid());
   fcntl(uart_dev2, F_SETFL, FASYNC);
	tcgetattr(uart_dev2,&uart_oldtio2);
	bzero(&uart_newtio2,sizeof(uart_newtio2));
	uart_newtio2.c_cflag = B57600 | CS8 | CLOCAL | CREAD;
	uart_newtio2.c_iflag = IGNPAR | ICRNL;
	uart_newtio2.c_oflag = 0;
	uart_newtio2.c_lflag = ~(ICANON | ECHO | ECHOE | ISIG);
	uart_newtio2.c_cc[VMIN]=1;
   uart_newtio2.c_cc[VTIME]=0;
   tcflush(uart_dev2, TCIFLUSH);
   tcsetattr(uart_dev2,TCSANOW,&uart_newtio2);
}

int user_uart1_write(unsigned char *ubuf, int size)
{
    return write(uart_dev1, ubuf, size);
}
int user_uart2_write(unsigned char *ubuf, int size)
{
    return write(uart_dev2, ubuf, size);
}


/*
    시리얼로부터 데이터를 읽어들이는 함수
*/
int user_uart1_read(unsigned char *ubuf, int size)
{
    int res=0;

    /* loop while waiting for input. normally we would do something
	useful here */
    while (STOP==FALSE) {

	/* after receiving SIGIO, wait_flag = FALSE, input is available
	    and can be read */
	if (wait_flag==FALSE) {
	    res = read(uart_dev1, ubuf, size);
	    wait_flag = TRUE;      /* wait for new input */
	    break;
	}
    }
    return res;
}

int user_uart2_read(unsigned char *ubuf, int size)
{
    int res=0;
	
    /* loop while waiting for input. normally we would do something
	useful here */
    while (STOP==FALSE) {
	/* after receiving SIGIO, wait_flag = FALSE, input is available
	    and can be read */
		usleep(10);
	if (wait_flag2==FALSE) {
	    res = read(uart_dev2, ubuf, size);	    
	    wait_flag2 = TRUE;      /* wait for new input */
	    break;
	}
    }
    return res;
}
/*
    시그널 처리 함수
*/
void signal_handler_IO (void)
{
    wait_flag = FALSE;
}

void signal_handler_IO2 (void)
{
    wait_flag2 = FALSE;
}


