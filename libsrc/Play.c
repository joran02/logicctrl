#include "cmn.h"

ValPlay PlayStatusFlag;
//PlayStatus : 0:live, 1:play, 2:pause, 3:fast, 4:back
U8 Play_Ch;
U8 Mouse_PlaySta;

U8* Play_Menu;
U8* playblock;
//U8* Play_Menu_Color;
U8 Play_Mode;
int Playstatus=0xff;

int play_quad_flag=0;

extern void rec_frm_set_tmp(int m);

U8 PlayStatus_Pos_PAL[]=
{
	20,25
};

U8 PlayStatus_Pos_NTSC[]=
{
	20,21
};

U8* PlayStatus_Pos[]=
{
	PlayStatus_Pos_NTSC,
	PlayStatus_Pos_PAL
};

void PlayStatusShow()
{
	int ret, flag, status;
	U8 Ypos;
	U8 Play_Status_Old=0;

	if (demo_num < PBFULL)
		return;
	ret = gstCommonParam->GST_FTC_GetPlayCurrentStauts(&status,&flag);
	ret = gstCommonParam->GST_FTC_get_play_status();
//	printf("Playstatus=%x\n",Playstatus);
//	printf("**********************status=%x,flag:%x,ret:%x\n",status,flag,ret);
//	printf("ret=%x\n",ret);
	if (Mouse_PlaySta != 10)
		Mouse_PlaySta = status;
	if (ret)
	{
		if (Playstatus != status)
		{
			Play_Status_Old = Playstatus;
			Playstatus = status;
		}
		else
			return;
	}
	else
	{
//		printf("***********************************************************************\n");
		if (Playstatus == 0xff)
			return;
		Playstatus = 0xff;
		status = 10;
	}
	Mouse_PlaySta = status;
	if (status ==PLAYLATERFRAME || status ==PLAYLASTFRAME || (status == PLAYPAUSE && (Play_Status_Old ==PLAYLATERFRAME || Play_Status_Old ==PLAYLASTFRAME)))
	{
		return;
	}
	printf("status=%x,Play_Status_Old=%x\n",status,Play_Status_Old);
	
//	InitOSDCol("/mnt/mtd/yuvfile/Play_Menu_Data/Play_Menu_Color.yuv");
	if (!SysMenu.VideoFormat)
		Ypos=0x40;
	else
		Ypos =0;
//	printf("Mouse_PlaySta = %d\n",Mouse_PlaySta);
//	Mouse_PlaySta = status;
//	printf("Mouse_PlaySta = %d\n",Mouse_PlaySta);
	switch(status)
	{
		case PLAYNORMAL:
		case PLAYFASTLOWSPEED:
		case PLAYFASTHIGHSPEED:
		case PLAYBACKFASTLOWSPEED:
		case PLAYBACKFASTHIGHSPEED:
		case PLAYPAUSE:
		case PLAYLATERFRAME:
		case PLAYLASTFRAME:
		case PLAYFASTHIGHSPEED_2:
		case PLAYFASTHIGHSPEED_4:
		case PLAYFASTHIGHSPEED_8:
		case PLAYFASTHIGHSPEED_16:
		case PLAYFASTHIGHSPEED_32:
		case PLAYBACKFASTHIGHSPEED_2:
		case PLAYBACKFASTHIGHSPEED_4:
		case PLAYBACKFASTHIGHSPEED_8:
		case PLAYBACKFASTHIGHSPEED_16:
		case PLAYBACKFASTHIGHSPEED_32:
		case 10:	//Stop
			break;
/*			
			bxs_pic = (BXS_PIC1*)Play_Menu;
			Bmp_Part_Show(300, High_Resolution-MouseMenuHight*3, 0, (PLAYLASTFRAME+1)*bxs_pic->pic_height/9, bxs_pic->pic_width, bxs_pic->pic_height/9, Play_Menu);
			break;
*/
		case 99:
//			WriteOSDBmpX_bmp(0x40,0xff-Ypos,WHITE,PlaySearch,Play_Menu);
			break;
	}

}

