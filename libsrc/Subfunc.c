#include	"cmn.h"
#include "Splite.h"

U8* White_H=NULL;
U8* White_V=NULL;
U8* Black_H=NULL;
U8* Black_V=NULL;
U8* Gray_H=NULL;
U8* Gray_V=NULL;
U8* Iconhddback=NULL;
U8* Iconhddfore = NULL;
U8* Iriconback = NULL;
U8* Iriconfore = NULL;

U8 diskstatus=0;

extern registerrw_t RegisterReadWrite;

void get_demo_stamp(U8 demo_num1,U8 demo_page1,U8 bcam)   //bcam=1,set buf_cam from eeprom
{															   //bcam=0,don't set buf_cam
#if 0
	U8 i,temp2;
	U8 temp1;
	demo_num_old = demo_num;
	demo_page_old = demo_page;
	demo_num=demo_num1;
	demo_page=demo_page1;
	pic_num=tDEMO_PIC_NUM[demo_num];
 	if (Cam_Num == 8)
 	{
		if (demo_num == L8)
			Reflash_Page = EightCh;
		else if (demo_num == LFULL)
			Reflash_Page = Full1 + demo_page;
		else if(bcam)
		{
			if (Reflash_Page == FourCh1)
				Reflash_Page = FourCh2;
			else
				Reflash_Page = FourCh1;
		}
		else
			Reflash_Page = FourCh1;
 	}
	else if (Cam_Num == 16)
	{
		if (demo_num == L16)
			Reflash_Page = SixtyCh;
		else if (demo_num == L8)
		{
			if(bcam)
			{
				if (Reflash_Page == EightCh)
					Reflash_Page = EightCh1;
				else
					Reflash_Page = EightCh;
			}
			else
				Reflash_Page = EightCh;
		}
		else if (demo_num == LFULL)
			Reflash_Page = Full1 + demo_page;
		else if(demo_num == L4)
		{
			
			if (bcam)
			{
				if (Reflash_Page == FourCh1)
					Reflash_Page = FourCh2;
				else if (Reflash_Page == FourCh2)
					Reflash_Page = FourCh3;
				else if (Reflash_Page == FourCh3)
					Reflash_Page = FourCh4;
				else
					Reflash_Page = FourCh1;
			}
			else
				Reflash_Page = FourCh1;
		}
	}
	else
	{
		if (demo_num == LFULL)
			Reflash_Page = Full1 + demo_page;
		else
			Reflash_Page = FourCh1;
	}
		

	temp1=pic_num*2;
	temp1*=SysMenu.VideoFormat;
	scale_ptr=&tDEMO_PIC_SCALE[demo_num][temp1];
	picpos_ptr=&tDEMO_PIC_POS[demo_num][temp1];

	for(i=0;i<5;i++)
	{
		temp1=pic_num*2;
		temp1*=i;
		temp2=pic_num*2;
		temp2*=5;
		temp2*=SysMenu.VideoFormat;
		temp1+=temp2;
		osd_ptr[i]=&tDEMO_OSD_POS[demo_num][temp1];
		switch (SysMenu.HDOutput)
		{
			case 0:
				hdosd_ptr[i]=&hdtDEMO_OSD_POS[demo_num][temp1];
				break;
			case 1:
				hdosd_ptr[i]=&VGA2tDEMO_OSD_POS[demo_num][temp1];
				break;
			case 2:
				hdosd_ptr[i]=&pbprtDEMO_OSD_POS[demo_num][temp1];
				break;
			case 3:
				hdosd_ptr[i]=&pbprtDEMO_OSD_POS[demo_num][temp1];
				break;
		}

	}

	if(Reflash_Page == FourCh1)
	{
		for (i=0;i<4;i++)
			buf_cam[i]=i;
	}
	else if (Reflash_Page == FourCh2)
	{
		for (i=0;i<4;i++)
			buf_cam[i]=i+4;
	}
	else if (Reflash_Page == EightCh)
	{
		if (Cam_Num>CHANNEL8)
		{
			for (i=0;i<9;i++)
				buf_cam[i]=i;
		}
		else
		{
			for (i=0;i<8;i++)
				buf_cam[i]=i;
		}
	}
	else if (Reflash_Page == FourCh3)
	{
		for (i=0;i<4;i++)
			buf_cam[i]=i+8;
	}
	else if (Reflash_Page == FourCh4)
	{
		for (i=0;i<4;i++)
			buf_cam[i]=i+12;
	}
	else if (Reflash_Page == EightCh1)
	{
		for (i=0;i<9;i++)
			buf_cam[i]=i+7;
	}
	else if (Reflash_Page == SixtyCh)
	{
		for (i=0;i<16;i++)
			buf_cam[i]=i;
	}
	else
		buf_cam[0]=demo_page;
	printf("get_demo_stamp : Reflash_Page = %x\n",Reflash_Page);
#endif
}
void write_dec(unsigned char address,unsigned char databuf)
{
	RegisterReadWrite.addr=address;
	RegisterReadWrite.value=databuf;
	if(g_sensor_id>0) ioctl(g_sensor_id, 0, &RegisterReadWrite);
	if(tw2865id>0)	ioctl(tw2865id, 6, &RegisterReadWrite);
}
void autochnamedisp(U8 ch)
{
#if 0
	U16 osd_posptr[2];
	U16 hdosd_posptr[2];
	U8 strbuf[6];
	U16 wtemp = WBIT(ch);
	BXS_PIC1 * bxs_pic = NULL;
	BXS_PIC1 * bxs_pic1 = NULL;
	osd_posptr[0] = tDEMO_OSD_POS[0][SysMenu.VideoFormat*10];
	osd_posptr[1] = tDEMO_OSD_POS[0][SysMenu.VideoFormat*10+1];
	hdosd_posptr[0] = hdtDEMO_OSD_POS[0][SysMenu.VideoFormat*10];
	hdosd_posptr[1] = hdtDEMO_OSD_POS[0][SysMenu.VideoFormat*10+1];
	memset(strbuf,0x00,sizeof(strbuf));
	memcpy(strbuf,CamMenu[ch].CamTitle,5);
	bxs_pic1 = getyuvfile(mousemenupic.recordicoground,"mousemenupic.recordicoground");
	Bmp_All_Show(osd_posptr[0], osd_posptr[1]-bxs_pic1->pic_height, mousemenupic.recordicoground,AD);
	Bmp_All_Show(hdosd_posptr[0], hdosd_posptr[1]-bxs_pic1->pic_height, mousemenupic.recordicoground,HD);
	if (CamMenu[ch].InputMatch)
	{
		bxs_pic = getyuvfile(cammenupic.camnamelocal,"cammenupic.camnamelocal");
		Bmp_Part_Show(osd_posptr[0]+10,osd_posptr[1]+1-bxs_pic1->pic_height, 0,ch*bxs_pic->pic_height/16,bxs_pic->pic_width,bxs_pic->pic_height/16,cammenupic.camnamelocal,AD);
		Bmp_Part_Show(hdosd_posptr[0]+10,hdosd_posptr[1]+1-bxs_pic1->pic_height, 0,ch*bxs_pic->pic_height/16,bxs_pic->pic_width,bxs_pic->pic_height/16,cammenupic.camnamelocal,HD);
	}
	else
	{
		Osd_line_display(osd_posptr[0]+10,osd_posptr[1]+1-bxs_pic1->pic_height,WHITE,strbuf,0xff,AD);
		Osd_line_display(hdosd_posptr[0]+10,hdosd_posptr[1]+1-bxs_pic1->pic_height,WHITE,strbuf,0xff,HD);
	}
	bxs_pic = getyuvfile(recordico,"recordico");
	bxs_pic1 = getyuvfile(mousemenupic.recordicoground,"mousemenupic.recordicoground");
	if (wtemp & cam_loss)
	{
		Bmp_Part_Show(osd_posptr[0]+bxs_pic1->pic_width-10-bxs_pic->pic_width,osd_posptr[1]+1-bxs_pic1->pic_height, 0, bxs_pic->pic_height*7/8, bxs_pic->pic_width, bxs_pic->pic_height/8, recordico,AD);
		Bmp_Part_Show(hdosd_posptr[0]+bxs_pic1->pic_width-10-bxs_pic->pic_width,hdosd_posptr[1]+1-bxs_pic1->pic_height, 0, bxs_pic->pic_height*7/8, bxs_pic->pic_width, bxs_pic->pic_height/8, recordico,HD);
	}
	else
	{
		Bmp_Part_Show(osd_posptr[0]+bxs_pic1->pic_width-10-bxs_pic->pic_width,osd_posptr[1]+1-bxs_pic1->pic_height, 0, bxs_pic->pic_height*6/8, bxs_pic->pic_width, bxs_pic->pic_height/8, recordico,AD);
		Bmp_Part_Show(hdosd_posptr[0]+bxs_pic1->pic_width-10-bxs_pic->pic_width,hdosd_posptr[1]+1-bxs_pic1->pic_height, 0, bxs_pic->pic_height*6/8, bxs_pic->pic_width, bxs_pic->pic_height/8, recordico,HD);
	}
#endif 
}


