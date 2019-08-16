#ifndef _IP_LOGIC_H_
#define _IP_LOGIC_H_

#include "AnalyseWifi.h"
////////STA//////
typedef struct _ap_info_{
    char ssid[32];
	char password[32];
	int channel;        //信道
    int signal;         //信号强度
}AP_INFO;

AP_INFO ipcam_AP;
AP_INFO nvr_AP;
AP_INFO netcam_AP[5];

AP_INFO self_AP;//hot

typedef struct{//通道信道
	unsigned char chnSignal[64];
}META_WIFI_CHN_SIGNAL;

void IpLogic_Set_Rj45_Conn_Status(int status);

void IpLogic_Set_AP_Conn_Status(int status);

void IpLogic_Set_WifiModule_Status(int status);

void IpLogic_Set_Dhcp_Enable(int status);

void IpLogic_Set_Allow_Nvr_Set_Wifi_Info(int status);

int get_from_menu();//从配置文件里读取的ip
void thread_ipaddr(void);
int thread_check_broadcast(void);
void thread_time(void);
void thread_ap(void);


int IpLogic_Thread_Ap_Create(void);

int IpLogic_Thread_Wave_Create(void);

int IpLogic_Thread_Broadcast_Create(void);

int IpLogic_Thread_Ipaddr_Create(void);

#endif
