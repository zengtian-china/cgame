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
    printf("%s",strs);
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
    GET_INT(level);
    GET_INT(exp);          // ✅ long 用 valuedouble 转换
    GET_INT(gold);
    GET_INT(race);  
    GET_INT(class);

    // ========== 基础属性 ==========
    GET_INT(strength);
    GET_INT(physique);
    GET_INT(endurance);
    GET_INT(agility);
    GET_INT(intelligence);

    // ========== 战斗属性 ==========
    GET_INT(max_hp);
    GET_INT(hp);
    GET_INT(max_mp);
    GET_INT(mp);
    GET_INT(attack);
    GET_INT(defense);
    GET_INT(speed);
    GET_INT(magic_attack);
    GET_INT(magic_defense);

    
    GET_FLOAT(dodge_rate);
    GET_FLOAT(crit_rate);
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
    GET_INT(inventory_count);

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
    GET_INT(skill_count);

    // ========== 位置 ==========
    GET_INT(current_map);
    GET_INT(pos_x);
    GET_INT(pos_y);
    // ========== 其他 ==========
    GET_INT(contribution);
    GET_INT(reputation);

    return user;
}

cJSON *json_serialize_user(User *user){
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"username",user->username);
    cJSON_AddNumberToObject(root,"level",user->level);
    SET_INT(root,exp);
    SET_INT(root,gold);
    SET_INT(root,race);
    SET_INT(root,class);
    //基础属性
    SET_INT(root,strength);
    SET_INT(root,physique);
    SET_INT(root,endurance);
    SET_INT(root,agility);
    SET_INT(root,intelligence);
    //战斗属性
    SET_INT(root,max_hp);
    SET_INT(root,hp);
    SET_INT(root,max_mp);
    SET_INT(root,mp);
    SET_INT(root,attack);
    SET_INT(root,defense);
    SET_INT(root,speed);
    SET_INT(root,magic_attack);
    SET_INT(root,magic_defense);
    SET_FLOAT(dodge_rate);
    SET_FLOAT(crit_rate);
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
    SET_INT(root,inventory_count);
    cJSON *tmp_skills = cJSON_CreateArray();
    for(int i =0;i<user->skill_count;i++){
        cJSON *tmp_1 = cJSON_CreateArray();
        cJSON_AddItemToArray(tmp_1,cJSON_CreateNumber(user->skills[i][0]));
        cJSON_AddItemToArray(tmp_1,cJSON_CreateNumber(user->skills[i][1]));
        cJSON_AddItemToArray(tmp_skills,tmp_1);
    }
    cJSON_AddItemToObject(root,"skills",tmp_skills);
    SET_INT(root,skill_count);
    SET_INT(root,current_map);
    SET_INT(root,pos_x);
    SET_INT(root,pos_y);
    SET_INT(root,contribution);
    SET_INT(root,reputation);
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