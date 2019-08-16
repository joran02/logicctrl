#include <sys/socket.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <net/route.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#if defined(__GLIBC__) && __GLIBC__ >=2 && __GLIBC_MINOR__ >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>
#else
#include <sys/types.h>
#include <netinet/if_ether.h>
#endif
//#include <arpa/inet.h>
#include "Ioctl_config_ip.h"
#include <errno.h>

#define DPRINTK(fmt, args...)	printf("(%s,%d)%s: " fmt,__FILE__,__LINE__, __FUNCTION__ , ## args)

typedef signed int SINT32;
typedef signed char SINT8;
#define FALSE 0
#define TRUE 1
#define DOWN 0
#define UP 1
#define DEFAULT_ETH "eth0"
#define IP_LENGTH 16

static int Ioctl_if_updown(char *ifname, int flag);
static int Ioctl_is_valid_ip(unsigned char ipaddr[16]);
static int Ioctl_set_addr(unsigned char ip[16], int flag, char *devName);
static int Ioctl_get_addr(unsigned char ip[16], int flag, char *devName);

int Ioctl_get_ip(unsigned char ip[16], char *devName)
{
    return Ioctl_get_addr(ip, SIOCGIFADDR, devName);
}

int Ioctl_get_ip_netmask(unsigned char ip[16], char *devName)
{
    return Ioctl_get_addr(ip, SIOCGIFNETMASK, devName);
}

int Ioctl_get_mac(unsigned char addr[6], char *devName)
{
    return Ioctl_get_addr(addr, SIOCGIFHWADDR, devName);
}

static int Ioctl_get_addr(unsigned char *addr, int flag, char *devName)
{
    SINT32 sockfd = 0;
    struct sockaddr_in *sin;
    struct ifreq ifr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error!\n");
        return FALSE;
    }

    memset(&ifr, 0, sizeof(ifr));

    if (devName == NULL || strlen(devName) < 1)
        snprintf(ifr.ifr_name, (sizeof(ifr.ifr_name) - 1), "%s", DEFAULT_ETH);
    else
        snprintf(ifr.ifr_name, (sizeof(ifr.ifr_name) - 1), "%s", devName);

    if(ioctl(sockfd, flag, &ifr) < 0 )
    {
        perror("ioctl error!\n");
        close(sockfd);
        return FALSE;
    }
    close(sockfd);

    if (SIOCGIFHWADDR == flag){
        memcpy((void *)addr, (const void *)&ifr.ifr_ifru.ifru_hwaddr.sa_data, 6);
        /*printf("mac address: %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x\n", addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);*/
    }else if(SIOCGIFFLAGS == flag){
      if((ifr.ifr_flags & IFF_UP)==IFF_UP)
        printf("%s is up, 0x%x\n", ifr.ifr_name, ifr.ifr_flags);
      else
        printf("%s is down, 0x%x\n", ifr.ifr_name, ifr.ifr_flags);
    }else{
        sin = (struct sockaddr_in *)&ifr.ifr_addr;
        snprintf((char *)addr, IP_LENGTH, "%s", inet_ntoa(sin->sin_addr));
    }

    return TRUE;
}

static int Ioctl_get_network_mtu(int *mtu_size, int flag, char *devName)
{
    SINT32 sockfd = 0;
    struct sockaddr_in *sin;
    struct ifreq ifr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error!\n");
        return FALSE;
    }

    memset(&ifr, 0, sizeof(ifr));
    if (devName == NULL || strlen(devName) < 1)
        snprintf(ifr.ifr_name, (sizeof(ifr.ifr_name) - 1), "%s", DEFAULT_ETH);
    else
        snprintf(ifr.ifr_name, (sizeof(ifr.ifr_name) - 1), "%s", devName);

    if(ioctl(sockfd, flag, &ifr) < 0 )
    {
        perror("ioctl error!\n");
        close(sockfd);
        return FALSE;
    }
    close(sockfd);

    if ( SIOCGIFMTU == flag){
        *mtu_size = ifr.ifr_ifru.ifru_mtu;
        printf("net mtu = %d____%d___\n", *mtu_size, ifr.ifr_ifru.ifru_mtu);
    }

    return TRUE;
}

