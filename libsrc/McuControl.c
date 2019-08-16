#include "cmn.h"
//#include <linux/watchdog.h>

#define    WATCHDOG_IOCTL_BASE    'W'

struct watchdog_info {
    HI_U32 options;        /* Options the card/driver supports */
    HI_U32 firmware_version;    /* Firmware version of the card */
    HI_U8  identity[32];    /* Identity of the board */
};

#define    WDIOC_GETSUPPORT     _IOR(WATCHDOG_IOCTL_BASE, 0, struct watchdog_info)
#define    WDIOC_GETSTATUS      _IOR(WATCHDOG_IOCTL_BASE, 1, int)
#define    WDIOC_GETBOOTSTATUS  _IOR(WATCHDOG_IOCTL_BASE, 2, int)
//#define    WDIOC_GETTEMP        _IOR(WATCHDOG_IOCTL_BASE, 3, int)
#define    WDIOC_SETOPTIONS     _IOWR(WATCHDOG_IOCTL_BASE, 4, int)
#define    WDIOC_KEEPALIVE      _IO(WATCHDOG_IOCTL_BASE, 5)
#define    WDIOC_SETTIMEOUT     _IOWR(WATCHDOG_IOCTL_BASE, 6, int)
#define    WDIOC_GETTIMEOUT     _IOR(WATCHDOG_IOCTL_BASE, 7, int)
//#define    WDIOC_SETPRETIMEOUT  _IOWR(WATCHDOG_IOCTL_BASE, 8, int)
//#define    WDIOC_GETPRETIMEOUT  _IOR(WATCHDOG_IOCTL_BASE, 9, int)
//#define    WDIOC_GETTIMELEFT    _IOR(WATCHDOG_IOCTL_BASE, 10, int)

#define    WDIOF_UNKNOWN        -1    /* Unknown flag error */
#define    WDIOS_UNKNOWN        -1    /* Unknown status error */

#define    WDIOF_OVERHEAT       0x0001    /* Reset due to CPU overheat */
#define    WDIOF_FANFAULT       0x0002    /* Fan failed */
#define    WDIOF_EXTERN1        0x0004    /* External relay 1 */
#define    WDIOF_EXTERN2        0x0008    /* External relay 2 */
#define    WDIOF_POWERUNDER     0x0010    /* Power bad/power fault */
#define    WDIOF_CARDRESET      0x0020    /* Card previously reset the CPU */
#define    WDIOF_POWEROVER      0x0040    /* Power over voltage */
#define    WDIOF_SETTIMEOUT     0x0080  /* Set timeout (in seconds) */
#define    WDIOF_MAGICCLOSE     0x0100    /* Supports magic close char */
#define    WDIOF_PRETIMEOUT     0x0200  /* Pretimeout (in seconds), get/set */
#define    WDIOF_KEEPALIVEPING  0x8000    /* Keep alive ping reply */

#define    WDIOS_DISABLECARD    0x0001    /* Turn off the watchdog timer */
#define    WDIOS_ENABLECARD     0x0002    /* Turn on the watchdog timer */
#define    WDIOS_TEMPPANIC      0x0004    /* Kernel panic on temperature trip */



U8 Led1,Led2,Led_Net1,Led_Net2;
U8 g_tw2835_mask=1;
U8 SysReboot=0;
int Wdgid = -1;
U8 Keyinfo = 2;

extern int adc_fd;

void wdginit()
{
	unsigned int time_sec=0;
	struct watchdog_info wi;
	int ret = 0;
	Wdgid=open("/dev/watchdog", O_RDWR );
//	Wdgid=open("/dev/misc/watchdog", O_RDWR );
	if(Wdgid <0)
		DPRINTK("Open wdg Fail! %x.\n", Wdgid);
	else
	{
		DPRINTK("Open wdg ok , Wdgid=%d\n",Wdgid);
#ifdef GM8126_DEV
		time_sec=60;
		if(Wdgid>0) ioctl(Wdgid, WDIOC_KEEPALIVE, &time_sec);
#endif
#ifdef HISI_DEV

		ioctl(Wdgid,WDIOC_GETSUPPORT,&wi);  
     		DPRINTK("%d,%s\n",wi.options,wi.identity);  

		time_sec=15; //3516ev100 的看门狗只能设置15秒以下的超时，设置30，60秒则看门狗不起作用
		if(Wdgid>0) 
		{
			ret = ioctl(Wdgid, WDIOC_SETTIMEOUT, &time_sec);
			if(ret != 0 )
			{
				DPRINTK("WDIOC_SETTIMEOUT error!\n");
			}

			DPRINTK("WDIOC_GETTIMEOUT %d\n",ioctl(Wdgid,WDIOC_GETTIMEOUT,&time_sec));  
     			DPRINTK("timeout = %d\n",time_sec); 
		}
#endif
	}
}

