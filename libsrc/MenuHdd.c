#include "cmn.h"

ValHdd HddMenu;

DISKMENU diskmenupic;
#define ETC_MAXLINE             1024

#define ETC_FILENOTFOUND        -1
#define ETC_SECTIONNOTFOUND     -2
#define ETC_KEYNOTFOUND         -3
#define ETC_TMPFILEFAILED       -4
#define ETC_FILEIOFAILED        -5
#define ETC_INTCONV             -6
#define ETC_INVALIDOBJ          -7
#define ETC_READONLYOBJ         -8
#define ETC_OK                  0



#define HDDINFOMENU 	5

#define HDDFIXEDMENU 8

U8* usbformatback;
U8* usbformatfore;
int have_usb=0,usb_format_is_right=0;
int usb_sel_flag=0;


extern U8 getip[];
//Remote_USB_Disk
//U8 SoftVer[]={"1.000"};

 U16 Menu_HDD_DataPos[]=
{
	280,62,
	280,118,
	280,144,
	280,170,
	280,196,
//	20,278-38,///
	20,278,	190,278,	360,278
};

U16 Pre_hddpos[] = 
{
	280,62,		300,82,
	280,118,		406,142,
	0,0,			0,0,
	0,0,			0,0,
	0,0,			0,0,
//	20,278-38,		180,302-38,///
	20,278,		180,302,
	190,278,		350,302,
	360,278,		520,302
};

U16 Info_Date_Pos[]=
{
	330,54,
	330,80,
	330,106,
	330,132,
	330,158,
	330,184,
	330,210,
	
	160,236,		
	160,262,		
	456,262,	
	160,288,
	160,314,		
	160,340,
	160,366,
	
		230,402
};

void MenuHddInit()
{
	I_read_byte_eeprom(ADDHDD, &HddMenu.Overwrite);
	gstCommonParam->GST_FS_EnableDiskReCover(HddMenu.Overwrite);
}

void MenuHddSet()
{
	if(have_usb&&usb_sel_flag==1)
	{
		HddMenu.HddSize=1;//pstDiskInfo[CurHdd]->size/1024/1024/1024;
		HddMenu.RemanSize =1;// gstCommonParam->GST_FS_get_disk_remain_rate(CurHdd);
		HddMenu.CoveRate =1;// gstCommonParam->GST_FS_get_cover_rate(CurHdd);
		HddMenu.BadSize =1;// gstCommonParam->GST_FS_GetBadBlockSize(CurHdd);
	}
	else if(Disk_Num)
	{
		HddMenu.HddSize=pstDiskInfo[CurHdd]->size/1024/1024/1024;
		HddMenu.RemanSize = gstCommonParam->GST_FS_get_disk_remain_rate(CurHdd);
		HddMenu.CoveRate = gstCommonParam->GST_FS_get_cover_rate(CurHdd);
		HddMenu.BadSize = gstCommonParam->GST_FS_GetBadBlockSize(CurHdd);
	}
}

void menu_hdd_data_disp(U8 pos,U16 color)
{
}

void menu_hdd_on()
{
}

void menu_hdd_down()
{
	menu_hdd_data_disp(cur_pos,WHITE);
	if(cur_pos==1)
		cur_pos = 5;
	else if (cur_pos==HDDFIXEDMENU-1)
		cur_pos=0;
	else
		cur_pos++;
	menu_hdd_data_disp(cur_pos,RED);
}
void menu_hdd_up()
{
	menu_hdd_data_disp(cur_pos,WHITE);
	if(cur_pos==0)
		cur_pos = HDDFIXEDMENU-1;
	else if (cur_pos==5)
		cur_pos=1;
	else
		cur_pos--;
	menu_hdd_data_disp(cur_pos,RED);
}

