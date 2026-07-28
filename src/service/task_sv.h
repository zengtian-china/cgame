#ifndef _TASK_H_
#define _TASK_H_
#include "json_storage.h"
#include "cJSON.h"
#define TASK_MAX 20

    typedef struct _list{
        int num;
        Task array[TASK_MAX];
    }ListTask;

    /* 玩家任务状态 */
typedef struct {
    int task_id;
    int status;           /* TaskStatus */
    int progress;         /* 当前进度 */
} PlayerTask;

    void init(const char *path);
    Task *get_by_id(int id);

int accept(User *user, int task_id);
#endif