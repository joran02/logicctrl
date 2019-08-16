#ifndef _PTZ_H_
#define _PTZ_H_

#define RS232_BUF_SIZE 10
//ptz protocol
enum
{
  PROTOCOL_PELCO_D=0,
  PROTOCOL_PELCO_D1,
  PROTOCOL_PELCO_P,
  PROTOCOL_PELCO_P1,
  PROTOCOL_DVR,
//  PROTOCOL_USER1,
//  PROTOCOL_USER2,
//  PROTOCOL_USER3,
//  PROTOCOL_USER4,
//  PROTOCOL_PTZDSCP,
//  PROTOCOL_LILIN,
//  PROTOCOL_MINKING,
//  PROTOCOL_USER5,
//  PROTOCOL_USER6,
//  PROTOCOL_USER7,
//  PROTOCOL_USER8,
};
//ptz command
//enum
//{
//	PTZ_LEFT,
//	PTZ_RIGHT,
//	PTZ_UP,
//	PTZ_DOWN,
//	PTZ_FAR,
//	PTZ_NEAR,
//	PTZ_ZOOMIN,
//	PTZ_ZOOMOUT,
//	PTZ_IRISOPEN,
//	PTZ_IRISCLOSE,
//	PTZ_AUTO,
//	PTZ_STOP,
//};
enum
{
	PTZ_LEFT=1,
	PTZ_RIGHT,
	PTZ_UP,
	PTZ_DOWN,
	PTZ_FAR,
	PTZ_NEAR,
	PTZ_ZOOMIN,
	PTZ_ZOOMOUT,
	PTZ_IRISOPEN,
	PTZ_IRISCLOSE,
	PTZ_AUTO,
	PTZ_STOP,
	PTZ_PRESET,
	PTZ_PRECLEAR,
	PTZ_PREVIEW,
	
	PTZ_UP_LEFT,
	PTZ_UP_RIGHT,
	PTZ_DOWN_LEFT,
	PTZ_DOWN_RIGHT,
	PTZ_LEFT_LIMIT,
	PTZ_RIGHT_LIMIT,
	PTZ_RUN_GROUP,
	PTZ_STOP_GROUP,
	PTZ_SET_GROUP,
	
	DVR_SET_ALARM,
	DVR_SET_ALARM_ON,
	DVR_SET_ALARM_OFF,
	DVR_SET_BUZZER,
	DVR_SET_BUZZER_ON,
	DVR_SET_BUZZER_OFF,
	
	DVR_KEY_UP,
	DVR_KEY_DOWN,
	DVR_KEY_LEFT,
	DVR_KEY_RIGHT,
	DVR_KEY_ENTER,
	DVR_KEY_MENU,
	DVR_KEY_ESC,
	DVR_KEY_REC,
	DVR_KEY_PLAY_PAUSE,
	DVR_KEY_STOP,
	DVR_KEY_FR,
	DVR_KEY_FF,
	DVR_KEY_SEARCH,
	DVR_KEY_SEQ,
	DVR_KEY_BACKUP,
	DVR_KEY_AUDIO,
	DVR_KEY_INFO,
	DVR_KEY_LOCK,
	DVR_KEY_UNLOCK,
	DVR_KEY_SWAP,
	DVR_KEY_SPLIT4,
	DVR_KEY_SPLIT9,
	DVR_KEY_SPLIT16,
	DVR_KEY_SPLIT25,
	DVR_KEY_SPLIT36,
	DVR_KEY_SPLIT49,
	DVR_KEY_SPLIT64,
	DVR_KEY_CAM,
	DVR_KEY_MONITOR,
	DVR_KEY_LISTEN,
	DVR_KEY_LISTEN_ON,
	DVR_KEY_LISTEN_OFF,
	DVR_KEY_SPEAKER,
	DVR_KEY_SPEAKER_ON,
	DVR_KEY_SPEAKER_OFF,
	DVR_KEY_SPOT_TMP,
	DVR_KEY_SPOT_SEQ,
};
extern unsigned char lib_ptzpelcod_left();
extern unsigned char lib_ptzpelcod_right();
extern unsigned char lib_ptzpelcod_up();
extern unsigned char lib_ptzpelcod_down();
extern unsigned char lib_ptzpelcod_near();
extern unsigned char lib_ptzpelcod_far();
extern unsigned char lib_ptzpelcod_zoomout();
extern unsigned char lib_ptzpelcod_zoomin();
extern unsigned char lib_ptzpelcod_irisopen();
extern unsigned char lib_ptzpelcod_irisclose();
extern unsigned char lib_ptzpelcod_auto();
extern unsigned char lib_ptzpelcod_auto1();
extern unsigned char lib_ptzpelcod_up_left();
extern unsigned char lib_ptzpelcod_up_right();
extern unsigned char lib_ptzpelcod_down_left();
extern unsigned char lib_ptzpelcod_down_right();
extern unsigned char lib_ptzpelcod_stop();
extern unsigned char lib_ptzpelcod_stop1();
extern unsigned char lib_ptzpelcod_preset();
extern unsigned char lib_ptzpelcod_preview();
extern unsigned char lib_ptzpelcod_predel();