void set_full_chinnal(U8 x,U8 y,U16 x1,U16 y1,U8 ch)
{
//	printf("x:%d,y:%d,x1:%d,y1:%d,w:%d,h:%d,ch:%d\n",(x/8)*8,(y/8)*8,(x1/8)*8,(y1/8)*8,352,High_Resolution/2,ch);
	if (ChipNum == MASTER)
		gstCommonParam->GST_DRA_Hisi_window_vo(1,ch,(x/8)*8,(y/8)*8,352,High_Resolution/2);
/*
	U8 i;
	gstCommonParam->GST_DRA_set_live_pause(1);
	for (i=0;i<Cam_Num;i++)
	{
		gstCommonParam->GST_DRA_set_live_chan_stats(i,0);
	}
	gstCommonParam->GST_DRA_set_single_cam_pos(ch,x,y,WIDTH_RESOLUTION/2,High_Resolution/2);
	gstCommonParam->GST_DRA_set_live_chan_stats(ch,1);
	gstCommonParam->GST_DRA_set_live_pause(0);
*/
}

void live_demo_set()
{
//	if (ChipNum == MASTER)
//		gstCommonParam->GST_DRA_Hisi_window_vo(0,0,0,0,0,0,0,0,0,0);
/*
	U8 i;
	gstCommonParam->GST_DRA_set_live_pause(1);
	for (i=0;i<Cam_Num;i++)
	{
		gstCommonParam->GST_DRA_set_live_chan_stats(i,0);
	}
	
	for (i=0;i<pic_num;i++)
	{
		gstCommonParam->GST_DRA_set_single_cam_pos(buf_cam[i],picpos_ptr[i*2],picpos_ptr[i*2+1],scale_ptr[i*2],scale_ptr[i*2+1]);
	}
	
	for (i=0;i<pic_num;i++)
	{
		gstCommonParam->GST_DRA_set_live_chan_stats(buf_cam[i],1);
	}
	gstCommonParam->GST_DRA_set_live_pause(0);
*/
}

/*
void live_demo_tset()
{
	gstCommonParam->GST_DRA_Hisi_window_vo(pic_num,buf_cam[0],0,0,0,0);
	U8 i;
	gstCommonParam->GST_DRA_set_live_pause(1);
	for (i=0;i<Cam_Num;i++)
	{
		gstCommonParam->GST_DRA_set_live_chan_stats(i,0);
	}
	
	gstCommonParam->GST_DRA_set_single_cam_pos(0,0,0,360,240);
	gstCommonParam->GST_DRA_set_single_cam_pos(1,360,0,360,240);
	gstCommonParam->GST_DRA_set_single_cam_pos(2,0,240,360,240);
	gstCommonParam->GST_DRA_set_single_cam_pos(3,360,240,360,240);
	
	for (i=0;i<Cam_Num;i++)
	{
		gstCommonParam->GST_DRA_set_live_chan_stats(i,1);
	}
	gstCommonParam->GST_DRA_set_live_pause(0);
}
*/

UC valid_name_disp()
{
	if(menu_num)
	{
		if (menu_num == mMouse && demo_num<PBFULL)
			return 1;
		else
			return 0;
	}

	return 1;
}

UC valid_time_disp()
{
	if((menu_num>0 && menu_num < Iconmenu && menu_num != mMouse) || PlayStatusFlag.PlayStatus  || demo_num > L16)//|| bshort_flag || speed_flag)
	{
		return 0;
	}
	return 1;
}

void border_set(U8 demo)
{
#if 0
	if (SysMenu.bordcolor==1)
	{
		Bmp_All_Show(0,0, Gray_H,AD);
		Bmp_All_Show(0,High_Resolution-2, Gray_H,AD);
		Bmp_All_Show(0,0, Gray_V,AD);
		Bmp_All_Show(WIDTH_RESOLUTION-2,0, Gray_V,AD);
		
		Bmp_All_Show(0,0, Gray_H,HD);
		Bmp_All_Show(0,HIGH_RESOLUTION_HD-2, Gray_H,HD);
		Bmp_All_Show(0,0, Gray_V,HD);
		Bmp_All_Show(WIDTH_RESOLUTION_HD-2,0, Gray_V,HD);
		switch(demo)
		{
			case LFULL:
				break;
			case L4:
				Bmp_All_Show(0,(High_Resolution-4)/2, Gray_H,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)/2,0, Gray_V,AD);
				
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)/2, Gray_H,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)/2,0, Gray_V,HD);
				break;
			case L8:
				Bmp_All_Show(0,(High_Resolution-4)/3, Gray_H,AD);
				Bmp_All_Show(0,(High_Resolution-4)*2/3, Gray_H,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)/3,0, Gray_V,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)*2/3,0, Gray_V,AD);
				
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)/3, Gray_H,HD);
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)*2/3, Gray_H,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)/3,0, Gray_V,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)*2/3,0, Gray_V,HD);
				break;
			case L16:
				Bmp_All_Show(0,(High_Resolution-4)/4, Gray_H,AD);
				Bmp_All_Show(0,(High_Resolution-4)/2, Gray_H,AD);
				Bmp_All_Show(0,(High_Resolution-4)*3/4, Gray_H,AD);
//				Bmp_All_Show(184,0, Gray_V,AD);
//				Bmp_All_Show(360,0, Gray_V,AD);
//				Bmp_All_Show(536,0, Gray_V,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)/4,0, Gray_V,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)/2,0, Gray_V,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)*3/4,0, Gray_V,AD);
				
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)/4, Gray_H,HD);
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)/2, Gray_H,HD);
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)*3/4, Gray_H,HD);
//				Bmp_All_Show(320,0, Gray_V,HD);
//				Bmp_All_Show(640,0, Gray_V,HD);
//				Bmp_All_Show(960,0, Gray_V,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)/4,0, Gray_V,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)/2,0, Gray_V,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)*3/4,0, Gray_V,HD);
				break;
			default:
				break;
		}
	}
	else if (SysMenu.bordcolor==2)	
	{
		Bmp_All_Show(0,0, White_H,AD);
		Bmp_All_Show(0,High_Resolution-2, White_H,AD);
		Bmp_All_Show(0,0, White_V,AD);
		Bmp_All_Show(WIDTH_RESOLUTION-2,0, White_V,AD);
		
		Bmp_All_Show(0,0, White_H,HD);
		Bmp_All_Show(0,HIGH_RESOLUTION_HD-2, White_H,HD);
		Bmp_All_Show(0,0, White_V,HD);
		Bmp_All_Show(WIDTH_RESOLUTION_HD-2,0, White_V,HD);
		switch(demo)
		{
			case LFULL:
				break;
			case L4:
				Bmp_All_Show((WIDTH_RESOLUTION-2)/2,0, White_V,AD);
				Bmp_All_Show(0,(High_Resolution-4)/2, White_H,AD);
				
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)/2,0, White_V,HD);
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)/2, White_H,HD);
				break;
			case L8:
				Bmp_All_Show(0,(High_Resolution-4)/3, White_H,AD);
				Bmp_All_Show(0,(High_Resolution-4)*2/3, White_H,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)/3,0, White_V,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)*2/3,0, White_V,AD);
				
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)/3, White_H,HD);
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)*2/3, White_H,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)/3,0, White_V,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)*2/3,0, White_V,HD);
				break;
			case L16:
				Bmp_All_Show(0,(High_Resolution-4)/4, White_H,AD);
				Bmp_All_Show(0,(High_Resolution-4)/2, White_H,AD);
				Bmp_All_Show(0,(High_Resolution-4)*3/4, White_H,AD);
