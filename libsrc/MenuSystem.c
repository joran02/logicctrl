#include	"cmn.h"

ValSystem SysMenu;
SYSTEMMENU systemmenupic;

U8 timebuf[7];
U8 satime[7];
U8 RemoteID;

#define SYSMAXMENU 19


 U16 tSys_data_pos[]=
{
	294,60,	354,60,	390,60,	438,60,	474,60,
	294,86,
	294,112,
	294,138,
	294,164,
	294,190,
	294,216,
	294,242,
	294,268,
	294,294,
	294,320,
	294,344,
	48,384,
	214,384,
	380,384
};


U8 Days_Mou[]=
{
	31,28,31,30,31,30,31,31,30,31,30,31
};

U16 Pre_syspos_ea[] = 
{
	294,60,		342,84,		
	354,60,		378,84,
	390,60,		414,84,
	438,60,		462,84,
	474,60,		498,84,
	
	294,86,		538,110,
	294,112,		538,136,
	294,138,		538,162,
	294,164,		538,188,
	294,190,		538,214,
	294,216,		538,240,
	294,242,		538,266,
	294,268,		538,292,
	294,294,		538,318,
	294,320,		538,342,
	294,344,		538,368,
	48,384,		206,414,
	214,384,		372,414,
	380,384,		538,414
};

U16 Pre_syspos_am_eu[]=
{
	294,60,		318,84,		
	330,60,		354,84,
	366,60,		414,84,
	438,60,		462,84,
	474,60,		498,84,
	
	294,86,		538,110,
	294,112,		538,136,
	294,138,		538,162,
	294,164,		538,188,
	294,190,		538,214,
	294,216,		538,240,
	294,242,		538,266,
	294,268,		538,292,
	294,294,		538,318,
	294,320,		538,342,
	294,344,		538,368,
	48,384,		206,414,
	214,384,		372,414,
	380,384,		538,414
};

U16 *Pre_syspos[] = 
{
	Pre_syspos_ea,
	Pre_syspos_am_eu,
	Pre_syspos_am_eu
};

void MenuSysInit()
{
	{
		  unsigned char by_tmp;
			IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
			IPCAM_SYS_ST *pSys_st=&pAllInfo->sys_st;
			
			if(pSys_st->ipcam_stardard==0)//TD_DRV_VIDEO_STARDARD_PAL)
			{
				by_tmp=1;
				I_write_block_eeprom(ADDSYS+VIDEOFORMAT,1,&by_tmp);
			}
			else if(pSys_st->ipcam_stardard==1)//TD_DRV_VIDEO_STARDARD_NTSC)
			{				
				by_tmp=0;
				I_write_block_eeprom(ADDSYS+VIDEOFORMAT,1,&by_tmp);
			}
	}
	I_read_block_eeprom(ADDSYS+TIMFORMAT,sizeof(SysMenu),&SysMenu.tmregion);
	I_read_block_eeprom(ADDSYS+0x0f,1,&SystemLock);
	I_read_block_eeprom(ADDSYS+0x0e,1,&curuser);
	MseType = SysMenu.MouseDisp;
	

}

void menu_system_data_disp(U8 pos,U16 color)
{
}

void hdmenu_system_data_disp(U8 pos,U16 color)
{
}

void Time_Format_Disp()
{
	char Osdtimebuf[20];
	memset(Osdtimebuf,0x00,sizeof(Osdtimebuf));
	if(SysMenu.tmregion ==0)
		sprintf(Osdtimebuf,"%04d/%02d/%02d  %02d:%02d",timebuf[ADDRYEAR]+2000,timebuf[ADDRMONTH],timebuf[ADDRDAY],timebuf[ADDRHOUR],timebuf[ADDRMINUTE]);
	else if (SysMenu.tmregion ==1)
		sprintf(Osdtimebuf,"%02d/%02d/%04d  %02d:%02d",timebuf[ADDRMONTH],timebuf[ADDRDAY],timebuf[ADDRYEAR]+2000,timebuf[ADDRHOUR],timebuf[ADDRMINUTE]);
	else if(SysMenu.tmregion ==2)
		sprintf(Osdtimebuf,"%02d/%02d/%04d  %02d:%02d",timebuf[ADDRDAY],timebuf[ADDRMONTH],timebuf[ADDRYEAR]+2000,timebuf[ADDRHOUR],timebuf[ADDRMINUTE]);
}