void menu_hdd_add()
{
	U8 i;
	if(cur_pos!=1)
		return;

	if(Disk_Num)
	{
		if (HddMenu.HDDName>=Disk_Num-1)
		{
			if(have_usb&&usb_sel_flag==0)
				usb_sel_flag=1;//HddMenu.HDDName=0;
			else
			{
				HddMenu.HDDName=0;
				usb_sel_flag=0;
			}
		}
		else
		{
			HddMenu.HDDName++;
		}
		CurHdd = HddMenu.HDDName;
	}
	else if(have_usb&&usb_sel_flag==0)
		usb_sel_flag=1;
	MenuHddSet();
	for(i=1;i<HDDFIXEDMENU;i++)
	{
		if(i!=cur_pos)menu_hdd_data_disp(i,WHITE);
		else menu_hdd_data_disp(cur_pos,RED);
	}
}
void menu_hdd_dec()
{
	U8 i;
	if(cur_pos!=1)
		return;
	if(Disk_Num)
	{
		if (HddMenu.HDDName)
		{
			HddMenu.HDDName--;
		}
		else
		{
			if(have_usb&&usb_sel_flag==0)
				usb_sel_flag=1;
			else
			{
				HddMenu.HDDName=Disk_Num-1;
				usb_sel_flag=0;
			}
		}
		CurHdd = HddMenu.HDDName;
	}
	else if(have_usb&&usb_sel_flag==0)
		usb_sel_flag=1;
	MenuHddSet();
	for(i=1;i<HDDFIXEDMENU;i++)
	{
		if(i!=cur_pos)menu_hdd_data_disp(i,WHITE);
		else menu_hdd_data_disp(cur_pos,RED);
	}
}

void menu_hdd_off()
{
    	OSD_ALL_CLEAR();
	MenuHddInit();
	menu_main_on();
}

void DISK_Format(int x,int y)
{
	int have_usb;//,format_is_right;
	gstCommonParam->GST_FTC_PreviewRecordStop(0);

	if(usb_sel_flag)
	{
			gstCommonParam->GST_FS_CheckUsb(&have_usb,&usb_format_is_right);
			if(have_usb)
				gstCommonParam->GST_FS_FormatUsb();
	}
	else
	{
		gstCommonParam->GST_FS_UMountAllPatition(CurHdd);
		gstCommonParam->GST_FS_PartitionDisk(CurHdd);
		gstCommonParam->GST_FS_BuildFileSystem(CurHdd);
		gstCommonParam->GST_FS_MountAllPatition(CurHdd);
		diskstatus = 0;
	}
	gstCommonParam->GST_FTC_PreviewRecordStart(5);
}

void menu_hdd_enter()
{
	switch(cur_pos)
	{
		case 0:
			if (HddMenu.Overwrite)
				HddMenu.Overwrite=0;
			else
				HddMenu.Overwrite=1;
			menu_hdd_data_disp(cur_pos,RED);
			break;
		case 5:
//			usb_sel_flag=0;
			InfoId = FORMATING;
			MenuErrOn();
			break;
//		case 5:
//			usb_sel_flag=1;
//			InfoId = FORMATING;
//			MenuErrOn();
//			break;
		case 6:
			I_write_byte_eeprom(ADDHDD, HddMenu.Overwrite);
			gstCommonParam->GST_FS_EnableDiskReCover(HddMenu.Overwrite);
			menu_hdd_off();
			break;
		case 7:
			menu_hdd_off();
			break;
	}
}
//--------------------

void MouseHddRClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_hddpos[cur_pos*4]+offsetx_hd;
		miny = Pre_hddpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_hddpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_hddpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_hddpos[cur_pos*4]+offsetx;
		miny = Pre_hddpos[cur_pos*4+1]+offsety;
		maxx = Pre_hddpos[cur_pos*4+2]+offsetx;
		maxy = Pre_hddpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && cur_pos==1)
		menu_hdd_add(); 
}

void MouseHddLClick(U16 x,U16 y)
{
	U16 minx,miny,maxx,maxy;
	if (MseType)
	{
		minx = Pre_hddpos[cur_pos*4]+offsetx_hd;
		miny = Pre_hddpos[cur_pos*4+1]+offsety_hd;
		maxx = Pre_hddpos[cur_pos*4+2]+offsetx_hd;
		maxy = Pre_hddpos[cur_pos*4+3]+offsety_hd;
	}
	else
	{
		minx = Pre_hddpos[cur_pos*4]+offsetx;
		miny = Pre_hddpos[cur_pos*4+1]+offsety;
		maxx = Pre_hddpos[cur_pos*4+2]+offsetx;
		maxy = Pre_hddpos[cur_pos*4+3]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy))
	{
		if (cur_pos == 1)
			menu_hdd_dec(); 
		else
			menu_hdd_enter();
	}
}

