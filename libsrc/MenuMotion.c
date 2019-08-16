#include "cmn.h"
#define MENUMOTIONNUM		8////4
ValMotion MotMenu[CHANNELNUM];

MOTMENU motmenupic;
U16 starx[CHANNELNUM],stary[CHANNELNUM];
U16 areawidth[CHANNELNUM],areahigh[CHANNELNUM],endx[CHANNELNUM],endy[CHANNELNUM];
U8 CamMotion;
//jlb090820MOTION_DETECT_INFO mval[CHANNELNUM];
//... Menu Motion Mask Table
 U16 tMotion_data_pos_N[]=
{
	520,72,
	520,98,
	520,202,
	520,228,
	520,254,
	520,280,
	150,356,
	354,356
};

 U16 tMotion_data_pos_P[]=
{
	520,78,
	520,104,
	520,242,
	520,268,
	520,294,
	520,320,
	152,388,
	356,388
};

 U16 *tMotion_data_pos[]=
{
	tMotion_data_pos_N,
	tMotion_data_pos_P
			
};
 
U16 Pre_motpos_N[] = 
{
	520,72,		628,92,	
	520,98,		628,118,
	520,202,		628,222,
	520,228,		628,248,
	520,254,		628,274,
	520,280,		628,300,
	150,356,		310,380,
	356,356,		516,380
};

U16 Pre_motpos_P[] = 
{
	520,78,		628,98,	
	520,104,		628,124,
	520,242,		628,262,
	520,268,		628,288,
	520,294,		628,314,
	520,320,		628,340,
	152,388,		312,412,
	356,388,		516,412
};

U16 *Pre_motpos[] = 
{
	Pre_motpos_N,	
	Pre_motpos_P
};


void set_motion(U8 flag)
{
//	if (dev_id==0)
	{
		if (flag)
			gstCommonParam->GST_DRA_drv_start_motiondetect();
		else
			gstCommonParam->GST_DRA_drv_stop_motiondetect();
	}
}
void MenuMotionInit()
{
	U8 i;
	for (i=0;i<Cam_Num;i++)
	{
//		printf("ADDMOT%d = %x\n",ADDMOT+ADDCOUNTMOT*i);
		I_read_block_eeprom(ADDMOT+ADDCOUNTMOT*i,sizeof(MotMenu[i]),&MotMenu[i].Sensitivity);
		starx[i] = MotMenu[i].StarX_H*256+MotMenu[i].StarX_L;
		stary[i] = MotMenu[i].StarY_H*256+MotMenu[i].StarY_L;
		endx[i] = MotMenu[i].EndX_H*256+MotMenu[i].EndX_L;
		endy[i] = MotMenu[i].EndY_H*256+MotMenu[i].EndY_L;

		if (endy[i]>High_Resolution/2)
		{
			endy[i]=High_Resolution/2;
			MotMenu[i].EndY_H = endy[i]/256;
			MotMenu[i].EndY_L = endy[i]%256;
			I_write_block_eeprom(ADDMOT+ADDCOUNTMOT*i+ENDYH,2,&MotMenu[i].EndY_H);
		}
		
		areawidth[i] = endx[i]-starx[i];
		areahigh[i] = endy[i]-stary[i];
//		set_motion_chen(i);
	}
	set_motion(1);
}

//U8 Add,Value,Readvalue;

void menu_motion_disp(U8 pos,U16 color)
{
}

