#include "cmn.h"

#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h> 

ValWifi Wifiinfo;
Valnetwifi netwifi;

ValNetStatic NetStaticMenu;
NETMENU netmenupic;
U8 CheckCodeflag=0;

#define NETSTATICDATA	  72


U16 Menu_NetStatic_DataPos[]=
{
	252,58,
	252,84,	264,84,	276,84,		300,84,	312,84,	324,84,		348,84,	360,84,	372,84,		396,84,	408,84,	420,84,
	252,110,	264,110,	276,110,		300,110,	312,110,	324,110,		348,110,	360,110,	372,110,		396,110,	408,110,	420,110,
	252,136,	264,136,	276,136,		300,136,	312,136,	324,136,		348,136,	360,136,	372,136,		396,136,	408,136,	420,136,
	252,162,	264,162,	276,162,		300,162,	312,162,	324,162,		348,162,	360,162,	372,162,		396,162,	408,162,	420,162,
	252,188,	264,188,	276,188,		300,188,	312,188,	324,188,		348,188,	360,188,	372,188,		396,188,	408,188,	420,188,
	252,214,	264,214,	276,214,	288,214,
	252,240,	264,240,	276,240,	288,240,

		34,274,	156,274,	312,274
};

U16 Pre_netpos[] = 
{
	252,58,		432,82,
		
	252,84,		264,108,
	264,84,		276,108,
	276,84,		288,108,
	
	300,84,		312,108,
	312,84,		324,108,
	324,84,		336,108,
	
	348,84,		360,108,
	360,84,		372,108,
	372,84,		384,108,

	396,84,		408,108,
	408,84,		420,108,
	420,84,		432,108,
	

	252,110,		264,134,
	264,110,		276,134,
	276,110,		288,134,
	
	300,110,		312,134,
	312,110,		324,134,
	324,110,		336,134,
	
	348,110,		360,134,
	360,110,		372,134,
	372,110,		384,134,

	396,110,		408,134,
	408,110,		420,134,
	420,110,		432,134,


	252,136,		264,160,
	264,136,		276,160,
	276,136,		288,160,
	
	300,136,		312,160,
	312,136,		324,160,
	324,136,		336,160,
	
	348,136,		360,160,
	360,136,		372,160,
	372,136,		384,160,

	396,136,		408,160,
	408,136,		420,160,
	420,136,		432,160,


	252,162,		264,186,
	264,162,		276,186,
	276,162,		288,186,
	
	300,162,		312,186,
	312,162,		324,186,
	324,162,		336,186,
	
	348,162,		360,186,
	360,162,		372,186,
	372,162,		384,186,

	396,162,		408,186,
	408,162,		420,186,
	420,162,		432,186,


	252,188,		264,212,
	264,188,		276,212,
	276,188,		288,212,
	
	300,188,		312,212,
	312,188,		324,212,
	324,188,		336,212,
	
	348,188,		360,212,
	360,188,		372,212,
	372,188,		384,212,

	396,188,		408,212,
	408,188,		420,212,
	420,188,		432,212,


	252,214,		264,238,
	264,214,		276,238,
	276,214,		288,238,
	288,214,		300,238,
	
	252,240,		264,264,
	264,240,		276,264,
	276,240,		288,264,
	288,240,		300,264,

	34,274,		148,298,
	156,274,		316,298,
	312,274,		472,298
};

U8 getip[4],getgateway[4],getsubway[4],getddns1[4],getddns2[4];
U8 wrflagnet = 0;
U8 autodns1[4],autodns2[4];
U8 netdhcpget=0;
char nvrip[15];

int command_sys(char * command);

#define NET_CFG_FILENAME "/mnt/mtd/net_cfg.dat"
#if  0//put into yb
int net_cfg_save(unsigned char *ipaddress,unsigned char *gateway,unsigned char *netmask,unsigned char *ddns1,unsigned char *ddns2,unsigned char *Wififlag)
{
	int ret;
	FILE *fp;
	
	fp = fopen(NET_CFG_FILENAME, "w + b");
	if(fp == NULL)
	{
		printf("open %s fail \n",NET_CFG_FILENAME);
		return -1;
	}
	ret = fwrite(ipaddress, 1, 4, fp);
	if(ret != 4)
		goto net_save_err;
	ret = fwrite(gateway, 1, 4, fp);
	if(ret != 4)
		goto net_save_err;
	ret = fwrite(netmask, 1, 4, fp);
	if(ret != 4)
		goto net_save_err;
	ret = fwrite(ddns1, 1, 4, fp);
	if(ret != 4)
		goto net_save_err;
	ret = fwrite(ddns2, 1, 4, fp);
	if(ret != 4)
		goto net_save_err;
	ret = fwrite(Wififlag, 1, 1, fp);
	if(ret != 1)
		goto net_save_err;
	
	fclose(fp);
	

	{
			IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
			IPCAM_NET_ST * pNetSt = &pAllInfo->net_st;
			pNetSt->ipv4[0]=ipaddress[0];
			pNetSt->ipv4[1]=ipaddress[1];
			pNetSt->ipv4[2]=ipaddress[2];
			pNetSt->ipv4[3]=ipaddress[3];
			
			pNetSt->gw[0]=gateway[0];
			pNetSt->gw[1]=gateway[1];
			pNetSt->gw[2]=gateway[2];
			pNetSt->gw[3]=gateway[3];
			
			pNetSt->netmask[0]=netmask[0];
			pNetSt->netmask[1]=netmask[1];
			pNetSt->netmask[2]=netmask[2];
			pNetSt->netmask[3]=netmask[3];
			
			pNetSt->dns1[0]=ddns1[0];
			pNetSt->dns1[1]=ddns1[1];
			pNetSt->dns1[2]=ddns1[2];
			pNetSt->dns1[3]=ddns1[3];
			
			pNetSt->dns2[0]=ddns2[0];
			pNetSt->dns2[1]=ddns2[1];
			pNetSt->dns2[2]=ddns2[2];
			pNetSt->dns2[3]=ddns2[3];
						
			gstCommonParam->GST_DRA_Hisi_save_ipcam_config_st(pAllInfo);						
	}
	return 0;
	
net_save_err:
	printf("net_cfg_save error\n");
	fclose(fp);	
	return -1;
}
int net_cfg_load(unsigned char *ipaddress,unsigned char *gateway,unsigned char *netmask,unsigned char *ddns1,unsigned char *ddns2,unsigned char *Wififlag)
{
	int ret;
	FILE *fp;
	char buf[100];
  int flag=0;
  
retry:
	flag++;
		
	fp = fopen(NET_CFG_FILENAME, "r + b");
	if(fp==NULL)
	{
		fp = fopen(NET_CFG_FILENAME, "w + b");
		if(fp==NULL)
		{
			printf("*************open net cfg file fail*************\n");
			return -1;
		}
		fclose(fp);
		
		ipaddress[0]=192;
		ipaddress[1]=168;
		ipaddress[2]=1;
		ipaddress[3]=60;
		
		gateway[0]=192;
		gateway[1]=168;
		gateway[2]=1;
		gateway[3]=1;
		
		netmask[0]=255;
		netmask[1]=255;
		netmask[2]=255;
		netmask[3]=0;
		
		ddns1[0]=202;
		ddns1[1]=96;
		ddns1[2]=134;
		ddns1[3]=133;
		
		ddns2[0]=202;
		ddns2[1]=96;
		ddns2[2]=128;
		ddns2[3]=166;
		
		Wififlag[0]=1;
		
		net_cfg_save(ipaddress,gateway,netmask,ddns1,ddns2,Wififlag);
	}
	else
	{
		ret = fread(ipaddress, 1, 4, fp);
		if(ret!=4)
			goto net_load_err;
		ret = fread(gateway, 1, 4, fp);
		if(ret!=4)
			goto net_load_err;
		ret = fread(netmask, 1, 4, fp);
		if(ret!=4)
			goto net_load_err;
		ret = fread(ddns1, 1, 4, fp);
		if(ret!=4)
			goto net_load_err;
		ret = fread(ddns2, 1, 4, fp);
		if(ret!=4)
			goto net_load_err;
		ret = fread(Wififlag, 1, 1, fp);
		if(ret!=1)
			goto net_load_err;
		fclose(fp);
	}
	
	return 0;
	
net_load_err:
	if(flag<3)
	{
		fclose(fp);
		memset(buf,0,sizeof(buf));
		sprintf(buf,"rm -f %s",NET_CFG_FILENAME);
		//system(buf);
		SendM(buf);
		goto retry;
	}
	else
		fclose(fp);
	
	return -1;
}
#endif


