// 名称：游戏核心类
// 版权：仅供学习
// 作者：Sean (eppesh@163.com)
// 环境：VS2019;EasyX 2021
// 时间：2021/07/15
// 说明：游戏操作相关的类,包括小人(Hero)的移动操作等


#ifndef _PUSH_BOX_H_
#define _PUSH_BOX_H_

#include <conio.h>
#include <stack>
#include "map.h"
#include "ui.h"

namespace pushbox
{

enum MoveType
{
    kUpMovable = 0,                 // 上方向可移动
    kUpUnMovable,                   // 上方向不可移动
    kDownMovable,                   // 下方向可移动
    kDownUnMovable,                 // 下方向不可移动
    kLeftMovable,                   // 左方向可移动
    kLeftUnMovable,                 // 左方向不可移动
    kRightMovable,                  // 右方向可移动
    kRightUnMovable                 // 右方向不可移动
};

const int kKeyArrowChar = 224;		// 0xE0, 方向键返回值第一个是224，第二个才是实际的方向键的值
const int kKeyArrowUp = 72;         // 方向键-Up键
const int kKeyArrowLeft = 75;       // 方向键-Left键
const int kKeyArrowRight = 77;      // 方向键-Right键
const int kKeyArrowDown = 80;       // 方向键-Down键

class PushBox
{
public:
    PushBox(Map &map_object);
    ~PushBox();
    void Play(const int offset, Map &map_object, Map2DVector &map_vector);      // 玩游戏
    bool IsWin(Map2DVector &map_vector);                                        // 当前关卡是否通关    
    void Run();

private:
    void UpdateCurrentPosition(const int hero_x, const int hero_y, Map2DVector &map_vector);   // 更新Hero移动后当前位置的地图元素
    void MoveUp(Map &map_object, Map2DVector &map_vector);                  // 向上移动
    void MoveDown(Map &map_object, Map2DVector &map_vector);                // 向下移动
    void MoveLeft(Map &map_object, Map2DVector &map_vector);                // 向左移动
    void MoveRight(Map &map_object, Map2DVector &map_vector);               // 向右移动

    void UndoMoveUp(int move_type);              // 撤销向上移动
    void UndoMoveDown(int move_type);            // 撤销向下移动
    void UndoMoveLeft(int move_type);            // 撤销向左移动
    void UndoMoveRight(int move_type);           // 撤销向右移动

    void UpdateReferenceMap(Map2DVector &map_vec);                           // 更新参考地图数据
    void OnMouseClick(const MOUSEMSG mouse_msg);

    void ClearStack();                           // 清空栈元素

private:
    bool is_move_enable;            // Hero是否可以移动;通关后Hero不再移动(不响应按键)
    Map *map_object_;               // Map对象
    int reference_map_[20][20];     // 参考地图,用于查看某位置移动前的元素;不能修改其中元素
    Map2DVector map_vector_;        // real_map_in_memory 实际操作时的内存中的地图（是Map::map_vec_的一个副本，它有修改后也要更新到Map::map_vec_中）
    std::stack<int> move_stack_;    // 记录Hero移动的栈,用于"退一步"功能
};
} // namespace pushbox
#endif