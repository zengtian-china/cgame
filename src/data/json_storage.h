#ifndef _JSON_H_
#define _JSON_H_
#include "cJSON.h"
#include <string.h>
#include "player_sv.h"


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
#define QUEST_REWARD_ITEM_NUM  3   /* 最多奖励3种物品 */

typedef struct {
    int item_id;        // 对应 ItemConfig.id
    int enhance_level;  // 强化等级
} PlayerEquip;

// 玩家任务实例（动态进度）
typedef struct {
    int task_id;    // 对应 Task 配置表的 id
    int status;     // 状态（上面枚举）
    int progress;   // 当前进度（如杀了几只怪）
} PlayerTask;

typedef struct _user{
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
    PlayerEquip equipment[MAX_EQUIPMENT]; //装备以及强化属性

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
    int map_id;
    int pos_x;
    int pos_y;
    // 其他
    int contribution;
    int reputation;
}User;


#define TMP_TIAN(X) \
    X(ITEM_TYPE_CONSUMABLE,0,"consumable") \
    X(ITEM_TYPE_MATERIAL,1,"material") \
    X(ITEM_TYPE_EQUIPMENT,2,"equipment") \
    X(ITEM_TYPE_QUEST,3,"quest")

typedef enum {
    #define TMP_ENUM(a,b,c) a = b,
    // ITEM_TYPE_CONSUMABLE = 0,   /* 消耗品（药水）→ 可堆叠 */
    // ITEM_TYPE_MATERIAL   = 1,   /* 材料         → 可堆叠 */
    // ITEM_TYPE_EQUIPMENT  = 2,   /* 装备         → 不可堆叠 */
    // ITEM_TYPE_QUEST      = 3,   /* 任务物品     → 不可堆叠 */
    TMP_TIAN(TMP_ENUM)
    #undef TMP_ENUM
    ITEM_TYPE_MAX
} ItemType;

typedef struct _item{
    /* ===== 通用字段（所有物品都有） ===== */
    int      id;
    char     name[64];
    ItemType type;              /* 决定行为 */
    int      max_stack;         /* 消耗品=99, 装备=1 */
    int      price;             /* 商店售价 */
    int      sell_price;        /* 出售价格（0=不可出售） */
    char     desc[256];         /* 描述文本 */
    int      usable;            /* 0=不可使用 1=可使用 */

    /* ===== 装备专属字段（非装备时全为 0，不影响） ===== */
    int      equip_slot;              /* 装备槽位 0~5 */
    int      quality;           //品质
    int      level_require;     /* 穿戴等级 */
    int      attack_bonus;
    int      defense_bonus;
    int      speed_bonus;
    int      magic_attack_bonus;
    int      magic_defense_bonus;
    int      max_hp_bonus;
    int      max_mp_bonus;
    float    crit_bonus;
    float    dodge_bonus;
    /* ===== 使用效果（消耗品/任务物品，装备时全为0） ===== */
    int      use_effect_type;   /* 0=无 1=恢复HP 2=恢复MP 3=加经验 4=加金币 */
    int      use_effect_value;  /* 效果数值（正数） */

} ItemConfig;




typedef enum {
    TASK_STATUS_NONE       = 0,  // 未接取
    TASK_STATUS_IN_PROGRESS = 1, // 进行中
    TASK_STATUS_COMPLETED  = 2,  // 已完成（可交差）
    TASK_STATUS_REWARDED   = 3   // 已领取奖励
}QuestState;

/* 任务完成条件类型 */
typedef enum {
    QUEST_COND_NONE       = 0,
    QUEST_COND_KILL       = 1,   /* 击杀指定怪物 */
    QUEST_COND_COLLECT    = 2,   /* 收集指定物品 */
    QUEST_COND_TALK       = 3,   /* 与NPC对话 */
    QUEST_COND_REACH_LVL  = 4,   /* 达到指定等级 */
} QuestCondType;

/* 任务配置表（从JSON加载，全局共享） */
typedef struct _task{
    int          id;
    char         name[64];
    char         desc[256];
    int          level_require;       /* 接取等级要求 */
    int          pre_quest_id;        /* 前置任务ID（0=无） */

    /* 完成条件 */
    QuestCondType cond_type;
    int          cond_target_id;      /* 目标怪物ID / 物品ID / NPC ID */
    int          cond_target_count;   /* 需要数量 */

    /* 奖励 */
    int          reward_exp;
    int          reward_gold;
    int          reward_items[QUEST_REWARD_ITEM_NUM][2]; /* [item_id, count] */
    int          reward_items_count;

} Task;




#define MAX_ACTIVE_QUESTS  10   /* 同时进行的最大任务数 */

typedef enum {
    QUEST_STATE_NONE      = 0,  /* 未接取 */
    QUEST_STATE_ACTIVE    = 1,  /* 进行中 */
    QUEST_STATE_COMPLETE  = 2,  /* 已完成（可交付） */
    QUEST_STATE_FINISHED  = 3,  /* 已交付（彻底结束） */
} QuestState;

typedef struct {
    int        quest_id;       /* 对应 QuestConfig.id */
    QuestState state;
    int        progress;       /* 当前进度（如已击杀数） */
} PlayerQuest;

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