void MenuNetDhcpset()
{
	int ret,a[4];
	U8 i;
	char dhcp_ip[200],dhcp_netmsk[200],dhcp_gateway[200],hdcp_ddns1[200],hdcp_ddns2[200];
	memset(dhcp_ip,0,sizeof(dhcp_ip));
	memset(dhcp_netmsk,0,sizeof(dhcp_netmsk));
	memset(dhcp_gateway,0,sizeof(dhcp_gateway));
	memset(hdcp_ddns1,0,sizeof(hdcp_ddns1));
	memset(hdcp_ddns2,0,sizeof(hdcp_ddns2));
	memset(getip,0,sizeof(getip));
	memset(getgateway,0,sizeof(getgateway));
	memset(getsubway,0,sizeof(getsubway));
	memset(getddns1,0,sizeof(getddns1));
	memset(getddns2,0,sizeof(getddns2));
	
	WatchDog();
	ret=gstCommonParam->GST_NET_try_dhcp(dhcp_ip,dhcp_netmsk,dhcp_gateway,hdcp_ddns1,hdcp_ddns2);
	if (ret<0)
	{
		memset(dhcp_ip,0,sizeof(dhcp_ip));
		memset(dhcp_netmsk,0,sizeof(dhcp_netmsk));
		memset(dhcp_gateway,0,sizeof(dhcp_gateway));
		memset(hdcp_ddns1,0,sizeof(hdcp_ddns1));
		memset(hdcp_ddns2,0,sizeof(hdcp_ddns2));
		I_write_byte_eeprom(ADDNSTC+SEVERID, 1);
	}
	else
	{
		memcpy(getsubway,NetStaticMenu.SubNet,sizeof(getsubway));
		I_write_byte_eeprom(ADDNSTC+SEVERID, 1);
		sscanf(dhcp_ip,"%3d.%3d.%3d.%3d",&a[0],&a[1],&a[2],&a[3]);	
		for (i=0;i<4;i++)
			getip[i] = a[i];
		sscanf(dhcp_netmsk,"%3d.%3d.%3d.%3d",&a[0],&a[1],&a[2],&a[3]);	
		for (i=0;i<4;i++)
			getsubway[i] = a[i];
		sscanf(dhcp_gateway,"%3d.%3d.%3d.%3d",&a[0],&a[1],&a[2],&a[3]);	
		for (i=0;i<4;i++)
			getgateway[i] = a[i];
		sscanf(hdcp_ddns1,"%3d.%3d.%3d.%3d",&a[0],&a[1],&a[2],&a[3]);	
		for (i=0;i<4;i++)
			getddns1[i] = a[i];
		sscanf(hdcp_ddns2,"%3d.%3d.%3d.%3d",&a[0],&a[1],&a[2],&a[3]);	
		for (i=0;i<4;i++)
			getddns2[i] = a[i];
	}
	memcpy(autodns1,getddns1,sizeof(autodns1));
	memcpy(autodns2,getddns2,sizeof(autodns2));
	netdhcpget=1;
	WatchDog();
}

void setnet(char* msgbuf)
{
	U8 tm;
	ValNetStatic valtmp;
	memcpy(&valtmp.IPAlloction,&NetStaticMenu.IPAlloction,sizeof(ValNetStatic));
	I_read_block_eeprom(ADDNSTC+SEVERID,1,&tm);
	if (NetStaticMenu.IPAlloction && tm)
	{
		memcpy(valtmp.IpAddress,getip,4);
		memcpy(valtmp.GateWay,getgateway,4);
		memcpy(valtmp.SubNet,getsubway,4);
		memcpy(valtmp.DDNS1,getddns1,4);
		memcpy(valtmp.DDNS2,getddns2,4);
	}
	memcpy(msgbuf,&valtmp.IPAlloction,sizeof(ValNetStatic));
}

extern int gm8126id;
extern int adc_fd;

unsigned char reset_cfg()
{
	FILE *file_fd = NULL;
	
	file_fd=fopen("/mnt/mtd/reset_cfg.txt","r");
	
	if(file_fd)
	{
		fclose(file_fd);
		return 0;
	}
	else
		return 1;
}
void SetNetValue(U8 flag)
{
	char cmd[256];
	char data_Buf1[15],data_Buf2[15],data_Buf3[15],data_Buf4[15],data_Buf5[15],hwaddr[40];
	char data_name0[7],data_name1[7],data_name2[7],data_name3[7];
	U8 macadd,i;
	int ret_flag;
	int val;
	unsigned char reset_key_cfg=1;

	reset_key_cfg=reset_cfg();
	
	if(Wifiinfo.Wififlag)
		ret_flag=1;
	else
		ret_flag=0;

	for(i=0;i<100;i++)
	{
		RegisterReadWrite.value=0;
		if(gm8126id>0) ioctl(gm8126id, 7, &RegisterReadWrite);
		if(tw2865id>0) 
		{
			ioctl(tw2865id, 7, &RegisterReadWrite);
			if(RegisterReadWrite.value)
				RegisterReadWrite.value=0;
			else
				RegisterReadWrite.value=1;
		}
		
    val=0;
		if(adc_fd>0)
			ioctl(adc_fd,0x200,&val);
		if(val)
			RegisterReadWrite.value=0;
		else
			RegisterReadWrite.value=1;
			
		if (RegisterReadWrite.value==0)
			break;
		if(reset_key_cfg==0)
			break;
		usleep(10000);
		printf("RegisterReadWrite.value:%d \n",RegisterReadWrite.value);
	}	
	user_menu_inital();
	memset(data_Buf1,0x00,sizeof(data_Buf1));
	memset(data_Buf2,0x00,sizeof(data_Buf2));
	memset(data_Buf3,0x00,sizeof(data_Buf3));
	memset(hwaddr,0,sizeof(hwaddr));

	memcpy(&netwifi.netall.IPAlloction,&NetStaticMenu.IPAlloction,sizeof(NetStaticMenu));
	if(i==100)
	{
		ret_flag=0;
		NetStaticMenu.IpAddress[0]=192;
		NetStaticMenu.IpAddress[1]=168;
		NetStaticMenu.IpAddress[2]=1;
		NetStaticMenu.IpAddress[3]=60;
		
		NetStaticMenu.GateWay[0]=192;
		NetStaticMenu.GateWay[1]=168;
		NetStaticMenu.GateWay[2]=1;
		NetStaticMenu.GateWay[3]=1;
		
		NetStaticMenu.SubNet[0]=255;
		NetStaticMenu.SubNet[1]=255;
		NetStaticMenu.SubNet[2]=255;
		NetStaticMenu.SubNet[3]=0;
		NetStaticMenu.Port=6802;
		NetStaticMenu.IEPort=80;
		CheckCodeflag = RegisterReadWrite.value;
		
		menupass[0].UserName[0]='A';
		menupass[0].UserName[1]='D';
		menupass[0].UserName[2]='M';
		menupass[0].UserName[3]='I';
		menupass[0].UserName[4]='N';
		menupass[0].UserName[5]='0';
		menupass[0].Password[0]=0;
		menupass[0].Password[1]=0;
		menupass[0].Password[2]=0;
		menupass[0].Password[3]=0;
		menupass[0].Password[4]=0;
		menupass[0].Password[5]=0;		
	}
	else if(net_cfg_load(NetStaticMenu.IpAddress,NetStaticMenu.GateWay,NetStaticMenu.SubNet,NetStaticMenu.DDNS1,NetStaticMenu.DDNS2,&ret_flag)<0)
	{
		printf("net_cfg_load fail \n");
		NetStaticMenu.IpAddress[0]=192;
		NetStaticMenu.IpAddress[1]=168;
		NetStaticMenu.IpAddress[2]=1;
		NetStaticMenu.IpAddress[3]=60;
		
		NetStaticMenu.GateWay[0]=192;
		NetStaticMenu.GateWay[1]=168;
		NetStaticMenu.GateWay[2]=1;
		NetStaticMenu.GateWay[3]=1;
		
		NetStaticMenu.SubNet[0]=255;
		NetStaticMenu.SubNet[1]=255;
		NetStaticMenu.SubNet[2]=255;
		NetStaticMenu.SubNet[3]=0;
				
	}

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
	sprintf(hwaddr," 00:1E:E2:8%d:B1:%d%d",NetStaticMenu.IpAddress[3]/100,(NetStaticMenu.IpAddress[3]%100)/10,NetStaticMenu.IpAddress[3]%100%10);
	macadd = NetStaticMenu.IpAddress[3];
	memcpy(nvrip,data_Buf1,sizeof(data_Buf1));

//	if(ret_flag==0)
		gstCommonParam->GST_NET_set_net_parameter(data_Buf1,NetStaticMenu.Port,data_Buf2,data_Buf3,data_Buf4,data_Buf5);

	/*sys_command("chmod 777 /mnt/mtd/wup.sh");
	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"sh /mnt/mtd/wup.sh %s %s %s %d %s &",data_Buf2,data_Buf1,hwaddr,ret_flag,data_Buf3);
	sys_command(cmd);*/
	
	wifi_use_flag=ret_flag;
	
	if (flag)
		gstCommonParam->GST_NET_start_mini_httpd(NetStaticMenu.IEPort);
	
	memset(data_Buf1,0x00,sizeof(data_Buf1));
	memset(data_Buf2,0x00,sizeof(data_Buf2));
	memset(data_Buf3,0x00,sizeof(data_Buf3));
	sprintf(data_Buf1,"%d%d%d%d%d%d",
					menupass[0].Password[0],menupass[0].Password[1],menupass[0].Password[2],menupass[0].Password[3],menupass[0].Password[4],menupass[0].Password[5]);
	sprintf(data_Buf2,"%d%d%d%d%d%d",
					menupass[1].Password[0],menupass[1].Password[1],menupass[1].Password[2],menupass[1].Password[3],menupass[1].Password[4],menupass[1].Password[5]);
	sprintf(data_Buf3,"%d%d%d%d%d%d",
					menupass[2].Password[0],menupass[2].Password[1],menupass[2].Password[2],menupass[2].Password[3],menupass[2].Password[4],menupass[2].Password[5]);
	sprintf(data_Buf4,"%d%d%d%d%d%d",
					menupass[3].Password[0],menupass[3].Password[1],menupass[3].Password[2],menupass[3].Password[3],menupass[3].Password[4],menupass[3].Password[5]);
	memset(data_name0,0x00,sizeof(data_name0));
	memset(data_name1,0x00,sizeof(data_name1));
	memset(data_name2,0x00,sizeof(data_name2));
	memset(data_name3,0x00,sizeof(data_name3));
	sprintf(data_name0,"%c%c%c%c%c%c",menupass[0].UserName[0],menupass[0].UserName[1],menupass[0].UserName[2],menupass[0].UserName[3],menupass[0].UserName[4],menupass[0].UserName[5]);//"ADMIN0");
	sprintf(data_name1,"%c%c%c%c%c%c",menupass[1].UserName[0],menupass[1].UserName[1],menupass[1].UserName[2],menupass[1].UserName[3],menupass[1].UserName[4],menupass[1].UserName[5]);//"USER01");
	sprintf(data_name2,"%c%c%c%c%c%c",menupass[2].UserName[0],menupass[2].UserName[1],menupass[2].UserName[2],menupass[2].UserName[3],menupass[2].UserName[4],menupass[2].UserName[5]);//"USER02");
	sprintf(data_name3,"%c%c%c%c%c%c",menupass[3].UserName[0],menupass[3].UserName[1],menupass[3].UserName[2],menupass[3].UserName[3],menupass[3].UserName[4],menupass[3].UserName[5]);//"USER02");

	gstCommonParam->GST_NET_get_admin_info(data_name0,data_Buf1,data_name1,data_Buf2,data_name2,data_Buf3,data_name3,data_Buf4);
	gstCommonParam->GST_NET_net_ddns_open_close_flag(1);
	printf("DVRPORT=%d,IEPORT:%d\n",NetStaticMenu.Port,NetStaticMenu.IEPort);
	printf("data_buf1=%s,name1 = %s,name:%s\n",data_Buf1,data_name0,menupass[0].UserName);//
	printf("data_buf2=%s,name2 = %s,name:%s\n",data_Buf2,data_name1,menupass[1].UserName);//
	printf("data_buf3=%s,name3 = %s,name:%s\n",data_Buf3,data_name2,menupass[2].UserName);//
	printf("data_buf4=%s,name4 = %s,name:%s\n",data_Buf4,data_name3,menupass[3].UserName);//
	
}

