#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "cJSON.h"


/**
 * @brief 加载配置文件
 * @param path 配置文件路径
 * @return 0成功，-1失败
 */
int config_load();

/**
 * @brief 保存配置文件
 * @param path 配置文件路径
 * @return 0成功，-1失败
 */
int config_save(const char *path);

/**
 * @brief 初始化默认配置
 */
void config_init_default(void);

/**
 * @brief 获取整数配置
 */
int config_get_int(const char *key, int default_val);

/**
 * @brief 获取浮点数配置
 */
float config_get_float(const char *key, float default_val);

/**
 * @brief 获取字符串配置
 */
const char *config_get_string(const char *key, const char *default_val);

/**
 * @brief 关闭配置管理
 */
void config_close(void);
    
#endif