static int Ioctl_set_network_mtu(int *mtu_size, int flag, char *devName)
{
    SINT32 sockfd = 0;
    struct sockaddr_in *sin;
    struct ifreq ifr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error!\n");
        return FALSE;
    }

    memset(&ifr, 0, sizeof(ifr));
    if (devName == NULL || strlen(devName) < 1)
        snprintf(ifr.ifr_name, (sizeof(ifr.ifr_name) - 1), "%s", DEFAULT_ETH);
    else
        snprintf(ifr.ifr_name, (sizeof(ifr.ifr_name) - 1), "%s", devName);
    ifr.ifr_ifru.ifru_mtu = *mtu_size;
    printf("net mtu = %d____%d___\n", *mtu_size, ifr.ifr_ifru.ifru_mtu);

    if(ioctl(sockfd, flag, &ifr) < 0 )
    {
        perror("ioctl error!\n");
        close(sockfd);
        return FALSE;
    }
    close(sockfd);

    return TRUE;
}

static int Ioctl_is_valid_ip(unsigned char ipaddr[16])
{
    int ret = 0;
    struct in_addr inp;
    ret = inet_aton(ipaddr, &inp);
    if (0 == ret)
    {
        return FALSE;
    }
    else
    {
        printf("inet_aton:ip=%lu\n",ntohl(inp.s_addr));
    }

    return TRUE;
}

/*

 * */
int Ioctl_is_valid_netmask(unsigned char netmask[16])
{
    if(Ioctl_is_valid_ip(netmask) > 0)
    {
        unsigned int b = 0, i, n[4];
        sscanf(netmask, "%u.%u.%u.%u", &n[3], &n[2], &n[1], &n[0]);

        if((b & (b - 1)) == 0) //判断是否为2^n
            return TRUE;
    }

    return FALSE;
}


int Ioctl_set_ip_netmask(unsigned char ip[16], char *devName)
{
    return Ioctl_set_addr(ip, SIOCSIFNETMASK, devName);
}

int Ioctl_set_ip(unsigned char ip[16], char *devName)
{
    return Ioctl_set_addr(ip, SIOCSIFADDR, devName);
}

int Ioctl_set_mac(unsigned char addr[6], char *devName)
{
    return Ioctl_set_addr(addr, SIOCSIFHWADDR, devName);
}

/*int set_mac_addr(char *ifname, char *mac)
{
    int fd, rtn;
    struct ifreq ifr;

    if( !ifname || !mac ) {
        return -1;
    }
    fd = socket(AF_INET, SOCK_DGRAM, 0 );
    if ( fd < 0 ) {
        perror("socket");
        return -1;
    }
    ifr.ifr_addr.sa_family = ARPHRD_ETHER;
    strncpy(ifr.ifr_name, (const char *)ifname, IFNAMSIZ - 1 );
    memcpy((unsigned char *)ifr.ifr_hwaddr.sa_data, mac, 6);
    
    if ( (rtn = ioctl(fd, SIOCSIFHWADDR, &ifr) ) != 0 ){
        perror("SIOCSIFHWADDR");
    }
    close(fd);
    return rtn;
}*/