void menu_system_on()
{
	int ret;
	
		if(g_rtc_fd>0)
		{
			ret = ioctl(g_rtc_fd, HI_RTC_RD_TIME, &hi_tm);
			if(ret<0)
			{
				printf("ioctl: HI_RTC_RD_TIME failed\n");
				hi_tm.weekday=0;
			}
		}
}

void time_add_com(U8* value,U8 max_val,U8 min_val)
{
	(*value)++;
	if((*value)>max_val) (*value)=min_val;
}

void time_dec_com(U8* value,U8 max_val,U8 min_val)
{
	if((*value)==min_val)
		(*value)=max_val;
	else
		(*value)--;
}

void setsystem(U8* buf)
{

	struct timeval settime;
	struct tm systime;
	struct timezone tz;
	int ret;

	tz.tz_dsttime=0;
	tz.tz_minuteswest=0;

	systime.tm_sec=buf[0];
	systime.tm_min=buf[1];
	systime.tm_hour=buf[2];
	systime.tm_mday=buf[4];
	systime.tm_mon=buf[5]-1;
	systime.tm_year=buf[6]+100;

	settime.tv_sec=mktime(&systime);
	settime.tv_usec=0; 
	//	
	ret= settimeofday(&settime,&tz);
	if (ret<0)
	{
		printf("Time Initial error!\n");
	}
	else
		sys_command("hwclock -w");

}

void menu_system_do()
{
	U8 tm,tm1;
  int i,ret;
  
	SysMenu.RemoteID = RemoteID;

	if(g_rtc_fd>0)
	{
		for(i=0;i<3;i++)
		{
			hi_tm.year=2000+timebuf[6];
			hi_tm.month=timebuf[5];
			hi_tm.date=timebuf[4]; 
			hi_tm.hour=timebuf[2];
			hi_tm.minute=timebuf[1];
			hi_tm.second=timebuf[0];
			ret = ioctl(g_rtc_fd, HI_RTC_SET_TIME, &hi_tm);
			if(ret<0)
					printf("ioctl: HI_RTC_SET_TIME failed\n");
			usleep(5000);
		}
	}
	I_read_byte_eeprom(ADDSYS+VIDEOFORMAT, &tm);
	I_read_byte_eeprom(ADDSYS+HDOUTPUT, &tm1);
	I_write_block_eeprom(ADDSYS+TIMFORMAT,sizeof(SysMenu),&SysMenu.tmregion);
	if (SysMenu.RemoteID == 1)
		LedControl(LEDIR,1);
	else
		LedControl(LEDIR,0);
	remote_state_save();
//	pstCommonParam->gst_ext_write_rtc(0,ADDDISPMODE,1,&SysMenu.HDOutput);
//	pstCommonParam->gst_ext_write_rtc(0,ADDVIDEO,1,&SysMenu.VideoFormat);
	MseType = SysMenu.MouseDisp;
	SDDisplayTimechange();
	if ((tm != SysMenu.VideoFormat) || (tm1 != SysMenu.HDOutput))
	{
		printf("********* system reset!\n");
		bReset = 1;
		return;
	}

}

