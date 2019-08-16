#include "cmn.h"
#include "Mail.h"
ValEmail EmailMenu;
EMAILMENU emailmenupic;

U8 keyshift = 0;
U8 keypos=0;
U8 inputmode = 0;
U8 cureditpos = 0;
U8 passwordconfirm[30];
U8 testflag=0;
#define EMAILDATA	  10

U8 KeyBorada[]=
{
	39,39,36
};

U16 Menu_Email_DataPos[]=
{
	248,58,
	248,84,
	248,110,
	248,136,
	248,162,
	248,188,
	248,214,
	68,342,	242,342,	416,342
};

U16 Menu_Buttona_Pos[]=
{
	40,258,	68,258,	96,258,	124,258,	152,258,	180,258,	208,258,	236,258,	264,258,	292,258,	320,258,	348,258,	376,258,	404,258,	432,258,	460,258,	488,258,	516,258,	544,258,	572,258,
	40,286,	68,286,	96,286,	124,286,	152,286,	180,286,	208,286,	236,286,	264,286,	292,286,	320,286,	348,286,	376,286,	404,286,	432,286,	460,286,	488,286,	516,286,	544,286
};

U16 Menu_Buttonb_Pos[]=
{
	40,258,	68,258,	96,258,	124,258,	152,258,	180,258,	208,258,	236,258,	264,258,	292,258,	320,258,	348,258,	376,258,	404,258,	432,258,	460,258,	488,258,	544,258,	572,258,
	40,286,	68,286,	96,286,	124,286,	152,286,	180,286,	208,286,	236,286,	264,286,	292,286,	320,286,	348,286,	376,286,	404,286,	432,286,	460,286,	544,286
};

U16* Menu_Button_Pos[]=
{
	Menu_Buttona_Pos,
	Menu_Buttona_Pos,
	Menu_Buttonb_Pos
};

U8 Menu_Char1_num[]=
{
	49,50,51,52,53,54,55,56,57,48,97,98,99,100,101,102,103,104,128,130,
	105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,129
};

U8 Menu_Char2_num[]=
{
	49,50,51,52,53,54,55,56,57,48,65,66,67,68,69,70,71,72,128,130,
	73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,129
};

U8 Menu_Char3_num[]=
{
	33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,58,64,128,130,
	59,60,61,62,63,91,92,93,94,95,96,123,124,125,126,32,129
};

U8* Menu_Char_num[]=
{
	Menu_Char1_num,
	Menu_Char2_num,
	Menu_Char3_num
};

U16 Pre_emailpos[] = 
{
	248,58,	319,78,
	248,84,	608,104,
	248,110,	608,130,
	248,136,	608,156,
	248,162,	608,182,
	248,188,	608,208,
	248,208,	608,228,

	68,342,	228,376,

	242,342,	402,376,

	416,342,	576,376,

	36,250,	607,322
};