static int Ioctl_set_addr(unsigned char ip[16], int flag, char *devName)
{
    printf("set_addr ip:%s__   flag=0x%x__\n", ip, flag);
    struct ifreq ifr;
    struct sockaddr_in sin;
    int sockfd;
    int ret = 0;

    if (Ioctl_is_valid_ip(ip) < 0)
    {
        printf("ip was invalid!\n");
        return FALSE;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1){
        fprintf(stderr, "Could not get socket.\n");
        perror("eth0\n");
        return FALSE;
    }

    if (devName == NULL || strlen(devName) < 1)
        snprintf(ifr.ifr_name, (sizeof(ifr.ifr_name) - 1), "%s", DEFAULT_ETH);
    else
        snprintf(ifr.ifr_name, (sizeof(ifr.ifr_name) - 1), "%s", devName);

    /* Read interface flags */
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
        fprintf(stderr, "ifdown: shutdown ");
        perror(ifr.ifr_name);
	  close(sockfd);
        return FALSE;
    }

    memset(&sin, 0, sizeof(struct sockaddr));
    if (SIOCSIFHWADDR == flag)
    {
	Ioctl_if_updown(DEFAULT_ETH, DOWN);
	ifr.ifr_addr.sa_family = ARPHRD_ETHER;
    	//memcpy((unsigned char *)ifr.ifr_hwaddr.sa_data, ip, 6);
        memcpy((unsigned char *)&ifr.ifr_ifru.ifru_hwaddr.sa_data,ip, 6);
    }else{
	    sin.sin_family = AF_INET;
	    inet_aton(ip, &sin.sin_addr.s_addr);
	    memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
    }
    ret = ioctl(sockfd, flag, &ifr);
    if (ret < 0){
      fprintf(stderr, "Cannot set IP address. ret=0x%x__\n",ret);
      perror(ifr.ifr_name);
      if (SIOCSIFHWADDR == flag)
        Ioctl_if_updown(DEFAULT_ETH, UP);
	close(sockfd);
      return FALSE;
    }
    if (SIOCSIFHWADDR == flag)
      Ioctl_if_updown(DEFAULT_ETH, UP);
    close(sockfd);
    return TRUE;
}


int Ioctl_set_gateway(unsigned char ip[16])
{
    int sockFd,ret=0;
    struct sockaddr_in sockaddr;
    struct rtentry rt;
    char tBuf[50]={0};

    if (Ioctl_is_valid_ip(ip) < 0)
    {
        printf("gateway was invalid!\n");
        return FALSE;
    }

    sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockFd < 0)
    {
        perror("Socket create error.\n");
        return FALSE;
    }

    memset(&rt, 0, sizeof(struct rtentry));
    memset(&sockaddr, 0, sizeof(struct sockaddr_in));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = 0;
    if(inet_aton(ip, &sockaddr.sin_addr)<0)
    {
        perror("inet_aton error\n" );
        close(sockFd);
        return FALSE;
    }

    memcpy ( &rt.rt_gateway, &sockaddr, sizeof(struct sockaddr_in));
    ((struct sockaddr_in *)&rt.rt_dst)->sin_family=AF_INET;
    ((struct sockaddr_in *)&rt.rt_genmask)->sin_family=AF_INET;
    rt.rt_flags = RTF_GATEWAY;
    ret = ioctl(sockFd, SIOCADDRT, &rt);
    if (ret < 0)
    {
        sprintf(tBuf, "%s", strerror(errno));
		if (sockFd >= 0)
		{
		    close(sockFd);
		  sockFd = -1;
		}
        
        if (strcmp("File exists", tBuf) == 0)
        {
        	printf("-1 error in set_default_route___errno:%s___\n", tBuf);
        	return -1;
        }else if (strcmp("No such process", tBuf) == 0)
        {
        	printf("-2 error in set_default_route___errno:%s___\n", tBuf);
        	return -2;
        }else if (strcmp("Invalid argument", tBuf) == 0)
        {
        	printf("-3 error in set_default_route___errno:%s___\n", tBuf);
        	return -3;
        }
        //return FALSE;
    }

    if (sockFd >= 0)
    {
        close(sockFd);
	  sockFd = -1;
    }
    return TRUE;
}

int Ioctl_del_gateway(unsigned char ip[16])
{
    int sockFd,ret=0;
    struct sockaddr_in sockaddr;
    struct rtentry rt;

    if (Ioctl_is_valid_ip(ip) < 0)
    {
        printf("gateway was invalid!\n");
        return FALSE;
    }

    sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockFd < 0)
    {
        perror("Socket create error.\n");
        return FALSE;
    }

    memset(&rt, 0, sizeof(struct rtentry));
    memset(&sockaddr, 0, sizeof(struct sockaddr_in));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = 0;
    if(inet_aton(ip, &sockaddr.sin_addr)<0)
    {
        perror("inet_aton error\n" );
        close(sockFd);
        return FALSE;
    }

    memcpy ( &rt.rt_gateway, &sockaddr, sizeof(struct sockaddr_in));
    ((struct sockaddr_in *)&rt.rt_dst)->sin_family=AF_INET;
    ((struct sockaddr_in *)&rt.rt_genmask)->sin_family=AF_INET;
    rt.rt_flags = RTF_GATEWAY;
    ret = ioctl(sockFd, SIOCDELRT, &rt);
    if (ret < 0)
    {
        printf("ioctl(SIOCADDRT) error in set_default_route___errno:%s___\n", strerror(errno));
        close(sockFd);
        return FALSE;
    }

    close(sockFd);

    return TRUE;
}

