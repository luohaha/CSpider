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
#include <http_parser.h>
#include "http_res_pro.h"


int dns(request_t *request, const char *, const char *);   
void http_connect(uv_getaddrinfo_t *req, int status, struct addrinfo *res);
void http_write(uv_connect_t *req, int status);
void http_read_start(uv_write_t *req, int status);
void http_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);

int parse_url(const char *url, char *host, char *path, uint16_t *port);



headers_t *default_headers()
{
    headers_t   *headers = (headers_t *)malloc(sizeof(headers_t));
    strncpy(headers->headers[0][0], CONNECTION, strlen(CONNECTION));

    strncpy(headers->headers[0][1], CONNECTION_DEFAULT, strlen(CONNECTION_DEFAULT));

    headers->number_headers = 1;

    return headers;
}


cookies_t *default_cookies()
{
    cookies_t   *cookies = (cookies_t *)malloc(sizeof(cookies_t));
    cookies->unumber_cookies = 0;
    return 0;
}



int lhttp_request(session_t *session, http_method_e method, const char *url){
    //at first, get the host name and path from url
    //......
    //
    char    *host = NULL;
    char    *path = NULL;
    uint16_t port = 0;
    parse_url(url, host, path, &port);
    //
    //这里首先应该去ConnectionPool中根据host查找对应的连接
    //查找到连接之后会直接跳过dns和connection的过程，直接向socket写数据
    //没查找到对应的session则会新建一个session并加到conectionpool中
    //因为关于连接池的部分还没有做，所以这里每次都是一个新session
    session = (session_t *)malloc(sizeof(session));
    
    //用于一个http请求的request
    request_t  *request = (request_t *)malloc(sizeof(request_t));
    request->url_len = strlen(url);
    request->url = (char *)malloc(request->url_len+1);
    memset(request->url, 0, request->url_len);
    memcpy(request->url, url, request->url_len);
    request->host = host;
    request->path = path;
    request->method = method;
    request->headers = default_headers();
    request->cookies = default_cookies();


    //这里设计会有一个dns缓存，首先会在dns缓存中查找dns,如果查找到dns则不需要dns的过程
    //因为dns缓存的部分还没有做，所以会每次都dns
    int r = dns(request, url, HTTP_PORT);
    return r;
}




/**********************************************************************************/
//
//
//

int parse_url(const char *url, char *host, char *path, uint16_t *port)
{
    struct http_parser_url u;
    if(0 == http_parser_parse_url(url, strlen(url), 0, &u))
    {
        if(u.field_set & (1 << UF_PORT))
        {
            *port = u.port;
        }
        else
        {
            *port = 80;
        }
        
        if(u.field_set & (1 << UF_HOST) )
        {
            host = (char*)malloc(u.field_data[UF_HOST].len+1);
            strncpy(host, url+u.field_data[UF_HOST].off, u.field_data[UF_HOST].len);
            host[u.field_data[UF_HOST].len] = 0;
        }

        if(u.field_set & (1 << UF_PATH))
        {
            path = (char*)malloc(u.field_data[UF_PATH].len+1);
            strncpy(path, url+u.field_data[UF_PATH].off, u.field_data[UF_PATH].len);
            path[u.field_data[UF_PATH].len] = 0;
        }

        return 0;
    }

    return -1;
}


void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf){
    *buf = uv_buf_init((char *)malloc(suggested_size), suggested_size);
}

//start the http get/post/...
//the first step dns
//the dns callback function is on_getaddrinfo
int dns(request_t *request, const char *host, const char *port){
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;

    uv_getaddrinfo_t *getaddrinfo_req = (uv_getaddrinfo_t *)malloc(sizeof(uv_getaddrinfo_t));
    getaddrinfo_req->data = request;
    fprintf(stderr, "%s is...", host);

    int r = uv_getaddrinfo(uv_default_loop(), getaddrinfo_req, http_connect, host, port, &hints);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    if(r)
    {
        fprintf(stderr, "uv_getaddrinfo call error %s\n", uv_err_name(r));
        return 1;
    }
    return 0;
}


//the 2nd step: connect to the server
//the connect callback function is on_connect
void http_connect(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
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

    int r = uv_tcp_connect(connect_req, socket, (struct sockaddr *)res->ai_addr, http_write);
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
void http_write(uv_connect_t *req, int status)
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
    int r = uv_write(write_req, req->handle, &a, 1, http_read_start);

    if(r)
    {
        fprintf(stderr, "uv_write call error %s\n", uv_err_name(r));
        return;
    }

    free(req);
    
}


//the 4th step: read something from the socket
//the readcallback function is on_read
void http_read_start(uv_write_t *req, int status){
   if(status < 0)
   {
        fprintf(stderr, "uv_write callback error %s\n", uv_err_name(status));
        return;
   }

   fprintf(stderr, "write write\n");
   int r = uv_read_start(req->handle, alloc_buffer, http_read);

   if(r)
   {
       fprintf(stderr, "uv_read_start call error %s\n", uv_err_name(r));
       return;
   }

   free(req);

}


//the 5th step: read data
//Tipe:The uv_read_cb callback will be made several times until there is no more data to read or uv_read_stop() is called
void http_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf){
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
