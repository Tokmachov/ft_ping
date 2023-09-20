#include "libft.h"
#include "parse_ipv4.h"

#include <stdio.h>
// remove for debugging

#include <stdlib.h>
// free
#define IPV4_DELIMITER '.'

#define IPV4_NUM_MAX 4294967295

#define OCTET_NUM_MAX 255

#define IPV4_OCTET_COUNT 4

static long str_to_ipv4_num(char *str)
{
    long result = 0;
	
	while (*str && *str >= '0' && *str <= '9' && result <= IPV4_NUM_MAX)
	{
		result = (result * 10) + (*str - '0');
        str++;
	}
    if (*str != 0 || result > IPV4_NUM_MAX)
    {
        return -1;
    }
	return result;
}

static void ipv4_num_to_octets_arr(unsigned char octets_arr[], unsigned long ipv4_num)
{
    int shift = 3 * 8;
    int index = 0;
    
    while (shift >= 0)
    {
        unsigned char octet_num = (ipv4_num >> shift) & 0Xff;

        octets_arr[index] = octet_num;
        shift -= 8;
        index++;
    }
}

static int is_valid_octet(char *octet)
{
    long result = 0;
	
	while (*octet && *octet >= '0' && *octet <= '9' && result <= OCTET_NUM_MAX)
	{
		result = (result * 10) + (*octet - '0');
        octet++;
	}
    if (*octet != 0 || result > OCTET_NUM_MAX)
    {
        return FALSE;
    }
	return TRUE;
} 

static int is_valid_octets(char **octets)
{
    while(*octets)
    {
        if (!is_valid_octet(*octets))
        {
            return FALSE;
        }
        octets++;
    }
    return TRUE;
}

static void octets_to_ipv4_str(char *dst, char **octets, int octets_count)
{   
    size_t offset = 0;
    if (octets_count == 2)
    {
        char *padding = ".0.0.";
        size_t padding_len = ft_strlen(padding);
        size_t first_octet_len = ft_strlen(octets[0]);
        size_t last_octet_len = ft_strlen(octets[1]);
        
        ft_memcpy(dst, octets[0], first_octet_len);
        
        offset += first_octet_len;
        ft_memcpy(dst + offset, padding, padding_len);
        
        offset += padding_len;
        ft_memcpy(dst + offset, octets[1], last_octet_len);
    }
    else
    {
        char *padding = ".0.";
        size_t padding_len = ft_strlen(padding);
        size_t first_octet_len = ft_strlen(octets[0]);
        size_t second_octet_len = ft_strlen(octets[1]);
        size_t fourth_octet_len = ft_strlen(octets[2]);
        
        ft_memcpy(dst, octets[0], first_octet_len);
        dst[first_octet_len] = '.';
        
        offset += first_octet_len + 1;
        ft_memcpy(dst + offset, octets[1], second_octet_len);
        
        offset += second_octet_len;
        ft_memcpy(dst + offset, padding, padding_len);
        
        offset += padding_len;
        ft_memcpy(dst + offset, octets[2], fourth_octet_len);
    }
}


//finished here
static int str_num_to_ipv4_octet_arr(unsigned char ipv4_octet_arr[], char *str)
{
    unsigned long result; 
    int is_failed = ft_atoi_unsigned_long_safe(&result, str);
    
    if (is_failed || result > IPV4_NUM_MAX)
        return FAILURE;

    ipv4_num_to_octets_arr(ipv4_octet_arr, result);
        return SUCCESS;
}

int parse_ipv4(unsigned char dst[], char *src) 
{
    ft_bzero(dst, IPV4_OCTET_COUNT);
    
    if (is_started_or_trailed_with_delimiter(src, IPV4_DELIMITER))
    {
        return FAILURE;
    }
    
    if (!has_repeated_delimeter(src, IPV4_DELIMITER))
    {
        return FAILURE;
    }
    
    char **octets = ft_split(src, IPV4_DELIMITER);
    int octet_count = ptr_arr_len(octets);
    
    int result = -1;
    if (octet_count == 1)
    {
        result = str_num_to_ipv4_str(dst, octets[0]);
    }
    else if ((octet_count == 2 || octet_count == 3) && is_valid_octets(octets))
    {
        octets_to_ipv4_str(dst, octets, octet_count);
        result = SUCCESS;
    }
    else if (octet_count == 4 && is_valid_octets(octets))
    {
        ft_memcpy(dst, src, ft_strlen(src));
        result = SUCCESS;
    }
    else
    {
        result = FAILURE;
    }
    ft_free_str_arr(octets);
    return result;
}