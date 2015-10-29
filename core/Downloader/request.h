#ifndef __LHTTP_H__
#define __LHTTP_H__

/*
 *         _____       _     __         
 *   _____/ ___/____  (_)___/ /__  _____
 *  / ___/\__ \/ __ \/ / __  / _ \/ ___/
 * / /__ ___/ / /_/ / / /_/ /  __/ /    
 * \___//____/ .___/_/\__,_/\___/_/     
 *          /_/                         
 *
 *
 *
 */

#include <uv.h>

#define HTTP_PORT   "80"
#define HTTPS_PORT  "443"

#define MAX_HEADERS         15
#define MAX_ELEMENT_SIZE    2048
#define MAX_COOKIES         10

#define MAX_HOSTNAME_SIZE   20


/* The connection pool, when start a connection, add it to the connection pool.
 * It is for session, because session should reuse a TCP connection to send several request. */
struct conn_pool_t{
};



/* http methods */
typedef enum http_method{
    OPTIONS,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    CONNECT
}http_method;


/* http head */
typedef struct headers_t{
    char    headers[MAX_HEADERS][2][MAX_ELEMENT_SIZE];
    int     number_headers;
}headers_t;

/* http request cookies */
typedef struct cookies_t{
    char    cookies[MAX_COOKIES][2][MAX_ELEMENT_SIZE];
    int     unumber_cookies;
}cookies_t;


/* The request is a http get/post/... */
typedef struct request_t{
    enum http_method    method;
    char                *url;
    headers_t           headers;
    cookies_t           cookies;
    char                *data;
}request_t;


/* The session allows you to persist certain parameters across requests.
 * If you're makeing several requests to the same host, the underlying TCP connection will be reused.
 * When you'are makeing a request, at first, find the host in conn_pool, if find it ,the connection
 * to the host will be reused, if not, it will start a new connection, and add it to the conn_pool*/
typedef struct session_t{
    char        host[MAX_HOSTNAME_SIZE];
    headers_t   headers;
    cookies_t   cookies;
}session_t;




/* the function will be called when */
typedef void (*on_http_cb)(char  *);

typedef void (*timer_cb)(void);

session_t   *http_default_session();

int http_session_request(session_t *, http_method , const char *url);

int http_session_setopt(request_t *, const char *, void *);

int http_request_get(request_t *, const char *);

#endif
