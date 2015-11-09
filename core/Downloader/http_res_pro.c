/*
 *
 *
 *
 *
 *
 *
 *
 */



#include "http_res_pro.h"

#include <string.h>

#define CRLF "\r\n"


int get_status_code(char *status_code, char *data, size_t len){
    char    *p, *q;
    p = strstr(data, CRLF);
    q = data;
    int f = 0, index[2] = {0}, idx = 0;
    while(f < 2)
    {
        idx++;
        if ((*q) == ' ')
        {
            index[f] = idx;
            f++;
        }
    }

    memcpy(status_code, data + index[0],index[1] - index[0]);
    return 0;
}