void Mouse_Hdd_Move_Func(U16 x,U16 y)
{
	U8 i;
	U16 minx,miny,maxx,maxy;
	for (i=0;i<HDDFIXEDMENU;i++)
	{
		if (MseType)
		{
			minx = Pre_hddpos[i*4]+offsetx_hd;
			miny = Pre_hddpos[i*4+1]+offsety_hd;
			maxx = Pre_hddpos[i*4+2]+offsetx_hd;
			maxy = Pre_hddpos[i*4+3]+offsety_hd;
		}
		else
		{
			minx = Pre_hddpos[i*4]+offsetx;
			miny = Pre_hddpos[i*4+1]+offsety;
			maxx = Pre_hddpos[i*4+2]+offsetx;
			maxy = Pre_hddpos[i*4+3]+offsety;
		}
		if ((x>=minx) && (y>=miny) && (x<=maxx) && (y<=maxy) && (cur_pos != i))
		{
			menu_hdd_data_disp(cur_pos,WHITE);
			cur_pos=i;
			menu_hdd_data_disp(cur_pos,RED);
			break;
		}
	}
}

void mHDD_key_func()
{
	if(byKey_val==0) return;

	switch(byKey_val)
	{
	case kESC:
		menu_hdd_off();
		break;
	case kUP:
		menu_hdd_up();
   		break;
	case kDN:
		menu_hdd_down();
		break;
	case kRH:
	case kAD:
 		menu_hdd_add(); 
		break;
	case kLT:
   	case kDE:
		menu_hdd_dec();
		break;
	case kET:
		menu_hdd_enter();
	default:
		break;
    }
	byKey_val=0;
}
U8 curpor=0;


int get_key_value (char *key_line, char **mykey, char **myvalue)
{
    char* current;
    char* tail;
    char* value;

    if (!key_line)
        return -1;

    current = key_line;

    while (*current == ' ' ||  *current == '\t') current++; 

    if (*current == ';' || *current == '#')
        return -1;

    if (*current == '[')
        return 1;

    if (*current == '\n' || *current == '\0' || *current == '\r')
        return -1;

    tail = current;
    while (*tail != '=' && *tail != '\n' &&
          *tail != ';' && *tail != '#' && *tail != '\0' && *tail != '\r')
          tail++;

    value = tail + 1;
    if (*tail != '=')
        *value = '\0'; 

    *tail-- = '\0';
    while (*tail == ' ' || *tail == '\t') {
        *tail = '\0';
        tail--; 
    }
        
    tail = value;
    while (*tail != '\n' && *tail != '\0' && *tail != '\r') tail++;
    *tail = '\0'; 

    if (mykey)
        *mykey = current;
    if (myvalue)
        *myvalue = value;

    return 0;
}

int etc_LocateKeyValue1(FILE* fp, const char* pKey, 
                               U8 bCurSection, char* pValue, int iLen,
                               FILE* bak_fp, char* nextSection)
{
    char szBuff[ETC_MAXLINE + 1 + 1];
    char* current;
    char* value;
    int ret;

    while (1) {
        int bufflen;

        if (!fgets(szBuff, ETC_MAXLINE, fp))
            return ETC_FILEIOFAILED;
        bufflen = strlen (szBuff);
        if (szBuff [bufflen - 1] == '\n' || szBuff [bufflen - 1] == '\r')
            szBuff [bufflen - 1] = '\0';

        ret = get_key_value (szBuff, &current, &value);
        if (ret < 0)
            continue;
        else if (ret > 0) {
            fseek (fp, -bufflen, SEEK_CUR);
            return ETC_KEYNOTFOUND;
        }
            
        if (strcmp (current, pKey) == 0) {
            if (pValue)
                strncpy (pValue, value, iLen);

            return ETC_OK; 
        }
        else if (bak_fp && *current != '\0') {

            char tmp_nam[256];
            sprintf (tmp_nam , "%s=%s\n", pKey, pValue);
            fputs (tmp_nam, bak_fp);
//            fprintf (bak_fp, "%s=%s\n", current, value);
        }
    }

    return ETC_KEYNOTFOUND;
}

char* get_section_name (char *section_line)
{
    char* current;
    char* name;

    if (!section_line)
        return NULL;

    current = section_line;

    while (*current == ' ' ||  *current == '\t') current++; 

    if (*current == ';' || *current == '#')
        return NULL;

    if (*current++ == '[')
        while (*current == ' ' ||  *current == '\t') current ++;
    else
        return NULL;

    name = current;
    while (*current != ']' && *current != '\n' &&
          *current != ';' && *current != '#' && *current != '\0' && *current != '\r')
          current++;
    *current = '\0';
    while (*current == ' ' || *current == '\t') {
        *current = '\0';
        current--; 
    }

    return name;
}

