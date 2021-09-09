#ifndef PARSE_FEED_H_
#define PARSE_FEED_H_

#include <json.h>
#include <string.h>
#include <stdio.h> // for DEBUG
#include <assert.h>

struct json_value_s *json_parse(const void *src, size_t src_size);
char* getLastJSON( char *feed);
char* getFenFromJson(char *feed);

#endif // include guard