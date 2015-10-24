#include<stdio.h>
#include<regex.h>

/*
  str:正文
  pattern:正则模式
*/
int regex(const char* str, char* pattern) {
  int cflags = REG_EXTENDED;
  regex_t reg;
  int status;//状态，用来纪录正则匹配的情况
  regmatch_t pmatch[1];
  int nmatch = 1;
  /*
    编译正则表达式
   */
  regcomp(&reg, pattern, cflags);
  status = regexec(&reg, str, nmatch, pmatch, 0);
  if (status == REG_NOMATCH) {
    //未匹配上
    return -1;
  } else if (status == 0){
    //匹配
    int i;
    for (i = pmatch[0].rm_so; i < pmatch[0].rm_eo; ++i) {
      putchar(str[i]);
    }
    printf("\n");
  }
  /*
    清理编译过的正则规则
   */
  regfree(&reg);
  return 0;
}

/*
  测试单元
*/
/*
int main() {
  regex("==123456@asd.comsas", "[0-9a-zA-Z]+@[a-zA-Z]+\.com");
  return 0;
}
*/
