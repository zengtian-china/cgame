#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "json_storage.h"

#define GET_INT(user,field) do { \
    cJSON *_tmp = cJSON_GetObjectItem(json, #field); \
    if (_tmp && _tmp->valueint) user->field = _tmp->valueint; \
} while(0)

#define GET_STRING(user,field) do { \
    cJSON *_tmp = cJSON_GetObjectItem(json,#field);\
    if(_tmp && _tmp->valuestring ){\
         strncpy(user->field,_tmp->valuestring,sizeof(user->field) -1);\
        user->field[sizeof(user->field)-1] = '\0';\
    }} while(0)

#define GET_FLOAT(user,number) do{ \
    cJSON *_tmp = cJSON_GetObjectItem(json,#number);\
    if (_tmp !=NULL) user->number = (float)_tmp->valuedouble;\
}while(0)


#define SET_INT(objects,number) do{ \
     cJSON_AddNumberToObject(root,#number,objects->number);  \
}while (0)


#define SET_STRING(root,str) do{ \
    cJSON_AddItemToArray(root,cJSON_CreateNumber(user->equipment[str])); \
} while (0)

#define SET_FLOAT(user,number) do { \
    char _buf[32]; \
    snprintf(_buf, sizeof(_buf), "%.2f", user->number); \
    cJSON_AddRawToObject(root, #number, _buf); \
} while(0)


// 从json中拿到的枚举类型 是一个字符串
ItemType str_type_to_item(const char * str) {
    if (str == NULL) return -1;
    #define X(a,b,c) if(strcmp(c,str) == 0) return a;
    TMP_TIAN(X)
    #undef X
    return -1;
}

#define GET_ENUM(user,xx,cc)  do{ \
    cJSON *_tmp = cJSON_GetObjectItem(json,#xx);      \
    if (_tmp && _tmp->valuestring) user->xx = (cc)(_tmp->valuestring); \
}while(0)

char * item_type_to_str(ItemType xx){
    #define X(a,b,c) if(xx == a) return c;
    TMP_TIAN(X)
    #undef X
    return "unknown";
}

#define SET_ENUM(user,xx,cc) do{ \
    cJSON_AddStringToObject(root,#xx,(cc)(user->xx)); \
}while(0)


//读取json文件
cJSON *json_load_file(const char *path){
    FILE *fp = fopen(path,"rb");
    if( fp ==NULL){
        printf("错误，无法打开\n");
        return NULL;
    }
    fseek(fp,0,SEEK_END);
    long size = ftell(fp);
    fseek(fp,0,SEEK_SET);
    if(size <=0){
        fclose(fp);
        return NULL;
    }
    char *strs = malloc(size+1);
    if(strs == NULL){
        fclose(fp);
        return NULL;
    }
    size_t bytes_read = fread(strs,1,size,fp);
    if (bytes_read != (size_t)size) {
        perror("读取文件不完整");
        free(strs);
        fclose(fp);
        return NULL;
    }
    strs[size] = '\0';
    cJSON *json_root = cJSON_Parse(strs);
    free(strs);
    fclose(fp);
    return json_root;
}