void menu_system_add()
{
	 U8 temp,i;
	switch(cur_pos)
	{
	case 0:  
		
		if(SysMenu.tmregion ==0)    //asia year
		{
			time_add_com(&timebuf[ADDRYEAR],MAXYEAR,00);
		}
		else if(SysMenu.tmregion ==1)  //usa month
		{
			if (timebuf[ADDRMONTH] < 12)
			{
				if (timebuf[ADDRDAY]>Days_Mou[timebuf[ADDRMONTH]])
				{
					timebuf[ADDRDAY] = Days_Mou[timebuf[ADDRMONTH]];
				}
			}
			time_add_com(&timebuf[ADDRMONTH],MAXMONTH,1);
		}
		else if(SysMenu.tmregion ==2)  //euro day
		{
			if (timebuf[ADDRMONTH]==2)
			{
				if ((timebuf[ADDRYEAR]%4) != 0)
					temp=28;
				else 
					temp=29;
				time_add_com(&timebuf[ADDRDAY],temp,1);
			}
			else
				time_add_com(&timebuf[ADDRDAY],Days_Mou[timebuf[ADDRMONTH]-1],1);
		}
		break;
	case 1:   
		if(SysMenu.tmregion ==0)     //asia month        
		{
			if (timebuf[ADDRMONTH] < 12)
			{
				if (timebuf[ADDRDAY]>Days_Mou[timebuf[ADDRMONTH]])
				{
					timebuf[ADDRDAY] = Days_Mou[timebuf[ADDRMONTH]];
					temp = (timebuf[ADDRDAY]/10)*16 + timebuf[ADDRDAY]%10;
				}
			}
			time_add_com(&timebuf[ADDRMONTH],MAXMONTH,1);
		}
		else if(SysMenu.tmregion ==1)  //usa day
		{
			if (timebuf[ADDRMONTH]==2)
			{
				if ((timebuf[ADDRYEAR]%4) != 0)
					temp=28;
				else 
					temp=29;
				time_add_com(&timebuf[ADDRDAY],temp,1);
			}
			else
				time_add_com(&timebuf[ADDRDAY],Days_Mou[timebuf[ADDRMONTH]-1],1);
		}
		else if(SysMenu.tmregion ==2)   //euro month
		{
			if (timebuf[ADDRMONTH] < 12)
			{
				if (timebuf[ADDRDAY]>Days_Mou[timebuf[ADDRMONTH]])
				{
					timebuf[ADDRDAY] = Days_Mou[timebuf[ADDRMONTH]];
					temp = (timebuf[ADDRDAY]/10)*16 + timebuf[ADDRDAY]%10;
				}
			}
			time_add_com(&timebuf[ADDRMONTH],MAXMONTH,1);
		}
		break;
	case 2:   
		if(SysMenu.tmregion ==0)  //asia day
		{
			if (timebuf[ADDRMONTH]==2)
			{
				if ((timebuf[ADDRYEAR]%4) != 0)
					temp=28;
				else 
					temp=29;
				time_add_com(&timebuf[ADDRDAY],temp,1);
			}
			else
				time_add_com(&timebuf[ADDRDAY],Days_Mou[timebuf[ADDRMONTH]-1],1);
		}
		else if(SysMenu.tmregion ==1)  //usa year
		{
			time_add_com(&timebuf[ADDRYEAR],MAXYEAR,00);
		}
		else if(SysMenu.tmregion ==2)   //euro year
		{
			time_add_com(&timebuf[ADDRYEAR],MAXYEAR,00);
		}
		break;
	case 3:   //hour
		time_add_com(&timebuf[ADDRHOUR],MAXHOUR,0);
		break;
	case 4:   //minute
		time_add_com(&timebuf[ADDRMINUTE],MAXMINUTE,0);
		break;
	case 5:   //date format
		SysMenu.tmregion++;
		if(SysMenu.tmregion>2) SysMenu.tmregion=0;	
		Time_Format_Disp();
		break;
	case 6:   //live time disp
		SysMenu.tmdisp++;
		if(SysMenu.tmdisp>2) SysMenu.tmdisp=0;
		break;
	case 7:    //border disp
		SysMenu.bordcolor++;
		if(SysMenu.bordcolor>3) SysMenu.bordcolor=0;
		break;
	case 8:    //N or P
		if (SysMenu.VideoFormat)
			SysMenu.VideoFormat = 0;
		else
			SysMenu.VideoFormat =1;
		break;
	case 9:
		if (RemoteID>99)
			RemoteID = 0;
		else
			RemoteID++;
		break;
	case 10:
		if (SysMenu.spotpop)
			SysMenu.spotpop =0;
		else
			SysMenu.spotpop =1;
		break;
	case 11:
		if (SysMenu.mainpop)
			SysMenu.mainpop =0;
		else
			SysMenu.mainpop =1;
		break;
	case 12:
		if (SysMenu.osdtran>=6)
			SysMenu.osdtran = 0;
		else
			SysMenu.osdtran++;
		set_osdAlpha(SysMenu.osdtran,AD);
		set_osdAlpha(SysMenu.osdtran,HD);
		break;
	case 13:
		if (SysMenu.HDOutput>=3)
			SysMenu.HDOutput = 0;
		else
			SysMenu.HDOutput++;
		if (SysMenu.HDOutput==1)
			SysMenu.HDOutput++;
		if (ChipSelect == HI3515 && SysMenu.HDOutput>=1)
			SysMenu.HDOutput = 0;
		break;
	case 14:
		if (SysMenu.MouseDisp)
			SysMenu.MouseDisp = 0;
		else
			SysMenu.MouseDisp = 1;
		break;
	case 15:
		temp = SysMenu.Laguage;
		if (SysMenu.Laguage)
			SysMenu.Laguage = 0;
		else
			SysMenu.Laguage  = 1;
//		Check_Language();
		if (temp != SysMenu.Laguage)
		{
			Menu_Load();
			menu_system_on();
		}
		break;
	}
	if (cur_pos<3)
	{
		for (i=0;i<3;i++)
		{
			if (i!=cur_pos)
			{
				menu_system_data_disp(cur_pos,WHITE);
				hdmenu_system_data_disp(cur_pos,WHITE);
			}
		}
	}
	menu_system_data_disp(cur_pos,RED);
	hdmenu_system_data_disp(cur_pos,RED);
}
void menu_system_dec()
{
	 U8 temp,i;
	switch(cur_pos)
	{
	case 0:   
		if(SysMenu.tmregion ==0)    //asia year
		{
			time_dec_com(&timebuf[ADDRYEAR],MAXYEAR,00);
		}
		else if(SysMenu.tmregion ==1)  //usa month
		{
			if (timebuf[ADDRMONTH] > 1)
			{
				if (timebuf[ADDRDAY]>Days_Mou[timebuf[ADDRMONTH]-2])
				{
					timebuf[ADDRDAY] = Days_Mou[timebuf[ADDRMONTH]-2];
					temp = (timebuf[ADDRDAY]/10)*16 + timebuf[ADDRDAY]%10;
				}
			}
			time_dec_com(&timebuf[ADDRMONTH],MAXMONTH,1);
		}
		else if(SysMenu.tmregion ==2)  //euro day
		{
			if (timebuf[ADDRMONTH]==2)
			{
				if ((timebuf[ADDRYEAR]%4) != 0)
					temp=28;
				else 
					temp=29;
				time_dec_com(&timebuf[ADDRDAY],temp,1);
			}
			else
				time_dec_com(&timebuf[ADDRDAY],Days_Mou[timebuf[ADDRMONTH]-1],1);
		}
		break;
	case 1:   
		if(SysMenu.tmregion ==0)     //asia month        
		{
			if (timebuf[ADDRMONTH] > 1)
			{
				if (timebuf[ADDRDAY]>Days_Mou[timebuf[ADDRMONTH]-2])
				{
					timebuf[ADDRDAY] = Days_Mou[timebuf[ADDRMONTH]-2];
					temp = (timebuf[ADDRDAY]/10)*16 + timebuf[ADDRDAY]%10;
				}
			}
			time_dec_com(&timebuf[ADDRMONTH],MAXMONTH,1);
		}
		else if(SysMenu.tmregion ==1)  //usa day
		{
			if (timebuf[ADDRMONTH]==2)
			{
				if ((timebuf[ADDRYEAR]%4) != 0)
					temp=28;
				else 
					temp=29;
				time_dec_com(&timebuf[ADDRDAY],temp,1);
			}
			else
				time_dec_com(&timebuf[ADDRDAY],Days_Mou[timebuf[ADDRMONTH]-1],1);
		}
		else if(SysMenu.tmregion ==2)   //euro month
		{
			if (timebuf[ADDRMONTH] > 1)
			{
				if (timebuf[ADDRDAY]>Days_Mou[timebuf[ADDRMONTH]-2])
				{
					timebuf[ADDRDAY] = Days_Mou[timebuf[ADDRMONTH]-2];
					temp = (timebuf[ADDRDAY]/10)*16 + timebuf[ADDRDAY]%10;
				}
			}
			time_dec_com(&timebuf[ADDRMONTH],MAXMONTH,1);
		}
		break;
	case 2:  
		if(SysMenu.tmregion ==0)    //asia day
		{
			if (timebuf[ADDRMONTH]==2)
			{
				if ((timebuf[ADDRYEAR]%4) != 0)
					temp=28;
				else 
					temp=29;
				time_dec_com(&timebuf[ADDRDAY],temp,1);
			}
			else
				time_dec_com(&timebuf[ADDRDAY],Days_Mou[timebuf[ADDRMONTH]-1],1);
		}
		else if(SysMenu.tmregion ==1)  //usa year
		{
			time_dec_com(&timebuf[ADDRYEAR],MAXYEAR,0);
		}
		else if(SysMenu.tmregion ==2)   //euro year
		{
			time_dec_com(&timebuf[ADDRYEAR],MAXYEAR,0);
		}
		break;
	case 3:     //hour
		time_dec_com(&timebuf[ADDRHOUR],MAXHOUR,00);
		break;
	case 4:   //mintue
		time_dec_com(&timebuf[ADDRMINUTE],MAXMINUTE,00);
		break;
	case 5:   //date format
		if(SysMenu.tmregion==0) SysMenu.tmregion=2;
		else SysMenu.tmregion--;
		Time_Format_Disp();
		break;
	case 6:   //lv time disp		
		if(SysMenu.tmdisp==0) SysMenu.tmdisp=2;
		else SysMenu.tmdisp--;
		break;
	case 7:   //border disp
		if(SysMenu.bordcolor==0) SysMenu.bordcolor=3;
		else SysMenu.bordcolor--;
		break;
	case 8:    //N or P
		if (SysMenu.VideoFormat)
			SysMenu.VideoFormat = 0;
		else
			SysMenu.VideoFormat =1;
		break;
	case 9:
		if (RemoteID==0)
			RemoteID = 100;
		else
			RemoteID--;
		break;
	case 10:
		if (SysMenu.spotpop)
			SysMenu.spotpop =0;
		else
			SysMenu.spotpop =1;
		break;
	case 11:
		if (SysMenu.mainpop)
			SysMenu.mainpop =0;
		else
			SysMenu.mainpop =1;
		break;
	case 12:
		if (SysMenu.osdtran==0)
			SysMenu.osdtran = 6;
		else
			SysMenu.osdtran--;
		set_osdAlpha(SysMenu.osdtran,AD);
		set_osdAlpha(SysMenu.osdtran,HD);
		break;	
	case 13:
		if (SysMenu.HDOutput==0)
			SysMenu.HDOutput = 3;
		else
			SysMenu.HDOutput--;
		if (SysMenu.HDOutput==1)
			SysMenu.HDOutput--;
		if (ChipSelect == HI3515 && SysMenu.HDOutput>=1)
			SysMenu.HDOutput = 0;
		break;
	case 14:
		if (SysMenu.MouseDisp)
			SysMenu.MouseDisp = 0;
		else
			SysMenu.MouseDisp = 1;
		break;
	case 15:
		temp = SysMenu.Laguage;
		if (SysMenu.Laguage)
			SysMenu.Laguage = 0;
		else
			SysMenu.Laguage  = 1;
		
//		Check_Language();
		if (temp != SysMenu.Laguage)
		{
			Menu_Load();
			menu_system_on();
		}
		break;
	default:
		break;
	}
	if (cur_pos<3)
	{
		for (i=0;i<3;i++)
		{
			if (i!=cur_pos)
			{
				menu_system_data_disp(cur_pos,WHITE);
				hdmenu_system_data_disp(cur_pos,WHITE);
			}
		}
	}
	menu_system_data_disp(cur_pos,RED);
	hdmenu_system_data_disp(cur_pos,RED);
}
void menu_system_down()
{
	menu_system_data_disp(cur_pos,WHITE);
	hdmenu_system_data_disp(cur_pos,WHITE);
	if(cur_pos==SYSMAXMENU-1)cur_pos=0;
	else cur_pos++;
	menu_system_data_disp(cur_pos,RED);
	hdmenu_system_data_disp(cur_pos,RED);
}
void menu_system_up()
{
	menu_system_data_disp(cur_pos,WHITE);
	hdmenu_system_data_disp(cur_pos,WHITE);
	if(cur_pos==0) cur_pos=SYSMAXMENU-1;
	else cur_pos--;
	menu_system_data_disp(cur_pos,RED);
	hdmenu_system_data_disp(cur_pos,RED);
}