static int Ioctl_if_updown(char *ifname, int flag)
{
    int fd, rtn;
    struct ifreq ifr;        
    printf("ifname:%s to _%d_\n",ifname,flag);
    if (!ifname) {
        return -1;
    }

    fd = socket(AF_INET, SOCK_DGRAM, 0 );
    if ( fd < 0 ) {
        perror("socket");
        return -1;
    }
    
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, (const char *)ifname, IFNAMSIZ - 1 );

    if ( (rtn = ioctl(fd, SIOCGIFFLAGS, &ifr) ) == 0 ) {
        if ( flag == DOWN )
            ifr.ifr_flags &= ~IFF_UP;
        else if ( flag == UP ) 
            ifr.ifr_flags |= IFF_UP;
        
    }

    if ( (rtn = ioctl(fd, SIOCSIFFLAGS, &ifr) ) != 0) {
        perror("SIOCSIFFLAGS");
    }

    close(fd);

    return rtn;
}

int Ioctl_set_br0_net(unsigned char ip[16], unsigned char netmsk[16], unsigned char gw[16])
{
    int count_set = 0;
    unsigned char tBuf[16];
    snprintf(tBuf, sizeof(tBuf),"%s",gw);
    Ioctl_set_ip("0.0.0.0", "eth0");
    Ioctl_set_ip("0.0.0.0", "ra0");
    Ioctl_set_ip("0.0.0.0", "br0");
    Ioctl_del_gateway("0.0.0.0");
    Ioctl_set_ip(ip, "br0");
    Ioctl_set_ip_netmask(netmsk, "br0");
retrySetGw:
	if (Ioctl_set_gateway(tBuf) < -1)//-1是存在网关，-2是设置网关失败，-3是非法网关
	{
		count_set++;
		printf("br0 set ipGateway:_%s_ error!\n", tBuf);
		if (strcmp(tBuf,"0.0.0.0") != 0)
		{
			if (count_set <= 1)
			{//防止ip错误一直循环。
				unsigned int tmp_num[4];					
				sscanf(ip,"%d.%d.%d.%d",&tmp_num[0],&tmp_num[1],&tmp_num[2],&tmp_num[3]);
				
				sprintf(tBuf, "%d.%d.%d.1",tmp_num[0],tmp_num[1],tmp_num[2]);
				printf("br0 change set ipGateway:_%s_ !\n", tBuf);
				goto retrySetGw;
			}
		}
	}
    Ioctl_set_ip(ip, "eth0");
    Ioctl_set_ip_netmask(netmsk, "eth0");
    Ioctl_set_ip(ip, "ra0");
    Ioctl_set_ip_netmask(netmsk, "ra0");
}

int Ioctl_set_eth0_net(unsigned char ip[16], unsigned char netmsk[16], unsigned char gw[16])
{
    int count_set = 0;
    unsigned char tBuf[16];
    snprintf(tBuf, sizeof(tBuf),"%s",gw);
    Ioctl_set_ip("0.0.0.0", "eth0");
    Ioctl_del_gateway("0.0.0.0");
    Ioctl_set_ip(ip, "eth0");
    Ioctl_set_ip_netmask(netmsk, "eth0");
retrySetGw:
	if (Ioctl_set_gateway(tBuf) < -1)//-1是存在网关，-2是设置网关失败，-3是非法网关
	{
		count_set++;
		printf("eth0 set ipGateway:_%s_ error!\n", tBuf);
		if (strcmp(tBuf,"0.0.0.0") != 0)
		{
			if (count_set <= 1)
			{//防止ip错误一直循环。
				unsigned int tmp_num[4];					
				sscanf(ip,"%d.%d.%d.%d",&tmp_num[0],&tmp_num[1],&tmp_num[2],&tmp_num[3]);
				
				sprintf(tBuf, "%d.%d.%d.1",tmp_num[0],tmp_num[1],tmp_num[2]);
				printf("eth0 change set ipGateway:_%s_ !\n", tBuf);
				goto retrySetGw;
			}
		}
	}
}