User *json_parse_user(cJSON *json){
    // 创建一个对象 
    User *user = calloc(1,sizeof(User));
    if( user == NULL){
        printf("user初始化失败\n");
        return NULL;
    }
    cJSON *username = cJSON_GetObjectItem(json,"username");
    if (username !=NULL) strcpy(user->username,username->valuestring);
    GET_INT(user,level);
    GET_INT(user,exp);          // ✅ long 用 valuedouble 转换
    GET_INT(user,gold);
    GET_INT(user,race);  
    GET_INT(user,class_id);

    // ========== 基础属性 ==========
    GET_INT(user,strength);
    GET_INT(user,physique);
    GET_INT(user,endurance);
    GET_INT(user,agility);
    GET_INT(user,intelligence);

    // ========== 战斗属性 ==========
    GET_INT(user,max_hp);
    GET_INT(user,hp);
    GET_INT(user,max_mp);
    GET_INT(user,mp);
    GET_INT(user,attack);
    GET_INT(user,defense);
    GET_INT(user,speed);
    GET_INT(user,magic_attack);
    GET_INT(user,magic_defense);
    GET_FLOAT(user,dodge_rate);
    GET_FLOAT(user,crit_rate);

     // ========== 装备 (数组) ==========
    cJSON *equipment = cJSON_GetObjectItem(json, "equipment");
    if (equipment != NULL && cJSON_IsArray(equipment)) {
        int size = cJSON_GetArraySize(equipment);
        if (size > 6) size = 6;
        for (int i = 0; i < size; i++) {
            cJSON *item = cJSON_GetArrayItem(equipment, i);
            if (item != NULL) {
                cJSON *item_id = cJSON_GetObjectItem(item,"item_id");
                cJSON *enhance_level = cJSON_GetObjectItem(item,"enhance_level");
                if(item_id !=NULL) user->equipment[i].item_id = item_id->valueint;
                if(enhance_level !=NULL) user->equipment[i].enhance_level = enhance_level->valueint;
            }
        }
    }

    // ========== 背包 (二维数组) ==========
    cJSON *inventory = cJSON_GetObjectItem(json, "inventory");
    if (inventory != NULL && cJSON_IsArray(inventory)) {
        int size = cJSON_GetArraySize(inventory);
        if (size > 20) size = 20;
        for (int i = 0; i < size; i++) {
            cJSON *row = cJSON_GetArrayItem(inventory, i);
            if (row != NULL && cJSON_IsArray(row)) {
                cJSON *id  = cJSON_GetArrayItem(row, 0);
                cJSON *qty = cJSON_GetArrayItem(row, 1);
                if (id  != NULL) user->inventory[i][0] = id->valueint;
                if (qty != NULL) user->inventory[i][1] = qty->valueint;
            }
        }
    }
    GET_INT(user,inventory_count);

    // ========== 技能 (二维数组) ==========
    cJSON *skills = cJSON_GetObjectItem(json, "skills");
    if (skills != NULL && cJSON_IsArray(skills)) {
        int size = cJSON_GetArraySize(skills);
        if (size > 10) size = 10;
        for (int i = 0; i < size; i++) {
            cJSON *row = cJSON_GetArrayItem(skills, i);
            if (row != NULL && cJSON_IsArray(row)) {
                cJSON *id  = cJSON_GetArrayItem(row, 0);
                cJSON *lv  = cJSON_GetArrayItem(row, 1);
                cJSON *ti  = cJSON_GetArrayItem(row, 2);
                if (id != NULL) user->skills[i][0] = id->valueint;
                if (lv != NULL) user->skills[i][1] = lv->valueint;
                if (ti != NULL) user->skills[i][2] = ti->valueint;
            }
        }
    }
    GET_INT(user,skill_count);
    GET_INT(user,skill_points);

    // ========== 任务 ==========
    cJSON *tasks = cJSON_GetObjectItem(json,"tasks");
    if (!tasks && cJSON_IsArray(tasks)){
        int size = cJSON_GetArraySize(tasks);
        // int child = cJSON
        for(int i =0;i<size;i++){
            cJSON * child = cJSON_GetArrayItem(tasks,i);
            if(child !=NULL){
                cJSON *task_id = cJSON_GetObjectItem(child,"task_id");
                cJSON *status = cJSON_GetObjectItem(child,"status");
                cJSON *progress = cJSON_GetObjectItem(child,"progress");
                if (task_id !=NULL) user->tasks[i].task_id = task_id->valueint;
                if (status !=NULL) user->tasks[i].status = status->valueint;
                if (progress != NULL) user->tasks[i].progress = progress->valueint;
            }
        }
    }

    GET_INT(user,task_count);

    // ========== 位置 ==========
    GET_INT(user,map_id);
    GET_INT(user,pos_x);
    GET_INT(user,pos_y);
    // ========== 其他 ==========
    GET_INT(user,contribution);
    GET_INT(user,reputation);

    return user;
}

