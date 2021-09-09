#include <parse_feed.h>

static struct json_object_element_s *
initJsonObjElement(struct json_value_s *rt) {
  assert(rt->type == json_type_object);
  struct json_object_s *object = (struct json_object_s *)rt->payload;
  struct json_object_element_s *a = object->start;
  return a;
}

static char *getKeyString(struct json_object_element_s *a) {
  struct json_string_s *a_name = a->name;
  return a_name->string;
}

static char *getValueString(struct json_object_element_s *a) {
  struct json_value_s *a_value = a->value;
  struct json_string_s *a_value_string =
      (struct json_string_s *)a_value->payload;
  return a_value_string->string;
}

char *getLastJSON(char *feed) {
  /*    get the pointer to the last character of the char string.
      Move backwards until return character is met of argument char pointer is
     met. return the pointer to the character after the return character
  */

  char *lastCharPtr = strchr(feed, '\0');
  size_t totalStrLength = strlen(feed),
         currStrLength = 0; // + 1 to include '\0'
  size_t returnCount = 0;
  while (currStrLength < totalStrLength) {
    //        if(*lastCharPtr == '\n' && *lastCharPtr == '\r')
    if (*lastCharPtr ==
        '{') // really bad way to extract strings. It works for now
      if (*(lastCharPtr + 2) == 't')
        break;
    ++currStrLength;
    --lastCharPtr;
  }
  return (*lastCharPtr == '\n' && *lastCharPtr == '\r') ? lastCharPtr + 1
                                                        : lastCharPtr;
}

char *getFenFromJson(char *feed) {
  char *fen;
  // printf("%s Feed is %s\n", feed);

  struct json_value_s *root = json_parse(feed, strlen(feed));
  struct json_object_element_s *a = initJsonObjElement(root);

  char *aName = getKeyString(a);
  char *aValue = getValueString(a);
  // TODO: Rewrite this whole thing to be more legible and codewise efficient
  if (!strcmp(aName, "t")) {
    if (!strcmp(aValue, "fen")) {
      a = a->next;
      struct json_value_s *a_value = a->value;
      a = json_value_as_object(a_value)->start;
      fen = (char *)malloc(sizeof(char) * strlen(getValueString(a)));
      strcpy(fen, getValueString(a));
    } else if (!strcmp(aValue, "featured")) {
      a = a->next;
      struct json_value_s *a_value = a->value;
      a = json_value_as_object(a_value)->start;
      a = a->next->next->next;
      fen = (char *)malloc(sizeof(char) * strlen(getValueString(a)));
      strcpy(fen, getValueString(a));
    }
  }
  free(root);
  return (fen) ? fen : "failed";
}

int isNewGame(char *unparsedJson) {
  int flag = 0;
  struct json_value_s *root = json_parse(unparsedJson, strlen(unparsedJson));
  struct json_object_element_s *a = initJsonObjElement(root);

  char *aName = getKeyString(a);
  char *aValue = getValueString(a);
  // printf("%s %s\n", aName, aValue);
  if (!strcmp(aName, "t") && !strcmp(aValue, "featured"))
    flag = 1;
  free(root);
  return flag;
}

uint fillGameInfo(lichess_data_t *liData) {
  uint errFlag = 1;
  return errFlag;
}