#include "ptz.h"

unsigned char bySbuf_232[RS232_BUF_SIZE];
//unsigned char byProtocol;
//unsigned char byBauderate;
unsigned char rs232_send_len;
unsigned char rs232_device_id,ptz_h_speed,ptz_v_speed,ptz_point;//,ptz_point_time,ptz_group_no;

void ptz_buf_get(unsigned char protocol,unsigned char comm)
{	
	switch(protocol)
	{  
		case PROTOCOL_PELCO_D:
			switch(comm)
			{
				case PTZ_LEFT:
					rs232_send_len=lib_ptzpelcod_left();
					break;
				case PTZ_RIGHT:
					rs232_send_len=lib_ptzpelcod_right();
					break;
				case PTZ_UP:
					rs232_send_len=lib_ptzpelcod_up();
					break;
				case PTZ_DOWN:
					rs232_send_len=lib_ptzpelcod_down();
					break;
				case PTZ_UP_LEFT:
					rs232_send_len=lib_ptzpelcod_up_left();
					break;
				case PTZ_UP_RIGHT:
					rs232_send_len=lib_ptzpelcod_up_right();
					break;
				case PTZ_DOWN_LEFT:
					rs232_send_len=lib_ptzpelcod_down_left();
					break;
				case PTZ_DOWN_RIGHT:
					rs232_send_len=lib_ptzpelcod_down_right();
					break;
				case PTZ_FAR:
					rs232_send_len=lib_ptzpelcod_near();
					break;
				case PTZ_NEAR:
					rs232_send_len=lib_ptzpelcod_far();
					break;
				case PTZ_ZOOMIN:
					rs232_send_len=lib_ptzpelcod_zoomout();
					break;
				case PTZ_ZOOMOUT:
					rs232_send_len=lib_ptzpelcod_zoomin();
					break;
				case PTZ_IRISOPEN:
					rs232_send_len=lib_ptzpelcod_irisopen();
					break;
				case PTZ_IRISCLOSE:
					rs232_send_len=lib_ptzpelcod_irisclose();
					break;
				case PTZ_AUTO:
					rs232_send_len=lib_ptzpelcod_auto();
//					rs232_send_len=lib_ptzpelcod_auto1();
					break;
				case PTZ_STOP:
					rs232_send_len=lib_ptzpelcod_stop();
//					rs232_send_len=lib_ptzpelcod_stop1();
					break;
				case PTZ_PRESET:
					rs232_send_len=lib_ptzpelcod_preset();
					break;
				case PTZ_PRECLEAR:
					rs232_send_len=lib_ptzpelcod_predel();
					break;
				case PTZ_PREVIEW:
					rs232_send_len=lib_ptzpelcod_preview();
					break;
			}
			break;
		case PROTOCOL_PELCO_D1:
			switch(comm)
			{
				case PTZ_LEFT:
					rs232_send_len=lib_ptzpelcod_left();
					break;
				case PTZ_RIGHT:
					rs232_send_len=lib_ptzpelcod_right();
					break;
				case PTZ_UP:
					rs232_send_len=lib_ptzpelcod_up();
					break;
				case PTZ_DOWN:
					rs232_send_len=lib_ptzpelcod_down();
					break;
				case PTZ_UP_LEFT:
					rs232_send_len=lib_ptzpelcod_up_left();
					break;
				case PTZ_UP_RIGHT:
					rs232_send_len=lib_ptzpelcod_up_right();
					break;
				case PTZ_DOWN_LEFT:
					rs232_send_len=lib_ptzpelcod_down_left();
					break;
				case PTZ_DOWN_RIGHT:
					rs232_send_len=lib_ptzpelcod_down_right();
					break;
				case PTZ_FAR:
					rs232_send_len=lib_ptzpelcod_near();
					break;
				case PTZ_NEAR:
					rs232_send_len=lib_ptzpelcod_far();
					break;
				case PTZ_ZOOMIN:
					rs232_send_len=lib_ptzpelcod_zoomout();
					break;
				case PTZ_ZOOMOUT:
					rs232_send_len=lib_ptzpelcod_zoomin();
					break;
				case PTZ_IRISOPEN:
					rs232_send_len=lib_ptzpelcod_irisopen();
					break;
				case PTZ_IRISCLOSE:
					rs232_send_len=lib_ptzpelcod_irisclose();
					break;
				case PTZ_AUTO:
					rs232_send_len=lib_ptzpelcod_auto1();
					break;
				case PTZ_STOP:
					rs232_send_len=lib_ptzpelcod_stop1();
					break;
				case PTZ_PRESET:
					rs232_send_len=lib_ptzpelcod_preset();
					break;
				case PTZ_PRECLEAR:
					rs232_send_len=lib_ptzpelcod_predel();
					break;
				case PTZ_PREVIEW:
					rs232_send_len=lib_ptzpelcod_preview();
					break;
			}
			break;
		case PROTOCOL_PELCO_P:
			switch(comm)
			{
				case PTZ_LEFT:
					rs232_send_len=lib_ptzpelcop_left();
					break;
				case PTZ_RIGHT:
					rs232_send_len=lib_ptzpelcop_right();
					break;
				case PTZ_UP:
					rs232_send_len=lib_ptzpelcop_up();
					break;
				case PTZ_DOWN:
					rs232_send_len=lib_ptzpelcop_down();
					break;
				case PTZ_UP_LEFT:
					rs232_send_len=lib_ptzpelcop_up_left();
					break;
				case PTZ_UP_RIGHT:
					rs232_send_len=lib_ptzpelcop_up_right();
					break;
				case PTZ_DOWN_LEFT:
					rs232_send_len=lib_ptzpelcop_down_left();
					break;
				case PTZ_DOWN_RIGHT:
					rs232_send_len=lib_ptzpelcop_down_right();
					break;
				case PTZ_FAR:
					rs232_send_len=lib_ptzpelcop_near();
					break;
				case PTZ_NEAR:
					rs232_send_len=lib_ptzpelcop_far();
					break;
				case PTZ_ZOOMIN:
					rs232_send_len=lib_ptzpelcop_zoomout();
					break;
				case PTZ_ZOOMOUT:
					rs232_send_len=lib_ptzpelcop_zoomin();
					break;
				case PTZ_IRISOPEN:
					rs232_send_len=lib_ptzpelcop_irisopen();
					break;
				case PTZ_IRISCLOSE:
					rs232_send_len=lib_ptzpelcop_irisclose();
					break;
				case PTZ_AUTO:
					rs232_send_len=lib_ptzpelcop_auto();
					break;
				case PTZ_STOP:
					rs232_send_len=lib_ptzpelcop_stop();
					break;
				case PTZ_PRESET:
					rs232_send_len=lib_ptzpelcop_preset();
					break;
				case PTZ_PRECLEAR:
					rs232_send_len=lib_ptzpelcop_predel();
					break;
				case PTZ_PREVIEW:
					rs232_send_len=lib_ptzpelcop_preview();
					break;
				case PTZ_RIGHT_LIMIT:
					rs232_send_len=lib_ptzpelcop_right_limit();
					break;
				case PTZ_LEFT_LIMIT:
					rs232_send_len=lib_ptzpelcop_left_limit();
					break;
				case PTZ_RUN_GROUP:
					rs232_send_len=lib_ptzpelcop_run_group();
					break;
				case PTZ_STOP_GROUP:
					rs232_send_len=lib_ptzpelcop_stop_group();
					break;
			}
			break;
		case PROTOCOL_PELCO_P1:
			switch(comm)
			{
				case PTZ_LEFT:
					rs232_send_len=lib_ptzpelcop_left();
					break;
				case PTZ_RIGHT:
					rs232_send_len=lib_ptzpelcop_right();
					break;
				case PTZ_UP:
					rs232_send_len=lib_ptzpelcop_up();
					break;
				case PTZ_DOWN:
					rs232_send_len=lib_ptzpelcop_down();
					break;
				case PTZ_UP_LEFT:
					rs232_send_len=lib_ptzpelcop_up_left();
					break;
				case PTZ_UP_RIGHT:
					rs232_send_len=lib_ptzpelcop_up_right();
					break;
				case PTZ_DOWN_LEFT:
					rs232_send_len=lib_ptzpelcop_down_left();
					break;
				case PTZ_DOWN_RIGHT:
					rs232_send_len=lib_ptzpelcop_down_right();
					break;
				case PTZ_FAR:
					rs232_send_len=lib_ptzpelcop_near();
					break;
				case PTZ_NEAR:
					rs232_send_len=lib_ptzpelcop_far();
					break;
				case PTZ_ZOOMIN:
					rs232_send_len=lib_ptzpelcop_zoomout();
					break;
				case PTZ_ZOOMOUT:
					rs232_send_len=lib_ptzpelcop_zoomin();
					break;
				case PTZ_IRISOPEN:
					rs232_send_len=lib_ptzpelcop_irisopen();
					break;
				case PTZ_IRISCLOSE:
					rs232_send_len=lib_ptzpelcop_irisclose();
					break;
				case PTZ_AUTO:
					rs232_send_len=lib_ptzpelcop_auto1();
					break;
				case PTZ_STOP:
					rs232_send_len=lib_ptzpelcop_stop1();
					break;
				case PTZ_PRESET:
					rs232_send_len=lib_ptzpelcop_preset();
					break;
				case PTZ_PRECLEAR:
					rs232_send_len=lib_ptzpelcop_predel();
					break;
				case PTZ_PREVIEW:
					rs232_send_len=lib_ptzpelcop_preview();
					break;
				case PTZ_RIGHT_LIMIT:
					rs232_send_len=lib_ptzpelcop_right_limit();
					break;
				case PTZ_LEFT_LIMIT:
					rs232_send_len=lib_ptzpelcop_left_limit();
					break;
				case PTZ_RUN_GROUP:
					rs232_send_len=lib_ptzpelcop_run_group();
					break;
				case PTZ_STOP_GROUP:
					rs232_send_len=lib_ptzpelcop_stop_group();
					break;
			}
			break;
//		case PROTOCOL_USER1:
//			switch(comm)
//			{
//				case PTZ_LEFT:
//					rs232_send_len=lib_ptzuser1_left();
//					break;
//				case PTZ_RIGHT:
//					rs232_send_len=lib_ptzuser1_right();
//					break;
//				case PTZ_UP:
//					rs232_send_len=lib_ptzuser1_up();
//					break;
//				case PTZ_DOWN:
//					rs232_send_len=lib_ptzuser1_down();
//					break;
//				case PTZ_FAR:
//					rs232_send_len=lib_ptzuser1_near();
//					break;
//				case PTZ_NEAR:
//					rs232_send_len=lib_ptzuser1_far();
//					break;
//				case PTZ_ZOOMIN:
//					rs232_send_len=lib_ptzuser1_zoomout();
//					break;
//				case PTZ_ZOOMOUT:
//					rs232_send_len=lib_ptzuser1_zoomin();
//					break;
//				case PTZ_IRISOPEN:
//					rs232_send_len=lib_ptzuser1_irisopen();
//					break;
//				case PTZ_IRISCLOSE:
//					rs232_send_len=lib_ptzuser1_irisclose();
//					break;
//				case PTZ_AUTO:
//					rs232_send_len=lib_ptzuser1_auto();
//					break;
//				case PTZ_STOP:
//					rs232_send_len=lib_ptzuser1_stop();
//					break;
//				case PTZ_PRESET:
//					rs232_send_len=lib_ptzuser1_preset();
//					break;
//				case PTZ_PRECLEAR:
//					rs232_send_len=lib_ptzuser1_predel();
//					break;
//				case PTZ_PREVIEW:
//					rs232_send_len=lib_ptzuser1_preview();
//					break;
//			}
//			break;
//		case PROTOCOL_USER2:
//			switch(comm)
//			{
//				case PTZ_LEFT:
//					rs232_send_len=lib_ptzuser2_left();
//					break;
//				case PTZ_RIGHT:
//					rs232_send_len=lib_ptzuser2_right();
//					break;
//				case PTZ_UP:
//					rs232_send_len=lib_ptzuser2_up();
//					break;
//				case PTZ_DOWN:
//					rs232_send_len=lib_ptzuser2_down();
//					break;
//				case PTZ_FAR:
//					rs232_send_len=lib_ptzuser2_near();
//					break;
//				case PTZ_NEAR:
//					rs232_send_len=lib_ptzuser2_far();
//					break;
//				case PTZ_ZOOMIN:
//					rs232_send_len=lib_ptzuser2_zoomout();
//					break;
//				case PTZ_ZOOMOUT:
//					rs232_send_len=lib_ptzuser2_zoomin();
//					break;
//				case PTZ_IRISOPEN:
//					rs232_send_len=lib_ptzuser2_irisopen();
//					break;
//				case PTZ_IRISCLOSE:
//					rs232_send_len=lib_ptzuser2_irisclose();
//					break;
//				case PTZ_AUTO:
//					rs232_send_len=lib_ptzuser2_auto();
//					break;
//				case PTZ_STOP:
//					rs232_send_len=lib_ptzuser2_stop();
//					break;
//				case PTZ_PRESET:
//					rs232_send_len=lib_ptzuser2_preset();
//					break;
//				case PTZ_PRECLEAR:
//					rs232_send_len=lib_ptzuser2_predel();
//					break;
//				case PTZ_PREVIEW:
//					rs232_send_len=lib_ptzuser2_preview();
//					break;
//			}
//			break;
//		case PROTOCOL_USER3:
//			switch(comm)
//			{
//				case PTZ_LEFT:
//					rs232_send_len=lib_ptzuser3_left();
//					break;
//				case PTZ_RIGHT:
//					rs232_send_len=lib_ptzuser3_right();
//					break;
//				case PTZ_UP:
//					rs232_send_len=lib_ptzuser3_up();
//					break;
//				case PTZ_DOWN:
//					rs232_send_len=lib_ptzuser3_down();
//					break;
//				case PTZ_FAR:
//					rs232_send_len=lib_ptzuser3_near();
//					break;
//				case PTZ_NEAR:
//					rs232_send_len=lib_ptzuser3_far();
//					break;
//				case PTZ_ZOOMIN:
//					rs232_send_len=lib_ptzuser3_zoomout();
//					break;
//				case PTZ_ZOOMOUT:
//					rs232_send_len=lib_ptzuser3_zoomin();
//					break;
//				case PTZ_IRISOPEN:
//					rs232_send_len=lib_ptzuser3_irisopen();
//					break;
//				case PTZ_IRISCLOSE:
//					rs232_send_len=lib_ptzuser3_irisclose();
//					break;
//				case PTZ_AUTO:
//					rs232_send_len=lib_ptzuser3_auto();
//					break;
//				case PTZ_STOP:
//					rs232_send_len=lib_ptzuser3_stop();
//					break;
//				case PTZ_PRESET:
//					rs232_send_len=lib_ptzuser3_preset();
//					break;
//				case PTZ_PRECLEAR:
//					rs232_send_len=lib_ptzuser3_predel();
//					break;
//				case PTZ_PREVIEW:
//					rs232_send_len=lib_ptzuser3_preview();
//					break;
//			}
//			break;
//		case PROTOCOL_USER4:
//			switch(comm)
//			{
//				case PTZ_LEFT:
//					rs232_send_len=lib_ptzuser4_left();
//					break;
//				case PTZ_RIGHT:
//					rs232_send_len=lib_ptzuser4_right();
//					break;
//				case PTZ_UP:
//					rs232_send_len=lib_ptzuser4_up();
//					break;
//				case PTZ_DOWN:
//					rs232_send_len=lib_ptzuser4_down();
//					break;
//				case PTZ_FAR:
//					rs232_send_len=lib_ptzuser4_near();
//					break;
//				case PTZ_NEAR:
//					rs232_send_len=lib_ptzuser4_far();
//					break;
//				case PTZ_ZOOMIN:
//					rs232_send_len=lib_ptzuser4_zoomout();
//					break;
//				case PTZ_ZOOMOUT:
//					rs232_send_len=lib_ptzuser4_zoomin();
//					break;
//				case PTZ_IRISOPEN:
//					rs232_send_len=lib_ptzuser4_irisopen();
//					break;
//				case PTZ_IRISCLOSE:
//					rs232_send_len=lib_ptzuser4_irisclose();
//					break;
//				case PTZ_AUTO:
//					rs232_send_len=lib_ptzuser4_auto();
//					break;
//				case PTZ_STOP:
//					rs232_send_len=lib_ptzuser4_stop();
//					break;
//				case PTZ_PRESET:
//					rs232_send_len=lib_ptzuser4_preset();
//					break;
//				case PTZ_PRECLEAR:
//					rs232_send_len=lib_ptzuser4_predel();
//					break;
//				case PTZ_PREVIEW:
//					rs232_send_len=lib_ptzuser4_preview();
//					break;
//			}
//			break;
//		case PROTOCOL_DVR:
//			switch(comm)
//			{
//				case DVR_KEY_SPOT_SEQ:
//					rs232_send_len=dvr_pro_monitor_seq();
//					break;
//				case DVR_KEY_UP:
//					rs232_send_len=dvr_pro_up();
//				  break;
//				case DVR_KEY_DOWN:
//					rs232_send_len=dvr_pro_down();
//				  break;
//				case DVR_KEY_LEFT:
//					rs232_send_len=dvr_pro_left();
//				  break;
//				case DVR_KEY_RIGHT:
//					rs232_send_len=dvr_pro_right();
//				  break;
//				case DVR_KEY_ENTER:
//					rs232_send_len=dvr_pro_enter();
//				  break;
//				case DVR_KEY_MENU:
//					rs232_send_len=dvr_pro_menu();
//				  break;
//				case DVR_KEY_ESC:
//					rs232_send_len=dvr_pro_esc();
//				  break;
//				case DVR_KEY_REC:
//					rs232_send_len=dvr_pro_rec();
//				  break;
//				case DVR_KEY_PLAY_PAUSE:
//					rs232_send_len=dvr_pro_play_pause();
//				  break;
//				case DVR_KEY_STOP:
//					rs232_send_len=dvr_pro_stop();
//				  break;
//				case DVR_KEY_FR:
//					rs232_send_len=dvr_pro_fr();
//				  break;
//				case DVR_KEY_FF:
//					rs232_send_len=dvr_pro_ff();
//				  break;
//				case DVR_KEY_SEARCH:
//					rs232_send_len=dvr_pro_search();
//				  break;
//				case DVR_KEY_SEQ:
//					rs232_send_len=dvr_pro_seq();
//				  break;
//				case DVR_KEY_BACKUP:
//					rs232_send_len=dvr_pro_backup();
//				  break;
//				case DVR_KEY_AUDIO:
//					rs232_send_len=dvr_pro_audio();
//				  break;
//				case DVR_KEY_INFO:
//					rs232_send_len=dvr_pro_info();
//				  break;
//				case DVR_KEY_LOCK:
//					rs232_send_len=dvr_pro_lock();
//				  break;
//				case DVR_KEY_UNLOCK:
//					rs232_send_len=dvr_pro_unlock();
//				  break;
//				case DVR_KEY_SWAP:
//					rs232_send_len=dvr_pro_swap();
//				  break;
//				case DVR_KEY_SPLIT4:
//					rs232_send_len=dvr_pro_split4();
//				  break;
//				case DVR_KEY_SPLIT9:
//					rs232_send_len=dvr_pro_split9();
//				  break;
//				case DVR_KEY_SPLIT16:
//					rs232_send_len=dvr_pro_split16();
//				  break;
//				case DVR_KEY_SPLIT25:
//					rs232_send_len=dvr_pro_split25();
//				  break;
//				case DVR_KEY_SPLIT36:
//					rs232_send_len=dvr_pro_split36();
//				  break;
//				case DVR_KEY_SPLIT49:
//					rs232_send_len=dvr_pro_split49();
//				  break;
//				case DVR_KEY_SPLIT64:
//					rs232_send_len=dvr_pro_split64();
//				  break;
//			 	case DVR_KEY_CAM:
//			 		rs232_send_len=dvr_pro_cam();
//				 	break;
//				case DVR_KEY_MONITOR:
//			 		rs232_send_len=dvr_pro_monitor();
//				 	break;
//				case DVR_SET_ALARM_ON:
//			 		rs232_send_len=dvr_pro_relay_on();
//					break;
//				case DVR_SET_ALARM_OFF:
//			 		rs232_send_len=dvr_pro_relay_off();
//					break;
//				case DVR_SET_BUZZER_ON:
//					break;
//				case DVR_SET_BUZZER_OFF:
//					break;
//				case DVR_KEY_LISTEN_ON:
//			 		rs232_send_len=dvr_pro_listen_on();
//					break;
//				case DVR_KEY_LISTEN_OFF:
//			 		rs232_send_len=dvr_pro_listen_off();
//					break;
//				case DVR_KEY_SPEAKER_ON:
//			 		rs232_send_len=dvr_pro_speaker_on();
//					break;
//				case DVR_KEY_SPEAKER_OFF:
//			 		rs232_send_len=dvr_pro_speaker_off();
//					break;
//			}
//			break;
	}
}
