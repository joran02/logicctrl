#include	"cmn.h"

/*
U16 NameY_Pos0[]=
{
	3,11,	25,11,   //name  NTSC
	3,22,	25,22,


	8,13,	30,13,   //name  PAL
	8,27,	30,27,
};

U16 NameY_Pos1[]=
{
	20,105,	60,105,   //name  NTSC
	20,220,	60,220,


	20,130,	60,130,   //name  PAL
	20,266,	60,266,
};

U16* NameY_Pos[]=
{
	NameY_Pos0,
	NameY_Pos1
};
*/

//extern unsigned char gt_as__fnt__16x20[];
/*
char *trim(char *str)
{
        char *p = str;
        char *p1;
        if(p)
        {
                p1 = p + strlen(str) - 1;
                while(*p && isspace(*p)) p++;
//                while(p1 > p && *p1<0x20) *p1-- = '\0';
                while(p1 > p && (isspace(*p1) || *p1<0x21)) *p1-- = '\0';
		
        }
        return p;
}
*/

U8 CurShow=0;
void recpathnamedisp()
{
#ifdef GM8126_DEV
	int ret;
	fiosd_string_t string;

	string.windex   = 0;
	string.start    = 0;
	string.fg_color = 1;
	string.bg_color = 0;
	
	sprintf(&string.string[0],"%s W ",CamMenu[0].CamTitle);
	ret = ioctl(fosd_fd[0], FIOSDS_STRING, &string);
	if(ret < 0) 
		printf("FIOSDS 0 STRING Fail!");

	string.windex   = 3;
	string.start    = 0;
	string.fg_color = 1;
	string.bg_color = 0;
	sprintf(&string.string[0],"%s W ",CamMenu[0].CamTitle);
	ret = ioctl(fosd_fd[1], FIOSDS_STRING, &string);
	if(ret < 0) 
		printf("FIOSDS 1 STRING Fail!");
#endif
#ifdef HISI_DEV
//  g_record_display_info='W';
#endif
}
void NoCard_Show(void)
{
#ifdef GM8126_DEV
	int ret;
	fiosd_string_t string;

	if (Disk_Num)
		return;
	if (CurShow)
		recpathnamedisp();
	else
	{
		string.windex   = 0;
		string.start    = 0;
		string.fg_color = 1;
		string.bg_color = 0;
		
		sprintf(&string.string[0],"NO CARD ");
		ret = ioctl(fosd_fd[0], FIOSDS_STRING, &string);
		if(ret < 0) 
			printf("FIOSDS 0 STRING Fail!");

		string.windex   = 0;
		string.start    = 0;
		string.fg_color = 1;
		string.bg_color = 0;
		sprintf(&string.string[0],"NO CARD ");
		ret = ioctl(fosd_fd[1], FIOSDS_STRING, &string);
		if(ret < 0) 
			printf("FIOSDS 1 STRING Fail!");
	}
#endif
#ifdef HISI_DEV
	if (Disk_Num)
		return;
//	if (CurShow)
//		recpathnamedisp();
//	else
// 		g_record_display_info='N';
#endif
	CurShow ^= 1;
}

