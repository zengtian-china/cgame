#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "json_storage.h"

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
    GET_INT(user,class);

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
                user->equipment[i] = item->valueint;
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
                if (id  != NULL) user->invertory[i][0] = id->valueint;
                if (qty != NULL) user->invertory[i][1] = qty->valueint;
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
                if (id != NULL) user->skills[i][0] = id->valueint;
                if (lv != NULL) user->skills[i][1] = lv->valueint;
            }
        }
    }
    GET_INT(user,skill_count);

    // ========== 位置 ==========
    GET_INT(user,current_map);
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
    cJSON_AddNumberToObject(root,"level",user->level);
    SET_INT(user,exp);
    SET_INT(user,gold);
    SET_INT(user,race);
    SET_INT(user,class);
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
    for(int i=0;i<6;i++) SET_STRING(equipment,i);
    cJSON_AddItemToObject(root,"equipment",equipment);
    cJSON *invertory = cJSON_AddArrayToObject(root,"invertory");
    for(int i =0; i<user->inventory_count;i++){
        cJSON *tmp = cJSON_CreateArray();
        cJSON_AddItemToArray(tmp,cJSON_CreateNumber(user->invertory[i][0]));
        cJSON_AddItemToArray(tmp,cJSON_CreateNumber(user->invertory[i][1]));
        cJSON_AddItemToArray(invertory,tmp);
    }
    SET_INT(user,inventory_count);
    cJSON *tmp_skills = cJSON_CreateArray();
    for(int i =0;i<user->skill_count;i++){
        cJSON *tmp_1 = cJSON_CreateArray();
        cJSON_AddItemToArray(tmp_1,cJSON_CreateNumber(user->skills[i][0]));
        cJSON_AddItemToArray(tmp_1,cJSON_CreateNumber(user->skills[i][1]));
        cJSON_AddItemToArray(tmp_skills,tmp_1);
    }
    cJSON_AddItemToObject(root,"skills",tmp_skills);
    SET_INT(user,skill_count);
    SET_INT(user,current_map);
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


Equips *json_parse_equipment(cJSON *json){
    Equips *equips = calloc(1,sizeof(Equips));
    if (equips == NULL){
        printf("申请空间失败\n");
        return NULL;
    }
    GET_INT(equips,id);
    cJSON *name = cJSON_GetObjectItem(json,"name");
    if (name !=NULL) strcpy(equips->name,name->valuestring);
    GET_INT(equips,slot);
    GET_INT(equips,quality);
    GET_INT(equips,level_require);
    GET_INT(equips,attack_bonus);
    GET_INT(equips,defense_bonus);
    GET_INT(equips,speed_bonus);
    GET_INT(equips,magic_attack_bonus);
    GET_INT(equips,magic_defense_bonus);
    GET_INT(equips,max_hp_bonus);
    GET_INT(equips,max_mp_bonus);
    GET_FLOAT(equips,crit_bonus);
    GET_FLOAT(equips,dodge_bonus);
    GET_INT(equips,set_id);
    GET_INT(equips,price);
    GET_STRING(equips,description);
    return equips;
}

cJSON *json_serialize_equipment(Equips *equips){
    cJSON *root = cJSON_CreateObject();
    SET_INT(equips,id);
    cJSON_AddStringToObject(root,"name",equips->name);
    SET_INT(equips,slot);
    SET_INT(equips,quality);
    SET_INT(equips,level_require);
    SET_INT(equips,attack_bonus);
    SET_INT(equips,defense_bonus);
    SET_INT(equips,speed_bonus);
    SET_INT(equips,magic_attack_bonus);
    SET_INT(equips,magic_defense_bonus);
    SET_INT(equips,max_hp_bonus);
    SET_INT(equips,max_mp_bonus);
    SET_FLOAT(equips,crit_bonus);
    SET_FLOAT(equips,dodge_bonus);
    SET_INT(equips,set_id);
    SET_INT(equips,price);
    cJSON_AddStringToObject(root,"description",equips->description);
    return root;
}



ItemConfig *json_parse_item(cJSON *json){
    ItemConfig * item = calloc(1,sizeof(ItemConfig));
    if (!item) return NULL;
    GET_INT(item,id);
    GET_STRING(item,name);
    GET_INT(item,type);
    // GET_INT(item,)
    GET_ENUM(item,type,str_type_to_item);
    GET_INT(item,max_stack);
    GET_INT(item,price);
    GET_STRING(item,desc);
    //判断类型
    if(item->type ==ITEM_TYPE_EQUIPMENT){
        //装备类型
        GET_INT(item,slot);
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
    // type
    SET_ENUM(item,type,item_type_to_str);
    SET_INT(item,max_stack);
    SET_INT(item,price);
    cJSON_AddStringToObject(root,"desc",item->desc);

    // 判断类型是不是装备类型
    if(item->type == ITEM_TYPE_EQUIPMENT){
        SET_INT(item,slot);
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