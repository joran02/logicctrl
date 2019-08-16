#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <pthread.h>
#include <time.h>

#include "cmn.h"
#include "IpLogic.h"
#include "Ioctl_config_ip.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <net/if.h>
#include <time.h>

#include <linux/prctl.h>


#ifndef DBGPRINT
//#define DPRINTK(fmt, args...)   printf("(%s,%d)%s: " fmt,__FILE__,__LINE__, __FUNCTION__ , ## args)
#define DBGPRINT(fmt, args...)   printf("(%s,%d)%s: " fmt,__FILE__,__LINE__, __FUNCTION__ , ## args)
#endif

static int wifi_ch_str[25][2]=
{
36,1,
40,2,
44,3,
48,4,
52,5,
56,6,
60,7,
64,8,
100,9,
104,10,
108,11,
112,12,
116,13,
120,14,
124,6,
128,6,
132,6,
136,6,
140,6,
144,6,
149,6,
153,6,
157,6,
161,6,
165,6,
};

int rj45_connect=0;
int wifi_module=0;
int waveing=0;
int dhcp_enanle=0;
int ap_connect=0;

unsigned char repeater_open=0;

char ip_addr[16]={0};
char ip_netmsk[16] = {0};
char ip_gw[16] = {0};
char ip_ddns1[16] = {0};
char ip_ddns2[16] = {0};
int tcp_port = 0;
char dhcp_ip_addr[16] = {0};
char dhcp_netmsk[16] = {0};
char dhcp_gw[16] = {0};
char dhcp_ddns1[16] = {0};
char idhcp_ddns2[16] = {0};


char NVR_set_IP[16]={0};
int wifi_auto_mode=0;   //0--netcam����"����NVR��������"û��ѡ, 1--��ѡ, �޸ĺ�ǹ��������

////////bylxxx
int dhcp_enable = 0;
int dhcp_status = 0;
time_t now_time;//ȫ��ʱ�亯��

SORT_LIST stList;

///////////////////////////////////////////
AP_INFO ap;
AP_INFO ap_list[128];

AP_INFO relate_ipcam_list[32];

AP_INFO tmp_list[128];


static int IpLogicRunCmd(char *str)
{
	return SendM(str);
}

void speaker(unsigned char force, char *content)
{
	DBGPRINT("%s", content);
	if(force==0 && waveing==1) return;
}

////////////////////////////init params///////////////////////
void IpLogic_Set_Rj45_Conn_Status(int status)
{
	DBGPRINT("RJ45 set\n");
	rj45_connect = status;
}

void IpLogic_Get_Rj45_Conn_Status(int *status)
{
	DBGPRINT("RJ45 get%d__\n", rj45_connect);
	*status = !rj45_connect;
}

void IpLogic_Set_AP_Conn_Status(int status)
{
	DBGPRINT("ap  set\n");
	ap_connect = status;
}

void IpLogic_Get_Sta_Conn_Name(char *sta_name)
{
	strcpy(sta_name, ap.ssid);
}

void IpLogic_Set_WifiModule_Status(int status)
{
	DBGPRINT("*******************module set*******************\n");
	wifi_module = status;
}

////////////////////////////set params///////////////////////
void IpLogic_Set_Dhcp_Enable(int status)
{
	//DBGPRINT("*******************dhcp set**--%d--*****************\n", status);
	dhcp_enable = status;
}


int XdvrCheckIpNetmskGatewayValid(char * pCheckIp, char *pCheckNetmsk, char *pGateway)
{
	int ip_value[4], msk_value[4], gw_value[4];	
	int i = 0;


	if( pCheckIp == NULL )
	{
		DPRINTK("pCheckIp is NULL error\n");
		goto err;
	}

	if( sscanf( pCheckIp,"%d.%d.%d.%d",&ip_value[0],&ip_value[1],&ip_value[2],&ip_value[3]) != 4 )
	{
		DPRINTK("pCheckIp %s  is not ip\n",pCheckIp);
		goto err;
	}
	if( sscanf( pCheckNetmsk,"%d.%d.%d.%d",&msk_value[0],&msk_value[1],&msk_value[2],&msk_value[3]) != 4 )
	{
		DPRINTK("pCheckNetmsk %s  is not ip\n",pCheckNetmsk);
		goto err;
	}else{
		unsigned int b = 0, n[4];  
		sscanf(pCheckNetmsk, "%u.%u.%u.%u", &n[3], &n[2], &n[1], &n[0]);  
		for(i = 0; i < 4; ++i) //�������������32λ�޷�������  
		    b += n[i] << (i * 8);
		b = ~b + 1;
		if((b & (b - 1)) != 0)   //�ж��Ƿ�Ϊ2^n  
		{
			DPRINTK("pCheckNetmsk %s  is not valid\n",pCheckNetmsk);
			goto err;;
		}
	}
	if( sscanf( pGateway,"%d.%d.%d.%d",&gw_value[0],&gw_value[1],&gw_value[2],&gw_value[3]) != 4 )
	{
		DPRINTK("pGateway %s  is not ip\n",pGateway);
		goto err;
	}

	for( i = 0; i < 4; i++)
	{
		if( ( ip_value[i] & msk_value[i] ) !=  ( gw_value[i] & msk_value[i] ) )
		{
			DPRINTK("[%d] ip %d  gw %d  mask %d__\n",i,ip_value[i],gw_value[i] ,msk_value[i]);
			goto err;
		}
	}
	
	
	return 0;
err:
	return ERROR;
}

/*int CheckHaveNetCable(char * pNetAdapter)
{
	struct ifreq ifr;
	int skfd = socket(AF_INET, SOCK_DGRAM, 0);

	if( skfd <= 0 )
	{
		DBGPRINT("Open socket err\n");
		return -1;
	}

	strcpy(ifr.ifr_name, pNetAdapter);
	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
	{
	    close(skfd);
	    return -1;
	}

   	close(skfd);

    	if(ifr.ifr_flags & IFF_RUNNING)
        	return 1;
    	else
		return 0;
}*/

///////////////////////wave///////////////////////////

int receive_wave(void) //�õ���ap_ssid���������˵�������õ��ָ�ȱʡ�������ָ�menu_nvr_ssid������
{
	int i = 0;
	int recvOk = 0;
	//��������
	if(recvOk)
	{
		speaker(1,"waving need to modify wireless info\n"/*���յ�����Ҫ�����SSID���������Ϣ*/);
		strcpy(netcam_AP[0].ssid, ""); //�յ��������õ�ssid������
		strcpy(netcam_AP[0].password, "");
		//������flash

		strcpy(ap.ssid, "");
		return 2;
	}
	return 0;
}

void thread_wave(void)
{
	prctl(PR_SET_NAME, (unsigned long)"thread_wave", 0, 0, 0);
	static int old_time=0;
	old_time=now_time;//read_system_time();
	speaker(1,"start recv waving\n"/*��ʼ��������������Ϣ*/);
	waveing=1;
	while(1)
	{		
		usleep(100);

		if((now_time-old_time)>5*60/*5����*/)
		{
			speaker(1,"overtime, stop recv waving\n"/*��ʱֹͣ��������������Ϣ*/);
			break;
		}
		
		if(rj45_connect==1 || ap_connect!=0)  
		{
			speaker(1,"linked, stop recv waving"/*�������ӣ�ֹͣ�������ա�*/);
			break;
		}
		
		if(receive_wave()) break;	  
	}	
	waveing=0;
		
}

