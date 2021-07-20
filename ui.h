// 名称：游戏UI类
// 版权：仅供学习
// 作者：Sean (eppesh@163.com)
// 环境：VS2019;EasyX 2021
// 时间：2021/07/17
// 说明：游戏UI相关的类,包括主界面的绘制及界面上相关交互元素的绘制与事件响应;

#ifndef _PUSH_BOX_UI_H_
#define _PUSH_BOX_UI_H_

#include "pushbox.h"

namespace pushbox
{

// 提示信息所在区域
enum TipsArea
{
    kOtherArea = 0,         // 其他区域的提示信息
    kTipsUpper,             // 位于游戏区上方区域的提示信息
    kTipsStatus             // 位于状态栏(菜单栏及游戏区下方)的提示信息
};

// 提示信息类型
enum TipsType
{
    kOtherType = 0,         // 其他类型
    kLevelOnly,             // 仅提示关卡信息,形如"第8关"
    kLevelSuccess           // 提示通关成功信息,形如"恭喜你顺利闯过第8关"
};

class UI
{
public:
    UI(const int menu_area_width, const int tips_area_height, const int game_area_width, 
        const int game_area_height, const int status_area_height);
    ~UI();

    // 在游戏区域的上方显示关卡提示信息
    //static void ShowLevelTips(const int level);

    // tips_area 表示提示信息所在区域(目前支持状态栏区域和游戏区域上方区域,详见TipsArea)
    // level 表示关卡数,对应tips_area为kTipsUpper;默认为0,表示kTipsStatus区域
    // tips_type 表示提示信息类型(目前支持仅关卡信息和通关成功信息,详见TipsType)
    static void ShowTips(const int tips_area, char *tips, const int text_size, 
        const int tips_type = 0, const COLORREF text_color= RGB(6, 31, 62));

    // 在指定位置(dst_x,dst_y)处显示提示信息(tips),并设置颜色和字体等属性
    //static void ShowTips(int dst_x, int dst_y, char *tips, const COLORREF text_color, const int text_size);
    
private:
    // 绘制菜单区域
    void DrawMenu();

    // 将贴图背景设置为透明,并在指定位置(dst_x,dst_y)处显示图片
    void SetTransparent(IMAGE image, const int img_width, const int img_height,
        const int dst_x, const int dst_y);
};

} // namespace pushbox
#endif