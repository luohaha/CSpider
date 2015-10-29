/*
 *
 *
 *
 *
 *
 */


#include "lhttp.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

lhttp_client_t lhttp_client;

void on_time(){
    lhttp_client_get(&lhttp_client, "www.qq.com");
    printf("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqq\n");
    lhttp_client_get(&lhttp_client, "www.github.com");
    printf("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
}

int main(int argc, char *argv[])
{
    //LHTTP_CLIENT   *lhttp_client = (lhttp_client_t *)malloc(sizeof(lhttp_client_t));
    lhttp_client_init(&lhttp_client);


    //int r = lhttp_client_run(lhttp_client);
    //printf("RRRRRRRRRR%d\n", r);
    //lhttp_client_get(&lhttp_client, "www.github.com");
    //printf("gggggggggggggggggggggggg\n");
    
    //lhttp_client_run(lhttp_client);
    //lhttp_client_get(&lhttp_client, "www.qq.com");
    //printf("qqqqqqqqqqqqqqqqqqqqqqq\n");
    lhttp_client_run(&lhttp_client, on_time);
    printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr\n");
    //lhttp_client_get(lhttp_client, "www.github.com");
    //lhttp_client_get(lhttp_client, "www.qq.com");
    //sleep(1);
    //free(lhttp_client);
    return 0;
}