int etc_LocateSection(FILE* fp, const char* pSection, FILE* bak_fp)
{
    char szBuff[ETC_MAXLINE + 1];
    char *name;

    while (1) {
        if (!fgets(szBuff, ETC_MAXLINE, fp)) {
            if (feof (fp))
                return ETC_SECTIONNOTFOUND;
            else
                return ETC_FILEIOFAILED;
        }
        else if (bak_fp && fputs (szBuff, bak_fp) == EOF)
            return ETC_FILEIOFAILED;
        
        name = get_section_name (szBuff);
        if (!name)
            continue;

        if (strcmp (name, pSection) == 0)
            return ETC_OK; 
    }

    return ETC_SECTIONNOTFOUND;
}

int GetValueFromEtcFile(const char* pEtcFile, const char* pSection,
                               const char* pKey, char* pValue, int iLen)
{
	FILE* fp;
	char tempSection [ETC_MAXLINE + 2];

	if (!(fp = fopen(pEtcFile, "r")))
		return ETC_FILENOTFOUND;

	if (pSection)
	{
		if (etc_LocateSection (fp, pSection, NULL) != ETC_OK) 
		{
			fclose (fp);
			return ETC_SECTIONNOTFOUND;
		}

		if (etc_LocateKeyValue1 (fp, pKey, pSection != NULL, pValue, iLen, NULL, tempSection) != ETC_OK) 
		{
			fclose (fp);
			return ETC_KEYNOTFOUND;
		}
	}
	fclose (fp);
	return ETC_OK;
}


long long DiviFun(long long a,long long b)
{
	if (b)
		return a/b;
	else
		return 1;
}

void getsoftver(char* buf)
{
	sprintf(buf,"%s %s",__DATE__,__TIME__);
}