U8* SmtpServer[]=
{
//	"smtp.foxmail.com.cn",		"foxmail.com.cn",
	"mail.bodachina.com",		"mail.bodachina.com",
//	"smtp.21cn.com",			"21cn.com",
//	"smtp.sina.com.cn",		"sina.com.cn",
//	"smtp.163.net",			"163.net",
//	"smtp.tom.com",			"tom.com",
//	"smtp.163.com",			"163.com",
//	"smtp.126.com",			"126.com",
//	"smtp.qq.com",			"qq.com",
//	"mail.fm365.com",			"fm365.com",
//	"smtp.263.net",			"263.net",
//	"smtp.x263.net",			"x263.net",
	"263.net.cn",				"263.net.cn",
//	"smtp.elong.com",			"elong.com",
//	"smtp.chinaren.com",		"chinaren.com",
//	"email.km169.net",			"km169.net",
//	"smtp.cmmail.com",		"cmmail.com",
//	"smtp.china.com",			"china.com",
//	"smtp.eazier.com",			"eazier.com",
//	"smtp.sohu.com",			"sohu.com",
//	"smtp.fd21.com",			"fd21.com",
//	"smtp.citiz.net",			"citiz.net",
//	"smtp.yeah.net",			"yeah.net",
//	"smtp.netease.com",		"netease.com",
	"smtp.free.etang.com",		"etang.com",
//	"smtp.myrice.com",		"myrice.com",
//	"mail.wx88.net",			"wx88.net",
	"www.email.com.cn",		"email.com.cn",
//	"smtp.cn99.com",			"cn99.com",
//	"smtp.tonghua.com.cn",		"tonghua.com.cn",
//	"mail.china1mail.com",		"china1mail.com",
//	"smtp.371.net",			"371.net",
//	"smtp.kali.com.cn",			"kali.com.cn",
//	"smtp.focus.com.cn",		"focus.com.cn",
//	"smtp.8848.net",			"8848.net",
	"email.com.cn",			"mailbox.com.cn",
	"990.net",				"990.net",
	"202.100.222.86",			"88998.com",
	"210.66.17.1",				"ucs.com.tw",
//	"mail.ctn.com.cn",			"ctn.com.cn",
//	"smtp.2911.net",			"2911.net",
//	"freemail.soim.com",		"soim.com",
	"sh.soim.com",			"sh.soim.com",
//	"mail.777.net.cn",			"777.net.cn",
   //    "smtp.telekbird.com.cn",	"telekbird.com.cn",
       "btamail.net.cn",			"btamail.net.cn",
       "chinanew.com",			"chinanew.com",
       "gzyp21.net",				"gzyp21.net",
       "freemail.online.tj.cn",		"freemail.online.tj.cn",
//       "smtp.363.net",			"363.net",
       "smtp.email.com.cn",		"w4f.com",
//       "www.dl163.com",			"dl163.com",
       "www.email.com.cn",		"chinaxian.com",
       "2888.com",				"2888.com",
//       "smtp.yes100.com"	,		"yes100.com",
//       "smtp.chinese.com",		"chinese.com",
       "yifan.net",				"yifan.net",
//       "mail.china183.com",		"china183.com",
//       "smtp.20520.com",			"20520.com",
//       "smtp.elawchina.com",		"elawchina.com",
       "smtp.email.com.cn",		"fugao.net",
//       "smtp.mybada.net"	,		"mybada.net",
//       "email.gz168.net",			"gz168.net",
//       "mail.kzinfo.net",			"kzinfo.net",
//       "smtp.7119.net",			"7119.net",
//       "smtp.ccident.com",		"ccidnet.com",
//       "www.cfe21.com",			"cfe21.com",
//       "smtp.etang.com",			"etang.com",
       "smtp.email.com.cn",		"hnet.com.cn",
//       "freemail.nethao.com",		"nethao.com",
//       "smtp.soyou.net",			"soyou.net",
       "smtp.email.com.cn",		"newstar.com.cn",
//       "citymail.cityhot.com",		"cityhot.com",
//       "mail.dfworld.net",			"dfworld.net",
       "www.email.com.cn",		"gzboy.com",
//       "freemail.fengyun.net",		"fengyun.net",
//       "smtp.citiz.net",			"citiz.net",
       "smtp.email.com.cn",		"tt.zj.cn",
	"100m.hl.cninfo.net",		"100m.hl.cninfo.net",
       "dg.163mail.net",			"dg.163mail.net",
       "public.guangzhou.gd.cn",	"public.guangzhou.gd.cn",
//       "smtp.bodachina.com",		"bodachina.com",
       "freemail.nx.cninfo.net ",	"freemail.nx.cninfo.net",
//       "smtp.softhome.net ",		"softhome.net",
//       "smtp.yesky.com",			"yesky.com",
//       "smtp.alibaba.com",		"alibaba.com",
//       "smtp.mweb.com.cn",		"mweb.com.cn",
       "210.77.145.189",			"51.com",
//       "email.km169.net",			"km169.net",
//       "wocall.com",				"wocall.com",
       "guohao.com",			"guohao.com",
       "bjmail.net",				"bjmail.net",
       "gzpublic.guangzhou.gd.cn",	"gzpublic.guangzhou.gd.cn",
//       "smtp.bj.cnuninet.com",		"cnuninet.com",
//       "smtp.cmmail.com",		"cmmail.com",
//       "smtp.peoplemail.com.cn",	"peoplemail.com.cn",
//       "smtp.21cn.net",			"21cn.net",
//       "mail.sh163.net",			"sh163.net",
       "smtp.mail.yahoo.com",		"yahoo.com",
       "smtp.mail.yahoo.com.cn",	"yahoo.com.cn",
       "smtp.mail.yahoo.com.tw",	"yahoo.com.tw",
       "smtp.mail.yahoo.com.hk",	"yahoo.com.hk",
       "smtp.mail.yahoo.com.mx",	"yahoo.com.mx",
       "smtp.mail.yahoo.com.br",	"yahoo.com.br",
       "smtp.mail.yahoo.com.ar",	"yahoo.com.ar",
       "smtp.mail.yahoo.com.au",	"yahoo.com.au",
       "smtp.mail.yahoo.co.kr",	"yahoo.co.kr",
       "smtp.mail.yahoo.co.uk",	"yahoo.co.uk",
       "smtp.mail.yahoo.co.jp",	"yahoo.co.jp",
       "smtp.mail.yahoo.ca",		"yahoo.ca",
       "smtp.mail.yahoo.dk",		"yahoo.dk",
       "smtp.mail.yahoo.de",		"yahoo.de",
       "smtp.mail.yahoo.fr",		"yahoo.fr",
       "smtp.mail.yahoo.es",		"yahoo.es",
       "smtp.mail.yahoo.it",		"yahoo.it",
       "smtp.mail.yahoo.se",		"yahoo.se",
       "smtp.mail.yahoo.no",		"yahoo.no",
	NULL
};
void MenuEmailInit()
{
	U8 tmp[2];
	I_read_block_eeprom(ADDEMAIL,1,&EmailMenu.TimeTev);
//	printf("1111111111111111111 timetev:%d\n",EmailMenu.TimeTev);
	I_read_block_eeprom(ADDEMAIL+EMAILPORT,2,tmp);
	EmailMenu.EmailPort = (tmp[0]<<8) | tmp[1];
//	printf("22222222222222222tmp[0]:%d,tmp[1]:%d, EmailPort:%d\n",tmp[0],tmp[1],EmailMenu.EmailPort);
	I_read_block_eeprom(ADDEMAIL+ESERVERADD,30,EmailMenu.EServerAdd);
//	printf("3333333333333333333333333333 EServerAdd:%s\n",EmailMenu.EServerAdd);
	I_read_block_eeprom(ADDEMAIL+EUSERNAME,30,EmailMenu.EUserName);
//	printf("444444444444444444444 EUserName:%s\n",EmailMenu.EUserName);
	I_read_block_eeprom(ADDEMAIL+EPASSWORD,30,EmailMenu.EPassWord);
//	printf("5555555555555555555555 EPassWord:%s\n",EmailMenu.EPassWord);
	I_read_block_eeprom(ADDEMAIL+ERECIVEADD,30,EmailMenu.EReciveAdd);
//	printf("666666666666666666666666666 EReciveAdd:%s\n",EmailMenu.EReciveAdd);
}

