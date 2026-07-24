#ifndef _ITEM_SV_H_
#define _ITEM_SV_H_

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
    char     desc[256];         /* 描述文本 */

    /* ===== 装备专属字段（非装备时全为 0，不影响） ===== */
    int      slot;              /* 装备槽位 0~5 */
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

} ItemConfig;

#endif