void Play_Ch_Show()
{
	Playstatus=0xff;
}

void set_play_cam(U8 flag,U8 mode)
{
	U8 i;
	printf("###################\n");
	memset(Play_Cam,0x00,16);
	for (i=0;i<mode;i++)
	{
		Play_Cam[i] = i+flag;
	}
}

void Play_On()
{
	demo_num = PB9;
	LedControl(LEDPLAY,1);
	PlayStatusFlag.PlayFlag=0;
	PlayStatusFlag.PlayStatus=PLAY;
	Play_Ch_Show();
	printf("Play Start!\n");
}

void Split_Play(U8 mode,U8 timemode,time_t starttime)
{
	int ret=0;
	time_t start_time2;
	struct timeval nowTime;
	U16 tmp=0;
	U8 i,tmmode,j=0;
//	if (Play_Mode == mode)
//		return;

//	gstCommonParam->GST_FTC_StopPlay();
	switch (mode)
	{
		case 100:
		case 101:
		case 102:
		case 103:
		case 104:
		case 105:
		case 106:
		case 107:
		case 108:
		case 109:
		case 110:
		case 111:
		case 112:
		case 113:
		case 114:
		case 115:
			set_play_cam(mode-100,1);
			break;
		case 40:
			set_play_cam(0,4);
			break;
		case 41:
			if (Cam_Num>CHANNEL4)
				set_play_cam(4,4);
			break;
		case 42:
			if (Cam_Num>CHANNEL8)
				set_play_cam(8,4);
			break;
		case 43:
			if (Cam_Num>CHANNEL8)
				set_play_cam(12,4);
			break;
		case 45:
			for (i=0;i<4;i++)
			{
				tmp |= WBIT(Play_Cam[i]);
			}
			if (Cam_Num==CHANNEL8)
				tmp = 0xff-tmp;
			else if (Cam_Num==CHANNEL16)
				tmp = 0xffff-tmp;
			for (i=0;i<Cam_Num;i++)
			{
				if (WBIT(i) & tmp )
				{
					Play_Cam[j++] = i;
				}
			}
			Play_Mode = 41;
			break;
		case 46:
			j=0;
			Play_Mode = 100+Play_Cam[0];
			for (i=1;i<4;i++)
			{
				if (Play_Cam[i])
					j++;
			}
			if (j)
				Play_Mode = 45;
			break;
		case 80:
			if (Cam_Num>CHANNEL8)
				set_play_cam(0,9);
			else
				set_play_cam(0,8);
			break;
		case 81:
			if (Cam_Num>CHANNEL8)
				set_play_cam(7,9);
			else
				set_play_cam(8,8);
			break;
		case 16:
			set_play_cam(0,16);
			break;
		default:
			break;
	}
	OSD_ALL_CLEAR();

	if (mode == 16)
		tmmode = Play_Mode;
	else
	{
		tmmode = Play_Mode/100;
		if (!tmmode)
			tmmode = Play_Mode/10;
	}
	gstCommonParam->GST_FTC_StopPlay();
	if (tmmode == 8)
	{
		gstCommonParam->GST_DRA_Hisi_playback_window_vo(9,0);
		if (Cam_Num>CHANNEL8)
			tmmode = 9;
	}
	else
		gstCommonParam->GST_DRA_Hisi_playback_window_vo(tmmode,0);
	tmp = 0;
	if (tmmode>1)
	{
		for (i=0;i<tmmode;i++)
		{
			if (Play_Cam[i])
				tmp |= WBIT(Play_Cam[i]);
		}
		if (tmp && Play_Cam[0]==0)
			tmp |= 1;
	}
	else
		tmp |= WBIT(Play_Cam[0]);
//	printf("#####################timemode : %d,Play_Mode:%d,tmp:%x\n",timemode,Play_Mode,tmp);
	if (timemode == 0)
	{
		gettimeofday( &nowTime, NULL );
		ret=gstCommonParam->GST_FTC_PlayNearRecFile(&nowTime.tv_sec);
		start_time_Whole=nowTime.tv_sec;
	}
	else if (timemode == 1)
	{
		if (Mouse_PlaySta == 10)
			start_time2 = start_time_Whole;
		else
			gstCommonParam->GST_FTC_get_play_time(&start_time2);
#ifdef TW54M27M
		ret=gstCommonParam->GST_NET_is_have_client();
		if(Cam_Num==CHANNEL4&&RecamMenu[0].Reselution)
		{
			if(tmp==0x0f)
			{
				play_quad_flag=1;
				rec_frm_set_tmp(7);
			}
			else 
			{
				if(gstCommonParam->GST_NET_is_have_client()>0)	
					rec_frm_set_tmp(15);
				else
					rec_frm_set_tmp(0);
				play_quad_flag=0;
			}
		}
#endif
		ret = gstCommonParam->GST_FTC_SetTimeToPlay(start_time2,0,tmp,1);
	}
	else if (timemode == 2)
	{
#ifdef TW54M27M
		ret=gstCommonParam->GST_NET_is_have_client();
		if(Cam_Num==CHANNEL4&&RecamMenu[0].Reselution)
		{
			if(tmp==0x0f)
			{
				play_quad_flag=1;
				rec_frm_set_tmp(7);
			}
			else 
			{
				if(gstCommonParam->GST_NET_is_have_client()>0)	
					rec_frm_set_tmp(15);
				else
					rec_frm_set_tmp(0);
				play_quad_flag=0;
			}			
		}
#endif
		ret = gstCommonParam->GST_FTC_SetTimeToPlay(starttime,0,tmp,1);
	}
	if( ret < 0 )
	{
		InfoId=TimeErr;
		Play_Err_Stop();
		MenuErrOn();
		return;
	}
//	Play_Mode =mode;
	if (Play_Mode/100)
		tmmode = LFULL;
	else if ((Play_Mode/10)==4)
		tmmode = L4;
	else if ((Play_Mode/10)==8)
		tmmode = L8;
	else if (Play_Mode==16)
		tmmode = L16;
	PlayStatusFlag.PlayStatus = PLAY;
	LedControl(LEDPLAY,1);
	setliveaudio();
	border_set(tmmode);
	Play_Ch_Show();
	if ((MenuStatus == mMouse) || (MenuStatus == (mMouse+Iconmenu)))
		Menu_Mouse_On();
}