int check_aster_id(char *ssid)
{
	//if is ipc id
	int i = 0, iA = 0, iB = 0, iC = 0;
	int nRet = 1;
	int len = strlen(ssid);
	if (len < 6)
	{
		nRet = 0;
		goto JUDGE_END;
	}

	if (len == 8)
	{
		DBGPRINT("IDstr %s__ready to fliter aster ipc._\n", ssid);
		for (i = 0; i < 8; i++)
		{
			if (i == 0 || i == 1)
			{
				if (ssid[i] != '0')
				{
					//nRet = 0;
					goto SECOND_JUDGE;
				}
			}else{
				if (!isupper(ssid[i]) && !isdigit(ssid[i]))
				{
					//nRet = 0;
					goto SECOND_JUDGE;
				}
			}
		}
		goto JUDGE_END;
	}
SECOND_JUDGE:
	//else if (len > 8)
	{
		DBGPRINT("IDstr %s__ready to fliter aster nvr._\n", ssid);
		for (i = 0; i < len; i++)
		{
			if (isdigit(ssid[i]))
			{
				if (iB == 1)
				{
					iC++;
				}else
				{
					iA = 1;
					iB = 0;
					iC = 0;
				}
			}else if (isupper(ssid[i]))
			{
				if (iC > 0)
				{
					nRet = 0;
					goto JUDGE_END;
				}else if (iA == 1)
				{
					iB = 1;
				}else
				{
					nRet = 0;
					goto JUDGE_END;
				}
			}else
			{
				nRet = 0;
				goto JUDGE_END;
			}
		}

		if (iC != 4)
			nRet = 0;
	}
	//else
	//	nRet = 0;
JUDGE_END:
	return nRet;
}

void search_signal(AP_INFO *pList)
{
	memset(pList, 0, sizeof(AP_INFO)*128);
	int i = 0;
	static unsigned long old_time=0;
	
	//if((now_time-old_time)<6)  return; //ȷ���������>5S

	IpLogicRunCmd("ifconfig ra0 up");
	usleep(200);
	IpLogicRunCmd("/mnt/mtd/iwlist ra0 scanning > /tmp/scan.txt");
	sleep(2);
	if (SortScanData(&stList) < 0)
		return;

	for (i = 0; i < stList.num; i++)
	{
		if (i >= 128)
			break;
		strcpy(pList[i].ssid, stList.member[i].name);
		pList[i].signal = stList.member[i].val;
	}
}

/*int check_apsignal(char *tmpSSID)
{
	//����⵽��ssid��¼��apsignal[100]
	//DBGPRINT("\nCHECK CHECK CHECK apsignal\n");
	//DBGPRINT("\nCHECK CHECK CHECK apsignal\n");

	char cmdStr_ca[256];
	char cmdSteam[256];
	
	memset(cmdStr_ca, 0, sizeof(cmdStr_ca));
	memset(cmdSteam, 0, sizeof(cmdSteam));
	
	sprintf( cmdStr_ca, "iwconfig ra0 | grep '%s'", tmpSSID);
	
	FILE *fp;
	fp = popen(cmdStr_ca, "r");
	if(fp != NULL)
	{
		if(fgets(cmdSteam, 256, fp) != NULL)
		{
			DBGPRINT("cmdSteam CHECK apsignal status: %s\n", cmdSteam);
		}
	}

	pclose(fp);
	
	if(strlen(cmdSteam) > 0)
		{
			printf("---CHECK CHECK CHECK apsignal----ok:%d \n", strlen(cmdSteam));
			return 1;
		}
	else{
			printf("---CHECK CHECK CHECK apsignal----error:%d \n", strlen(cmdSteam));
			return 0;
		}
}*/

int test_connect(char *ssid)
{
	if (check_apsignal(ssid) == 0)
		return 0;
	return 1;
}

void disconect_ap()
{
	IpLogicRunCmd("killall wpa_supplicant");
}

int try_connect(char *ssid, char *pwd)
{
	char cmdStr[256]={0};
	int wait_time = 30, t_conn_ok = 0;

	IpLogicRunCmd("killall wpa_supplicant");
	usleep(500000);
	
	memset(cmdStr, 0, sizeof(cmdStr));
	IpLogicRunCmd("echo 'ctrl_interface=/var/run/wpa_supplicant' > /mnt/mtd/wifi_wpa.conf");
	//sys_command("echo 'ap_scan=1' >> /mnt/mtd/wifi_wpa.conf");
	sprintf( cmdStr, "wpa_passphrase %s %s >> /mnt/mtd/wifi_wpa.conf", ssid, pwd);
	IpLogicRunCmd(cmdStr);
	usleep(500000);

	if (access("/ramdisk/mulInterface", F_OK) == 0)
		IpLogicRunCmd("wpa_supplicant -ira0 -Dwext -c/mnt/mtd/wifi_wpa.conf -b br0 -B &");
	else
		IpLogicRunCmd("wpa_supplicant -ira0 -Dwext -c/mnt/mtd/wifi_wpa.conf -B &");
	sleep(1);

	while(wait_time--)
	{
		if (check_apsignal(ssid) == 1)
		{
			DBGPRINT("try conn success!\n");
			t_conn_ok = 1;
			break;
		}
		if (rj45_connect)
		{
			break;
		}
		DBGPRINT("try conn ing*****!\n");
		sleep(1);
	}

	if (t_conn_ok == 0)
	{
		DBGPRINT("try conn failed!\n");
		//IpLogicRunCmd("killall wpa_supplicant");
		usleep(500000);
	}
	return t_conn_ok;
}

void connect_ap()
{
	int tmp=0;
	static char old_ssid[32]={0};
	if(strcmp(old_ssid, ap.ssid) != 0)
	{		
		if(strlen(old_ssid)  > 0|| strlen(ap.ssid) == 0)
		{
			disconect_ap();
			ap_connect=0;
			
			speaker(0, "stop link wireless router\n"/*ֹͣ��������·����*/);
		}
		
		if(strlen(ap.ssid) > 0)
		{
			tmp=try_connect(ap.ssid, ap.password);
			speaker(0, "start link new wireless router\n"/*��ʼ�����µ�����·����*/);
		}
		strcpy(old_ssid, ap.ssid);
	}	
	else if(strlen(ap.ssid) > 0) tmp=test_connect(ap.ssid);  //����Ƿ�������״̬
	
	if(tmp!=0)
	{
		if(strcmp(ap.ssid, ipcam_AP.ssid) == 0) ap_connect=1;
		else if(strcmp(ap.ssid, nvr_AP.ssid) == 0) ap_connect=2;
		else ap_connect=3;	
	}
	else{
	//	IpLogicRunCmd("killall wpa_supplicant");
		ap_connect=0;
	}
	
}

