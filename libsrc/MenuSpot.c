#include "cmn.h"

U8 MENUSPOT;
SPOTANDAUDIO spotandaudiomenupic;
U8 spot_cam[2][CHANNELNUM+1];

 U16 tSpot_title_pos[]=
{
	58,86,	144,86,	230,86,	316,86,
	58,112,	144,112,	230,112,	316,112,
	58,138,	144,138,	230,138,	316,138,
	58,164,	144,164,	230,164,	316,164,
};

U16 Spot_list_pos[]=
{
	120,60,
	120,86,	206,86,	292,86,	378,86,
	120,112,	206,112,	292,112,	378,112,
	120,138,	204,138,	292,138,	378,138,
	120,164,	204,164,	292,164,	378,164,
			152,230//,	240,230
};

U16 Pre_spotpos[]=
{
	120,60,		144,84,

	120,86,		144,110,
	206,86,		230,110,
	292,86,		316,110,
	378,86,		402,110,

	120,112,		144,136,
	206,112,		230,136,
	292,112,		316,136,
	378,112,		402,136,
		
	120,138,		144,162,
	206,138,		230,162,
	292,138,		316,162,
	378,138,		402,162,

	120,164,		144,188,
	206,164,		230,188,
	292,164,		316,188,
	378,164,		402,188,

//	64,230,		224,254,
	152,230,		312,254
};

U8 SpotTimes,Spotch;

void SpotchShow(U8 ch)
{
}

void Spot3520Set()
{
	U8 i;
	if (ChipSelect == HI3515)
		return;
	if (spot_cam[1][0])
	{
		i = Spotch;
		if (SpotTimes>=CamMenu[Spotch].SpotTime)
		{
			if ((++Spotch)>=Cam_Num)
				Spotch = 0;
			while(i!=Spotch)
			{
				if (CamMenu[Spotch].SpotTime)
					break;
				else if ((++Spotch)>=Cam_Num)
					Spotch=0;
			}
			if (i != Spotch)
			{
//				gstCommonParam->GST_DRA_Hisi_set_spot_window(Spotch);
				SpotchShow(Spotch);
			}
			SpotTimes = 0;
		}
		SpotTimes++;
	}
}

void Set_Spot()
{
	U8 i,tmp=0;
	if (spot_cam[1][0])
	{
//		pstCommonParam->gst_ext_write_rtc(0,RTC_RAM_SPOT_FLAG_ADDR,1,&tmp);
	}
	else
	{
		for (i=0;i<Cam_Num;i++)
		{
			if (spot_cam[1][i+1])
			{
				if (ChipSelect == HI3520)
				{
//					gstCommonParam->GST_DRA_Hisi_set_spot_window(i);
					SpotchShow(i);
				}
				tmp = i+1;
//				pstCommonParam->gst_ext_write_rtc(0,RTC_RAM_SPOT_FLAG_ADDR,1,&tmp);
				break;
			}
		}
	}
	
}

void spotkeyboradset(U8 ch)
{
	memset(spot_cam[1],0x00,sizeof(spot_cam[1]));
	spot_cam[1][ch+1] = 1;
	Set_Spot();
}

void SpotMenu_Inital()
{
}

void menu_spot_disp(U8 pos,U16 color)
{
}

void menu_spot_on()
{
}


void menu_spot_off()
{
	cur_pos = 0;
	menu_num = 0;
	Set_Spot();
    	OSD_ALL_CLEAR();
	demo_osd_disp();
}


void menu_spot_up()
{
	menu_spot_disp(cur_pos,WHITE);
	if(cur_pos<=0) 
		cur_pos=17;
	else if (cur_pos==17)
		cur_pos=MENUSPOT-2;
	else
		cur_pos--;
	menu_spot_disp(cur_pos,RED);
}

void menu_spot_down()
{
	menu_spot_disp(cur_pos,WHITE);
	if(cur_pos==17) 
		cur_pos=0;
	else
		cur_pos++;
	
	if ((Cam_Num < 16) && (cur_pos==MENUSPOT-1))
		cur_pos = 17;
	menu_spot_disp(cur_pos,RED);
}

void menu_spot_enter()
{
	U8 i;

	if (cur_pos==17)
	{
		menu_spot_off();
	}
//	else if (cur_pos==18)
//		menu_spot_off();
	else if (cur_pos<MENUSPOT-1)
	{
		for (i=0;i<17;i++)
		{
			if (i !=cur_pos && spot_cam[0][i])
			{
				spot_cam[0][i] = 0;
				menu_spot_disp(i,WHITE);
				break;
			}
		}
		spot_cam[0][cur_pos] =1;
		menu_spot_disp(cur_pos,RED);
		memcpy(spot_cam[1],spot_cam[0],sizeof(spot_cam[1]));
		Set_Spot();
	}
}

void SpotMouseLClick(U16 x, U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_spotpos[cur_pos*4]+offsetx_hd;
		miny = Pre_spotpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_spotpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_spotpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_spotpos[cur_pos*4]+offsetx;
		miny = Pre_spotpos[cur_pos*4+1]+offsety;
		maxx = Pre_spotpos[cur_pos*4+2]+offsetx;
		maxy = Pre_spotpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		menu_spot_enter();
	}
}


void Mouse_spot_Move_Func(U16 x,U16 y)
{
	U8 i,tmp;
	U16 minx,miny,maxx,maxy;
	for (i=0;i<MENUSPOT;i++)
	{
		if (Cam_Num < 16)
		{
			if (i==MENUSPOT-1)
				tmp = 17;
//			else if (i==MENUSPOT-1)
//				tmp = 18;
			else
				tmp = i;
		}
		else
			tmp = i;
		if (MseType)
		{
			minx = Pre_spotpos[tmp*4]+offsetx_hd;
			miny = Pre_spotpos[tmp*4+1]+offsety_hd;
			maxx = Pre_spotpos[tmp*4+2]+offsetx_hd;
			maxy = Pre_spotpos[tmp*4+3]+offsety_hd;
		}
		else
		{
			minx = Pre_spotpos[tmp*4]+offsetx;
			miny = Pre_spotpos[tmp*4+1]+offsety;
			maxx = Pre_spotpos[tmp*4+2]+offsetx;
			maxy = Pre_spotpos[tmp*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
		{
			menu_spot_disp(cur_pos,WHITE);
			cur_pos=tmp;
			menu_spot_disp(cur_pos,RED);
			break;
		}
	}
}

void mSPOT_key_func()
{
	if(byKey_val==0) return;
	printf("mSCHEDUL_key_func\n");
	switch(byKey_val)                   
	{
	case kESC:	//menu        exit
		menu_spot_off();
		break;
	case kLT:
	case kUP:
		menu_spot_up();
		break;
	case kRH:
	case kDN:
		menu_spot_down();
		break;
	case kET:
		menu_spot_enter();
		break;
	default:
		break;
       }
	byKey_val=0;
}


