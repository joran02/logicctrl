#ifndef _FUNC_DST_H_
#define _FUNC_DST_H_

#ifdef __cplusplus
extern "C" {
#endif
//buf_time (y+2000)md hms
typedef unsigned char		UINT8;
typedef unsigned short		UINT16;

typedef struct _meta_dst_week_time_
{
    UINT8  u8Month; //January=0,...
    UINT8  u8Week;//��һ�ܵ�������.Ӣ����ʾ1st 2nd .. last. ���Ϊ�ڼ������ֵ����ڼ���
    UINT8  u8Day;//����һ~������  (cmsҪ������һ��ʼ��ʾ)
    UINT8  u8Hour;
    UINT8  u8Minute;
    UINT8 u8Reserved[1];
}META_DST_WEEK_TIME;

typedef struct _meta_day_time_
{
    UINT16 u16Year;
    UINT8  u8Month; //January=0,...
    UINT8  u8Day;
    UINT8  u8Hour;
    UINT8  u8Minute;
    UINT8  u8Second;
    UINT8 u8Reserved[1];
}META_DAY_TIME;

typedef struct _meta_dst_element_{//Daylight Saving Time��DST
    int m_DST_On;//�Ƿ�������ʱ��0~1.
    int m_DST_Type;//0~1.�ܡ����ڡ�
    //������
    META_DAY_TIME m_DST_Date_StartTime;
    META_DAY_TIME m_DST_Date_EndTime;
    //����
    META_DST_WEEK_TIME m_DST_Week_StartTime;
    META_DST_WEEK_TIME m_DST_Week_EndTime;
    int iHaveSet;//�Ƿ��Ѿ���������ʱ
}META_DST_ELEMENT;


int getDayOfMonth(int year, int mon);
void trans_DayOfWeek_to_Date(time_t time, int *months, int *weeks, int *days, int *hour, int *min);
void adjust_DST_Use(unsigned char buf[]);
void adjust_DST_Exit(unsigned char buf[]);
int compare_DST_Type_Week(META_DST_WEEK_TIME *wTime, time_t sysTime, int isNeedAjust);
int compare_DST_Type_Date(META_DAY_TIME *dTime, time_t sysTime, int isNeedAjust);
int func_Dst_Check(time_t time, unsigned char buf[]);

extern META_DST_ELEMENT g_dstSt;
#ifdef __cplusplus
};
#endif

#endif

