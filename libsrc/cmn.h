//#include"SST89x5x4.H"
#include <sys/ioctl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include "devfile.h"
#include <sys/time.h>
#include <time.h>
//#include "gmcommon.h"
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include "devfile.h"
#include "filesystem.h"
#include "hi_rtc.h"
#include <sys/socket.h>
#include <netinet/in.h>
/*
//#include "tw2815.h"
//#include "tw2865_def.h"
#include "tw2865.h"
*/
//#include "gpm.h"

//#define vodebug

//#define UPDATE
//#define UART
//#ifdef UART
//#include "libext.h"
//#endif
//#include "vcap_osd.h" 
//#include "ioctl_isp210.h"

#include "hi_common.h"
#include "hi_comm_sys.h"
#include "hi_comm_vb.h"
#include "hi_comm_isp.h"
#include "hi_comm_vi.h"
#include "hi_comm_vo.h"
#include "hi_comm_venc.h"
#include "hi_comm_vpss.h"
#include "hi_comm_region.h"
#include "hi_comm_adec.h"
#include "hi_comm_aenc.h"
#include "hi_comm_ai.h"
#include "hi_comm_ao.h"
#include "hi_comm_aio.h"
#include "hi_defines.h"

#include "mpi_sys.h"
#include "mpi_vb.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "mpi_venc.h"
#include "mpi_vpss.h"
#include "mpi_region.h"
#include "mpi_adec.h"
#include "mpi_aenc.h"
#include "mpi_ai.h"
#include "mpi_ao.h"
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"
#include "mpi_af.h"

//#define GM8126_DEV	1	
#define HISI_DEV	1

#ifdef HISI_DEV
	#define ISP_IOC_SET_BRIGHTNESS      0xffff
	#define ISP_IOC_SET_CONTRAST        0xffff
	#define ISP_IOC_SET_HUE             0xffff
	#define ISP_IOC_SET_SATURATION      0xffff
	#define ISP_IOC_AE_IOCTL            0xffff
	#define AE_SET_MIN_GAIN							0xffff
	#define DATA_APP_TO_DRIVER					0xffff
	#define AE_SET_TARGETY							0xffff
	#define AE_SET_MODE									0xffff
	#define ISP_IOC_SET_SENSOR_MIRROR		0xffff
	
	#define CAM_NAME_MAX_W	360//160
	#define CAM_NAME_MAX_H	40//24
#endif

#define _DEBUG
//调试函数
#ifdef _DEBUG
#define DBGPRINT(fmt, arg...) printf("%s(%d) %s:"fmt, __FILE__, __LINE__,__FUNCTION__, ##arg)
#else
#define DBGPRINT(fmt, arg...)
#endif
//==================================================================================
//						Common definition & variable description
//==================================================================================
#define	BIT0		(0x0001)
#define	BIT1		(0x0002)
#define	BIT2		(0x0004)
#define	BIT3		(0x0008)
#define	BIT4		(0x0010)
#define	BIT5		(0x0020)
#define	BIT6		(0x0040)
#define	BIT7		(0x0080)
#define	BIT8		(0x0100)
#define	BIT9		(0x0200)
#define	BIT10		(0x0400)
#define	BIT11		(0x0800)
#define	BIT12		(0x1000)
#define	BIT13		(0x2000)
#define	BIT14		(0x4000)
#define	BIT15		(0x8000)


#define MASTER		0x32
#define SLAVE		0x68
#define MASTER1		0x322

//===================================================================================

#define mMAIN 1
#define mPOWER 2

#define mTEST 11
#define mCAMERA 12
#define mSYSTEM 13
#define mRECORD 14
#define mEVENT 15
#define mRESET 16
#define mVFILTER 17
#define mMIRROR 18
#define mTESTOSD 19
#define mPROTOCOL 30
#define mHVOFFSET 80

#define mPICSET 21
#define mMOTION 22
#define mPRIOR 23
#define mBAKSTAU 24
#define mCALL 25
#define mSENSOR 26
#define mAUTO 27
#define mSTATIC 29
#define mPTZ	31
#define mEMAIL	32
#define mUSER 33
#define mHDD	34
#define mMTPLAN 35
#define mSENPLAN 36
#define mMouseBak	  37
#define mMENUALARM 38
#define mALARMSEARCH 39
#define mSCHEDULE		40
#define mDISKFORMAT		41
#define mZOOM		42
#define mCdBAK		43
#define mSPOT		44
#define mRECSEARCH	45
#define mERRINFO	46
#define mAUDIO		47
#define mPASS		48
#define mTIMEPLAY	49
#define mHDDINFO	50
#define mSEARCH  	51
#define mPTZControl  52
#define mMouse  		53
#define mMouseCam 	54
#define mMouseOth 	55
#define mMousePTZ	58
#define Iconmenu	59
#define SaveNetSetup		60
#define GetNetSetup		61
#define GetAlarm			62
#define GetSysVer		63
#define SetNetRec		64
#define SetDVRTime		65
#define StartSendAlarmMsg			76
#define StopSendAlarmMsg			77

#define RTC_RAM_REMOTE_FLAG_ADDR	0x19
#define RTC_RAM_SPOT_FLAG_ADDR	0x20


#define Emotion  0
#define Esensor  1
#define Evloss   2
#define Etime    3
#define Emanual  4
#define Eidle    5
#define Eclear   6
#define Evon   7
#define Erecon   8
#define Erecoff   9


#define WordHigh		24
#define WordWith		12
#define MouseMenuWidth	32//48
#define MouseMenuHight	34//32


#define POSXRMENU	650
#define POSYRMENU	35

#define ICON_MENU_OFFSET	14

#define RecodSeaFileCount	300

//#define TEST

#define PAL_MOUSE_LEFT		0
#define PAL_MOUSE_RIGHT	176
#define PAL_MOUSE_TOP		0
#define PAL_MOUSE_BOTTOM	18

#define NTSC_MOUSE_LEFT	0
#define NTSC_MOUSE_RIGHT	176
#define NTSC_MOUSE_TOP		0
#define NTSC_MOUSE_BOTTOM 15

#define	Mouse_L	0
#define 	Mouse_R	1
#define 	Mouse_T	2
#define 	Mouse_B	3


#define MOUSECAMMENUHIGH	7
#define WIDTH_RESOLUTION 	720



//================================================================================
//#define  kZM		0x001c
//#define  kFPLAY	0x0026

#define BXS8604	0
#define BXS7608	1
#define BXS7204 	2
#define BXS8xxx  3
#define BXS81xx  4
#define BXSThi	5


#define  k10A	0x0f
#define  k10 		0x10//0x23
#define  k01 		0x11//0x07
#define  k02 		0x12//0x04
#define  k03 		0x13//0x25
#define  k04 		0x14//0x21
#define  k05 		0x15//0x06
#define  k06 		0x16//0x03
#define  k07 		0x17//0x26
#define  k08 		0x18//0x22
#define  k09 		0x19//0x24
#define  kUP 		0x20//0x09
#define  kDN 		0x21//0x0d
#define  kLT 		0x22//0x0f
#define  kRH 		0x23//0x0b
#define  kET 		0x24//0x0c
#define  kESC 	0x25//0x10
#define  kMU 		0x26//0x0a
#define  kREC 	0x27//0x0e
#define  kPLAY 	0x28//0x08
#define  kSTOP 	0x29//0x05
#define  kIN 		0x30//0x02
#define  kCHAD	0x31
#define  kCHDE	0x32
#define  kPW 	0x800
#define  kAUDIO 	0x801
#define  kLOCK 	0x802
#define  kSQ 		0x803
#define  kSpot 	0x804
#define  kSEA 	0x805
#define  kPTZ 	0x806
#define  kBAK	0x807
#define  kCRUISE 0x808
#define  kZOOMIN 0x809
#define  kZOOMOUT 0x80a
#define  kFOUCEIN  0x80b
#define  kFOUCEOUT 0x80c
#define  kIROUT	0x80d
#define  kIRIN	0x80e
#define  kFOPEN	0x80f
#define  kFCLOSE	0x810
#define  kPTZAUTO 0x811
#define  kCR		0x812

#define  k11    	0x0101
#define  k12    	0x0102//0x000c
#define  k13    	0x0103
#define  k14    	0x0104
#define  k15    	0x0105
#define  k16    	0x0106
#define  k13q	0x011c
#define  k16q    	0x0117
#define  k8q		0x0119
#define  k4q    	0x0114
#define  kAD		0x0217
#define  kDE		0x0219
#define  kRFAST	0x011a
#define  kFFAST	0x0115
#define  kIR		0x0116


#define kSWAP	0x0112
//#define kLOCK	0x0113
#define KNUM0	0x0200
#define KNUM1	0x0201
#define KNUM2	0x0202
#define KNUM3	0x0203
#define KNUM4	0x0204
#define KNUM5	0x0205
#define KNUM6	0x0206
#define KNUM7	0x0207
#define KNUM8	0x0208
#define KNUM9	0x0209
#define kNLT		0x000f//0x0301
#define kNRH		0x000b//0x0302
#define kNUP		0x0009//0x0303
#define kNDN		0x000d//0x0304



