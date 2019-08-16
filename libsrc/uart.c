#include "cmn.h"

#define    TTY_DEV    "/dev/ttyAMA"    /*端口路径*/ 
#define TIMEOUT_SEC(buflen,baud) (buflen*20/baud+2)  /*接收超时*/ 
#define TIMEOUT_USEC 1000 
int fdcom=-1;
typedef struct{
    char    prompt;        /*prompt after reciving data*/
    int     baudrate;        /*baudrate*/
    char    databit;        /*data bits, 5, 6, 7, 8*/
    char     debug;            /*debug mode, 0: none, 1: debug*/
    char     echo;            /*echo mode, 0: none, 1: echo*/
    char    fctl;            /*flow control, 0: none, 1: hardware, 2: software*/
    char     tty;            /*tty: 0, 1, 2, 3, 4, 5, 6, 7*/
    char    parity;        /*parity 0: none, 1: odd, 2: even*/
    char    stopbit;        /*stop bits, 1, 2*/
    const int reserved;        /*reserved, must be zero*/
}portinfo_t;
typedef portinfo_t *pportinfo_t;

portinfo_t portinfo ={ 
    '0',                              /* print prompt after receiving*/ 
     9600,                          /* baudrate: 9600*/ 
     '8',                              /* databit: 8*/ 
     '0',                              /* debug: off*/ 
     '0',                              /* echo: off*/ 
     '0',                              /* flow control: software*/ 
     '1',                              /* default tty: COM*/ 
     '0',                              /* parity: none*/ 
     '1',                              /* stopbit: 1*/ 
      0                              /* reserved*/ 
}; 

char *get_ptty(pportinfo_t pportinfo) 
{ 
    char *ptty; 
 
    switch(pportinfo->tty){ 
        case '0':{ 
          ptty = TTY_DEV"0"; 
        }break; 
        case '1':{ 
            ptty = TTY_DEV"1"; 
        }break; 
        case '2':{ 
            ptty = TTY_DEV"2"; 
        }break; 
	default:
		 ptty = TTY_DEV"0"; 
		 break;
    } 
    return(ptty); 
} 
 
/******************************************* 
 *    波特率转换函数（请确认是否正确） 
********************************************/ 
int convbaud(unsigned long int baudrate) 
{ 
    switch(baudrate){ 
        case 1200: 
            return B1200; 		
        case 2400: 
            return B2400; 
        case 4800: 
            return B4800; 
        case 9600: 
            return B9600; 
        case 19200: 
            return B19200; 
        case 38400: 
            return B38400; 
        case 57600: 
            return B57600; 
        case 115200: 
            return B115200; 
        default: 
            return B9600; 
    } 
} 
 
