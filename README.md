# CSpider


c语言实现的易用的高效的网络爬虫框架:)。

##编译与配置方法：
* 编译：  
```
make
```
* 配置：  
```
make move
```

##使用例子
```c
#include<cspider/spider.h>

/*
	用户需要自定义的函数，用于解析抓取到的原始数据，并选择下一步的操作
	char *d : 获取到的原始数据，存放在字符串中
	
*/
void proc(cspider_t *cspider, char *d) {
  char *get = regex("http://(.+?).com", d, REGEX_ALL);
  saveString(cspider, get);
  return;
    //addUrl(cspider, get, NULL, 1);
}

void save(char *str) {
  printf("%s\n", str);
  return;
}

int main() {
  
  cspider_t *spider = init_cspider();
  
  cs_setopt_url(spider, "www.baidu.com", NULL, 1);
  cs_setopt_url(spider, "movie.douban.com", NULL, 1);
  cs_setopt_url(spider, "www.youku.com", NULL, 1);
  cs_setopt_url(spider, "www.baidu.com", NULL, 1);
  cs_setopt_process(spider, proc);
  cs_setopt_save(spider, save);
  
  return cs_run(spider);
}
```

####贡献者：
[littleneko](https://github.com/littleneko)
[luohaha](https://github.com/luohaha)       