#define kMLeftdn		0x0320
#define kMLeftup		0x0321
#define kMRightdn	0x0322
#define kMRightup	0x0323


//================================================================
#define LFULL		0x00
#define L4			0x01
#define L8	 		0x02
#define L13			0x03
#define L16			0x04

#define PBFULL		0x05
#define PB2			0x06
#define PB4			0x07
#define PB9			0x0d
#define PB16		0x0e
#define PB8			0x0f
#define PB13		0x10
#define PBPIP1		0x11
#define LVPB88		0x12
#define RECORD		0x13

#define LEDALARM		1
#define LEDPTZ		2
#define LEDSEQ		3
#define LEDIR		4
#define LEDPLAY		5
#define LEDREC		6
#define LEDPWR		7
#define LEDUSB		8
#define LEDNET		9
#define LEDLOCK		10
//==================================================================================
#define U8  unsigned char
#define U16 unsigned int
#define S8  signed char
#define S16 signed int
#define UC unsigned char
#define US unsigned short
//typedef bit boolean;
//#define BIT bit

//==================================================================================
//#define	NULL		0
#define	ON			1
#define	OFF		0
//==================================================================================
#define	VDO_NTSC			0
#define	VDO_PAL				1


//==================================================================================
#define	SetBit(x,y)			((x) |= (y))         
#define	ClearBit(x,y)		((x) &= ~(y))        
#define	BitSet(x,y)			(((x)&(y))== (y))    
#define	BitClear(x,y)		(((x)&(y))== 0)      
#define	IfBitSet(x,y)		if(((x)&(y)) == (y)) 
#define	IfBitClear(x,y)	if(((x)&(y)) == 0)
#define 	BIT(X)	(1<<X)
#define 	WBIT(X) ((U16)(1<<X))
//==================================================================================

#define  WHITE				0
#define  GREEN				3
#define  RED					1
#define  RED2				0x40//0x04
#define  GRAY				0


#define  BLACK				0x102020
#define  BLUE				0x3ca55f

#define MIX_WHITE		0
#define MIX_RED			1
#define BLACK_WHITE		2
#define BLUE_WHITE		3
#define RED_WHITE		4

#define MIXCOLOR		RGB(250,2,2)

#define WHOLELINES	42//26

//////////////////////////////////////

//-----------date time---------------
#define TIMEDISPNON 0
#define TIMEDISPTOP 1
#define TIMEDISPBOTTOM 2
#define TIMEDISPTOPLEFT 3
#define TIMEDISPTOPRIGHT 4
#define TIMEDISPBOTTOMLEFT 5
#define TIMEDISPBOTTOMRIGHT 6
#define TIMEDISPMENU 7
/*
#define YEAR 1
#define MONTH 2
#define DAY 3
#define HOUR 4
#define MINUTE 5
#define SECOND 6
*/
//---------------EEPROM----------------
#define	MAXYEAR    37
#define	MAXMONTH   12
#define	MAXDAY     31
#define	MAXHOUR    23
#define	MAXMINUTE  59
#define	MAXSECOND  59

#define	ADDRYEAR    6
#define	ADDRMONTH   5
#define	ADDRDAY     4
#define	ADDRHOUR    2
#define	ADDRMINUTE  1
#define	ADDRSECOND  0
#define 	RTC				372
#define	DALAS			372

#define NTSC	  		0
#define PAL		  	1


#define BackUping				9
#define BackUpStoping			10
#define Blanking				11
#define AUTOSeq				12
#define PtzControl			13
#define PtzCam				14
#define PtzBraud				15
#define PtzProto				16
#define CheckHdd			17
#define NoHdd				18
#define HddFull				19
#define Recording			20
#define PlaySearch			21


/////////////////////////////////alarm.c
#define DEAL_INVAL 5
#define DISP_TIME 5
#define BUZZER_TIME 20
#define RELAY_TIME 30
#define ALARM_RECORD_TIME 2//90//30

#define RECOFF				0
#define MANUALREC			1
#define ALARMREC			2
#define MOTIONREC			3
#define TIMEREC				4
#define PREREC				5
#define LOSSREC				6

//#define D1MODE				1

#define DAY 1
#define NIGHT 0
#define MAXCOUNT 10





#define _DEBUG
//调试函数
#ifdef _DEBUG
#define DBGPRINT(fmt, arg...) printf("%s(%d) %s:"fmt, __FILE__, __LINE__,__FUNCTION__, ##arg)
#else
#define DBGPRINT(fmt, arg...)
#endif


#define	INIT_IPC_ORD1	0xaf
#define	INIT_IPC_ORD2	0xad
#define	INIT_IPC_ORD3	0xab
#define	INIT_IPC_ORD4	0xa9
#define INIT_IPC_ALL	0x3a
#define INIT_IPC_TIME	0x0a
#define INIT_IPC_TITLE	0x4a
#define SET_IPC_RELAY	0x4b
#define SET_IPC_PTZ		0x7f
#define GET_IPC_ALARM	0x7e
#define GET_IPC_MOTION	0x7d
#define SET_PTZ_LEFT	0x51
#define SET_PTZ_RIGHT	0x52
#define SET_PTZ_UP		0x53
#define SET_PTZ_DOWN	0x54
#define SET_PTZ_FAR		0x55
#define SET_PTZ_NEAR	0x56
#define SET_PTZ_ZOOMOUT	0x57
#define SET_PTZ_ZOOMIN	0x58
#define SET_PTZ_IROPEN	0x59
#define SET_PTZ_IRCLOSE	0x5a
#define SET_PTZ_AUTO	0x5b
#define SET_PTZ_AUTO1	0x5c
#define SET_PTZ_STOP	0x5d
#define SET_PTZ_STOP1	0x5e
#define SET_PTZ_PRESET	0x5f
#define SET_PTZ_PREVIEW	0x60
typedef struct _registerrw_{/*register read and write*/
	unsigned char addr;//register address
	unsigned char value;//register value
	unsigned char ChipNo;//chip number, base on zero
	unsigned char reserve[5];
}registerrw_t;
typedef struct _IMAGE_DATA_
{
	int w;
	int h;
	unsigned char *buf;
}IMAGE_DATA;

extern  U16 byKey_val;

////////////////////////////////////////////

/////////////////////////struct////////////////////////////////////////////

typedef enum hiVOU_DEV_E
{
	HD = 0,
	AD = 1,
	SD = 2,
	MD = 3,
	HMD = 4,
	VOU_DEV_BUTT
}VOU_DEV_E;



typedef struct
{
	U8 Knum;
	U8 Kget1;
	U8 Kget2;
	U8 Kget3;
	U8 Kget4;
	U8 MenuNum;
	U8 PlayStau;
	U16 Kset;
} KeyValSys;


typedef struct
{
	U8 tmregion;
	U8 tmdisp;
	U8 bordcolor;
	U8 VideoFormat;
	U8 RemoteID;
	U8 spotpop;
	U8 mainpop;
	U8 osdtran;
	U8 HDOutput;
	U8 MouseDisp;
	U8 Laguage;
} ValSystem; 

typedef enum
{
	TIMFORMAT,
	TIMDISP,
	BORDDISP,
	VIDEOFORMAT,
	REMOTEID,
	SPOTPOP,
	MAINPOP,
	OSDTRAN,
	HDOUTPUT,
	MOUSEDISP,
	LAGUAGE
}Adrsys;

typedef struct
{
	U8 CamTitle[6];
	U8 Brightness;
	U8 Hue;
	U8 Contrast;
	U8 Satutrat;
	U8 InputMatch;
	U8 PicHdel;
	U8 PicVdel;
	U8 SpotTime;
	U16 Starx;
	U16 Stary;
	U16 Endx;
	U16 Endy;
	U8 dayval;
	U8 nightval;
	U8 backlight;
} ValCammra;

typedef enum
{
	CAMTITLE,
	BRIGHTNESS=5,
	HUE,
	CONTRAST,
	SATURAT,
	INPUTMATCH,
	PICHDEL,
	PICVDEL,
	SPOTTIME,
	STARTX,
	STARTY=15,
	ENDX=17,
	ENDY=19,
	DAYV=21,
	NIGHTV
}Adrcam;

typedef struct
{
	U8 Sensitivity;
	U8 StarX_H;
	U8 StarX_L;
	U8 EndX_H;
	U8 EndX_L;
	U8 StarY_H;
	U8 StarY_L;
	U8 EndY_H;
	U8 EndY_L;
} ValMotion;

typedef enum
{
	SENSITIVITY,
	STARXH,
	STARXL,
	ENDXH,
	ENDXL,
	STARYH,
	STARYL,
	ENDYH,
	ENDYL,
}Adrmot;

