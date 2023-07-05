#include <icmp.h>

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

typedef struct
{
    struct icmphdr hdr;
    char msg[PING_PKT_SIZE - sizeof(struct icmphdr)];
} s_ping_pkt;