void Hdd_Info_Show()
{
	 U8 i,bt;
	 char IPChar[30];
	 long long buf[4];
	 long long tmp1;
	 int ret,tmp2;
	time_t start_time[8]={0,0,0,0,0,0,0,0};
	time_t end_time[8]={0,0,0,0,0,0,0,0};
	struct tm tp;
	char time_buf[2][25];
	char server_ip[200],mac_id[200],netinfostr[400];
//	BXS_PIC1 * bxs_pic = NULL;
	char SoftVer[20];
//	menu_num=mHDDINFO;
//	bxs_pic = getyuvfile(diskmenupic.systeminfo,"diskmenupic.systeminfo");
//	offsetx=(WIDTH_RESOLUTION-bxs_pic->pic_width)/2;
//	offsety=(High_Resolution-bxs_pic->pic_height)/2;
//	offsetx_hd=(WIDTH_RESOLUTION_HD-bxs_pic->pic_width)/2;
//	offsety_hd=(HIGH_RESOLUTION_HD-bxs_pic->pic_height)/2;

//	 OSD_ALL_CLEAR();
//	curpor = 0x10;
//	Bmp_All_Show(-1,-1, diskmenupic.systeminfo,AD);
//	Bmp_All_Show(-1,-1, diskmenupic.systeminfo,HD);

	memset( time_buf, 0x00,sizeof(time_buf));
	memset( buf, 0x00,sizeof(buf));
	if (Disk_Num)
	{
//		printf("pstDiskInfo[0]->size = %x\n",pstDiskInfo[0]->size);
		bt=0;
/*
		buf[0] =pstDiskInfo[i]->size/1024/1024/1024;
		buf[1] = gstCommonParam->GST_FS_get_disk_remain_rate(i);
		buf[2] = gstCommonParam->GST_FS_get_cover_rate(i);
		buf[3] = gstCommonParam->GST_FS_GetBadBlockSize(i);
*/
		for (i=0;i<Disk_Num;i++)
		{
			tmp1=tmp2=0;
			buf[0] += pstDiskInfo[i]->size/1024/1024/1024;
			tmp1 = pstDiskInfo[i]->size/1024/1024/1024;
			tmp2 = gstCommonParam->GST_FS_get_disk_remain_rate(i);
			buf[1] += tmp1 * tmp2;
//			printf("##disk%d:tmp2:%ld,buf[1]:%ld\n",i,tmp2,buf[1]);
			tmp2=0;
			tmp2 = gstCommonParam->GST_FS_get_cover_rate(i);

			buf[2] += tmp1 * tmp2;
			buf[3] += gstCommonParam->GST_FS_GetBadBlockSize(i);
			tmp2=0;
			ret=gstCommonParam->GST_FTC_get_disk_rec_start_end_time(&start_time[i],&end_time[i],i);
			if (ret<0)
				printf("get time error!\n");
		}
//		printf("&&&&&&buf[0]:%ld,buf[1]:%ld,buf[2]:%ld\n",buf[0],buf[1],buf[2]);
		buf[1] = DiviFun(buf[1],buf[0]);
		buf[2] = DiviFun(buf[2],buf[0]);
//		printf("#####buf[0]:%ld,buf[1]:%ld,buf[2]:%ld\n",buf[0],buf[1],buf[2]);
		for (i=0;i<(Disk_Num-1);i++)
		{
			if (start_time[0]==0 && start_time[i])
				start_time[0] = start_time[i];
			if (end_time[0]==0 && end_time[i])
				end_time[0] = end_time[i];
		}
		for (i=0;i<(Disk_Num-1);i++)
		{
			if (start_time[0]>start_time[i+1] && start_time[i+1])
				start_time[0] = start_time[i+1];
			if (end_time[0]<end_time[i+1] && end_time[i+1])
				end_time[0] = end_time[i+1];
		}
		if (start_time[0] !=0 && end_time[0] != 0)
		{
			gmtime_r(&start_time[0],&tp);
			memset(time_buf[0],0x00,sizeof(time_buf[0]));
			if(SysMenu.tmregion ==0)
				sprintf(time_buf[0],"%04d/%02d/%02d %02d:%02d:%02d",
				tp.tm_year+1900,tp.tm_mon+1,tp.tm_mday,tp.tm_hour,tp.tm_min,tp.tm_sec);
			else if (SysMenu.tmregion ==1)
				sprintf(time_buf[0],"%02d/%02d/%04d %02d:%02d:%02d",
				tp.tm_mon+1,tp.tm_mday,tp.tm_year+1900,tp.tm_hour,tp.tm_min,tp.tm_sec);
			else if(SysMenu.tmregion ==2)
				sprintf(time_buf[0],"%02d/%02d/%04d %02d:%02d:%02d",
				tp.tm_mday,tp.tm_mon+1,tp.tm_year+1900,tp.tm_hour,tp.tm_min,tp.tm_sec);
			 gmtime_r(&end_time[0],&tp);
			memset(time_buf[1],0x00,sizeof(time_buf[1]));
			if(SysMenu.tmregion ==0)
				sprintf(time_buf[1],"%04d/%02d/%02d %02d:%02d:%02d",
				tp.tm_year+1900,tp.tm_mon+1,tp.tm_mday,tp.tm_hour,tp.tm_min,tp.tm_sec);
			else if (SysMenu.tmregion ==1)
				sprintf(time_buf[1],"%02d/%02d/%04d %02d:%02d:%02d",
				tp.tm_mon+1,tp.tm_mday,tp.tm_year+1900,tp.tm_hour,tp.tm_min,tp.tm_sec);
			else if(SysMenu.tmregion ==2)
				sprintf(time_buf[1],"%02d/%02d/%04d %02d:%02d:%02d",
				tp.tm_mday,tp.tm_mon+1,tp.tm_year+1900,tp.tm_hour,tp.tm_min,tp.tm_sec);
		}
	}
	else
	{
		for (i=0;i<4;i++)
			buf[i]=0;
	}
	sysinfostr.m_HDD_QTY = Disk_Num;
//	HddMenu.HDDName = CurHddInfo;
//	bxs_pic = (BXS_PIC1*) diskmenupic.disknamefore;
//	Osd_One_display(Info_Date_Pos[0]+offsetx,Info_Date_Pos[1]+offsety, WHITE, Disk_Num, 0x0f,AD);
//	Osd_One_display(Info_Date_Pos[0]+offsetx_hd,Info_Date_Pos[1]+offsety_hd, WHITE, Disk_Num, 0x0f,HD);
//	Bmp_Part_Show(Info_Date_Pos[0]+offsetx,Info_Date_Pos[1]+offsety, 0,HddMenu.HDDName*bxs_pic->pic_height/6, bxs_pic->pic_width, bxs_pic->pic_height/6, diskmenupic.disknamefore);
//	memset(SoftVer,0x00,sizeof(SoftVer));
//	sprintf(SoftVer,"%d",(int)buf[0]);
	sysinfostr.m_CAPACITY = buf[0];
	if (Disk_Num==1)
	{
		buf[1] = gstCommonParam->GST_FS_get_disk_remain_rate(0);
		buf[2] = gstCommonParam->GST_FS_get_cover_rate(0);
	}
	sysinfostr.m_REMALNCAPACITY = buf[1];
	sysinfostr.m_COVERAGE = buf[2];
	sysinfostr.m_BADAREA = buf[3];
/*	
	for (i=2;i<5;i++)
	{
		Osd_Dec3_display(Info_Date_Pos[i*2]+offsetx,Info_Date_Pos[i*2+1]+offsety, WHITE, buf[i-1],0x0f,AD);
		Osd_Dec3_display(Info_Date_Pos[i*2]+offsetx_hd,Info_Date_Pos[i*2+1]+offsety_hd, WHITE, buf[i-1],0x0f,HD);
	}
*/
	memcpy(sysinfostr.m_RECORDSTARTTIME,time_buf[0],sizeof(time_buf[0]));
	memcpy(sysinfostr.m_RECORDENDTIME,time_buf[1],sizeof(time_buf[1]));
//	printf("DiskNum = %x\n",DiskNum);
	memset( time_buf, 0x00,sizeof(time_buf));
	if (NetStaticMenu.IPAlloction & 0x0f)
	{
//		I_read_byte_eeprom(ADDNSTC+SEVERID, &bt);
//		printf("&&&&&&&&&&&&&&&&&&&&&& bt : %d\n",bt);
		memset(server_ip,0x00,sizeof(server_ip));
		memset(mac_id,0x00,sizeof(mac_id));
		memset(netinfostr,0x00,sizeof(netinfostr));
		gstCommonParam->GST_NET_get_svr_mac_id(server_ip,mac_id);
		if(server_ip[0]==0)
			sprintf(server_ip,"__");
		if(mac_id[0]==0)
			sprintf(mac_id,"__");
//		if (bt)
//		{
			if (gstCommonParam->GST_NET_is_conneted_svr())
			{
				sprintf(netinfostr,"%s:%s  %s\n",server_ip,mac_id,"@");
			}
			else
				sprintf(netinfostr,"%s:%s  \n",server_ip,mac_id);
			printf("ddnsserver:%s\n",netinfostr);
//		}
//		else
//			sprintf(netinfostr,"%s:%s  \n",server_ip,mac_id);
		memcpy(sysinfostr.m_DDNS,netinfostr,sizeof(sysinfostr.m_DDNS));
	}
//	else
//	{
		memset(time_buf[0],0x00,sizeof(time_buf[0]));
		sprintf(time_buf[0],"%d.%d.%d.%d",
								NetStaticMenu.IpAddress[0],NetStaticMenu.IpAddress[1],NetStaticMenu.IpAddress[2],NetStaticMenu.IpAddress[3]);	
//	}
	memcpy(sysinfostr.m_NVRIP,time_buf[0],sizeof(time_buf[0]));
//	printf("###########IP:%s,aa:%d\n",time_buf[0],NetStaticMenu.IPAlloction & 0x0f);
	sysinfostr.m_NVRPORT = NetStaticMenu.Port;
	if (NetStatusFlag)
	{
		bt=gstCommonParam->GST_NET_get_client_user_num();
//		printf("client user num is %x\n",bt);
//		for (i=0;i<bt;i++)
		if (bt)
		{
			memset(IPChar,0,sizeof(IPChar));
			ret=gstCommonParam->GST_NET_get_client_info_by_id(IPChar,0);
			sprintf (IPChar,"%s  ",IPChar);
			memcpy(sysinfostr.m_CLIENTIP,IPChar,sizeof(IPChar));
			printf("IPChar is %s,clientip:%s\n",IPChar,sysinfostr.m_CLIENTIP);
		}
	}
	memset(SoftVer,0x00,sizeof(SoftVer));
	memset(mac_id,0x00,sizeof(mac_id));
	getsoftver(SoftVer);
	ret = GetValueFromEtcFile("/mnt/mtd/version.txt","VERSION","APPVER",mac_id,10);
	if (ret == ETC_OK)
		sprintf(sysinfostr.m_SQFTWAVEVER,"%s / %s",SoftVer,mac_id);

	printf("sysinfostr.m_HDD_QTY = %d\n",sysinfostr.m_HDD_QTY);
	printf("sysinfostr.m_CAPACITY = %d\n",sysinfostr.m_CAPACITY);
	printf("sysinfostr.m_HDD_QTY = %d\n",sysinfostr.m_REMALNCAPACITY);
	printf("sysinfostr.m_CAPACITY = %d\n",sysinfostr.m_COVERAGE);
	printf("sysinfostr.m_HDD_QTY = %d\n",sysinfostr.m_BADAREA);
	printf("sysinfostr.m_RECORDSTARTTIME = %s\n",sysinfostr.m_RECORDSTARTTIME);
	printf("sysinfostr.m_RECORDENDTIME = %s\n",sysinfostr.m_RECORDENDTIME);
	printf("sysinfostr.m_DDNS = %s\n",sysinfostr.m_DDNS);
	printf("sysinfostr.m_NVRIP = %s\n",sysinfostr.m_NVRIP);
	printf("sysinfostr.m_NVRPORT = %d\n",sysinfostr.m_NVRPORT);
	printf("sysinfostr.m_CLIENTIP = %s\n",sysinfostr.m_CLIENTIP);
	printf("sysinfostr.m_SQFTWAVEVER = %s\n",sysinfostr.m_SQFTWAVEVER);

}
/*
void HddinfoRClick(U16 x,U16 y)
{
	U8 i;
	if ((x>=Info_Date_Pos[0]+offsetx) && (y>=Info_Date_Pos[1]+offsety) && (x<=Info_Date_Pos[0]+126+offsetx) && (y<=Info_Date_Pos[3]+ offsety))
	{
		i = CurHddInfo;
		if (CurHddInfo<(Disk_Num-1))
			CurHddInfo++;
		else
			CurHddInfo = 0;
		if (CurHddInfo != i)
			Hdd_Info_Show();
	}
}
*/
void HddinfoLClick(U16 x,U16 y)
{
	U16 minx,miny;
	if (MseType)
	{
		minx = Info_Date_Pos[28]+offsetx_hd;
		miny = Info_Date_Pos[29]+offsety_hd;
	}
	else
	{
		minx = Info_Date_Pos[28]+offsetx;
		miny = Info_Date_Pos[29]+offsety;
	}
	if ((x>=minx) && (y>=miny) && (x<=minx+160) && (y<=miny+24))
	{
		cur_pos = 0;
		curpor = 0;
		menu_num = 0;
	    	OSD_ALL_CLEAR();
		demo_osd_disp();
	}

}


