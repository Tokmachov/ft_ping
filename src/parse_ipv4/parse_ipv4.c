#include "libft.h"
#include "parse_ipv4.h"

#include <stdlib.h>
// free
#define IPV4_DELIMITER '.'

#define IPV4_NUM_MAX 4294967295

#define OCTET_NUM_MAX 255

#define IPV4_OCTET_COUNT 4

#define IPV4_STR_LEN 16

static void num_to_octets_arr(unsigned char octets_arr[], unsigned long ipv4_num)
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

static int str_num_to_ipv4_octets(unsigned char ipv4_octet_arr[], char *str)
{
    unsigned long result; 
    int is_success = ft_atoi_unsigned_long_safe(&result, str);
    
    if (!is_success || result > IPV4_NUM_MAX)
        return FAILURE;

    num_to_octets_arr(ipv4_octet_arr, result);
        return SUCCESS;
}

static void add_padding(char **str_arr_with_padding, char **str_arr)
{
    int octets_count = ptr_arr_len(str_arr);
    if (octets_count == 2)
    {
       str_arr_with_padding[0] = str_arr[0];
       str_arr_with_padding[1] = 0;
       str_arr_with_padding[2] = 0;
       str_arr_with_padding[3] = str_arr[1];
    } 
    else if (octets_count == 3)
    {
        str_arr_with_padding[0] = str_arr[0];
        str_arr_with_padding[1] = str_arr[1];
        str_arr_with_padding[2] = 0;
        str_arr_with_padding[3] = str_arr[2];
        
    }
    else if (octets_count == 4)
    {
        str_arr_with_padding[0] = str_arr[0];
        str_arr_with_padding[1] = str_arr[1];
        str_arr_with_padding[2] = str_arr[2];
        str_arr_with_padding[3] = str_arr[3];
    }
}

static int str_arr_to_ipv4_octets(unsigned char ipv4_octet_arr[], char **str_arr)
{
    int str_count = ptr_arr_len(str_arr);
    char *str_arr_with_padding[IPV4_OCTET_COUNT];
    
    if (str_count > IPV4_OCTET_COUNT || str_count < 1)
        return FAILURE;
    
    add_padding(str_arr_with_padding, str_arr);
    
    int i = 0;
    while (i < IPV4_OCTET_COUNT)
    {
        if (str_arr_with_padding[i])
        {
            unsigned char octet;
            if (ft_atoi_unsigned_char_safe(&octet, str_arr_with_padding[i]))
                ipv4_octet_arr[i] = octet;
            else
                return FAILURE;
        }
        else
            ipv4_octet_arr[i] = 0;
        i++;
    }
    return SUCCESS;
}

int parse_ipv4_octets(unsigned char octets[], char *src_str)
{
    ft_bzero(octets, IPV4_OCTET_COUNT);
    
    if (is_started_or_trailed_with_delimiter(src_str, IPV4_DELIMITER))
    {
        return FAILURE;
    }
    
    if (!has_repeated_delimeter(src_str, IPV4_DELIMITER))
    {
        return FAILURE;
    }
    
    char **str_arr = ft_split(src_str, IPV4_DELIMITER);
    size_t len = ptr_arr_len(str_arr);
    int result;
    
    if (len == 1)
        result = str_num_to_ipv4_octets(octets, str_arr[0]);
    else
        result = str_arr_to_ipv4_octets(octets, str_arr);
    ft_free_str_arr(str_arr);
    return result;
}

int parse_ipv4_str(char *ipv4_str, char *src)
{
    unsigned char octets[IPV4_OCTET_COUNT];
    char *ipv4_str_tmp = ipv4_str;

    if (parse_ipv4_octets(octets, src) == FAILURE) 
        return FAILURE;
    
    ft_bzero(ipv4_str, IPV4_STR_LEN);
    int i = 0;
    while (i < IPV4_OCTET_COUNT)
    {
        char *octet = ft_itoa(octets[i]);
   
        size_t octet_len = ft_strlen(octet);
        
        ft_memcpy(ipv4_str_tmp, octet, octet_len);
       
        if (i < 3)
        {
            ipv4_str_tmp[octet_len] = '.'; 
            octet_len += 1;
        }
        
        ipv4_str_tmp += octet_len;
        free(octet);
        i++;
    }
    return SUCCESS;
}