#include "downloader.h"

uv_loop_t *loop;
uv_process_t child_req;
uv_process_options_t options;
uv_async_t *async;

/*
存放数据池的队列
*/
cs_rawText_queue *data_queue;



void on_exit(uv_process_t *req, int64_t exit_status, int signal) {
  fprintf(stderr, "process exit with status %lld, signal %d\n", exit_status, signal);
  uv_close((uv_handle_t*)req, NULL);
}

size_t save_data(void *ptr, size_t size, size_t nmemb, char *save) {
  size_t current = strlen(save);
  size_t all = size * nmemb;
  save = (char*)realloc(save, all+current);
  strcpy(save+current, (char*)ptr);
  return all;
}

void download(uv_work_t *req) {
  
  char *buf = (char*)malloc(sizeof(char));
  createDataAndInsert(data_queue, buf, "html");
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, req->data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
    
    res = curl_easy_perform(curl);
    
    uv_async_send(async);

    curl_easy_cleanup(curl);
  }
}

void print_data(uv_async_t *handle) {
  printf("%s\n", (char*)data_queue->next->data->data);
  printf("下载完成\n");
  uv_close((uv_handle_t*)handle, NULL);
}

int main(int argc, char **argv) {
  loop = uv_default_loop();
  async = (uv_async_t*)malloc(sizeof(uv_async_t));
  data_queue = initDataQueue();
  /*
  char* args[3];
  args[0] = "./Downloader/download";
  args[1] = "www.baidu.com";
  args[2] = NULL;

  options.exit_cb = on_exit;
  options.file = "./Downloader/download";
  options.args = args;

  int r;
  if ((r = uv_spawn(loop, &child_req, &options))) {
    fprintf(stderr, "错误:%s\n", uv_strerror(r));
    return 1;
  } else {
    fprintf(stderr, "lanched process id : %d\n", child_req.pid);
  }
  */
  uv_work_t req;
  char url[] = "www.baidu.com";
  req.data = (void*) &url;

  uv_async_init(loop, async, print_data);
  uv_queue_work(loop, &req, download, NULL);
  
  return uv_run(loop, UV_RUN_DEFAULT);
}
