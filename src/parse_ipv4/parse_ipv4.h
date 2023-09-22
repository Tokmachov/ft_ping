#ifndef IPV4_STRING_FORMATTER_H
#define IPV4_STRING_FORMATTER_H

#define IPV4_MAX_STR_LEN 16

#define SUCCESS 0
#define FAILURE -1

int parse_ipv4_octets(unsigned char dst[], char *src);
int parse_ipv4_str(char *ipv4_str, char *src);
#endif