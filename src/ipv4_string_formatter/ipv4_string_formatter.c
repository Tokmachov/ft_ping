#include "libft.h"
#include "ipv4_string_formatter.h"

#include <stdio.h>
// remove for debugging

#include <stdlib.h>
// free
#define IPV4_DELIMITER '.'

#define IPV4_NUM_MAX 4294967295

#define OCTET_NUM_MAX 255

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

static void ipv4_num_to_ipv4_str(char *ipv4_str, long ipv4_num)
{
    int shift = 3 * 8;
    char * next_octet_start_ptr = ipv4_str;
    
    if (ipv4_num < 0 || ipv4_num > IPV4_NUM_MAX) {
        return;
    }
    
    while (shift >= 0)
    {
        unsigned char octet_num = (ipv4_num >> shift) & 0Xff;

        
        char *octet_str = ft_itoa(octet_num);
        size_t octet_str_len = ft_strlen(octet_str);
        ft_memcpy(next_octet_start_ptr, octet_str, octet_str_len);
        free(octet_str);
        
        if (shift != 0)
        {
            next_octet_start_ptr[octet_str_len] = '.';
            next_octet_start_ptr += octet_str_len + 1;
        }
        shift -= 8;
    }
}

static int is_valid_octet_num(char *octet_num)
{
    long result = 0;
	
	while (*octet_num && *octet_num >= '0' && *octet_num <= '9' && result <= OCTET_NUM_MAX)
	{
		result = (result * 10) + (*octet_num - '0');
        octet_num++;
	}
    if (*octet_num != 0 || result > OCTET_NUM_MAX)
    {
        return FALSE;
    }
	return TRUE;
} 

static int octets_valid(char **octets)
{
    while(*octets)
    {
        if (!is_valid_octet_num(*octets))
        {
            printf("Here\n");
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

int format_as_ipv4(char *dst, char *src) 
{
    ft_bzero(dst, IPV4_MAX_STR_LEN);
    
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
    if (octet_count == 1)
    {
        long num = str_to_ipv4_num(octets[0]);
        if (num != -1)
        {
            ipv4_num_to_ipv4_str(dst, num);
            ft_free_str_arr(octets);
            return SUCCESS;
        }
        else
        {
            ft_free_str_arr(octets);
            return FAILURE;
        }
    }
    else if ((octet_count == 2 || octet_count == 3) && octets_valid(octets))
    {
        octets_to_ipv4_str(dst, octets, octet_count);
        ft_free_str_arr(octets);
        return SUCCESS;
    }
    else if (octet_count == 4 && octets_valid(octets))
    {
        ft_memcpy(dst, src, ft_strlen(src));
        ft_free_str_arr(octets);
        return SUCCESS;
    }
    else
    {
        ft_free_str_arr(octets);
        return FAILURE;
    }
}