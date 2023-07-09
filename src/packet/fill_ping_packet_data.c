#include "packet.h"
#include "libft.h"

void fill_ping_packet_data(t_ping_pkt *ping_packet) {
    ft_bzero(ping_packet, sizeof(ping_packet));
}