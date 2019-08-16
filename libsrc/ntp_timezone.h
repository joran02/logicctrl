#ifndef _NTP_TIMEZONE_H_
#define _NTP_TIMEZONE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _NTP_TIMEZONE_
{
	int use_ntp;
	int time_zone_sec;   
					   // GMT+8:30 ���ʱ�� time_zone_sec = ntp_time_sec + 8*3600 + 30*60  
					    //GMT-6:00 ���ʱ�� time_zone_sec = ntp_time_sec - 6*3600
	int refress_time_sec;  
					   // �´θ���NTP ������ʱ��
	char ntp_server_url[256];  // ntp �������ĵ�ַ
	int   ntp_server_port;    // ntp�������Ķ˿�
}NTP_TIMEZONE_ST;

typedef void (*ntp_callback)(unsigned long ntp_timezone_time);


/************************************************************************
 ������:start_ntp_server
 ����: ����ntp server
 ����:
 		callback   		��ȡ��ntpʱ��󴥷��Ļص�����
 ���:��
 ����: -1   ʧ��
 ˵��:
************************************************************************/
int start_ntp_server(ntp_callback  callback); 


/************************************************************************
 ������:ntp_set_parameters
 ����: ����ntp �Ĺ���ģʽ
 ����:
 		timezone   		NTP ��������
 ���:��
 ����: -1   ʧ��
 ˵��:
************************************************************************/
int  ntp_set_parameters(NTP_TIMEZONE_ST timezone);

#ifdef __cplusplus
};
#endif

#endif
