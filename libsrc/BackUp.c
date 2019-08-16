#include "cmn.h"

U16 osdoffx,osdoffy;
U16 osdoffx_hd,osdoffy_hd;

U8 bakerrtype,bakdevice;

U16 filepos[]=
{
	70,10,
	70,72,	242,72,

	70,28,

	70,10,
	160,72,
};

int bakingstatus=0xff;
extern U8 BackType;
#define BAKINFOLINES	9

void FixDiskInfo()
{
}

void BackUpOn()
{
	int ret, flag, status;

	U8 tmp = Play_Mode/100;
//	printf("******************************************** tmp:%d\n",tmp);
	if ((demo_num < PBFULL) || (tmp == 0))
		return;
	BackType = 3;
	if (!bakdevicecheck())
		return;

	gstCommonParam->GST_FTC_PlayUsbWrite(1);
	
	ret = gstCommonParam->GST_FTC_GetPlayCurrentStauts(&status,&flag);
	if (!flag)
	{
	}
	ret = gstCommonParam->GST_FTC_get_play_status();
	if (!ret || (ret && status))
	{
		return;
	}
	backupmenuon(BACKING,BackType);
/*	
	ret = gstCommonParam->GST_FTC_PlayUsbWriteStatus();
	
	switch(ret)
	{
		case USBBACKUPSTOP:
			backupmenuon(BACKING,BackType);
			printf("ret = USBBACKUPSTOP\n");
//			WriteOSDBmpX_bmp(120,25,WHITE,BackUping,Play_Menu);
//			gstCommonParam->GST_FTC_PlayUsbWrite(BackUpFlag);
			break;
		case USBBACKUPING:
			printf("ret = USBBACKUPING\n");
//			WriteOSDBmpX_bmp(120,25,WHITE,BackUping,Play_Menu);
			break;
		case USBSPACENOTENOUGH:
			printf("ret = USBSPACENOTENOUGH\n");
			break;
		case USBWRITEERROR:
			printf("ret = USBWRITEERROR\n");
			break;
		case NOUSBDEV:
			printf("ret = NOUSBDEV\n");
			break;
	}
*/
}

void backupmenuoff()
{
	if (demo_num<PBFULL)
		menu_num = mMouseBak;
	else
		menu_num = 0;
	switch(bakerrtype)
	{
		case 0xff:
		case BACKING:
		case FILECOUNT:
			break;
		case BAKOK:
		case NOUSB:
		case BAKFAIL:
		case BakTimeErr:
		case SIZEMORE:
			break;
		default:
			break;
	}
}

void BackUpOff()
{
	if (bakdevice==3)
	{
		if (demo_num == PB9)
			gstCommonParam->GST_FTC_PlayUsbWrite(0);
		else
			usbbakstop();
	}
	backupmenuoff();
}

void backupmenuon(U8 errtype, U8 device)
{
	menu_num = mBAKSTAU;
	bakerrtype = errtype;
	bakdevice = device;
	bakingstatus=0xff;

	switch(errtype)
	{
		case BACKING:
			bakerrtype = 0xff;
			break;
		case BAKOK:
		case NOUSB:
		case BAKFAIL:
		case BakTimeErr:
		case SIZEMORE:
			break;
		case FILECOUNT:
			break;
	}
}

void backup_enter()
{
//	printf("@@@@@@@@@@@@@@ bakerrtype  =%d\n",bakerrtype );
	switch(bakerrtype)
	{
		case 0xff:
		case USBnotEnough:
		case BAKFAIL:
		case BACKING:
			BackUpOff();
			break;
		case BAKOK:
		case NOUSB:
		case BakTimeErr:
		case SIZEMORE:
			backupmenuoff();
			break;
		default:
			break;
	}
}

U8 sleeptime;
void BackUpStatusCheck()
{
	int ret;

	if ((sleeptime++)<0xfe)
		return;
	else
		sleeptime=0;
//	printf("############# bakerrtype  =%d\n",bakerrtype );
	if (bakerrtype != 0xff)
		return;
	if (bakdevice==3)
	{
		if (demo_num > L16)
		{
			ret = gstCommonParam->GST_FTC_get_play_status();
			if (ret)
				ret = gstCommonParam->GST_FTC_PlayUsbWriteStatus();
			else
				ret = USBBACKUPSTOP;
		}
		else
			ret = gstCommonParam->GST_FS_UsbWriteStatus();
//		printf("******************* bakingstatus  =%d,ret=%d\n",bakingstatus,ret );
		if (bakingstatus == ret)
			return;
//		printf("******************* bakerrtype  =%d\n",bakerrtype );
		bakingstatus = ret;
		switch(ret)
		{
			case USBSPACENOTENOUGH:
//				printf("****************** stop USBSPACENOTENOUGH!\n");
				bakerrtype = USBnotEnough;
				break;
			case USBWRITEERROR:
				bakerrtype = BAKFAIL;
				break;
			case USBBACKUPSTOP:
				bakerrtype = BAKOK;
				break;
			case USBBACKUPING:
				break;
			default:
				break;
		}
	}
	else
	{
		ret = gstCommonParam->GST_FS_WriteCdStatus();
//		printf("check writecd status ret : %d\n",ret);
//		printf("******************* bakerrtype  =%d\n",bakerrtype );
//		printf("******************* bakingstatus  =%d,ret=%d\n",bakingstatus,ret );
		if (bakingstatus == ret)
			return;
		bakingstatus = ret;
		switch(ret)
		{
			case 0:
				bakerrtype = BAKOK;
				break;
			case 1:
//				bakerrtype = BACKING;
				break;
			case 2:
				bakerrtype = BAKFAIL;
				break;
			default:
				break;
		}
		
	}
}

void BackupLclick(U16 x, U16 y)
{
}

void BackUp_KeyFun()
{
	BackUpStatusCheck();
	if(byKey_val==0) return;
	printf("BackUp_KeyFun byKey_val = %x\n",byKey_val);

	switch(byKey_val)
	{
		case kET:
			backup_enter();
			break;
    }
	byKey_val=0;
}

