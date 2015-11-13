#ifndef __HTTP_RES_PARSER_H__
#define __HTTP_RES_PARSER_H__

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
 
 
#include "request.h"

/*
 * @paramer res:需要解析的字符串
 * @paramer response:返回解析后的response
 * @paramer body_off:返回正文部分在当前res中的偏移，如果没有正文则返回-1，全部为正文则返回1
 * @return 0：res中包含了完整的http head
 *         1：res中没有包含完整的http head
 *         -1：出错
 */
int http_res_parser(const char *res, response_t *response, int *body_off);

#endif
