#ifndef _JSON_H_
#define _JSON_H_
#include "cJSON.h"
#include <string.h>
#include "player_sv.h"
#include "equip_sv.h"
#include "item_sv.h"
// #include "equip_sv.h"
/* 任务结构 */
#define ITEM_MAX 20
#define ONE 1
#define ZERO 0
#define SLOT_MAX 6
#define MAX_PLAYER_TASKS 20   // 最多同时接取20个任务
#define MAX_INVENTORY   20
#define MAX_SKILLS      10
#define MAX_EQUIPMENT    6
#define MAX_TASKS       20
#define MAX_NAME_LEN    20
enum {
    TASK_STATUS_NONE       = 0,  // 未接取
    TASK_STATUS_IN_PROGRESS = 1, // 进行中
    TASK_STATUS_COMPLETED  = 2,  // 已完成（可交差）
    TASK_STATUS_REWARDED   = 3   // 已领取奖励
};
// 玩家任务实例（动态进度）
typedef struct {
    int task_id;    // 对应 Task 配置表的 id
    int status;     // 状态（上面枚举）
    int progress;   // 当前进度（如杀了几只怪）
} PlayerTask;
typedef struct {
char username[MAX_NAME_LEN];
    int level;
    long exp;
    int gold;
    int race;             // 1=人 2=仙 3=魔
    int class_id;         // 门派ID（原 class，避免关键字冲突）

    // 基础属性
    int strength;
    int physique;
    int endurance;
    int agility;
    int intelligence;

    // 战斗属性
    int max_hp;
    int hp;
    int max_mp;
    int mp;
    int attack;
    int defense;
    int speed;
    int magic_attack;
    int magic_defense;
    float dodge_rate;
    float crit_rate;

    // 装备
    int equipment[MAX_EQUIPMENT];

    // 背包
    int inventory[MAX_INVENTORY][2];   // [物品ID, 数量]
    int inventory_count;

    // 技能（修正：3列，含冷却）
    int skills[MAX_SKILLS][3];         // [技能ID, 等级, 当前冷却]
    int skill_count;
    int skill_points;                  // ★ 新增：技能点

    // 任务（★ 新增）
    PlayerTask tasks[MAX_TASKS];
    int task_count;

    // 位置
    int current_map;
    int pos_x;
    int pos_y;

    // 其他
    int contribution;
    int reputation;
}User;


typedef struct _tasks{
    int id;
    char name[100];
    int type;
    int level_require;
    int accept_npc_id;
    int complete_type;
    int complete_target;
    int complete_count;
    int exp_reward;
    int gold_reward;
    int item_reward_id;
    int item_reward_count;
    int contribution_reward;
    int description;
    int prev_task_id;
    int c;
} Task;




cJSON *json_load_file(const char *path);
int json_save_file(const char *path, cJSON *json);
void json_free(cJSON *json);

User *json_parse_user(cJSON *json);
cJSON *json_serialize_user(User *user);

// Monster *json_parse_monster(cJSON *json);
// cJSON *json_serialize_monster(Monster *monster);

// Skill *json_parse_skill(cJSON *json);
// cJSON *json_serialize_skill(Skill *skill);

// Equips *json_parse_equipment(cJSON *json);

// cJSON *json_serialize_equipment(Equips *equip);

ItemConfig *json_parse_item(cJSON *json);
cJSON *json_serialize_item(ItemConfig *item);

Task *json_parse_task(cJSON *json);
cJSON *json_serialize_task(Task *task);

// MapInfo *json_parse_map(cJSON *json);
// cJSON *json_serialize_map(MapInfo *map);
#endif

