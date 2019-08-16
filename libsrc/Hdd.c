#include "cmn.h"

U8 Disk_Num,CurHdd,DiskCheck;
//U8 WatchDogTime_5s;

 
U8 InitalAlldeviceFlag=0;
//extern U8 threadcreat;
int flag_1080p=0,flag_480p=0,flag_576p=0,flag_960p=0;
int flag_ov2710 = 0;// incase 2M to 1M, osd also need change.
int flag_adc_cfg=0,flag_ircut_cfg=0,flag_light_cfg=0;

extern pthread_mutex_t eprom_file_edit_lock;
extern int eprom_file_edit_flag;
extern int STREAMFORMAT_WIDTH;
extern int STREAMFORMAT_HEIGHT;

static pthread_t g_Threadfixdisk = -1;
//U8 fixdisk=0;
void fixdisk_thread()
{
	int count=359;
	int net_check_count=0;
	
	SET_PTHREAD_NAME(NULL);
	while(1)
	{
		sleep(10);
		if(eprom_file_edit_flag)
		{
			pthread_mutex_lock(&eprom_file_edit_lock);
			gstCommonParam->GST_DRA_Net_cam_mtd_store();
			eprom_file_edit_flag=0;
			pthread_mutex_unlock(&eprom_file_edit_lock);
		}
		count++;
		if(count>=360)
		{
			count=0;
			gstCommonParam->GST_DRA_Clear_UPnP2(NetStaticMenu.Port);
			gstCommonParam->GST_DRA_Clear_UPnP2(NetStaticMenu.IEPort);
			gstCommonParam->GST_DRA_set_UPnP2(nvrip,NetStaticMenu.Port);
			gstCommonParam->GST_DRA_set_UPnP2(nvrip,NetStaticMenu.IEPort);
		}
		//net_check_count++;
		if(net_check_count>=60)
		{
			//printf("Connect. client failed \n");
			//printf("system reboot \n");
			//bReset = 1;
		}
		if(gstCommonParam->GST_DRA_Net_dvr_have_client())
			net_check_count=0;
	}
}

void creatfixdiskthread()
{
	int ret;
	ret = pthread_create(&g_Threadfixdisk, NULL,(void *) fixdisk_thread,NULL);
	if ( ret<0 )
	{
		printf( "creatfixdiskthread create error\n");
	}
}
#ifdef vodebug
extern U8 m1,m2;
#endif

void GetHddInfo()
{
	int a,b;
	if (SysMenu.VideoFormat)
		a=0;
	else
		a=1;

	if (RecamMenu[0].Reselution)
		b=0;//TD_DRV_VIDEO_SIZE_D1;
	else
		b=2;//TD_DRV_VIDEO_SIZE_CIF;
	
	gstCommonParam->GST_FTC_GetRecMode(a,b);
	gstCommonParam->GST_FTC_CurrentPlayMode(&a,&b);

	RecorSet();
	SetRecAudio();
	setliveaudio();
	get_demo_stamp(DEMO_DISP,0,0);
	demo_set();
	demo_osd_disp();
	event_write(POWER_ON,0,0);
	event_write(RESTORE_POWER_ON,0,0);
}

int hd1080p_check(void)
{
	FILE *file_fd = NULL;//fopen("sound.dat","w+b");
	
	file_fd=fopen("/mnt/mtd/1080p.txt","r");
	
	if(file_fd)
	{
		fclose(file_fd);
		return 1;
	}
	else
		return 0;
	
}

int hd480p_check(void)
{
	FILE *file_fd = NULL;//fopen("sound.dat","w+b");
	
	file_fd=fopen("/mnt/mtd/480p.txt","r");
	
	if(file_fd)
	{
		fclose(file_fd);
		return 1;
	}
	else
		return 0;
	
}
int hd576p_check(void)
{
	FILE *file_fd = NULL;//fopen("sound.dat","w+b");
	
	file_fd=fopen("/mnt/mtd/576p.txt","r");
	
	if(file_fd)
	{
		fclose(file_fd);
		return 1;
	}
	else
		return 0;
	
}
int hd960p_check(void)
{
	FILE *file_fd = NULL;
	
	file_fd=fopen("/mnt/mtd/960p.txt","r");
	
	if(file_fd)
	{
		fclose(file_fd);
		return 1;
	}
	else
		return 0;
	
}
int flag_adc_cfg_check(int *val)
{
	FILE *file_fd = NULL;
	char msg[20];
	
	file_fd=fopen("/mnt/mtd/adc_cfg.txt","r");
	
	if(file_fd)
	{
		memset(msg,0,sizeof(msg));
		fgets(msg,10,file_fd);
		if(msg[0]=='1')
			*val=1;
		else
			*val=0;
		
		fclose(file_fd);
		return 1;
	}
	else
		return 0;
}
int flag_ircut_cfg_check(int *val)
{
	FILE *file_fd = NULL;
	
	file_fd=fopen("/mnt/ircut_cfg_A.txt","r");
	
	if(file_fd)
	{
		*val=1;
		fclose(file_fd);
	}
	else
		*val=0;
		
	return 1;
}
int flag_normal_light_cfg_check(int *val)
{
	FILE *file_fd = NULL;
	char msg[20];
	file_fd=fopen("/mnt/mtd/light_cfg.txt","r");
	if(file_fd)
	{
		memset(msg,0,sizeof(msg));
		fgets(msg,10,file_fd);
		if(msg[0]=='1')
			*val=1;
		else
			*val=0;
		fclose(file_fd);
		return 1;
	}
	else
		return 0;
}


