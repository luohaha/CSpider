#ifndef __HTTP_RES_PRO_H__
#define __HTTP_RES_PRO_H__

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
 
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "request.h"


int get_status_code(char *status_code, char *data, size_t len);

int get_res_head(http_head_t *head, void *data, size_t len);


#endif