typedef struct
{
	U8 KeepTime_A;
} ValAuto;

typedef struct
{
	U8 BaudRate;
	U8 DeviceID;
	U8 Protocol;
	U8 Speed;
} ValPTZ;

typedef enum
{
	BAUDRATE,
	DEVICEID,
	PROTOCOL,
	SPEED
} Adrptz;

typedef struct
{
	U8 HDDName;
	U16 HddSize;
	U8 RemanSize;
	U8 CoveRate;
	U8 BadSize;
	U8 Overwrite;
} ValHdd;

typedef struct
{
	U8 All_Date;
	U8 Record_Stytle[8];
	U8 Hour_Star[8][4];
	U8 Minute_Star[8][4];
	U8 Hour_End[8][4];
	U8 Minute_End[8][4];
	U8 Alarm_Stytle[8][4];
	U8 T1;
	U8 T2;
	U8 T3;
} ValRecSchedule;

typedef struct
{
	U8 IPAlloction;
	U8 IpAddress[4];
	U8 GateWay[4];
	U8 SubNet[4];
	U8 DDNS1[4];
	U8 DDNS2[4];
	U16 Port;
	U16 IEPort;
} ValNetStatic;

typedef struct
{
	unsigned char Wififlag;
	unsigned char AuthMode;
	unsigned char EncryType;
	unsigned char KeyID;
	unsigned char WifiID[20];
	unsigned char WifiPaw[20];
}ValWifi;

typedef struct 
{
	ValWifi rt73;
	ValNetStatic netall;
}Valnetwifi;


typedef enum
{
	IPALLOCTION,
	IPADDRESS,
	GATEWAY=5,
	SUBNET=9,
	DDNS1=13,
	DDNS2=17,
	PORT=21,
	IEPORT=23,
	DHCPIP=25,
	DHCPGATEWAY=29,
	DHCPDNS1=33,
	DHCPDNS2=37,
	SEVERID=41
} Adrnetstatic;

typedef struct
{
	U8 TimeTev;
	U16 EmailPort;
	U8 EServerAdd[30];
	U8 EUserName[30];
	U8 EPassWord[30];
	U8 EReciveAdd[30];
} ValEmail;

typedef enum
{
	TIMETEV,
	EMAILPORT,
	ESERVERADD=3,
	EUSERNAME=33,
	EPASSWORD=63,
	ERECIVEADD=93
} Adremail;
/*
#define MAX_TIMEZONE_NUM 34     //时区的数量
const char TimeZoneName[][16] =
{
    "GMT+00:00",// GMT, Dublin, Edinburgh, London, Lisbon, Monrovia, Casablanca", // GMT+0BST-1,M3.5.0/01:00:00,M10.5.0/02:00:00
    "GMT+01:00",// Berlin, Stockholm, Rome, Bern, Brussels, Vienna, Paris, Madrid, Amsterdam, Prague, Warsaw, Budapest", // CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00
    "GMT+02:00",// Athens, Helsinki, Istanbul, Cairo, Eastern Europe, Harare, Pretoria, Israel", // EET-2EEST-3,M3.5.0/03:00:00,M10.5.0/04:00:00
    "GMT+03:00",// Baghdad, Kuwait, Nairobi, Riyadh, Moscow, St. Petersburg, Kazan, Volgograd", // MSK-3MSD,M3.5.0/2,M10.5.0/3
    "GMT+03:30",
    "GMT+04:00",// Abu Dhabi, Muscat, Tbilisi",
    "GMT+04:30",
    "GMT+05:00",// Islamabad, Karachi, Ekaterinburg, Tashkent",
    "GMT+05:30",
    "GMT+05:45",
    "GMT+06:00",// Alma Ata, Dhaka",
    "GMT+06:30",
    "GMT+07:00",// Bangkok, Jakarta, Hanoi",
    "GMT+08:00",// Taipei, Beijing, Chongqing, Urumqi, Hong Kong, Perth, Singapore",
    "GMT+09:00",// Tokyo, Osaka, Sapporo, Seoul, Yakutsk",
    "GMT+09:30",
    "GMT+10:00",// Brisbane, Melbourne, Sydney, Guam, Port Moresby, Vladivostok, Hobart",
    "GMT+11:00",// Magadan, Solomon Is., New Caledonia",
    "GMT+12:00",// Fiji, Kamchatka, Marshall Is., Wellington, Auckland"
    "GMT+13:00",
    "GMT-01:00",// Azores, Cape Verdes Is.",
    "GMT-02:00",// Mid-Atlantic",
    "GMT-03:00",// Brasilia, Buenos Aires, Georgetown",
    "GMT-03:30",
    "GMT-04:00",// Atlantic Time (Canada), Caracas, La Paz", // AST4ADT,M4.1.0/00:01:00,M10.5.0/00:01:00
    "GMT-04:30",
    "GMT-05:00",// Eastern Time (US & Canada), Indiana(East), Bogota, Lima", // EST5EDT,M3.2.0,M11.1.0
    "GMT-06:00",// Central Time (US & Canada), Mexico City, Tegucigalpa, Saskatchewan", // CST6CDT,M3.2.0,M11.1.0
    "GMT-07:00",// Mountain Time (US & Canada), Arizona", // MST7
    "GMT-08:00",// Pacific Time (US & Canada), Tijuana", // PST8PDT,M3.2.0,M11.1.0
    "GMT-09:00",// Alaska", // AKST9AKDT,M3.2.0,M11.1.0
    "GMT-10:00",// Hawaii", // HAW10
    "GMT-11:00",// Midway Island, Samoa",
    "GMT-12:00",// Eniwetok, Kwajalein",
};
*/
typedef struct
{
    U8 NtpOn;    //是否开启ntp功能.0~1.
    U8 ServerAddr[32];//服务器地址
    U16 NtpPort;  //端口。0~65535.
    U8 TimeZonePos;//时区.0~34.
    U16 TimeTev;//更新周期:0~65535. 单位小时
    U16 NtpSec;//相差的秒，用于计算。+-3600.
}ValNtp;

typedef enum
{
	ADRSECOND,
	ADRMINUTE,
	ADRHOUR,
	ADRWEEK,
	ADRDAY,
	ADRMONTH,
	ADRYEAR
} Adrrtc;

typedef struct
{
	U8 PlayFlag;
	U8 PlayFSpeed;
	U8 PlayBSpeed;
	U8 PlayStatus;
} ValPlay;

typedef enum
{
	LIVE,
	PLAY,
	PAUSE,
	FAST,
	BACK,
	FRMPLAY
} StatusPlay;

typedef enum
{
	eEVENTHEAD,
	eEVENTLOG
} Adreven;

typedef struct
{
	U8 Reselution;
	U8 PicQty;
	U8 FrmRate;
	U8 AudioOnoff;
} MenuRec;

typedef enum
{
	RESELUTION,
	RECPICQTY,
	FRMRATE,
	AUDIOONOFF
} Adrreccam;

typedef enum
{
	FORMATING=1,
	FORMATOK,
	PASSERR,
	TimeErr,
	PermissonErr,
	StopRecord,
	RecErr,
	UpdateFail,
	UpdateOk,
	TimeSetErr,
	COPYRIGHTSHOW,
	Loaddefault,
	ConRec,
	HddFullRec,
	PASSWORDERR,
	SERCONNETCTERR,
	USERNOTEXITERR,
	SENDERR,
	UNKNOWNERR,
	INFONOTFULLERR,
	SENDTESTORNOT,
	TESTOK
} ErrInfo;

typedef enum
{
	BACKING=0,
	BAKOK,
	NOUSB,
	BAKFAIL,
	FILECOUNT,
	BakTimeErr,
	SIZEMORE,
	USBnotEnough,
//	UsbWriteErr,
	FIXDISKINFO
}BAKINFO;

typedef enum
{
	FourCh1,
	FourCh2,
	FourCh3,
	FourCh4,
	Full1,
	Full2,
	Full3,
	Full4,
	Full5,
	Full6,
	Full7,
	Full8,
	Full9,
	Full10,
	Full11,
	Full12,
	Full13,
	Full14,
	Full15,
	Full16,
	EightCh,		//0x14
	EightCh1,	//0x15
	SixtyCh		//0x16
} ReflashPage;

typedef struct _MAINMENU_
{
	U8* menumain;
	U8* systemback;
	U8* systemfore;
	U8* cameraback;
	U8* camerafore;
	U8* recordback;
	U8* recordfore;
	U8* scheduleback;
	U8* schedulefore;
	U8* motionback;
	U8* motionfore;
	U8* ptzback;
	U8* ptzfore;
	U8* networkback;
	U8* networkfore;
	U8* alarmback;
	U8* alarmfore;
	U8* diskback;
	U8* diskfore;
	U8* userback;
	U8* userfore;
}MAINMENU;