//				Bmp_All_Show(184,0, White_V,AD);
//				Bmp_All_Show(360,0, White_V,AD);
//				Bmp_All_Show(536,0, White_V,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)/4,0, White_V,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)/2,0, White_V,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)*3/4,0, White_V,AD);
				
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)/4, White_H,HD);
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)/2, White_H,HD);
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)*3/4, White_H,HD);
//				Bmp_All_Show(320,0, White_V,HD);
//				Bmp_All_Show(640,0, White_V,HD);
//				Bmp_All_Show(960,0, White_V,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)/4,0, White_V,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)/2,0, White_V,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)*3/4,0, White_V,HD);
				break;
			default:
				break;
		}
	}
	else if (SysMenu.bordcolor==3)
	{
		Bmp_All_Show(0,0, Black_H,AD);
		Bmp_All_Show(0,High_Resolution-2, Black_H,AD);
		Bmp_All_Show(0,0, Black_V,AD);
		Bmp_All_Show(WIDTH_RESOLUTION-2,0, Black_V,AD);
		
		Bmp_All_Show(0,0, Black_H,HD);
		Bmp_All_Show(0,HIGH_RESOLUTION_HD-2, Black_H,HD);
		Bmp_All_Show(0,0, Black_V,HD);
		Bmp_All_Show(WIDTH_RESOLUTION_HD-2,0, Black_V,HD);
		switch(demo)
		{
			case LFULL:
				break;
			case L4:
				Bmp_All_Show(0,(High_Resolution-4)/2, Black_H,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)/2,0, Black_V,AD);
				
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)/2, Black_H,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)/2,0, Black_V,HD);
				break;
			case L8:
				Bmp_All_Show(0,(High_Resolution-4)/3, Black_H,AD);
				Bmp_All_Show(0,(High_Resolution-4)*2/3, Black_H,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)/3,0, Black_V,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)*2/3,0, Black_V,AD);
				
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)/3, Black_H,HD);
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)*2/3, Black_H,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)/3,0, Black_V,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)*2/3,0, Black_V,HD);
				break;
			case L16:
				Bmp_All_Show(0,(High_Resolution-4)/4, Black_H,AD);
				Bmp_All_Show(0,(High_Resolution-4)/2, Black_H,AD);
				Bmp_All_Show(0,(High_Resolution-4)*3/4, Black_H,AD);
//				Bmp_All_Show(184,0, Black_V,AD);
//				Bmp_All_Show(360,0, Black_V,AD);
//				Bmp_All_Show(536,0, Black_V,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)/4,0, Black_V,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)/2,0, Black_V,AD);
				Bmp_All_Show((WIDTH_RESOLUTION-2)*3/4,0, Black_V,AD);
				
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)/4, Black_H,HD);
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)/2, Black_H,HD);
				Bmp_All_Show(0,(HIGH_RESOLUTION_HD-4)*3/4, Black_H,HD);
//				Bmp_All_Show(320,0, Black_V,HD);
//				Bmp_All_Show(640,0, Black_V,HD);
//				Bmp_All_Show(960,0, Black_V,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)/4,0, Black_V,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)/2,0, Black_V,HD);
				Bmp_All_Show((WIDTH_RESOLUTION_HD-2)*3/4,0, Black_V,HD);
				break;
			default:
				break;
		}
	}
	Time_Date_Disp();
#endif	
}

void demo_set()   
{				 
	live_demo_set();
}


void Brightness_Ajust(U8 ch,U8 data)
{
	int val=data;
	
  if(flag_480p)
  {
//		RegisterReadWrite.addr=0x9b;
//		if(data<=50)
//			RegisterReadWrite.value=0;
//		else
//			RegisterReadWrite.value=data;
//		ioctl(g_sensor_id, 0, &RegisterReadWrite); 
		RegisterReadWrite.addr=0x9b;
		if(data==50)
			RegisterReadWrite.value=8;
		else if(data<50)
			RegisterReadWrite.value=8+(50-data);
		else 
			RegisterReadWrite.value=8+(data-50);
		if(g_sensor_id>0) ioctl(g_sensor_id, 0, &RegisterReadWrite);  
		if(data>=50)
			RegisterReadWrite.value=0x06;
		else
			RegisterReadWrite.value=0x0e;
		RegisterReadWrite.addr=0xab;
		if(g_sensor_id>0) ioctl(g_sensor_id, 0, &RegisterReadWrite); 		 
  }
  else if(flag_576p)
  {
		if(data==50)
			val=0;
		else if(data>50)
			val=(data-50)*2;
		else if(data<50)
			val=255-(50-data)*2;
		write_dec(0x10,val);
		printf("brightess write value is %d\n",val);
  }
  else
  {
//			return ;
			
  	if(data==50)
  	{
	  	IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
			IPCAM_ISP_ST *pIsp_st=&pAllInfo->isp_st;
  		val=pIsp_st->u32LumaVal;
  	}
  	else
  		val=data;
  	
  	if(val!=stCscAttr.u32LumaVal)
		{
			stCscAttr.u32LumaVal=val;
			HI_MPI_VI_SetCSCAttr(0, &stCscAttr);
		}
  }
	printf("brightess write value is %d\n",val);
}

void Hue_Ajust(U8 ch,U8 data)
{
  int val=data;
  
  if(flag_480p)
  {
  }
  else if(flag_576p)
  {
		if(data==50)
			val=0;
		else if(data>50)
			val=(data-50)*2;
		else if(data<50)
			val=255-(50-data)*2;
		write_dec(0x15,val);
		printf("Hue value is %d\n",val);
  }
  else
  {
//			return ;
			
  	if(data==50)
  	{
	  	IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
			IPCAM_ISP_ST *pIsp_st=&pAllInfo->isp_st;
  		val=pIsp_st->u32HueVal;
  	}
  	else
  		val=data;
  	if(val!=stCscAttr.u32HueVal)
		{
			stCscAttr.u32HueVal=val;
			HI_MPI_VI_SetCSCAttr(0, &stCscAttr);
		}
  }
	printf("Hue value is %d\n",val);
}

