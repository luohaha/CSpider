#include "utils.h"
#include "spider.h"
/**
   log : print log to log file
   @flag : could be true or false
   @str1 : string ready to print

   if @flag equal 0
   then print @str1 and @str2

**/
void logger(int flag, const char *str1, char *str2, cspider_t *cspider) {
  if (!flag) {
    //false
    if (cspider->log != NULL) {
      uv_rwlock_wrlock(cspider->log_lock);
      fprintf(cspider->log, str1, str2);
      uv_rwlock_wrunlock(cspider->log_lock);
    }
  }
}