extern unsigned char lib_ptzpelcop_left();
extern unsigned char lib_ptzpelcop_right();
extern unsigned char lib_ptzpelcop_up();
extern unsigned char lib_ptzpelcop_down();
extern unsigned char lib_ptzpelcop_near();
extern unsigned char lib_ptzpelcop_far();
extern unsigned char lib_ptzpelcop_zoomout();
extern unsigned char lib_ptzpelcop_zoomin();
extern unsigned char lib_ptzpelcop_irisopen();
extern unsigned char lib_ptzpelcop_irisclose();
extern unsigned char lib_ptzpelcop_up_left();
extern unsigned char lib_ptzpelcop_up_right();
extern unsigned char lib_ptzpelcop_down_left();
extern unsigned char lib_ptzpelcop_down_right();
extern unsigned char lib_ptzpelcop_auto();
extern unsigned char lib_ptzpelcop_stop();
extern unsigned char lib_ptzpelcop_auto1();
extern unsigned char lib_ptzpelcop_stop1();
extern unsigned char lib_ptzpelcop_preset();
extern unsigned char lib_ptzpelcop_preview();
extern unsigned char lib_ptzpelcop_predel();
extern unsigned char lib_ptzpelcop_right_limit();
extern unsigned char lib_ptzpelcop_left_limit();
extern unsigned char lib_ptzpelcop_run_group();
extern unsigned char lib_ptzpelcop_stop_group();

//extern unsigned char ptzuser1_left();
//extern unsigned char ptzuser1_right();
//extern unsigned char ptzuser1_up();
//extern unsigned char ptzuser1_down();
//extern unsigned char ptzuser1_near();
//extern unsigned char ptzuser1_far();
//extern unsigned char ptzuser1_zoomout();
//extern unsigned char ptzuser1_zoomin();
//extern unsigned char ptzuser1_irisopen();
//extern unsigned char ptzuser1_irisclose();
//extern unsigned char ptzuser1_auto();
//extern unsigned char ptzuser1_stop();
//extern unsigned char ptzuser1_preset();
//extern unsigned char ptzuser1_preview();
//extern unsigned char ptzuser1_predel();
//
//extern unsigned char ptzuser2_left();
//extern unsigned char ptzuser2_right();
//extern unsigned char ptzuser2_up();
//extern unsigned char ptzuser2_down();
//extern unsigned char ptzuser2_near();
//extern unsigned char ptzuser2_far();
//extern unsigned char ptzuser2_zoomout();
//extern unsigned char ptzuser2_zoomin();
//extern unsigned char ptzuser2_irisopen();
//extern unsigned char ptzuser2_irisclose();
//extern unsigned char ptzuser2_auto();
//extern unsigned char ptzuser2_stop();
//extern unsigned char ptzuser2_preset();
//extern unsigned char ptzuser2_preview();
//extern unsigned char ptzuser2_predel();
//
//extern unsigned char ptzuser3_left();
//extern unsigned char ptzuser3_right();
//extern unsigned char ptzuser3_up();
//extern unsigned char ptzuser3_down();
//extern unsigned char ptzuser3_near();
//extern unsigned char ptzuser3_far();
//extern unsigned char ptzuser3_zoomout();
//extern unsigned char ptzuser3_zoomin();
//extern unsigned char ptzuser3_irisopen();
//extern unsigned char ptzuser3_irisclose();
//extern unsigned char ptzuser3_auto();
//extern unsigned char ptzuser3_stop();
//extern unsigned char ptzuser3_preset();
//extern unsigned char ptzuser3_preview();
//extern unsigned char ptzuser3_predel();
//
//extern unsigned char ptzuser4_left();
//extern unsigned char ptzuser4_right();
//extern unsigned char ptzuser4_up();
//extern unsigned char ptzuser4_down();
//extern unsigned char ptzuser4_near();
//extern unsigned char ptzuser4_far();
//extern unsigned char ptzuser4_zoomout();
//extern unsigned char ptzuser4_zoomin();
//extern unsigned char ptzuser4_irisopen();
//extern unsigned char ptzuser4_irisclose();
//extern unsigned char ptzuser4_auto();
//extern unsigned char ptzuser4_stop();
//extern unsigned char ptzuser4_preset();
//extern unsigned char ptzuser4_preview();
//extern unsigned char ptzuser4_predel();

extern unsigned char dvr_pro_up();
extern unsigned char dvr_pro_down();
extern unsigned char dvr_pro_left();
extern unsigned char dvr_pro_right();
extern unsigned char dvr_pro_enter();
extern unsigned char dvr_pro_menu();
extern unsigned char dvr_pro_esc();
extern unsigned char dvr_pro_rec();
extern unsigned char dvr_pro_play_pause();
extern unsigned char dvr_pro_stop();
extern unsigned char dvr_pro_fr();
extern unsigned char dvr_pro_ff();
extern unsigned char dvr_pro_search();
extern unsigned char dvr_pro_seq();
extern unsigned char dvr_pro_backup();
extern unsigned char dvr_pro_audio();
extern unsigned char dvr_pro_info();
extern unsigned char dvr_pro_lock();
extern unsigned char dvr_pro_unlock();
extern unsigned char dvr_pro_swap();
extern unsigned char dvr_pro_split4();
extern unsigned char dvr_pro_split9();
extern unsigned char dvr_pro_split16();
extern unsigned char dvr_pro_split25();
extern unsigned char dvr_pro_split36();
extern unsigned char dvr_pro_split49();
extern unsigned char dvr_pro_split64();
extern unsigned char dvr_pro_cam();
extern unsigned char dvr_pro_monitor();
extern unsigned char dvr_pro_monitor_seq();
extern unsigned char dvr_pro_relay_on();
extern unsigned char dvr_pro_relay_off();
extern unsigned char dvr_pro_listen_on();
extern unsigned char dvr_pro_listen_off();
extern unsigned char dvr_pro_speaker_on();
extern unsigned char dvr_pro_speaker_off();

extern void ptz_buf_get(unsigned char protocol,unsigned char comm);

extern unsigned char bySbuf_232[RS232_BUF_SIZE];
//extern unsigned char byProtocol;
//extern unsigned char byBauderate;
extern unsigned char rs232_send_len;
extern unsigned char rs232_device_id,ptz_h_speed,ptz_v_speed,ptz_point;

#endif//_PTZ_H_
