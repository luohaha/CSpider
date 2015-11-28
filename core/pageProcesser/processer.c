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
/*
返回指定的字串

*/

char **regexAll(const char *regex, char *str, int *num, int flag) {
  assert(flag == 0 || flag == 1);
  const char *error;
    int   erroffset;
    pcre *re;
    int   rc;
    int   i;
    int   ovector[3];
    char *subb[512];

    re = pcre_compile (regex,          /* the pattern */
                       PCRE_MULTILINE,
                       &error,         /* for error message */
                       &erroffset,     /* for error offset */
                       0);             /* use default character tables */
    if (!re)
    {
        printf("pcre_compile failed (offset: %d), %s\n", erroffset, error);
        return NULL;
    }

    unsigned int offset = 0;
    unsigned int len    = strlen(str);
    int index = 0;
    while (offset < len && (rc = pcre_exec(re, 0, str, len, offset, 0, ovector, sizeof(ovector))) >= 0)
    {
      if (flag == 1) {
	int length = ovector[3] - ovector[2];
	subb[index] = (char*)malloc(sizeof(char)*length);
	strncpy(subb[index], str+ovector[2], length);
	index++;
	offset = ovector[1];
      } else {
	int length = ovector[1] - ovector[0];
	subb[index] = (char*)malloc(sizeof(char)*length);
	strncpy(subb[index], str+ovector[0], length);
	index++;
	offset = ovector[1];
      }
      
    }
    *num = index;
  
  return subb;
}

//text unit
/*
int main() {
  char *src="-a-*-b-*-c-";
  printf("%s\n", regex("-(.+?)-", src, 1));
  //regexAll("-(.+?)-", src);
  return 0;
}
*/