void menu_system_off()
{
	U8 tmp = SysMenu.Laguage;
	U8 tmp1 = SysMenu.VideoFormat;
	MenuSysInit();
	if (tmp != SysMenu.Laguage || tmp1 != SysMenu.VideoFormat)
	{
		Menu_Load();
	}
	set_osdAlpha(SysMenu.osdtran,AD);
	set_osdAlpha(SysMenu.osdtran,HD);
    	menu_main_on();
}

U8 checktimechange()
{
	U8 i;
	U8 tm;
	I_read_byte_eeprom(ADDSYS+VIDEOFORMAT, &tm);

	if (tm != SysMenu.VideoFormat)
	{
		SysMenu.VideoFormat = tm;
		return 1;
	}
	for(i=0;i<3;i++)
	{
		if ((timebuf[i] !=satime[i]) || (timebuf[i+4] !=satime[i+4]))
		{
			return 1;
		}
	}
	return 0;
}

void checkrel()
{
	U8 ltm;
	U8 tm = SysMenu.Laguage;
	I_read_byte_eeprom(ADDSYS+LAGUAGE, &ltm);
	MenuSysInit();
	if (ltm != tm)
		Menu_Load();

}

void menu_system_enter()
{
	switch(cur_pos)
	{
		case 16:
			if (rec_on)
			{
				checkrel();
				InfoId = RecErr;
				OSD_ALL_CLEAR();
				MenuErrOn();
			}
			else
			{
				InfoId = Loaddefault;
				OSD_ALL_CLEAR();
				MenuErrOn();
			}
			break;
		case 17:
			if (rec_on && checktimechange())
			{
				checkrel();
				InfoId = RecErr;
				OSD_ALL_CLEAR();
				MenuErrOn();
				break;
			}
			else
				menu_system_do();
//			break;
		case 18:
			menu_system_off();
		default:
			break;
	}
}