void auto_ap_setup(void)
{
	int i = 0, j = 0;
	int k=0, m = 0;
	if(strlen(ipcam_AP.ssid) > 0)
	{
		for(i=0;i<128;i++)    //���ȼ���յ����б�����ipcam_AP��NVRָ�����ϼ��м�IPCAM
		{
			if(strcmp(ipcam_AP.ssid, ap_list[i].ssid) == 0)
			{
				strcpy(ap.ssid, ipcam_AP.ssid);
				strcpy(ap.password, "88888888");
				DBGPRINT("CHECK IPCAM_AP IS IN LIST\n");
				k=1;
				break;
			}
		}						
	}
	
	if(k==0)   //�ڶ�����յ����б�����nvr_AP
	{
		if(strlen(nvr_AP.ssid) > 0)
		{
			for(i=0;i<128;i++)  
			{
				if(strcmp(nvr_AP.ssid, ap_list[i].ssid) == 0)
				{
					strcpy(ap.ssid, nvr_AP.ssid);
					strcpy(ap.password, "88888888");
					DBGPRINT("CHECK NVR_AP IS IN LIST\n");
					k=1;
					break;
				}						
			}
		}												
	}
	
	if(k==0)   //��������յ����б������뱾ǹ��ͬ��һ̨nvr��ipcam ap(��ipcam��ָ���ϼ��м�)
	{						
		m=0;
		for(i=0;i<128;i++)
		{
			if(strlen(ap_list[i].ssid) > 0)
			{
				for(j=0;j<32;j++)
				{
					if(strlen(relate_ipcam_list[j].ssid) > 0)
					{
						if(strcmp(relate_ipcam_list[j].ssid, ap_list[i].ssid) == 0)  //Ҳ���кü����������͵�ipcam ap��������
						{
							strcpy(tmp_list[m].ssid, ap_list[i].ssid);
							tmp_list[m].signal=ap_list[i].signal;
							strcpy(tmp_list[m].password, "88888888");
							DBGPRINT("CHECK RELATE_AP NO:%d__%s IS IN LIST\n", m, ap_list[i].ssid);
							m+=1;							
						}
					}
				}
			}
		}
		
		if(m>0)
		{
			if(strlen(ap.ssid) > 0 && ap_connect != 0)
			{
				for(i=0;i<m;i++)      
				{
					if(strcmp(ap.ssid, tmp_list[i].ssid) == 0)  //��ǰ�����Ѿ��Ǳ������ӣ��Ͳ���ap.ssid
					{
						k=1;
						DBGPRINT("CHECK RELATE_AP_%s IS IN LINKED\n", ap.ssid);
						break;
					}
				}
			}
		
			if(k==0)
			{
				strcpy(ap.ssid, "");
				ap.signal=0;
				for(i=0;i<m;i++)  //���������ź���ǿ��
				{
					if(tmp_list[i].signal>ap.signal)
					{
						strcpy(ap.ssid, tmp_list[i].ssid);
						strcpy(ap.password, "88888888");
						ap.signal=tmp_list[i].signal;
						k=1;
					}				
				}
				DBGPRINT("CHECK RELATE_AP IS NOT LINK, ready to link ssid:%s___\n", ap.ssid);
			}
		}						
	}


	if(k==0)  //���ļ���յ����б�����aster��Ʒ
	{
		for(i=0;i<128;i++)
		{
			strcpy(tmp_list[i].ssid,"");
			tmp_list[i].signal=0;
		}
		m=0;					
		for(i=0;i<128;i++)
		{
			if(strlen(ap_list[i].ssid) > 0)
			{
				if(check_aster_id(ap_list[i].ssid))  //Ҳ���кü����������͵��豸��������
				{
					strcpy(tmp_list[m].ssid, ap_list[i].ssid);
					tmp_list[m].signal=ap_list[i].signal;
					strcpy(tmp_list[m].password, "88888888");
					DBGPRINT("CHECK ASTER_AP_%s IN LIST\n", ap_list[i].ssid);
					m+=1;
				}
			}
		}
		
		if(m>0)
		{
			if(strlen(ap.ssid) > 0 && ap_connect != 0)
			{
				for(i=0;i<m;i++)      
				{
					if(strcmp(ap.ssid, tmp_list[i].ssid) == 0)  //��ǰ�����Ѿ��Ǳ������ӣ��Ͳ���ap.ssid
					{
						k=1;
						DBGPRINT("CHECK ASTER_AP_%s IS IN LINKED\n", ap.ssid);
						break;
					}
				}
			}
			if(k==0)
			{
				strcpy(ap.ssid, "");
				ap.signal=0;
				for(i=0;i<m;i++)  //���������ź���ǿ��
				{
					if(tmp_list[i].signal>ap.signal)
					{
						strcpy(ap.ssid, tmp_list[i].ssid);
						strcpy(ap.password, "88888888");
						ap.signal=tmp_list[i].signal;
						k=1;
					}
				}
				DBGPRINT("CHECK ASTER_AP IS NOT LINK, ready to link ssid:%s___\n", ap.ssid);
			}
		}	
		
	}

}

void netcam_ap_setup(void)
{
	int m = 0, i = 0, j = 0, k = 0;
	for(i=0;i<5;i++)
	{
		netcam_AP[i].signal=0;
	}
	m=0;
	for(i=0;i<128;i++)
	{
		if(strlen(ap_list[i].ssid) > 0)
		{
			for(j=0;j<5;j++)
			{
				if(strlen(netcam_AP[j].ssid) > 0)
				{
					if(strcmp(netcam_AP[j].ssid, ap_list[i].ssid) == 0)  //Ҳ���кü���netcam_ap��������
					{
						strcpy(tmp_list[m].ssid, ap_list[i].ssid);
						tmp_list[m].signal=ap_list[i].signal;
						strcpy(tmp_list[m].password, netcam_AP[j].password);
						m+=1;
					}
				}
			}
		}
	}	
	strcpy(ap.ssid, "");
	ap.signal=0;					
	if(m>0)
	{
		for(i=0;i<m;i++)  //���������ź���ǿ��
		{
			if(tmp_list[i].signal>ap.signal)
			{
				strcpy(ap.ssid, tmp_list[i].ssid);
				strcpy(ap.password, tmp_list[i].password);
				ap.signal=tmp_list[i].signal;
				k=1;
			}		
		}
		DBGPRINT("CHECK NETCAM_RELATE_AP IS NOT LINK, ready to link ssid:%s___\n", ap.ssid);
	}	

}

void killrepeater()
{//close repeater
	DBGPRINT("repeater is to close_\n");
	IpLogicRunCmd("killall hostapd");//�ȹر��ٿ���
	sleep(1);
	repeater_open = 0;
}

void openrepeater(AP_INFO *pInfo)
{//open repeater
	int APchannel = -161, t_channel;
	DBGPRINT("hot is to open_\n");
	IpLogicRunCmd("killall hostapd");//�ȹر��ٿ���
	sleep(1);
	if (1)
	{
		FILE *hostapd_conf = NULL;

		hostapd_conf = fopen("/mnt/mtd/hostapd.conf","w+");

		if(hostapd_conf == NULL)
		{
			DBGPRINT("open hostapd.conf error\n");
			return;
		}

	    fprintf(hostapd_conf, "#################### hostapd configuration file #################### \n");
	    fprintf(hostapd_conf, "ctrl_interface=/var/run/hostapd\n");
	    fprintf(hostapd_conf, "interface=ra1\n");			
	    fprintf(hostapd_conf, "bridge=br0\n");			
	    fprintf(hostapd_conf, "ssid=%s\n", pInfo->ssid);

	APchannel = pInfo->channel;

	if(APchannel < 0)
		t_channel = -APchannel;
	else 
		t_channel = APchannel;

	/*if(APchannel == 24)
		t_channel = 165;
	else if(APchannel >= 0 && APchannel < 8)
	{
		t_channel = 36 + APchannel*4;
	}else if(APchannel >=8 && APchannel < 20)
	{
		t_channel = 100 + (APchannel - 8)*4;
	}else if(APchannel >= 20 && APchannel < 24)
	{
		t_channel = 149 + (APchannel - 20)*4;
	}*/

	    fprintf(hostapd_conf, "channel=%d\n", t_channel);	

	    //fprintf(hostapd_conf, "country_code=US\n");
	    fprintf(hostapd_conf, "wpa=2\n");
	    fprintf(hostapd_conf, "wpa_passphrase=%s\n", pInfo->password);
	    fprintf(hostapd_conf, "\n");
	    fprintf(hostapd_conf, "\n");
	    fprintf(hostapd_conf, "#################### default configuration #################### \n");
	    fprintf(hostapd_conf, "driver=rtl871xdrv\n");
	    fprintf(hostapd_conf, "beacon_int=100\n");
	if (t_channel > 14)
	    fprintf(hostapd_conf, "hw_mode=a\n");
	else
	    fprintf(hostapd_conf, "hw_mode=n\n");
	   // fprintf(hostapd_conf, "auth_algs= 1\n");			
	   // fprintf(hostapd_conf, "ieee80211n=1\n");
	   // fprintf(hostapd_conf, "ieee80211d=1\n");
	   // fprintf(hostapd_conf, "wme_enabled=1\n");	//ʡ��ģʽ

	/*ht_way1 = (t_channel - 36) % 8;
	ht_way2 = (t_channel - 40) % 8;
	ht_way3 = (t_channel - 149) % 8;
	ht_way4 = (t_channel - 153) % 8;

	if(t_channel == 165)
		fprintf(hostapd_conf, "ht_capab=[SHORT-GI-40][DSSS_CCK-40]\n");
	else if(ht_way1 == 0 || ht_way3 == 0)
		fprintf(hostapd_conf, "ht_capab=[HT40+][SHORT-GI-40][DSSS_CCK-40]\n");
	else if(ht_way2 == 0 || ht_way4 == 0)
		fprintf(hostapd_conf, "ht_capab=[HT40-][SHORT-GI-40][DSSS_CCK-40]\n");*/

	    fprintf(hostapd_conf, "wpa_key_mgmt=WPA-PSK\n");
	    fprintf(hostapd_conf, "wpa_pairwise=CCMP\n");			
	fprintf(hostapd_conf, "ignore_broadcast_ssid=0\n");//no��ʾssid

	fclose(hostapd_conf);

	IpLogicRunCmd("chmod 777 /mnt/mtd/hostapd.conf");

	DBGPRINT("Current AP channel is --%d--\n", t_channel);
	IpLogicRunCmd("ifconfig ra1 up");
	IpLogicRunCmd("hostapd /mnt/mtd/hostapd.conf -B &");
	}
	repeater_open = 1;
}