void MenuNetStaticInit()
{
	U8 tmp[2],tmp1[2];
	
	
	I_read_block_eeprom(ADDNSTC,sizeof(NetStaticMenu),&NetStaticMenu.IPAlloction);
	I_read_block_eeprom(ADDNSTC+PORT,2,tmp);
	I_read_block_eeprom(ADDNSTC+IEPORT,2,tmp1);
	NetStaticMenu.Port=(tmp[0]<<8)|tmp[1];
	NetStaticMenu.IEPort=(tmp1[0]<<8)|tmp1[1];
//	I_read_block_eeprom(ADDNSTC+DDNS1,4,NetStaticMenu.DDNS1);
//	I_read_block_eeprom(ADDNSTC+DDNS2,4,NetStaticMenu.DDNS2);
	I_read_block_eeprom(ADDNSTC+DHCPIP,4,getip);
	I_read_block_eeprom(ADDNSTC+DHCPGATEWAY,4,getgateway);
	I_read_block_eeprom(ADDNSTC+DHCPDNS1,4,getddns1);
	I_read_block_eeprom(ADDNSTC+DHCPDNS2,4,getddns2);
	
	I_read_block_eeprom(ADDWIFI,sizeof(Wifiinfo),&Wifiinfo.Wififlag);
	I_read_block_eeprom(ADDNSTC,sizeof(NetStaticMenu),&netwifi.netall.IPAlloction);
	I_read_block_eeprom(ADDWIFI,sizeof(Wifiinfo),&netwifi.rt73.Wififlag);
			
	{
			IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
			IPCAM_NET_ST * pNetSt = &pAllInfo->net_st;
			
			net_cfg_load(NetStaticMenu.IpAddress,NetStaticMenu.GateWay,NetStaticMenu.SubNet,NetStaticMenu.DDNS1,NetStaticMenu.DDNS2,&Wifiinfo.Wififlag);

			NetStaticMenu.IpAddress[0]=pNetSt->ipv4[0];
			NetStaticMenu.IpAddress[1]=pNetSt->ipv4[1];
			NetStaticMenu.IpAddress[2]=pNetSt->ipv4[2];
			NetStaticMenu.IpAddress[3]=pNetSt->ipv4[3];
			
			NetStaticMenu.GateWay[0]=pNetSt->gw[0];
			NetStaticMenu.GateWay[1]=pNetSt->gw[1];
			NetStaticMenu.GateWay[2]=pNetSt->gw[2];
			NetStaticMenu.GateWay[3]=pNetSt->gw[3];
							
			NetStaticMenu.SubNet[0]=pNetSt->netmask[0];
			NetStaticMenu.SubNet[1]=pNetSt->netmask[1];
			NetStaticMenu.SubNet[2]=pNetSt->netmask[2];
			NetStaticMenu.SubNet[3]=pNetSt->netmask[3];		
				
			NetStaticMenu.DDNS1[0]=pNetSt->dns1[0];
			NetStaticMenu.DDNS1[1]=pNetSt->dns1[1];
			NetStaticMenu.DDNS1[2]=pNetSt->dns1[2];
			NetStaticMenu.DDNS1[3]=pNetSt->dns1[3];		
			
			NetStaticMenu.DDNS2[0]=pNetSt->dns2[0];
			NetStaticMenu.DDNS2[1]=pNetSt->dns2[1];
			NetStaticMenu.DDNS2[2]=pNetSt->dns2[2];
			NetStaticMenu.DDNS2[3]=pNetSt->dns2[3];		
			
			if(net_cfg_save(NetStaticMenu.IpAddress,NetStaticMenu.GateWay,NetStaticMenu.SubNet,NetStaticMenu.DDNS1,NetStaticMenu.DDNS2,&Wifiinfo.Wififlag)<0)
			{
				printf("******net_cfg_save false ********\n");
			}
	}
	
	if(net_cfg_load(NetStaticMenu.IpAddress,NetStaticMenu.GateWay,NetStaticMenu.SubNet,NetStaticMenu.DDNS1,NetStaticMenu.DDNS2,&Wifiinfo.Wififlag)==0)
	{
		I_write_block_eeprom(ADDNSTC,sizeof(NetStaticMenu),&NetStaticMenu.IPAlloction);
		tmp1[0]=NetStaticMenu.IEPort>>8;
		tmp1[1]=NetStaticMenu.IEPort&0xff;
		I_write_block_eeprom(ADDNSTC+IEPORT,2,tmp1);
		tmp1[0]=NetStaticMenu.Port>>8;
		tmp1[1]=NetStaticMenu.Port&0xff;
		I_write_block_eeprom(ADDNSTC+PORT,2,tmp1);		
		I_write_block_eeprom(ADDWIFI,sizeof(Wifiinfo),&Wifiinfo.Wififlag);		
		
		I_read_block_eeprom(ADDNSTC,sizeof(NetStaticMenu),&NetStaticMenu.IPAlloction);
		I_read_block_eeprom(ADDWIFI,sizeof(Wifiinfo),&netwifi.rt73.Wififlag);

		I_read_block_eeprom(ADDNSTC+PORT,2,tmp);
		I_read_block_eeprom(ADDNSTC+IEPORT,2,tmp1);
		NetStaticMenu.Port=(tmp[0]<<8)|tmp[1];
		NetStaticMenu.IEPort=(tmp1[0]<<8)|tmp1[1];
	}
	SetNetValue(1);
	memcpy(getsubway,NetStaticMenu.SubNet,sizeof(getsubway));
	
	
}

void menu_netstatic_data_disp(U8 pos,U16 color)
{
}

void menu_netstatic_on()
{
}

