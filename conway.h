#ifndef CONWAY_H
#define CONWAY_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void clearscreen(void);

typedef enum {
    Dead,
    Alive,
    Other,
    None,
} GridStatus;

typedef struct {
    // Grid在数组中的位置
    int x, y;
    // Grid的当前状态
    GridStatus Status;
} Grid;

typedef struct {
    // 行数
    uint16_t row;
    // 列数
    uint16_t column;
    // 用于存储所有Grid的二维数组
    Grid** grids;
} Conway;

// 创建一个Grid并为其分配内存
Grid* createGrid(int x, int y);
// 根据周围Grid状态更新
void updateStatus(Grid* grid, Conway* current, Conway* next);
// 在终端输出Grid的状态
void printStatus(Grid* grid);
// 创建一个Conway并为其分配内存
Conway* createConway(const uint16_t m, const uint16_t n);
// 释放一个Conwey指针
void freeConway(Conway *c);
// 随机地初始化格点
void initConway(Conway *c);
// 将Conway演化到下一世代，
void evolveConway(Conway **c);
// 在终端输出Conway状态
void printConway(Conway *c);
// 保存格点到文件（可能得考虑一下数据保存到文件的格式）
// 成功则返回0，失败返回非0值
int saveConway(const Conway *c, const char *path);
// 从文件读取格点
// 失败则Conway._grids = NULL
// 涉及malloc()
void loadConway(Conway *c, const char *path);
// 在终端输出帮助信息
void printHelp();
#endif