void thread_ap(void)	
{
	prctl(PR_SET_NAME, (unsigned long)"thread_ap", 0, 0, 0);
	int wirelessMode = i_is2_4G;// 1-2.4G, 2-5.8G
//	unsigned char repeater_open=0;
	int count=0;
	int i,j,tmp = 0,x = 0;
	int old_channel=0;
	char tmp_ssid[32]={0};
	AP_INFO tmp_AP;
	WIFIAP stwifiap_get[65];
	int tCur_Ap_Signalch = -1;
	char tBak_Ap_Signalch[5] = {0};

	int repeater = 0;
	
	unsigned char old_rj45_connect=0;

	old_rj45_connect=rj45_connect;

	//Sta_load_user_set_ssid_info();

	/*����devctrl�и�ֵ
	self_AP.ssid;                 //�����Լ���Ϊ�����Ĳ���
	strcpy(self_AP.password, "88888888");
	self_AP.channel;*/
	old_channel=self_AP.channel;	

	if(wifi_auto_mode==1)   
	{
		strcpy(netcam_AP[0].ssid, "");//netcam ���ý�.dat�ļ��ڵ�ssid.
		strcpy(netcam_AP[1].ssid, "");
		strcpy(netcam_AP[2].ssid, "");
		strcpy(netcam_AP[3].ssid, "");
		strcpy(netcam_AP[4].ssid, "");
		//ipcam_AP.ssid;       //��flash��ȡ
		strcpy(ipcam_AP.password, "88888888");
		//nvr_AP.ssid;         //��flash��ȡ
		strcpy(nvr_AP.password, "88888888");
	}
	else
	{
		/*strcpy(netcam_AP[0].ssid, "");              //��flash��ȡ5��ssidֵ������
		strcpy(netcam_AP[0].password, "");
		strcpy(netcam_AP[1].ssid, "");
		strcpy(netcam_AP[1].password, "");
		strcpy(netcam_AP[2].ssid, "");
		strcpy(netcam_AP[2].password, "");
		strcpy(netcam_AP[3].ssid, "");
		strcpy(netcam_AP[3].password, "");
		strcpy(netcam_AP[4].ssid, "");
		strcpy(netcam_AP[4].password, "");*/
		
		//strcpy(ipcam_AP.ssid, "");
		//strcpy(nvr_AP.ssid, "");
	}
	
	if(wirelessMode == 1) repeater=0;
	else
	{//��flash����
		//����Ƿ�Ϊ�ȵ�
		if(iw_get_nvr_set("/mnt/mtd/openap.txt", tBak_Ap_Signalch))
		{
			g_Cur_Ap_Signalch = atoi(tBak_Ap_Signalch);
			//repeater = 1;//����ʱ�ж��Ƿ���Ҫ����AP
		}
	}
	
	/*if(repeater==1)
	{
		openrepeater(&self_AP);
	}*/
	
	strcpy(ap.ssid, "");
	
	if(old_rj45_connect==0 && wifi_auto_mode==1)
	{
		if(wirelessMode == 1)
		{
			strcpy(ipcam_AP.ssid, "");
			strcpy(ap.ssid, nvr_AP.ssid);
			strcpy(ap.password, "88888888");
		}
		else
		{
			if (strlen(ipcam_AP.ssid) !=0)
			{
				strcpy(ap.ssid, ipcam_AP.ssid);
				strcpy(ap.password, "88888888");
			}
			else
			{
				if(strlen(nvr_AP.ssid) !=0)
				{
					strcpy(ap.ssid, nvr_AP.ssid);
					strcpy(ap.password, "88888888");
				}
			}	
		}
		connect_ap();
	}
	//sleep(2);
	
	while(1)
	{
		Sta_load_nvr_bc_ssid_info(&stwifiap_get, nvr_AP.ssid);
		tCur_Ap_Signalch = g_Cur_Ap_Signalch;
		repeater = 0;//����б��������Լ����м�
		for(i=0;i<32;i++)
		{
			strcpy(relate_ipcam_list[i].ssid, stwifiap_get[i].channelid);
			if (strcmp(stwifiap_get[i].channelid, myid) == 0)
			{
				strcpy(ipcam_AP.ssid, stwifiap_get[i].wifiapssid);
				if (tCur_Ap_Signalch > 24 || tCur_Ap_Signalch < 0 )
				{
					self_AP.channel = chanChannel[i*2 + 1];    //��ͨ��λ��ȷ���ŵ����߲˵�����				
					printf("_________bc signal is overflow change to %d__\n", self_AP.channel);
				}else if (wifi_ch_str[tCur_Ap_Signalch][0] != self_AP.channel)
				{
					printf("_________signal is change %d to %d__\n", self_AP.channel, wifi_ch_str[tCur_Ap_Signalch][0]);
					self_AP.channel = wifi_ch_str[tCur_Ap_Signalch][0];
				}
			}
			if (strcmp(stwifiap_get[i].wifiapssid, myid) == 0)
			{
				repeater = 1;//��⵽�Լ����м�
			}
		}

		sleep(1);		
//-----------------------�Լ���Ϊ�м���-----2.4G��֧�ִ�ģʽ---------------------
		if(wirelessMode == 1) tmp=0;
		else tmp=repeater;

		if(wifi_auto_mode==0) tmp=0;
		else
		{
			if (old_rj45_connect != 1)
			{
				if(ap_connect!=1 && ap_connect!=2) tmp=0;
				if(ap_connect==1 && strlen(tmp_ssid) > 0 && strcmp(ap.ssid, tmp_ssid) == 0)  ;
				else if(ap_connect==2 && strlen(nvr_AP.ssid) > 0 && strlen(tmp_ssid) == 0 && strcmp(ap.ssid, nvr_AP.ssid) == 0) ;
				else tmp=0;
			}
		}
		
		if(tmp)
		{
			if(repeater_open==0)
			{
				openrepeater(&self_AP); //Ҫ���ŵ�
				speaker(0,"open repeater\n"/*���м�*/);
			}	
		}
		else
		{
			if(repeater_open==1)
			{
				killrepeater();	
				speaker(0,"close repeater\n"/*�ر��м�*/);
			}		
		}
		
		tmp=rj45_connect;
		if(old_rj45_connect==0 && tmp==1)  //�������ߣ�������Ҫ�ı��ŵ�
		{
			old_channel=0;				
		}	
		old_rj45_connect=tmp;
		
		tmp=self_AP.channel;
		if(old_channel!=tmp)   //�Լ���Ϊ�м������ŵ��ı�
		{
			if(repeater_open==1 && old_rj45_connect==1)  //���û�����ߣ��ŵ������ϼ�AP�������ؿ������м�
			{
				killrepeater();
				openrepeater(&self_AP);  //���ŵ����´�repeater
				
				speaker(0,"wireless channel restart set\n"/*�ŵ��ı����´��м�*/);
			}		
			old_channel=tmp;
		}
		

//------------------------------------------------------------------------
		if (x) x--;
		if(old_rj45_connect==0)
		{
		
			if(wifi_auto_mode==1)  //netcam����"����NVR��������"��ѡ
			{
				if(wirelessMode == 1) strcpy(tmp_ssid, "");
				else strcpy(tmp_ssid, ipcam_AP.ssid);
				
				if(ap_connect==1 && strcmp(tmp_ssid, ap.ssid) != 0) strcpy(ap.ssid, "");
				
				if(ap_connect==2 && strcmp(nvr_AP.ssid, ap.ssid) != 0) strcpy(ap.ssid, "");
				
				if(ap_connect==1 && strlen(tmp_ssid) > 0)  ;
				else if(ap_connect==2 && strlen(nvr_AP.ssid) > 0 && strlen(tmp_ssid) == 0) ;
				else if (x == 0)
				{
					search_signal(ap_list);							
					auto_ap_setup();	
					//sleep(5);
					x = 5;
				}			
			}
			else
			{
				tmp=0;
				for(i=0;i<5;i++)
				{
					if(strlen(netcam_AP[i].ssid) !=0) 
					{
						tmp++;
						strcpy(tmp_AP.ssid, netcam_AP[i].ssid);
						strcpy(tmp_AP.password, netcam_AP[i].password);
					}
				}
				
				if(tmp==1)    //ֻ��һ��netcam���õ�ssid, �����ĸ���
				{
					strcpy(ap.ssid, tmp_AP.ssid);
					strcpy(ap.password, tmp_AP.password);
				}				
				else 
				{
					if(ap_connect==0 && x == 0) 
					{
						search_signal(ap_list); 					
						netcam_ap_setup();	
						//sleep(5);
						x = 5;
					}
				}				
			}		
			connect_ap();
		}
		else
		{
			strcpy(ap.ssid, "");
			connect_ap();		
		}
		
	}
}	 
////////////////////////////////////////////
#if 0
void thread_broadcast(void)
{
	prctl(PR_SET_NAME, (unsigned long)"thread_broadcast", 0, 0, 0);
	int i = 0;
	int repeater = 0;
	int getSelfBroadcast = 0;
	int selfAPNeedOpen = 0;
	while(1)
	{
		sleep(3);
		//�㲥�Լ���ID��IP��ַ
		
		//rj45_connect = CheckHaveNetCable("eth0");  //ÿ3�����������߲���
		
		if(getSelfBroadcast/*���յ��Ĺ㲥��������ID��Ϣ*/)
		{
			//���㲥���ݵ�¼���ID��¼������ӵ�ǹ��ID�������flash��ͬ�����µ�flash
			for(i=0;i<32;i++)
			{
				relate_ipcam_list[i].ssid;
			}
			self_AP.channel;    //��ͨ��λ��ȷ���ŵ����߲˵�����
			ipcam_AP.ssid;
			nvr_AP.ssid;
			
			NVR_set_IP;   //¼�������IP�������棬�յ�NVR����IP��ַ��ֹͣDHCP
			
			if(selfAPNeedOpen) //���NVR�㲥Ҫ�󱾻��м̴�
			{
				if(repeater!=1)
				{
					repeater=1;
					//���浽flash					
				}
			}
			else  //���NVR�㲥Ҫ�󱾻��м̹ر�
			{
				if(repeater!=0)
				{
					repeater=0;
					//���浽flash
				}
			}
		}
		

	}
}
#endif
int thread_check_broadcast(void)
{//����AP �㲥
	char channel_broadcast_ap[5];
	int tmpStr_chanChannel;
	char nvr_id[20];
	char cmdStr_bro[256];
	int broadcastCount = 0;
	int currentChannelNum;//record cam current channel
	int flag_getSIG_ok = 0;
	char tmpStr_Sigend[10];

	META_WIFI_CHN_SIGNAL tChnSignalch;
	
	DBGPRINT("\nrecv  ap broadcast\n");
	struct sockaddr_in addrto;
	bzero(&addrto, sizeof(struct sockaddr_in));
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = htonl(INADDR_ANY);
	addrto.sin_port = htons(12301);

	struct sockaddr_in from;
	bzero(&from, sizeof(struct sockaddr_in));
	from.sin_family = AF_INET;
	from.sin_addr.s_addr = htonl(INADDR_ANY);
	from.sin_port = htons(12301);
	
	int sock = -1;
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		printf("socket error.\n");
		return 0;
	}

	const int opt = 1;
	int nb = 0;
	nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	if(nb == -1)
	{
		printf("set socket error...\n");
		return 0;
	}

	if(bind(sock, (struct sockaddr *)&addrto, sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind error.\n");
		return 0;
	}

	int len = sizeof(struct sockaddr_in);
	char smsg[1400] = {0};
	char judgeOK[20];

	const char *token = "\r\n";
	char *p;

	memset(stwifiap_store, 0, sizeof(WIFIAP)*65);
	Sta_load_nvr_bc_ssid_info(&stwifiap_store, NULL);
		
	while(1)
	{	
		len = sizeof(from);
		memset(&from, 0, len);
		memset(channel_broadcast_ap, 0, sizeof(channel_broadcast_ap));
		tmpStr_chanChannel = -1;
		memset(nvr_id, 0, sizeof(nvr_id));
		memset(smsg, 0, sizeof(smsg));
		broadcastCount = 0;

		flag_getSIG_ok = 0;

		memset(stwifiap, 0, sizeof(WIFIAP)*65);
		memset(&tChnSignalch, 0, sizeof(META_WIFI_CHN_SIGNAL));
		
		int ret = recvfrom(sock, smsg, 1400, 0, (struct sockaddr*)&from, (int*)&len);

		if(ret <= 0)
		{
			printf("read error...\n");
		}else
		{
			int i = 0;
			DBGPRINT("recv  broadcast ok.-----------------\n");		
			//printf("Receive : \n%s\n", smsg);
			/*sscanf(smsg, "%s%s%s%s%s%s%s%s", 
				stwifiap[0].channelid, stwifiap[0].wifiapssid, 
				stwifiap[1].channelid, stwifiap[1].wifiapssid, 
				stwifiap[2].channelid, stwifiap[2].wifiapssid, 
				stwifiap[3].channelid, stwifiap[3].wifiapssid);*/
			p = strtok(smsg, token);
			memcpy(channel_broadcast_ap, p, strlen(p));

			p = strtok(NULL, token);
			memcpy(nvr_id, p, strlen(p));

			p = strtok(NULL, token);
			while(p)
				{
					if(i%2 == 0)
						{
							memcpy(stwifiap[i/2].channelid, p, strlen(p));
							//DBGPRINT("set channelid %s location 1.-----------------\n", stwifiap[i/2].channelid);
							memset(judgeOK, 0, sizeof(judgeOK));
							memcpy(judgeOK, stwifiap[i/2].channelid,strlen(stwifiap[i/2].channelid));
							if(strcmp(judgeOK, "BROADCASTEND") == 0)
							{
								strcpy(tmpStr_Sigend, "");
								strncpy(tmpStr_Sigend, p+13+sizeof(META_WIFI_CHN_SIGNAL), 6);
								printf("%s___\n", tmpStr_Sigend);
								if (strcmp(tmpStr_Sigend, "SIGEND") == 0)
								{
									DBGPRINT("recev SIGEND ok_\n");
									memcpy(&tChnSignalch, p+13, sizeof(META_WIFI_CHN_SIGNAL));
									//DBGPRINT("signall:%d--------\n", tChnSignalch.chnSignal[120]);
									flag_getSIG_ok = 1;
								}
								break;
							}
						}
					else
						{
							if(strcmp("nvr", p) == 0)
								memcpy(stwifiap[i/2].wifiapssid, nvr_id, strlen(nvr_id));
							else
								memcpy(stwifiap[i/2].wifiapssid, p, strlen(p));
							//DBGPRINT("set channelid %s location 1.-----------------\n", stwifiap[i/2].wifiapssid);
						}

					if(i == 128)//�������Ϊ64·
						break;
					
					i++;
					p = strtok(NULL, token);
				}

			broadcastCount = i/2;

		if(i <= 128)
			{//avoid loss data
				memset(judgeOK, 0, sizeof(judgeOK));
				memcpy(judgeOK, stwifiap[broadcastCount].channelid,strlen(stwifiap[broadcastCount].channelid));
				if(strcmp(judgeOK, "BROADCASTEND") == 0)
					DBGPRINT("recev completely ok\n");
				else{
					DBGPRINT("recev completely fail!continue, %s\n", judgeOK);
					continue;
					}
			}

	/*	if (memcmp(stwifiap_store, stwifiap, sizeof(stwifiap)) != 0)
		{
			memcpy(stwifiap_store, stwifiap, sizeof(stwifiap));
			Sta_save_nvr_bc_ssid_info(&stwifiap_store);
			if (i_is2_4G)
				IpLogic_Set_hot_SSID(nvr_id, "88888888", 1, 2);
			else
				IpLogic_Set_hot_SSID(nvr_id, "88888888", 161, 2);
		}*/
		
		{//����ap
			DBGPRINT("recv  the broadcast id:%s--num:%d.-----------------\n", nvr_id, broadcastCount);
			for(i = 0; i < broadcastCount; i++)
			{
				if(strcmp(stwifiap[i].channelid, myid) == 0)
					{
						currentChannelNum = i;//��¼ipc��NVRͨ����
						if (memcmp(stwifiap_store, stwifiap, sizeof(WIFIAP)*65) != 0)
						{
							memcpy(stwifiap_store, stwifiap, sizeof(WIFIAP)*65);
							Sta_save_nvr_bc_ssid_info(&stwifiap_store, nvr_id);
						}
						tmpStr_chanChannel = tChnSignalch.chnSignal[currentChannelNum];
						if (flag_getSIG_ok && g_Cur_Ap_Signalch != tmpStr_chanChannel)
						{
							FILE *fp = NULL;
							fp = fopen("/mnt/mtd/openap.txt","w+");
							fprintf(fp, "%d\n", tChnSignalch.chnSignal[i]);
							fclose(fp);
							g_Cur_Ap_Signalch = tmpStr_chanChannel;
							DBGPRINT("ap_sig_ch:%d__\n", g_Cur_Ap_Signalch);
						}
						break;
					}
			}

			if(i == broadcastCount)
			{
				continue;
			}
		}//end ap
		}

		sleep(1);	
	}
	
	return 0;
}
/////////////////////////////sta/////////////////////
int check_ID_In_BCList(char *Ssid)//0:not in.  1:is channel id.   2:is apssid. 3.both 1&2.
{
	int i = 0, ret = 0;
	if (Ssid == NULL)
		return 0;

	for (i = 0; i < 32; i++)
	{
		if (strlen(stwifiap_store[i].channelid) != 0 && strcmp(stwifiap_store[i].channelid, Ssid) == 0)
			ret |= 1;
		if (strlen(stwifiap_store[i].wifiapssid) != 0 && strcmp(stwifiap_store[i].wifiapssid, Ssid) == 0)
			ret |= 1<1;
	}
	DBGPRINT("id :%s is in list:%d___\n",Ssid, ret);
	return ret;
}

