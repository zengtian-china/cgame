#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "equip_sv.h"
#include "json_storage.h"
#include "item_mgr.h"


// 从背包穿戴
int EquipFromInventory(User *user, int inventory_index){
    //判断输入数据是否正常
    if(user == NULL || inventory_index<0 || inventory_index>=ITEM_MAX) return -1;
    //获取装备信息 判断是否为物品存在的装备
    ItemConfig * item = item_mgr_get_by_id(user->invertory[inventory_index][0]);
    if(item == NULL || item->type != ITEM_TYPE_EQUIPMENT){
        printf("装备信息不存在\n!");
        return -1;
    }
    //获取装备slot 是否是正常参数
    if(item->slot>=SLOT_MAX || item->slot <ZERO) return -1;
    //判断是否够等级
    if(item->level_require > user->level){
        printf("等级不够，不能穿戴装备\n");
        return -1;
    }
    //定义一个临时数，存放装备id
    int tmp = user->equipment[item->slot];
    //穿上装备
    user->equipment[item->slot] = user->invertory[inventory_index][0];
    //卸下来的装备存放到背包中
    user->invertory[inventory_index][0] = tmp;

    //将数据初始化，移除了装备带来的数据加成
    calc_battle_stats(user);
    //  重新计算战斗属性（移除装备加成）
    equip_sv_apply_stats(user);
    //返回成功值
    return 0;
}



//穿戴装备 从外部穿戴
int EquipDirectly(User *user, int equip_id){
    int found =0;
    // 查找是否背包是否存在装备id
    for(int i=0;i<user->inventory_count;i++){
        if(user->invertory[i][0] == equip_id){
            found = 1;
            break;
        }
    }
    if (user == NULL || equip_id <= 0) {
        printf("用户或装备ID无效\n");
        return -1;
    }

    // 2. 获取配置并校验类型
    ItemConfig *equips = item_mgr_get_by_id(equip_id);
    if (equips == NULL || equips->type != ITEM_TYPE_EQUIPMENT) {
        printf("物品不存在或不是装备\n");
        return -1;
    } 
    //判断是否够等级
    if(equips->level_require > user->level){
        printf("等级不够，不能穿戴装备\n");
        return -1;
    }
    //装备栏
    if(user->equipment[equips->slot] >0){
        //装备栏有东西了,将装备卸下
        int equips_status = Unslot(user,equips->slot);
        // 判断是否已经卸下
        if (equips_status == 0) {
            user->equipment[equips->slot] = equip_id;
            return 0;
        }
        else{
            printf("穿戴失败!\n");
        }
    } else{
        user->equipment[equips->slot] = equip_id;
        //将数据初始化，移除了装备带来的数据加成
        calc_battle_stats(user);
        //  重新计算战斗属性（移除装备加成）
        equip_sv_apply_stats(user);
        return 0;
    }
    
    return -1;

}

/*
    0 正常卸下装备
    1 输入数据异常
    2 背包已经满了
    3 装备槽没有装备
    4 装备槽的装备不存在物品表中
*/
//卸下装备槽
int Unslot(User *user,int slot){
    // 先将物品排序,预留函数位置

    //检验输入数据是否正常
    if(user == NULL || slot <0 || slot >=SLOT_MAX) return 1;
    //判断装备槽是否有装备
    if(user->equipment[slot] <=0){
        printf("该槽位没有装备\n");
        return 3;
    }
    //判断当前背包容量是否是极限
    if(user->inventory_count >=ITEM_MAX){
        printf("背包已经满了\n");
        return 2;
    }
    //检验装备槽的装备是否真正的存在
    ItemConfig * equips = item_mgr_get_by_id(user->equipment[slot]);
    if (!equips){
        printf("该装备不存在\n");
        return 4;
    }
    //将装备槽的装备存放到背包中
    user->invertory[user->inventory_count][0] = user->equipment[slot];
    user->invertory[user->inventory_count][1] = ONE;
    //更新背包数量
     user->inventory_count++;
    //将装备槽的装备标识位设置为-1
    user->equipment[slot] = -1;
    //将数据初始化，移除了装备带来的数据加成
    calc_battle_stats(user);
    //  重新计算战斗属性（移除装备加成）
    equip_sv_apply_stats(user);





    return 0;
}


//装备出售操作

//自动穿戴装备

//计算装备属性 数据会不断累加，bug
void equip_sv_apply_stats(User *user){
    if (!user) return;
    for(int i =0;i<SLOT_MAX;i++){
        // 获取装备id
        ItemConfig * item = item_mgr_get_by_id(user->equipment[i]);
        //判断是否是装备，装备是否存在
        if( !item && item->type!=ITEM_TYPE_EQUIPMENT) return ;
        user->attack += item->attack_bonus;
        user->defense += item->defense_bonus;
        user->speed += item->speed_bonus;
        user->magic_attack += item->magic_attack_bonus;
        user->magic_defense += item->magic_defense_bonus;
        user->max_hp += item->max_hp_bonus;
        user->max_mp += item->max_mp_bonus;
        user->crit_rate += item->crit_bonus;
        user->dodge_rate += item->dodge_bonus;
    }
}



int main(){
    init();
    return 0;
}