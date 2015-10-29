/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>

#include <uv.h>
#include <uv-linux.h>
#include <uv-errno.h>

#include "request.h"
#include "http_res_pro.h"
    
void on_getaddrinfo_end(uv_getaddrinfo_t *req, int status, struct addrinfo *res);
void on_connect_end(uv_connect_t *req, int status);
void on_write_end(uv_write_t *req, int status);
void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);

int start_all(uv_loop_t *, const char *, const char *);


int lhttp_client_init(lhttp_client_t *lhttp_client){
    return 0;
}


int lhttp_client_run(lhttp_client_t *lhttp_client, uv_timer_cb on_time){
    uv_timer_t timer_req;
    uv_timer_init(uv_default_loop(), &timer_req);
    uv_timer_start(&timer_req, on_time, 5000, 2000);
    int r = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    printf("RUNRUNRUNRUN%d\n", r);
    return r;
}


int lhttp_client_get(lhttp_client_t *lhttp_client, const char *url){
    //at first, get the host name from url
    //......
    //
    return start_all(uv_default_loop(), url, HTTP_PORT);
}




/**********************************************************************************/
//
//
//

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf){
    *buf = uv_buf_init((char *)malloc(suggested_size), suggested_size);
}

//start the http get/post/...
//the first step dns
//the dns callback function is on_getaddrinfo
int start_all(uv_loop_t *loop, const char *host, const char *port){
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;

    uv_getaddrinfo_t *getaddrinfo_req = (uv_getaddrinfo_t *)malloc(sizeof(uv_getaddrinfo_t));
    fprintf(stderr, "%s is...", host);

    int r = uv_getaddrinfo(loop, getaddrinfo_req, on_getaddrinfo_end, host, port, &hints);

    if(r)
    {
        fprintf(stderr, "uv_getaddrinfo call error %s\n", uv_err_name(r));
        return 1;
    }
    return 0;
}


//the 2nd step: connect to the server
//the connect callback function is on_connect
void on_getaddrinfo_end(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
    if(status < 0)
    {
        fprintf(stderr, "uv_getaddrinfo callback error %s\n", uv_err_name(status));
        return;
    }

    char addr[17] = {'\0'};
    uv_ip4_name((struct sockaddr_in *)res->ai_addr, addr, 16);
    fprintf(stderr, "%s\n", addr);

    uv_connect_t *connect_req = (uv_connect_t *)malloc(sizeof(uv_connect_t));
    uv_tcp_t *socket = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(req->loop, socket);
    //uv_tcp_keepalive(socket, 1, 60);

    int r = uv_tcp_connect(connect_req, socket, (struct sockaddr *)res->ai_addr, on_connect_end);
    fprintf(stderr, "connect ....\n");

    if(r)
    {
        fprintf(stderr, "uv_connect call error %s\n", uv_err_name(r));
        return;
    }
    uv_freeaddrinfo(res);
    free(req);

}


//the 3rd step: send somethinf to the server
//the write callback function is on_write
void on_connect_end(uv_connect_t *req, int status)
{
    if(status < 0)
    {
        fprintf(stderr, "uv_connect callback error %s\n", uv_err_name(status));
        return;
    }

    fprintf(stderr, "onconnect onconnect\n");
    
    char *base = "GET / HTTP/1.1\r\nHost: www.qq.com\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nAccept: text/html\r\nUpgrade-Insecure-Requests: 1\r\nAccept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.6,ja;q=0.4\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.80 Safari/537.36\r\nReferer: https://www.baidu.com/link?url=4jZdpA3I10NSqcywskm4nh5J7KQqPbx6TC52-mYGJSC&wd=&eqid=e194d8ab000486bf00000002562b3c71\r\nAccept-Encoding: gzip, deflate, sdch\r\nAccept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.6,ja;q=0.4\r\nCookie: pgv_info=ssid=s9417603086; ts_last=www.qq.com/; ts_refer=www.baidu.com/link; pgv_pvid=8923037070; ts_uid=5774452315; ad_play_index=66\r\n\r\n";
    size_t len = strlen(base);
    uv_buf_t a = {.base = base, .len = len};
    uv_write_t *write_req = (uv_write_t *)malloc(sizeof(uv_write_t));
    int r = uv_write(write_req, req->handle, &a, 1, on_write_end);

    if(r)
    {
        fprintf(stderr, "uv_write call error %s\n", uv_err_name(r));
        return;
    }

    free(req);
    
}


//the 4th step: read something from the socket
//the readcallback function is on_read
void on_write_end(uv_write_t *req, int status){
   if(status < 0)
   {
        fprintf(stderr, "uv_write callback error %s\n", uv_err_name(status));
        return;
   }

   fprintf(stderr, "write write\n");
   int r = uv_read_start(req->handle, alloc_buffer, on_read);

   if(r)
   {
       fprintf(stderr, "uv_read_start call error %s\n", uv_err_name(r));
       return;
   }

   free(req);

}


//the 5th step: read data
//Tipe:The uv_read_cb callback will be made several times until there is no more data to read or uv_read_stop() is called
void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf){
    if(nread == UV_EOF)
    {
        fprintf(stderr, "EOF\n");
        uv_read_stop(stream);
        return;
    }
    if(nread < 0)
    {
        fprintf(stderr, "uv_read callback error \n");
        return;
    }

   fprintf(stderr, "onread onread\n");
   fprintf(stderr, "%s\n", buf->base);
   
   //char *status_code = (char *)malloc(4);
   //memset(status_code, 0, 4);
   //get_status_code(status_code, buf->base, buf->len);

   //printf("status_code: %s\n", status_code);

}
