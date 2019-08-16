// Mail.h: interface for the CMail class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_MAIL_H__1FEA1065_B7F9_4037_A638_42ABD06A153F__INCLUDED_
#define AFX_MAIL_H__1FEA1065_B7F9_4037_A638_42ABD06A153F__INCLUDED_

#define NETINIT_ERROR -2
#define CONNECTSERVER_DDNS_ERROR -3
#define CONNECT_ERROR -4
#define WSA_RECV_ERROR -5
#define WSA_SEND_ERROR -6
#define UNKNOWEN_ERROR -7
#define HELO_ERROR -8
#define AUTH_LOGIN_ERROR -9
#define USER_ERROR -10
#define PASS_ERROR -11
#define MAIL_FROM_ERROR -12
#define RCPT_TO_ERROR -13
#define DATA_ERROR -14
#define MAILHEADER_ERROR -15
#define OPEN_FILE_ERROR -16
#define END_DATA_ERROR -17
#define SOCK_CREATE_ERROR -18

#define XPRIORITY_HIGH 1
#define XPRIORITY_NORMAL 2
#define XPRIORITY_LOW 3

int dvr_send_mail(char * pSmtpServer,int iPort,char * pUserName,char * pPass,
					char * pMailFrom,char * pMailSendto,
					char * pSubject,char* pData,char* p_attachment,int m_priority);


#endif // !defined(AFX_MAIL_H__1FEA1065_B7F9_4037_A638_42ABD06A153F__INCLUDED_)
