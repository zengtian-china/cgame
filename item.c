#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "save.h"
#include "item.h"


//物品详情初始化
items *newitems(){
    items *item = malloc(sizeof(items));
    if (item ==NULL) return NULL;
    return item;
}

//物品详情列表初始化
itemList * newItemsList(){
    itemList *list = calloc(1,sizeof(*list));
    return list;
}
//角色物品详情列表初始化
myItemList* newMyItemList(){
    myItemList *mylist = calloc(1,sizeof(myItemList));
    if (mylist == NULL) return NULL;
    return mylist;
}

//物品详情列表获取
itemList* createItemsList(){
    //创建物品详情列表
    itemList *list = newItemsList();
    FILE *fp = fopen("./data/item.txt","r");
    int i =0;
  while (i < MAX_ITEMS && fscanf(fp, "%d|%[^|]|%[^|]|%d", 
            &list->array[i].id,
            list->array[i].item,
            list->array[i].effect,
            &list->array[i].price) == 4) 
    {
        // 只有读取成功才增加计数
        list->size += 1;
        i++;
    }
    fclose(fp);
    // printf("list->size = %d\n",list->size);
    return list;

}
void show_itemList(itemList *list){
        if (list == NULL || list->size == 0) {
        printf("物品列表为空。\n");
        return;
    }

    // 打印表格顶部边框
    printf("+------+----------------------+----------------------+----------+\n");
    // 打印表头
    printf("| %-4s | %-20s | %-20s | %-8s |\n", "ID", "物品", "效果", "数量");
    // 打印表头与数据之间的分隔线
    printf("+------+----------------------+----------------------+----------+\n");

    // 遍历列表并打印每一行
    for (int i = 0; i < list->size; i++) {
        // 使用固定的字段宽度来对齐各列
        // %-10d 表示左对齐，宽度为10的整数
        // %-20s 表示左对齐，宽度为20的字符串
        printf("| %-4d | %-20s | %-20s | %-8d |\n",
               list->array[i].id,
               list->array[i].item,
               list->array[i].effect,
               list->array[i].price);
    }

    // 打印表格底部边框
    printf("+------+----------------------+----------------------+----------+\n");
}
//根据物品id来查找对于的物品详情数据
items * find_items(int id,itemList *list){
    items *item = newitems();
    for(int i=0;i<list->size;i++){
        if(list->array[i].id == id){
            item->id = list->array[i].id;
            item->price = list->array[i].price;
            strcpy(item->item,list->array[i].item);
            strcpy(item->effect,list->array[i].effect);
            break;
        }
    }
    return item;
}

//角色物品详情列表获取
myItemList* getItemsList(User *user){
    myItemList* mylist = newMyItemList();
    itemList *list = createItemsList();
    for(int i=0;i<MAX_ITEMS;i++){
        if (user->invertory[i].item_id== 0) break;
        mylist->array[i].id = user->invertory[i].item_id;
        mylist->array[i].number = user->invertory[i].item_count;
        items * it= find_items(user->invertory[i].item_id,list);
        mylist->size +=1;
        strcpy(mylist->array[i].item,it->item);
        strcpy(mylist->array[i].effect,it->effect);
        free(it);
    }
    free(list);

    return mylist;
}



//背包展示
void printItemsList(myItemList *mylist) {
    if (mylist == NULL || mylist->size == 0) {
        printf("物品列表为空。\n");
        return;
    }

    // 打印表格顶部边框
    printf("+------+----------------------+----------------------+----------+\n");
    // 打印表头
    printf("| %-4s | %-20s | %-20s | %-8s |\n", "ID", "物品", "效果", "数量");
    // 打印表头与数据之间的分隔线
    printf("+------+----------------------+----------------------+----------+\n");

    // 遍历列表并打印每一行
    for (int i = 0; i < mylist->size; i++) {
        // 使用固定的字段宽度来对齐各列
        // %-10d 表示左对齐，宽度为10的整数
        // %-20s 表示左对齐，宽度为20的字符串
        printf("| %-4d | %-21s | %-21s | %-8d |\n",
               mylist->array[i].id,
               mylist->array[i].item,
               mylist->array[i].effect,
               mylist->array[i].number);
    }

    // 打印表格底部边框
    printf("+------+----------------------+----------------------+----------+\n");

}

int main(){
    User *user = read_save_main();
    //正常
    // itemList *list = createItemsList();
    // //list 详细
    // show_itemList(list);
    myItemList *mylist = getItemsList(user);
    printItemsList(mylist);
    // free(list);
    free(user);
    free(mylist);
}