/*
 * uart_api.h
 */

#ifndef __UART_API_H__
#define __UART_API_H__

#define FALSE 0
#define TRUE 1

#define UART_PARNONE	0	/* Defines for setting parity */
#define UART_PARODD	1
#define UART_PAREVEN	2

#define UART_STOP_BITS_1    0
#define UART_STOP_BITS_2    1


/* UART API Function Prototypes */

int user_uart1_open (char *device);
void user_uart1_close (void);
void user_uart1_config(int baud, int bits,int parity, int stops);
int user_uart1_write(unsigned char *ubuf, int size);
int user_uart1_read(unsigned char *ubuf, int size);

int user_uart2_open(void);
void user_uart2_close (void);
void user_uart2_config(void);
int user_uart2_write(unsigned char *ubuf, int size);
int user_uart2_read(unsigned char *ubuf, int size);

#endif // __UART_API_H__