void menu_email_data_disp(U8 pos,U16 color)
{
}

void email_keyboard_disp(U8 pos,U16 color)
{
}

void menu_email_on()
{
}

void menu_email_down()
{
	if (inputmode)
	{
		email_keyboard_disp(keypos,WHITE);
		if (keypos == KeyBorada[keyshift]-1)
			keypos = 0;
		else
			keypos++;
		email_keyboard_disp(keypos,RED);
	}
	else
	{
		menu_email_data_disp(cur_pos,WHITE);
		if (cur_pos == EMAILDATA-1)
			cur_pos = 0;
		else
			cur_pos++;
		menu_email_data_disp(cur_pos,RED);
	}
}
void menu_email_up()
{
	if (inputmode)
	{
		email_keyboard_disp(keypos,WHITE);
		if (keypos == 0)
			keypos = KeyBorada[keyshift]-1;
		else
			keypos--;
		email_keyboard_disp(keypos,RED);
	}
	else
	{
		menu_email_data_disp(cur_pos,WHITE);
		if (cur_pos == 0)
			cur_pos = EMAILDATA-1;
		else
			cur_pos--;
		menu_email_data_disp(cur_pos,RED);
	}
}

U8 menu_email_save()
{
	U8 tmp[2];
	U8 i;
	if (passwordconfirm[0]==0 || EmailMenu.EPassWord[0]==0 || EmailMenu.EReciveAdd[0]==0 || EmailMenu.EUserName[0] == 0 || EmailMenu.EServerAdd[0]==0)
	{
		InfoId = INFONOTFULLERR;
		MenuErrOn();
		return 0;
	}
	for (i=0;i<30;i++)
	{
		if (passwordconfirm[i] != EmailMenu.EPassWord[i])
		{
			InfoId = PASSWORDERR;
			MenuErrOn();
			return 0;
		}
	}
	I_write_block_eeprom(ADDEMAIL,1,&EmailMenu.TimeTev);
	tmp[1]= EmailMenu.EmailPort;
	tmp[0]= EmailMenu.EmailPort>>8;
	I_write_block_eeprom(ADDEMAIL+EMAILPORT,2,tmp);
	I_write_block_eeprom(ADDEMAIL+ESERVERADD,30,EmailMenu.EServerAdd);
	I_write_block_eeprom(ADDEMAIL+EUSERNAME,30,EmailMenu.EUserName);
	I_write_block_eeprom(ADDEMAIL+EPASSWORD,30,EmailMenu.EPassWord);
	I_write_block_eeprom(ADDEMAIL+ERECIVEADD,30,EmailMenu.EReciveAdd);
	return 1;
}
extern U8 getip[];

