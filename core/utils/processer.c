#include "utils.h"

/*
  正则匹配， 返回所有的子串
*/
int regexAll(const char *regex, char *str, char **res, int flag) {

  pcre *re;
  //错误信息
  const char *errptr;
  //错误位置
  int erroffset;
  pcre *code;
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
  } while( rc > 0);

  return index;
}