int check_ID_In_ScanData(char *Ssid)//0:not in.  1:in.
{
	int i = 0, ret = 0;
	if (Ssid == NULL)
		return 0;

	for (i = 0; i < stList.num; i++)
	{
		if (strlen(stList.member[i].name) != 0 && strcmp(stList.member[i].name, Ssid) == 0)
		{
			if (stList.member[i].val >= 20)
			{
				ret = stList.member[i].val;
				break;
			}
		}
	}
	DBGPRINT("id :%s is in list:%d__,signal:%d__\n",Ssid, ret, stList.member[i].val);
	return ret;
}
////////////////////////////////////////////
time_t get_sysytem_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec;
}

void thread_time(void)
{
	int count = 0;
	int old_wired_status = 0, new_wired_status = 0;
	new_wired_status = CheckHaveNetCable("eth0");
	old_wired_status = rj45_connect;
	now_time=get_sysytem_time();
	while(1)
	{
		sleep(1);
		now_time=get_sysytem_time();

		IpLogic_Set_Dhcp_Enable((NetStaticMenu.IPAlloction&0x01));

		new_wired_status = CheckHaveNetCable("eth0");
		if (old_wired_status != new_wired_status)
		{
			count++;
			if (count > 2)
			{
				count = 0;
				old_wired_status = new_wired_status;
				rj45_connect = old_wired_status;
				DBGPRINT("rj45 status is change to_ %d____\n", rj45_connect);
				if (rj45_connect)
				{
					IpLogicRunCmd("touch /tmp/stopDetect");
					gstCommonParam->GSt_NET_set_ipcam_mode(IPCAM_ID_MODE,"eth0");
					//play_audio_stream("/mnt/mtd/vocal/wireIsOn.pcm");
				}else if (wifi_module)
				{
					IpLogicRunCmd("rm -f /tmp/stopDetect");
					gstCommonParam->GSt_NET_set_ipcam_mode(IPCAM_ID_MODE,"ra0");
					//play_audio_stream("/mnt/mtd/vocal/wireIsOff.pcm");
				}
			}
		}else
			count = 0;
	}
}

