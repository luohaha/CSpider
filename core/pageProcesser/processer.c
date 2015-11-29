#include "pageProcesser.h"

/*
正则匹配，并返回匹配成功的第一个子串
*/
char *regex(const char *regex, char *str, int flag) {
  assert(flag == 1 || flag == 0);
  pcre *code;
  int ovector[6];
  int rc;
  const char *errptr;//保存错误信息
  int erroffset;//错误位置
  char *sub;

  code = pcre_compile(regex, 0, &errptr, &erroffset, NULL);
  rc = pcre_exec(code, NULL, str, strlen(str), 0, 0, ovector, 6);
  pcre_get_substring(str, ovector, rc, flag, &sub);

  /*回收*/
  pcre_free(code);
  
  return sub;
}
