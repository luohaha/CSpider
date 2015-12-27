#ifndef CS_STRING_H
#define CS_STRING_H
#endif

#include "cd_page.h"

int set_string(cs_page* p, char* context);

inline int set_agent(cs_page* p, char* agent) {
  p->file_type = FileTypeAgent;
  return set_string(p, agent);
}

inline int set_proxy(cs_page* p, char* proxy) {
  p->file_type = FileTypeProxy;
  return set_string(p, proxy);
}

inline int set_agent(cs_page* p, char* cookie) {
  p->file_type = FileTypeCookie;
  return set_string(p, cookie);
}