int Ioctl_Set_Eth0_Default_Network()
{
    DPRINTK("start set default network info\n");
    unsigned char ip_value[16]={0};
    unsigned char msk_value[16]={0};
    unsigned char gw_value[16]={0};

    strcpy(ip_value, "192.168.1.18");
    strcpy(msk_value, "255.255.255.0");
    strcpy(gw_value, "192.168.1.1");
    Ioctl_set_eth0_net(ip_value, msk_value, gw_value);
    return 0;
}

int Ioctl_Adjust_Dhcp_Result(char *devName)
{
    unsigned char ip_value[16]={0};
    unsigned char msk_value[16]={0};
    unsigned char gw_value[16]={0};

    int ret = 0;
    unsigned char tmp_value[16] = {0};
    ret = Ioctl_get_ip(tmp_value,devName);
    if (ret == 0)
    {//未获取到DHCP信息
        Ioctl_Set_Eth0_Default_Network();
        return 1;
    }else{
        return 0;
    }
}

int read_interface(char *interface, int *ifindex, u_int32_t *addr, unsigned char *arp)
{
 int fd = -1;
 struct ifreq ifr;
 struct sockaddr_in *our_ip;

 memset(&ifr, 0, sizeof(struct ifreq));
 if((fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) 
 {
	 ifr.ifr_addr.sa_family = AF_INET;
	 strcpy(ifr.ifr_name, interface);

	 /*this is not execute*/
	 if (addr) 
	 {  
		 if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) 
		 {
			 our_ip = (struct sockaddr_in *) &ifr.ifr_addr;
			 *addr = our_ip->sin_addr.s_addr;
			// DEBUG(LOG_INFO, "%s (our ip) = %s", ifr.ifr_name, inet_ntoa(our_ip->sin_addr));
		 } else {
			printf("SIOCGIFADDR failed, is the interface up and configured?: %s", strerror(errno));
			 goto err;
		 }
	 }

	 if (ioctl(fd, SIOCGIFINDEX, &ifr) == 0) 
	 {
		// DEBUG(LOG_INFO, "adapter index %d", ifr.ifr_ifindex);
		 *ifindex = ifr.ifr_ifindex;
	 } else {
		printf("SIOCGIFINDEX failed!: %s", strerror(errno));
		  goto err;
	 }
	 if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0)
	 {
		 memcpy(arp, ifr.ifr_hwaddr.sa_data, 6);
		//DEBUG(LOG_INFO, "adapter hardware address %02x:%02x:%02x:%02x:%02x:%02x",
		//arp[0], arp[1], arp[2], arp[3], arp[4], arp[5]);
	 } else {
		printf("SIOCGIFHWADDR failed!: %s", strerror(errno));
		goto err;
	 }
 }
 else 
 {
 	printf("socket failed!: %s", strerror(errno));
 	goto err;
 }
 close(fd);
 	return 0;
err:
	if( fd >= 0 )
		 close(fd);
	return -1;
}

int ipcam_get_host_ip2(char * dev,unsigned char * host_ip,unsigned char * pMac)
{
	struct server_config_t source_config;
	struct in_addr temp;

	 if (read_interface(dev, &source_config.ifindex,&source_config.server, source_config.arp) < 0)
	 {	 	
		DPRINTK("get %s info error!\n", dev);
		return -1;
	 }		

	 memcpy(((u_int *) host_ip),&source_config.server ,sizeof(source_config.server));
	 memcpy(pMac,source_config.arp,6);

	return 1;
}