void demo_name_disp1(UC dc,U8 color)
{
#if 0
	 U8 i,temp;
	 short j;
	 U16 wtemp;
	 U8 str_len=6;
	 U8 buf_title[6];
	BXS_PIC1 * bxs_pic = NULL;
	BXS_PIC1 * bxs_pic1 = getyuvfile(mousemenupic.recordicoground,"mousemenupic.recordicoground");
	 U8 chnum;
	chnum=pic_num;
	if (pic_num>Cam_Num)
		chnum=Cam_Num;
	for(i=0;i<chnum;i++)
	{
		wtemp=WBIT(buf_cam[i]);
//		printf("buf_cam[i] = %x\n",buf_cam[i]);
		if(wtemp)
		{
			str_len=6;
			buf_title[5] = 0;
			temp = 0;
			if(dc)
			{
				I_read_block_eeprom(ADDCAM+ADDCOUNTCAM*buf_cam[i]+CAMTITLE,5,buf_title);

				buf_title[5]='\0';
			}
			else
			{
				for(j=0;j<str_len;j++)
					buf_title[j]=' ';
				buf_title[5]=0;
				str_len = 6;
			}
			Bmp_All_Show(osd_ptr[0][i*2], osd_ptr[0][i*2+1]-bxs_pic1->pic_height, mousemenupic.recordicoground,AD);
			Bmp_All_Show(hdosd_ptr[0][i*2], hdosd_ptr[0][i*2+1]-bxs_pic1->pic_height, mousemenupic.recordicoground,HD);
			if (CamMenu[buf_cam[i]].InputMatch)
			{
				bxs_pic = getyuvfile(cammenupic.camnamelocal,"cammenupic.camnamelocal");
				Bmp_Part_Show(osd_ptr[0][i*2]+10,osd_ptr[0][i*2+1]+1-bxs_pic1->pic_height, 0,buf_cam[i]*bxs_pic->pic_height/16,bxs_pic->pic_width,bxs_pic->pic_height/16,cammenupic.camnamelocal,AD);
				Bmp_Part_Show(hdosd_ptr[0][i*2]+10,hdosd_ptr[0][i*2+1]+1-bxs_pic1->pic_height, 0,buf_cam[i]*bxs_pic->pic_height/16,bxs_pic->pic_width,bxs_pic->pic_height/16,cammenupic.camnamelocal,HD);
			}
			else
			{
				Osd_line_display(osd_ptr[0][i*2]+10,osd_ptr[0][i*2+1]+1-bxs_pic1->pic_height,color,buf_title,0xff,AD);
				Osd_line_display(hdosd_ptr[0][i*2]+10,hdosd_ptr[0][i*2+1]+1-bxs_pic1->pic_height,color,buf_title,0xff,HD);
			}
			bxs_pic = getyuvfile(recordico,"recordico");
			bxs_pic1 = getyuvfile(mousemenupic.recordicoground,"mousemenupic.recordicoground");
			if (wtemp & cam_loss)
			{
				Bmp_Part_Show(osd_ptr[0][i*2]+bxs_pic1->pic_width-10-bxs_pic->pic_width,osd_ptr[0][i*2+1]+1-bxs_pic1->pic_height, 0, bxs_pic->pic_height*7/8, bxs_pic->pic_width, bxs_pic->pic_height/8, recordico,AD);
				Bmp_Part_Show(hdosd_ptr[0][i*2]+bxs_pic1->pic_width-10-bxs_pic->pic_width,hdosd_ptr[0][i*2+1]+1-bxs_pic1->pic_height, 0, bxs_pic->pic_height*7/8, bxs_pic->pic_width, bxs_pic->pic_height/8, recordico,HD);
			}
			else
			{
				Bmp_Part_Show(osd_ptr[0][i*2]+bxs_pic1->pic_width-10-bxs_pic->pic_width,osd_ptr[0][i*2+1]+1-bxs_pic1->pic_height, 0, bxs_pic->pic_height*6/8, bxs_pic->pic_width, bxs_pic->pic_height/8, recordico,AD);
				Bmp_Part_Show(hdosd_ptr[0][i*2]+bxs_pic1->pic_width-10-bxs_pic->pic_width,hdosd_ptr[0][i*2+1]+1-bxs_pic1->pic_height, 0, bxs_pic->pic_height*6/8, bxs_pic->pic_width, bxs_pic->pic_height/8, recordico,HD);
			}
		}
	}
#endif 
}

void name_disp(U8 color)
{
	if(valid_name_disp())
	{
		demo_name_disp1(1,color);
//		demo_name_disp1(0,color);
	}
}