typedef struct _SYSTEMMENU_
{
	U8* menusystem;
	U8* timeregionback;
	U8* timeregionfore;
	U8* timeposback;
	U8* timeposfore;
	U8* bordercolorback;
	U8* bordercolorfore;
	U8* videoformatback;
	U8* videoformatfore;
	U8* onoffback;
	U8* onofffore;
	U8* languageback;
	U8* languagefore;
	U8* loadback;
	U8* loadfore;
	U8* numcharback;
	U8* numcharfore;
	U8* numcharlocal;
	U8* saveback;
	U8* savefore;
	U8* okback;
	U8* okfore;
	U8* exitback;
	U8* exitfore;
	U8* promptchar;
	U8* timegroud;
	U8* HDoutputfore;
	U8* HDoutputback;
	U8* MouseDispfore;
	U8* MouseDispback;
}SYSTEMMENU;

typedef struct _CAMMENU_
{
	U8* menucam;
	U8* camnameback;
	U8* camnamelocal;
	U8* camnamefore;
	U8* recname;
	U8* camdefnameback;
	U8* camdefnamefore;
} CAMMENU;

typedef struct _RECMENU_
{
	U8* menurec;
	U8* recexcel;
	U8* recresolutionback;
	U8* recresolutionfore;
	U8* recpicqtyback;
	U8* recpicqtyfore;
	U8* checkboxback;
	U8* checkboxfore;
	U8* pageback;
	U8* pagefore;
}RECMENU;

typedef struct _SCHMENU_
{
	U8* menuschedul;
	U8* weekallback;
	U8* weekallfore;
	U8* recstytleback;
	U8* recstytlefore;
} SCHMENU;

typedef struct _MOTMENU_
{
	U8* menumotion;
	U8* mix;
}MOTMENU;

typedef struct _PTZMENU_
{
	U8* menuptz;
	U8* baudrateback;
	U8* baudratefore;
	U8* portocolidback;
	U8* portocolidfore;
	U8* ptzlocal;
}PTZMENU;

typedef struct _NETMENU_
{
	U8* menunet;
	U8* ipalloctionback;
	U8* ipalloctionfore;
	U8* emailbottonback;
	U8* emailbottonfore;
}NETMENU;

typedef struct _EMAILMENU_
{
	U8* menuemail;
	U8* charback;
	U8* charfore;
	U8* clearback;
	U8* clearfore;
	U8* testback;
	U8* testfore;
	U8* shiftback;
	U8* shiftfore;
	U8* atback;
	U8* atfore;
	U8* spaceback;
	U8* spacefore;
}EMAILMENU;

typedef struct _ALARMMENU_
{
	U8* menualarm;
	U8* buzzerrelay;
	U8* alarmbuzzerback;
	U8* alarmbuzzerfore;
}ALARMMENU;

typedef struct _DISKMENU_
{
	U8* menudisk;
	U8* systeminfo;
	U8* disknameback;
	U8* disknamefore;
	U8* formatback;
	U8* formatfore;
}DISKMENU;

typedef struct _USERMENU_
{
	U8* menuuser;
	U8* usernameback;
	U8* usernamefore;
	U8* userbottomback;
	U8* userbottomfore;
}USERMENU;

typedef struct _BAKMENU_
{
	U8* menubak;
	U8* cdtypeback;
	U8* cdtypefore;
	U8* buttonback;
	U8* buttonfore;
	U8* unrecord;
	U8* arrowback;
	U8* arrowfore;
	U8* bakinfo;
	U8* bakinfoground;
	U8* bakinfobuttonback;
	U8* bakinfobuttonfore;
	U8* bakinfobuttonskip;
}BAKMENU;

typedef struct _SEARCHMENU_
{
	U8* menusearch;
	U8* seabuttonback;
	U8* seabuttonfore;
	U8* searchline;
	U8* block;
}SEARCHMENU;

typedef struct _EVENTMENU_
{
	U8* menuevent;
	U8* evenbuttonback;
	U8* evenbuttonfore;
}EVENTMENU;

typedef struct _MOUSEMENU_
{
	U8* mainmenuallpic4;
	U8* mainmenuallpic8;
	U8* mainmenuallpic16;
	U8* mainmenuplaypic4;
	U8* mainmenuplaypic8;
	U8* mainmenuplaypic16;
	U8* mainmenuplaypic16_20;
	U8* powerback;
	U8* powerfore;
	U8* menuexitback;
	U8* menuexitfore;
	U8* spotback;
	U8* spotfore;
	U8* display;
	U8* quadmenuallpic;
	U8* othermenuallpic;
	U8* ptzmenuallpic;
	U8* quadpicback;
	U8* quadpicfore;
	U8* otherpicback;
	U8* otherpicfore;
	U8* menupicback;
	U8* menupicfore;
	U8* recordpicback;
	U8* recordpicfore;
	U8* playpicback;
	U8* playpicfore;
//	U8* playaudiopicfore;
//	U8* playaudiopicback;
	U8* playpausepicback;
	U8* playpausepicfore;
	U8* backpicback;
	U8* backpicfore;
	U8* fastpicback;
	U8* fastpicfore;
	U8* playexitback;
	U8* playexitfore;
	U8* stoppicback;
	U8* stoppicfore;
	U8* ninepicback;
	U8* ninepicfore;
	U8* sixtypicback;
	U8* sixtypicfore;
	U8* seqpicback;
	U8* seqpicfore;
	U8* searchpicback;
	U8* searchpicfore;
	U8* audiopicback;
	U8* audiopicfore;
	U8* ptzpicback;
	U8* ptzpicfore;
	U8* infopicback;
	U8* infopicfore;
	U8* bakpicback;
	U8* bakpicfore;
	U8* ptzuppicback;
	U8* ptzuppicfore;
	U8* ptzdownpicback;
	U8* ptzdownpicfore;
	U8* ptzleftpicback;
	U8* ptzleftpicfore;
	U8* ptzrightpicback;
	U8* ptzrightpicfore;
	U8* ptzautopicback;
	U8* ptzautopicfore;
	U8* ptztelepicback;
	U8* ptztelepicfore;
	U8* ptzwidepicback;
	U8* ptzwidepicfore;
	U8* ptzviewpicback;
	U8* ptzviewpicfore;
	U8* ptzpresetpicback;
	U8* ptzpresetpicfore;
	U8* ptzmenuexitback;
	U8* ptzmenuexitfore;
	U8* recordicoground;
	U8* rightupiconback;
}MOUSEMENU;

typedef struct _SPOTANDAUDIO_
{
	U8* menuspot;
	U8* menuaudio;
}SPOTANDAUDIO;

typedef struct _ERRMENU_
{
	U8* menuerr;
	U8* errinfo;
	U8* errbuttonback;
	U8* errbuttonfore;
}ERRMENU;

typedef struct _OSDSTATUS_
{
	U8* loading[8];
} OSDSTATUS;


typedef struct _ALM_INFO 
{
	unsigned int alm_lev;
	unsigned int max_alarm_hold_time;
	unsigned int alarm_hold_time[8];
	UC galarm_flat;
	UC galarm;
	UC galarm_old;
	UC galarm_stop;
}ALM_INFO;

typedef enum
{
	Live,
	Record,
	Play,
	Ptz
} MouseStatus;

typedef struct _USERMAGE_
{
	U8 UserId;
	U8 Password[6];
	U8 Menucfig;
	U8 Seqcfig;
	U8 Ptzcfig;
	U8 Reccfig;
	U8 Spotcfig;
	U8 Playcfig;
	U8 Bakcfig;
	U8 Stopcfig;
	U8 Audiocfig;
	U8 UserName[6];
}USERMAGE;

typedef enum
{
	USID,
	PASS,
	MENUCON=7,
	SEQCON,
	PTZCON,
	RECCON,
	SPOTCON,
	PLAYCON,
	STOPCON,
	BAKCON,
	AUDIOCON
} Adrpass;

typedef enum
{
	HI3515=0,
	HI3520=1,
	HIMASTER=2,
	HIVGA=4,
	HIHDMI=8
} _CHIPSELECT_;

typedef enum
{
	ADDBRIGHTNESS=0,
	ADDCONTRAST,
	ADDSAT1,
	ADDSAT2,
	ADDHUE,
	ADDLOSS,
	ADDVDELAY,
	ADDVACTIVE,
	ADDHDELAY,
	ADDHACTIVE
} _OPRATEADD_;

#define MAINMENUWINNUM		12
#define MOUSEQUADMENUNUM 	4
#define MOUSEOTHERMENUNUM	7
#define MOUSEPTZMENUNUN		11


#define CHANNELNUM		16

#define CHANNEL1		1
#define CHANNEL4		4
#define CHANNEL8		8
#define CHANNEL9		9
#define CHANNEL16		16

typedef struct _ALARM_NET_
{
	int order;
	int alch;
	int motioninfo;
	int sensorinfo;
}alarm_net;



