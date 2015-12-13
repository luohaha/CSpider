# CSpider


c语言实现的易用的高效的网络爬虫框架:)。

##安装与使用方法：
* 确认你的电脑中，已经安装下面的库：  
	* [curl](https://github.com/bagder/curl)
	* [libuv](https://github.com/libuv/libuv)
	* [libxml2](http://xmlsoft.org/index.html)
	* [pcre](http://www.pcre.org)
* 进入`core`文件夹，编译文件，命令如下：  
```
make
```
* 接着要配置动态库，将`.so`文件和`.h`文件移动到相应的位置，命令如下:  
```
make install
```
* 这时，就可以开始编译你写好的文件（例如 `test.c`），命令如下:  
```
gcc -o test test.c -lcspider -I /usr/include/libxml2
```

>其中，`-lcspider`将链接我们上述编译完成的cspider动态链接库，而`-I /usr/include/libxml2`则是让编译器能够找到libxml2库的头文件，当然你最好使用Makefile文件。  

##API

###初始化设置

* `cspider_t *init_cspider()`  
	在程序一开始的时候，获取到`cspider_t`变量。是必不可少的函数。  
	
* `void cs_setopt_url(cspider_t *, char *)`  
	为`cspider_t`设置程序启动时要抓取的url，在第二个参数传入url字符串即可，url字符串可以不用包含`http://`或`htpps://`。可以多次调用，即可插入多个url任务。
	
* `void cs_setopt_cookie(cspider_t *, char *)`  
	第二个参数设置cookie，格式为`var1=abc; var2=qwe`。可选项。
	
* `void cs_setopt_useragent(cspider_t *, char *)`  
	第二个参数设置user agent，可选项。
	
* `void cs_setopt_proxy(cspider_t *, char *)`  
	第二个参数设置代理的地址，可选项。  
	
* `void cs_setopt_timeout(cspider_t *, long)`  
	设置超时时间，单位为毫秒，可选项。
	
* `void cs_setopt_logfile(cspider_t *, FILE *)`  
	设置输出日志的打印文件，可选项。不设置，则不会输出日志。  
	
* `void cs_setopt_process(cspider_t *, void (*process)(cspider_t *, char*, void *), void *)`   
	设置解析函数process，解析函数process的第二个参数为抓取到的完整的数据字符串，第三个参数void*指向用户自定义的上下文信息。该上下文指针通过`cs_setopt_process`的第三个参数来设置。
	
* `void cs_setopt_save(cspider_t *, void (*save)(void*, void*), void*)`  
	设置数据持久化函数save，save的参数中第一个指针指向解析函数传入的信息，第二个指针指向用户自定义的上下文信息。该上下文指针同样通过`cs_setopt_save`的第三个参数来设置。
	
* `void cs_setopt_threadnum(cspider_t *cspider, int , int )`  
	设置线程的数量，其中，第二个参数，可以传入`DOWNLOAD`与`SAVE`，分别表示，将要设置下载线程的数量，或是解析与保存线程的数量。第三个参数为要设置的线程的数量。可选项，默认的线程数量均为5个。  
	
* `int cs_run(cspider_t *)`  
	启动cspider的函数，在程序的最后调用。  

###自定义函数部分
	
* `void saveString(cspider_t *, char *)`  
	在自定义的解析函数中，可以调用此函数。数据持久化，将字符串传入自定义的数据持久化函数。  
	
* `void addUrl(cspider_t *cspider, char *url)`  
	在自定义的解析函数中，调用此函数。可以将解析获取到的url，再次加入任务队列。

###解析工具API

1. 正则表达式：  

	* `int regexAll(const char *regex, char *str, char **res, int num, int flag);`  
	regex : 正则匹配的规则  
	str : 待匹配的字符串  
	res : 用来保存返回的字符串的数组  
	num : res数组的大小  
	flag : 可为`REGEX_ALL`和`REGEX_NO_ALL`，输出全部，或者只是匹配的部分。  
	返回获取的字符串的个数
	
	* `int match(char *regex, char *str);`  
	是否匹配。返回1表示成功，返回0标示失败。  
	
2. xpath解析html和xml

	* `int xpath(char *xml, char *path, char **get, int num);`  
	xml : 待匹配的字符串  
	path : xpath规则  
	get : 保存返回字符串的数组   
	num : get数组的大小  
	返回获取的字符串的个数 
	
3. 解析json

	cspider里集成了cJSON库，可以非常方便地解析json。具体的使用方法，请看[这里](https://github.com/kbranigan/cJSON)。

	
##使用例子
输出优酷电影页面，第一页的所有电影名称。  

```c
#include<cspider/spider.h>

void p(cspider_t *cspider, char *d, void *user_data) {
  
  char *get[100];
  int size = xpath(d, "//div[@id='listofficial']/div[@class='yk-row yk-v-80']/div[@class='yk-col3']/div[@class='p p-small']/div[@class='p-meta pa']/div[@class='p-meta-title']/a/@title", get, 100);
  
  int i;
  for (i = 0; i < size; i++) {
    saveString(cspider, get[i]);
  }
  
}

void s(void *str, void *user_data) {
  char *get = (char *)str;
  FILE *file = (FILE*)user_data;
  fprintf(file, "%s\n", get);
  return;
}

int main() {
  cspider_t *spider = init_cspider(); 
  char *agent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:42.0) Gecko/20100101 Firefox/42.0";
  
  cs_setopt_url(spider, "http://www.youku.com/v_olist/c_96_s_0_d_0_g_0_a_0_r_0_u_0_pt_0_av_0_ag_0_sg_0_mt_0_lg_0_q_0_pr_0_h_0_p_1.html");
  
  cs_setopt_useragent(spider, agent);
  //指向自定义的解析函数，和数据持久化函数
  cs_setopt_process(spider, p, NULL);
  cs_setopt_save(spider, s, stdout);
  //设置抓取线程数量，和数据持久化的线程数量
  cs_setopt_threadnum(spider, DOWNLOAD, 2);
  cs_setopt_threadnum(spider, SAVE, 2);
  
  return cs_run(spider);
}

```