void menu_netstatic_down()
{
	menu_netstatic_data_disp(cur_pos,WHITE);
	if (cur_pos == NETSTATICDATA-1)
		cur_pos = 0;
	else
		cur_pos++;
/*	
	if (NetStaticMenu.IPAlloction)
	{
		if (cur_pos>0 && cur_pos<10)
			cur_pos = 10;
		else if (cur_pos>12 && cur_pos<37)
			cur_pos = 37;
	}
*/
	menu_netstatic_data_disp(cur_pos,RED);
}
void menu_netstatic_up()
{
	menu_netstatic_data_disp(cur_pos,WHITE);
	if (cur_pos == 0)
		cur_pos = NETSTATICDATA-1;
	else
		cur_pos--;
/*
	if (NetStaticMenu.IPAlloction)
	{
		if (cur_pos>0 && cur_pos<10)
			cur_pos = 0;
		else if (cur_pos>12 && cur_pos<37)
			cur_pos = 12;
	}
*/
	menu_netstatic_data_disp(cur_pos,RED);
}

void netsavedate(char* msgbuf)
{
	ValNetStatic valtmp;
	U8 tm;
	U8 tmp[2],tmp1[2];
	
	memcpy(&valtmp.IPAlloction,msgbuf,sizeof(ValNetStatic));
	I_read_block_eeprom(ADDNSTC+SEVERID,1,&tm);
	if (NetStaticMenu.IPAlloction && tm)
	{
		memcpy(getip,valtmp.IpAddress,4);
		memcpy(getgateway,valtmp.GateWay,4);
		memcpy(getsubway,valtmp.SubNet,4);
		memcpy(getddns1,valtmp.DDNS1,4);
		memcpy(getddns2,valtmp.DDNS2,4);
		NetStaticMenu.Port = valtmp.Port;
		NetStaticMenu.IEPort = valtmp.IEPort;
	}
	else
		memcpy(&valtmp.IPAlloction,msgbuf,sizeof(ValNetStatic));
	tmp[0]=NetStaticMenu.Port>>8;
	tmp[1]=NetStaticMenu.Port;
	tmp1[0]=NetStaticMenu.IEPort>>8;
	tmp1[1]=NetStaticMenu.IEPort;
	
	
	I_write_block_eeprom(ADDNSTC,sizeof(NetStaticMenu),&NetStaticMenu.IPAlloction);


	I_write_block_eeprom(ADDNSTC+PORT,2,tmp);
	I_write_block_eeprom(ADDNSTC+IEPORT,2,tmp1);
	I_write_block_eeprom(ADDNSTC+DHCPIP,4,getip);
	I_write_block_eeprom(ADDNSTC+DHCPGATEWAY,4,getgateway);
	I_write_block_eeprom(ADDNSTC+DHCPDNS1,4,getddns1);
	I_write_block_eeprom(ADDNSTC+DHCPDNS2,4,getddns2);
}

void save_net()
{
	U8 tmp[2],tmp1[2];
	tmp[0]=NetStaticMenu.Port>>8;
	tmp[1]=NetStaticMenu.Port;
	tmp1[0]=NetStaticMenu.IEPort>>8;
	tmp1[1]=NetStaticMenu.IEPort;
	I_write_block_eeprom(ADDNSTC,sizeof(NetStaticMenu),&NetStaticMenu.IPAlloction);


	I_write_block_eeprom(ADDNSTC+PORT,2,tmp);
	I_write_block_eeprom(ADDNSTC+IEPORT,2,tmp1);
	I_write_block_eeprom(ADDNSTC+DHCPIP,4,getip);
	I_write_block_eeprom(ADDNSTC+DHCPGATEWAY,4,getgateway);
	I_write_block_eeprom(ADDNSTC+DHCPDNS1,4,getddns1);
	I_write_block_eeprom(ADDNSTC+DHCPDNS2,4,getddns2);

	if(net_cfg_save(NetStaticMenu.IpAddress,NetStaticMenu.GateWay,NetStaticMenu.SubNet,NetStaticMenu.DDNS1,NetStaticMenu.DDNS2,&Wifiinfo.Wififlag)<0)
	{
		printf("net_cfg_save false\n");
	}
//	gstCommonParam->GST_DRA_Net_cam_mtd_store();
}

void menu_netstatic_do()
{
	save_net();
	MenuNetStaticInit();
//	SetNetValue();
}