typedef struct _OSDBLANK_
{
	U8 OsdBlank;
	U8 Blankflag;
	int Osdx;
	int Osdy;
	U8 OsdColor;
	char* Blankptr;
	
} OSDBLANK;

typedef struct BXS_PIC1_
{
	unsigned short pic_mode; // 1 is YUV , 2 is RGB
	unsigned short bit_color; // 2 is 2color 1bit, 16 is 16 color 4bit,24 is true color is 24 bit
	unsigned short pic_width;
	unsigned short pic_height;
}BXS_PIC1;

typedef struct
{
	int m_HDD_QTY;
	int m_CAPACITY;
	int m_REMALNCAPACITY;
	int m_COVERAGE;
	int m_BADAREA;
	char m_RECORDSTARTTIME[100];
	char m_RECORDENDTIME[100];
	char m_DDNS[100];
	char m_NVRIP[100];
	int m_NVRPORT;
	char m_CLIENTIP[100];
	char m_SQFTWAVEVER[100];
}NVRInfoStruct;


#define ADDCOUNTRTC		16//7
#define ADDCOUNTSYS		16//15
#define ADDCOUNTCAM		32//17
#define ADDCOUNTMOT		16//9
#define ADDCOUNTPTZ		16//  4
#define ADDCOUNTNETSTIC	48//42
#define ADDCOUNTRECCAM	16// 4
#define ADDCOUNTPAS		32//16
#define ADDCOUNTHDD		16//16
#define ADDCOUNTEMAIL		208
#define ADDCOUNTWIFI		48

#define eINITIAL				0x1A//0x6ff
#define ADDDISPMODE		0x1B
#define ADDVIDEO			0x1C

#define cINIFLAG				0x38

#define ADDRTC				0
#define ADDSYS				(ADDRTC+16)					//16~~~~0x10
#define ADDCAM				(ADDSYS+16)					//16+16=32~~~0x20
#define ADDMOT				(ADDCAM+32*16)				//32+32*16=544~~~0x0220		****
#define ADDPTZ				(ADDMOT+16*16)				//544+16*16=800~~~0x0320		****
#define ADDNSTC				(ADDPTZ+16*16)				//800+16*16=1056~~~0x0420
#define ADDRECAM			(ADDNSTC+48)				//1056+48=1104~~~0x0450
#define ADDPAS				(ADDRECAM+16*16)		//1104+16*16=1360~~~0x0550
#define ADDHDD				(ADDPAS+32*4)				//1360+32*4=1488~~~0x05d0
#define ADDEMAIL			(ADDHDD+16)					//1488+16=1504~~~0x05e0
#define ADDWIFI				(ADDEMAIL+ADDCOUNTEMAIL)	//1504+208=1712~~~0x06b0
#define ADDOFF				(ADDWIFI+ADDCOUNTWIFI)		//1712+48=1760~~~0x06e0
#define ADDCHIP				0x0f
//==================================================================================
//						Page2 initialize table description
//==================================================================================

#define RGN_NUM_MAX	9

#define OSD_STR_LEN_MAX	90

#ifdef HISI_DEV 
extern int rgn_osd_info_w[RGN_NUM_MAX],rgn_osd_info_h[RGN_NUM_MAX];
extern char osd_info_buf[RGN_NUM_MAX][24*24*OSD_STR_LEN_MAX*2];
extern int osd_info_buf_w[RGN_NUM_MAX],osd_info_buf_h[RGN_NUM_MAX];
extern VI_CSC_ATTR_S stCscAttr;
extern RGN_HANDLE RgnHandle[RGN_NUM_MAX];
extern MPP_CHN_S stChn[RGN_NUM_MAX];
extern BITMAP_S unParamtime;
extern BITMAP_S unParamname;
extern BITMAP_S unParamFace;
extern BITMAP_S info_osd_str;
extern IMAGE_DATA s_TimeNum[12],s_CamName[40];
extern IMAGE_DATA s_CharEn,s_CharEn_1;
extern unsigned char iw_info_osd_buf[24*24*60*2];
extern int iw_info_osd_buf_w,iw_info_osd_buf_h;
#endif

extern int g_record_display_info;
//extern  unsigned char tbl_osd_nr_typ[];
//extern  unsigned char	str_null[];
extern MAINMENU mainmenupic;
extern SYSTEMMENU systemmenupic;
extern CAMMENU cammenupic;
extern RECMENU recmenupic;
extern SCHMENU schmenupic;
extern MOTMENU motmenupic;
extern PTZMENU ptzmenupic;
extern NETMENU netmenupic;
extern EMAILMENU emailmenupic;
extern ALARMMENU alarmmenupic;
extern DISKMENU diskmenupic;
extern USERMENU usermenupic;
extern U8* passmenupic;
extern U8* keynumberback;
extern U8* keynumberfore;
extern BAKMENU bakmenupic;
extern SEARCHMENU searchmenupic;
extern MOUSEMENU mousemenupic;
extern SPOTANDAUDIO spotandaudiomenupic;
extern ERRMENU errmenupic;
extern OSDSTATUS osdstatuspic;
extern EVENTMENU eventmenupic;
extern U8* menupoweroffpic;
extern U8* recordico;
extern U8* autoicon;
extern U8* audiobmpback;
extern U8* audiobmpfore;
extern U8* Iconexitback;
extern U8* Iconexitfore;
//extern U8* usbformatback;
//extern U8* usbformatfore;
extern int have_usb,usb_format_is_right;

extern U8* Iriconback;
extern U8* Iriconfore;
extern U8 MOUSEMAINMENUNUM;
extern U16 offsetx,offsety;
extern U16 offsetx_hd,offsety_hd;
extern U8 Play_Mode;
extern U8 Cam_Num;
extern  U16 cam_loss,loss_old;
extern  UC menu_num,cur_pos,demo_num,demo_page,demo_num_old,demo_page_old;
extern  UC buf_time[6];
extern  UC pic_num;
extern  UC buf_cam[CHANNELNUM];
extern  U16 *scale_ptr;
extern  U16 *picpos_ptr;
extern  U16 *osd_ptr[5];
extern  U16 *hdosd_ptr[5];
extern UC bReset;
extern UC bsecond_change,bminute_change,bhour_change,bday_change,bmouth_change,byear_change;
extern U16 cam_sensor,net_sensor;
extern U16 cam_motion,net_motion;
extern UC auto_flag;
extern U8 RecFlag[], RecFlagOld[],RecStatus;
extern U8 zoom_flag;
extern U8 Mouse_PlaySta;
extern U8 diskstatus;
//extern U8 prp;
extern U8 InfoId;
extern U8 userflag;
extern U8 ChipNum;
registerrw_t RegisterReadWrite;
extern  ValPlay PlayStatusFlag;
extern  U8 Disk_Num,CurHdd,DiskCheck;
//extern  U8 tMENU_CAM_DATA[];
extern  U8 Days_Mou[];
extern int alarmch;

extern  GST_DEV_FILE_PARAM *gstCommonParam;
extern GST_DISKINFO * pstDiskInfo[];

extern KEYBOARD_LED_INFO* net_info_ptr;
extern ValSystem SysMenu;
extern ValCammra CamMenu[];
extern ValMotion MotMenu[];
//extern ValCall CallMenu[CAMNUM];
extern ValAuto AutoMenu[];

extern U8 timreadbuf[7];

extern ValPTZ PtzMenu[];
extern ValHdd HddMenu;
extern ValRecSchedule SchedulMenu[];

extern ValNetStatic NetStaticMenu;
extern MenuRec RecamMenu[];
extern ValEmail EmailMenu;
//jlb090820extern MOTION_DETECT_INFO mval[CHANNELNUM];
extern time_t start_time_Whole;
extern int Playstatus;
extern U8 g_tw2835_mask;
extern U16 KeyOld;
extern int NetStatusFlag;
//extern WordType SysWordType;
extern U8* CamName;

extern U8* Play_Menu;
extern U8* playblock;

extern unsigned int sensor_old,sensor_new;
extern unsigned int camloss_old,loss_new;
extern unsigned int motion_old,motion_new;

extern unsigned char buzzer_keep_time;
extern unsigned char relay_keep_time;
extern unsigned char record_keep_time[CHANNELNUM];
extern U8 CamMotion;
extern unsigned char rec_on;
extern unsigned int manualrecch;
extern U8 CopyRight[5];
extern U8 Play_Ch;
extern U8 Reflash_Page;
extern unsigned int BITVAL[];
extern U8 Cur_CH;

extern ALM_INFO alarm_pam;
extern ALM_INFO alarm_pam1;

extern ALM_INFO loss_pam;
extern ALM_INFO motion_pam;

extern ALM_INFO loss_pam1;
extern ALM_INFO motion_pam1;
extern ALM_INFO loss_pam2;
extern ALM_INFO motion_pam2;
extern ALM_INFO loss_pam3;
extern ALM_INFO motion_pam3;
extern UC mtdval;
extern UC lossval;