void Contrast_Ajust(U8 ch,U8 data)
{
  int val=data;
  if(flag_480p)
  {
//		RegisterReadWrite.addr=0x9c;
//		if(data<=50)
//			RegisterReadWrite.value=0x20;
//		else
//			RegisterReadWrite.value=data;
//		ioctl(g_sensor_id, 0, &RegisterReadWrite);  
		RegisterReadWrite.addr=0x9c;
		if(data==50)
			RegisterReadWrite.value=0x20;
		else if(data<50)
			RegisterReadWrite.value=0x20-(50-data);
		else 
			RegisterReadWrite.value=0x20+(data-50);
		if(g_sensor_id>0) ioctl(g_sensor_id, 0, &RegisterReadWrite); 
  }
  else if(flag_576p)
  {
		if(data==50)
			val=0x64;
		else if(data>50)
			val=0x64+(data-50)*2;
		else if(data<50)
			val=0x64-(50-data)*2;
		write_dec(0x11,val);
		printf("Contrast value is %d\n",val);
  }
  else
  {
//			return ;
			
  	if(data==50)
  	{
	  	IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
			IPCAM_ISP_ST *pIsp_st=&pAllInfo->isp_st;
  		val=pIsp_st->u32ContrVal;
  	}
  	else
  		val=data;
  	if(val!=stCscAttr.u32ContrVal)
		{
			stCscAttr.u32ContrVal=val;
			HI_MPI_VI_SetCSCAttr(0, &stCscAttr);
		}
  }
	printf("Contrast value is %d\n",val);
}
extern unsigned char stauration_val;
extern unsigned char isday;
void Stauration_Ajust(U8 ch,U8 data)
{
  int val=data;
  
  stauration_val=val;
  
  if(isday==NIGHT)
  	return;
  	
  if(flag_480p)
  {
		RegisterReadWrite.addr=0xa7;
		if(data==50)
			RegisterReadWrite.value=0x40;
		else if(data<50)
			RegisterReadWrite.value=0x40-(50-data);
		else 
			RegisterReadWrite.value=0x40+(data-50);
		if(g_sensor_id>0) ioctl(g_sensor_id, 0, &RegisterReadWrite);			
		RegisterReadWrite.addr=0xa8;
		if(data==50)
			RegisterReadWrite.value=0x40;
		else if(data<50)
			RegisterReadWrite.value=0x40-(50-data);
		else 
			RegisterReadWrite.value=0x40+(data-50);
		if(g_sensor_id>0) ioctl(g_sensor_id, 0, &RegisterReadWrite);	
  }
  else if(flag_576p)
  {
		if(data==50)
			val=0x80;
		else if(data>50)
			val=(data-50)*2;
		else if(data<50)
			val=255-(50-data)*2;
		write_dec(0x13,val);
		write_dec(0x14,val);
		printf("Stauration value is %d\n",val);
  }
  else
  {
//			return ;
			
  	if(data==50)
  	{
	  	IPCAM_ALL_INFO_ST * pAllInfo = gstCommonParam->GST_DRA_Hisi_get_ipcam_config_st();
			IPCAM_ISP_ST *pIsp_st=&pAllInfo->isp_st;
  		val=pIsp_st->u32SatuVal;
  	}
  	else
  		val=data;
  	if(val!=stCscAttr.u32SatuVal)
	{
		stCscAttr.u32SatuVal=val;
		HI_MPI_VI_SetCSCAttr(0, &stCscAttr);
	}
  }
	printf("Stauration value is %d\n",val);
}

void Hdelay_Ajust(U8 ch,U8 data)
{
}

void Vdelay_Ajust(U8 ch,U8 data)
{
}
//OCW-T400TG-1AJF

void remote_state_save()
{
}
void event_write(int event_no,int cam_no,U8 num)   //cam_no start from 0
{
}


U8 valid_ms_disp()
{
	if(demo_num>=PBFULL || menu_num)
		return 0;

	return 1;
}

U8 valid_loss_disp()
{
	if((demo_num>=PBFULL && demo_num != RECORD) || menu_num)
		return 0;

	return 1;
}


U8 valid_dispmode_change()
{
	if(menu_num || demo_num>=PBFULL || zoom_flag) 
		return 0;
	
	return 1;
}

U8 valid_reauto()
{
	if(menu_num==0 && auto_flag)
		return 1;

	return 0;
}

U8 valid_camno(U8 camno)
{
	U8 i;
	U8 chnum;
	chnum=pic_num;
	if (pic_num>Cam_Num)
		chnum=Cam_Num;
	for(i=0;i<chnum;i++)
	{
		if(camno==buf_cam[i])
			return 1;
	}
	return 0;
}


U8 valid_rec_disp()
{
	if( (demo_num == PB9) || (menu_num <mPTZControl && menu_num>0))
	{
//		printf("AAAAAAAAA menu_num = %d\n",menu_num);
		return 0;
	}
//	printf("BBBBBBBBBBBBBB menu_num = %d\n",menu_num);
	return 1;
}

void loss_disp(unsigned char channel,unsigned char onoff)  
{
#if 0
///////////please add code
	U8 i;
	BXS_PIC1* picbuf = getyuvfile(recordico,"recordico");
	BXS_PIC1 * bxs_pic1 = getyuvfile(mousemenupic.recordicoground,"mousemenupic.recordicoground");
	U8 chnum;
	chnum=pic_num;
	if (pic_num>Cam_Num)
		chnum=Cam_Num;
	if ((demo_num == PB9) ||  (menu_num <mPTZControl && menu_num>0))
		return;
	for(i=0;i<chnum;i++)
	{
		if (channel == buf_cam[i])
		{
			if (!rec_on)
			{
				if (onoff)
				{
					Bmp_Part_Show(osd_ptr[0][i*2]+bxs_pic1->pic_width-10-picbuf->pic_width,osd_ptr[0][i*2+1]+1-bxs_pic1->pic_height, 0, picbuf->pic_height*7/8, picbuf->pic_width, picbuf->pic_height/8, recordico,AD);
					Bmp_Part_Show(hdosd_ptr[0][i*2]+bxs_pic1->pic_width-10-picbuf->pic_width,hdosd_ptr[0][i*2+1]+1-bxs_pic1->pic_height, 0, picbuf->pic_height*7/8, picbuf->pic_width, picbuf->pic_height/8, recordico,HD);
				}
				else
				{
					Bmp_Part_Show(osd_ptr[0][i*2]+bxs_pic1->pic_width-10-picbuf->pic_width,osd_ptr[0][i*2+1]+1-bxs_pic1->pic_height, 0, picbuf->pic_height*6/8, picbuf->pic_width, picbuf->pic_height/8, recordico,AD);
					Bmp_Part_Show(hdosd_ptr[0][i*2]+bxs_pic1->pic_width-10-picbuf->pic_width,hdosd_ptr[0][i*2+1]+1-bxs_pic1->pic_height, 0, picbuf->pic_height*6/8, picbuf->pic_width, picbuf->pic_height/8, recordico,HD);
				}
				break;
			}
		}
	}
#endif
}

int alarmch=0;