void McuStart(void)
{
}

void McuReset()
{
	printf("************ McuReset ***************\n");
	if (ChipNum == SLAVE || SysReboot)
		return;
	SysReboot = 1;
	
	I_read_block_eeprom(ADDNSTC,sizeof(NetStaticMenu),&NetStaticMenu.IPAlloction);
	DBGPRINT("DVRPORT=%d,IEPORT:%d\n",NetStaticMenu.Port,NetStaticMenu.IEPort);
	
	gstCommonParam->GST_DRA_Net_cam_mtd_store();
	printf("mcu reset\n");

	/*{//lx 20151120 
		sys_command("killall hostapd");
		sys_command("killall wpa_supplicant");

		sleep(1);
//hisi		sys_command("rmmod 8192du.ko");
		sys_command("rmmod 8192du");//goke

		sleep(3);
	}*/

	sys_command("reboot");
	exit(0);//防止上面的reboot不成功
	
}

U8 remo_Led=0;


void LedControl(U8 KeyFlag,U8 SetorCle)
{
	if (ChipNum == SLAVE || SysReboot)
		return;
	if (((KeyFlag == LEDUSB) && SetorCle) || ((remo_Led == SetorCle) && (KeyFlag == LEDIR)))
		return;
	switch(KeyFlag)
	{
	
		case LEDALARM:
			if (Keyinfo==1)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT0);
					SetBit(Led_Net1, BIT0);
				}
				else
				{
					ClearBit(Led1, BIT0);
					ClearBit(Led_Net1, BIT2);
				}
			}
			else if (Keyinfo==0)
			{
				if (SetorCle)
				{
	//				SetBit(Led1, BIT2);
					SetBit(Led_Net1, BIT0);
				}
				else
				{
	//				ClearBit(Led1, BIT2);
					ClearBit(Led_Net1, BIT2);
				}
			}
			else if (Keyinfo==2)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT2);
					SetBit(Led_Net1, BIT0);
				}
				else
				{
					ClearBit(Led1, BIT2);
					ClearBit(Led_Net1, BIT2);
				}
			}
			else if (Keyinfo==3)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT1);
					SetBit(Led_Net1, BIT0);
				}
				else
				{
					ClearBit(Led1, BIT1);
					ClearBit(Led_Net1, BIT2);
				}
			}
			break;
		case LEDPTZ:
			if (Keyinfo==3)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT4);
					SetBit(Led_Net1, BIT1);
				}
				else
				{
					ClearBit(Led1, BIT4);
					ClearBit(Led_Net1, BIT1);
				}
			}
			else
			{
				if (SetorCle)
				{
//					SetBit(Led1, BIT4);
					SetBit(Led_Net1, BIT1);
				}
				else
				{
//					ClearBit(Led1, BIT4);
					ClearBit(Led_Net1, BIT1);
				}
			}
			break;
		case LEDSEQ:
			if (Keyinfo==3)
			{
				if (SetorCle)
				{
					SetBit(Led2, BIT5);
					SetBit(Led_Net1, BIT2);
				}
				else
				{
					ClearBit(Led2, BIT5);
					ClearBit(Led_Net1, BIT2);
				}
			}
			else
			{
				if (SetorCle)
				{
//					SetBit(Led1, BIT4);
					SetBit(Led_Net1, BIT2);
				}
				else
				{
//					ClearBit(Led1, BIT4);
					ClearBit(Led_Net1, BIT2);
				}
			}
			break;

		case LEDIR:
			if (Keyinfo==1)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT3);
					SetBit(Led_Net1, BIT3);
					remo_Led = 1;
				}
				else
				{
					ClearBit(Led1, BIT3);
					ClearBit(Led_Net1, BIT3);
					remo_Led = 0;
				}
			}
			else if (Keyinfo==0)
			{
				if (SetorCle)
				{
					SetBit(Led2, BIT6);
					SetBit(Led_Net1, BIT3);
					remo_Led = 1;
				}
				else
				{
					ClearBit(Led2, BIT6);
					ClearBit(Led_Net1, BIT3);
					remo_Led = 0;
				}
			}
			else if (Keyinfo==2)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT4);
					SetBit(Led_Net1, BIT3);
					remo_Led = 1;
				}
				else
				{
					ClearBit(Led1, BIT4);
					ClearBit(Led_Net1, BIT3);
					remo_Led = 0;
				}
			}
			else if (Keyinfo==3)
			{
				if (SetorCle)
				{
					SetBit(Led2, BIT6);
					SetBit(Led_Net1, BIT3);
					remo_Led = 1;
				}
				else
				{
					ClearBit(Led2, BIT6);
					ClearBit(Led_Net1, BIT3);
					remo_Led = 0;
				}
			}
			else if (Keyinfo==4 || Keyinfo==5)
			{
				if (SetorCle)
				{
//					SetBit(Led2, BIT6);
					SetBit(Led_Net1, BIT3);
					remo_Led = 1;
				}
				else
				{
//					ClearBit(Led2, BIT6);
					ClearBit(Led_Net1, BIT3);
					remo_Led = 0;
				}
			}
			break;
		case LEDPLAY:
			if (Keyinfo==3)
			{
				if (SetorCle)
				{
					SetBit(Led2, BIT7);
					SetBit(Led_Net1, BIT4);
				}
				else
				{
					ClearBit(Led2, BIT7);
					ClearBit(Led_Net1, BIT4);
				}
			}
			else
			{
				if (SetorCle)
				{
//					SetBit(Led1, BIT4);
					SetBit(Led_Net1, BIT4);
				}
				else
				{
//					ClearBit(Led1, BIT4);
					ClearBit(Led_Net1, BIT4);
				}
			}
			break;
		case LEDREC:
			if (Keyinfo==1)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT5);
					SetBit(Led_Net1, BIT5);
				}
				else
				{
					ClearBit(Led1, BIT5);
					ClearBit(Led_Net1, BIT5);
				}
			}
			else if (Keyinfo==0)
			{
				if (SetorCle)
				{
	//				SetBit(Led1, BIT1);
					SetBit(Led_Net1, BIT5);
				}
				else
				{
	//				ClearBit(Led1, BIT1);
					ClearBit(Led_Net1, BIT5);
				}
			}
			else if (Keyinfo==2)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT1);
					SetBit(Led_Net1, BIT5);
				}
				else
				{
					ClearBit(Led1, BIT1);
					ClearBit(Led_Net1, BIT5);
				}
			}
			else if (Keyinfo==3)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT5);
					SetBit(Led_Net1, BIT5);
				}
				else
				{
					ClearBit(Led1, BIT5);
					ClearBit(Led_Net1, BIT5);
				}
			}
			break;

		case LEDPWR:
			if (Keyinfo==1)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT7);
					SetBit(Led_Net1, BIT6);
				}
				else
				{
					SetBit(Led1, BIT6);
					ClearBit(Led1, 0xBf);
					Led2=0;
					Led_Net1=0;
					Led_Net2=0;
				}
				
			}
			else if (Keyinfo==0)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT6);
					SetBit(Led_Net1, BIT6);
				}
				else
				{
					SetBit(Led1, BIT7);
					ClearBit(Led1, 0x7f);
					Led2=0;
					Led_Net1=0;
					Led_Net2=0;
				}
			}
			else if (Keyinfo==3 || Keyinfo==4 || Keyinfo==5)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT6);
					SetBit(Led_Net1, BIT6);
				}
				else
				{
					SetBit(Led1, BIT7);
					ClearBit(Led1, 0x7f);
					Led2=0;
					Led_Net1=0;
					Led_Net2=0;
				}
			}
			else
			{
				if (!SetorCle)
				{
					SetBit(Led1, BIT7);
					ClearBit(Led1, 0x7f);
					Led2=0;
					Led_Net1=0;
					Led_Net2=0;
				}
				else
				{
					SetBit(Led1, BIT6);
					SetBit(Led_Net1, BIT6);
				}
			}
			break;

		case LEDUSB:
			if (SetorCle)
			{
//				SetBit(Led1, BIT0);
//				SetBit(Led_Net1, BIT7);
			}
			else
			{
//				ClearBit(Led1, BIT0);
//				ClearBit(Led_Net1, BIT7);
			}
			break;

		case LEDNET:
			if (Keyinfo==1)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT2);
					SetBit(Led_Net2, BIT0);
				}
				else
				{
					ClearBit(Led1, BIT2);
					ClearBit(Led_Net2, BIT0);
				}
			}
			else if (Keyinfo==0)
			{
				if (SetorCle)
				{
	//				SetBit(Led1, BIT5);
					SetBit(Led_Net2, BIT0);
				}
				else
				{
	//				ClearBit(Led1, BIT5);
					ClearBit(Led_Net2, BIT0);
				}
			}
			else if (Keyinfo==2)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT5);
					SetBit(Led_Net2, BIT0);
				}
				else
				{
					ClearBit(Led1, BIT5);
					ClearBit(Led_Net2, BIT0);
				}
			}
			else if (Keyinfo==3)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT3);
					SetBit(Led_Net2, BIT0);
				}
				else
				{
					ClearBit(Led1, BIT3);
					ClearBit(Led_Net2, BIT0);
				}
			}
			break;
		case LEDLOCK:
			if (Keyinfo==1)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT4);
					SetBit(Led_Net1, BIT7);
				}
				else
				{
					ClearBit(Led1, BIT4);
					ClearBit(Led_Net1, BIT7);
				}
			}
			else if (Keyinfo==0)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT0);
					SetBit(Led_Net1, BIT7);
				}
				else
				{
					ClearBit(Led1, BIT0);
					ClearBit(Led_Net1, BIT7);
				}
			}
			else if (Keyinfo==2)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT3);
					SetBit(Led_Net1, BIT7);
				}
				else
				{
					ClearBit(Led1, BIT3);
					ClearBit(Led_Net1, BIT7);
				}
			}
			else if (Keyinfo==3)
			{
				if (SetorCle)
				{
					SetBit(Led1, BIT0);
					SetBit(Led_Net1, BIT7);
				}
				else
				{
					ClearBit(Led1, BIT0);
					ClearBit(Led_Net1, BIT7);
				}
			}
			break;
		default:
			break;
	}
}

void setled(U8 l1,U8 l2)
{
}

void BuzzerControl(U8 flag)
{
}

void AlarmReset()
{
}


void RelayControl(U8 flag)
{
	int val;
	
	RegisterReadWrite.value=flag;
	if(gm8126id>0) ioctl(gm8126id, 11, &RegisterReadWrite);
	if(tw2865id>0) ioctl(tw2865id, 11, &RegisterReadWrite);
		
	val=flag;
	if(adc_fd>0)  
		ioctl(adc_fd,0x500,&val); 

}

extern U8 BufKeyinfo[];

void TurnOnOff()
{
}

void WatchDog()
{
	unsigned int time_sec = 60;
	int ret;
#ifdef HISI_DEV
	if(Wdgid>0)	
	{
	//WDIOC_KEEPALIVE
		ret = ioctl(Wdgid, WDIOC_KEEPALIVE, 0);	
		//ret = unlocked_ioctl(Wdgid, WDIOC_KEEPALIVE, 0);	
	
		if(ret != 0 )
		{
			printf("WDIOC_KEEPALIVE error! Wdgid=%d\n",Wdgid);
		}
	}
#endif



	//printf("WWWWWWWWWWWWWWWW    WatchDog! %d\n",Wdgid);
}


