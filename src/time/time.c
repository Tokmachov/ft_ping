#include <sys/time.h>

float delta_in_ms(struct timeval *start, struct timeval *end) 
{
    float seconds = end->tv_sec - start->tv_sec;
    float microseconds = end->tv_usec - start->tv_usec;

    return seconds * 1000 + microseconds / 1000;
}