void net_get_dns(char * dns1,char * dns2)
{
	char cmd[350];
	FILE *fp=NULL;
//	long fileOffset = 0;
	char * pcmd = NULL;
	int i;
	int not_url = 0;
	int get_num = 0;

	dns1[0] = 0;
	dns2[0] = 0;

	fp = fopen("/etc/resolv.conf","r");
	if( fp == NULL )
	{
		printf(" open /etc/resolv.conf error!\n");
		goto G_ERROR;
	}

	while(1)
	{	
		pcmd = fgets(cmd,100,fp);

		if( pcmd == NULL )
		{
			DPRINTK("can't get lines\n");
			goto G_ERROR;
		}else
		{
			not_url = 0;

			for(i=0;i<100;i++)
			{
				if( (pcmd[i] == '\n') || (pcmd[i] == '\r') )
				{
					pcmd[i] = 0;
					break;
				}
			}
			
			
			if( (pcmd[0] != 'n') || (pcmd[1] != 'a') || (pcmd[2] != 'm')|| (pcmd[3] != 'e')  )
			{
					not_url = 1;
					DPRINTK("%s is not ip\n",pcmd);					
			}
			

			if( not_url == 0 )
			{
				if( get_num == 0 )
				{
					strcpy(dns1,&pcmd[strlen("nameserver")+1]);
					DPRINTK("dns1=%s\n",dns1);
					if( strcmp(dns1,"127.0.0.1" ) != 0 )
						get_num++;
				}else
				{
					strcpy(dns2,&pcmd[strlen("nameserver")+1]);
					DPRINTK("dns2=%s\n",dns2);
					goto G_ERROR;
				}
				
			}
		}
	}

	

	G_ERROR:

	if( fp )
		fclose(fp);

	return ;
}

#define NET_ROUTE_INFO_FILE  "/tmp/route.txt"
int GetNetGetWayAndMask(char * dev,unsigned char * pNetGate,unsigned char * pNetMask)
{
	int ret;
	char cmd[500];
	char tmp_buf[500];
	char arg[13][20] = {0};
	FILE * wifi_con = NULL;
	int arg_num = 0;
	unsigned int ip_value[4];
	int get_value = 0;

	sprintf(cmd,"route -n > %s",NET_ROUTE_INFO_FILE);
	
	ret = sys_command(cmd);
	if( ret < 0 )
		return -1;


	wifi_con = fopen(NET_ROUTE_INFO_FILE, "r");
	if(wifi_con == NULL)
	{
		DPRINTK("open route.txt error!\n");
		goto err;
	}

	while (fgets((char *)tmp_buf, 256, wifi_con) != NULL)
	{
		arg_num = sscanf (tmp_buf, "%s %s %s %s %s %s %s %s",arg[0], arg[1],arg[2], arg[3],arg[4], arg[5],arg[6], arg[7]);

		if(  arg_num != 8)	 	
			continue;		
	
		if(  strcmp(arg[7],dev) != 0 )
			continue;

		sscanf(arg[1],"%d.%d.%d.%d",&ip_value[0],&ip_value[1],&ip_value[2],&ip_value[3]);

		if( ip_value[0] !=  0 )
		{
			pNetGate[0] = ip_value[0];
			pNetGate[1] = ip_value[1];
			pNetGate[2] = ip_value[2];
			pNetGate[3] = ip_value[3];

			get_value = 1;
		}


		sscanf(arg[2],"%d.%d.%d.%d",&ip_value[0],&ip_value[1],&ip_value[2],&ip_value[3]);

		if( ip_value[0] !=  0 )
		{
			pNetMask[0] = ip_value[0];
			pNetMask[1] = ip_value[1];
			pNetMask[2] = ip_value[2];
			pNetMask[3] = ip_value[3];

			get_value = 1;
		}

	}


	if( get_value == 1 )
	{
		/*
		DPRINTK("Gateway:%d.%d.%d.%d   mask:%d.%d.%d.%d\n",\
			pNetGate[0],pNetGate[1],pNetGate[2],pNetGate[3],\
			pNetMask[0],pNetMask[1],pNetMask[2],pNetMask[3]);
			*/
	}else
		goto err;
	
	

ok:
	if( wifi_con )
		fclose(wifi_con);
	return 100;//ALLRIGHT;
err:
	if( wifi_con )
		fclose(wifi_con);

	DPRINTK("Get error!\n");
	
	return -1;//ERROR;
}