extern UC mtdval1;
extern UC lossval1;

extern UC mtdval2;
extern UC lossval2;

extern UC mtdval3;
extern UC lossval3;
//extern U8 * CdbakMenu;
//extern U8 SearchFlag;
extern unsigned char rec_flag,rec_succeed_flag;

extern U8 MenuStatus;
extern U16 MENUWIDTH,MENUITEMHIGH;
extern U16 mouse_posx,mouse_posy;
extern int focus_x,focus_y,focus_type;
extern unsigned char frame_rate[CHANNELNUM];
extern int reccam;
extern unsigned char rec_req;
extern U8 Play_Cam[];
extern U8 Record_Thread;
extern U8 PTZ_ON;
extern U8 WatchDogTime_5s;
extern U8 spot_flag;
extern U8 MenuAlarm_Date[3][CHANNELNUM][3];
extern OSDBLANK Osdptr;
extern U8 SystemLock;
extern U8 curuser;
extern U8 DEMO_DISP;
extern int High_Resolution;
extern USERMAGE menupass[];
extern U8* White_H;
extern U8* White_V;
extern U8* Black_H;
extern U8* Black_V;
extern U8* Gray_H;
extern U8* Gray_V;
extern U8* Iconhddback;
extern U8* Iconhddfore;
extern U8 MseType;
extern unsigned char rec_key;
extern U8 audio_cambuf[][18];
extern U8 spot_cam[][CHANNELNUM+1];
extern U16 tTIME_POS[];
extern U8 Keyinfo;
extern U8 mouseflash;
extern U8 remo_Led;
extern U8 strup;
extern U8 slavego;
extern U8 ChipSelect;
extern U8 dev_id;
extern U8 oprateadd[][10];
extern int WIDTH_RESOLUTION_HD;
extern int HIGH_RESOLUTION_HD;
extern int POSXRMENU_HD;
extern U8 HDOSDOFFSETX,HDOSDOFFSETY;
extern NVRInfoStruct sysinfostr;
extern char nvrip[15];
extern ValWifi Wifiinfo;
extern int Baud[];
extern int fosd_fd[];
extern int isp_fd;
extern U8 LongKey;
//extern U8 tw2835_flag = 0;
//extern unsigned char gt_tst_fnt_16x20_tst_fnt_16x20[];
extern int flag_ov2710,flag_1080p,flag_480p,flag_576p,flag_960p;
extern int g_sensor_id;
extern int load_cfg_cnt;

extern int iw_min_cnt;
extern char iw_essid[17];
extern int iw_signalval;;
extern int wifi_use_flag;
extern int i_is2_4G;

extern int g_rtc_fd;
extern rtc_time_t hi_tm;

extern int gm8126id ,tw2865id;
extern int g_sub_video_w;
extern char iw_net_info_str[40];




typedef struct {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
} rect_t;



#define MAX_FACE_VIDEO_WIDTH 1920
#define MAX_FACE_VIDEO_HEIGHT 1080
#define MAX_FACE_VIDEO_EAGE_W 5




#ifdef GOKE_DEV
	extern	char goke_str[35];
	extern ValNtp g_ntpSt;
#endif
///////////////prtcl.c///////////////////////
//char *trim(char *str);
void osd_bmp_photo(unsigned char * yuv_buf,int point_x,int point_y,int standard,int start_x,int start_y,
					int pic_width,int pic_height,int transparence_color,int transparence_degree);
int OsdLayerInit();
void SDDisplayTimechange();
//void Load_Menu_Pic(U8** pic_buf,char* filename);
//void picture_load(U8** ptr,char*str);
void Osd_line_display(U16 posX,U16 posY,U8 color,U8* pchar, U8 fond_index,U8 layer);
void Osd_char_clear(U16 posX,U16 posY,U8 color,U8 length,U8 index,U8 layer);
void Osd_Dec3_display(U16 posX,U16 posY,U8 color,U16 value,U8 index,U8 layer);
//void Osd_Dec4_display(U8 posX,U8 posY,U8 color,U16 value);
void Osd_Dec2_display(U16 posX,U16 posY,U8 color,U8 value,U8 index,U8 layer);
void Osd_Hex2_display(U16 posX,U16 posY,U8 color,U8 value,U8 index,U8 layer);
void Osd_One_display(U16 posX,U16 posY,U8 color,U8 value, U8 langue,U8 layer);
void OSD_ALL_CLEAR();
U8	GetCodeLength(unsigned char *_str);
void Bmp_All_Show(int x,int y,U8* buf,U8 layer);
void Bmp_Part_Show(int x,int y,int starx, int stary, int width, int high, U8* buf,U8 layer);
void clear_osd(int x,int y,int w,int h,U8 layer);
void draw_line(int x,int y,int x1,int y1,int minx,int miny,int maxx,int maxy,U8 layer,U16 color);
void color_full(U16 x,U16 y,U16 w,U16 h,U16 color,U8 layer);
U8	GetCodeLength(unsigned char *_str);
void set_osdAlpha(U8 alpha,U8 layer);
void remote_state_save();
void Icon_disp(U8 pos,U8 color);
//void CreateBoxPopFrame(U8 _pth, U8 _box_num, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght);
//void	Set2DBoxTable(U8 _pth, U8 _dvc, U8 _ch, U8 _osd_pos_x, U8 _osd_pos_y, U8 _lngth, U8 _hght, U8 _osd_per_cell,U8 osd_per_high);
void mouse_osd_thread_create(void);

void set_osd_display(int layer,int mode,int x,int y,int x1,int y1,int flag,char *osd_prt);
////////////////////menumain.c/////////////////////

void menu_main_on();
void mMAIN_key_func();
void MainLClick(U16 x,U16 y);
void Mouse_MainMenu_Fun(U16 x,U16 y);
////////////////////splite.c////////////////////////
void demo_osd_disp();
void menu_main_off(void);
void demo_set();
void mEVENT_key_func();
void menu_event_on();
void MouseEventRClick(U16 x,U16 y);
void MouseEventLClick(U16 x,U16 y);
void Mouse_Event_Move_Func(U16 x,U16 y);
void getsoftver(char* buf);

//void NameY_disp();
////////////////////subfunc.c////////////////////////
UC valid_name_disp();
UC valid_time_disp();
void border_set(U8 demo);
void Brightness_Ajust(U8 ch,U8 data);
void Hue_Ajust(U8 ch,U8 data);
void Contrast_Ajust(U8 ch,U8 data);
void Stauration_Ajust(U8 ch,U8 data);
void Vdelay_Ajust(U8 ch,U8 data);
void Hdelay_Ajust(U8 ch,U8 data);
void event_write(int event_no,int cam_no,U8 num) ;
U8 valid_dispmode_change();
void Full_Disk_Disp();
void NO_Disk_Disp();
void get_demo_stamp(U8 demo_num1,U8 demo_page1,U8 bcam) ;
//void loss_disp(U16 cam_seq);
void loss_disp(unsigned char channel,unsigned char onoff);  
void alarmboardshow(U8 color,U8 ch,U8 flag);
void record_disp(unsigned char channel,unsigned char event);
void SlaveMotionInfo(U8 chip);

///////////////////menusystem.c///////////////////////
void menu_system_on();
void mSYSTEM_key_func();
void setsystem(U8* buf);
void MenuSysInit();
void MouseLClick(U16 x,U16 y);
void MouseRClick(U16 x,U16 y);
void Mouse_Sys_Move_Func(U16 x,U16 y);
///////////////////rtc.c///////////////////////////////
void RTCInit();
void Time_Check();
void Time_Date_Disp();
void date_disp(U8 pos_x,U16 pos_y,U8 *buf_time1,U8 redptr,U8 color); 
void time_disp(U8 pos_x,U16 pos_y,U8 *buf_time1,U8 redptr,U8 color);
void Set_Time_Pos();
void Set_L8_Time_Pos();
//////////////////menucammra.c/////////////////////

void mCAMERA_key_func();
void menu_camera_on();
void MenuCamInit();

void Mouse_Cam_Move_Func(U16 x, U16 y, U8 type);
///////////////////menupictset.c////////////////////

//void mPICSET_key_func();
//void menu_picset_on();

void audio_ch_on();

//////////////////menumotion.c//////////////////////
void Motion_Speed_Set();
void Motion_Sensetive_Set();
void MenuMotionInit();
void menu_motion_on();
U16 GetMotionInfo(U8 ch);
void mMOTION_key_func();

void Mouse_Mot_Move_Func(U16 x, U16 y, U8 type);
//////////////////////menumotplan.c///////////////
//void MenuMotPlanInit();
//void menu_motplan_on();
//void mMOTPLAN_key_func();

////////////////////menupass.c/////////////////////

void mPASS_key_func();
void menu_pass_on(U8 flag);
void MousePassLClick(U16 x,U16 y);
void MousePassRClick(U16 x,U16 y);
void Mouse_Pass_Move_Func(U16 x,U16 y);
////////////////////menupotocol.c///////////////////

