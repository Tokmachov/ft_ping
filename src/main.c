#include <stdio.h>
//printf

#include <sys/socket.h>
//socket
//1st arg to socket. constant AF_INET

#include <netdb.h>

#include <sys/time.h>
//for struct timeval tv_out

#include <stdlib.h>
//for exit
#define TRUE 1;
#define FALSE 0;


// Gives the timeout delay for receiving packets
// in seconds
#define RECV_TIMEOUT 1

#include "packet.h"

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

int main(int ac, char **av)
{	
	ac = (int) ac;
	av = (char**)av;
	
	//int should_ping = TRUE;
	int icmp_num = get_icmp_ptotocol_num();
	
	//to open raw socket. 
	//1st arg - domain argument. protocol family which will be used for communication
	//AF_INET - IPv4 Internet protocols
	//2nd arg - communication type
	//SOCK_RAW - provides raw network access
	//raw socket can be opened only under sudo. So program should be launched from sudo user
	//3arg - protocol
	int socket_fd = socket(AF_INET, SOCK_RAW, icmp_num);
	printf("socket fd num is %d\n", socket_fd);
	
	// setting timeout of recv setting
    struct timeval tv_out;
    tv_out.tv_sec = RECV_TIMEOUT; //seconds
    tv_out.tv_usec = 0; //microseconds
	//how does this option influence the program, please test it
	int setsockopt_result = setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);
	if (setsockopt_result != 0) {
		//we can read errno and print it. Also maybe I should allow this print only in verbose mode
		printf("Error occured when setting timeout of receiving packets on socket");
		exit(-1);
	} else {
		printf("Set socket receive timeout option succesfully\n");
	}
	
	t_ping_pkt ping_packet;

	fill_ping_packet_data(&ping_packet);

	return 0;
}
