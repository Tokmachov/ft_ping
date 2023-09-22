#include "args.h"
#include "stdio.h"
// dprintf

#include <unistd.h>
// STDERR_FILENO

#include <stdlib.h>
// exit

#include <arpa/inet.h>

#include "parse_ipv4.h"

void get_ping_data(int ac, char **av, t_ping_data *ping_data)
{
    if (ac < 2) {
        dprintf(STDERR_FILENO, "ping: missing host operand\n");
        dprintf(STDERR_FILENO, "Try 'ping --help' or 'ping --usage' for more information.\n");
        exit(130);
    }
    char ipv4_str[IPV4_MAX_STR_LEN];
    int result = parse_ipv4_str(ipv4_str, av[1]);
    if (result == SUCCESS)
    {
        printf("Formatted ip string: %s\n", ipv4_str);
    } 
    else
    {
        printf("Failure\n");
    }
}