void mail_send(U16 cam,U8 event)
{

	char buf[100];
	char albuf[256];
	char midbuf[256];
	char* p=NULL;
	int ret,l=0;
	U8 i;
	
	
	testflag = 0;
	if (strlen(EmailMenu.EServerAdd)==0 || strlen(EmailMenu.EReciveAdd)==0 || strlen(EmailMenu.EUserName)==0)
		return;
	memset(buf,0x00,sizeof(buf));
	memset(albuf,0x00,sizeof(albuf));
	memset(midbuf,0x00,sizeof(midbuf));
	while(SmtpServer[l*2] != NULL)
	{
		p = strstr(SmtpServer[l*2],EmailMenu.EServerAdd);
		if (p == SmtpServer[l*2])
			break;
		else l++;
	}

	if (p==NULL)
	{
		p = index(EmailMenu.EServerAdd,'.');
		p++;
		if (p[0] == 0)
			return;
		sprintf(buf,"%s@%s",EmailMenu.EUserName,p);
	}
	else
		sprintf(buf,"%s@%s",EmailMenu.EUserName,SmtpServer[l*2+1]);
//	printf("p:####%s####\n",p);
//	printf("buf:####%s####\n",buf);
	for (i=0;i<16;i++)
	{
		memset(midbuf,0x00,sizeof(midbuf));
		if (WBIT(i)&cam)
		{
			switch(event)
			{
				case Emotion:
					if (NetStaticMenu.IPAlloction)
					{
						if (SysMenu.tmregion == 0)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Motion at %d-%02d-%02d %02d:%02d:%02d \n",getip[0],getip[1],getip[2],getip[3],i+1,buf_time[5]+2000,buf_time[4],buf_time[3],buf_time[2],buf_time[1],buf_time[0]);
						else if (SysMenu.tmregion == 2)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Motion at %02d-%02d-%d %02d:%02d:%02d\n",getip[0],getip[1],getip[2],getip[3],i+1,buf_time[3],buf_time[4],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
						else if (SysMenu.tmregion == 1)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Motion at %02d-%02d-%d %02d:%02d:%02d\n",getip[0],getip[1],getip[2],getip[3],i+1,buf_time[4],buf_time[3],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
					}
					else
					{
						if (SysMenu.tmregion == 0)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Motion at %d-%02d-%02d %02d:%02d:%02d \n",NetStaticMenu.IpAddress[0],NetStaticMenu.IpAddress[1],NetStaticMenu.IpAddress[2],NetStaticMenu.IpAddress[3],i+1,buf_time[5]+2000,buf_time[4],buf_time[3],buf_time[2],buf_time[1],buf_time[0]);
						else if (SysMenu.tmregion == 2)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Motion at %02d-%02d-%d %02d:%02d:%02d\n",NetStaticMenu.IpAddress[0],NetStaticMenu.IpAddress[1],NetStaticMenu.IpAddress[2],NetStaticMenu.IpAddress[3],i+1,buf_time[3],buf_time[4],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
						else if (SysMenu.tmregion == 1)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Motion at %02d-%02d-%d %02d:%02d:%02d\n",NetStaticMenu.IpAddress[0],NetStaticMenu.IpAddress[1],NetStaticMenu.IpAddress[2],NetStaticMenu.IpAddress[3],i+1,buf_time[4],buf_time[3],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
					}
					break;
				case Esensor:
					if (NetStaticMenu.IPAlloction)
					{
						if (SysMenu.tmregion == 0)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Sensor at %d-%02d-%02d %02d:%02d:%02d \n",getip[0],getip[1],getip[2],getip[3],i+1,buf_time[5]+2000,buf_time[4],buf_time[3],buf_time[2],buf_time[1],buf_time[0]);
						else if (SysMenu.tmregion == 2)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Sensor at %02d-%02d-%d %02d:%02d:%02d\n",getip[0],getip[1],getip[2],getip[3],i+1,buf_time[3],buf_time[4],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
						else if (SysMenu.tmregion == 1)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Sensor at %02d-%02d-%d %02d:%02d:%02d\n",getip[0],getip[1],getip[2],getip[3],i+1,buf_time[4],buf_time[3],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
					}
					else
					{
						if (SysMenu.tmregion == 0)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Sensor at %d-%02d-%02d %02d:%02d:%02d \n",NetStaticMenu.IpAddress[0],NetStaticMenu.IpAddress[1],NetStaticMenu.IpAddress[2],NetStaticMenu.IpAddress[3],i+1,buf_time[5]+2000,buf_time[4],buf_time[3],buf_time[2],buf_time[1],buf_time[0]);
						else if (SysMenu.tmregion == 2)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Sensor at %02d-%02d-%d %02d:%02d:%02d\n",NetStaticMenu.IpAddress[0],NetStaticMenu.IpAddress[1],NetStaticMenu.IpAddress[2],NetStaticMenu.IpAddress[3],i+1,buf_time[3],buf_time[4],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
						else if (SysMenu.tmregion == 1)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Sensor at %02d-%02d-%d %02d:%02d:%02d\n",NetStaticMenu.IpAddress[0],NetStaticMenu.IpAddress[1],NetStaticMenu.IpAddress[2],NetStaticMenu.IpAddress[3],i+1,buf_time[4],buf_time[3],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
					}
					break;
				case Evloss:
					if (NetStaticMenu.IPAlloction)
					{
						if (SysMenu.tmregion == 0)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Loss at %d-%02d-%02d %02d:%02d:%02d \n",getip[0],getip[1],getip[2],getip[3],i+1,buf_time[5]+2000,buf_time[4],buf_time[3],buf_time[2],buf_time[1],buf_time[0]);
						else if (SysMenu.tmregion == 2)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Loss at %02d-%02d-%d %02d:%02d:%02d\n",getip[0],getip[1],getip[2],getip[3],i+1,buf_time[3],buf_time[4],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
						else if (SysMenu.tmregion == 1)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Loss at %02d-%02d-%d %02d:%02d:%02d\n",getip[0],getip[1],getip[2],getip[3],i+1,buf_time[4],buf_time[3],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
					}
					else
					{
						if (SysMenu.tmregion == 0)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Loss at %d-%02d-%02d %02d:%02d:%02d \n",NetStaticMenu.IpAddress[0],NetStaticMenu.IpAddress[1],NetStaticMenu.IpAddress[2],NetStaticMenu.IpAddress[3],i+1,buf_time[5]+2000,buf_time[4],buf_time[3],buf_time[2],buf_time[1],buf_time[0]);
						else if (SysMenu.tmregion == 2)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Loss at %02d-%02d-%d %02d:%02d:%02d\n",NetStaticMenu.IpAddress[0],NetStaticMenu.IpAddress[1],NetStaticMenu.IpAddress[2],NetStaticMenu.IpAddress[3],i+1,buf_time[3],buf_time[4],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
						else if (SysMenu.tmregion == 1)
							sprintf(midbuf,"IP:%d.%d.%d.%d Cam:%02d is Loss at %02d-%02d-%d %02d:%02d:%02d\n",NetStaticMenu.IpAddress[0],NetStaticMenu.IpAddress[1],NetStaticMenu.IpAddress[2],NetStaticMenu.IpAddress[3],i+1,buf_time[4],buf_time[3],buf_time[5]+2000,buf_time[2],buf_time[1],buf_time[0]);
					}
					break;
				default:
					break;
			}
//			printf("midbuf:%s\n",midbuf);
			strcat(albuf,midbuf);
//			printf("albuf:%s\n",albuf);
		}
		
	}

//	ret = gstCommonParam->GST_NET_dvr_send_mail(EmailMenu.EServerAdd,EmailMenu.EmailPort,EmailMenu.EUserName,
//												EmailMenu.EPassWord,buf,EmailMenu.EReciveAdd,"DVR Alarm!",albuf,NULL,XPRIORITY_NORMAL);
	ret = dvr_send_mail(EmailMenu.EServerAdd,EmailMenu.EmailPort,EmailMenu.EUserName,
												EmailMenu.EPassWord,buf,EmailMenu.EReciveAdd,"DVR Alarm!",albuf,NULL,XPRIORITY_NORMAL);
	if (ret>0)
		printf("***********cam:%x,event:%d, mail:%s send ok!\n",cam,event,albuf);
	else
		printf("######## mail send fail!ret:%d\n",ret);

}