char * getMaxStrengthSsid()
{
	int i = 0;
	char ssid[] = "maxStrengthid";

	for (i = 0; i < stList.num; i++)
	{
		if (check_ID_In_BCList(stList.member[i].name) > 0)
		{
			strcpy(ssid, stList.member[i].name);
			break;
		}
	}
	DBGPRINT("getMaxStrengthSsid is to %s_\n", ssid);

	return ssid;
}
////////////////////////////////////////////
void  netcam_set(/*int netcam_ip*/)
{
	//У������
	// netcam_ip���ô��flash, ��Ϊ�ֹ�����IP��ַ
	if(dhcp_status==0)  ip_addr;//=netcam_ip;
}

void kill_dhcp()
{
	DBGPRINT("killdhcp\n");
	IpLogicRunCmd("killall -9 udhcpc");
}

int open_dhcp(char *dev)
{
	int rel = 0;
	char devName[5] = {0};
	char cmd[200] = {0};
	if (dev == NULL)
		return -1;
	else
		strcpy(devName, dev);

	if (access("/ramdisk/mulInterface",F_OK) == 0)
		strcpy(devName, "br0");

	if (strcmp(devName, "br0") == 0)
		strcpy(cmd,"udhcpc -b -i br0 -s /mnt/mtd/simple.script &");
	else if(strcmp(devName, "ra0") == 0)
		strcpy(cmd,"udhcpc -b -i ra0 -s /mnt/mtd/simple.script &");
	else
		strcpy(cmd,"udhcpc -b -i eth0 -s /mnt/mtd/simple.script &");

	printf("dhcpcmd : %s \n",cmd);

	rel = IpLogicRunCmd(cmd);
	if( rel < 0 )
	{
		printf("exec dhcp error!\n");
	}
}

int get_from_menu()
{
	//strcpy(addr, "192.168.17.181");
	char data_Buf1[15],data_Buf2[15],data_Buf3[15],data_Buf4[15],data_Buf5[15];
	//MenuNetStaticInit();
	if (net_cfg_load(NetStaticMenu.IpAddress,NetStaticMenu.GateWay,NetStaticMenu.SubNet,NetStaticMenu.DDNS1,NetStaticMenu.DDNS2,&Wifiinfo.Wififlag)==0)
	{
		sprintf(data_Buf1,"%d.%d.%d.%d",
					NetStaticMenu.IpAddress[0],NetStaticMenu.IpAddress[1],NetStaticMenu.IpAddress[2],NetStaticMenu.IpAddress[3]);
		sprintf(data_Buf2,"%d.%d.%d.%d",
					NetStaticMenu.GateWay[0],NetStaticMenu.GateWay[1],NetStaticMenu.GateWay[2],NetStaticMenu.GateWay[3]);
		sprintf(data_Buf3,"%d.%d.%d.%d",
					NetStaticMenu.SubNet[0],NetStaticMenu.SubNet[1],NetStaticMenu.SubNet[2],NetStaticMenu.SubNet[3]);
		sprintf(data_Buf4,"%d.%d.%d.%d",
					NetStaticMenu.DDNS1[0],NetStaticMenu.DDNS1[1],NetStaticMenu.DDNS1[2],NetStaticMenu.DDNS1[3]);
		sprintf(data_Buf5,"%d.%d.%d.%d",
					NetStaticMenu.DDNS2[0],NetStaticMenu.DDNS2[1],NetStaticMenu.DDNS2[2],NetStaticMenu.DDNS2[3]);
		memcpy(nvrip,data_Buf1,sizeof(data_Buf1));

		strcpy(ip_addr, data_Buf1);
		strcpy(ip_netmsk, data_Buf3);
		strcpy(ip_gw, data_Buf2);
		strcpy(ip_ddns1, data_Buf4);
		strcpy(ip_ddns2, data_Buf5);
		tcp_port = NetStaticMenu.Port;

		
		DBGPRINT("get from menu %s_%s_%s_%s_%s_%d_\n", ip_addr, ip_netmsk, ip_gw,ip_ddns1, ip_ddns2, tcp_port);
	}
	else
		DBGPRINT("get_from_menu error\n");
	
	return 0;
}