void PlayCamAdjust(U8 flag)
{
	if (flag==0)
	{
		if (Play_Ch==0)
			Play_Ch = Cam_Num-1;
		else
			Play_Ch --;
	}
	else 
	{
		if (Play_Ch == Cam_Num-1)
			Play_Ch = 0;
		else
			Play_Ch ++;
	}
	PlayCam(Play_Ch);
}

void Play_Start()
{
//	struct timeval nowTime;
//	U8 i,buf_pos[2];
//	int ret;
	if (CheckUser(menupass[curuser].Playcfig))
		return;
	Play_Mode = 100;
	demo_num = PB9;
	LedControl(LEDPLAY,1);
/*
	gettimeofday( &nowTime, NULL );

	gstCommonParam->GST_DRA_Hisi_playback_window_vo(1,0);
	ret=gstCommonParam->GST_FTC_PlayNearRecFile(&nowTime.tv_sec);
	Play_Ch = 0;
	if (ret>0)
		start_time_Whole=nowTime.tv_sec;
*/
	
	Split_Play(Play_Mode,0,0);
	OSD_ALL_CLEAR();
//	memset(Play_Cam,0x00,16);
	
	PlayStatusFlag.PlayFlag=0;
	PlayStatusFlag.PlayStatus=PLAY;

	setliveaudio();

	Play_Ch_Show();

}