void InitAllDevice()
{
	int tmpVideoFormat,tmpReselution,ret;
	if (InitalAlldeviceFlag == 0xff)
		return;
	InitalAlldeviceFlag=0xff;
	if (SysMenu.VideoFormat)
		tmpVideoFormat=0;
	else
		tmpVideoFormat=1;

	if (RecamMenu[0].Reselution)
		tmpReselution=0;//TD_DRV_VIDEO_SIZE_D1;
	else
		tmpReselution=2;//TD_DRV_VIDEO_SIZE_CIF;
		
	flag_1080p=0;
	flag_480p=0;
	flag_576p=0;
	flag_960p=0;
	STREAMFORMAT_WIDTH=1280;
	STREAMFORMAT_HEIGHT=720;
	
	flag_adc_cfg=0;
	flag_adc_cfg_check(&flag_adc_cfg);
	flag_ircut_cfg=0;
	flag_ircut_cfg_check(&flag_ircut_cfg);
	flag_light_cfg=0;
	flag_normal_light_cfg_check(&flag_light_cfg);
	printf("********** flag_adc_cfg:%d ***********\n",flag_adc_cfg);
	
	if(hd1080p_check())
	{
		printf("**********1080p set***********\n");
		gstCommonParam->GST_DRA_Net_cam_set_support_max_video(6);
		flag_1080p=1;
		STREAMFORMAT_WIDTH=1920;
		STREAMFORMAT_HEIGHT=1080;
	}
	else
	{
		if(hd480p_check())
		{
			DBGPRINT("**********480p set***********\n");
			gstCommonParam->GST_DRA_Net_cam_set_support_max_video(7);
			flag_480p=1;
		}
		else if(hd576p_check())
		{
			DBGPRINT("**********576p set***********\n");
			gstCommonParam->GST_DRA_Net_cam_set_support_max_video(8);
			flag_576p=1;
		}
		else if(hd960p_check())
		{
			DBGPRINT("**********960p set***********\n");
			gstCommonParam->GST_DRA_Net_cam_set_support_max_video(9);
			flag_960p=1;
			STREAMFORMAT_WIDTH=1280;
			STREAMFORMAT_HEIGHT=960;
		}
	}

	gstCommonParam->GST_DRA_set_dvr_max_chan(Cam_Num);

//	gstCommonParam->GST_DRA_set_rec_type(c);


	printf("######################tmpVideoFormat %d tmpReselution:%d,Cam_Num:%d\n",tmpVideoFormat,tmpReselution,Cam_Num);


	ret = gstCommonParam->GST_DRV_CTRL_InitAllDevice(NULL,tmpVideoFormat,tmpReselution);
	if ( ret < 0 )
	{
		printf( "main() GST_DRV_CTRL_InitAllDevice error\n" );
		exit( 1);
	}

//	printf( "*************************FTC_CreateAllThread,SysMenu.VideoFormat = %d\n",SysMenu.VideoFormat );
	ret = FTC_CreateAllThread();

	if ( ret < 0 )
	{
		printf( "main() threadmenu_module_init error\n" );
		exit(1);
	}
//	fixdisk = 1;
}

void checksdcard(void)
{
	int ret,tmp[10];
	U8 i,tt;

	memset(tmp,0,sizeof(tmp));
	gstCommonParam->GST_FS_PhysicsFixAllDisk();
	
	gstCommonParam->GST_DISK_GetAllDiskInfo();
	Disk_Num = gstCommonParam->GST_DISK_GetDiskNum();
	CurHdd = 0;
	for (i=0;i<Disk_Num;i++)
	{
		gstCommonParam->GST_FS_UMountAllPatition(i);
		ret=gstCommonParam->GST_FS_CheckNewDisk(i);
		
		if (ret==1)
		{
			gstCommonParam->GST_FS_PartitionDisk(i);
			gstCommonParam->GST_FS_BuildFileSystem(i);
		}
		tmp[i] = gstCommonParam->GST_FS_MountAllPatition(i);
		pstDiskInfo[i]=gstCommonParam->GST_FS_GetDiskInfoPoint(i);
		if (tmp[i]<0)
		{
			Disk_Num = 0;
			return;
		}
	}
	ret = 0;
	tt = 0;
	for (i=0;i<Disk_Num;i++)
	{
		if (tmp[i]>=0)
		{
			ret++;
			if (tt==0)
			{
				CurHdd = i;
				tt = 1;
			}
		}
	}
	Disk_Num = ret;
}


void DiskCheckNew()
{
	OSD_ALL_CLEAR();

	RecorSet();
	SetRecAudio();
	setliveaudio();
	get_demo_stamp(DEMO_DISP,0,0);
	demo_set();
	demo_osd_disp();
	event_write(RESTORE_POWER_ON,0,0);
	event_write(POWER_ON,0,0);

}