int saveipmenu(char *ipaddress,char *gateway,char *netmask,char *ddns1,char *ddns2)
{
	DBGPRINT("save ok!\n");

	int i = 0, a[4];
	IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
	IPCAM_NET_ST * pNetSt = &pAllInfo->net_st;
	
	net_cfg_load(NetStaticMenu.IpAddress,NetStaticMenu.GateWay,NetStaticMenu.SubNet,NetStaticMenu.DDNS1,NetStaticMenu.DDNS2,&Wifiinfo.Wififlag);

	sscanf(ipaddress,"%3d.%3d.%3d.%3d",&a[0],&a[1],&a[2],&a[3]);	
		for (i=0;i<4;i++)
			NetStaticMenu.IpAddress[i] = a[i];
		sscanf(gateway,"%3d.%3d.%3d.%3d",&a[0],&a[1],&a[2],&a[3]);	
		for (i=0;i<4;i++)
			NetStaticMenu.GateWay[i] = a[i];
		sscanf(netmask,"%3d.%3d.%3d.%3d",&a[0],&a[1],&a[2],&a[3]);	
		for (i=0;i<4;i++)
			NetStaticMenu.SubNet[i] = a[i];
		sscanf(ddns1,"%3d.%3d.%3d.%3d",&a[0],&a[1],&a[2],&a[3]);	
		for (i=0;i<4;i++)
			NetStaticMenu.DDNS1[i] = a[i];
		sscanf(ddns2,"%3d.%3d.%3d.%3d",&a[0],&a[1],&a[2],&a[3]);	
		for (i=0;i<4;i++)
			NetStaticMenu.DDNS2[i] = a[i];

{
	U8 tmp[2],tmp1[2];
	tmp[0]=NetStaticMenu.Port>>8;
	tmp[1]=NetStaticMenu.Port;
	tmp1[0]=NetStaticMenu.IEPort>>8;
	tmp1[1]=NetStaticMenu.IEPort;
	I_write_block_eeprom(ADDNSTC,sizeof(NetStaticMenu),&NetStaticMenu.IPAlloction);


	I_write_block_eeprom(ADDNSTC+PORT,2,tmp);
	I_write_block_eeprom(ADDNSTC+IEPORT,2,tmp1);
	I_write_block_eeprom(ADDNSTC+DHCPIP,4,NetStaticMenu.IpAddress);
	I_write_block_eeprom(ADDNSTC+DHCPGATEWAY,4,NetStaticMenu.GateWay);
	I_write_block_eeprom(ADDNSTC+DHCPDNS1,4,NetStaticMenu.DDNS1);
	I_write_block_eeprom(ADDNSTC+DHCPDNS2,4,NetStaticMenu.DDNS2);
}

	if(net_cfg_save(NetStaticMenu.IpAddress,NetStaticMenu.GateWay,NetStaticMenu.SubNet,NetStaticMenu.DDNS1,NetStaticMenu.DDNS2,&Wifiinfo.Wififlag)<0)
	{
		printf("******net_cfg_save false ********\n");
	}
}

