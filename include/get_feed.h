#ifndef GET_FEED_H_
#define GET_FEED_H_

#include <curl/curl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "control.h"

struct mem_struct_t {
  char *memory;
  size_t size;
};

void initCurl();
void curlHandler();
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                           void *userp);

#endif // include guard