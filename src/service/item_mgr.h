#ifndef ITEM_MGR_H
#define ITEM_MGR_H

#include "item_sv.h" // 包含你之前写的枚举定义

// 对外只暴露函数，绝对不暴露全局数组本身！
void item_mgr_init();
void item_mgr_load_from_json(const char *path);
ItemType* item_mgr_get_by_id(int id); // 通过 ID 拿指针
int item_mgr_get_count();

#endif