int XdvrGetNetCtrlInfo(char *gNetDevName, XDVR_NET_CTRL_ST * pNetCtrl)
{
	int ret;
	char strDns1[50] = "";
	char strDns2[50] = "";
	int value[4];

	//if( net_line_adapter_check_flag == 0 )
	//	goto err;

	pNetCtrl->uIp[0] = 0;
	pNetCtrl->uIp[1] = 0;
	pNetCtrl->uIp[2] = 0;
	pNetCtrl->uIp[3] = 0;

	pNetCtrl->uNetGate[0] = 0;
	pNetCtrl->uNetGate[1] = 0;
	pNetCtrl->uNetGate[2] = 0;
	pNetCtrl->uNetGate[3] = 0;

	pNetCtrl->uNetMask[0] = 0;
	pNetCtrl->uNetMask[1] = 0;
	pNetCtrl->uNetMask[2] = 0;
	pNetCtrl->uNetMask[3] = 0;

	pNetCtrl->uDns[0][0] = 0;
	pNetCtrl->uDns[0][1] = 0;
	pNetCtrl->uDns[0][2] = 0;
	pNetCtrl->uDns[0][3] = 0;
	
	pNetCtrl->uDns[1][0] = 0;
	pNetCtrl->uDns[1][1] = 0;
	pNetCtrl->uDns[1][2] = 0;
	pNetCtrl->uDns[1][3] = 0;

printf("teest:8-4\n\n");
	ipcam_get_host_ip2(gNetDevName,pNetCtrl->uIp,pNetCtrl->uMac);
printf("teest:8-5\n\n");
	GetNetGetWayAndMask(gNetDevName,pNetCtrl->uNetGate,pNetCtrl->uNetMask);
printf("teest:8-6\n\n");

	net_get_dns(strDns1,strDns2);
printf("teest:8-7\n\n");
	if( strDns1[0] != NULL )
	{
		sscanf(strDns1,"%d.%d.%d.%d",&value[0],&value[1],&value[2],&value[3]);
		pNetCtrl->uDns[0][0] = value[0];
		pNetCtrl->uDns[0][1] = value[1];
		pNetCtrl->uDns[0][2] = value[2];
		pNetCtrl->uDns[0][3] = value[3];
		
	}

	if( strDns2[0] != NULL )
	{		
		sscanf(strDns2,"%d.%d.%d.%d",&value[0],&value[1],&value[2],&value[3]);
		pNetCtrl->uDns[1][0] = value[0];
		pNetCtrl->uDns[1][1] = value[1];
		pNetCtrl->uDns[1][2] = value[2];
		pNetCtrl->uDns[1][3] = value[3];		
	}

	if (pNetCtrl->uIp[0] == 0 && pNetCtrl->uIp[0] == 0 && pNetCtrl->uIp[0] == 0 && pNetCtrl->uIp[0] == 0)
	{
		pNetCtrl->uNetGate[0] = 0;
		pNetCtrl->uNetGate[1] = 0;
		pNetCtrl->uNetGate[2] = 0;
		pNetCtrl->uNetGate[3] = 0;

		pNetCtrl->uNetMask[0] = 0;
		pNetCtrl->uNetMask[1] = 0;
		pNetCtrl->uNetMask[2] = 0;
		pNetCtrl->uNetMask[3] = 0;
	}

	
	DPRINTK("%d.%d.%d.%d\n",pNetCtrl->uIp[0],pNetCtrl->uIp[1],pNetCtrl->uIp[2],pNetCtrl->uIp[3]);
	DPRINTK("%d.%d.%d.%d\n",pNetCtrl->uNetMask[0],pNetCtrl->uNetMask[1],pNetCtrl->uNetMask[2],pNetCtrl->uNetMask[3]);
	DPRINTK("%d.%d.%d.%d\n",pNetCtrl->uNetGate[0],pNetCtrl->uNetGate[1],pNetCtrl->uNetGate[2],pNetCtrl->uNetGate[3]);
	DPRINTK("%d.%d.%d.%d\n",pNetCtrl->uDns[0][0],pNetCtrl->uDns[0][1],pNetCtrl->uDns[0][2],pNetCtrl->uDns[0][3]);
	DPRINTK("%d.%d.%d.%d\n",pNetCtrl->uDns[1][0],pNetCtrl->uDns[1][1],pNetCtrl->uDns[1][2],pNetCtrl->uDns[1][3]);
	DPRINTK("%0.2x:%0.2x:%0.2x:%0.2x:%0.2x:%0.2x\n",pNetCtrl->uMac[0],pNetCtrl->uMac[1],pNetCtrl->uMac[2],\
		pNetCtrl->uMac[3],pNetCtrl->uMac[4],pNetCtrl->uMac[5]);
		
ok:
	return 100;//ALLRIGHT;
err:
	return -1;//ERROR;
}



