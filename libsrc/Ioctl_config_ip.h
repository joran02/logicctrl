
#ifndef __IOCTL_CONFIG_IP_H
#define __IOCTL_CONFIG_IP_H


struct server_config_t {
 u_int32_t server; /* Our IP, in network order */
 u_int32_t start; /* Start address of leases, network order */
 u_int32_t end; /* End of leases, network order */
 struct option_set *options; /* List of DHCP options loaded from the config file */
 char *interface; /* The name of the interface to use */
 int ifindex; /* Index number of the interface to use */
 unsigned char arp[6]; /* Our arp address */
 unsigned long lease; /* lease time in seconds (host order) */
 unsigned long max_leases; /* maximum number of leases (including reserved address) */
 char remaining; /* should the lease file be interpreted as lease time remaining, or
 * as the time the lease expires */
 unsigned long auto_time; /* how long should udhcpd wait before writing a config file.
 * if this is zero, it will only write one on SIGUSR1 */
 unsigned long decline_time; /* how long an address is reserved if a client returns a
 * decline message */
 unsigned long conflict_time; /* how long an arp conflict offender is leased for */
 unsigned long offer_time; /* how long an offered address is reserved */
 unsigned long min_lease; /* minimum lease a client can request*/
 char *lease_file;
 char *pidfile;
 char *notify_file; /* What to run whenever leases are written */
 u_int32_t siaddr; /* next server bootp option */
 char *sname; /* bootp server name */
 char *boot_file; /* bootp boot file option */
}; 

typedef struct _net_ctrl_
{
	//XDVR_NET_CTRL_MODE_E enCtrlMode;   //XDVR_NET_CTRL_MODE_E
	unsigned char uIp[4];					// �ֶ�ʱ����
	unsigned char uNetMask[4];			// �ֶ�ʱ����
	unsigned char uNetGate[4];				// �ֶ�ʱ����
	//XDVR_NET_DNS_MODE_E  enDnsMode;   //DNS �����ֶ����Զ�ģʽ
	unsigned char uDns[2][4];				// �ֶ�ʱ��dhcpʱ������
	unsigned char uMac[6]; 				//����mac ��ַ
}XDVR_NET_CTRL_ST;


int Ioctl_get_ip(unsigned char ip[16], char *devName);
int Ioctl_get_ip_netmask(unsigned char ip[16], char *devName);
int Ioctl_get_mac(unsigned char addr[6], char *devName);

//int is_valid_ip(unsigned char ipaddr[16]);

/*
 * ����֤�Ƿ�Ϊ�Ϸ�IP��Ȼ������ת����32�޷������ͣ�ȡ��Ϊ000...00111...1��
 * Ȼ���ټ�1Ϊ00...01000...0����ʱΪ2^n����������Ϊ�Ϸ�����
 *
 * */
int Ioctl_is_valid_netmask(unsigned char netmask[16]);


int Ioctl_set_ip_netmask(unsigned char ip[16], char *devName);

int Ioctl_set_ip(unsigned char ip[16], char *devName);

int Ioctl_set_gateway(unsigned char ip[16]);

int Ioctl_set_br0_net(unsigned char ip[16], unsigned char netmsk[16], unsigned char gw[16]);

int Ioctl_Set_Eth0_Default_Network();

int Ioctl_Adjust_Dhcp_Result(char *devName);//����dhcp�����ip��ϢΪ�գ�������Ĭ��IP��

#endif /* __IOCTL_CONFIG_IP_H */
