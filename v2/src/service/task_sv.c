#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "task_sv.h"
#include "item_sv.h"
static Task taskList[10];
int tasklist_count =0;


void init(const char *path){
    char *p = "../../task";
    cJSON * root = json_load_file(p);
    if( !root) return ;
    int size = cJSON_GetArraySize(root);
    
    for(int i=0;i<size;i++){
        // 获取了单个数据
        cJSON *child = cJSON_GetArrayItem(root,i);
        Task *task = json_parse_task(child);
        if (task == NULL) continue;
        taskList[tasklist_count] = *task;
    }
}


Task *get_by_id(int id){
    for(int i=0;i<tasklist_count;i++){
        if(taskList[i].id ==id){
            return &taskList[i];
        }
    }
    return NULL;
}

ListTask *get_available(User *user){
    ListTask *list = calloc(1,sizeof(ListTask));
    int j =0;
    for(int i=0;i<tasklist_count;i++){
        if(user->level >= taskList[i].level_require){
            list->array[i] = taskList[i];
            list->num ++;
            j++;
        }
    }
    return list;
}
int accept(User *user, int task_id){
    /*
    - 0：成功
- 1：等级不足
- 2：前置任务未完成
- 3：已接取
*/
    if(!user || task_id<=0) return -1;
    //获取任务结构体
    Task *task = get_by_id(task_id);
    if(!task) return -1;
    if(task->level_require > user->level) return 1;


        // 前置任务检查                                    fix #2
    if (task->pre_quest_id != 0) {
        int done = 0;
        for (int i = 0; i < user->task_count; i++) {
            if (user->tasks[i].task_id == task->pre_quest_id &&
                user->tasks[i].status == TASK_STATUS_REWARDED) {
                done = 1;
                break;
            }
        }
        if (!done) return 2;
    }
        // 判断任务是否已经接取
    for(int i=0;i<user->task_count;i++){
       if(user->tasks[i].task_id == task_id) return 3;

    }
    // 判断任务是否已满
    if(user->task_count >=MAX_PLAYER_TASKS) return 1;

    
    user->tasks[user->task_count].task_id =task_id;
    user->tasks[user->task_count].status = TASK_STATUS_IN_PROGRESS;
    user->tasks[user->task_count].progress = 0;
    user->task_count++;
    return 0;

}

int update_progress(User *user, int task_id, int progress){
    Task *task = get_by_id(task_id);
    if (!task || progress <=0) return 1; //任务不存在

    for(int i=0;i<user->task_count;i++){
        if(user->tasks[i].task_id == task_id  && user->tasks[i].status ==TASK_STATUS_IN_PROGRESS){
            user->tasks[i].progress += progress;
            if(user->tasks[i].progress >= task->cond_target_count){
                user->tasks[i].status = TASK_STATUS_COMPLETED;
            }
            return 0;
        }
    }
    return 1;
}

// 2 物品没有完全领完
int claim_reward(User *user,int task_id){
    Task *task = get_by_id(task_id);
    if (!task) return 1; //任务不存在
    for(int i=0;i<user->task_count;i++){
        if(user->tasks[i].task_id == task_id && user->tasks[i].status ==TASK_STATUS_COMPLETED){
            // 领奖励
            user->exp += task->reward_exp;
            user->gold += task->reward_gold;
            user->tasks[i].status ==TASK_STATUS_REWARDED;
            // 领物品 判断背包是否满， 判断背包
            for(int i=0;i<task->reward_items_count && user->inventory_count<=MAX_INVENTORY;i++){
                int tmp_number = task->reward_items[i][1];
                int * p = &tmp_number;
                 int status = add(user,task->reward_items[i][0],p);
                 if(status !=0) {
                    printf("物品添加失败\n");
                    // 添加到邮件中;
                    // 传入 task i p
                    return 2;
                 }
            }

            return 0;
        }
    }
    return -1;
}