//void menu_protocol_on();
//void mPROTOCOL_key_func();

////////////////////menureset.c//////////////////////

void mRESET_key_func();
void menu_reset_on();
void Mouse_QuadMenu_Fun(U16 x, U16 y);

///////////////////menurecord.c////////////////////////
void RecorSet();
U16 Get_Fream_Setup();
void mRECORD_key_func();
void menu_record_off();
void menu_record_on();
void MenuRecInit();
void SetRecAudio();
void rec_quad();
void	rec_mux();
void rec_cif();
void	rec_hd1();
void MouseRecRClick(U16 x,U16 y);
void MouseRecLClick(U16 x,U16 y,U8 type);
void Mouse_Rec_Move_Func(U16 x,U16 y);
//////////////////////menucall.c///////////////////////

//void menu_call_on();
//void mCALL_key_func();
//void MenuCallInit();
/////////////////////menusensor.c////////////////////
//void menu_sensor_on();

//void mSENSOR_key_func();
//void MenuSenInit();

///////////////////////menusenplan.c//////////////////

//void MenuSenPlanInit();
//void menu_senplan_on();
//void mSENPLAN_key_func();
//////////////////////menuauto.c///////////////////////

//void menu_auto_on();
//void mAUTO_key_func();
//void MenuAutoInit();
////////////////////menunet.c/////////////////////////

//void menu_net_on();
//void mNET_key_func();
//void NetMenuInit();
///////////////////menustatic.c//////////////////////////

void MenuNetStaticInit();
void menu_netstatic_on();
void mNETSTATIC_key_func();
void SetNetValue(U8 flag);
void MouseNETRClick(U16 x,U16 y);
void MouseNETLClick(U16 x,U16 y);
void Mouse_Net_Move_Func(U16 x,U16 y);

//////////////////menuptz.c///////////////////////////
void menu_ptz_on();
void mPTZ_key_func();
void MenuPtzInit();
void MousePTZRClick(U16 x,U16 y);
void MousePTZLClick(U16 x,U16 y);
void Mouse_Ptz_Move_Func(U16 x,U16 y);

/////////////////menurecordplan.c///////////////////////

//void menu_recordplan_on();
//void mRECORDplan_key_func();
//void MenuRecPlanInit();

///////////////////menurecplan.c///////////////////////
//void MenuRePlanInit();
//void menu_recplan_on();
//void mRECPLAN_key_func();


//////////////////live.c////////////////////////////////
//void LiveTimeCheck();
//void Stop_flash();
//unsigned char * get_array_data(char * file_name);
void System_Lock();
void Demo_num_key_pro(U8 demonum,U8 demopage);
void Hdd_Error_Check();
void Net_Status_Check();
void Set_Audio(U8 bt);
void LIVE_key_func(void);
void LFULL_cam_key_pro(U8 camno) ;
void FullCamAdjust(U8 flag);
/////////////////key.c//////////////////////////////////
U8 getkeyvalue(KeyValSys keyid);
void GetKey();


/////////////////play.c////////////////////////////////
void Play_On();
void Play_Back();
void Play_Fast();
void Play_Start();
void Play_Stop();
void Split_Play(U8 mode,U8 timemode,time_t starttime);
int Rplay_Start();
void Play_Err_Inital();
void Play_Err_Stop();
void PLAY_key_func();
void PlayStatusShow();
void Play_Status_Show();
void Play_Ch_Show();
void Play_Status_Show();
void Mouse_Play_Func(U16 x, U16 y,U8 type);
/////////////////eprom.c////////////////////////////

void InitE2Prom();
void I_read_byte_eeprom(U16 sub_addr,U8 *ptr);
void I_write_byte_eeprom(U16 sub_addr,U8 val);
void I_read_block_eeprom(U16 sub_addr,U16 data_num,U8 *ptr);
void I_write_block_eeprom(U16 sub_addr,U16 data_num,U8 *ptr);
void readmsg(char *  buf);
void writemsg(char * bufll);
/////////////////menuuart.c////////////////////////
BXS_PIC1* getyuvfile(U8* buffile,char* strbuf);
//void MenuUartInit();
//void menu_uart_on();
//void mUART_key_func();

////////////////////menuhdd.c///////////////////////

void MenuHddInit();
void menu_hdd_on();
void mHDD_key_func();

void mHDDINFO_key_func();
void Hdd_Info_Show();
void Hdd_Error_Show();
void MouseHddRClick(U16 x,U16 y);
void MouseHddLClick(U16 x,U16 y);
void Mouse_Hdd_Move_Func(U16 x,U16 y);
//void HddinfoRClick(U16 x,U16 y);
void HddinfoLClick(U16 x,U16 y);
void Mouse_HddInfo_Move_Func(U16 x,U16 y);

///////////////////menualarmsearch.c////////////////////

//void menu_alarm_search_on();
//void menu_alarm_search_init();
//void mALARMSEARCH_key_func();
////////////////////menurecsearch.c////////////////////

void menu_rec_search_on();
void mRECSEARCH_key_func();
void menu_rec_search_init();
void menu_rec_time_on();
void Bak_Status();
U8 menu_rec_search(int page);
void PlayCam(U8 ch);
void menu_time_back_init();
/////////////////////hdd.c/////////////////////////////

void GetHddInfo();
void DiskCheckNew();

/////////////////////menudiskformat.c/////////////////////

void menu_diskformat_on();
void Format_LClick_Fun(U16 x,U16 y);
void Mouse_Format_Move_Func(U16 x,U16 y);
void mDISKFORMAT_key_func();
/////////////////////record.c///////////////////////////
void Record_Stop_Function();
void Record_Start_Function();
void Rec_key_func();
void CheckRecord();

/////////////////////zoom.c////////////////////////////
//void Zoom_On_Func();
//void mZOOM_key_func();
void Zoom_Off_Func();
void Area_Zoom_Func(U8 pos);
void ZoomFun();
////////////////////auto.c//////////////////////////////
void AUTOSEQ_key_func();

void auto_on();
void auto_relate();
void MouseAutoClick(U8 flag);
void auto_func();
void backupmenuoff();

//////////////////mcucontrol.c/////////////////////////////

void TurnOnOff();
void RelayControl(U8 flag);
void BuzzerControl(U8 flag);
void LedControl(U8 KeyFlag,U8 SetorCle);
void McuReset();
void WatchDog();
void McuStart(void);
void get_tw2835_mask(void);
//void back_tw2835_mask(void);
U8 check_mask(void);
void date_disp_pos_set(U8 *date_pos);
//unsigned char send_check_sum(void);
void mcu_disp_refresh(void);
void Get_Loss(U8 *alarm,U8 *loss,U8 *motion);
///////////////////menuerrinfo.c/////////////////////////////

void mErrInfo_key_func();
void MenuErrOn();

void rec_disp(unsigned char channel,unsigned char event);
//////////////////menusearch.c/////////////////////////////
void setliveaudio();
/////////////////menupower.c/////////////////////////////

void Menu_Power_On();
void mMENUPOWER_Keyfun();
void PowerLClick(U16 x,U16 y);
void Mouse_Power_Move_Func(U16 x,U16 y);
/////////////////menubackup.c///////////////////////////
void usbbakstop();
void BackUpOn();
void BackupLclick(U16 x, U16 y);
void BackUp_KeyFun();
U8 bakdevicecheck();
///////////////////ptzcontrol.c/////////////////////////////
void NETPTZ_Control_Fun(U8 ch,U8 ptzspeed,U8 ptzdata);
void PTZ_Control_KeyFun();
void ptz_stop_func();
void PTZ_Control_ON();
void BackUpOff();
void ptzpelcod_groupset(U8 ch,U8 track,U8 point,U8 speed,U8 time);
void ptzpelcop_groupsetstar(U8 ch,U8 track);
void ptzpelcop_groupsetend(U8 ch);
void ptzpelcod_groupsetstar(U8 ch,U8 track);
void ptzpelcod_groupsetend(U8 ch);
void ptzupclick();
void ptzdownclick();
void ptzleftclick();
void ptzrightclick();
void ptzautoclick();
void ptzteleclick();
void ptzwideclick();
void ptzviewclick();
void ptzpresetclick();
void ptzmenuexitclick();
void ptzmousekeyfun(U16 x, U16 y, U8 type);
void ptzmenu_mouse_off();
void ptzmousestop();
//////////////////menuschedule.c///////////////////////////

void mSCHEDUL_key_func();
void menu_schedule_on();
void MenuSCHInit();
void MouseSchRClick(U16 x,U16 y);
void MouseSchLClick(U16 x,U16 y);
void Mouse_Sch_Move_Func(U16 x,U16 y);

////////////////alarm.c///////////////////////////////////
void sensor_detect(void);
void sensor_process(void);
void motion_detect(void);
void motion_process(void);
void vloss_detect(void);
void vloss_process(void);