void Area_Setup(U8 flag)
{
//	U16 i,j;
	U16 chstarx,chstary;
	U16 chstarxhd,chstaryhd;
	if (areawidth[CamMotion] == 0 || areahigh[CamMotion] == 0)
		return;
	chstarx=28+offsetx;
	chstary = 62+offsety;
	chstarxhd=28+offsetx_hd;
	chstaryhd = 62+offsety_hd;

	if (flag==0)
	{
		draw_line(starx[CamMotion]+chstarx, stary[CamMotion]+chstary, starx[CamMotion]+chstarx+areawidth[CamMotion],stary[CamMotion]+chstary+areahigh[CamMotion],chstarx, chstary,378+offsetx,300+44*SysMenu.VideoFormat+offsety,AD,0x0000);
		draw_line(starx[CamMotion]+chstarxhd, stary[CamMotion]+chstaryhd,starx[CamMotion]+chstarxhd+areawidth[CamMotion],stary[CamMotion]+chstaryhd+areahigh[CamMotion], chstarxhd, chstaryhd,378+offsetx_hd,300+44*SysMenu.VideoFormat+offsety_hd,HD,0x0000);
	}
	else
	{
		draw_line(starx[CamMotion]+chstarx, stary[CamMotion]+chstary, starx[CamMotion]+chstarx+areawidth[CamMotion],stary[CamMotion]+chstary+areahigh[CamMotion],chstarx, chstary,378+offsetx,300+44*SysMenu.VideoFormat+offsety,AD,0x7c1f);
		draw_line(starx[CamMotion]+chstarxhd, stary[CamMotion]+chstaryhd,starx[CamMotion]+chstarxhd+areawidth[CamMotion],stary[CamMotion]+chstaryhd+areahigh[CamMotion], chstarxhd, chstaryhd,378+offsetx_hd,300+44*SysMenu.VideoFormat+offsety_hd,HD,0x7c1f);
	}
/*	
	if (dev_id==1)
		set_motion_chen(CamMotion);
//	printf("******************** flag : %d\n",flag);
	for (i=0;i<areawidth[CamMotion]/4;i++)
	{
		for (j=0;j<areahigh[CamMotion]/4;j++)
		{
			osd_bmp_photo(motmenupic.mix,starx[CamMotion]+i*4+chstarx, stary[CamMotion]+j*4+chstary,AD,0,(1-flag)*4,4,4,MIXCOLOR,15);
			osd_bmp_photo(motmenupic.mix,starx[CamMotion]+i*4+chstarxhd, stary[CamMotion]+j*4+chstaryhd,HD,0,(1-flag)*4,4,4,MIXCOLOR,15);
		}
	}
*/
}

void Area_Clear()
{
//	U16 i,j;
	U16 chstarx,chstary;
	U16 chstarxhd,chstaryhd;
	chstarx=28+offsetx;
	chstary = 62+offsety;
	chstarxhd=28+offsetx_hd;
	chstaryhd = 62+offsety_hd;
	clear_osd(chstarx, chstary, 350, 240+SysMenu.VideoFormat*48,AD);
	clear_osd(chstarxhd, chstaryhd, 350, 240+SysMenu.VideoFormat*48,HD);
}

void menu_motion_on()
{
}

void menu_motion_off()
{
	OSD_ALL_CLEAR();
	MenuMotionInit();
//	enable_box(0);
//	set_motion(rec_flag);
	menu_num=0;
	demo_set();
    	menu_main_on();
}


void menu_motion_up()
{
	menu_motion_disp(cur_pos,WHITE);
	if (cur_pos==0)
		cur_pos = MENUMOTIONNUM-1;
	else
		cur_pos --;
	menu_motion_disp(cur_pos,RED);
}

void menu_motion_down()
{
	menu_motion_disp(cur_pos,WHITE);
	if (cur_pos==MENUMOTIONNUM-1)
		cur_pos = 0;
	else
		cur_pos ++;
	menu_motion_disp(cur_pos,RED);
}

void netsetmotion()
{
	starx[0] = MotMenu[0].StarX_H*256+MotMenu[0].StarX_L;
	stary[0] = MotMenu[0].StarY_H*256+MotMenu[0].StarY_L;
	endx[0] = MotMenu[0].EndX_H*256+MotMenu[0].EndX_L;
	endy[0] = MotMenu[0].EndY_H*256+MotMenu[0].EndY_L;

	I_write_block_eeprom(ADDMOT,sizeof(MotMenu[0]),&MotMenu[0].Sensitivity);
	if (endy[0]>High_Resolution/2)
	{
		endy[0]=High_Resolution/2;
		MotMenu[0].EndY_H = endy[0]/256;
		MotMenu[0].EndY_L = endy[0]%256;
		I_write_block_eeprom(ADDMOT+ENDYH,2,&MotMenu[0].EndY_H);
	}
}

