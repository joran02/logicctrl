#include "cmn.h"




MenuRec RecamMenu[CHANNELNUM+1];
video_profile   video_setting_tw[CHANNELNUM];
RECMENU recmenupic;



U8 RECSETMAXMENU;

U8 Menu_Page = 0;

U16 tRec_data_pos[]=
{
	114,92,	256,92,	372,92,	480,92,
	114,116,	256,116,	372,116,	480,116,
	114,140,	256,140,	372,140,	480,140,
	114,164,	256,164,	372,164,	480,164,
	114,188,	256,188,	372,188,	480,188,
	114,212,	256,212,	372,212,	480,212,
	114,236,	256,236,	372,236,	480,236,
	114,260,	256,260,	372,260,	480,260,
						544,292,
			130,326,	314,326

};

U16 Pre_recpos[] = 
{
	114,92,		214,112,	
	256,92,		356,112,
	372,92,		396,112,
	480,92,		504,112,
	
	114,116,		214,136,
	256,116,		356,136,
	372,116,		396,136,
	480,116,		504,136,
	
	114,140,		214,160,
	256,140,		356,160,
	372,140,		396,160,
	480,140,		504,160,
	
	114,164,		214,184,
	256,164,		356,184,
	372,164,		396,184,
	480,164,		504,184,

	114,188,		214,208,
	256,188,		356,208,
	372,188,		396,208,
	480,188,		504,208,
	
	114,212,		214,232,
	256,212,		356,232,
	372,212,		396,232,
	480,212,		504,232,
	
	114,236,		214,256,
	256,236,		356,256,
	372,236,		396,256,
	480,236,		504,256,

	114,260,		214,280,
	256,260,		356,280,
	372,260,		396,280,
	480,260,		504,280,

	544,292,		578,326,

	130,326,		300,350,
	314,326,		484,350
};
/*
U8 Menu_Frm_Pal_Data[]=
{
	12,18,22,25
};

U8 Menu_Frm_Ntsc_Data[]=
{
	18,22,26,30
};
*/
U8 Menu_Frm_CIF_Pal_Data[]=
{
//	12,18,22,25
	10,15,20,25
};

U8 Menu_Frm_D1_Pal_Data[]=
{
//	3,4,5,6	
	10,15,20,25
};


U8 Menu_Frm_CIF_Ntsc_Data[]=
{
//	18,22,26,30
	15,20,25,30
};

U8 Menu_Frm_D1_Ntsc_Data[]=
{
//	3,4,5,6
	15,20,25,30
};


U8* Menu_Frm_Pal_Data[]=
{
	Menu_Frm_CIF_Pal_Data,
	Menu_Frm_D1_Pal_Data,
};

U8* Menu_Frm_Ntsc_Data[]=
{
	Menu_Frm_CIF_Ntsc_Data,
	Menu_Frm_D1_Ntsc_Data,
};


U8 **Menu_Frm_Data[]=
{
	Menu_Frm_Ntsc_Data,	
	Menu_Frm_Pal_Data
};


void MenuRecInit()
{
	U8 i;
	for (i=0;i<17;i++)
	{
		I_read_block_eeprom(ADDRECAM+ADDCOUNTRECCAM*i,sizeof(RecamMenu[i]),&RecamMenu[i].Reselution);
//		if (Cam_Num==CHANNEL16)
//			RecamMenu[i].Reselution = 1;
	}
}