void Play_Err_Inital()
{
	PlayStatusFlag.PlayFlag=0;
	PlayStatusFlag.PlayStatus=LIVE;
	PlayStatusFlag.PlayFSpeed =0;
	PlayStatusFlag.PlayBSpeed =0;
	Playstatus = 0xff;
}

void Play_Err_Stop()
{
	gstCommonParam->GST_FTC_StopPlay();
	sleep(1);
	PlayStatusFlag.PlayFlag=0;
	PlayStatusFlag.PlayStatus=LIVE;
	PlayStatusFlag.PlayFSpeed =0;
	PlayStatusFlag.PlayBSpeed =0;
	Playstatus = 0xff;
	LedControl(LEDPLAY,0);
	demo_num = DEMO_DISP;
	get_demo_stamp(demo_num,demo_page,0);
	live_demo_set();
	setliveaudio();
#ifdef TW54M27M
	if(Cam_Num==CHANNEL4&&RecamMenu[0].Reselution&&gstCommonParam->GST_NET_is_have_client()<0)
		rec_frm_set_tmp(0);
	play_quad_flag=0;
#endif
}

void Play_Stop()
{
//	printf("Play_Stop function********************!\n");
	gstCommonParam->GST_FTC_StopPlay();
	sleep(1);
	PlayStatusFlag.PlayFlag=0;
	PlayStatusFlag.PlayStatus=LIVE;
	PlayStatusFlag.PlayFSpeed =0;
	PlayStatusFlag.PlayBSpeed =0;
	demo_num = DEMO_DISP;
	Playstatus = 0xff;
	LedControl(LEDPLAY,0);
	get_demo_stamp(demo_num,0,0);
	OSD_ALL_CLEAR();
//	I_read_byte_eeprom(ADDSYS+TIMDISP,&SysMenu.tmdisp);
//	Set_Time_Pos();

	demo_set();

	demo_osd_disp();
	setliveaudio();
#ifdef TW54M27M
	if(Cam_Num==CHANNEL4&&RecamMenu[0].Reselution&&gstCommonParam->GST_NET_is_have_client()<0)
		rec_frm_set_tmp(0);
	play_quad_flag=0;
#endif
//	printf("#################################Play End!\n");
}

void Play_Pause()
{
	PlayStatusFlag.PlayFlag ^= 1;
	PlayStatusFlag.PlayBSpeed =0;
	PlayStatusFlag.PlayFSpeed =0;
	gstCommonParam->GST_FTC_PlayPause(PlayStatusFlag.PlayFlag);
	if (PlayStatusFlag.PlayFlag)
		PlayStatusFlag.PlayStatus=PAUSE;
	else
		PlayStatusFlag.PlayStatus=PLAY;
//	printf("PlayStatusFlag.PlayFlag = %x,PlayStatusFlag.PlayStatus = %x\n",PlayStatusFlag.PlayFlag,PlayStatusFlag.PlayStatus);
}

void Fram_Play(int flag)
{
	gstCommonParam->GST_FTC_PlaySingleFrame(flag);//  1 f,2,b
//	printf("GST_FTC_PlaySingleFrame : %x\n",flag);
	if (flag)
		PlayStatusFlag.PlayStatus=FRMPLAY;
	else 
		PlayStatusFlag.PlayStatus=PLAY;
}

