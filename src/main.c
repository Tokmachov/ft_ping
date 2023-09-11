#include <unistd.h> //for usleep

#include <stdio.h>
//printf
#include "libft.h"
#include "ip_packet.h"
#include <sys/socket.h>
//socket
//1st arg to socket. constant AF_INET
#include "time.h" //where i define all time related stuff: like time difference and include of gerrimeofaday

#include <netdb.h>


//for struct timeval tv_out

#include <stdlib.h>
//for exit

#include <sys/time.h>

#include "packet.h"

#include <arpa/inet.h>
//for inet_aton 
#include <netinet/in.h>

#include "ip_packet.h"

#define IP_PACKET_SIZE  65535 //move to ip_packet.h

#include <errno.h> 

//static is a function restricted to one file. Functions are global by default, so by defautl they can be
// used in all files
static int get_icmp_ptotocol_num() {
	//getprotobyname looksup protocol number by name in system file /etc/protocols and returns struct
	//representing entry in file protocols
	// struct protoent {
    //            char  *p_name;       /* official protocol name */
    //            char **p_aliases;    /* alias list */
    //            int    p_proto;      /* protocol number */
    //        }
	//ptotocol number is used in 3rd parameter of int socket(int domain, int type, int protocol);
	struct protoent *protocol = getprotobyname("icmp");
	return protocol->p_proto;
}

void inc_first(int arr[]) { 
    (arr[0])++; 
}
extern int g_sent_msg_id;
int main(int ac, char **av)
{	
	
	//to open raw socket. 
	//1st arg - domain argument. protocol family which will be used for communication
	//AF_INET - IPv4 Internet protocols
	//2nd arg - communication type
	//SOCK_RAW - provides raw network access
	//raw socket can be opened only under sudo. So program should be launched from sudo user
	//3arg - protocol
	int socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	printf("socket fd num is %d\n", socket_fd);
	
	// setting timeout of recv setting
    struct timeval tv_out;
    tv_out.tv_sec = RECV_TIMEOUT; //seconds
    tv_out.tv_usec = 0; //microseconds
	//how does this option influence the program, please test it
	int setsockopt_result = setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);
	if (setsockopt_result != 0) {
		//we can read errno and print it. Also maybe I should allow this print only in verbose mode
		printf("Error occured when setting timeout of receiving packets on socket\n");
		//exit(-1);
	} else {
		printf("Set socket receive timeout option succesfully\n");
	}

	//print_packet_hex(&ping_packet);
	//printf("Sent n bytes: %d\n", result);
	
	t_ping_pkt ping_packet;
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	int result = inet_aton("8.8.8.8", &(addr.sin_addr));
	if (result == 0) {
		printf("Address was invalid");
	}
	socklen_t addr_len;

	char *ip_packet = init_ip_packet_buff();

	while (TRUE) {
		usleep(1000000);
		ft_bzero(&ping_packet, sizeof(ping_packet));
		// fill_ping_packet_data(&ping_packet);
		
		// struct timeval start_time;
		// struct timeval end_time;
		// int r = gettimeofday(&start_time, NULL);
		
		// // ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
		// // Upon successful completion, sendto() shall return the number of bytes sent. Otherwise, -1 shall be returned and errno set to indicate the error.
		// // result = sendto(socket_fd, &ping_packet, sizeof(ping_packet), 0, (struct sockaddr *)&addr, sizeof(addr));
		// // printf("Sent n bytes: %d\n", result);
		ft_bzero(ip_packet, IP_PACKET_SIZE);
		
		struct iovec iovec[1];
		
		iovec[0].iov_base = ip_packet;
		iovec[0].iov_len = IP_PACKET_SIZE;

		struct msghdr msghdr;

		msghdr.msg_iov = iovec;
		msghdr.msg_iovlen = 1;
		struct sockaddr_in addr;
    	socklen_t addr_len = sizeof(addr);		
		msghdr.msg_name = &addr;
		msghdr.msg_namelen = addr_len;
		int r = recvmsg(socket_fd, &msghdr, 0);
		printf("Received %d\n", r);
		char addr_str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, (&addr.sin_addr), addr_str, INET_ADDRSTRLEN);
		printf("Received from: %s\n", addr_str);
		char ip_header_length = get_ip_header_length_bytes(msghdr.msg_iov[0].iov_base);
		printf("Ip header %d\n", ip_header_length);
		t_ping_pkt *received_ping_packet = (t_ping_pkt *)(ip_packet + ip_header_length);
		print_packet_hex(received_ping_packet);
		//check that packet is:
			// - reply pakcet
			// - packet that my process sent
			// - packet that has sequence number i just sent
			// if not, then i should discard packet and try to receive packet again
			// if so, then i should print print status of packet
			print_icmp_packet(received_ping_packet);
			
		// 	if (received_ping_packet->hdr.type == ICMP_ECHOREPLY && 
		// 		received_ping_packet->hdr.code == 0 && 
		// 		received_ping_packet->hdr.id == getpid() &&
		// 		received_ping_packet->hdr.sequence == g_sent_msg_id) 
		// 	{
		// 		r = gettimeofday(&end_time, NULL);
		// 		float delta = delta_in_ms(&start_time, &end_time);
		// 		printf("Ping time: %.3f\n", delta);
		// 	} else {
		// 		printf("Received wrong packet\n");
		// 	}
		// }
		
	}
	
	return 0;
}
