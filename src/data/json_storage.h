#ifndef _JSON_H_
#define _JSON_H_
#include "cJSON.h"
#include "player_sv.h"
//ai给出的
#define GET_INT(field) do { \
    cJSON *_tmp = cJSON_GetObjectItem(json, #field); \
    if (_tmp != NULL) user->field = _tmp->valueint; \
} while(0)


#define GET_FLOAT(number) do{ \
    cJSON *_tmp = cJSON_GetObjectItem(json,#number);\
    if (_tmp !=NULL) user->number = (float)_tmp->valuedouble;\
}while(0)


#define SET_INT(root,number) do{ \
     cJSON_AddNumberToObject(root,#number,user->number);  \
}while (0)

#define SET_STRING(root,str) do{ \
    cJSON_AddItemToArray(root,cJSON_CreateNumber(user->equipment[str])); \
} while (0)


cJSON *json_load_file(const char *path);
int json_save_file(const char *path, cJSON *json);
void json_free(cJSON *json);

User *json_parse_user(cJSON *json);
cJSON *json_serialize_user(User *user);

// Monster *json_parse_monster(cJSON *json);
// cJSON *json_serialize_monster(Monster *monster);

// Skill *json_parse_skill(cJSON *json);
// cJSON *json_serialize_skill(Skill *skill);

// Equipment *json_parse_equipment(cJSON *json);
// cJSON *json_serialize_equipment(Equipment *equip);

// Item *json_parse_item(cJSON *json);
// cJSON *json_serialize_item(Item *item);

// Task *json_parse_task(cJSON *json);
// cJSON *json_serialize_task(Task *task);

// MapInfo *json_parse_map(cJSON *json);
// cJSON *json_serialize_map(MapInfo *map);
#endif

