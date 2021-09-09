#include <parse_feed.h>

static struct json_value_s* root; // TODO: get rid of global vars

char* getLastJSON( char *feed){
/*    get the pointer to the last character of the char string.
    Move backwards until return character is met of argument char pointer is met.
    return the pointer to the character after the return character 
*/
    
    char *lastCharPtr = strchr(feed ,'\0');
    size_t totalStrLength = strlen(feed), currStrLength = 0; // + 1 to include '\0'
    size_t returnCount = 0;
    while(currStrLength < totalStrLength){
//        if(*lastCharPtr == '\n' && *lastCharPtr == '\r')
        if(*lastCharPtr == '{') // really bad way to extract strings. It works for now
            if(*(lastCharPtr+2) == 't')
                break;
        ++currStrLength;
        --lastCharPtr;
    }
    return (*lastCharPtr == '\n' && *lastCharPtr == '\r') ? lastCharPtr + 1 : lastCharPtr;
}

char* getFenFromJson(char *feed){
    char *fen;
    //printf("%s Feed is %s\n", feed);

    root = json_parse(feed, strlen(feed));
    assert(root->type == json_type_object);

    struct json_object_s* object = (struct json_object_s*)root->payload;
    struct json_object_element_s* a = object->start;

    struct json_string_s* a_name = a->name;
    struct json_value_s* a_value = a->value;
    struct json_string_s* a_value_string = (struct json_string_s*)a_value->payload;

    // TODO: Rewrite this whole thing to be more legible and codewise efficient
    if(!strcmp(a_name->string,"t")){
        if(!strcmp(a_value_string->string,"fen")){
            a = a->next;
            a_value = a->value;
            a = json_value_as_object(a_value)->start;
            a_value = a->value;
            a_value_string = a_value->payload;
            fen = a_value_string->string;
        }
        else if(!strcmp(a_value_string->string, "featured")){
            a = a->next;
            a_value = a->value;
            a = json_value_as_object(a_value)->start;
            a = a->next;
            a = a->next;
            a = a->next;
            a_value = a->value;
            a_value_string = a_value->payload;
            fen = a_value_string->string;
        }
    }
    //free(root);
    return (fen) ? fen : "failed";
}

