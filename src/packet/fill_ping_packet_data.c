#include "packet.h"
#include "libft.h"
#include <unistd.h>
//for getpid()
#include <stdio.h>

// static unsigned short calculate_checksum(t_ping_pkt *icmp_echo_msg_ptr);
int g_sent_msg_id;

void fill_ping_packet_data(t_ping_pkt *ping_packet) {
    ft_bzero(ping_packet, sizeof(ping_packet));
    ping_packet->hdr.type = ICMP_ECHO;
    ping_packet->hdr.code = 0;
    ping_packet->hdr.checksum = 0;
    ping_packet->hdr.id = htons(getpid()); //identifier can be whatever 16 bit number. It is used by process to distinguish its own packets
    printf("Packet id/pid: %d\n", ping_packet->hdr.id);
    ping_packet->hdr.sequence = htons(++g_sent_msg_id);
    size_t i = 0;
    while(i < sizeof(ping_packet->msg) - 1) { //go over all indices except last one.
        (ping_packet->msg)[i] = i + '0'; //-> and [] have same priority but evalueated from left to right
        i++;
    }
    (ping_packet->msg)[i] = 0;
    ping_packet->hdr.checksum = calculate_checksum(ping_packet);
}

// (1)  Adjacent octets to be checksummed are paired to form 16-bit
//         integers, and the 1's complement sum of these 16-bit integers is
//         formed.

// (2)  To generate a checksum, the checksum field itself is cleared,
//     the 16-bit 1's complement sum is computed over the octets
//     concerned, and the 1's complement of this sum is placed in the
//     checksum field.

// (3)  To check a checksum, the 1's complement sum is computed over the
//     same set of octets, including the checksum field.  If the result
//     is all 1 bits (-0 in 1's complement arithmetic), the check
//     succeeds.

// echo request
// 0                   1                   2                   3
//     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |     Type      |     Code      |          Checksum             |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |           Identifier          |        Sequence Number        |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |     Data ...
//    +-+-+-+-+-
//type - 8 bit
//code - 8 bit
//checksum (in request it is 0, length 16 bit)
//identifier (16 bit)
//sequnece numm 16 bit
//data - arbitrary length
//36 types
unsigned short calculate_checksum(t_ping_pkt *icmp_echo_msg_ptr)
{   
    unsigned short *u_short_icmp_echo_msg_ptr = (unsigned short*)icmp_echo_msg_ptr; //short is often 16 bits long
    unsigned int sum = 0;
    unsigned short checksum;
    sum += *u_short_icmp_echo_msg_ptr; //type + code
    u_short_icmp_echo_msg_ptr++; //move to checksum
    sum += *u_short_icmp_echo_msg_ptr; //add checksum, which is zero in echo request
    u_short_icmp_echo_msg_ptr++; //move to identifier
    sum += *u_short_icmp_echo_msg_ptr; //add identifier
    u_short_icmp_echo_msg_ptr++; //move to sequence number
    sum += *u_short_icmp_echo_msg_ptr; //add sequence number
    //then add message data which might be of arbitrary length
    int msg_len = sizeof((icmp_echo_msg_ptr)->msg);
    u_short_icmp_echo_msg_ptr++; //move ptr to data part
    while (msg_len > 1) {
        sum += *u_short_icmp_echo_msg_ptr++; //first ++ retrurn not increased ptr, then dereferences ptr, then increases ptr
        msg_len -= 2; //decrease counter by 16 bits
    }
    //if message length was odd, then we have one more byte, so we need to add last bite + zeroed byte
    if (msg_len > 0) {
        sum += * (unsigned char *) u_short_icmp_echo_msg_ptr;
    }
    //at this point we might overflow short. So we need to "fold" overflowed part
    while (sum >> 16) { //here we move overflowed bytes to least significat position
        sum = (sum & 0xffff)  //take only 16 bit least significat part without overflow and zero/erase overflowed part
        + (sum >> 16); //move overflowed part to list significant bit positon
    }
    checksum = ~sum; //here we covert sum to one's complement. i.e. we covert all zeros to ones and ones to zeros
    return checksum;
}

#include <stdio.h>
#include <stdlib.h>
 
char* toBinary(int n, int len)
{
    char* binary = (char*)malloc(sizeof(char) * len);
    int k = 0;
    for (unsigned i = (1 << (len - 1)); i > 0; i = i / 2) {
        binary[k++] = (n & i) ? '1' : '0';
    }
    binary[k] = '\0';
    return binary;
}

unsigned short calculate_checksum1(t_ping_pkt *icmp_echo_msg_ptr)
{   
    unsigned short *u_short_icmp_echo_msg_ptr = (unsigned short*)icmp_echo_msg_ptr; //short is often 16 bits long
    unsigned int sum = 0;
    unsigned short checksum;
    sum += *u_short_icmp_echo_msg_ptr; //type + code
    u_short_icmp_echo_msg_ptr++; //move to checksum
    sum += *u_short_icmp_echo_msg_ptr; //add checksum, which is zero in echo request
    u_short_icmp_echo_msg_ptr++; //move to identifier
    sum += *u_short_icmp_echo_msg_ptr; //add identifier
    u_short_icmp_echo_msg_ptr++; //move to sequence number
    sum += *u_short_icmp_echo_msg_ptr; //add sequence number
    //then add message data which might be of arbitrary length
    int msg_len = sizeof(((t_ping_pkt*)icmp_echo_msg_ptr)->msg);
    u_short_icmp_echo_msg_ptr++; //move ptr to data part
    while (msg_len > 1) {
        sum += *u_short_icmp_echo_msg_ptr++; //first ++ retrurn not increased ptr, then dereferences ptr, then increases ptr
        msg_len -= 2; //decrease counter by 16 bits
    }
    //if message length was odd, then we have one more byte, so we need to add last bite + zeroed byte
    if (msg_len > 0) {
        sum += * (unsigned char *) u_short_icmp_echo_msg_ptr;
    }
    //at this point we might overflow short. So we need to "fold" overflowed part
    while (sum >> 16) { //here we move overflowed bytes to least significat position
        sum = (sum & 0xffff)  //take only 16 bit least significat part without overflow and zero/erase overflowed part
        + (sum >> 16); //move overflowed part to list significant bit positon
    }
    checksum = ~sum; //here we covert sum to one's complement. i.e. we covert all zeros to ones and ones to zeros
    return (unsigned short)sum;
}

void print_packet_hex(t_ping_pkt * ping_packet_ptr) {
    unsigned char *ping_packet_byte_ptr = (unsigned char*)ping_packet_ptr;
    size_t len = sizeof(t_ping_pkt);
	while (len > 0) {
		unsigned char byte = *ping_packet_byte_ptr++;
		printf("%X ", byte);
		if (len % 16 == 0) {
			printf("\n");
		}
		len--;
	}
    printf("\n");
}

void print_icmp_packet(t_ping_pkt *packet) {
    printf("Icmp packet: \n");
    printf("type: %d\n", packet->hdr.type);
    printf("code: %d\n", packet->hdr.code);
    printf("Identifier: %d\n", ntohs(packet->hdr.id));
    printf("Sequence number: %d\n", ntohs(packet->hdr.sequence));
}