void menu_netstatic_add()
{
	 U8 temp;
	switch(cur_pos)
	{
		case 0:
			if (NetStaticMenu.IPAlloction)
				NetStaticMenu.IPAlloction = 0;
			else
				NetStaticMenu.IPAlloction = 1;
			wrflagnet = 1;
			menu_netstatic_data_disp(cur_pos,RED);
			if (NetStaticMenu.IPAlloction)
				MenuNetDhcpset();
			for (temp=1;temp<61;temp++)
				menu_netstatic_data_disp(temp,BLACK_WHITE);
			break;
		case 1:
		case 4:
		case 7:
		case 10:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getip[(cur_pos-1)/3]>155 && getip[(cur_pos-1)/3]<200)
					getip[(cur_pos-1)/3] -= 100;
				else if (getip[(cur_pos-1)/3]>=200)
					getip[(cur_pos-1)/3] -= 200;
				else 
					getip[(cur_pos-1)/3] += 100;
			}
			else
			{
				if (NetStaticMenu.IpAddress[(cur_pos-1)/3]>155 && NetStaticMenu.IpAddress[(cur_pos-1)/3]<200)
					NetStaticMenu.IpAddress[(cur_pos-1)/3] -= 100;
				else if (NetStaticMenu.IpAddress[(cur_pos-1)/3]>=200)
					NetStaticMenu.IpAddress[(cur_pos-1)/3] -= 200;
				else 
					NetStaticMenu.IpAddress[(cur_pos-1)/3] += 100;
			}
			break;
		case 2:
		case 5:
		case 8:
		case 11:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getip[(cur_pos-1)/3]>250)
					getip[(cur_pos-1)/3] -= 50;
	//			else if (NetStaticMenu.IpAddress[(cur_pos-1)/3] > 190)
	//				NetStaticMenu.IpAddress[(cur_pos-1)/3] -= 90;
				else 
					getip[(cur_pos-1)/3] += 10;
			}
			else
			{
				if (NetStaticMenu.IpAddress[(cur_pos-1)/3]>250)
					NetStaticMenu.IpAddress[(cur_pos-1)/3] -= 50;
	//			else if (NetStaticMenu.IpAddress[(cur_pos-1)/3] > 190)
	//				NetStaticMenu.IpAddress[(cur_pos-1)/3] -= 90;
				else 
					NetStaticMenu.IpAddress[(cur_pos-1)/3] += 10;
			}
			break;
		case 3:
		case 6:
		case 9:
		case 12:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getip[(cur_pos-1)/3]==255)
					getip[(cur_pos-1)/3] -= 5;
				else if ((getip[(cur_pos-1)/3]%10)==9)
					getip[(cur_pos-1)/3] -= 9;
				else 
					getip[(cur_pos-1)/3] += 1;
			}
			else
			{
				if (NetStaticMenu.IpAddress[(cur_pos-1)/3]==255)
					NetStaticMenu.IpAddress[(cur_pos-1)/3] -= 5;
				else if ((NetStaticMenu.IpAddress[(cur_pos-1)/3]%10)==9)
					NetStaticMenu.IpAddress[(cur_pos-1)/3] -= 9;
				else 
					NetStaticMenu.IpAddress[(cur_pos-1)/3] += 1;
			}
			break;
		case 13:
		case 16:
		case 19:
		case 22:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getgateway[(cur_pos-13)/3]>155 && getgateway[(cur_pos-13)/3]<200)
					getgateway[(cur_pos-13)/3] -= 100;
				else if (getgateway[(cur_pos-13)/3]>=200)
					getgateway[(cur_pos-13)/3] -= 200;
				else 
					getgateway[(cur_pos-13)/3] += 100;
			}
			else
			{
				if (NetStaticMenu.GateWay[(cur_pos-13)/3]>155 && NetStaticMenu.GateWay[(cur_pos-13)/3]<200)
					NetStaticMenu.GateWay[(cur_pos-13)/3] -= 100;
				else if (NetStaticMenu.GateWay[(cur_pos-13)/3]>=200)
					NetStaticMenu.GateWay[(cur_pos-13)/3] -= 200;
				else 
					NetStaticMenu.GateWay[(cur_pos-13)/3] += 100;
			}
			break;
		case 14:
		case 17:
		case 20:
		case 23:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getgateway[(cur_pos-13)/3]>250)
					getgateway[(cur_pos-13)/3] -= 50;
				else if (getgateway[(cur_pos-13)/3] > 190)
					getgateway[(cur_pos-13)/3] -= 90;
				else 
					getgateway[(cur_pos-13)/3] += 10;
			}
			else
			{
				if (NetStaticMenu.GateWay[(cur_pos-13)/3]>250)
					NetStaticMenu.GateWay[(cur_pos-13)/3] -= 50;
				else if (NetStaticMenu.GateWay[(cur_pos-13)/3] > 190)
					NetStaticMenu.GateWay[(cur_pos-13)/3] -= 90;
				else 
					NetStaticMenu.GateWay[(cur_pos-13)/3] += 10;
			}
			break;
		case 15:
		case 18:
		case 21:
		case 24:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getgateway[(cur_pos-13)/3]==255)
					getgateway[(cur_pos-13)/3] -= 5;
				else if ((getgateway[(cur_pos-13)/3]%10)==9)
					getgateway[(cur_pos-13)/3] -= 9;
				else 
					getgateway[(cur_pos-13)/3] += 1;
			}
			else
			{
				if (NetStaticMenu.GateWay[(cur_pos-13)/3]==255)
					NetStaticMenu.GateWay[(cur_pos-13)/3] -= 5;
				else if ((NetStaticMenu.GateWay[(cur_pos-13)/3]%10)==9)
					NetStaticMenu.GateWay[(cur_pos-13)/3] -= 9;
				else 
					NetStaticMenu.GateWay[(cur_pos-13)/3] += 1;
			}
			break;
		case 25:
		case 28:
		case 31:
		case 34:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getsubway[(cur_pos-25)/3]>155 && getsubway[(cur_pos-25)/3]<200)
					getsubway[(cur_pos-25)/3] -= 100;
				else if (getsubway[(cur_pos-25)/3]>=200)
					getsubway[(cur_pos-25)/3] -= 200;
				else 
					getsubway[(cur_pos-25)/3] += 100;
			}
			else
			{
				if (NetStaticMenu.SubNet[(cur_pos-25)/3]>155 && NetStaticMenu.SubNet[(cur_pos-25)/3]<200)
					NetStaticMenu.SubNet[(cur_pos-25)/3] -= 100;
				else if (NetStaticMenu.SubNet[(cur_pos-25)/3]>=200)
					NetStaticMenu.SubNet[(cur_pos-25)/3] -= 200;
				else 
					NetStaticMenu.SubNet[(cur_pos-25)/3] += 100;
			}
			break;
		case 26:
		case 29:
		case 32:
		case 35:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getsubway[(cur_pos-25)/3]>250)
					getsubway[(cur_pos-25)/3] -= 50;
				else if (getsubway[(cur_pos-25)/3] > 190)
					getsubway[(cur_pos-25)/3] -= 90;
				else 
					getsubway[(cur_pos-25)/3] += 10;
			}
			else
			{
				if (NetStaticMenu.SubNet[(cur_pos-25)/3]>250)
					NetStaticMenu.SubNet[(cur_pos-25)/3] -= 50;
				else if (NetStaticMenu.SubNet[(cur_pos-25)/3] > 190)
					NetStaticMenu.SubNet[(cur_pos-25)/3] -= 90;
				else 
					NetStaticMenu.SubNet[(cur_pos-25)/3] += 10;
			}
			break;
		case 27:
		case 30:
		case 33:
		case 36:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getsubway[(cur_pos-25)/3]==255)
					getsubway[(cur_pos-25)/3] -= 5;
				else if ((getsubway[(cur_pos-25)/3]%10)==9)
					getsubway[(cur_pos-25)/3] -= 9;
				else 
					getsubway[(cur_pos-25)/3] += 1;
			}
			else
			{
				if (NetStaticMenu.SubNet[(cur_pos-25)/3]==255)
					NetStaticMenu.SubNet[(cur_pos-25)/3] -= 5;
				else if ((NetStaticMenu.SubNet[(cur_pos-25)/3]%10)==9)
					NetStaticMenu.SubNet[(cur_pos-25)/3] -= 9;
				else 
					NetStaticMenu.SubNet[(cur_pos-25)/3] += 1;
			}
			break;
		case 37:
		case 40:
		case 43:
		case 46:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getddns1[(cur_pos-37)/3]>155 && getddns1[(cur_pos-37)/3]<200)
					getddns1[(cur_pos-37)/3] -= 100;
				else if (getddns1[(cur_pos-37)/3]>=200)
					getddns1[(cur_pos-37)/3] -= 200;
				else 
					getddns1[(cur_pos-37)/3] += 100;
			}
			else
			{
				if (NetStaticMenu.DDNS1[(cur_pos-37)/3]>155 && NetStaticMenu.DDNS1[(cur_pos-37)/3]<200)
					NetStaticMenu.DDNS1[(cur_pos-37)/3] -= 100;
				else if (NetStaticMenu.DDNS1[(cur_pos-37)/3]>=200)
					NetStaticMenu.DDNS1[(cur_pos-37)/3] -= 200;
				else 
					NetStaticMenu.DDNS1[(cur_pos-37)/3] += 100;
			}
			break;
		case 38:
		case 41:
		case 44:
		case 47:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getddns1[(cur_pos-37)/3]>250)
					getddns1[(cur_pos-37)/3] -= 50;
	//			else if (NetStaticMenu.DDNS1[(cur_pos-37)/3] > 190)
	//				NetStaticMenu.DDNS1[(cur_pos-37)/3] -= 90;
				else 
					getddns1[(cur_pos-37)/3] += 10;
			}
			else
			{
				if (NetStaticMenu.DDNS1[(cur_pos-37)/3]>250)
					NetStaticMenu.DDNS1[(cur_pos-37)/3] -= 50;
	//			else if (NetStaticMenu.DDNS1[(cur_pos-37)/3] > 190)
	//				NetStaticMenu.DDNS1[(cur_pos-37)/3] -= 90;
				else 
					NetStaticMenu.DDNS1[(cur_pos-37)/3] += 10;
			}
			break;
		case 39:
		case 42:
		case 45:
		case 48:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getddns1[(cur_pos-37)/3]==255)
					getddns1[(cur_pos-37)/3] -= 5;
				else if ((getddns1[(cur_pos-37)/3]%10)==9)
					getddns1[(cur_pos-37)/3] -= 9;
				else 
					getddns1[(cur_pos-37)/3] += 1;
			}
			else
			{
				if (NetStaticMenu.DDNS1[(cur_pos-37)/3]==255)
					NetStaticMenu.DDNS1[(cur_pos-37)/3] -= 5;
				else if ((NetStaticMenu.DDNS1[(cur_pos-37)/3]%10)==9)
					NetStaticMenu.DDNS1[(cur_pos-37)/3] -= 9;
				else 
					NetStaticMenu.DDNS1[(cur_pos-37)/3] += 1;
			}
			break;
		case 49:
		case 52:
		case 55:
		case 58:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getddns2[(cur_pos-49)/3]>155 && getddns2[(cur_pos-49)/3]<200)
					getddns2[(cur_pos-49)/3] -= 100;
				else if (getddns2[(cur_pos-49)/3]>=200)
					getddns2[(cur_pos-49)/3] -= 200;
				else 
					getddns2[(cur_pos-49)/3] += 100;
			}
			else
			{
				if (NetStaticMenu.DDNS2[(cur_pos-49)/3]>155 && NetStaticMenu.DDNS2[(cur_pos-49)/3]<200)
					NetStaticMenu.DDNS2[(cur_pos-49)/3] -= 100;
				else if (NetStaticMenu.DDNS2[(cur_pos-49)/3]>=200)
					NetStaticMenu.DDNS2[(cur_pos-49)/3] -= 200;
				else 
					NetStaticMenu.DDNS2[(cur_pos-49)/3] += 100;
			}
			break;
		case 50:
		case 53:
		case 56:
		case 59:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getddns2[(cur_pos-49)/3]>250)
					getddns2[(cur_pos-49)/3] -= 50;
	//			else if (NetStaticMenu.DDNS2[(cur_pos-49)/3] > 190)
	//				NetStaticMenu.DDNS2[(cur_pos-49)/3] -= 90;
				else 
					getddns2[(cur_pos-49)/3] += 10;
			}
			else
			{
				if (NetStaticMenu.DDNS2[(cur_pos-49)/3]>250)
					NetStaticMenu.DDNS2[(cur_pos-49)/3] -= 50;
	//			else if (NetStaticMenu.DDNS2[(cur_pos-49)/3] > 190)
	//				NetStaticMenu.DDNS2[(cur_pos-49)/3] -= 90;
				else 
					NetStaticMenu.DDNS2[(cur_pos-49)/3] += 10;
			}
			break;
		case 51:
		case 54:
		case 57:
		case 60:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getddns2[(cur_pos-49)/3]==255)
					getddns2[(cur_pos-49)/3] -= 5;
				else if ((getddns2[(cur_pos-49)/3]%10)==9)
					getddns2[(cur_pos-49)/3] -= 9;
				else 
					getddns2[(cur_pos-49)/3] += 1;
			}
			else
			{
				if (NetStaticMenu.DDNS2[(cur_pos-49)/3]==255)
					NetStaticMenu.DDNS2[(cur_pos-49)/3] -= 5;
				else if ((NetStaticMenu.DDNS2[(cur_pos-49)/3]%10)==9)
					NetStaticMenu.DDNS2[(cur_pos-49)/3] -= 9;
				else 
					NetStaticMenu.DDNS2[(cur_pos-49)/3] += 1;
			}
			break;
		case 61:
			wrflagnet = 1;
			temp = NetStaticMenu.Port/1000;
			NetStaticMenu.Port = NetStaticMenu.Port-temp*1000;
			if (temp>=9)
				temp=0;
			else 
				temp++;
			NetStaticMenu.Port=NetStaticMenu.Port+temp*1000;
			break;
		case 62:
			wrflagnet = 1;
			temp = (NetStaticMenu.Port%1000)/100;
			NetStaticMenu.Port = NetStaticMenu.Port-temp*100;
			if (temp>=9)
				temp=0;
			else 
				temp++;
			NetStaticMenu.Port=NetStaticMenu.Port+temp*100;
			break;
		case 63:
			wrflagnet = 1;
			temp = (NetStaticMenu.Port%1000%100)/10;
			NetStaticMenu.Port = NetStaticMenu.Port-temp*10;
			if (temp>=9)
				temp=0;
			else 
				temp++;
			NetStaticMenu.Port=NetStaticMenu.Port+temp*10;
			break;
		case 64:
			wrflagnet = 1;
			temp = NetStaticMenu.Port%1000%100%10;
			NetStaticMenu.Port = NetStaticMenu.Port-temp;
			if (temp>=9)
				temp=0;
			else 
				temp++;
			NetStaticMenu.Port=NetStaticMenu.Port+temp;
			break;
		case 65:
			wrflagnet = 1;
			temp = NetStaticMenu.IEPort/1000;
			NetStaticMenu.IEPort = NetStaticMenu.IEPort-temp*1000;
			if (temp>=9)
				temp=0;
			else 
				temp++;
			NetStaticMenu.IEPort=NetStaticMenu.IEPort+temp*1000;
			break;
		case 66:
			wrflagnet = 1;
			temp = (NetStaticMenu.IEPort%1000)/100;
			NetStaticMenu.IEPort = NetStaticMenu.IEPort-temp*100;
			if (temp>=9)
				temp=0;
			else 
				temp++;
			NetStaticMenu.IEPort=NetStaticMenu.IEPort+temp*100;
			break;
		case 67:
			wrflagnet = 1;
			temp = (NetStaticMenu.IEPort%1000%100)/10;
			NetStaticMenu.IEPort = NetStaticMenu.IEPort-temp*10;
			if (temp>=9)
				temp=0;
			else 
				temp++;
			NetStaticMenu.IEPort=NetStaticMenu.IEPort+temp*10;
			break;
		case 68:
			wrflagnet = 1;
			temp = NetStaticMenu.IEPort%1000%100%10;
			NetStaticMenu.IEPort = NetStaticMenu.IEPort-temp;
			if (temp>=9)
				temp=0;
			else 
				temp++;
			NetStaticMenu.IEPort=NetStaticMenu.IEPort+temp;
			break;
	}
	if (cur_pos != 0)
		menu_netstatic_data_disp(cur_pos,RED);

}
void menu_netstatic_dec()
{

	 U8 temp;
//	 U8 wtemp;
	switch(cur_pos)
	{
		case 0:
			if (NetStaticMenu.IPAlloction)
				NetStaticMenu.IPAlloction = 0;
			else
				NetStaticMenu.IPAlloction = 1;
			wrflagnet = 1;
			menu_netstatic_data_disp(cur_pos,RED);
			if (NetStaticMenu.IPAlloction)
				MenuNetDhcpset();
				
			for (temp=1;temp<61;temp++)
				menu_netstatic_data_disp(temp,BLACK_WHITE);

			break;
		case 1:
		case 4:
		case 7:
		case 10:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getip[(cur_pos-1)/3]>55 && getip[(cur_pos-1)/3]<100)
					getip[(cur_pos-1)/3] += 100;
				else if (getip[(cur_pos-1)/3]<=55)
					getip[(cur_pos-1)/3] += 200;
				else 
					getip[(cur_pos-1)/3] -= 100;
			}
			else
			{
				if (NetStaticMenu.IpAddress[(cur_pos-1)/3]>55 && NetStaticMenu.IpAddress[(cur_pos-1)/3]<100)
					NetStaticMenu.IpAddress[(cur_pos-1)/3] += 100;
				else if (NetStaticMenu.IpAddress[(cur_pos-1)/3]<=55)
					NetStaticMenu.IpAddress[(cur_pos-1)/3] += 200;
				else 
					NetStaticMenu.IpAddress[(cur_pos-1)/3] -= 100;
			}
			break;
		case 2:
		case 5:
		case 8:
		case 11:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getip[(cur_pos-1)/3]>205 && getip[(cur_pos-1)/3]<210)
					getip[(cur_pos-1)/3] += 40;
				else if (getip[(cur_pos-1)/3] > 199 && getip[(cur_pos-1)/3]<206)
					getip[(cur_pos-1)/3] += 50;
				else if (((getip[(cur_pos-1)/3]/10) == 10) || (getip[(cur_pos-1)/3]<10))
					getip[(cur_pos-1)/3] += 90;
				else 
					getip[(cur_pos-1)/3] -= 10;
			}
			else
			{
				if (NetStaticMenu.IpAddress[(cur_pos-1)/3]>205 && NetStaticMenu.IpAddress[(cur_pos-1)/3]<210)
					NetStaticMenu.IpAddress[(cur_pos-1)/3] += 40;
				else if (NetStaticMenu.IpAddress[(cur_pos-1)/3] > 199 && NetStaticMenu.IpAddress[(cur_pos-1)/3]<206)
					NetStaticMenu.IpAddress[(cur_pos-1)/3] += 50;
				else if (((NetStaticMenu.IpAddress[(cur_pos-1)/3]/10) == 10) || (NetStaticMenu.IpAddress[(cur_pos-1)/3]<10))
					NetStaticMenu.IpAddress[(cur_pos-1)/3] += 90;
				else 
					NetStaticMenu.IpAddress[(cur_pos-1)/3] -= 10;
			}
			break;
		case 3:
		case 6:
		case 9:
		case 12:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getip[(cur_pos-1)/3]==250)
					getip[(cur_pos-1)/3] += 5;
				else if ((getip[(cur_pos-1)/3]%10)==0)
					getip[(cur_pos-1)/3] += 9;
				else 
					getip[(cur_pos-1)/3] -= 1;
			}
			else
			{
				if (NetStaticMenu.IpAddress[(cur_pos-1)/3]==250)
					NetStaticMenu.IpAddress[(cur_pos-1)/3] += 5;
				else if ((NetStaticMenu.IpAddress[(cur_pos-1)/3]%10)==0)
					NetStaticMenu.IpAddress[(cur_pos-1)/3] += 9;
				else 
					NetStaticMenu.IpAddress[(cur_pos-1)/3] -= 1;
			}
			break;
		case 13:
		case 16:
		case 19:
		case 22:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getgateway[(cur_pos-13)/3]>55 && getgateway[(cur_pos-13)/3]<100)
					getgateway[(cur_pos-13)/3] += 100;
				else if (getgateway[(cur_pos-13)/3]<=55)
					getgateway[(cur_pos-13)/3] += 200;
				else 
					getgateway[(cur_pos-13)/3] -= 100;
			}
			else
			{
				if (NetStaticMenu.GateWay[(cur_pos-13)/3]>55 && NetStaticMenu.GateWay[(cur_pos-13)/3]<100)
					NetStaticMenu.GateWay[(cur_pos-13)/3] += 100;
				else if (NetStaticMenu.GateWay[(cur_pos-13)/3]<=55)
					NetStaticMenu.GateWay[(cur_pos-13)/3] += 200;
				else 
					NetStaticMenu.GateWay[(cur_pos-13)/3] -= 100;
			}
			break;
		case 14:
		case 17:
		case 20:
		case 23:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getgateway[(cur_pos-13)/3]>205 && getgateway[(cur_pos-13)/3]<210)
					getgateway[(cur_pos-13)/3] += 40;
				else if (getgateway[(cur_pos-13)/3] > 199 && getgateway[(cur_pos-13)/3]<206)
					getgateway[(cur_pos-13)/3] += 50;
				else if (((getgateway[(cur_pos-13)/3]/10) == 10) || (getgateway[(cur_pos-13)/3]<10))
					getgateway[(cur_pos-13)/3] += 90;
				else 
					getgateway[(cur_pos-13)/3] -= 10;
			}
			else
			{
				if (NetStaticMenu.GateWay[(cur_pos-13)/3]>205 && NetStaticMenu.GateWay[(cur_pos-13)/3]<210)
					NetStaticMenu.GateWay[(cur_pos-13)/3] += 40;
				else if (NetStaticMenu.GateWay[(cur_pos-13)/3] > 199 && NetStaticMenu.GateWay[(cur_pos-13)/3]<206)
					NetStaticMenu.GateWay[(cur_pos-13)/3] += 50;
				else if (((NetStaticMenu.GateWay[(cur_pos-13)/3]/10) == 10) || (NetStaticMenu.GateWay[(cur_pos-13)/3]<10))
					NetStaticMenu.GateWay[(cur_pos-13)/3] += 90;
				else 
					NetStaticMenu.GateWay[(cur_pos-13)/3] -= 10;
			}
			break;
		case 15:
		case 18:
		case 21:
		case 24:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getgateway[(cur_pos-13)/3]==250)
					getgateway[(cur_pos-13)/3] += 5;
				else if ((getgateway[(cur_pos-13)/3]%10)==0)
					getgateway[(cur_pos-13)/3] += 9;
				else 
					getgateway[(cur_pos-13)/3] -= 1;
			}
			else
			{
				if (NetStaticMenu.GateWay[(cur_pos-13)/3]==250)
					NetStaticMenu.GateWay[(cur_pos-13)/3] += 5;
				else if ((NetStaticMenu.GateWay[(cur_pos-13)/3]%10)==0)
					NetStaticMenu.GateWay[(cur_pos-13)/3] += 9;
				else 
					NetStaticMenu.GateWay[(cur_pos-13)/3] -= 1;
			}
			break;
		case 25:
		case 28:
		case 31:
		case 34:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getsubway[(cur_pos-25)/3]>55 && getsubway[(cur_pos-25)/3]<100)
					getsubway[(cur_pos-25)/3] += 100;
				else if (getsubway[(cur_pos-25)/3]<=55)
					getsubway[(cur_pos-25)/3] += 200;
				else 
					getsubway[(cur_pos-25)/3] -= 100;
			}
			else
			{
				if (NetStaticMenu.SubNet[(cur_pos-25)/3]>55 && NetStaticMenu.SubNet[(cur_pos-25)/3]<100)
					NetStaticMenu.SubNet[(cur_pos-25)/3] += 100;
				else if (NetStaticMenu.SubNet[(cur_pos-25)/3]<=55)
					NetStaticMenu.SubNet[(cur_pos-25)/3] += 200;
				else 
					NetStaticMenu.SubNet[(cur_pos-25)/3] -= 100;
			}
			break;
		case 26:
		case 29:
		case 32:
		case 35:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getsubway[(cur_pos-25)/3]>205 &&getsubway[(cur_pos-25)/3]<210)
					getsubway[(cur_pos-25)/3] += 40;
				else if (getsubway[(cur_pos-25)/3] > 199 && getsubway[(cur_pos-25)/3]<206)
					getsubway[(cur_pos-25)/3] += 50;
				else if (((getsubway[(cur_pos-25)/3]/10) == 10) || (getsubway[(cur_pos-25)/3]<10))
					getsubway[(cur_pos-25)/3] += 90;
				else 
					getsubway[(cur_pos-25)/3] -= 10;
			}
			else
			{
				if (NetStaticMenu.SubNet[(cur_pos-25)/3]>205 && NetStaticMenu.SubNet[(cur_pos-25)/3]<210)
					NetStaticMenu.SubNet[(cur_pos-25)/3] += 40;
				else if (NetStaticMenu.SubNet[(cur_pos-25)/3] > 199 && NetStaticMenu.SubNet[(cur_pos-25)/3]<206)
					NetStaticMenu.SubNet[(cur_pos-25)/3] += 50;
				else if (((NetStaticMenu.SubNet[(cur_pos-25)/3]/10) == 10) || (NetStaticMenu.SubNet[(cur_pos-25)/3]<10))
					NetStaticMenu.SubNet[(cur_pos-25)/3] += 90;
				else 
					NetStaticMenu.SubNet[(cur_pos-25)/3] -= 10;
			}
			break;
		case 27:
		case 30:
		case 33:
		case 36:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getsubway[(cur_pos-25)/3]==250)
					getsubway[(cur_pos-25)/3] += 5;
				else if ((getsubway[(cur_pos-25)/3]%10)==0)
					getsubway[(cur_pos-25)/3] += 9;
				else 
					getsubway[(cur_pos-25)/3] -= 1;
			}
			else
			{
				if (NetStaticMenu.SubNet[(cur_pos-25)/3]==250)
					NetStaticMenu.SubNet[(cur_pos-25)/3] += 5;
				else if ((NetStaticMenu.SubNet[(cur_pos-25)/3]%10)==0)
					NetStaticMenu.SubNet[(cur_pos-25)/3] += 9;
				else 
					NetStaticMenu.SubNet[(cur_pos-25)/3] -= 1;
			}
			break;
		case 37:
		case 40:
		case 43:
		case 46:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getddns1[(cur_pos-37)/3]>55 && getddns1[(cur_pos-37)/3]<100)
					getddns1[(cur_pos-37)/3] += 100;
				else if (getddns1[(cur_pos-37)/3]<=55)
					getddns1[(cur_pos-37)/3] += 200;
				else 
					getddns1[(cur_pos-37)/3] -= 100;
			}
			else
			{
				if (NetStaticMenu.DDNS1[(cur_pos-37)/3]>55 && NetStaticMenu.DDNS1[(cur_pos-37)/3]<100)
					NetStaticMenu.DDNS1[(cur_pos-37)/3] += 100;
				else if (NetStaticMenu.DDNS1[(cur_pos-37)/3]<=55)
					NetStaticMenu.DDNS1[(cur_pos-37)/3] += 200;
				else 
					NetStaticMenu.DDNS1[(cur_pos-37)/3] -= 100;
			}
			break;
		case 38:
		case 41:
		case 44:
		case 47:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getddns1[(cur_pos-37)/3]>205 && getddns1[(cur_pos-37)/3]<210)
					getddns1[(cur_pos-37)/3] += 40;
				else if (getddns1[(cur_pos-37)/3] > 199 && getddns1[(cur_pos-37)/3]<206)
					getddns1[(cur_pos-37)/3] += 50;
				else if (((getddns1[(cur_pos-37)/3]/10) == 10) || (getddns1[(cur_pos-37)/3]<10))
					getddns1[(cur_pos-37)/3] += 90;
				else 
					getddns1[(cur_pos-37)/3] -= 10;
			}
			else
			{
				if (NetStaticMenu.DDNS1[(cur_pos-37)/3]>205 && NetStaticMenu.DDNS1[(cur_pos-37)/3]<210)
					NetStaticMenu.DDNS1[(cur_pos-37)/3] += 40;
				else if (NetStaticMenu.DDNS1[(cur_pos-37)/3] > 199 && NetStaticMenu.DDNS1[(cur_pos-37)/3]<206)
					NetStaticMenu.DDNS1[(cur_pos-37)/3] += 50;
				else if (((NetStaticMenu.DDNS1[(cur_pos-37)/3]/10) == 10) || (NetStaticMenu.DDNS1[(cur_pos-37)/3]<10))
					NetStaticMenu.DDNS1[(cur_pos-37)/3] += 90;
				else 
					NetStaticMenu.DDNS1[(cur_pos-37)/3] -= 10;
			}
			break;
		case 39:
		case 42:
		case 45:
		case 48:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getddns1[(cur_pos-37)/3]==250)
					getddns1[(cur_pos-37)/3] += 5;
				else if ((getddns1[(cur_pos-37)/3]%10)==0)
					getddns1[(cur_pos-37)/3] += 9;
				else 
					getddns1[(cur_pos-37)/3] -= 1;
			}
			else
			{
				if (NetStaticMenu.DDNS1[(cur_pos-37)/3]==250)
					NetStaticMenu.DDNS1[(cur_pos-37)/3] += 5;
				else if ((NetStaticMenu.DDNS1[(cur_pos-37)/3]%10)==0)
					NetStaticMenu.DDNS1[(cur_pos-37)/3] += 9;
				else 
					NetStaticMenu.DDNS1[(cur_pos-37)/3] -= 1;
			}
			break;
		case 49:
		case 52:
		case 55:
		case 58:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getddns2[(cur_pos-49)/3]>55 && getddns2[(cur_pos-49)/3]<100)
					getddns2[(cur_pos-49)/3] += 100;
				else if (getddns2[(cur_pos-49)/3]<=55)
					getddns2[(cur_pos-49)/3] += 200;
				else 
					getddns2[(cur_pos-49)/3] -= 100;
			}
			else
			{
				if (NetStaticMenu.DDNS2[(cur_pos-49)/3]>55 && NetStaticMenu.DDNS2[(cur_pos-49)/3]<100)
					NetStaticMenu.DDNS2[(cur_pos-49)/3] += 100;
				else if (NetStaticMenu.DDNS2[(cur_pos-49)/3]<=55)
					NetStaticMenu.DDNS2[(cur_pos-49)/3] += 200;
				else 
					NetStaticMenu.DDNS2[(cur_pos-49)/3] -= 100;
			}
			break;
		case 50:
		case 53:
		case 56:
		case 59:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getddns2[(cur_pos-49)/3]>205 && getddns2[(cur_pos-49)/3]<210)
					getddns2[(cur_pos-49)/3] += 40;
				else if (getddns2[(cur_pos-49)/3] > 199 && getddns2[(cur_pos-49)/3]<206)
					getddns2[(cur_pos-49)/3] += 50;
				else if (((getddns2[(cur_pos-49)/3]/10) == 10) || (getddns2[(cur_pos-49)/3]<10))
					getddns2[(cur_pos-49)/3] += 90;
				else 
					getddns2[(cur_pos-49)/3] -= 10;
			}
			else
			{
				if (NetStaticMenu.DDNS2[(cur_pos-49)/3]>205 && NetStaticMenu.DDNS2[(cur_pos-49)/3]<210)
					NetStaticMenu.DDNS2[(cur_pos-49)/3] += 40;
				else if (NetStaticMenu.DDNS2[(cur_pos-49)/3] > 199 && NetStaticMenu.DDNS2[(cur_pos-49)/3]<206)
					NetStaticMenu.DDNS2[(cur_pos-49)/3] += 50;
				else if (((NetStaticMenu.DDNS2[(cur_pos-49)/3]/10) == 10) || (NetStaticMenu.DDNS2[(cur_pos-49)/3]<10))
					NetStaticMenu.DDNS2[(cur_pos-49)/3] += 90;
				else 
					NetStaticMenu.DDNS2[(cur_pos-49)/3] -= 10;
			}
			break;
		case 51:
		case 54:
		case 57:
		case 60:
			wrflagnet = 1;
			if (NetStaticMenu.IPAlloction)
			{
				if (getddns2[(cur_pos-49)/3]==250)
					getddns2[(cur_pos-49)/3] += 5;
				else if ((getddns2[(cur_pos-49)/3]%10)==0)
					getddns2[(cur_pos-49)/3] += 9;
				else 
					getddns2[(cur_pos-49)/3] -= 1;
			}
			else
			{
				if (NetStaticMenu.DDNS2[(cur_pos-49)/3]==250)
					NetStaticMenu.DDNS2[(cur_pos-49)/3] += 5;
				else if ((NetStaticMenu.DDNS2[(cur_pos-49)/3]%10)==0)
					NetStaticMenu.DDNS2[(cur_pos-49)/3] += 9;
				else 
					NetStaticMenu.DDNS2[(cur_pos-49)/3] -= 1;
			}
			break;
		case 61:
			wrflagnet = 1;
			temp = NetStaticMenu.Port/1000;
			NetStaticMenu.Port = NetStaticMenu.Port-temp*1000;
			if (temp==0)
				temp=9;
			else 
				temp--;
			NetStaticMenu.Port=NetStaticMenu.Port+temp*1000;
			break;
		case 62:
			wrflagnet = 1;
			temp = (NetStaticMenu.Port%1000)/100;
			NetStaticMenu.Port = NetStaticMenu.Port-temp*100;
			if (temp==0)
				temp=9;
			else 
				temp--;
			NetStaticMenu.Port=NetStaticMenu.Port+temp*100;
			break;
		case 63:
			wrflagnet = 1;
			temp = (NetStaticMenu.Port%1000%100)/10;
			NetStaticMenu.Port = NetStaticMenu.Port-temp*10;
			if (temp==0)
				temp=9;
			else 
				temp--;
			NetStaticMenu.Port=NetStaticMenu.Port+temp*10;
			break;
		case 64:
			wrflagnet = 1;
			temp = NetStaticMenu.Port%1000%100%10;
			NetStaticMenu.Port = NetStaticMenu.Port-temp;
			if (temp==0)
				temp=9;
			else 
				temp--;
			NetStaticMenu.Port=NetStaticMenu.Port+temp;
			break;
		case 65:
			wrflagnet = 1;
			temp = NetStaticMenu.IEPort/1000;
			NetStaticMenu.IEPort = NetStaticMenu.IEPort-temp*1000;
			if (temp==0)
				temp=9;
			else 
				temp--;
			NetStaticMenu.IEPort=NetStaticMenu.IEPort+temp*1000;
			break;
		case 66:
			wrflagnet = 1;
			temp = (NetStaticMenu.IEPort%1000)/100;
			NetStaticMenu.IEPort = NetStaticMenu.IEPort-temp*100;
			if (temp==0)
				temp=9;
			else 
				temp--;
			NetStaticMenu.IEPort=NetStaticMenu.IEPort+temp*100;
			break;
		case 67:
			wrflagnet = 1;
			temp = (NetStaticMenu.IEPort%1000%100)/10;
			NetStaticMenu.IEPort = NetStaticMenu.IEPort-temp*10;
			if (temp==0)
				temp=9;
			else 
				temp--;
			NetStaticMenu.IEPort=NetStaticMenu.IEPort+temp*10;
			break;
		case 68:
			wrflagnet = 1;
			temp = NetStaticMenu.IEPort%1000%100%10;
			NetStaticMenu.IEPort = NetStaticMenu.IEPort-temp;
			if (temp==0)
				temp=9;
			else 
				temp--;
			NetStaticMenu.IEPort=NetStaticMenu.IEPort+temp;
			break;
	}
	if (cur_pos != 0)
		menu_netstatic_data_disp(cur_pos,RED);
}