void SetRecAudio()
{
#if 0
	int ret,tmp,i;
	tmp = 0;
	for (i=0;i<Cam_Num;i++)
	{
		if (RecamMenu[i].AudioOnoff)
			tmp |= WBIT(i);
	}
	ret=gstCommonParam->GST_FTC_AudioRecCam(tmp);
	if (ret<0)
		printf("Record audio set error!\n");
#endif
}
U16 Cal_bitrate(U8 framrate,U8 quality,U8 resolu)
{
	U16 tmp;
//	printf("############## quality: %d\n",quality);
	if (resolu)
	{
		if (quality == 2)
		{
			if (SysMenu.VideoFormat == VDO_NTSC)
				tmp = (650*framrate*4)/30;
			else
				tmp = (650*framrate*4)/25;
		}
		else if (quality == 1)
		{
			if (SysMenu.VideoFormat == VDO_NTSC)
				tmp = (512*framrate*4)/30;
			else
				tmp = (512*framrate*4)/25;
		}
		else
		{
			if (SysMenu.VideoFormat == VDO_NTSC)
				tmp = (384*framrate*4)/30;
			else
				tmp = (384*framrate*4)/25;
		}
	}
	else
	{
		if (quality == 2)
		{
			if (Cam_Num != CHANNEL16)
			{
				if (SysMenu.VideoFormat == VDO_NTSC)
					tmp = (1024*framrate)/30;
				else
					tmp = (1024*framrate)/25;
			}
			else
			{
				if (SysMenu.VideoFormat == VDO_NTSC)
					tmp = (650*framrate)/30;
				else
					tmp = (650*framrate)/25;
			}
		}
		else if (quality == 1)
		{
			if (SysMenu.VideoFormat == VDO_NTSC)
				tmp = (512*framrate)/30;
			else
				tmp = (512*framrate)/25;
		}
		else
		{
			if (SysMenu.VideoFormat == VDO_NTSC)
				tmp = (384*framrate)/30;
			else
				tmp = (384*framrate)/25;
		}
	}
//	tmp /= 64;
//	return((tmp+1)*64);
	return tmp;
	
}

U16 Get_Fream_Setup()
{
	U16 tmp=0;
	U8 i;
	for (i=0;i<Cam_Num;i++)
		tmp += RecamMenu[i].FrmRate+1;//Menu_Frm_Data[SysMenu.VideoFormat][RecamMenu[i].Reselution][RecamMenu[i].FrmRate];
	return tmp;
}

void RecorSet()
{
	 U8 NorP,CiforD1,picqty,chg=0;
	 int ret,tmp=0,ll;
	U8 i;
	
//peng edit 2010-10-20 5:11:22
//pal 12
//n 15
	if(Cam_Num==CHANNEL8)
	{
		Menu_Frm_D1_Pal_Data[3]=12;
		Menu_Frm_D1_Ntsc_Data[3]=15;
	}
//peng edit 2010-10-20 5:11:24

	if (Cam_Num==CHANNEL16)
	{
		for (i=0;i<Cam_Num;i++)
		{
			if (RecamMenu[i].AudioOnoff)
			{
				chg = 1;
				break;
			}
		}
	}

	for (i=0;i<Cam_Num;i++)
	{
		video_setting_tw[i].qmax = 31;
		video_setting_tw[i].qmin = 1;
		video_setting_tw[i].quant = 0;
		tmp |= WBIT(i);

		video_setting_tw[i].framerate = RecamMenu[i].FrmRate+1;//Menu_Frm_Data[SysMenu.VideoFormat][RecamMenu[i].Reselution][RecamMenu[i].FrmRate];
		picqty = RecamMenu[i].PicQty;
		if (RecamMenu[i].PicQty==2 && chg)
			picqty = 1;
		ll = Cal_bitrate(video_setting_tw[i].framerate,picqty,RecamMenu[i].Reselution);
		video_setting_tw[i].bit_rate = ll;//Menu_Frm_Data[SysMenu.VideoFormat][RecordMenu.Resolution][RecamMenu[i].FrmRate];
//		printf("RecordMenu.Resolution = %x,RecamMenu[%x].PicQty = %x,RecamMenu[%x].FrmRate=%x\n",RecordMenu.Resolution,i,RecamMenu[i].PicQty,i,RecamMenu[i].FrmRate);
		printf("video_setting_tw[%d].bit_rate = %d,ll=%d,video_setting_tw[%d].framerate=%d\n",i,video_setting_tw[i].bit_rate,ll,i,video_setting_tw[i].framerate+1);
		video_setting_tw[i].frame_rate_base = 1;
		video_setting_tw[i].gop_size = video_setting_tw[i].framerate;	
/*		
		video_setting_tw[i].width = 720;
		if (SysMenu.VideoFormat)
			video_setting_tw[i].height = 576;
		else
			video_setting_tw[i].height = 480;
		video_setting_tw[i].decode_width = 720;
		if (SysMenu.VideoFormat)
			video_setting_tw[i].decode_height = 576;
		else
			video_setting_tw[i].decode_height = 480;
*/
	}
	if (SysMenu.VideoFormat)
		NorP =  0;
	else
		NorP = 1;
	if (RecamMenu[0].Reselution)
		CiforD1=0;
	else
		CiforD1=TYPE_CIF;


	printf("NorP = %x,CiforD1 = %x,RecamMenu[0].Reselution=%d\n",NorP,CiforD1,RecamMenu[0].Reselution);

	if(1)
	{//设置码流从公用数据中得到。
		IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
		IPCAM_VCODE_VS_ST vcode;
		
		 
		int chan = 0;

		if( CiforD1 == TYPE_CIF)
			chan = 1;	
		else
			chan = 0;
			
		vcode = pAllInfo->onvif_st.vcode[chan];		
//		vcode = pAllInfo->onvif_st.vcode[0];		

		if( NorP  == TD_DRV_VIDEO_STARDARD_PAL)
			video_setting_tw[0].bit_rate =  vcode.BitrateLimit * vcode.FrameRateLimit /25;
		else
			video_setting_tw[0].bit_rate =  vcode.BitrateLimit * vcode.FrameRateLimit /30;

		video_setting_tw[0].framerate = vcode.FrameRateLimit;
		video_setting_tw[0].gop_size =  vcode.FrameRateLimit * vcode.GovLength;

		DPRINTK("set encode %d %d %d\n",video_setting_tw[0].bit_rate,
			video_setting_tw[0].framerate,video_setting_tw[0].gop_size);
		
	}
	ret = gstCommonParam->GST_DRA_change_device_set(&video_setting_tw[0],NorP,CiforD1);
	if (ret<0)
		printf("change_device_set error!\n");
	gstCommonParam->GST_DRA_local_instant_i_frame(tmp);

}