void email_send_test()
{
/*
	int ret,l=0;
	char buf[100];
	char* p=NULL;
	if (strlen(EmailMenu.EServerAdd)==0 || strlen(EmailMenu.EReciveAdd)==0 || strlen(EmailMenu.EUserName)==0)
		return;
	testflag = 0;
	memset(buf,0x00,sizeof(buf));
	while(SmtpServer[l*2] != NULL)
	{
		p = strstr(SmtpServer[l*2],EmailMenu.EServerAdd);
		if (p == SmtpServer[l*2])
			break;
		else l++;
	}
	if (p==NULL)
	{
		p = index(EmailMenu.EServerAdd,'.');
		p++;
		if (p[0] == 0)
			return;
		sprintf(buf,"%s@%s",EmailMenu.EUserName,p);
	}
	else
		sprintf(buf,"%s@%s",EmailMenu.EUserName,SmtpServer[l*2+1]);
//	printf("buf:####%s####\n",buf);
	ret = gstCommonParam->GST_NET_dvr_send_mail(EmailMenu.EServerAdd,EmailMenu.EmailPort,EmailMenu.EUserName,
												EmailMenu.EPassWord,buf,EmailMenu.EReciveAdd,"DVR Mail Test!","This is a test mail!",NULL,XPRIORITY_NORMAL);
//	printf("*********************email test return ret:%d\n",ret);
	if (ret<0)
	{
		switch(ret)
		{
			case CONNECT_ERROR:
			case CONNECTSERVER_DDNS_ERROR:
			case NETINIT_ERROR:
				InfoId = SERCONNETCTERR;
				break;
			case USER_ERROR:
			case PASS_ERROR:
			case MAIL_FROM_ERROR:
			case AUTH_LOGIN_ERROR:
				InfoId = USERNOTEXITERR;
				break;
			case WSA_RECV_ERROR:
			case WSA_SEND_ERROR:
				InfoId = SENDERR;
				break;
			default:
				InfoId = UNKNOWNERR;
				break;
				
		}
		MenuErrOn();
	}
	else
	{
		testflag = 1;
		InfoId = TESTOK;
		MenuErrOn();
	}
*/
}