cJSON *json_serialize_user(User *user){
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"username",user->username);
    SET_INT(user,level);
    SET_INT(user,exp);
    SET_INT(user,gold);
    SET_INT(user,race);
    SET_INT(user,class_id);
    //基础属性
    SET_INT(user,strength);
    SET_INT(user,physique);
    SET_INT(user,endurance);
    SET_INT(user,agility);
    SET_INT(user,intelligence);
    //战斗属性
    SET_INT(user,max_hp);
    SET_INT(user,hp);
    SET_INT(user,max_mp);
    SET_INT(user,mp);
    SET_INT(user,attack);
    SET_INT(user,defense);
    SET_INT(user,speed);
    SET_INT(user,magic_attack);
    SET_INT(user,magic_defense);
    SET_FLOAT(user,dodge_rate);
    SET_FLOAT(user,crit_rate);
    cJSON *equipment = cJSON_CreateArray();
    for(int i=0;i<6;i++){
        cJSON *child = cJSON_CreateObject();
        cJSON_AddNumberToObject(child,"item_id",user->equipment[i].item_id);
        cJSON_AddNumberToObject(child,"enhance_level",user->equipment[i].enhance_level);
        cJSON_AddItemToArray(equipment,child);
    }
    cJSON_AddItemToObject(root,"equipment",equipment);

    cJSON *invertory = cJSON_AddArrayToObject(root,"invertory");
    for(int i =0; i<user->inventory_count;i++){
        cJSON *tmp = cJSON_CreateArray();
        cJSON_AddItemToArray(tmp,cJSON_CreateNumber(user->inventory[i][0]));
        cJSON_AddItemToArray(tmp,cJSON_CreateNumber(user->inventory[i][1]));
        cJSON_AddItemToArray(invertory,tmp);
    }
    SET_INT(user,inventory_count);
    cJSON *tmp_skills = cJSON_CreateArray();
    for(int i =0;i<user->skill_count;i++){
        cJSON *tmp_1 = cJSON_CreateArray();
        cJSON_AddItemToArray(tmp_1,cJSON_CreateNumber(user->skills[i][0]));
        cJSON_AddItemToArray(tmp_1,cJSON_CreateNumber(user->skills[i][1]));
        cJSON_AddItemToArray(tmp_1,cJSON_CreateNumber(user->skills[i][2]));
        cJSON_AddItemToArray(tmp_skills,tmp_1);
    }
    cJSON_AddItemToObject(root,"skills",tmp_skills);

    SET_INT(user,skill_count);
    SET_INT(user,skill_points);
    cJSON *tasks = cJSON_CreateArray();
    for(int i=0;i<user->task_count;i++){
        cJSON *child = cJSON_CreateObject();
        cJSON_AddNumberToObject(child,"task_id",user->tasks[i].task_id);
        cJSON_AddNumberToObject(child,"status",user->tasks[i].status);
        cJSON_AddNumberToObject(child,"progress",user->tasks[i].progress);
        cJSON_AddItemToArray(tasks,child);
    }
    cJSON_AddItemToObject(root,"tasks",tasks);
    SET_INT(user,task_count);

    SET_INT(user,map_id);
    SET_INT(user,pos_x);
    SET_INT(user,pos_y);
    SET_INT(user,contribution);
    SET_INT(user,reputation);
    return root;
}


int json_save_file(const char *path, cJSON *json){
    printf("path :%s\n",path);
    FILE *fp = fopen(path,"w");
    if (fp == NULL) {
        printf("文件写入失败\n");
        fclose(fp);  
        return -1;
    }
    char * strs = cJSON_Print(json);
    if (strs == NULL){
        printf("json数据为空\n");
        fclose(fp);
        return -1;
    }
    int ww = fprintf(fp, "%s", strs);
    if( ww <0) {
        printf("写入失败\n");
        fclose(fp);  
        return -1;
    }
    printf("%s\n",strs);
    fclose(fp);
    free(strs);
    return 0;

}


ItemConfig *json_parse_item(cJSON *json){
    ItemConfig * item = calloc(1,sizeof(ItemConfig));
    if (!item) return NULL;
    GET_INT(item,id);
    GET_STRING(item,name);
    GET_ENUM(item,type,str_type_to_item);
    GET_INT(item,max_stack);
    GET_INT(item,price);
    GET_INT(item,sell_price); //出售价格
    GET_STRING(item,desc);
    GET_INT(item,usable); //0 =不可使用 1=可使用
    GET_INT(item,use_effect_type);
    GET_INT(item,use_effect_value);
    //判断类型
    if(item->type ==ITEM_TYPE_EQUIPMENT){
        //装备类型
        GET_INT(item,equip_slot);
        GET_INT(item,quality);
        GET_INT(item,level_require);
        GET_INT(item,attack_bonus);
        GET_INT(item,defense_bonus);
        GET_INT(item,speed_bonus);
        GET_INT(item,magic_attack_bonus);
        GET_INT(item,magic_defense_bonus);
        GET_INT(item,max_hp_bonus);
        GET_INT(item,max_mp_bonus);
        GET_FLOAT(item,crit_bonus);
        GET_FLOAT(item,dodge_bonus);
    }
    return item;
}