#ifdef TW54M27M
void	rec_frm_set_tmp(int m)
{
	U8 NorP,CiforD1,picqty;
	int ret,tmp=0,i;
	static int run_flag=0xff;
	
	if(run_flag!=m)
		run_flag=m;
	else 
		return ;

	if(m)
	{	
		for (i=0;i<Cam_Num;i++)
		{
			video_setting_tw[i].qmax = 31;
			video_setting_tw[i].qmin = 1;
			video_setting_tw[i].quant = 0;
			tmp |= WBIT(i);
			video_setting_tw[i].framerate =m;
			picqty = RecamMenu[i].PicQty;
			video_setting_tw[i].bit_rate = Cal_bitrate(video_setting_tw[i].framerate,picqty,RecamMenu[i].Reselution);
			video_setting_tw[i].frame_rate_base = 1;
			video_setting_tw[i].gop_size = video_setting_tw[i].framerate;	
		}
		if (SysMenu.VideoFormat)
			NorP =  0;
		else
			NorP = 1;
		if (RecamMenu[0].Reselution)
			CiforD1=0;
		else
			CiforD1=TYPE_CIF;
		ret = gstCommonParam->GST_DRA_change_device_set(&video_setting_tw[0],NorP,CiforD1);
		if (ret<0)
			printf("change_device_set error!\n");
		gstCommonParam->GST_DRA_local_instant_i_frame(tmp);
	}
	else
		RecorSet();
}
#endif

void menu_record_data_disp(U8 pos,U16 color)
{
}

void menu_record_on()
{
}

void menu_record_do()
{
	U8 i;
//	if (Cam_Num!=CHANNEL16)
	{
		I_read_block_eeprom(ADDRECAM+ADDCOUNTRECCAM,1,&i);
		if (i != RecamMenu[0].Reselution)
			bReset = 1;
	}
	for (i=0;i<Cam_Num;i++)
	{
		I_write_block_eeprom(ADDRECAM+ADDCOUNTRECCAM*i,sizeof(RecamMenu[i]),&RecamMenu[i].Reselution);
//		printf("********* reselution:%d\n",RecamMenu[i].Reselution);
	}
	SetRecAudio();
	RecorSet();
}