void menu_motion_do()
{
	U8 i;
	for (i=0;i<Cam_Num;i++)
	{
		MotMenu[i].StarX_H = starx[i]/256;
		MotMenu[i].StarX_L = starx[i]%256;
		MotMenu[i].StarY_H = stary[i]/256;
		MotMenu[i].StarY_L = stary[i]%256;
		MotMenu[i].EndX_H = endx[i]/256;
		MotMenu[i].EndX_L = endx[i]%256;
		MotMenu[i].EndY_H = endy[i]/256;
		MotMenu[i].EndY_L = endy[i]%256;
		I_write_block_eeprom(ADDMOT+ADDCOUNTMOT*i,sizeof(MotMenu[i]),&MotMenu[i].Sensitivity);
/*		
		MotMenu[i].StarX_H = starx[i]/256;
		MotMenu[i].StarX_L = starx[i]%256;
		I_write_block_eeprom(ADDMOT+ADDCOUNTMOT*i+STARXH,1,&MotMenu[i].StarX_H);
		I_write_block_eeprom(ADDMOT+ADDCOUNTMOT*i+STARXL,1,&MotMenu[i].StarX_L);

		MotMenu[i].StarY_H = stary[i]/256;
		MotMenu[i].StarY_L = stary[i]%256;
		I_write_block_eeprom(ADDMOT+ADDCOUNTMOT*i+STARYH,1,&MotMenu[i].StarY_H);
		I_write_block_eeprom(ADDMOT+ADDCOUNTMOT*i+STARYL,1,&MotMenu[i].StarY_L);

		MotMenu[i].EndX_H = endx[i]/256;
		MotMenu[i].EndX_L = endx[i]%256;
		I_write_block_eeprom(ADDMOT+ADDCOUNTMOT*i+ENDXH,1,&MotMenu[i].EndX_H);
		I_write_block_eeprom(ADDMOT+ADDCOUNTMOT*i+ENDXL,1,&MotMenu[i].EndX_L);

		MotMenu[i].EndY_H = endy[i]/256;
		MotMenu[i].EndY_L = endy[i]%256;
		I_write_block_eeprom(ADDMOT+ADDCOUNTMOT*i+ENDYH,1,&MotMenu[i].EndY_H);
		I_write_block_eeprom(ADDMOT+ADDCOUNTMOT*i+ENDYL,1,&MotMenu[i].EndY_L);
*/
	}

}

void get_area()
{
	areawidth[CamMotion] = endx[CamMotion]- starx[CamMotion];
	areahigh[CamMotion] = endy[CamMotion]- stary[CamMotion];
	if ((areawidth[CamMotion]==0)&&(areahigh[CamMotion]!=0))
		areawidth[CamMotion] =1;
	if ((areawidth[CamMotion]!=0)&&(areahigh[CamMotion]==0))
		areahigh[CamMotion] = 1;
	Area_Setup(0);
}

void menu_motion_add()
{
	U8 i;
	switch(cur_pos)
	{
		case 0:
			Area_Clear();
			if (CamMotion>=Cam_Num-1)
				CamMotion = 0;
			else
				CamMotion++;
			set_full_chinnal(28+offsetx,62+offsety,28+offsetx_hd,62+offsety_hd,CamMotion);
			Area_Setup(0);
			for(i=1;i<MENUMOTIONNUM;i++)
				menu_motion_disp(i,WHITE);
			break;
		case 1:
			if (MotMenu[CamMotion].Sensitivity>=10)
				MotMenu[CamMotion].Sensitivity=1;
			else
				MotMenu[CamMotion].Sensitivity++;
			break;
		case 2:
			if (starx[CamMotion]<endx[CamMotion])
			{
				Area_Clear();
				starx[CamMotion]++;
				get_area();
			}
 			break;
		case 3:
			if (endx[CamMotion]<350)
			{
				Area_Clear();
				endx[CamMotion] ++;
				get_area();
			}
			break;
		case 4:
			if (stary[CamMotion]<endy[CamMotion])
			{
				Area_Clear();
				stary[CamMotion] ++;
				get_area();
			}
			break;
		case 5:
			if (endy[CamMotion]<High_Resolution/2)
			{
				Area_Clear();
				endy[CamMotion] ++;
				get_area();
			}
			break;
		default:
			break;
	}
	menu_motion_disp(cur_pos,RED);
}

