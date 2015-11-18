/*
 *
 *
 *
 *
 *
 *
 *
 */



#include "http_res_parser.h"

#include <string.h>

#define CRLF "\r\n"

/* http response head 的格式如下：
 *
 * HTTP/1.1 200 OK\r\n
 * Server: bfe/1.0.8.9\r\n
 * Date: Mon, 09 Nov 2015 14:05:38 GMT\r\n
 * Content-Type: text/html;charset=utf-8\r\n
 * Transfer-Encoding: chunked\r\n
 * Connection: keep-alive\r\n
 * Cache-Control: private\r\n
 * Expires: Mon, 09 Nov 2015 14:05:38 GMT\r\n
 * Content-Encoding: gzip\r\n
 * X-UA-Compatible: IE=Edge,chrome=1\r\n
 * BDPAGETYPE: 2\r\n
 * BDQID: 0xf94de66f0002b69e\r\n
 * BDUSERID: 327861481\r\n
 * Set-Cookie: BDSVRTM=215; path=/\r\n
 * Set-Cookie: BD_HOME=1; path=/\r\n
 * Set-Cookie: H_PS_PSSID=17518_17746_1445_17619_17900_17948_17783_17502_17836_17001_17072_15121_11892_17422; path=/; domain=.baidu.com\r\n
 * Set-Cookie: __bsi=16735578152215325157_00_1_R_N_220_0303_C02F_N_I_I_0; expires=Mon, 09-Nov-15 14:05:43 GMT; domain=www.baidu.com; path=/\r\n\r\n
*/

/* 由于缓冲区大小的原因，可能res中不包含完整的http头，这里认为读到“\r\n\r\n”表示整个http头已经读完
 */

int  http_res_parser(const char *res, response_t *res_parser, int *body_off)
{
    int res_len = strlen(res);
    /* Status line */
    const char *p = strstr(res, CRLF);
    if(p != NULL)
    {
        //strncpy(res_parse)
    }

    return 0;
}