void menu_record_dec()
{
	U8 i;
	 switch(cur_pos)
	 {
		case 0:
		case 4:
		case 8:
		case 12:
		case 16:
		case 20:
		case 24:
		case 28:
//			if (Cam_Num!=CHANNEL16)
			{
				for (i=0;i<Cam_Num;i++)
				{
					if(RecamMenu[i].Reselution)
						RecamMenu[i].Reselution = 0;
					else
						RecamMenu[i].Reselution = 1;
				}
				for (i=0;i<Cam_Num;i++)
				{
					if ((cur_pos != (i*4)) && i<8)
						menu_record_data_disp(i*4,WHITE);
					menu_record_data_disp(i*4+2,WHITE);
				}
			}
			break;
		case 1:
		case 5:
		case 9:
		case 13:
		case 17:
		case 21:
		case 25:
		case 29:
			if(RecamMenu[(cur_pos-1)/4+Menu_Page*8].PicQty==0)
				RecamMenu[(cur_pos-1)/4+Menu_Page*8].PicQty = 2;
			else
				RecamMenu[(cur_pos-1)/4+Menu_Page*8].PicQty--;
			break;
		case 2:
		case 6:
		case 10:
		case 14:
		case 18:
		case 22:
		case 26:
		case 30:
			if (RecamMenu[(cur_pos-2)/4+Menu_Page*8].FrmRate ==0)
				RecamMenu[(cur_pos-2)/4+Menu_Page*8].FrmRate = 3;
			else
				RecamMenu[(cur_pos-2)/4+Menu_Page*8].FrmRate --;
			break;
	 }
	menu_record_data_disp(cur_pos,RED);
}
void menu_record_add()
{
	U8 i;
	 switch(cur_pos)
	 {
		case 0:
		case 4:
		case 8:
		case 12:
		case 16:
		case 20:
		case 24:
		case 28:
//			if (Cam_Num!=CHANNEL16)
			{
				for (i=0;i<Cam_Num;i++)
				{
					if(RecamMenu[i].Reselution)
						RecamMenu[i].Reselution = 0;
					else
						RecamMenu[i].Reselution = 1;
				}
				for (i=0;i<Cam_Num;i++)
				{
					if ((cur_pos != (i*4)) && i<8)
						menu_record_data_disp(i*4,WHITE);
					menu_record_data_disp(i*4+2,WHITE);
				}
			}
			break;
		case 1:
		case 5:
		case 9:
		case 13:
		case 17:
		case 21:
		case 25:
		case 29:
			if(RecamMenu[(cur_pos-1)/4+Menu_Page*8].PicQty>=2)
				RecamMenu[(cur_pos-1)/4+Menu_Page*8].PicQty = 0;
			else
				RecamMenu[(cur_pos-1)/4+Menu_Page*8].PicQty++;
			break;
		case 2:
		case 6:
		case 10:
		case 14:
		case 18:
		case 22:
		case 26:
		case 30:
			if (RecamMenu[(cur_pos-2)/4+Menu_Page*8].FrmRate >=3)
				RecamMenu[(cur_pos-2)/4+Menu_Page*8].FrmRate = 0;
			else
				RecamMenu[(cur_pos-2)/4+Menu_Page*8].FrmRate ++;
			break;
	 }
	menu_record_data_disp(cur_pos,RED);
}
void menu_record_down()
{
	menu_record_data_disp(cur_pos,WHITE);

	if(cur_pos==34) 
		cur_pos=0;
	else
		cur_pos++;
	
	if ((Cam_Num < 16) && (cur_pos==RECSETMAXMENU-2))
		cur_pos = 33;
	
	menu_record_data_disp(cur_pos,RED);
}
void menu_record_up()
{
	menu_record_data_disp(cur_pos,WHITE);
	
	if(cur_pos<=0) 
		cur_pos=34;
	else if (cur_pos==33)
		cur_pos=RECSETMAXMENU-3;
	else
		cur_pos--;
	
	menu_record_data_disp(cur_pos,RED);
}

void menu_record_off()
{
    	OSD_ALL_CLEAR();
	MenuRecInit();
	menu_main_on();
}