void menu_email_off()
{
	if (inputmode)
		inputmode = 0;
	else
	 {
	 	OSD_ALL_CLEAR();
		menu_netstatic_on();
	}
}
//--------------------

void menu_email_enter()
{
	U8 i;
	if (cur_pos==EMAILDATA-2)
	{
		if (testflag==0)
		{
			InfoId = SENDTESTORNOT;
			MenuErrOn();
			return;
		}
		if (!menu_email_save())
			return;
		inputmode = 0;
		menu_email_off();
	}
	else if (cur_pos==EMAILDATA-1)
	{
		inputmode = 0;
		MenuEmailInit();
		menu_email_off();
	}
	else if (cur_pos==EMAILDATA-3)
		email_send_test();
	else
	{
		inputmode = 1;
		cureditpos = 0;
		switch(cur_pos)
		{
			case 1:
				for (i=0;i<30;i++)
				{
					if (EmailMenu.EServerAdd[i])
					{
						cureditpos++;
					}
				}
				break;
			case 3:
				for (i=0;i<30;i++)
				{
					if (EmailMenu.EUserName[i])
						cureditpos++;
				}
				break;
			case 4:
				for (i=0;i<30;i++)
				{
					if (EmailMenu.EPassWord[i])
						cureditpos++;
				}
				break;
			case 5:
				for (i=0;i<30;i++)
				{
					if (passwordconfirm[i])
						cureditpos++;
				}
				break;
			case 6:
				for (i=0;i<30;i++)
				{
					if (EmailMenu.EReciveAdd[i])
						cureditpos++;
				}
				break;
			default:
				break;
		}
		
	}
}

void charbreakback()
{
	switch(cur_pos)
	{
		case 0:
			EmailMenu.TimeTev = 0;
			cureditpos = 0;
			break;
		case 1:
			if (cureditpos>0)
				cureditpos --;
			EmailMenu.EServerAdd[cureditpos] = 0;
			break;
		case 2:
			EmailMenu.EmailPort = 0;
			cureditpos = 0;
			break;
		case 3:
			if (cureditpos>0)
				cureditpos --;
			EmailMenu.EUserName[cureditpos]= 0;
			break;
		case 4:
			if (cureditpos>0)
				cureditpos --;
			EmailMenu.EPassWord[cureditpos]=0;
			break;
		case 5:
			if (cureditpos>0)
				cureditpos --;
			passwordconfirm[cureditpos]=0;
			break;
		case 6:
			if (cureditpos>0)
				cureditpos --;
			EmailMenu.EReciveAdd[cureditpos]=0;
			break;
		default:
			break;
	}
	testflag = 0;
	menu_email_data_disp(cur_pos,RED);
}