cJSON *json_serialize_item(ItemConfig *item){
    //item 转成 json
    cJSON *root = cJSON_CreateObject();
    SET_INT(item,id);
    cJSON_AddStringToObject(root,"name",item->name);
    SET_ENUM(item,type,item_type_to_str);
    SET_INT(item,max_stack);
    SET_INT(item,price);
    SET_INT(item,sell_price); 
    cJSON_AddStringToObject(root,"desc",item->desc);
    SET_INT(item,usable);
    SET_INT(item,use_effect_type);
    SET_INT(item,use_effect_value);

    // 判断类型是不是装备类型
    if(item->type == ITEM_TYPE_EQUIPMENT){
        SET_INT(item,equip_slot);
        SET_INT(item,quality);
        SET_INT(item,level_require);
        SET_INT(item,attack_bonus);
        SET_INT(item,defense_bonus);
        SET_INT(item,speed_bonus);
        SET_INT(item,magic_attack_bonus);
        SET_INT(item,magic_defense_bonus);
        SET_INT(item,max_hp_bonus);
        SET_INT(item,max_mp_bonus);
        SET_FLOAT(item,crit_bonus);
        SET_FLOAT(item,dodge_bonus);
    }
    return root;
}



Task *json_parse_task(cJSON *json){
    Task *task = calloc(1,sizeof(Task));
    if (!task) return NULL;
    GET_INT(task,id);
    GET_STRING(task,name);
    GET_STRING(task,desc);
    GET_INT(task,level_require);
    GET_INT(task,pre_quest_id);

    cJSON *type = cJSON_GetObjectItem(json,"cond_type");
    if (type !=NULL) task->cond_type = type->valueint;
    GET_INT(task,cond_target_id);
    GET_INT(task,cond_target_count);
    GET_INT(task,reward_exp);
    GET_INT(task,reward_gold);

    cJSON *reward_items = cJSON_GetObjectItem(json,"reward_items");
    if(reward_items !=NULL && cJSON_IsArray(reward_items)){
        int size = cJSON_GetArraySize(reward_items);
        for(int i=0;i<size;i++){
            cJSON *child = cJSON_GetArrayItem(reward_items,i);
            if (child !=NULL){
                cJSON *nu1 = cJSON_GetArrayItem(child,0);
                cJSON *nu2 = cJSON_GetArrayItem(child,1);
                task->reward_items[i][0] = nu1->valueint;
                task->reward_items[i][1] = nu2->valueint;
            }
        }
    }
    GET_INT(task,reward_items_count);
    return task;
}
cJSON *json_serialize_task(Task *task){
    cJSON *root =cJSON_CreateObject();
    if(!root) return NULL;
    SET_INT(task,id);
    cJSON_AddStringToObject(root,"name",task->name);
    cJSON_AddStringToObject(root,"desc",task->desc);
    SET_INT(task,level_require);
    SET_INT(task,pre_quest_id);

    SET_INT(task,cond_type);
    SET_INT(task,cond_target_id);
    SET_INT(task,cond_target_count);

    SET_INT(task,reward_exp);
    SET_INT(task,reward_gold);
    cJSON *reward_items = cJSON_CreateArray();
    for(int i=0;i<task->reward_items_count;i++){
        cJSON *child =cJSON_CreateArray();
        cJSON_AddItemToArray(child,task->reward_items[i][0]);
        cJSON_AddItemToArray(child,task->reward_items[i][1]);
        cJSON_AddItemToArray(reward_items,child);
    }
    cJSON_AddItemToObject(root,"reward_items",reward_items);
    SET_INT(task,reward_items_count);
    return root;
    
}