#ifdef _IOCTL_TEST
int main(int argv, char *argc[])
{
    unsigned char ip[16]={0};
    unsigned char netmsk[16] = {0};
    unsigned char mac[6] = {0};
    int num = 0, ret = 0;
    while(1)
    {
    	printf("***************option*******************\n");
    	printf("***************0.get ip*****************\n");
    	printf("***************1.get netmask************\n");
    	printf("***************2.get mac****************\n");
    	printf("***************3.get interface status***\n");
    	printf("***************4.get mtu size***********\n");
    	printf("***************5.set gateway************\n");
      printf("***************6.get gateway************\n");
      printf("***************7.set 192.168.17************\n");
      printf("***************8.set 172.20.120************\n");
      printf("***************10.set 172.20.120 err************\n");
    	printf("****************************************\n");
    	scanf("%d", &num);
    	switch(num)
    	{
    		case 0:
    			ret = Ioctl_get_ip(ip,"eth0");
    			if (ret == 1)
			    {
			      printf("ip:%s___\n", ip);
			      Ioctl_set_ip("192.168.19.11","eth0");
			    }
			    else
			      perror("get ip error!\n");
    			break;
    		case 1:
    			ret = Ioctl_get_ip_netmask(netmsk,"eth0");
    			if (ret == 1)
			    {
			      printf("netmsk:%s___\n", netmsk);
			      //set_ip_netmask("255.255.255.0");
			    }
			    else
			      perror("get netmsk error!\n");
    			break;
    		case 2:
    			ret = Ioctl_get_mac(mac,"eth0");
			    if (ret == 1)
			    {
			      printf("mac:%02x:%02x:%02x:%02x:%02x:%02x___\n", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
			      /*if(mac[5]==0x11)
			        mac[5]=0x22;
			      else
			        mac[5]=0x11;
			      set_mac(mac);*/
			      //del_gateway("0.0.0.0");//del default gw, first one.
			    }
			    else
			      perror("get mac error!\n");
    			break;
    		case 3:
    			ret = Ioctl_get_addr(ip, SIOCGIFFLAGS,"eth0");
    			if( ret == 0)
			        printf("active fault.\n");
			    else
			        printf("%s is working.\n", DEFAULT_ETH);
    			break;
    		case 4:
    			//set_network_mtu(&ret, SIOCSIFMTU);
    			ret = Ioctl_get_network_mtu(&ret, SIOCGIFMTU,"eth0");
    			break;
    	  case 5:
			      //set_gateway("192.168.22.22");
			      Ioctl_set_gateway("0.0.0.0");
    			break;
    	  case 6:
			      Ioctl_del_gateway("0.0.0.0");//del default gw, first one.
    			break;
        case 7:
			      Ioctl_set_br0_net("192.168.17.182","255.255.255.0","192.168.17.1");
    			break;
        case 8:
			      Ioctl_set_br0_net("172.20.120.169","255.255.240.0","172.20.120.1");
    			break;
        case 10:
			      Ioctl_set_br0_net("172.168.12.182","255.255.255.0","192.168.17.1");
    			break;
    		case 9:
    			return 0;
    			break;
    		default:
    			printf("error params!!\n");
    			break;
    	}
    	printf("ret = %d___\n", ret);
    }
    return 0;
}
#endif
