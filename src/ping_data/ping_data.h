#ifndef PING_DATA_H
#define PING_DATA_H

typedef s_ping_data
{
    unsigned long address;
    char *host_name;
    int id;
    int packet_size;
} t_ping_data;


#endif