void Play_Fast()
{
	int tmp1,tmp2;
	gstCommonParam->GST_FTC_GetPlayCurrentStauts(&tmp1,&tmp2);
//	printf("Fast *& Fast *& Fast &* Fast &* Fast &* Fast &*Play Status = %x\n",tmp1);
	PlayStatusFlag.PlayBSpeed =0;
	if (tmp1<PLAYPAUSE || tmp1>PLAYLASTFRAME)
	{
		PlayStatusFlag.PlayFlag = 0;
		if (PlayStatusFlag.PlayFSpeed>=7)
		{
			PlayStatusFlag.PlayFSpeed =0;
			PlayStatusFlag.PlayStatus=PLAY;
		}
		else if (PlayStatusFlag.PlayFSpeed == 2)
			PlayStatusFlag.PlayFSpeed = 4;
		else
		{
			PlayStatusFlag.PlayFSpeed++;
			PlayStatusFlag.PlayStatus=FAST;
		}
		gstCommonParam->GST_FTC_PlayFast(PlayStatusFlag.PlayFSpeed);
//		printf("GST_FTC_PlayFast : %x\n",PlayStatusFlag.PlayFSpeed);
	}
	else
		Fram_Play(1);
}

void Play_Back()
{
	int tmp1,tmp2;
	gstCommonParam->GST_FTC_GetPlayCurrentStauts(&tmp1,&tmp2);
//	printf("back***back***back***back***back***Play Status = %x\n",tmp1);
	PlayStatusFlag.PlayFSpeed =0;
	if (tmp1<PLAYPAUSE || tmp1>PLAYLASTFRAME)
	{
		PlayStatusFlag.PlayFlag = 0;
		if (PlayStatusFlag.PlayBSpeed>=7)
		{
			PlayStatusFlag.PlayBSpeed =0;
			PlayStatusFlag.PlayStatus=PLAY;
		}
		else if (PlayStatusFlag.PlayBSpeed==2)
			PlayStatusFlag.PlayBSpeed=4;
		else
		{
			PlayStatusFlag.PlayBSpeed++;
			PlayStatusFlag.PlayStatus=BACK;
		}
		gstCommonParam->GST_FTC_PlayBack(PlayStatusFlag.PlayBSpeed);
	}
	else
		Fram_Play(2);
}

int Rplay_Start()
{
//	struct timeval nowTime;
	int ret;
//	U8 i;
	ret = gstCommonParam->GST_FTC_get_play_status();
//	printf("Get play status is %x\n",ret);
	if (ret == 0)
	{
		Play_Stop();
/*
		PlayStatusFlag.PlayFlag = 0;
		PlayStatusFlag.PlayStatus=PLAY;
		gstCommonParam->GST_FTC_PlayPause(PlayStatusFlag.PlayFlag);
		gstCommonParam->GST_FTC_get_play_time(&start_time_Whole);
//		Split_Play(Play_Mode,1,0);
		gettimeofday( &nowTime, NULL );
//		printf("tv_sec : %d\n",nowTime.tv_sec);
		if (Play_Mode==1)
		{
			gstCommonParam->GST_FTC_PlayNearRecFile(&nowTime.tv_sec);
			Play_Ch = 0;
			memset(Play_Cam,0x00,16);
			Play_Cam[0] = 0;
		}
		else
		{
			
			gstCommonParam->GST_FTC_SetTimeToPlay(nowTime.tv_sec,0,WBIT(Play_Cam[0])|WBIT(Play_Cam[1]),1);
		}
		Play_Ch_Show();
*/
		return 1;
	}
//	printf("PlayStatusFlag.PlayStatus is %x\n",PlayStatusFlag.PlayStatus);
	switch (PlayStatusFlag.PlayStatus)
	{
		case FRMPLAY:
			Fram_Play(0);
			break;
		case PLAY:
		case PAUSE:
			Play_Pause();
			break;
		case FAST:
			PlayStatusFlag.PlayFSpeed=0;
			PlayStatusFlag.PlayStatus=PAUSE;
			gstCommonParam->GST_FTC_PlayFast(PlayStatusFlag.PlayFSpeed);
			break;
		case BACK:
			PlayStatusFlag.PlayBSpeed=0;
			PlayStatusFlag.PlayStatus=PAUSE;
			gstCommonParam->GST_FTC_PlayFast(PlayStatusFlag.PlayBSpeed);
			break;
		default:
			break;
	}
	return 0;
//	PlayStatusFlag.PlayStatus=PLAY;
}

