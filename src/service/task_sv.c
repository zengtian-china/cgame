#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "task_sv.h"

static Task taskList[100];
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
    if (task->prev_task_id != 0) {
        int done = 0;
        for (int i = 0; i < user->task_count; i++) {
            if (user->tasks[i].task_id == task->prev_task_id &&
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