void alarmboardshow(U8 color,U8 ch,U8 flag)
{
#if 0
	U16 i,tmp;
	U8 pos,posy,offx;
	U8 l,r,t,b;
	U8* hline;
	U8* vline;
//	U8 withbuf[2]={180,180};
//	U16 posbuf[5] = {0,180,360,540,718};
//	U16 withbufhd[2]={324,316};
//	U16 posbufhd[5] = {0,324,640,956,1274};
	BXS_PIC1* picptrh = getyuvfile( Gray_H,"Gray_H");
	BXS_PIC1* picptrV = getyuvfile( Gray_V,"Gray_V");
	tmp = WBIT(ch) & alarmch;
//	printf("############: %d\n",color);
	l=r=t=b=0xff;
	if(SysMenu.bordcolor==1)
	{
		hline = Gray_H;
		vline = Gray_V;
	}
	else if (SysMenu.bordcolor==2)
	{
		hline = White_H;
		vline = White_V;
	}
	else
	{
		hline = Black_H;
		vline = Black_V;
	}
	if (tmp && color>1)
		color = RED;

	switch(demo_num)
	{
		case LFULL:
			if (color>1 )
			{
				if (SysMenu.bordcolor)
				{
					Bmp_All_Show(0,0, hline,AD);
					Bmp_All_Show(0,High_Resolution-2, hline,AD);
					Bmp_All_Show(0,0, vline,AD);
					Bmp_All_Show(WIDTH_RESOLUTION-8,0, vline,AD);
					
					Bmp_All_Show(0,0, hline,HD);
					Bmp_All_Show(0,HIGH_RESOLUTION_HD-2, hline,HD);
					Bmp_All_Show(0,0, vline,HD);
					Bmp_All_Show(WIDTH_RESOLUTION_HD-8,0, vline,HD);
				}
				else
				{
					clear_osd(0, 0,picptrV->pic_width, High_Resolution-2,AD);
					clear_osd(0, 0, WIDTH_RESOLUTION-8, picptrh->pic_height,AD);
					clear_osd(0, High_Resolution-2, WIDTH_RESOLUTION-8, picptrh->pic_height,AD);
					clear_osd(WIDTH_RESOLUTION-8, 0, picptrV->pic_width, High_Resolution-2,AD);
					
					clear_osd(0, 0,picptrV->pic_width, HIGH_RESOLUTION_HD-2,HD);
					clear_osd(0, 0, WIDTH_RESOLUTION_HD-8, picptrh->pic_height,HD);
					clear_osd(0, HIGH_RESOLUTION_HD-2, WIDTH_RESOLUTION_HD-8, picptrh->pic_height,HD);
					clear_osd(WIDTH_RESOLUTION_HD-8, 0, picptrV->pic_width, HIGH_RESOLUTION_HD-2,HD);
				}
			}
			else
			{
				for (i=0;i<(WIDTH_RESOLUTION-20);i++)
				{
					osd_bmp_photo(motmenupic.mix,i, 0,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
					osd_bmp_photo(motmenupic.mix,i, High_Resolution-4,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
				for (i=0;i<(High_Resolution-20);i++)
				{
					osd_bmp_photo(motmenupic.mix,0, i,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
					osd_bmp_photo(motmenupic.mix,WIDTH_RESOLUTION-6, i,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
				
				for (i=0;i<(WIDTH_RESOLUTION_HD-20);i++)
				{
					osd_bmp_photo(motmenupic.mix,i, 0,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
					osd_bmp_photo(motmenupic.mix,i, HIGH_RESOLUTION_HD-4,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
				for (i=0;i<(HIGH_RESOLUTION_HD-20);i++)
				{
					osd_bmp_photo(motmenupic.mix,0, i,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
					osd_bmp_photo(motmenupic.mix,WIDTH_RESOLUTION_HD-6, i,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
			}
			break;
		case L4:
			pos = ch%2;
			posy = ch/2;
			if (color>1 )
			{
				if(pos)
					l=ch-1;
				if (!pos)
					r=ch+1;
				if (posy)
					t=ch-2;
				if(!posy)
					b=ch+2;
				if (SysMenu.bordcolor)
				{
					if (t!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch-2)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(t) & alarmch;
						}
						else
						{
							tmp = WBIT(t) & alarmch;
							if (tmp)
							{
								for (i=0;i<(WIDTH_RESOLUTION-6)/2;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*posy/2,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(WIDTH_RESOLUTION_HD-6)/2;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*posy/2,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						Bmp_Part_Show((WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*posy/2, 0, 0, (WIDTH_RESOLUTION-6)/2, picptrh->pic_height, hline,AD);
						Bmp_Part_Show((WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*posy/2, 0, 0, (WIDTH_RESOLUTION_HD-6)/2, picptrh->pic_height, hline,HD);
					}
					if (b!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch+2)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(b) & alarmch;
						}
						else
						{
							tmp = WBIT(b) & alarmch;
							if (tmp)
							{
								for (i=0;i<(WIDTH_RESOLUTION-6)/2;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*(posy+1)/2,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(WIDTH_RESOLUTION_HD-6)/2;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*(posy+1)/2,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						Bmp_Part_Show((WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*(posy+1)/2, 0, 0, (WIDTH_RESOLUTION-6)/2, picptrh->pic_height, hline,AD);
						Bmp_Part_Show((WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*(posy+1)/2, 0, 0, (WIDTH_RESOLUTION_HD-6)/2, picptrh->pic_height, hline,HD);
					}
					if (l!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch-1)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(l) & alarmch;
						}
						else
						{
							tmp = WBIT(l) & alarmch;
							if (tmp)
							{
								for (i=0;i<(High_Resolution-4)/2;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*pos/2,i+(High_Resolution-4)*posy/2,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(HIGH_RESOLUTION_HD-4)/2;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*pos/2,i+(HIGH_RESOLUTION_HD-4)*posy/2,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						Bmp_Part_Show((WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*posy/2, 0, 0, picptrV->pic_width, (High_Resolution-4)/2, vline,AD);
						Bmp_Part_Show((WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*posy/2, 0, 0, picptrV->pic_width, (HIGH_RESOLUTION_HD-4)/2, vline,HD);
					}
					if (r!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch+1)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(r) & alarmch;
						}
						else
						{
							tmp = WBIT(r) & alarmch;
							if (tmp)
							{
								for (i=0;i<(High_Resolution-4)/2;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*(pos+1)/2,i+(High_Resolution-4)*posy/2,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(HIGH_RESOLUTION_HD-4)/2;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*(pos+1)/2,i+(HIGH_RESOLUTION_HD-4)*posy/2,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						Bmp_Part_Show((WIDTH_RESOLUTION-6)*(pos+1)/2, (High_Resolution-4)*posy/2, 0, 0, picptrV->pic_width, (High_Resolution-4)/2, vline,AD);
						Bmp_Part_Show((WIDTH_RESOLUTION_HD-6)*(pos+1)/2, (HIGH_RESOLUTION_HD-4)*posy/2, 0, 0, picptrV->pic_width, (HIGH_RESOLUTION_HD-4)/2, vline,HD);
					}
				}
				else
				{
					if (t!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch-2)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(t) & alarmch;
						}
						else
						{
							tmp = WBIT(t) & alarmch;
							if (tmp)
							{
								for (i=0;i<(WIDTH_RESOLUTION-6)/2;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*posy/2,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(WIDTH_RESOLUTION_HD-6)/2;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*posy/2,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else 
						tmp = 0;
					if (!tmp)
					{
						clear_osd((WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*posy/2, (WIDTH_RESOLUTION-6)/2, picptrh->pic_height,AD);
						clear_osd((WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*posy/2, (WIDTH_RESOLUTION_HD-6)/2, picptrh->pic_height,HD);
					}
					if (b!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch+2)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(b) & alarmch;
						}
						else
						{
							tmp = WBIT(b) & alarmch;
							if (tmp)
							{
								for (i=0;i<(WIDTH_RESOLUTION-6)/2;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*(posy+1)/2,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(WIDTH_RESOLUTION_HD-6)/2;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*(posy+1)/2,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						clear_osd((WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*(posy+1)/2, (WIDTH_RESOLUTION-6)/2, picptrh->pic_height,AD);
						clear_osd((WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*(posy+1)/2, (WIDTH_RESOLUTION_HD-6)/2, picptrh->pic_height,HD);
					}
					if (l!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch-1)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(l) & alarmch;
						}
						else
						{
							tmp = WBIT(l) & alarmch;
							if (tmp)
							{
								for (i=0;i<(High_Resolution-4)/2;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*pos/2,i+(High_Resolution-4)*posy/2,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(HIGH_RESOLUTION_HD-4)/2;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*pos/2,i+(HIGH_RESOLUTION_HD-4)*posy/2,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						clear_osd((WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*posy/2, picptrV->pic_width, (High_Resolution-4)/2,AD);
						clear_osd((WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*posy/2, picptrV->pic_width, (HIGH_RESOLUTION_HD-4)/2,HD);
					}
					if (r!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch+1)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(r) & alarmch;
						}
						else
						{
							tmp = WBIT(r) & alarmch;
							if (tmp)
							{
								for (i=0;i<(High_Resolution-4)/2;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*(pos+1)/2,i+(High_Resolution-4)*posy/2,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(HIGH_RESOLUTION_HD-4)/2;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*(pos+1)/2,i+(HIGH_RESOLUTION_HD-4)*posy/2,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						clear_osd((WIDTH_RESOLUTION-6)*(pos+1)/2, (High_Resolution-4)*posy/2, picptrV->pic_width, (High_Resolution-4)/2,AD);
						clear_osd((WIDTH_RESOLUTION_HD-6)*(pos+1)/2, (HIGH_RESOLUTION_HD-4)*posy/2, picptrV->pic_width, (HIGH_RESOLUTION_HD-4)/2,HD);
					}
				}
			}
			else 
			{
				for (i=0;i<(WIDTH_RESOLUTION-6)/2;i++)
				{
					if ((Cur_CH == (ch-2)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*posy/2,AD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*posy/2,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
					if ((Cur_CH == (ch+2)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*(posy+1)/2,AD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/2, (High_Resolution-4)*(posy+1)/2,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
				for (i=0;i<(WIDTH_RESOLUTION_HD-6)/2;i++)
				{
					if ((Cur_CH == (ch-2)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*posy/2,HD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*posy/2,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
					if ((Cur_CH == (ch+2)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*(posy+1)/2,HD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/2, (HIGH_RESOLUTION_HD-4)*(posy+1)/2,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
				for (i=0;i<(High_Resolution-4)/2;i++)
				{
					if ((Cur_CH == (ch-1)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*pos/2,i+(High_Resolution-4)*posy/2,AD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*pos/2,i+(High_Resolution-4)*posy/2,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
					if ((Cur_CH == (ch+1)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*(pos+1)/2,i+(High_Resolution-4)*posy/2,AD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*(pos+1)/2,i+(High_Resolution-4)*posy/2,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
				for (i=0;i<(HIGH_RESOLUTION_HD-4)/2;i++)
				{
					if ((Cur_CH == (ch-1)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*pos/2,i+(HIGH_RESOLUTION_HD-4)*posy/2,HD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*pos/2,i+(HIGH_RESOLUTION_HD-4)*posy/2,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
					if ((Cur_CH == (ch+1)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*(pos+1)/2,i+(HIGH_RESOLUTION_HD-4)*posy/2,HD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*(pos+1)/2,i+(HIGH_RESOLUTION_HD-4)*posy/2,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
			}
			break;
		case L8:
			pos = ch%3;
			posy = ch/3;
			if (color>1 )
			{
				if(pos)
					l=ch-1;
				if (pos<2)
					r=ch+1;
				if (posy)
					t=ch-3;
				if(posy<2)
					b=ch+3;
				if (SysMenu.bordcolor)
				{
					if (t!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch-3)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(t) & alarmch;
						}
						else
						{
							tmp = WBIT(t) & alarmch;
							if (tmp)
							{
								for (i=0;i<(WIDTH_RESOLUTION-6)/3;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*posy/3,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(WIDTH_RESOLUTION_HD-6)/3;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*posy/3,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else 
						tmp = 0;
					if (!tmp)
					{
						Bmp_Part_Show((WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*posy/3, 0, 0, (WIDTH_RESOLUTION-6)/3, picptrh->pic_height, hline,AD);
						Bmp_Part_Show((WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*posy/3, 0, 0, (WIDTH_RESOLUTION_HD-6)/3, picptrh->pic_height, hline,HD);
					}
					if (b!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch+3)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(b) & alarmch;
						}
						else
						{
							tmp = WBIT(b) & alarmch;
							if (tmp)
							{
								for (i=0;i<(WIDTH_RESOLUTION-6)/3;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*(posy+1)/3,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(WIDTH_RESOLUTION_HD-6)/3;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*(posy+1)/3,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						Bmp_Part_Show((WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*(posy+1)/3, 0, 0, (WIDTH_RESOLUTION-6)/3, picptrh->pic_height, hline,AD);
						Bmp_Part_Show((WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*(posy+1)/3, 0, 0, (WIDTH_RESOLUTION_HD-6)/3, picptrh->pic_height, hline,HD);
					}
					if (l!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch-1)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(l) & alarmch;
						}
						else
						{
							tmp = WBIT(l) & alarmch;
							if (tmp)
							{
								for (i=0;i<(High_Resolution-4)/3;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*pos/3, i+(High_Resolution-4)*posy/3,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(HIGH_RESOLUTION_HD-4)/3;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*pos/3, i+(HIGH_RESOLUTION_HD-4)*posy/3,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						Bmp_Part_Show((WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*posy/3, 0, 0, picptrV->pic_width, (High_Resolution-4)/3, vline,AD);
						Bmp_Part_Show((WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*posy/3, 0, 0, picptrV->pic_width, (HIGH_RESOLUTION_HD-4)/3, vline,HD);
					}
					if (r!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch+1)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(r) & alarmch;
						}
						else
						{
							tmp = WBIT(r) & alarmch;
							if (tmp)
							{
								for (i=0;i<(High_Resolution-4)/3;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*(pos+1)/3, i+(High_Resolution-4)*posy/3,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(HIGH_RESOLUTION_HD-4)/3;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*(pos+1)/3, i+(HIGH_RESOLUTION_HD-4)*posy/3,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						Bmp_Part_Show((WIDTH_RESOLUTION-6)*(pos+1)/3, (High_Resolution-4)*posy/3, 0, 0, picptrV->pic_width, (High_Resolution-4)/3, vline,AD);
						Bmp_Part_Show((WIDTH_RESOLUTION_HD-6)*(pos+1)/3, (HIGH_RESOLUTION_HD-4)*posy/3, 0, 0, picptrV->pic_width, (HIGH_RESOLUTION_HD-4)/3, vline,HD);
					}
				}
				else
				{
					if (t!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch-3)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(t) & alarmch;
						}
						else
						{
							tmp = WBIT(t) & alarmch;
							if (tmp)
							{
								for (i=0;i<(WIDTH_RESOLUTION-6)/3;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*posy/3,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(WIDTH_RESOLUTION_HD-6)/3;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*posy/3,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else 
						tmp = 0;
					if (!tmp)
					{
						clear_osd((WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*posy/3, (WIDTH_RESOLUTION-6)/3, picptrh->pic_height,AD);
						clear_osd((WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*posy/3, (WIDTH_RESOLUTION_HD-6)/3, picptrh->pic_height,HD);
					}
						
					if (b!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch+3)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(b) & alarmch;
						}
						else
						{
							tmp = WBIT(b) & alarmch;
							if (tmp)
							{
								for (i=0;i<(WIDTH_RESOLUTION-6)/3;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*(posy+1)/3,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(WIDTH_RESOLUTION_HD-6)/3;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*(posy+1)/3,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						clear_osd((WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*(posy+1)/3, (WIDTH_RESOLUTION-6)/3, picptrh->pic_height,AD);
						clear_osd((WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*(posy+1)/3, (WIDTH_RESOLUTION_HD-6)/3, picptrh->pic_height,HD);
					}
					if (l!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch-1)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(l) & alarmch;
						}
						else
						{
							tmp = WBIT(l) & alarmch;
							if (tmp)
							{
								for (i=0;i<(High_Resolution-4)/3;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*pos/3, i+(High_Resolution-4)*posy/3,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(HIGH_RESOLUTION_HD-4)/3;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*pos/3, i+(HIGH_RESOLUTION_HD-4)*posy/3,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						clear_osd((WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*posy/3, picptrV->pic_width, (High_Resolution-4)/3,AD);
						clear_osd((WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*posy/3, picptrV->pic_width, (HIGH_RESOLUTION_HD-4)/3,HD);
					}
					if (r!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch+1)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(r) & alarmch;
						}
						else
						{
							tmp = WBIT(r) & alarmch;
							if (tmp)
							{
								for (i=0;i<(High_Resolution-4)/3;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*(pos+1)/3, i+(High_Resolution-4)*posy/3,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(HIGH_RESOLUTION_HD-4)/3;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*(pos+1)/3, i+(HIGH_RESOLUTION_HD-4)*posy/3,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						clear_osd((WIDTH_RESOLUTION-6)*(pos+1)/3, (High_Resolution-4)*posy/3, picptrV->pic_width, (High_Resolution-4)/3,AD);
						clear_osd((WIDTH_RESOLUTION_HD-6)*(pos+1)/3, (HIGH_RESOLUTION_HD-4)*posy/3, picptrV->pic_width, (HIGH_RESOLUTION_HD-4)/3,HD);
					}
				}
			}
			else 
			{
				for (i=0;i<(WIDTH_RESOLUTION-6)/3;i++)
				{
					if ((Cur_CH == (ch-3)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*posy/3,AD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*posy/3,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
					if ((Cur_CH == (ch+3)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*(posy+1)/3,AD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-6)*pos/3, (High_Resolution-4)*(posy+1)/3,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
				for (i=0;i<(WIDTH_RESOLUTION_HD-6)/3;i++)
				{
					if ((Cur_CH == (ch-3)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*posy/3,HD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*posy/3,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
					if ((Cur_CH == (ch+3)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*(posy+1)/3,HD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-6)*pos/3, (HIGH_RESOLUTION_HD-4)*(posy+1)/3,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
				for (i=0;i<(High_Resolution-4)/3;i++)
				{
					if ((Cur_CH == (ch-1)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*pos/3,i+(High_Resolution-4)*posy/3,AD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*pos/3,i+(High_Resolution-4)*posy/3,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
					if ((Cur_CH == (ch+1)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*(pos+1)/3,i+(High_Resolution-4)*posy/3,AD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-6)*(pos+1)/3,i+(High_Resolution-4)*posy/3,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
				for (i=0;i<(HIGH_RESOLUTION_HD-4)/3;i++)
				{
					if ((Cur_CH == (ch-1)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*pos/3,i+(HIGH_RESOLUTION_HD-4)*posy/3,HD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*pos/3,i+(HIGH_RESOLUTION_HD-4)*posy/3,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
					if ((Cur_CH == (ch+1)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*(pos+1)/3,i+(HIGH_RESOLUTION_HD-4)*posy/3,HD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-6)*(pos+1)/3,i+(HIGH_RESOLUTION_HD-4)*posy/3,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
			}
			break;
		case L16:
			pos = ch%4;
			posy = ch/4;
			if (pos==0 || pos == 3)
				offx = 0;
			else
				offx = 1;
			if (color>1 )
			{
				if(pos)
					l=ch-1;
				if (pos<3)
					r=ch+1;
				if (posy)
					t=ch-4;
				if(posy<3)
					b=ch+4;
				
				if (SysMenu.bordcolor)
				{
					if (t!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch-4)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(t) & alarmch;
						}
						else
						{
							tmp = WBIT(t) & alarmch;
							if (tmp)
							{
								for (i=0;i<(WIDTH_RESOLUTION-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*posy/4,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(WIDTH_RESOLUTION_HD-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*posy/4,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else 
						tmp = 0;
					if (!tmp)
					{
						Bmp_Part_Show((WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*posy/4, 0, 0, (WIDTH_RESOLUTION-4)/4, picptrh->pic_height, hline,AD);
						Bmp_Part_Show((WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*posy/4, 0, 0, (WIDTH_RESOLUTION_HD-4)/4, picptrh->pic_height, hline,HD);
					}
					if (b!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch+4)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(b) & alarmch;
						}
						else
						{
							tmp = WBIT(b) & alarmch;
							if (tmp)
							{
								for (i=0;i<(WIDTH_RESOLUTION-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*(posy+1)/4,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(WIDTH_RESOLUTION_HD-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*(posy+1)/4,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						Bmp_Part_Show((WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*(posy+1)/4, 0, 0, (WIDTH_RESOLUTION-4)/4, picptrh->pic_height, hline,AD);
						Bmp_Part_Show((WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*(posy+1)/4, 0, 0, (WIDTH_RESOLUTION_HD-4)/4, picptrh->pic_height, hline,HD);
					}
					if (l!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch-1)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(l) & alarmch;
						}
						else
						{
							tmp = WBIT(l) & alarmch;
							if (tmp)
							{
								for (i=0;i<(High_Resolution-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-4)*pos/4, i+(High_Resolution-4)*posy/4,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(HIGH_RESOLUTION_HD-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-4)*pos/4, i+(HIGH_RESOLUTION_HD-4)*posy/4,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						Bmp_Part_Show((WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*posy/4, 0, 0, picptrV->pic_width, (High_Resolution-4)/4, vline,AD);
						Bmp_Part_Show((WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*posy/4, 0, 0, picptrV->pic_width, (HIGH_RESOLUTION_HD-4)/4, vline,HD);
					}
					if (r!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch+1)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(r) & alarmch;
						}
						else
						{
							tmp = WBIT(r) & alarmch;
							if (tmp)
							{
								for (i=0;i<(High_Resolution-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-4)*(pos+1)/4, i+(High_Resolution-4)*posy/4,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(HIGH_RESOLUTION_HD-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-4)*(pos+1)/4, i+(HIGH_RESOLUTION_HD-4)*posy/4,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						Bmp_Part_Show((WIDTH_RESOLUTION-4)*(pos+1)/4, (High_Resolution-4)*posy/4, 0, 0, picptrV->pic_width, (High_Resolution-4)/4, vline,AD);
						Bmp_Part_Show((WIDTH_RESOLUTION_HD-4)*(pos+1)/4, (HIGH_RESOLUTION_HD-4)*posy/4, 0, 0, picptrV->pic_width, (HIGH_RESOLUTION_HD-4)/4, vline,HD);
					}
				}
				else
				{
					if (t!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch-4)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(t) & alarmch;
						}
						else
						{
							tmp = WBIT(t) & alarmch;
							if (tmp)
							{
								for (i=0;i<(WIDTH_RESOLUTION-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*posy/4,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(WIDTH_RESOLUTION_HD-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*posy/4,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else 
						tmp = 0;
					if (!tmp)
					{
						clear_osd((WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*posy/4, (WIDTH_RESOLUTION-4)/4, picptrh->pic_height,AD);
						clear_osd((WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*posy/4, (WIDTH_RESOLUTION_HD-4)/4, picptrh->pic_height,HD);
					}
					if (b!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch+4)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(b) & alarmch;
						}
						else
						{
							tmp = WBIT(b) & alarmch;
							if (tmp)
							{
								for (i=0;i<(WIDTH_RESOLUTION-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*(posy+1)/4,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(WIDTH_RESOLUTION_HD-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*(posy+1)/4,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						clear_osd((WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*(posy+1)/4, (WIDTH_RESOLUTION-4)/4, picptrh->pic_height,AD);
						clear_osd((WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*(posy+1)/4, (WIDTH_RESOLUTION_HD-4)/4, picptrh->pic_height,HD);
					}
					if (l!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch-1)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(l) & alarmch;
						}
						else
						{
							tmp = WBIT(l) & alarmch;
							if (tmp)
							{
								for (i=0;i<(High_Resolution-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-4)*pos/4, i+(High_Resolution-4)*posy/4,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(HIGH_RESOLUTION_HD-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-4)*pos/4, i+(HIGH_RESOLUTION_HD-4)*posy/4,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						clear_osd((WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*posy/4, picptrV->pic_width, (High_Resolution-4)/4,AD);
						clear_osd((WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*posy/4, picptrV->pic_width, (HIGH_RESOLUTION_HD-4)/4,HD);
					}
					if (r!=0xff)
					{
						if (flag)
						{
							if ((Cur_CH == (ch+1)) && (menu_num == mPTZControl))
								tmp =1;
							else
								tmp = WBIT(r) & alarmch;
						}
						else
						{
							tmp = WBIT(r) & alarmch;
							if (tmp)
							{
								for (i=0;i<(High_Resolution-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-4)*(pos+1)/4, i+(High_Resolution-4)*posy/4,AD,0,0,2,2,MIXCOLOR,15);
								for (i=0;i<(HIGH_RESOLUTION_HD-4)/4;i++)
									osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-4)*(pos+1)/4, i+(HIGH_RESOLUTION_HD-4)*posy/4,HD,0,0,2,2,MIXCOLOR,15);
							}
						}
					}
					else
						tmp = 0;
					if (!tmp)
					{
						clear_osd((WIDTH_RESOLUTION-4)*(pos+1)/4, (High_Resolution-4)*posy/4, picptrV->pic_width, (High_Resolution-4)/4,AD);
						clear_osd((WIDTH_RESOLUTION_HD-4)*(pos+1)/4, (HIGH_RESOLUTION_HD-4)*posy/4, picptrV->pic_width, (HIGH_RESOLUTION_HD-4)/4,HD);
					}
				}
			}
			else 
			{
//				printf("############## with[%d]:%d,posbuf[%d]:%d,posbuf+1[%d]:%d\n",offx,withbuf[offx],pos,posbuf[pos],pos+1,posbuf[pos+1]);
				for (i=0;i<(WIDTH_RESOLUTION-4)/4;i++)
				{
					if ((Cur_CH == (ch-4)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*posy/4,AD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*posy/4,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
					if ((Cur_CH == (ch+4)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*(posy+1)/4,AD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION-4)*pos/4, (High_Resolution-4)*(posy+1)/4,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
				for (i=0;i<(WIDTH_RESOLUTION_HD-4)/4;i++)
				{
					if ((Cur_CH == (ch-4)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*posy/4,HD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*posy/4,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
					if ((Cur_CH == (ch+4)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*(posy+1)/4,HD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,i+(WIDTH_RESOLUTION_HD-4)*pos/4, (HIGH_RESOLUTION_HD-4)*(posy+1)/4,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
				for (i=0;i<(High_Resolution-4)/4;i++)
				{
					if ((Cur_CH == (ch-1)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-4)*pos/4,i+(High_Resolution-4)*posy/4,AD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-4)*pos/4,i+(High_Resolution-4)*posy/4,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
					if ((Cur_CH == (ch+1)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-4)*(pos+1)/4,i+(High_Resolution-4)*posy/4,AD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION-4)*(pos+1)/4,i+(High_Resolution-4)*posy/4,AD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
				for (i=0;i<(HIGH_RESOLUTION_HD-4)/4;i++)
				{
					if ((Cur_CH == (ch-1)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-4)*pos/4,i+(HIGH_RESOLUTION_HD-4)*posy/4,HD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-4)*pos/4,i+(HIGH_RESOLUTION_HD-4)*posy/4,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
					if ((Cur_CH == (ch+1)) && (menu_num == mPTZControl))
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-4)*(pos+1)/4,i+(HIGH_RESOLUTION_HD-4)*posy/4,HD,0,4,2,2,MIXCOLOR,15);
					else
						osd_bmp_photo(motmenupic.mix,(WIDTH_RESOLUTION_HD-4)*(pos+1)/4,i+(HIGH_RESOLUTION_HD-4)*posy/4,HD,0,(1-color)*4,2,2,MIXCOLOR,15);
				}
			}
			break;
		default:
			break;
	}
#endif
}

U8 Rec_Wait=0;
int g_record_display_info='N';

void record_wait(void)
{
#ifdef GM8126_DEV
	int ret;
	fiosd_string_t string;

	if (Rec_Wait != 5)
		return;
	string.windex   = 0;
	string.start    = 0;
	string.fg_color = 1;
	string.bg_color = 0;
	
	sprintf(&string.string[0],"%s W ",CamMenu[0].CamTitle);

	ret = ioctl(fosd_fd[0], FIOSDS_STRING, &string);
	if(ret < 0) 
		printf("FIOSDS 0 STRING Fail!");

	string.windex   = 2;
	string.start    = 0;
	string.fg_color = 1;
	string.bg_color = 0;
	ret = ioctl(fosd_fd[1], FIOSDS_STRING, &string);
	if(ret < 0) 
		printf("FIOSDS 1 STRING Fail!");
#endif
#ifdef HISI_DEV
  			g_record_display_info='N';
//  			g_record_display_info='W';
#endif
	Rec_Wait = 0;
}

void record_disp(unsigned char channel,unsigned char event)
//0_motion  1_sensor  2_vloss  3_time  4_manual  5_idle 6_clear
{
	int ret;
#ifdef GM8126_DEV
	fiosd_string_t string;
#endif

//	printf("RRRRRRRRRRRRRRRRRRRRRRRRRRevent:%d\n",event);
	if (event <2)
	{
		alarmch  = 1;
		if (event == 0)
			net_motion = 1;
		else if (event == 1)
			net_sensor = 1;
		alarm_sent_net();
	}
	else if (alarmch)
	{
		alarmch = 0;
		net_motion = 0;
		net_sensor = 0;
		alarm_sent_net();
	}
	
#ifdef GM8126_DEV
//	if (rec_on)
	{
		string.windex   = 0;
		string.start    = 0;
		string.fg_color = 1;
		string.bg_color = 0;
		
		switch (event)
		{
			case 0:
				Rec_Wait = 0;
				sprintf(&string.string[0],"%s M ",CamMenu[0].CamTitle);
				break;
			case 1:
				Rec_Wait = 0;
				sprintf(&string.string[0],"%s S ",CamMenu[0].CamTitle);
				break;
			case 3:
				Rec_Wait = 0;
				sprintf(&string.string[0],"%s T ",CamMenu[0].CamTitle);
				break;
			case 5:
				Rec_Wait = 5;
				break;
			default:
				Rec_Wait = 0;
				sprintf(&string.string[0],"%s W ",CamMenu[0].CamTitle);
				break;
		}

		if (Rec_Wait==0)
		{
			ret = ioctl(fosd_fd[0], FIOSDS_STRING, &string);
			if(ret < 0) 
				printf("FIOSDS 0 STRING Fail!");

			string.windex   = 2;
			string.start    = 0;
			string.fg_color = 1;
			string.bg_color = 0;
			ret = ioctl(fosd_fd[1], FIOSDS_STRING, &string);
			if(ret < 0) 
				printf("FIOSDS 1 STRING Fail!");
		}
	}
#endif
#ifdef HISI_DEV
    switch (event)
    {
        case 0:
            Rec_Wait = 0;
        g_record_display_info='M';
//          sprintf(&string.string[0],"%s M ",CamMenu[0].CamTitle);
            break;
        case 1:
            Rec_Wait = 0;
        g_record_display_info='S';
//          sprintf(&string.string[0],"%s S ",CamMenu[0].CamTitle);
            break;
        case 3:
            Rec_Wait = 0;
        g_record_display_info='T';
//          sprintf(&string.string[0],"%s T ",CamMenu[0].CamTitle);
            break;
        case 5:
            Rec_Wait = 5;
        g_record_display_info='N';
            break;
        default:
            Rec_Wait = 0;
        g_record_display_info='N';
//              g_record_display_info='W';
//          sprintf(&string.string[0],"%s W ",CamMenu[0].CamTitle);
            break;
    }
     DBGPRINT("*********** s2016/09/21 09:16:28 **************** \n");
    switch(g_record_display_info)
    {
        case 'W':
            cam_name_rec_display(1);
            break;
        case 'M':
            cam_name_rec_display(2);
            break;
        case 'S':
            cam_name_rec_display(3);
            break;
        case 'T':
            cam_name_rec_display(4);
            break;
        case 'N':
            cam_name_rec_display(5);
            break;
    }
#endif
}

void Full_Disk_Disp()
{
	int erro;
	time_t time_err;
//	BXS_PIC1* picbuf=NULL;
	gstCommonParam->GST_FS_GetErrorMessage(&erro,&time_err);
//	erro = ALLDISKNOTENABLETOWRITE;
//	printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF erro:%d,diskstatus:%d\n",erro,diskstatus);
	if (erro == ALLDISKNOTENABLETOWRITE && diskstatus != 1)
	{
/*	
		picbuf = (BXS_PIC1*)Iconhddback;
		diskstatus = 1;
		osd_bmp_photo(playblock,POSXRMENU,POSYRMENU,AD,0,0,picbuf->pic_width,picbuf->pic_height/3,MIXCOLOR,0);
		Bmp_Part_Show(POSXRMENU, POSYRMENU, 0, diskstatus*picbuf->pic_height, picbuf->pic_width,picbuf->pic_height/3, Iconhddback);
		buzzer_set(1);
*/
		stop_key_set();
		InfoId = HddFullRec;
		diskstatus = 1;
		buzzer_set(1);
		MenuErrOn();
	}
}