U8 onesec;

void Play_Status_Show()
{
	if (++onesec>0x10)
	{
		PlayStatusShow();
		onesec = 0;
	}
}

void getorderch(U8* buf,U8 cn)
{
	U8 i,j=0;
	U16 tmp=0;

//	printf("playmode:%x\n",Play_Mode);
	for (i=0;i<Cam_Num;i++)
		tmp |= WBIT(Play_Cam[i]);
	for (i=0;i<Cam_Num;i++)
	{
		if (WBIT(i)&tmp)
		{
			*(buf+j) = i;
			j++;
		}
	}
}
U8 thflag=0;

void Mouse_Play_Func(U16 x, U16 y,U8 type)
{
	U16 minx,miny,maxx,maxy,mx,my;
	if (thflag)
		return;
	else 
		thflag = 1;
	switch(type)
	{
		case 0:
			if (MseType)
			{
				minx = 0;
				miny = HIGH_RESOLUTION_HD-MouseMenuHight*2;
				maxx = WIDTH_RESOLUTION_HD;
				maxy = HIGH_RESOLUTION_HD-MouseMenuHight;
			}
			else
			{
				minx = 0;
				miny = High_Resolution-MouseMenuHight*2;
				maxx = WIDTH_RESOLUTION;
				maxy = HIGH_RESOLUTION_HD-MouseMenuHight;
			}
			if((x > minx && y > miny) && (x < maxx && y < maxy))
			{
				if ((MenuStatus==mMouse) || (MenuStatus==(mMouse+Iconmenu)))
					menu_num = mMouse;
//				Menu_Mouse_On();
			}
			break;
		case 26:
			if ((Play_Mode/100) == 1)
			{
				if (Play_Cam[0])
					Play_Cam[0] --;
				else
					Play_Cam[0] = Cam_Num-1;
				PlayCam(Play_Cam[0]);
			}
			else if ((Play_Mode/10)==2)
			{
				if (MseType)
				{
					minx = WIDTH_RESOLUTION_HD/2;
					miny = HIGH_RESOLUTION_HD/4;
					maxx = WIDTH_RESOLUTION_HD/2;
					maxy = HIGH_RESOLUTION_HD*3/4;
				}
				else
				{
					minx = WIDTH_RESOLUTION/2;
					miny = High_Resolution/4;
					maxx = WIDTH_RESOLUTION/2;
					maxy = High_Resolution*3/4;
				}
				if (x<minx && y>miny && y<maxy)
					PlayCam(Play_Cam[0]);
				else if (x>minx && y>miny && y<maxy)
					PlayCam(Play_Cam[1]);
			}
			else if ((Play_Mode/10)==4)
			{
//				getorderch(tmp,4);
				if (MseType)
				{
					minx = WIDTH_RESOLUTION_HD/2;
					miny = HIGH_RESOLUTION_HD/2;
					maxx = WIDTH_RESOLUTION_HD/2;
					maxy = HIGH_RESOLUTION_HD*3/4;
				}
				else
				{
					minx = WIDTH_RESOLUTION/2;
					miny = High_Resolution/2;
					maxx = WIDTH_RESOLUTION/2;
					maxy = High_Resolution*3/4;
				}
				if (x<minx && y<miny)
					PlayCam(Play_Cam[0]);
				else if (x>minx && y<miny)
					PlayCam(Play_Cam[1]);
				else if (x<minx && y>miny)
					PlayCam(Play_Cam[2]);
				else if (x>minx && y>miny)
					PlayCam(Play_Cam[3]);
			}
			else if ((Play_Mode/10)==8)
			{
//				getorderch(tmp,8);
				if (MseType)
				{
					minx = WIDTH_RESOLUTION_HD/3;
					miny = HIGH_RESOLUTION_HD/3;
					maxx = WIDTH_RESOLUTION_HD*2/3;
					maxy = HIGH_RESOLUTION_HD*2/3;
				}
				else
				{
					minx = WIDTH_RESOLUTION/3;
					miny = High_Resolution/3;
					maxx = WIDTH_RESOLUTION*2/3;
					maxy = High_Resolution*2/3;
				}
				if (x<minx && y<miny)
					PlayCam(Play_Cam[0]);
				else if (x<maxx && y<miny)
					PlayCam(Play_Cam[1]);
				else if (x>maxx && y<miny)
					PlayCam(Play_Cam[2]);
				else if (x<minx && y<maxy)
					PlayCam(Play_Cam[3]);
				else if (x<maxx && y<maxy)
					PlayCam(Play_Cam[4]);
				else if (x>maxx && y<maxy)
					PlayCam(Play_Cam[5]);
				else if (x<minx && y>maxy)
					PlayCam(Play_Cam[6]);
				else if (x<maxx && y>maxy)
					PlayCam(Play_Cam[7]);
				else if (x>maxx && y>maxy && Cam_Num>CHANNEL8)
					PlayCam(Play_Cam[8]);
			}
			else if (Play_Mode==16)
			{
				if (!SystemLock)
				{
					if (MseType)
					{
						minx = WIDTH_RESOLUTION_HD*3/4;
						miny = HIGH_RESOLUTION_HD*3/4;
						maxx = WIDTH_RESOLUTION_HD/2;
						maxy = HIGH_RESOLUTION_HD/2;
						mx = WIDTH_RESOLUTION_HD/4;
						my = HIGH_RESOLUTION_HD/4;
					}
					else
					{
						minx = WIDTH_RESOLUTION*3/4;
						miny = High_Resolution*3/4;
						maxx = WIDTH_RESOLUTION/2;
						maxy = High_Resolution/2;
						mx = WIDTH_RESOLUTION/4;
						my = High_Resolution/4;
					}
					if (x<my && y<my)
						PlayCam(Play_Cam[0]);
					else if (x<maxx && y<my)
						PlayCam(Play_Cam[1]);
					else if (x<minx && y<my)
						PlayCam(Play_Cam[2]);
					else if (x>minx && y<my)
						PlayCam(Play_Cam[3]);
					else if (x<my && y<maxy)
						PlayCam(Play_Cam[4]);
					else if (x<maxx && y<maxy)
						PlayCam(Play_Cam[5]);
					else if (x<minx && y<maxy)
						PlayCam(Play_Cam[6]);
					else if (x>minx && y<maxy)
						PlayCam(Play_Cam[7]);
					else if (x<my && y<miny)
						PlayCam(Play_Cam[8]);
					else if (x<maxx && y<miny)
						PlayCam(Play_Cam[9]);
					else if (x<minx && y<miny)
						PlayCam(Play_Cam[10]);
					else if (x>minx && y<miny)
						PlayCam(Play_Cam[11]);
					else if (x<my && y>miny)
						PlayCam(Play_Cam[12]);
					else if (x<maxx && y>miny)
						PlayCam(Play_Cam[13]);
					else if (x<minx && y>miny)
						PlayCam(Play_Cam[14]);
					else if (x>minx && y>miny)
						PlayCam(Play_Cam[15]);
				}
			}
			break;
		case 25:
			if ((MenuStatus != mMouse) && (MenuStatus != (mMouse+Iconmenu)))
				Menu_Mouse_On();
			else
			{
				if ((Play_Mode/100) == 1)
				{
					Play_Cam[0]++;
					if (Play_Cam[0]>=Cam_Num)
						Play_Cam[0] = 0;
					PlayCam(Play_Cam[0]);
				}
				else if ((Play_Mode/10)==2)
				{
					Play_Mode++;
					if((Play_Mode%20)>=(Cam_Num/2))
						Play_Mode = 20;
					Split_Play(Play_Mode, 1, 0);
				}
				else if ((Play_Mode/10)==4)
				{
					Play_Mode++;
					if((Play_Mode%40)>=(Cam_Num/4))
						Play_Mode = 40;
					Split_Play(Play_Mode, 1, 0);
				}
				else if ((Play_Mode/10)==8)
				{
					Play_Mode++;
					if((Play_Mode%80)>=(Cam_Num/8))
						Play_Mode = 80;
					Split_Play(Play_Mode, 1, 0);
				}
			}
			break;
		default:
			break;
	}
	Play_Status_Show();
	thflag =0;
}