void MouseLClick(U16 x,U16 y)
{
	U16 ypos1;
	U16 ypos2;
	if (MseType == 0)
	{
		ypos1 = Pre_syspos[SysMenu.tmregion][cur_pos*4+1] + offsety;
		ypos2 = Pre_syspos[SysMenu.tmregion][cur_pos*4+3] + offsety;
		if ((x>=Pre_syspos[SysMenu.tmregion][cur_pos*4]+offsetx) && (y>=ypos1) && (x<=Pre_syspos[SysMenu.tmregion][cur_pos*4+2]+offsetx) && (y<=ypos2))
		{
			if (cur_pos <16)
				menu_system_dec(); 
			else
				menu_system_enter();
		}
	}
	else
	{
		ypos1 = Pre_syspos[SysMenu.tmregion][cur_pos*4+1] + offsety_hd;
		ypos2 = Pre_syspos[SysMenu.tmregion][cur_pos*4+3] + offsety_hd;
		if ((x>=Pre_syspos[SysMenu.tmregion][cur_pos*4]+offsetx_hd) && (y>=ypos1) && (x<=Pre_syspos[SysMenu.tmregion][cur_pos*4+2]+offsetx_hd) && (y<=ypos2))
		{
			if (cur_pos <16)
				menu_system_dec(); 
			else
				menu_system_enter();
		}
	}
}

