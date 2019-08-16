/*
	AnalyseWifi.h
*/
//#include "netlogic.h"

#ifdef __cplusplus
extern "C"
{
#endif

////////////////////define varias////////////////////
#define MN_MAX_BUF_SIZE (256*1024)


typedef struct 
{
	char WirelessMode[16];
	char Essid[32];
	char Channel[16];
	char Quality[16];
	char AuthMode[16];
	char EncrypType[16];
	char KeyIndex[16];
	char PassWord[32]; 
	int pos;
}WifiInfo;

char * AnalyseData(int * wifi_num);

typedef struct _SORT_LIST_ELEMENT_
{
	int val;
	char name[32];
}SORT_LIST_ELEMENT;

typedef struct _SORT_LIST_
{
	int num;
	SORT_LIST_ELEMENT member[128];
}SORT_LIST;


#ifdef __cplusplus
}
#endif