void Mouse_HddInfo_Move_Func(U16 x,U16 y)
{
	U16 minx,miny;
		if (MseType)
		{
			minx = Info_Date_Pos[28]+offsetx_hd;
			miny = Info_Date_Pos[29]+offsety_hd;
		}
		else
		{
			minx = Info_Date_Pos[28]+offsetx;
			miny = Info_Date_Pos[29]+offsety;
		}
	if ((x>=minx) && (y>=miny) && (x<=miny+160) && (y<=miny+24))
	{
		if (curpor != 0x10)
		{
			curpor = 0x10;
		}
	}
	else
	{
		if (curpor != 0x11)
		{
			curpor = 0x11;
		}
	}
}

void mHDDINFO_key_func()
{
	switch(byKey_val)
	{
		case kET:
			if (curpor == 0x11)
				break;
		case kESC:
			cur_pos = 0;
			menu_num = 0;
		    	OSD_ALL_CLEAR();
			demo_osd_disp();
			byKey_val=0;
			break;
/*
		case kAD:
			if (curpor==0x10)
			{
				cur_pos = 0;
				curpor = 0;
				menu_num = 0;
			    	OSD_ALL_CLEAR();
				demo_osd_disp();
			}
			break;
*/
		case kIN:
			if (Disk_Num && curuser==0)
				menu_event_on();
			break;
		case kUP:
		case kDN:
		case kLT:
		case kRH:
			if (curpor == 0x11)
			{
				Bmp_All_Show(Info_Date_Pos[28]+offsetx,Info_Date_Pos[29]+offsety,systemmenupic.exitfore,AD);
				Bmp_All_Show(Info_Date_Pos[28]+offsetx_hd,Info_Date_Pos[29]+offsety_hd,systemmenupic.exitfore,HD);
				curpor = 0x10;
			}
			break;
		default:
			break;
	}
}

