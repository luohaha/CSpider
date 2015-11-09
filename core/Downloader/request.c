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


typedef struct url_parsed_t{
    char    *host;
    char    *port;
    char    *path;
}url_parsed_t;


int dns(request_t *request, const char *);   
void http_connect(uv_getaddrinfo_t *req, int status, struct addrinfo *res);
void http_write(uv_connect_t *req, int status);
void http_read_start(uv_write_t *req, int status);
void http_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);

int parse_url(const char *url, url_parsed_t *url_parsed);
char *prepared_request(const request_t *request);



headers_t *default_headers()
{
    headers_t   *headers = (headers_t *)malloc(sizeof(headers_t));
    strncpy(headers->headers[0][0], CONNECTION, strlen(CONNECTION));
    strncpy(headers->headers[0][1], CONNECTION_DEFAULT, strlen(CONNECTION_DEFAULT));
	
	strncpy(headers->headers[1][0], ACCEPT, strlen(ACCEPT));
    strncpy(headers->headers[1][1], ACCEPT_DEFAULT, strlen(ACCEPT_DEFAULT));
	
	strncpy(headers->headers[2][0], ACCEPT_ENCODING, strlen(ACCEPT_ENCODING));
    strncpy(headers->headers[2][1], ACCEPTENCODING_DEFAULT, strlen(ACCEPTENCODING_DEFAULT));
	
	strncpy(headers->headers[3][0], USER_AGENT, strlen(USER_AGENT));
    strncpy(headers->headers[3][1], USERAGENT_DEFAULT, strlen(USERAGENT_DEFAULT));

    headers->number_headers = 4;

    return headers;
}


cookies_t *default_cookies()
{
    cookies_t   *cookies = (cookies_t *)malloc(sizeof(cookies_t));
    cookies->number_cookies = 0;
    return cookies;
}



int lhttp_request(session_t *session, http_method_e method, const char *url){
    //at first, get the host name and path from url
    //......
    //
    url_parsed_t    *url_parsed = (url_parsed_t *)malloc(sizeof(url_parsed_t));
    if(parse_url(url, url_parsed) != -1)
    {
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
        request->host = url_parsed->host;
        request->host_len = strlen(url_parsed->host);
        request->path = url_parsed->path;
        request->path_len = strlen(url_parsed->path);
        request->method = method;
        request->headers = default_headers();
        request->cookies = default_cookies();


        //这里设计会有一个dns缓存，首先会在dns缓存中查找dns,如果查找到dns则不需要dns的过程
        //因为dns缓存的部分还没有做，所以会每次都dns
        int r = dns(request, HTTP_PORT);
        return r;
    }
    return -1;
}




/**********************************************************************************/
//
//
//