void menu_motion_dec()
{
	U8 i;
	switch(cur_pos)
	{
		case 0:
			Area_Clear();
			if (CamMotion==0)
				CamMotion = Cam_Num-1;
			else
				CamMotion--;
			set_full_chinnal(28+offsetx,62+offsety,28+offsetx_hd,62+offsety_hd,CamMotion);
			Area_Setup(0);
			for(i=1;i<MENUMOTIONNUM;i++)
				menu_motion_disp(i,WHITE);
			break;
		case 1:
			if (MotMenu[CamMotion].Sensitivity<=1)
				MotMenu[CamMotion].Sensitivity=10;
			else
				MotMenu[CamMotion].Sensitivity--;
			break;
		case 2:
			if (starx[CamMotion]>0x00)
			{
				Area_Clear();
				starx[CamMotion]--;
				get_area();
			}
 			break;
		case 3:
			if (endx[CamMotion]>starx[CamMotion])
			{
				Area_Clear();
				endx[CamMotion] --;
				get_area();
			}
			break;
		case 4:
			if (stary[CamMotion]>0)
			{
				Area_Clear();
				stary[CamMotion] --;
				get_area();
			}
			break;
		case 5:
			if (endy[CamMotion]>stary[CamMotion])
			{
				Area_Clear();
				endy[CamMotion] --;
				get_area();
			}
			break;
		default:
			break;
	}
	menu_motion_disp(cur_pos,RED);
}

void menu_motion_enter()
{
	if (cur_pos==MENUMOTIONNUM-1)
		menu_motion_off();
	else if (cur_pos==MENUMOTIONNUM-2)
	{
		menu_motion_do();
		menu_motion_off();
	}
}

void MouseMotionRClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_motpos[SysMenu.VideoFormat][cur_pos*4]+offsetx_hd;
		miny = Pre_motpos[SysMenu.VideoFormat][cur_pos*4+1]+offsety_hd;
		maxx = Pre_motpos[SysMenu.VideoFormat][cur_pos*4+2]+offsetx_hd;
		maxy = Pre_motpos[SysMenu.VideoFormat][cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_motpos[SysMenu.VideoFormat][cur_pos*4]+offsetx;
		miny = Pre_motpos[SysMenu.VideoFormat][cur_pos*4+1]+offsety;
		maxx = Pre_motpos[SysMenu.VideoFormat][cur_pos*4+2]+offsetx;
		maxy = Pre_motpos[SysMenu.VideoFormat][cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
		menu_motion_add(); 
}

void MouseMotionLClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_motpos[SysMenu.VideoFormat][cur_pos*4]+offsetx_hd;
		miny = Pre_motpos[SysMenu.VideoFormat][cur_pos*4+1]+offsety_hd;
		maxx = Pre_motpos[SysMenu.VideoFormat][cur_pos*4+2]+offsetx_hd;
		maxy = Pre_motpos[SysMenu.VideoFormat][cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_motpos[SysMenu.VideoFormat][cur_pos*4]+offsetx;
		miny = Pre_motpos[SysMenu.VideoFormat][cur_pos*4+1]+offsety;
		maxx = Pre_motpos[SysMenu.VideoFormat][cur_pos*4+2]+offsetx;
		maxy = Pre_motpos[SysMenu.VideoFormat][cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		if (cur_pos<MENUMOTIONNUM-2)
			menu_motion_dec(); 
		else
			menu_motion_enter();
	}
}


void getstar(U16 x,U16 y)
{
	U16 chstarx,chstary;
	if (MseType)
	{
		chstarx=28+offsetx_hd;
		chstary = 62+offsety_hd;
	}
	else
	{
		chstarx=28+offsetx;
		chstary = 62+offsety;
	}
	starx[CamMotion] = x-chstarx;

	stary[CamMotion] = y-chstary;
}

void getend(U16 x,U16 y)
{
	U16 tmpx,tmpy,tmpt,tmpb;
	if (MseType == 0)
	{
		tmpt = 62+offsety;
		tmpb = tmpt + 240+SysMenu.VideoFormat*48;
		if (x<(28+offsetx))
			tmpx = 0;
		else if (x>378+offsetx)
			tmpx = 350;
		else
			tmpx = x-(28+offsetx);
	}
	else
	{
		tmpt = 62+offsety_hd;
		tmpb = tmpt + 240+SysMenu.VideoFormat*48;
		if (x<(28+offsetx_hd))
			tmpx = 0;
		else if (x>378+offsetx_hd)
			tmpx = 350;
		else
			tmpx = x-(28+offsetx_hd);
	}
	
	if (y<tmpt)
		tmpy = 0;
	else if (y>tmpb)
		tmpy=tmpb-tmpt;
	else
		tmpy = y-tmpt;
	
	if (starx[CamMotion]>tmpx)
	{
		endx[CamMotion] = starx[CamMotion];
		starx[CamMotion] = tmpx;
	}
	else
		endx[CamMotion] = tmpx;
	
	if (endx[CamMotion] == starx[CamMotion])
		endx[CamMotion]++;
	
	areawidth[CamMotion] = endx[CamMotion] - starx[CamMotion];

	if (stary[CamMotion]>tmpy)
	{
		endy[CamMotion] = stary[CamMotion];
		stary[CamMotion] = tmpy;
	}
	else
		endy[CamMotion] = tmpy;

	if (endy[CamMotion] == stary[CamMotion])
		endy[CamMotion]++;
	
	areahigh[CamMotion] = endy[CamMotion]-stary[CamMotion];
}

U8 redpos=0;
U16 lineMx,lineMy;

U8 getmousestatus2()
{
	return redpos;
}

U8 outflag=0;
void Mouse_Mot_Move_Func(U16 x, U16 y, U8 type)
{
	U16 i;
	U16 chstarx,chstary,chendx,chendy;
	U16 minx,miny,maxx,maxy;
	U8 tmp = redpos;
	 if( outflag == 1)
	 	return;
	outflag=1;
	if (MseType)
	{
		chstarx=28+offsetx_hd;
		chendx=378+offsetx_hd;
		chstary = 62+offsety_hd;
		chendy = 302+SysMenu.VideoFormat*48+offsety_hd;
	}
	else
	{
		chstarx=28+offsetx;
		chendx=378+offsetx;
		chstary = 62+offsety;
		chendy = 302+SysMenu.VideoFormat*48+offsety;
	}
	switch (type)
	{
		case 0:
			for (i=0;i<MENUMOTIONNUM;i++)
			{
				if (MseType == 0)
				{
					minx = Pre_motpos[SysMenu.VideoFormat][i*4]+offsetx;
					miny = Pre_motpos[SysMenu.VideoFormat][i*4+1]+offsety;
					maxx = Pre_motpos[SysMenu.VideoFormat][i*4+2]+offsetx;
					maxy = Pre_motpos[SysMenu.VideoFormat][i*4+3]+offsety;
				}
				else
				{
					minx = Pre_motpos[SysMenu.VideoFormat][i*4]+offsetx_hd;
					miny = Pre_motpos[SysMenu.VideoFormat][i*4+1]+offsety_hd;
					maxx = Pre_motpos[SysMenu.VideoFormat][i*4+2]+offsetx_hd;
					maxy = Pre_motpos[SysMenu.VideoFormat][i*4+3]+offsety_hd;
				}
				if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != i))
				{
					menu_motion_disp(cur_pos,WHITE);
					cur_pos=i;
					menu_motion_disp(cur_pos,RED);
					break;
				}
			}
			if (redpos)
			{
				if (MseType)
					draw_line(lineMx, lineMy, x, y,chstarx,chstary,chendx,chendy,HD,0x0000);
				else
					draw_line(lineMx, lineMy, x, y,chstarx,chstary,chendx,chendy,AD,0x0000);
			}
			break;
		case 24:
			if (x>chstarx && x<chendx && y>chstary && y<chendy)
			{
				redpos = 1;
				lineMx = x;
				lineMy = y;
				Area_Clear();
				getstar(x,y);
			}
			break;
		case 26:
			if (redpos)
			{
				redpos = 0;
				getend(x,y);
/*
				if (dev_id==0)
				{
					if (MseType == 0)
						clear_osd(chstarx, chstary, chendx-chstarx, chendy-chstary,AD);
					else
						clear_osd(chstarx, chstary, chendx-chstarx, chendy-chstary,HD);
				}
*/
				Area_Setup(0);
				for (i=2;i<MENUMOTIONNUM-2;i++)
				{
					menu_motion_disp(i,WHITE);
				}
			}
			else
			{
				redpos = 1;
				MouseMotionLClick(x,y);
				redpos = tmp;
			}
			break;
		case 25:
		case 30:
			redpos = 1;
			MouseMotionRClick(x,y);
			redpos = tmp;
			break;
		case 31:
			if (!redpos && cur_pos<MENUMOTIONNUM-2)
			{
				redpos = 1;
				MouseMotionLClick(x,y);
				redpos = tmp;
			}
			break;
		default:
			break;
	}
	outflag = 0;
		
}

