#ifndef PARSE_FEED_H_
#define PARSE_FEED_H_

#include "json.h"

struct json_value_s *json_parse(const void *src, size_t src_size);

#endif // include guard