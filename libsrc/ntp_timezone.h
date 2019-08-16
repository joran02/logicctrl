#ifndef _NTP_TIMEZONE_H_
#define _NTP_TIMEZONE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _NTP_TIMEZONE_
{
	int use_ntp;
	int time_zone_sec;   
					   // GMT+8:30 这个时区 time_zone_sec = ntp_time_sec + 8*3600 + 30*60  
					    //GMT-6:00 这个时区 time_zone_sec = ntp_time_sec - 6*3600
	int refress_time_sec;  
					   // 下次更新NTP 服务器时间
	char ntp_server_url[256];  // ntp 服务器的地址
	int   ntp_server_port;    // ntp服务器的端口
}NTP_TIMEZONE_ST;

typedef void (*ntp_callback)(unsigned long ntp_timezone_time);


/************************************************************************
 函数名:start_ntp_server
 功能: 开启ntp server
 输入:
 		callback   		获取到ntp时间后触发的回调函数
 输出:无
 返回: -1   失败
 说明:
************************************************************************/
int start_ntp_server(ntp_callback  callback); 


/************************************************************************
 函数名:ntp_set_parameters
 功能: 设置ntp 的工作模式
 输入:
 		timezone   		NTP 工作参数
 输出:无
 返回: -1   失败
 说明:
************************************************************************/
int  ntp_set_parameters(NTP_TIMEZONE_ST timezone);

#ifdef __cplusplus
};
#endif

#endif