void PLAY_key_func()
{
	if (thflag)
		return;
	else 
		thflag = 1;
	Play_Status_Show();
	if(byKey_val==0) 
	{
		thflag = 0;
		return;
	}
	printf("PLAY_key_func   byKey_val =%d\n",byKey_val);
	switch(byKey_val)
	{
		case kPLAY:
			Rplay_Start();
			break;
		case kFFAST:
		case kRH://kFFAST:
			Play_Fast();
			break;
		case kRFAST:
		case kLT://kRFAST:
			Play_Back();
			break;
		case kESC:
			Play_Stop();
			break;
		case kSTOP:
//			Play_Stop();
//			break;
		case kSEA:
			if (!CheckUser(menupass[curuser].Playcfig))
			{
				Play_Err_Stop();
				menu_time_back_on();
			}
			break;
		case k4q:
		case kUP:
//			if(menu_num == mMouse)
//				;
//			else
			{
				if (Cam_Num>CHANNEL4)
				{
					if (4!=(Play_Mode/10))
						Play_Mode = 40;
					else if(Play_Mode<45)
					{
						Play_Mode++;
						if (Play_Mode>=(40+Cam_Num/4))
							Play_Mode = 40;
					}
					Split_Play(Play_Mode,1,0);
				}
				else if (Cam_Num==CHANNEL4)
				{
//					peng 2010-11-12 10:51:16
					if(RecamMenu[0].Reselution==0)
					{
						if (4!=(Play_Mode/10))    
						{
							Play_Mode = 40;
							Split_Play(Play_Mode,1,0);
						}
					}
				}
			}
			break;
		case k8q:
		case kET:
			if (Cam_Num>CHANNEL8)
			{
				if (8!=(Play_Mode/10))
					Play_Mode = 80;
				else if(Play_Mode<82)
				{
					Play_Mode++;
					if (Play_Mode>=82)
						Play_Mode = 80;
				}
				Split_Play(Play_Mode,1,0);
			}
			else if (Cam_Num==CHANNEL8)
			{
				if (8!=(Play_Mode/10))
				{
					Play_Mode = 80;
					Split_Play(Play_Mode,1,0);
				}
			}
			break;
		case k16q:
		case kDN:
			if (ChipSelect == HI3520 && Cam_Num>CHANNEL8)
			{
				if (16 != Play_Mode)
				{
					Play_Mode = 16;
					Split_Play(Play_Mode,1,0);
				}
			}
			break;	
		case k01:
		case k02:
		case k03:
		case k04:
		case k05: 
		case k06:
		case k07:
		case k08:
		case k09:
			PlayCam((U8)(byKey_val-k01));
			break;
		case k10:
				PlayCam(9);
		case k11:
		case k12:
		case k13:
		case k14:
		case k15:
		case k16:
			PlayCam((U8)(byKey_val-0xf7));
			break;
		case kREC:
//			BackUpOn();
			break;
		case kAUDIO:
//			audio_ch_on();
			break;
		case kCHAD:
			PlayCamAdjust(1);
			break;
		case kCHDE:
			PlayCamAdjust(0);
			break;
		default:
			break;
	}
	byKey_val=0;
	thflag = 0;
}


