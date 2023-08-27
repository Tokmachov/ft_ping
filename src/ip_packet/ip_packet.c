#include "libft.h"
#define IP_PACKET_SIZE  65535

char *init_ip_packet_buff() 
{
    void *buff = ft_malloc_or_exit_error(IP_PACKET_SIZE);
    ft_bzero(buff, IP_PACKET_SIZE);
    return buff;
}

char get_ip_header_length_bytes(char p[])
{
    char length_in_4_bite_words = p[0];
    length_in_4_bite_words = length_in_4_bite_words & 0XF;
    return length_in_4_bite_words * 4;
}
