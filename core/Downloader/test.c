/*
 *
 *
 *
 *
 *
 */


#include "request.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    session_t *session;
    lhttp_request(session, GET, argv[1]);
    return 0;
}
