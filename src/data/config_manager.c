#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "config_manager.h"

static cJSON *g_config = NULL;

int config_load()
{
    /* ① 参数兜底，不重复声明 */
    char *path = "../config.json";

    FILE *fp = fopen(path, "rb");
    if (!fp) {
        LOGE("config_load: 打开失败: %s", path);
        return -1;
    }

    /* ④ 检查 ftell */
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    if (size < 0) {
        LOGE("config_load: ftell 失败");
        fclose(fp);
        return -1;
    }
    fseek(fp, 0, SEEK_SET);

    /* 空文件也挡一下 */
    if (size == 0) {
        LOGE("config_load: 文件为空: %s", path);
        fclose(fp);
        return -1;
    }

    char *buf = (char *)malloc((size_t)size + 1);
    if (!buf) {
        LOGE("config_load: malloc 失败 (%ld 字节)", size);
        fclose(fp);
        return -1;
    }

    /* ⑤ 检查 fread 返回值 */
    size_t got = fread(buf, 1, (size_t)size, fp);
    fclose(fp);                    /* ② 无论成败都关文件 */

    if (got != (size_t)size) {
        LOGE("config_load: 读取不完整 (%zu/%ld)", got, size);
        free(buf);
        return -1;
    }
    buf[got] = '\0';

    /* ⑥ 检查解析结果 */
    cJSON *parsed = cJSON_Parse(buf);
    free(buf);                     /* 解析完立即释放，不管成功失败 */

    if (!parsed) {
        LOGE("config_load: JSON 解析失败: %s", cJSON_GetErrorPtr());
        return -1;
    }

    /* ⑦ 释放旧配置，再赋新值 */
    if (g_config) {
        cJSON_Delete(g_config);
    }
    g_config = parsed;

    LOGI("config_load: 加载成功: %s (%ld 字节)", path, size);
    return 0;                      /* ③ 有明确返回值 */
}