void cleancharback()
{
	switch(cur_pos)
	{
		case 0:
			EmailMenu.TimeTev = 0;
			break;
		case 1:
			memset(EmailMenu.EServerAdd,0,sizeof(EmailMenu.EServerAdd));
			break;
		case 2:
			EmailMenu.EmailPort= 0;
			break;
		case 3:
			memset(EmailMenu.EUserName,0,sizeof(EmailMenu.EUserName));
			break;
		case 4:
			memset(EmailMenu.EPassWord,0,sizeof(EmailMenu.EPassWord));
			break;
		case 5:
			memset(passwordconfirm,0,sizeof(passwordconfirm));
			break;
		case 6:
			memset(EmailMenu.EReciveAdd,0,sizeof(EmailMenu.EReciveAdd));
			break;
		default:
			break;
	}
	cureditpos = 0;
	testflag = 0;
	menu_email_data_disp(cur_pos,RED);
}

void getinput()
{
	U16 tmp=0;
	testflag = 0;
	switch(cur_pos)
	{
		case 0:
			if (keyshift<2 && keypos<10)
			{
				if (cureditpos)
				{
					if (keypos<9)
						EmailMenu.TimeTev = (EmailMenu.TimeTev/10)*10+keypos+1;
					else
						EmailMenu.TimeTev = (EmailMenu.TimeTev/10)*10;
				}
				else
				{
					if (keypos<9)
						EmailMenu.TimeTev = EmailMenu.TimeTev%10 + (keypos+1)*10;
					else
						EmailMenu.TimeTev = EmailMenu.TimeTev%10;
					cureditpos = 1;
				}
			}
			break;
		case 1:
			EmailMenu.EServerAdd[cureditpos] = Menu_Char_num[keyshift][keypos];
			if (cureditpos<30)
				cureditpos++; 
			break;
		case 2:
			if (keyshift<2 && keypos<10)
			{
				if (cureditpos==0)
				{
					tmp = EmailMenu.EmailPort%1000;
					if (keypos<9)
						EmailMenu.EmailPort = tmp + 1000*(keypos+1);
					else
						EmailMenu.EmailPort = tmp;
				}
				else if (cureditpos==1)
				{
					tmp = EmailMenu.EmailPort-((EmailMenu.EmailPort%1000)/100)*100;
					if (keypos<9)
						EmailMenu.EmailPort = tmp + 100*(keypos+1);
					else
						EmailMenu.EmailPort = tmp; 
				}
				else if (cureditpos==2)
				{
					tmp = EmailMenu.EmailPort-((EmailMenu.EmailPort%100)/10)*10;
					if (keypos<9)
						EmailMenu.EmailPort = tmp + 10*(keypos+1);
					else
						EmailMenu.EmailPort = tmp; 
				}
				else if (cureditpos==3)
				{
					tmp = EmailMenu.EmailPort-(EmailMenu.EmailPort%10);
					if (keypos<9)
						EmailMenu.EmailPort = tmp + (keypos+1);
					else
						EmailMenu.EmailPort = tmp; 
				}
				if (cureditpos<3)
					cureditpos++;
			}
			break;
		case 3:
			EmailMenu.EUserName[cureditpos] = Menu_Char_num[keyshift][keypos];
			if (cureditpos<30)
				cureditpos++; 
			break;
		case 4:
			EmailMenu.EPassWord[cureditpos] = Menu_Char_num[keyshift][keypos];
			if (cureditpos<30)
				cureditpos++; 
			break;
		case 5:
			passwordconfirm[cureditpos] = Menu_Char_num[keyshift][keypos];
			if (cureditpos<30)
				cureditpos++; 
			break;
		case 6:
			EmailMenu.EReciveAdd[cureditpos] = Menu_Char_num[keyshift][keypos];
			if (cureditpos<30)
				cureditpos++; 
			break;
		default:
			break;
	}
	menu_email_data_disp(cur_pos,RED);
}

void email_keyboard_enter()
{
	U8 i;
	if (keypos == KeyBorada[keyshift]-1)
	{
		if (keyshift>=2)
			keyshift = 0;
		else 
			keyshift ++;
		keypos = KeyBorada[keyshift]-1;
		for (i=0;i<KeyBorada[keyshift];i++)
		{
			if(i!=keypos)email_keyboard_disp(i,WHITE);
			else email_keyboard_disp(keypos,RED);
		}
		return;
	}
	if (keyshift<2)
	{
		if (keypos == 18)
			charbreakback();
		else if (keypos == 19)
			cleancharback();
		else
			getinput();
	}
	else
	{
		if (keypos == 17)
			charbreakback();
		else if (keypos == 18)
			cleancharback();
		else
			getinput();
	}
}