#ifdef GM8126_DEV
U16 motioncheck()
{
	int i,j;
	int left=0,top=0,right=0,bottom=0;
	int hcount,vcount,motionboxcount=0,alarm_info=0;
	int ret;
//	unsigned char msg[2];
	unsigned char buf[10*1000];
	
	ret=gstCommonParam->GST_DRA_Get_motion_alarm_buf(buf,&hcount,&vcount);
	if(ret>0)
	{
		left=starx[0];
		right=endx[0];
		top=stary[0];
		bottom=endy[0];
		if(left>right)
		{
			i=right;
			right=left;
			left=i;
		}
		if(top>bottom)
		{
			i=bottom;
			bottom=top;
			top=i;
		}
		for(i=0;i<hcount;i++)
			for(j=0;j<vcount;j++)
				if(((i*16)>left)&&((i*16)<right)&&((j*16)>top)&&((j*16)<bottom))
					if(buf[i*hcount+j])
						motionboxcount++;
		if(motionboxcount>(51-MotMenu[0].Sensitivity))
			alarm_info=1;
	}
//	printf("motionboxcount:%3d,MotMenu[0].Sensitivity:%d,alarm_info:%d\n",motionboxcount,MotMenu[0].Sensitivity,alarm_info);
	return alarm_info;
}
#endif
#ifdef HISI_DEV
U16 motioncheck()
{
	static int s_alarm_finfo=0;
	int camno,i,j,start_x=0,start_y=0,end_x=0,end_y=0;
	int left=0,top=0,right=0,bottom=0,motionboxcount=0,alarm_info=0;
	unsigned short* motioninfo[40],*pInfo=NULL;
//	int hcount=(720-352*mmmConfig.reselution[0])/16;
//	int hcount=720/16;
	int hcount=640/16;
	int vcount=0,iTmp;
	unsigned char msg[2];
	float t;
		
	RecamMenu[0].Reselution=1;
	camno=0;
//	for(camno=mmmConfig.ver1;camno<g_CamNum;camno++)
	{
//		motioninfo[camno]=gstCommonParam->GST_DRA_get_motion_buf_hisi(camno);
		if(RecamMenu[0].Reselution)
			motioninfo[camno]=gstCommonParam->GST_DRA_get_motion_buf_hisi(0);
		else
			motioninfo[camno]=gstCommonParam->GST_DRA_get_motion_buf_hisi(0);
		if(motioninfo[camno]!=NULL)
		{
			pInfo = motioninfo[camno];

			left=(MotMenu[0].StarX_H<<8)|MotMenu[0].StarX_L;
			right=(MotMenu[0].EndX_H<<8)|MotMenu[0].EndX_L;
			top=(MotMenu[0].StarY_H<<8)|MotMenu[0].StarY_L;
			bottom=(MotMenu[0].EndY_H<<8)|MotMenu[0].EndY_L;

//			if(RecamMenu[0].Reselution)
//			{
//				left=left*2;
//				right=right*2;
//			}
			t=(float)left*1.8;
			left=(int)t;
			t=(float)right*1.8;
			right=(int)t;
			
			if(left>right)
			{
				i=right;
				right=left;
				left=i;
			}
//			if(RecamMenu[0].Reselution)
//			{
//				top=top*3;
//				bottom=bottom*3;
//			}
			t=(float)top*1.66;
			top=(int)t;
			t=(float)bottom*1.66;
			bottom=(int)t;
			
			if(top>bottom)
			{
				i=bottom;
				bottom=top;
				top=i;
			}		
//			if(SysMenu.VideoFormat==VDO_PAL)
//			{
//				if(top>576)
//					top=576;
//				if(bottom>576)
//					bottom=576;
//				vcount=(576-248*RecamMenu[0].Reselution)/16;
//			}
//			else
//			{
//				if(top>480)
//					top=480;
//				if(bottom>480)
//					bottom=480;
//				vcount=(480-244*RecamMenu[0].Reselution)/16;
//			}

			vcount=480/16;
			
			for(i= 1;i<hcount;i++)
			{
				if (left>=i*16)
					start_x = i;
				if (i*16<=right)
					end_x = i-1;
			}
			for(i= 1;i<vcount;i++)
			{
				if (top>=i*16)
					start_y = i;
				if (i*16<=bottom)
					end_y = i-1;
			}
			for(i= start_y;i<end_y;i++)
			{
				for(j=start_x;j<end_x;j++)
				{
					if (pInfo[i*hcount+j]>40)
						motionboxcount++;
				}
			}
			iTmp=MotMenu[0].Sensitivity;
			//printf("Sensitivity=%d motionboxcount=%d  (%d.%d)->(%d.%d)\n",
			//	iTmp,motionboxcount,start_x,start_y,end_x,end_y);
			if(motionboxcount>=(51-iTmp))
			{
				alarm_info=1;
			}
		}
	}
	return alarm_info;
}
#endif

