#ifndef _IPCAM_SHOW_MSG_INFO_HEADER
#define _IPCAM_SHOW_MSG_INFO_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#define CMD_SET_SHOW_MSG_INFO (0xa3)

#define NET_DVR_NET_SELF_DATA (1003)

#define MN_MAX_BUF_SIZE (256*1024)

#define MAX_SHOW_MSG_NUM (5)
#define MAX_MSG_NUM (40)


#define IPCAM_SOCKET_ERR (-2)
#define IPCAM_CONNECT_TIME_OUT (IPCAM_SOCKET_ERR-1)

typedef struct _IPCAM_SHOW_MSG_ST_
{
	int show_on_screen;   // show_on_screen = 1, msg will show on ipcam screen. show_on_screen = 0, not show on ipcam screen.
	int show_pos_x;       // The start x pos on screen when msg show. The value is a multiple of 4.    main_stream
	int show_pos_y;		  // The start y pos on screen when msg show. The value is a multiple of 4.      main_stream
	int show_pos_x1;       // The start x pos on screen when msg show. The value is a multiple of 4.   sub-stream
	int show_pos_y1;		  // The start y pos on screen when msg show. The value is a multiple of 4.    sub-stream
	int show_mode;      // show_mode=0 is main_stream  show_mode=1 is sub-stream.  show_mode=2 is osd show on both stream.
	char show_msg[MAX_MSG_NUM];  // The msg you want to show.
}IPCAM_SHOW_MSG_ST;

typedef struct _IPCAM_SHOW_MSG_INFO_
{
	IPCAM_SHOW_MSG_ST show_msg_st[MAX_SHOW_MSG_NUM];
}IPCAM_SHOW_MSG_INFO;

typedef struct _NET_DATA_ST{
	int cmd;
	int data_length;
}NET_DATA_ST;

int SetIpcamShowInfo(char * ip,int port,IPCAM_SHOW_MSG_INFO * pInfo);


#ifdef __cplusplus
}
#endif

#endif