void MouseEmailRClick(U16 x,U16 y)
{
	inputmode = 0;
}

void MouseEmailLClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (inputmode)
	{
		if (MseType)
		{
			minx = Menu_Button_Pos[keyshift][keypos*2]+offsetx_hd;
			miny = Menu_Button_Pos[keyshift][keypos*2+1]+offsety_hd;
		}
		else
		{
			minx = Menu_Button_Pos[keyshift][keypos*2]+offsetx;
			miny = Menu_Button_Pos[keyshift][keypos*2+1]+offsety;
		}
		if (keyshift<2)
		{
			if (keypos==KeyBorada[keyshift]-1)
				maxx = minx+56;
			else
				maxx = minx+28;
		}
		else
		{
			if ((keypos==KeyBorada[keyshift]-1) || (keypos==16))
				maxx = minx+56;
			else if (keypos==KeyBorada[keyshift]-2)
				maxx = minx+84;
			else
				maxx = minx+28;
		}
		maxy =miny+28;
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
			email_keyboard_enter();
	}
	else
	{
		if (MseType)
		{
			minx = Pre_emailpos[cur_pos*4]+offsetx_hd;
			miny = Pre_emailpos[cur_pos*4+1]+offsety_hd;
			maxx = Pre_emailpos[cur_pos*4+2]+offsetx_hd;
			maxy = Pre_emailpos[cur_pos*4+3]+offsety_hd;
		}
		else
		{
			minx = Pre_emailpos[cur_pos*4]+offsetx;
			miny = Pre_emailpos[cur_pos*4+1]+offsety;
			maxx = Pre_emailpos[cur_pos*4+2]+offsetx;
			maxy = Pre_emailpos[cur_pos*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
			menu_email_enter();
	}
}

void Mouse_Email_Move_Func(U16 x,U16 y)
{
	U8 i;
	U16 minx,miny,maxx,maxy;
	if (inputmode)
	{
		for (i=0;i<KeyBorada[keyshift];i++)
		{
			if (MseType)
			{
				minx = Menu_Button_Pos[keyshift][i*2]+offsetx_hd;
				miny = Menu_Button_Pos[keyshift][i*2+1]+offsety_hd;
			}
			else
			{
				minx = Menu_Button_Pos[keyshift][i*2]+offsetx;
				miny = Menu_Button_Pos[keyshift][i*2+1]+offsety;
			}
			if (keyshift<2)
			{
				if (i==KeyBorada[keyshift]-1)
					maxx = minx+56;
				else
					maxx = minx+28;
			}
			else
			{
				if ((i==KeyBorada[keyshift]-1) || (i==16))
					maxx = minx+56;
				else if (i==KeyBorada[keyshift]-2)
					maxx = minx+84;
				else
					maxx = minx+28;
			}
			maxy =miny+28;
			if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (keypos != i))
			{
				email_keyboard_disp(keypos,WHITE);
				keypos=i;
				email_keyboard_disp(keypos,RED);
				break;
			}
		}
	}
	else
	{
		for (i=0;i<EMAILDATA;i++)
		{
			if (MseType)
			{
				minx = Pre_emailpos[i*4]+offsetx_hd;
				miny = Pre_emailpos[i*4+1]+offsety_hd;
				maxx = Pre_emailpos[i*4+2]+offsetx_hd;
				maxy = Pre_emailpos[i*4+3]+offsety_hd;
			}
			else
			{
				minx = Pre_emailpos[i*4]+offsetx;
				miny = Pre_emailpos[i*4+1]+offsety;
				maxx = Pre_emailpos[i*4+2]+offsetx;
				maxy = Pre_emailpos[i*4+3]+offsety;
			}
			if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != i))
			{
				menu_email_data_disp(cur_pos,WHITE);
				cur_pos=i;
				menu_email_data_disp(cur_pos,RED);
				break;
			}
		}
	}
}

void mEmail_key_func()
{
	if(byKey_val==0) return;

	switch(byKey_val)
	{
	case kESC:	//menu/exit   full/zoom      
		menu_email_off();
		break;
	case kUP:	   //PIP        up
		menu_email_up();
   		break;
	case kDN:        //4SP        down
		menu_email_down();
		break;
	case kLT:
	case kDE:

		break;
	case kRH:
	case kAD:

		break;
	case kET:
		menu_email_enter();
		break;
    }
	byKey_val=0;
}