U16 GetchMotion(U16 sx,U16 ex,U16 sy,U16 ey,U8 ch)
{
	unsigned short* alarm_info1 = NULL;
	int i,j;
//	U8 buf[2];
	U16 Motioninfo = 0,motionboxcount=0;
	U8 hcount,vcount;
	U8 ssx=0,ssy=0,eex=0,eey=0;

	alarm_info1=gstCommonParam->GST_DRA_get_motion_buf_hisi(0);
		hcount = 22*(1+RecamMenu[0].Reselution);
		vcount = (15+SysMenu.VideoFormat*3)*(1+RecamMenu[0].Reselution);
		if (RecamMenu[0].Reselution)
		{
			sx *= 2;
			sy *= 2;
			ex *= 2;
			ey *= 2;
		}
		for (i= 0;i<vcount;i++)
		{
			if (sy>=i*16 && sy <=(i+1)*16)
				ssy = i;
			if (ey>=i*16 && ey <=(i+1)*16)
				eey = i+1;
		}
		for (i= 0;i<hcount;i++)
		{
			if (sx>=i*16 && sx <=(i+1)*16)
				ssx = i;
			if (ex>=i*16 && ex <=(i+1)*16)
				eex = i+1;
		}
		if (alarm_info1 != NULL)
		{
			for (i= ssy;i<eey;i++)
			{
				for (j=ssx;j<eex;j++)
				{
					if (alarm_info1[i*hcount+j]>10)//(11-MotMenu[ch].Sensitivity)*6)
						motionboxcount++;
				}
			}
		}
//		printf("%d,%d\n",ch,motionboxcount);
		if (motionboxcount>=(50-MotMenu[ch].Sensitivity))
		{
			Motioninfo = WBIT(ch);
//			if (ch<2)
//				printf("%d,%d\n",ch,motionboxcount);
		}
//	}
	return Motioninfo;
}

U16 GetMotionInfo(U8 ch)
{
	U16 Motioninfo = 0;
//	Motioninfo = GetchMotion(starx[ch],endx[ch],stary[ch],endy[ch],ch);
	Motioninfo = motioncheck();
//	if (Motioninfo)
//		printf("sx:%d,ex:%d,sy:%d,ey:%d,sen:%d,mo:%d\n",starx[0],endx[0],stary[0],endy[0],MotMenu[ch].Sensitivity,Motioninfo);
	return Motioninfo;
}

U16 alarm_old;
void Motioncheck()
{
	U16 alarm_info1;
	int tmp;
	if (outflag==0)
	{
		outflag = 1;
		alarm_info1 = GetMotionInfo(CamMotion);
	//jlb090820	alarm_info1 = gstCommonParam->GST_DRA_get_alarm_cam_info();
		tmp = alarm_info1 ^ alarm_old;
//		printf("********tmp:%x,alarm_info1:%x\n",tmp,alarm_info1);
		if (tmp && WBIT(CamMotion))
		{
			if (alarm_info1 && WBIT(CamMotion))
				Area_Setup(1);
			else
				Area_Setup(0);
		}
		alarm_old = alarm_info1;
		outflag = 0;
	}
}

void mMOTION_key_func()
{
	if(byKey_val==0 || outflag == 1) return;
	outflag=1;
	printf("mSCHEDUL_key_func\n");
	switch(byKey_val)                   
	{
	case kESC:	//menu        exit
		menu_motion_off();
		break;
	case kUP:
		menu_motion_up();
		break;
	case kDN:
		menu_motion_down();
		break;
	case kRH:
	case kAD:
		menu_motion_add();
		break;
	case kLT:
	case kDE:
		menu_motion_dec();
		break;
	case kET:
		menu_motion_enter();
		break;
	default:
		break;
       }
	byKey_val=0;
	outflag=0;
}


