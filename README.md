# CSpider

A scalable and convenient crawler framework base on C:).  
[中文文档.](https://github.com/luohaha/CSpider/wiki/中文文档)

##INSTALL
* Make sure you have installed these libs already.
	* [curl](https://github.com/bagder/curl)
	* [libuv](https://github.com/libuv/libuv)
	* [libxml2](http://xmlsoft.org/index.html)
	* [pcre](http://www.pcre.org)
* Then enter `core`，and compile:

```
make
```

* move `.so` and `.h` files to relevant directory:  

```
make install
```

* finally, you could compile your code(such as `test.c`) using `-lcspider`:  

```
gcc -o test test.c -lcspider -I /usr/include/libxml2
```

> using `-lcspider` to link dynamic link libary, and `-I /usr/include/libxml2` could let compiler to find libxml2's head files.  

##API

###Initial settings
* `cspider_t *init_cspider()`  
	At the beginning of your code, you can get `cspider_t` by this function. It's essential.
	
* `void cs_setopt_url(cspider_t *, char *)`  
	Insert urls into work queue when cspider start, passing url's string to second param. url's string could not contain `http://` or `https://`. You could call this function many times to insert many urls.
	
* `void cs_setopt_cookie(cspider_t *, char *)`  
	Passing cookies to second param, the format is `var1=abc; var2=qwe`. Optional.
	
* `void cs_setopt_useragent(cspider_t *, char *)`  
	Passing user agent to second param. Optional.
	
* `void cs_setopt_proxy(cspider_t *, char *)`  
	Passing proxy to second param. Optional.  
	
* `void cs_setopt_timeout(cspider_t *, long)`  
	Passing timeout value(ms) to second param. Optional.
	
* `void cs_setopt_logfile(cspider_t *, FILE *)`  
	Passing file pointer to second param, which is used to print log information.  
	
* `void cs_setopt_process(cspider_t *, void (*process)(cspider_t *, char*, void *), void *)`   
	Passing process function to second param, and user's costom context pointer to third param. You could use this costom context in process function's third param and string which is downloaded in second param.
	
* `void cs_setopt_save(cspider_t *, void (*save)(void*, void*), void*)`  
	Passing data persistence function to second param, and alse user costom context pointer to third param. In this custom function, you can get pointer to prepared data in second param.
	
* `void cs_setopt_threadnum(cspider_t *cspider, int , int )`  
	Setting the number of thread. The second param could be `DOWNLOAD` and `SAVE`, which indicates two kinds of thread. The third param could be the number of thread you want to set.
	
* `int cs_run(cspider_t *)`  
	Start cspider. Using this at the end of your code.  
	
###More functions

* `void saveString(cspider_t *, char *)`  
	Using this function in costom process function could pass data pointer to costom data persistence function. 
	
* `void addUrl(cspider_t *cspider, char *url)`  
	Using this function in costom process function could insert url to work queue.  
	
###Tools

1. Regular expressions:  

	* `int regexAll(const char *regex, char *str, char **res, int num, int flag);`  
	regex : regular matching rule  
	str : the source string.  
	res : array used for saving strings which is matched.    
	num : array's size.   
	flag : it could be `REGEX_ALL` and `REGEX_NO_ALL`, which means whether to return the whole string.  
	This function returns the number of matched strings.  
	
	* `int match(char *regex, char *str);`  
	Whether it matches. Return 1 for yes, 0 for no.  
	
2. Using xpath to deal with html and xml:

	* `int xpath(char *xml, char *path, char **get, int num);`  
	xml : prepared to parse.  
	path : xpath's rule.  
	get : array used for saving strings.   
	num : array's size.  
	This function returns the number of array which we get.  
	
3. Json:  

	cspider contains cJSON. We could use it to parse json data. Usage is [here](https://github.com/kbranigan/cJSON)。  
	
##Example
Print the Github's main page's source code.  

```c
#include<cspider/spider.h>
/*
	costom process function
*/
void p(cspider_t *cspider, char *d, void *user_data) {

  saveString(cspider, d);
  
}
/*
	costom data persistence function
*/
void s(void *str, void *user_data) {
  char *get = (char *)str;
  FILE *file = (FILE*)user_data;
  fprintf(file, "%s\n", get);
  return;
}

int main() {
  cspider_t *spider = init_cspider(); 
  char *agent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:42.0) Gecko/20100101 Firefox/42.0";
  
  cs_setopt_url(spider, "github.com");
  
  cs_setopt_useragent(spider, agent);
  //
  cs_setopt_process(spider, p, NULL);
  cs_setopt_save(spider, s, stdout);
  //set the thread's number
  cs_setopt_threadnum(spider, DOWNLOAD, 2);
  cs_setopt_threadnum(spider, SAVE, 2);
  
  return cs_run(spider);
}
``` 