void demo_osd_disp()
{
#if 0
	U8 i;
	if (ChipNum == SLAVE)
		return;
	name_disp(WHITE);
	border_set(demo_num);
//	printf("****************demo_num = %x *********************\n",demo_num);
	
 	if(demo_num<PBFULL || demo_num==RECORD)
	{
//		loss_disp(0xffff);
		record_disp_refresh();
	}
	if (rec_on)
	{
		for (i=0;i<Cam_Num;i++)
			RecFlagOld[i]=0;
//		record_disp();
	}
	if (!auto_flag)
	{
		if (MenuStatus == Iconmenu)
			Icon_Menu_On(0, 0);
		else if (MenuStatus == mMouse)
			Menu_Mouse_On();
		else if (MenuStatus == (mMouse+Iconmenu))
		{
			Icon_Menu_On(0, 0);
			Menu_Mouse_On();
		}
	}
#endif
}
//**********************************************software update**************************************//


#define COMMAND_FILE "/tmp/command.txt"
#define COMMAND_FAILD_FILE "/tmp/command_faild.txt"
#define RM_COMMAND_FILE "rm -rf /tmp/command.txt"
#define RM_COMMAND_FAILD_FILE "rm -rf /tmp/command_faild.txt"
#define SLEEP_TIME 100000
U8 uok = 0;

int sys_command(char * cmd)
{
	FILE * fp = NULL;
	int rel;
	int sleep_time;
	long fileOffset = 0;	
	char cmd_buf[200];
	int  count = 0;

	if(-1 != system(cmd))
	//if(-1 != SendM(cmd))
		return;

	fileOffset = strlen(cmd);

	if( fileOffset > 199 )
	{
		printf(" command %s is too long!\n",cmd);
	}

FIRST_COMMAD_NOT_RUN:	
	fp = fopen(COMMAND_FILE,"r");
	if( fp != NULL)
	{
		fclose(fp);
		fp = NULL;
		usleep(SLEEP_TIME);
		count++;
		if( count > 10 )
		{
//			printf("LAST_COMMAD_NOT_RUN\n");
			count = 0;
		}
		goto FIRST_COMMAD_NOT_RUN;
	}
	

	fp = fopen(COMMAND_FILE,"wb");
	if( fp == NULL)
		return -1;

	strcpy(cmd_buf,cmd);

	rel = fwrite(cmd_buf,1,fileOffset+1,fp);

	if( rel != fileOffset+1)
	{
		DPRINTK("fwrite error!\n");
		fclose(fp);
		fp = NULL;
		return -1;
	}


	fclose(fp);
	fp = NULL;

	//等待命令执行程序处理
	sleep_time = SLEEP_TIME*2;	
	usleep(sleep_time);

LAS_COMMAD_NOT_RUN:	
	fp = fopen(COMMAND_FILE,"r");
	if( fp != NULL)
	{
		fclose(fp);
		fp = NULL;
		usleep(SLEEP_TIME);
		count++;

		if( count > 10 )
		{
//			printf("END_COMMAD_NOT_RUN\n");
			count = 0;
		}
		goto LAS_COMMAD_NOT_RUN;
	}


	fp = fopen(COMMAND_FAILD_FILE,"rb");
	if( fp != NULL )
	{
		fclose(fp);
		fp = NULL;
		return -1;
	}

	return 1;
	
}


int command_sys(char * command)
{	
	int	res = 1;	

	//DPRINTK("command: %s 1\n",command);
	//res = system(command);	
	res = -1;
	//DPRINTK("command: %s 2\n",command);
	if(res == 0x00)
	{				
		return res;
	}
	else
	{	
		//printf(" command_drv: %s error! code=%d\n",command,res);

		if( res < 0 )
		{
			res = sys_command(command);
			if( res > 0 )
			{
				printf(" sys_command : %s sucess!\n",command);
				return 1;
			}else
			{
				printf(" sys_command : %s faild!\n",command);
				return -1;
			}
		}
		return -1;
	}	
	
}