int parse_url(const char *url, url_parsed_t *url_parsed)
{
    if(url)
    {
        int url_len = strlen(url);
        int idx = 0;
        int host_off = 0, path_off = 0;
        int host_len = 0, path_len = 0;
        char    port[10];
        /* url以标准的http://开头，port默认为HTTP_PORT(80)*/
        if (strncmp(url, "http://", 7) == 0)
        {
            host_off = 7;
        }
        /* url以标准的https://开头，默认port为HTTPS_PORT(443)*/
        else if(strncmp(url, "https://", 8) == 0)
        {
            host_off = 8;
        }
        /* 此时认为url以host开头，在没有指定port的情况下，port默认为80。*/
        else
        {
            host_off = 0;
        }

        idx = host_off;
        while(idx < url_len)
        {
            /* port*/
            if(url[idx] == ':' && path_off == 0)
            {
                host_len = idx - host_off;
                idx++;
            }
            /* path*/
            else if(url[idx] == '/')
            {
                if(host_len == 0)
                {
                    host_len = idx - host_off;
                    path_off = idx;
                    idx++;
                }
                else
                {
                    strncpy(port, url+host_off+host_len, idx);
                    path_off = idx;
                    idx++;
                }
            }
            /* just host or path*/
            else
            {
                idx++;
            }
        }
        path_len = url_len - path_off;

        if(host_len == 0)
            host_len = url_len - host_off;

        url_parsed->host = (char *)malloc(host_len + 1);
        strncpy(url_parsed->host, url+host_off, host_len);
        url_parsed->host[host_len] = 0;

        if (path_off != 0)
        {
            url_parsed->path = (char *)malloc(path_len + 1);
            strncpy(url_parsed->path, url + path_off, path_len);
            url_parsed->path[path_len] = 0;
        }
        else
        {
            url_parsed->path = (char *)malloc(2);
            url_parsed->path[0] = '/';
            url_parsed->path[1] = 0;
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
int dns(request_t *request, const char *port){
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;

    uv_getaddrinfo_t *getaddrinfo_req = (uv_getaddrinfo_t *)malloc(sizeof(uv_getaddrinfo_t));
    getaddrinfo_req->data = request;
    fprintf(stderr, "%s is...", request->host);

    int r = uv_getaddrinfo(uv_default_loop(), getaddrinfo_req, http_connect, request->host, port, &hints);

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
	connect_req->data = req->data;
    uv_tcp_t *socket = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(req->loop, socket);
    uv_tcp_keepalive(socket, 1, 60);

    /* where on_connect will be called after the connection is established. 
     * The callback receives the uv_connect_t struct,  which has a member .handle pointing to the socket. */
    int r = uv_tcp_connect(connect_req, socket, (struct sockaddr *)res->ai_addr, http_write);
    fprintf(stderr, "connect ....\n");

	uv_freeaddrinfo(res);
    free(req);
	//free(socket);
	
    if(r)
    {
        fprintf(stderr, "uv_connect call error %s\n", uv_err_name(r));
        return;
    }

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
	
    char *request_line = prepared_request((request_t *)req->data);
	size_t len = strlen(request_line);
    printf("%s\n",request_line);
    uv_buf_t a = {.base = request_line, .len = len};
    uv_write_t *write_req = (uv_write_t *)malloc(sizeof(uv_write_t));
	write_req->data = req->data;
    int r = uv_write(write_req, req->handle, &a, 1, http_read_start);

	free(request_line);
	
	free(req);
	
    if(r)
    {
        fprintf(stderr, "uv_write call error %s\n", uv_err_name(r));
        return;
    }
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
   free(req);
   
   if(r)
   {
       fprintf(stderr, "uv_read_start call error %s\n", uv_err_name(r));
       return;
   }

}


//the 5th step: read data
//Tipe:The uv_read_cb callback will be made several times until there is no more data to read or uv_read_stop() is called
void http_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf){
    if(nread == UV_EOF)
    {
        fprintf(stderr, "EOF\n");
        free(stream);
        return;
    }
    if(nread < 0)
    {
        fprintf(stderr, "uv_read callback error \n");
        return;
    }

   fprintf(stderr, "onread onread\n");
   fprintf(stderr, "%s\n", buf->base);
   //uv_read_stop(stream);
}


/* */
char *prepared_request(const request_t *request){
	char	*pp_req = (char *)malloc(HEAD_SIZE);
	
	switch(request->method){
		case GET:
			sprintf(pp_req, "GET %s HTTP/1.1\r\n", request->path);
			//xxx
			break;
		case POST:
			break;
		default:
			break;
	}

    sprintf(pp_req+strlen(pp_req), "%s: %s\r\n", HOST, request->host);
	int	head_num = request->headers->number_headers;
	for(int i = 0; i<head_num; i++)
	{
		sprintf(pp_req+strlen(pp_req), "%s: %s\r\n", request->headers->headers[i][0], request->headers->headers[i][1]);

	}
	
	int	cookies_num = request->cookies->number_cookies;
	for(int i = 0; i<cookies_num; i++)
	{
		sprintf(pp_req+strlen(pp_req), "%s: %s\r\n", request->cookies->cookies[i][0], request->cookies->cookies[i][1]);
	}
	
	sprintf(pp_req+strlen(pp_req), "\r\n");
    return pp_req;
}