U8 CheckUser(U8 UserId);
void alarm_second(void);
void motion_handle();
void loss_handle();
void sensor_handle(U16 tmpsensor);
void alarm_init(void);
void relay_set(unsigned char onoff);
void buzzer_set(unsigned char onoff);
//void alarm_record_set(unsigned char channel,unsigned char alarm_style, unsigned char onoff);
void alarm_disp(unsigned int channel,unsigned char event);
void alarm_log_write(unsigned char channel,unsigned char event);
void alarm_disp_refresh(void);
void alarm_record_exit(void);
void alarm_record_prepare(void);
void alarm_minute(void);
void record_key_set(void);
void stop_key_set(void);
void manual_rec_set();
void record_disp_refresh(void);
void record_set1(void);
///////////////////////////////////menurecsearch.c/////////////////////////////////

void mEVENTPLAY_key_func();
 void menu_event_play_on();
void menu_event_play_init();
void Menu_Load();

void set_channelId();
void Mouse_Sea_Move_Func(U16 x,U16 y,U8 type);
///////////////////////////////////menuspot.c/////////////////////////////////////////
void mSPOT_key_func();
void menu_spot_on();
void SpotMouseLClick(U16 x, U16 y);
void Mouse_spot_Move_Func(U16 x,U16 y);
///////////////////////////////////alarmload.c////////////////////////////////////////

void LM_check(void);

void get_tw2835_mask1(void);


void menu_cdbak_on();
void mCDBAK_key_func();


void TotalSizeCheck();
void AlarmReset();


//////////////////////////////////////////menumouse.c/////////////////////////////////////////////
void Menu_Mouse_On();
void menu_mouse_off();
void mouse_handle();
void Icon_Menu_Off();
/////////////////////////////////////////menumousecam.c//////////////////////////////////////////


/////////////////////////////////////////mouse.c//////////////////////////////////////////////
void mouse_handle();
int InitialMouse();
void Icon_Menu_On(U16 x,U16 y);
int Icon_LClick_Fun(U16 x,U16 y);
void Icon_Move_Fun(U16 x,U16 y);
/////////////////////////////////////////menubackup.c//////////////////////////////////////////
 void menu_time_back_on();
void mBACKUP_key_func();
void Back_Key_func(U16 x,U16 y,U8 type);
void backupmenuon(U8 errtype, U8 device);

//**********************************Spot_Fun.c**************************************//
void set_motion(U8 flag);
void Spot_key_fun();
void Spot_ch_on();
void SpotMenu_Inital();
void get_key_type(void);
//***********************************MenuAlarm.c************************************//
void DISK_Format(int x,int y);
void ErrMouseLClick(U16 x,U16 y);
void Mouse_Err_Move_Func(U16 x,U16 y);
void MenuAlarm_Initial();
void MenuAlarm_On();
void mMenuAlarm_KeyFun();
void MouseAlarmLClick(U16 x,U16 y);
void Mouse_Alarm_Move_Func(U16 x,U16 y);


void set_full_chinnal(U8 x,U8 y,U16 x1,U16 y1,U8 ch);
void menu_user_on();
void mUser_key_func();
void user_menu_inital();
void MouseUserRClick(U16 x,U16 y);
void MouseUserLClick(U16 x,U16 y,U8 type);
void Mouse_User_Move_Func(U16 x,U16 y);

void Mouse_Live_Fun(U16 x,U16 y,U8 type);
void Mouse_LiveMenu_Fun(U16 x,U16 y);
void MouseMainLClick(U16 x,U16 y);
void MouseQuadMenuShow();
void mouse_list_show(U8 pos,U8 color);

void AudioMouseLClick(U16 x, U16 y);
void Mouse_audio_Move_Func(U16 x,U16 y);
void audio_key_fun();
void live_demo_set();
void block_show(U16 x,U16 y,U8 layer);

void PtzMenu_Mouse_On();
void PTZMouseMainLClick(U16 x,U16 y);
void PTZMouseMainLUClick(U16 x,U16 y);
void Mouse_PTZMenu_Fun(U16 x,U16 y);

void OsdY_line_display(U8 ch,U8 timeposx,U8* pchar, U8 fond_index);
void recpathnamedisp();
void fileinit();
void netcontrolinital(void);
void net_ptz_reconfig();
void init_nvp1114(U8 system1);
void audioMenu_Inital();


int Initialinfolayer();
void infoup(U16 x,U16 y,U8 index);
void infoshowhide(U8 flag);
void chip_check(void);
int CoverRegionControl(int x,int y,int w,int h,unsigned char flag,unsigned char ch);
long long DiviFun(long long a,long long b);
void menu_ptz_do();
void menu_camera_do();
void menu_record_do();
int FileWrite();
void set_schedule();
void menu_motion_do();
void menu_netstatic_do();
int AlarmFileWrite();
void user_data_save();
void net_user_set();
int Check_Language();
void Set_Spot();
void setnet(char* msgbuf);
void netsavedate(char* msgbuf);
U8 getmousestatus();
U8 getmousestatus2();
void Spot3520Set();
void spotkeyboradset(U8 ch);
void FixDiskInfo();
void mEmail_key_func();
void MouseEmailLClick(U16 x,U16 y);
void MouseEmailRClick(U16 x,U16 y);
void Mouse_Email_Move_Func(U16 x,U16 y);
void menu_email_on();
void MenuEmailInit();
U8 menu_email_save();
void email_send_test();
void mail_send(U16 cam,U8 event);
void Motioncheck();
void sil9034_1080i50_init(void);
void sil9034_1080i60_init(void);
int sys_command(char * cmd);
void wdginit();
void sensor_check();
void creatfixdiskthread();
void wifigoon(void);
void alarm_sent_net();
//int net_cfg_save(unsigned char *ipaddress,unsigned char *gateway,unsigned char *netmask,unsigned char *ddns1,unsigned char *ddns2,unsigned char *Wififlag);
void setuart(int baudrat);
void uartsend(char * msg, int len);
int osd_init(void);
void sar_fun(void);
void ircut_func(void);
void checksdcard(void);
void NoCard_Show(void);
void record_wait(void);
int hd1080p_check(void);
int net_info_display(int flag);
int hd480p_check(void);
int hd576p_check(void);
int hd960p_check(void);
void write_dec(unsigned char address,unsigned char databuf);

char * Hisi_get_chan_bitmap_array(int chan,char * bit_file,int * pic_width,int * pic_height);

void char_line_display(char *str);


#ifdef GOKE_DEV

extern GADI_SYS_HandleT goke_ispHandle;

	int goke_osd_init();
	int goke_show_time(void);	
	int goke_osd_set_text(char *str);
	int goke_set_pm_param(int x,int y,int w,int h);
	int goke_gpio_get(int number,int *val);
	int goke_gpio_set(int number,int flag);
	int goke_dev_ctrl(int number,int *val);
	
	int goke_isp_init(void);
#endif

/////////////////////////////////////wired & wireless

typedef struct _WIFI_AP{
	char channelid[20];
	char wifiapssid[20];
}WIFIAP;

extern WIFIAP stwifiap[];
extern WIFIAP stwifiap_store[];
extern int g_Cur_Ap_Signalch;
extern char myid[20];

/*typedef struct _IFREQ {
    char    ifr_name[16];  // interface name, e.g., "eth0"
    union {
        struct sockaddr ifru_addr;
        struct sockaddr ifru_dstaddr;
        struct sockaddr ifru_broadaddr;
        short   ifru_flags;
        int     ifru_metric;
        caddr_t ifru_data;
    } ifr_ifru;
}IFREQ;*/
 
//unsigned long iw_ioctl_get_ipaddress(char *dev);

int iw_get_nvr_set(char *fileUrl, char *conStr);

int set_if_ip(char *if_name, char *ip_str) ;

int iw_FindSSID(char *url, char *essid, int isWifiWork);
	
int check_wire(void);

int check_apsignal(char *tmpSSID);

//int check_parassid();

void wire_on(void);

void wire_off(void);

//int connectap(char * tmpSSID);

unsigned char valid_ap(void);

unsigned char check_ssid_table(void);

void sta_on(void);

void sta_off(void);

void ap_on(void);

void ap_off(void);

void ipchange_check(void);

//void upDown_Wifi(void);
	
//void search_ssid(void);

void threat1(void);

int cam_wired_wireless_connect_thread_create(void);

int check_broadcast(void);

extern unsigned char threat2_start;

void threat2(void);

int cam_recv_broadcast_thread_create(void);

//void threat3(void);

//int cam_count_thread_create(void);

int GetValueFromEtcFile(const char* pEtcFile, const char* pSection,
                               const char* pKey, char* pValue, int iLen);
int SetValueToEtcFile (const char* pEtcFile, const char* pSection, const char* pKey, char* pValue);



int showRectOnVenc(rect_t *pRect,int count);



extern int chanChannel[128];

