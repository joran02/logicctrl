#include "cmn.h"

#define    TTY_DEV    "/dev/ttyAMA"    /*�˿�·��*/ 
#define TIMEOUT_SEC(buflen,baud) (buflen*20/baud+2)  /*���ճ�ʱ*/ 
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
 *    ������ת����������ȷ���Ƿ���ȷ�� 
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
 *    fdcom: �����ļ���������pportinfo: �����õĶ˿���Ϣ����ȷ�ϣ� 
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
    /*------------���ö˿�����----------------*/ 
    /*baudrates*/ 
    baudrate = convbaud(pportinfo -> baudrate); 
    cfsetispeed(&termios_new, baudrate);        /*���봮������˵Ĳ�����*/ 
    cfsetospeed(&termios_new, baudrate);        /*���봮������˵Ĳ�����*/ 
    termios_new.c_cflag |= CLOCAL;            /*����ģʽ����֤���򲻻��Ϊ�˿ڵ�ռ����*/ 
    termios_new.c_cflag |= CREAD;            /*����ģʽ��ʹ�ܶ˿ڶ�ȡ���������*/ 
 
    /* ����ģʽ��flow control*/ 
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
 
    /*����ģʽ��data bits*/ 
    termios_new.c_cflag &= ~CSIZE;        /*����ģʽ�������ַ���Сλ*/ 
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
 
    /*����ģʽ parity check*/ 
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
 
    /*����ģʽ��stop bits*/ 
    stopbit = pportinfo -> stopbit; 
    if(stopbit == '2'){ 
        termios_new.c_cflag |= CSTOPB;    /*2 stop bits*/ 
    } 
    else{ 
        termios_new.c_cflag &= ~CSTOPB;    /*1 stop bits*/ 
    } 
 
    /*other attributions default*/ 
    termios_new.c_oflag &= ~OPOST;            /*���ģʽ��ԭʼ�������*/ 
    termios_new.c_cc[VMIN]  = 1;            /*�����ַ�, ��Ҫ��ȡ�ַ�����С����*/ 
    termios_new.c_cc[VTIME] = 1;        /*�����ַ�, ��ȡ��һ���ַ��ĵȴ�ʱ��    unit: (1/10)second*/ 
 
    tcflush(fdcomk, TCIFLUSH);                /*��������ݿ��Խ��գ�������*/ 
    tmp = tcsetattr(fdcomk, TCSANOW, &termios_new);    /*���������ԣ�TCSANOW�����иı�������Ч*/   
    tcgetattr(fdcomk, &termios_old); 
    return(tmp); 
} 
 
/******************************************* 
 *    Open serial port 
 *    tty: �˿ں� ttyS0, ttyS1, .... 
 *    ����ֵΪ�����ļ������� 
********************************************/ 
int PortOpen(pportinfo_t pportinfo) 
{ 
    int fdcomk;    /*�����ļ�������*/ 
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
 *    fdcom: ������������data: ���������ݣ�datalen: ���ݳ��� 
 *    ����ʵ�ʷ��ͳ��� 
*********************************************/ 
int PortSend(int fdcomk, char *data, int datalen) 
{ 
    int len = 0; 
     printf("PortSend len: %d \n",datalen);
    len = write(fdcomk, data, datalen);    /*ʵ��д��ĳ���*/ 
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

