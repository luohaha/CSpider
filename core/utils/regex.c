#include "utils.h"

/**
  regexAll : get strings by regex
  @regex : regular expression
  @str : input string
  @res : array of string what we get
  @num : size of @res
  @flag : 

  return the number of string what we get
**/
int regexAll(const char *regex, char *str, char **res, int num, int flag) {

  pcre *re;
  //错误信息
  const char *errptr;
  //错误位置
  int erroffset;
  int ovector[30];
  //匹配到的结果数量
  int rc;
  //返回的字符数组的下标
  int index;
  /*
    pcre_exec执行的偏移量
   */
  int exec_offset = 0;

  re = pcre_compile(regex, 0, &errptr, &erroffset, NULL);

  if (re == NULL) {
    printf("compilation at %d: %s\n", erroffset, errptr);
    return -1;
  }

  index = 0;
  do {
    rc = pcre_exec(re, NULL, str, strlen(str), exec_offset, 0, ovector, 30);

    if (rc > 0) {
      pcre_get_substring(str, ovector, rc, flag, &res[index++]);
      //设置偏移量
      exec_offset = ovector[1];
    }
  } while( rc > 0 && index < num);

  return index;
}

/**
  match : is it match?
  @regex : regular expression
  @str : input string

  return 1 for yes, 0 for no
**/
int match(char *regex, char *str) {
  pcre *re;
  //错误信息
  const char *errptr;
  //错误位置
  int erroffset;
  int ovector[30];
  //匹配到的结果数量
  int rc;
  

  re = pcre_compile(regex, 0, &errptr, &erroffset, NULL);

  if (re == NULL) {
    printf("compilation at %d: %s\n", erroffset, errptr);
    return -1;
  }

  
    rc = pcre_exec(re, NULL, str, strlen(str), 0, 0, ovector, 30);

    if (rc > 0) {
      //成功
      return 1;
    } else {
      return 0;
    }
}