void menu_netstatic_off()
{
	if (wrflagnet)
		MenuNetStaticInit();
    	OSD_ALL_CLEAR();
	menu_main_on();
}
//--------------------

void menu_netstatic_enter()
{
	U8 i;
	if (cur_pos==NETSTATICDATA-2)
	{
		if (netdhcpget)
		{
			netdhcpget = 0;
			for (i=0;i<4;i++)
			{
				if (getddns1[i] != autodns1[i] || getddns2[i] != autodns2[i])
				{
					bReset = 1;
					save_net();
					return;
				}
			}
		}
		if (wrflagnet)
			menu_netstatic_do();
		menu_netstatic_off();
	}
	else if (cur_pos==NETSTATICDATA-1)
	{
		menu_netstatic_off();
	}
	else if (cur_pos==NETSTATICDATA-3)
		menu_email_on();
}

void MouseNETRClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_netpos[cur_pos*4]+offsetx_hd;
		miny = Pre_netpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_netpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_netpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_netpos[cur_pos*4]+offsetx;
		miny = Pre_netpos[cur_pos*4+1]+offsety;
		maxx = Pre_netpos[cur_pos*4+2]+offsetx;
		maxy = Pre_netpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
		menu_netstatic_add(); 
}

void MouseNETLClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_netpos[cur_pos*4]+offsetx_hd;
		miny = Pre_netpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_netpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_netpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_netpos[cur_pos*4]+offsetx;
		miny = Pre_netpos[cur_pos*4+1]+offsety;
		maxx = Pre_netpos[cur_pos*4+2]+offsetx;
		maxy = Pre_netpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		if (cur_pos<NETSTATICDATA-3)
			menu_netstatic_dec(); 
		else
			menu_netstatic_enter();
	}
}