/******************************************* 
 *    Setup comm attr 
 *    fdcom: 串口文件描述符，pportinfo: 待设置的端口信息（请确认） 
 * 
********************************************/ 
int PortSet(int fdcomk, const pportinfo_t pportinfo) 
{ 
    struct termios termios_old, termios_new; 
    int     baudrate, tmp; 
    char    databit, stopbit, parity, fctl; 
 
    bzero(&termios_old, sizeof(termios_old)); 
    bzero(&termios_new, sizeof(termios_new)); 
    cfmakeraw(&termios_new); 
    tcgetattr(fdcomk, &termios_old);            /*get the serial port attributions*/ 
    /*------------设置端口属性----------------*/ 
    /*baudrates*/ 
    baudrate = convbaud(pportinfo -> baudrate); 
    cfsetispeed(&termios_new, baudrate);        /*填入串口输入端的波特率*/ 
    cfsetospeed(&termios_new, baudrate);        /*填入串口输出端的波特率*/ 
    termios_new.c_cflag |= CLOCAL;            /*控制模式，保证程序不会成为端口的占有者*/ 
    termios_new.c_cflag |= CREAD;            /*控制模式，使能端口读取输入的数据*/ 
 
    /* 控制模式，flow control*/ 
    fctl = pportinfo-> fctl; 
    switch(fctl){ 
        case '0':{ 
            termios_new.c_cflag &= ~CRTSCTS;        /*no flow control*/ 
        }break; 
        case '1':{ 
            termios_new.c_cflag |= CRTSCTS;            /*hardware flow control*/ 
        }break; 
        case '2':{ 
            termios_new.c_iflag |= IXON | IXOFF |IXANY;    /*software flow control*/ 
        }break; 
    } 
 
    /*控制模式，data bits*/ 
    termios_new.c_cflag &= ~CSIZE;        /*控制模式，屏蔽字符大小位*/ 
    databit = pportinfo -> databit; 
    switch(databit){ 
        case '5': 
            termios_new.c_cflag |= CS5; 
        case '6': 
            termios_new.c_cflag |= CS6; 
        case '7': 
            termios_new.c_cflag |= CS7; 
        default: 
            termios_new.c_cflag |= CS8; 
    } 
 
    /*控制模式 parity check*/ 
    parity = pportinfo -> parity; 
    switch(parity){ 
        case '0':{ 
            termios_new.c_cflag &= ~PARENB;        /*no parity check*/ 
        }break; 
        case '1':{ 
            termios_new.c_cflag |= PARENB;        /*odd check*/ 
            termios_new.c_cflag &= ~PARODD; 
        }break; 
        case '2':{ 
            termios_new.c_cflag |= PARENB;        /*even check*/ 
            termios_new.c_cflag |= PARODD; 
        }break; 
    } 
 
    /*控制模式，stop bits*/ 
    stopbit = pportinfo -> stopbit; 
    if(stopbit == '2'){ 
        termios_new.c_cflag |= CSTOPB;    /*2 stop bits*/ 
    } 
    else{ 
        termios_new.c_cflag &= ~CSTOPB;    /*1 stop bits*/ 
    } 
 
    /*other attributions default*/ 
    termios_new.c_oflag &= ~OPOST;            /*输出模式，原始数据输出*/ 
    termios_new.c_cc[VMIN]  = 1;            /*控制字符, 所要读取字符的最小数量*/ 
    termios_new.c_cc[VTIME] = 1;        /*控制字符, 读取第一个字符的等待时间    unit: (1/10)second*/ 
 
    tcflush(fdcomk, TCIFLUSH);                /*溢出的数据可以接收，但不读*/ 
    tmp = tcsetattr(fdcomk, TCSANOW, &termios_new);    /*设置新属性，TCSANOW：所有改变立即生效*/   
    tcgetattr(fdcomk, &termios_old); 
    return(tmp); 
} 
 
/******************************************* 
 *    Open serial port 
 *    tty: 端口号 ttyS0, ttyS1, .... 
 *    返回值为串口文件描述符 
********************************************/ 
int PortOpen(pportinfo_t pportinfo) 
{ 
    int fdcomk;    /*串口文件描述符*/ 
    char *ptty; 
 
    ptty = get_ptty(pportinfo); 
    fdcomk = open(ptty, O_RDWR | O_NOCTTY | O_NONBLOCK); 

	printf("port=%d\n",getpid());
 
    return (fdcomk); 
} 
 
/******************************************* 
 *    Close serial port 
********************************************/ 
void PortClose(int fdcomk) 
{ 
    close(fdcomk); 
} 
 
/******************************************** 
 *    send data 
 *    fdcom: 串口描述符，data: 待发送数据，datalen: 数据长度 
 *    返回实际发送长度 
*********************************************/ 
int PortSend(int fdcomk, char *data, int datalen) 
{ 
    int len = 0; 
     printf("PortSend len: %d \n",datalen);
    len = write(fdcomk, data, datalen);    /*实际写入的长度*/ 
    if(len == datalen){ 
        return (len); 
    } 
    else{ 
        tcflush(fdcomk, TCOFLUSH); 
        return -1; 
    } 
} 


void setuart(int baudrat)
{
	if(fdcom<0)
	{
		fdcom = PortOpen(&portinfo); 
		portinfo.baudrate=Baud[PtzMenu[0].BaudRate];
		printf("PortSet baudrates:%d\n",portinfo.baudrate);
		PortSet(fdcom, &portinfo);
	}
	else
	{
		portinfo.baudrate=baudrat;
		printf("PortSet baudrates:%d\n",portinfo.baudrate);
		PortSet(fdcom, &portinfo);
	}
}

void uartsend(char * msg, int len)
{
	if(fdcom<0)
	{
		portinfo.baudrate = Baud[PtzMenu[0].BaudRate];
		fdcom = PortOpen(&portinfo); 
		portinfo.baudrate=Baud[PtzMenu[0].BaudRate];
		printf("PortSet baudrates:%d\n",portinfo.baudrate);
		PortSet(fdcom, &portinfo);
	}
	else
	{
		PortSend(fdcom,msg,len);
		usleep(5000);
	}
}