void MouseRClick(U16 x,U16 y)
{
	U16 ypos1;
	U16 ypos2;
	if (MseType == 0)
	{
		ypos1 = Pre_syspos[SysMenu.tmregion][cur_pos*4+1] + offsety;
		ypos2 = Pre_syspos[SysMenu.tmregion][cur_pos*4+3] + offsety;
		if ((x>=Pre_syspos[SysMenu.tmregion][cur_pos*4]+offsetx) && (y>=ypos1) && (x<=Pre_syspos[SysMenu.tmregion][cur_pos*4+2]+offsetx) && (y<=ypos2))
			menu_system_add(); 
	}
	else
	{
		ypos1 = Pre_syspos[SysMenu.tmregion][cur_pos*4+1] + offsety_hd;
		ypos2 = Pre_syspos[SysMenu.tmregion][cur_pos*4+3] + offsety_hd;
		if ((x>=Pre_syspos[SysMenu.tmregion][cur_pos*4]+offsetx_hd) && (y>=ypos1) && (x<=Pre_syspos[SysMenu.tmregion][cur_pos*4+2]+offsetx_hd) && (y<=ypos2))
			menu_system_add(); 
	}
}

void Mouse_Sys_Move_Func(U16 x,U16 y)
{
	U8 i;
	U16 ypos1;
	U16 ypos2;
	if (MseType == 0)
	{
		for (i=0;i<SYSMAXMENU;i++)
		{
			ypos1 = Pre_syspos[SysMenu.tmregion][i*4+1]+offsety;
			ypos2 = Pre_syspos[SysMenu.tmregion][i*4+3]+offsety;
			if ((x>=Pre_syspos[SysMenu.tmregion][i*4]+offsetx) && (y>=ypos1) && (x<=Pre_syspos[SysMenu.tmregion][i*4+2]+offsetx) && (y<=ypos2) && (cur_pos != i))
			{
				hdmenu_system_data_disp(cur_pos,WHITE);
				menu_system_data_disp(cur_pos,WHITE);
				cur_pos=i;
				hdmenu_system_data_disp(cur_pos,RED);
				menu_system_data_disp(cur_pos,RED);
				break;
			}
		}
	}
	else
	{
		for (i=0;i<SYSMAXMENU;i++)
		{
			ypos1 = Pre_syspos[SysMenu.tmregion][i*4+1]+offsety_hd;
			ypos2 = Pre_syspos[SysMenu.tmregion][i*4+3]+offsety_hd;
			if ((x>=Pre_syspos[SysMenu.tmregion][i*4]+offsetx_hd) && (y>=ypos1) && (x<=Pre_syspos[SysMenu.tmregion][i*4+2]+offsetx_hd) && (y<=ypos2) && (cur_pos != i))
			{
				menu_system_data_disp(cur_pos,WHITE);
				hdmenu_system_data_disp(cur_pos,WHITE);
				cur_pos=i;
				menu_system_data_disp(cur_pos,RED);
				hdmenu_system_data_disp(cur_pos,RED);
				break;
			}
		}
	}
}


//******************************key func*************************************
//int tstpos;
void mSYSTEM_key_func()
{
	if(byKey_val==0 || bReset) return;

	printf("mSYSTEM_key_func\n");
	switch(byKey_val)
	{
		case kESC:	//menu/exit   full/zoom      
			menu_system_off();
			break;
		case kUP:	   //PIP        up
			menu_system_up();
	   		break;
		case kDN:        //4SP        down
			menu_system_down();
			break;
		case kRH:	    //16sp/pip/8sp/13sp     right
		case kAD:
			menu_system_add(); 
			break;
		case kLT:	    //9sp          left
		case kDE:
			menu_system_dec();
			break;
		case kET:
			menu_system_enter();
			break;
		default:
			break;
    }
	byKey_val=0;
}