void menu_record_enter()
{
	U8 i;
	
	switch(cur_pos)
	{
		case 3:
		case 7:
		case 11:
		case 15:
		case 19:
		case 23:
		case 27:
		case 31:
			if (RecamMenu[(cur_pos-2)/4+Menu_Page*8].AudioOnoff)
				RecamMenu[(cur_pos-2)/4+Menu_Page*8].AudioOnoff = 0;
			else
				RecamMenu[(cur_pos-2)/4+Menu_Page*8].AudioOnoff=1;
			menu_record_data_disp(cur_pos,RED);
			break;
		case 32:
			if (Menu_Page==0)
				Menu_Page = 1;
			else
				Menu_Page = 0;
			if (Cam_Num == CHANNEL16)
			{
				for (i=0;i<8;i++)
				{
					Bmp_All_Show(offsetx+20,offsety+88+i*24, recmenupic.recexcel,AD);
					Bmp_All_Show(offsetx_hd+20,offsety_hd+88+i*24, recmenupic.recexcel,HD);
					Osd_Dec2_display(offsetx+52,offsety+92+i*24, WHITE, i+1+Menu_Page*8,0x0f,AD);
					Osd_Dec2_display(offsetx_hd+52,offsety_hd+92+i*24, WHITE, i+1+Menu_Page*8,0x0f,HD);
				}
			}
			for (i=0;i<RECSETMAXMENU;i++)
			{
				if (i==cur_pos)
					menu_record_data_disp(i,RED);
				else
					menu_record_data_disp(i,WHITE);
			}
			break;
		case 33:
			menu_record_do();
//			break;
		case 34:
			menu_record_off();
			break;
		default:
			break;
	}
}

void MouseRecRClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_recpos[cur_pos*4]+offsetx_hd;
		miny = Pre_recpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_recpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_recpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_recpos[cur_pos*4]+offsetx;
		miny = Pre_recpos[cur_pos*4+1]+offsety;
		maxx = Pre_recpos[cur_pos*4+2]+offsetx;
		maxy = Pre_recpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
		menu_record_add();
}

void MouseRecLClick(U16 x,U16 y,U8 type)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_recpos[cur_pos*4]+offsetx_hd;
		miny = Pre_recpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_recpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_recpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_recpos[cur_pos*4]+offsetx;
		miny = Pre_recpos[cur_pos*4+1]+offsety;
		maxx = Pre_recpos[cur_pos*4+2]+offsetx;
		maxy = Pre_recpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		menu_record_dec(); 
		if (type == 26)
			menu_record_enter();
	}
}

void Mouse_Rec_Move_Func(U16 x,U16 y)
{
	U8 i,tmp;
	U16 minx,miny,maxx,maxy;
	for (i=0;i<RECSETMAXMENU;i++)
	{
		if (Cam_Num < 16)
		{
			if (i==RECSETMAXMENU-2)
				tmp = 33;
			else if (i==RECSETMAXMENU-1)
				tmp = 34;
			else
				tmp = i;
		}
		else
			tmp = i;
		if (MseType)
		{
			minx = Pre_recpos[tmp*4]+offsetx_hd;
			miny = Pre_recpos[tmp*4+1]+offsety_hd;
			maxx = Pre_recpos[tmp*4+2]+offsetx_hd;
			maxy = Pre_recpos[tmp*4+3]+offsety_hd;
		}
		else
		{
			minx = Pre_recpos[tmp*4]+offsetx;
			miny = Pre_recpos[tmp*4+1]+offsety;
			maxx = Pre_recpos[tmp*4+2]+offsetx;
			maxy = Pre_recpos[tmp*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != tmp))
		{
			menu_record_data_disp(cur_pos,WHITE);
			cur_pos=tmp;
			menu_record_data_disp(cur_pos,RED);
			break;
		}
	}
}


//******************************key func*************************************
void mRECORD_key_func()
{
	if(byKey_val==0) return;

	switch(byKey_val)
	{
	case kESC:
		menu_record_off();
		break;
	case kUP:
		menu_record_up();
   		break;
	case kDN:
		menu_record_down();
		break;
	case kRH:
	case kAD:
		menu_record_add();   
		break;
   	case kLT:
	case kDE:
		menu_record_dec();
		break;
	case kET:
		menu_record_enter();
	default:
		break;
    }
	byKey_val=0;
}


