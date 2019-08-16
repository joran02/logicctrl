#include	"cmn.h"
#include "Func_dst.h"

//META_DST_ELEMENT g_dstSt;//����ʱ
//����month-1  weeks:0-4.
//��ʱ��year-1900   month-1  day:1-31.
void InitDstParams1()
{
	int tmp = 0, ret = 0, port = 0;
	U8 tmp1=0;
	char str[100]={0};
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTON",str,1);
	if (!ret)
	{
		tmp = atoi(str);
//		DPRINTK("DSTON:%s,%d\n",str,tmp);
		if (tmp == 1)
			g_dstSt.m_DST_On = 1;
		else
			g_dstSt.m_DST_On = 0;
	}
	else
	{
		g_dstSt.m_DST_On = 0;
		DPRINTK("Read [NTPMENU] NTPON fail!\n");
	}

	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTTYPE",str,1);
	if (!ret)
	{
		tmp = atoi(str);
//		DPRINTK("DSTTYPE:%s,%d\n",str,tmp);
		if (tmp == 1)
			g_dstSt.m_DST_Type = 1;
		else
			g_dstSt.m_DST_Type = 0;
	}
	else
	{
		g_dstSt.m_DST_Type = 1;
		DPRINTK("Read [DSTMENU] DSTTYPE fail!\n");
	}
//-------------------------------week  start time----------
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTWEEKMONS",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTWEEKMONS:%s,%d\n",str,port);
		g_dstSt.m_DST_Week_StartTime.u8Month = port;
	}
	else
	{
		g_dstSt.m_DST_Week_StartTime.u8Month = 0;
		DPRINTK("Read [DSTMENU] DSTWEEKMONS fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTWEEKWEEKS",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTWEEKWEEKS:%s,%d\n",str,tmp);
		if (tmp<=4)
			g_dstSt.m_DST_Week_StartTime.u8Week = port;
		else
			g_dstSt.m_DST_Week_StartTime.u8Week = 0;
	}
	else
	{
		g_dstSt.m_DST_Week_StartTime.u8Week = 0;
		DPRINTK("Read [DSTMENU] DSTWEEKWEEKS fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTWEEKDAYS",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTWEEKDAYS:%s,%d\n",str,port);
		g_dstSt.m_DST_Week_StartTime.u8Day = port;
	}
	else
	{
		g_dstSt.m_DST_Week_StartTime.u8Day = 0;
		DPRINTK("Read [DSTMENU] DSTWEEKDAYS fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTWEEKHOURS",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTWEEKHOURS:%s,%d\n",str,port);
		g_dstSt.m_DST_Week_StartTime.u8Hour = port;
	}
	else
	{
		g_dstSt.m_DST_Week_StartTime.u8Hour = 1;
		DPRINTK("Read [DSTMENU] DSTWEEKHOURS fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTWEEKMINS",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTWEEKMINS:%s,%d\n",str,port);
		g_dstSt.m_DST_Week_StartTime.u8Minute = port;
	}
	else
	{
		g_dstSt.m_DST_Week_StartTime.u8Minute = 1;
		DPRINTK("Read [DSTMENU] DSTWEEKMINS fail!\n");
	}
//-------------------------------week  end time----------
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTWEEKMONE",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTWEEKMONE:%s,%d\n",str,port);
		g_dstSt.m_DST_Week_EndTime.u8Month = port;
	}
	else
	{
		g_dstSt.m_DST_Week_EndTime.u8Month = 0;
		DPRINTK("Read [DSTMENU] DSTWEEKMONE fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTWEEKWEEKE",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTWEEKWEEKE:%s,%d\n",str,tmp);
		if (tmp<=4)
			g_dstSt.m_DST_Week_EndTime.u8Week = port;
		else
			g_dstSt.m_DST_Week_EndTime.u8Week = 0;
	}
	else
	{
		g_dstSt.m_DST_Week_EndTime.u8Week = 0;
		DPRINTK("Read [DSTMENU] DSTWEEKWEEKE fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTWEEKDAYE",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTWEEKDAYE:%s,%d\n",str,port);
		g_dstSt.m_DST_Week_EndTime.u8Day = port;
	}
	else
	{
		g_dstSt.m_DST_Week_EndTime.u8Day = 0;
		DPRINTK("Read [DSTMENU] DSTWEEKDAYE fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTWEEKHOURE",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTWEEKHOURE:%s,%d\n",str,port);
		g_dstSt.m_DST_Week_EndTime.u8Hour = port;
	}
	else
	{
		g_dstSt.m_DST_Week_EndTime.u8Hour = 1;
		DPRINTK("Read [DSTMENU] DSTWEEKHOURE fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTWEEKMINE",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTWEEKMINE:%s,%d\n",str,port);
		g_dstSt.m_DST_Week_EndTime.u8Minute = port;
	}
	else
	{
		g_dstSt.m_DST_Week_EndTime.u8Minute = 1;
		DPRINTK("Read [DSTMENU] DSTWEEKMINE fail!\n");
	}
//-------------------------------DATE  start time----------
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEYEARS",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTDATEYEARS:%s,%d\n",str,port);
		if (port > 1900)
			g_dstSt.m_DST_Date_StartTime.u16Year = port-1900;
		else
			g_dstSt.m_DST_Date_StartTime.u16Year = 0;
	}
	else
	{
		g_dstSt.m_DST_Date_StartTime.u16Year = 0;
		DPRINTK("Read [DSTMENU] DSTDATEYEARS fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEMONS",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTDATEMONS:%s,%d\n",str,tmp);
		if (tmp<=4)
			g_dstSt.m_DST_Date_StartTime.u8Month = port;
		else
			g_dstSt.m_DST_Date_StartTime.u8Month = 0;
	}
	else
	{
		g_dstSt.m_DST_Date_StartTime.u8Month = 0;
		DPRINTK("Read [DSTMENU] DSTDATEMONS fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEDAYS",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTDATEDAYS:%s,%d\n",str,port);
		g_dstSt.m_DST_Date_StartTime.u8Day = port;
	}
	else
	{
		g_dstSt.m_DST_Date_StartTime.u8Day = 1;
		DPRINTK("Read [DSTMENU] DSTDATEDAYS fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEHOURS",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTDATEHOURS:%s,%d\n",str,port);
		g_dstSt.m_DST_Date_StartTime.u8Hour = port;
	}
	else
	{
		g_dstSt.m_DST_Date_StartTime.u8Hour = 1;
		DPRINTK("Read [DSTMENU] DSTDATEHOURS fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEMINS",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTDATEMINS:%s,%d\n",str,port);
		g_dstSt.m_DST_Date_StartTime.u8Minute = port;
	}
	else
	{
		g_dstSt.m_DST_Date_StartTime.u8Minute = 1;
		DPRINTK("Read [DSTMENU] DSTDATEMINS fail!\n");
	}
//-------------------------------DATE  end time----------
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEYEARE",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTDATEYEARE:%s,%d\n",str,port);
		if (port > 1900)
			g_dstSt.m_DST_Date_EndTime.u16Year = port-1900;
		else
			g_dstSt.m_DST_Date_EndTime.u16Year = 0;
	}
	else
	{
		g_dstSt.m_DST_Date_EndTime.u16Year = 0;
		DPRINTK("Read [DSTMENU] DSTDATEYEARE fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEMONE",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTDATEMONE:%s,%d\n",str,tmp);
		if (tmp<=4)
			g_dstSt.m_DST_Date_EndTime.u8Month = port;
		else
			g_dstSt.m_DST_Date_EndTime.u8Month = 0;
	}
	else
	{
		g_dstSt.m_DST_Date_EndTime.u8Month = 0;
		DPRINTK("Read [DSTMENU] DSTDATEMONE fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEDAYE",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTDATEDAYE:%s,%d\n",str,port);
		g_dstSt.m_DST_Date_EndTime.u8Day = port;
	}
	else
	{
		g_dstSt.m_DST_Date_EndTime.u8Day = 1;
		DPRINTK("Read [DSTMENU] DSTDATEDAYE fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEHOURE",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTDATEHOURE:%s,%d\n",str,port);
		g_dstSt.m_DST_Date_EndTime.u8Hour = port;
	}
	else
	{
		g_dstSt.m_DST_Date_EndTime.u8Hour = 1;
		DPRINTK("Read [DSTMENU] DSTDATEHOURE fail!\n");
	}
	memset(str,0,sizeof(str));
	ret = GetValueFromEtcFile("/mnt/mtd/ntp_dst.txt","DSTMENU","DSTDATEMINE",str,8);
	if (!ret)
	{
		port = atoi(str);
//		DPRINTK("DSTDATEMINE:%s,%d\n",str,port);
		g_dstSt.m_DST_Date_EndTime.u8Minute = port;
	}
	else
	{
		g_dstSt.m_DST_Date_EndTime.u8Minute = 1;
		DPRINTK("Read [DSTMENU] DSTDATEMINE fail!\n");
	}
}

int getDayOfMonth(int year, int mon)
{//mon : 0 ~11
	int x[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	if (year%4 == 0 && year%100 != 0 || year%400 == 0)
		x[1] = 29;
	else
		x[1] = 28;

	return x[mon];//���ص�������
}


//struct tm
//{
//    int tm_sec;      /*����Ŀǰ������������ΧΪ0-59����������61�� */
//    int tm_min;     /*����Ŀǰ��������Χ0-59*/
//    int tm_hour;   /* ����ҹ�����ʱ������ΧΪ0-23 */
//    int tm_mday;  /* Ŀǰ�·ݵ���������Χ01-31 */
//    int tm_mon;   /*����Ŀǰ�·ݣ���һ�����𣬷�Χ��0-11 */
//    int tm_year;   /*��1900 ���������������*/
//    int tm_wday;   /* һ���ڵ�������������һ���𣬷�ΧΪ0-6��*/
//    int tm_yday;   /* Days in year.[0-365] */
//    int tm_isdst;   /*�չ��Լʱ������DST. [-1/0/1]*/
//};

void trans_DayOfWeek_to_Date(time_t time, int *months, int *weeks, int *days, int *hour, int *min)
{//ת���ɱ�׼ʱ��.weeks
	struct tm p_tm;  
	gmtime_r(&time, &p_tm);
	*min = p_tm.tm_min;
	*hour = p_tm.tm_hour;
	*months = p_tm.tm_mon;
	*days = p_tm.tm_wday;//����һ��ʼ��0~6
	*weeks = (p_tm.tm_mday - p_tm.tm_wday + 6)/7;//�ڼ���.0~4.
	static int a = 0;
	a++;
	if (a==88)
	{
		a=0;
		DBGPRINT("%d - %d - %d - %d:%d\n", *months, *weeks, *days, *hour, *min);
	}
}

void adjust_DST_Use(unsigned char buf[])
{//start to culculate dst. +1 hour.
/*U8 buf[7] = {0};//654 210
buf[6] = buf_time[5];//year
buf[5] = buf_time[4];//mon
buf[4] = buf_time[3];//day
buf[2] = buf_time[2];//hour
buf[1] = buf_time[1];//min
buf[1] = buf_time[0];//sec*/
DBGPRINT("lllllllll  ---  into dst setting   ---   xxxx\n");
	if (buf[4] == getDayOfMonth(buf[6]+1900, buf[5]))
		{//last day of month
		DBGPRINT("lllllllll  --- end of month day --- xxxx\n");
			if (buf[2] <= 22)
			{//before 22 hour
				buf[2] += 1;
			}
			else
			{//after 23 hour
				if (buf[5] == 11)
				{//last month of year
					buf[6] += 1;
					buf[5] = 0;
					buf[4] = 1;
					buf[2] = 0;
				}
				else
				{// 0~10 month
					buf[5] += 1;
					buf[4] = 1;
					buf[2] = 0;
				}
			}
		}
	else
		{//not last day of month
		DBGPRINT("lllllllll  --- normal day --- xxxx\n");
			if (buf[2] <= 22)
			{//before 22 hour
				buf[2] += 1;
			}
			else
			{//after 23 hour
				buf[4] += 1;
				buf[2] = 0;
			}
		}
	g_dstSt.iHaveSet = 1;
	
}

void adjust_DST_Exit(unsigned char buf[])
{//exit dst. -1 hour.
/*U8 buf[7] = {0};//654 210
buf[6] = buf_time[5];//year
buf[5] = buf_time[4];//mon
buf[4] = buf_time[3];//day
buf[2] = buf_time[2];//hour
buf[1] = buf_time[1];//min
buf[1] = buf_time[0];//sec*/
DBGPRINT("lllllllll  ---  exit dst setting   ---   xxxx\n");
	if (buf[4] == 1)
	{//first day of month
		if (buf[2] >= 1)
		{//after 1:00 am
			buf[2] -= 1;
		}
		else
		{//before 1:00 am
			if (buf[5] == 0)
			{//first day of year
				buf[6] -= 1;
				buf[5] = 12;
				buf[4] = 31;
				buf[2] = 23;
			}
			else
			{// 2~12 month
				buf[5] -= 1;
				buf[4] = getDayOfMonth(buf[6]+2000, buf[5]-1);
				buf[2] = 23;
			}
		}
	}
	else
	{//not first day of month
		if (buf[4] >= 1)
		{//after 1:00 am
			buf[2] -= 1;
		}
		else
		{//before 1:00 am
			buf[4] -= 1;
			buf[2] = 23;
		}
	}
	g_dstSt.iHaveSet = 0;
}
/*
	@lx
		compare_DST_Type_Week(META_DST_WEEK_TIME *wTime, time_t sysTime, int isNeedAjust)
	@return
		1: sysTime is bigger than wTime
		-1: sysTime is smaller than wTime
*/
int compare_DST_Type_Week(META_DST_WEEK_TIME *wTime, time_t sysTime, int isNeedAjust)
{
	int months = 0, weeks = 0, days = 0, hour = 0, min = 0;
	if (isNeedAjust)
		sysTime -= 3600;//ֱ�Ӽ�һ��Сʱ���������տ�ȼ��㡣
	trans_DayOfWeek_to_Date(sysTime, &months, &weeks, &days, &hour, &min);

	//DBGPRINT("week dst:%d - %d - %d - %d:%d\n", wTime->u8Month, wTime->u8Week, wTime->u8Day, wTime->u8Hour, wTime->u8Minute);
	if (months < wTime->u8Month)
		return -1;
	else 
	{
		if (months > wTime->u8Month)
			return 1;
		
		if (weeks < wTime->u8Week)
			return -1;
		else
		{
			if (weeks > wTime->u8Week)
				return 1;
			
			if (days < wTime->u8Day)
				return -1;
			else
			{
				if (days > wTime->u8Day)
					return 1;
				
				if (hour < wTime->u8Hour)
					return -1;
				else
				{
					if (hour > wTime->u8Hour)
						return 1;
					
					if (min < wTime->u8Minute)
						return -1;
					else
					{
						return 1;
					}
				}
			}
		}
	}
}

/*
	@lx
		compare_DST_Type_Date(META_DAY_TIME *dTime, time_t sysTime, int isNeedAjust)
	@return
		1: sysTime is bigger than dTime
		-1: sysTime is smaller than dTime
*/
int compare_DST_Type_Date(META_DAY_TIME *dTime, time_t sysTime, int isNeedAjust)
{
	int ret;
	time_t timet;
	ret = 0;

	timet = gstCommonParam->GST_FTC_CustomTimeTotime_t(dTime->u16Year,//116(2016)
		dTime->u8Month,dTime->u8Day,dTime->u8Hour,dTime->u8Minute,dTime->u8Second);

	if (isNeedAjust)//����ʱ�ǰ�������ʱ��
		timet += 3600;//ֱ�Ӽ�һ��Сʱ������������տ�ȡ�

	static int a = 0;
a++;
if (a==88)
{
a=0;
DBGPRINT("s:%ld, d%ld, date dst:%d - %d - %d - %d:%d:%d\n", sysTime, timet, dTime->u16Year, dTime->u8Month, dTime->u8Day, dTime->u8Hour, dTime->u8Minute, dTime->u8Second);
}
	if ( sysTime - timet >= 0)
		return 1;
	else 
		return -1;
}

int func_Dst_Check(time_t time, unsigned char buf[])
{//dst check logic
static int a = 0;
a++;
if (a==88)
{
a=0;
DBGPRINT("lllllllll  --- check on:%d -- haveSet:%d   ---   xxxx\n", g_dstSt.m_DST_On,g_dstSt.iHaveSet);
}
 	if (g_dstSt.m_DST_On == 1 
		|| g_dstSt.iHaveSet == 1)
	{
		if (g_dstSt.iHaveSet == 0)
		{//not set dst
			if (g_dstSt.m_DST_Type == 0)
			{//week
				if (compare_DST_Type_Week(&g_dstSt.m_DST_Week_StartTime, time, 0) > 0)
					{//equal 
						if (compare_DST_Type_Week(&g_dstSt.m_DST_Week_EndTime, time, 0) < 0)
						{
							adjust_DST_Use(buf);
							return 1;
						}
					}
					
			}
			else if (g_dstSt.m_DST_Type == 1)
			{//date
				if (compare_DST_Type_Date(&g_dstSt.m_DST_Date_StartTime, time, 0) > 0)
					{//equal dst start time
						if (compare_DST_Type_Date(&g_dstSt.m_DST_Date_EndTime, time, 0) < 0)
						{
							adjust_DST_Use(buf);
							return 1;
						}
					}
			}
		}
		else if (g_dstSt.iHaveSet == 1)
		{//already set dst, whether ntp is on or not.
			if (g_dstSt.m_DST_Type == 0)
			{//week
				if (compare_DST_Type_Week(&g_dstSt.m_DST_Week_StartTime, time, 0) < 0 
					|| compare_DST_Type_Week(&g_dstSt.m_DST_Week_EndTime, time, 1) > 0 
					|| g_dstSt.m_DST_On == 0)
					{//equal 
						adjust_DST_Exit(buf);
						return 1;
					}
					
			}
			else if (g_dstSt.m_DST_Type == 1)
			{//date
				if (compare_DST_Type_Date(&g_dstSt.m_DST_Date_StartTime, time, 0) < 0 
					|| compare_DST_Type_Date(&g_dstSt.m_DST_Date_EndTime, time, 1) > 0 
					|| g_dstSt.m_DST_On == 0)
					{//equal dst start time
						adjust_DST_Exit(buf);
						return 1;
					}
			}
		}
	}
	return -1;
}