void thread_ipaddr(void)
{
	prctl(PR_SET_NAME, (unsigned long)"thread_ipaddr", 0, 0, 0);
	XDVR_NET_CTRL_ST tNetCtrl;
	unsigned char uIp[16], uMsk[16], uGw[16], uDns1[16], uDns2[16];
	int iIs_MulInterface = 0;//if br0
	int ip_IsChange = 0;

	static char old_ip_addr[16]={0};
	static int old_dhcp_enable=0;
	static int new_dhcp_server = 0;
	static int new_dhcp_enable = 0;
	static int new_dhcp_status = 0;
	int local_rj45_conn = 0;
	char tmp[16] = {0};
	
	int dhcp_port=0;    // 1--eth0    2--ra0 

	if (access("/ramdisk/mulInterface",F_OK) == 0)
		iIs_MulInterface = 1;
	
	get_from_menu();

	while(1)
	{	
		strcpy(tmp, NVR_set_IP);
		if(strlen(tmp) > 0)
		{
			if(dhcp_port!=0) 
			{
				kill_dhcp();
				dhcp_port=0;
				
				speaker(0,"nvr is set ipaddr, stop get ip from router\n"/*¼������˵�ַ��ֹͣ��·������ȡ��ַ*/);
			}			
			strcpy(ip_addr, tmp);		
		}
		else
		{
			new_dhcp_enable = dhcp_enable;
			old_dhcp_enable = new_dhcp_enable;

			local_rj45_conn = rj45_connect;
			if(new_dhcp_enable)
			{
				if(ap_connect)
				{
					if(dhcp_port!=2)
					{
						if(dhcp_port!=0) kill_dhcp();				
						open_dhcp("ra0");
						dhcp_port=2;
						
						speaker(0,"try get ip from wireless router\n"/*���Դ�����·������ȡ��ַ*/);
					}
				}
				else
				{
					if(dhcp_port!=1)
					{
						if(dhcp_port!=0) kill_dhcp();				
						open_dhcp("eth0");
						dhcp_port=1;
						speaker(0,"try get ip from wlan router\n"/*���Դ�����·������ȡ��ַ*/);
					}				
				}		

				if (access("/ramdisk/dhcpDone", F_OK) == 0)
				{
					dhcp_status = 1;
					printf("detect dhcp is over now******sleep5s__\n");
					usleep(50000);
				}else
					dhcp_status = 0;

				new_dhcp_status = dhcp_status;

				if(dhcp_status==1)
				{
					;//��ϵͳ��ȡ����IP��ַ
					if (wifi_module && local_rj45_conn == 0 && ap_connect)
						Ioctl_get_ip(dhcp_ip_addr, "ra0");
					else
					{
						Ioctl_get_ip(dhcp_ip_addr, "eth0");
					}
					if(strcmp(dhcp_ip_addr, ip_addr) != 0)
					{
						//У������
						if (strlen(dhcp_ip_addr) > 0)
							strcpy(ip_addr, dhcp_ip_addr);
						
						speaker(0,"get new ip from router\n"/*��·�����õ��µ�ַ*/);
					}
				}						
			}
			else
			{
				if(dhcp_port!=0) 
				{
					kill_dhcp();
					dhcp_port=0;
					
					speaker(0,"stop get ip from router\n"/*ֹͣ��·������ȡ��ַ*/);
				}else
				{
					/*if (access("/ramdisk/camsetDone", F_OK) == 0)
					{
						XDVR_NET_CTRL_ST tNetCtrl;
						if (iIs_MulInterface)
							XdvrGetNetCtrlInfo("br0", &tNetCtrl);
						else
						{
							printf("camsetDone:\n\n");
							XdvrGetNetCtrlInfo("eth0", &tNetCtrl);
						}

						sprintf(uIp, "%d.%d.%d.%d",tNetCtrl.uIp[0],tNetCtrl.uIp[1],tNetCtrl.uIp[2],tNetCtrl.uIp[3]);
						sprintf(uMsk, "%d.%d.%d.%d",tNetCtrl.uNetMask[0],tNetCtrl.uNetMask[1],tNetCtrl.uNetMask[2],tNetCtrl.uNetMask[3]);
						sprintf(uGw, "%d.%d.%d.%d",tNetCtrl.uNetGate[0],tNetCtrl.uNetGate[1],tNetCtrl.uNetGate[2],tNetCtrl.uNetGate[3]);
						sprintf(uDns1, "%d.%d.%d.%d",tNetCtrl.uDns[0][0],tNetCtrl.uDns[0][1],tNetCtrl.uDns[0][2],tNetCtrl.uDns[0][3]);
						sprintf(uDns2, "%d.%d.%d.%d",tNetCtrl.uDns[1][0],tNetCtrl.uDns[1][1],tNetCtrl.uDns[1][2],tNetCtrl.uDns[1][3]);

						printf("camsetDone:%s__tcp_port:%d__\n",uIp, tcp_port);
						if (strcmp(ip_addr, uIp) !=0)
						{
							if (strlen(uIp) > 0)
							{
								gstCommonParam->GST_NET_set_net_parameter(uIp, tcp_port, uGw, uMsk, uDns1, uDns2);
								saveipmenu(uIp,uGw,uMsk,uDns1,uDns2);
								sleep(2);
								IpLogicRunCmd("rm -f /ramdisk/camsetDone");
								strcpy(old_ip_addr, uIp);
							}
						}else
							IpLogicRunCmd("rm -f /ramdisk/camsetDone");
					}*/
					get_from_menu();
				}
			}		
		}

		printf("teest:8-3, dhcp:%d,m:%d__old:%s__rj:%d__ap:%d__\n\n", dhcp_enable, wifi_module, old_ip_addr, local_rj45_conn, ap_connect);
		if(strlen(ip_addr) > 0 && strcmp(old_ip_addr, ip_addr) != 0 
			&& ((local_rj45_conn == 1&&ap_connect == 0) 
			|| (ap_connect && local_rj45_conn == 0)))
		{
			speaker(0,"change new ipaddress\n"/*�����µ������ַ*/);
			//���õ�ַǰ��������豸��ǰ��ַ�������ͬ�������ٴ�����
			//�������ڵ�ַ
			printf("teest:8-4\n\n");
			if (old_dhcp_enable && new_dhcp_status)
			{
				printf("teest:8-5\n\n");
//				sleep(2);
				XDVR_NET_CTRL_ST tNetCtrl;
				if (iIs_MulInterface)
					XdvrGetNetCtrlInfo("br0", &tNetCtrl);
				else
				{
					printf("teest:8-6\n\n");
					XdvrGetNetCtrlInfo("eth0", &tNetCtrl);
				}

				sprintf(uIp, "%d.%d.%d.%d",tNetCtrl.uIp[0],tNetCtrl.uIp[1],tNetCtrl.uIp[2],tNetCtrl.uIp[3]);
				sprintf(uMsk, "%d.%d.%d.%d",tNetCtrl.uNetMask[0],tNetCtrl.uNetMask[1],tNetCtrl.uNetMask[2],tNetCtrl.uNetMask[3]);
				sprintf(uGw, "%d.%d.%d.%d",tNetCtrl.uNetGate[0],tNetCtrl.uNetGate[1],tNetCtrl.uNetGate[2],tNetCtrl.uNetGate[3]);
				sprintf(uDns1, "%d.%d.%d.%d",tNetCtrl.uDns[0][0],tNetCtrl.uDns[0][1],tNetCtrl.uDns[0][2],tNetCtrl.uDns[0][3]);
				sprintf(uDns2, "%d.%d.%d.%d",tNetCtrl.uDns[1][0],tNetCtrl.uDns[1][1],tNetCtrl.uDns[1][2],tNetCtrl.uDns[1][3]);

				printf("teest:tcp_port:%d__\n", tcp_port);
				gstCommonParam->GST_NET_set_net_parameter(uIp, tcp_port, uGw, uMsk, uDns1, uDns2);
				saveipmenu(uIp,uGw,uMsk,uDns1,uDns2);

				//strcpy(old_ip_addr, uIp);
			}else{
				get_from_menu();
				gstCommonParam->GST_NET_set_net_parameter(ip_addr, tcp_port, ip_gw, ip_netmsk, ip_ddns1, ip_ddns2);
				//strcpy(old_ip_addr, ip_addr);
				saveipmenu(ip_addr,ip_gw,ip_netmsk,ip_ddns1,ip_ddns2);
			}
			strcpy(old_ip_addr, ip_addr);
		}
		
		sleep(1);

	}
}
////////////////////////////////////////

//void main(void)
void IpLogic_Main_Start(int iallow_nvr_set_wifi_info)
{
	int tf_card = 0, tf_valid = 0;
	//����Ƿ������߲���
	if(rj45_connect==1) speaker(1,"cable in\n"/*���������߲���*/);
	else speaker(1,"no cable\n"/*δ���������߲���*/);

	//�����������ģ��
	if(wifi_module==0)  speaker(1, "no wireless module\n"/*δ��������ģ��*/);
	else
	{
		speaker(1,"find wireless module\n"/*��������ģ��*/);
		
		if (iallow_nvr_set_wifi_info) //���netcam����"����NVR��������"��ѡ
		{
			wifi_auto_mode=1;  //��flash
			speaker(1,"nvr set mode\n"/*¼�������ģʽ*/);
		}	
		else 
		{
			wifi_auto_mode=0;
			speaker(1,"netcam set mode\n"/*�����������ģʽ*/);
		}
		//���߳�thread_wave
		//IpLogic_Thread_Wave_Create();
		//���߳�thread_ap
		IpLogic_Thread_Ap_Create();
	}
		
/*	���߳�thread_ipaddr*/
	IpLogic_Thread_Ipaddr_Create();
/*	���߳�thread_broadcast*/
	IpLogic_Thread_Broadcast_Create();
	
	//�������TF��  �޿�0  �п�1  ��δ��ʽ����2
	if(tf_card==0) speaker(0,"no tfcard\n"/*δ����TF��*/);	
	else if(tf_card==1)  //�п�
	{
		speaker(0,"find tfcard\n"/*����TF��*/);
		speaker(0,"tfcard start record\n"/*TF����ʼ¼��*/);
	}
	else   //��δ��ʽ����
	{	
		speaker(0,"tfcard is new, need format!\n"/*������TF��,��ʼ��ʽ��*/);
		//���TF���Ƿ�Ƿ���
		if(tf_valid==0) speaker(0,"tfcard is invalid, need change it!\n"/*"�Ƿ�TF���������TF��"*/);
		else speaker(0,"tfcard start record\n"/*TF����ʼ¼��*/);
	}	
}

int IpLogic_Thread_Ap_Create(void)
{
	int ret = 0;
	pthread_t camthreadap = NULL;
	ret = pthread_create(&camthreadap, NULL,(void *) thread_ap,NULL);
	if ( ret < 0 )
	{
		printf("thread_ap create error\n");
		return -1;
	}
	return 1;
}

int IpLogic_Thread_Wave_Create(void)
{
	int ret = 0;
	pthread_t camthreadwave = NULL;
	ret = pthread_create(&camthreadwave, NULL,(void *) thread_wave,NULL);
	if ( ret < 0 )
	{
		printf("thread_wave create error\n");
		return -1;
	}
	return 1;
}

int IpLogic_Thread_Broadcast_Create(void)
{
	int ret = 0;
	pthread_t camthreadBC= NULL;
	ret = pthread_create(&camthreadBC, NULL,(void *) thread_check_broadcast,NULL);
	if ( ret < 0 )
	{
		printf("thread_broadcast create error\n");
		return -1;
	}
	return 1;
}

int IpLogic_Thread_Ipaddr_Create(void)
{
	int ret = 0;
	pthread_t camthreadIpaddr= NULL;
	ret = pthread_create(&camthreadIpaddr, NULL,(void *) thread_ipaddr,NULL);
	if ( ret < 0 )
	{
		printf("thread_ipaddr create error\n");
		return -1;
	}
	return 1;
}