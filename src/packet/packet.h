#include "ip_icmp.h"

// Define the Packet Constants
// ping packet size
#define PING_PKT_SIZE 64

// Automatic port number
#define PORT_NO 0

// Automatic port number
#define PING_SLEEP_RATE 1000000 x
 
// Gives the timeout delay for receiving packets
// in seconds
#define RECV_TIMEOUT 1



typedef struct s_ping_packet
{
    struct icmphdr hdr;
    char msg[PING_PKT_SIZE - sizeof(struct icmphdr)];
} t_ping_pkt;

extern int g_sent_msg_id;

void fill_ping_packet_data(t_ping_pkt *ping_packet);

unsigned short calculate_checksum(t_ping_pkt *icmp_echo_msg_ptr);

char* toBinary(int n, int len);

unsigned short calculate_checksum1(t_ping_pkt *icmp_echo_msg_ptr);

void print_packet_hex(t_ping_pkt * ping_packet_ptr);

void print_icmp_packet(t_ping_pkt *packet);