void Mouse_Net_Move_Func(U16 x,U16 y)
{
	U8 i;
	U16 minx,miny,maxx,maxy;
	for (i=0;i<NETSTATICDATA;i++)
	{
		if (MseType)
		{
			minx = Pre_netpos[i*4]+offsetx_hd;
			miny = Pre_netpos[i*4+1]+offsety_hd;
			maxx = Pre_netpos[i*4+2]+offsetx_hd;
			maxy = Pre_netpos[i*4+3]+offsety_hd;
		}
		else
		{
			minx = Pre_netpos[i*4]+offsetx;
			miny = Pre_netpos[i*4+1]+offsety;
			maxx = Pre_netpos[i*4+2]+offsetx;
			maxy = Pre_netpos[i*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != i))
		{
//			if (NetStaticMenu.IPAlloction && ((i>0 && i<10) || (i>12&& i<37)))
//				break;
//			else
//			{
				menu_netstatic_data_disp(cur_pos,WHITE);
				cur_pos=i;
				menu_netstatic_data_disp(cur_pos,RED);
				break;
//			}
		}
	}
}

void mNETSTATIC_key_func()
{
	if(byKey_val==0) return;

	switch(byKey_val)
	{
	case kESC:	//menu/exit   full/zoom      
		menu_netstatic_off();
		break;
	case kUP:	   //PIP        up
		menu_netstatic_up();
   		break;
	case kDN:        //4SP        down
		menu_netstatic_down();
		break;
	case kLT:
	case kDE:
		menu_netstatic_dec();
		break;
	case kRH:
	case kAD:
		menu_netstatic_add();
		break;
	case kET:
		menu_netstatic_enter();
		break;
    }
	byKey_val=0;
}

