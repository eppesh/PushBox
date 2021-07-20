#include "UI.h"

namespace pushbox
{
UI::UI(const int menu_area_width, const int tips_area_height, 
    const int game_area_width, const int game_area_height, const int status_area_height)
{
    initgraph(menu_area_width + game_area_width, tips_area_height + game_area_height + status_area_height, EW_SHOWCONSOLE);
    setbkmode(TRANSPARENT);
    setbkcolor(RGB(183, 145, 106));
    cleardevice();

    // 设置主界面分隔线类型、粗细、颜色、位置
    setlinestyle(PS_SOLID, 2);
    setlinecolor(LIGHTGRAY);
    line(menu_area_width, 0, menu_area_width, tips_area_height + game_area_height);
    line(0, tips_area_height + game_area_height, menu_area_width + game_area_width, tips_area_height + game_area_height);

    DrawMenu();

    // 状态栏提示信息
    /*TCHAR tips[64] = { 0 };
    swprintf_s(tips, "祝你顺利！");
    settextcolor(RGB(6, 31, 62));
    settextstyle(24, 0, "微软雅黑");
    outtextxy(20, 903, tips);*/

    //char *tips = "祝你顺利！";
    char tips[128] = { 0 };
    sprintf_s(tips, "祝你顺利!");
    //ShowTips(20, tips_area_height + 903, tips, RGB(6, 31, 62), 24);
    ShowTips(kTipsStatus, tips, 24);
}

UI::~UI()
{
    closegraph();
}

void UI::DrawMenu()
{
    IMAGE img_btn_replay;
    IMAGE img_btn_undo;
    IMAGE img_btn_previous;
    IMAGE img_btn_next;
    IMAGE img_btn_choose;
    
    loadimage(&img_btn_replay, "PNG", MAKEINTRESOURCE(IDB_BTN_EMPTY));
    loadimage(&img_btn_undo, "PNG", MAKEINTRESOURCE(IDB_BTN_EMPTY));
    loadimage(&img_btn_previous, "PNG", MAKEINTRESOURCE(IDB_BTN_EMPTY));
    loadimage(&img_btn_next, "PNG", MAKEINTRESOURCE(IDB_BTN_EMPTY));
    loadimage(&img_btn_choose, "PNG", MAKEINTRESOURCE(IDB_BTN_EMPTY));

    // 此处按钮贴图的尺寸是163*60
    SetTransparent(img_btn_replay, 163, 60, 47, 200);
    SetTransparent(img_btn_undo, 163, 60, 47, 300);
    SetTransparent(img_btn_previous, 163, 60, 47, 400);
    SetTransparent(img_btn_next, 163, 60, 47, 500);
    SetTransparent(img_btn_choose, 163, 60, 47, 600);

    // 绘制文件要放到SetTransparent之后
    settextcolor(RGB(50, 50, 50));
    settextstyle(40, 15, "微软雅黑");
    outtextxy(68, 210, "重新开始");
    outtextxy(84, 310, "退一步");
    outtextxy(84, 410, "上一关");
    outtextxy(84, 510, "下一关");
    outtextxy(98, 610, "选关");
}

void UI::SetTransparent(IMAGE image, const int img_width, const int img_height, const int dst_x, const int dst_y)
{
    IMAGE mask(img_width, img_height);
    DWORD *p_memory_close = GetImageBuffer(&image);
    DWORD *p_memory_mask = GetImageBuffer(&mask);
    COLORREF mask_background = p_memory_close[0];
    for (int i = 0; i < img_width * img_height; ++i)
    {
        if (p_memory_close[i] <= 0x555555 && p_memory_close[i] >= 0)  // 0x555555 是深灰色
        {
            p_memory_mask[i] = WHITE;
        }
        else
        {
            p_memory_mask[i] = BLACK;
        }
    }
    BeginBatchDraw();
    putimage(dst_x, dst_y, &mask, SRCAND);
    putimage(dst_x, dst_y, &image, SRCPAINT);
    FlushBatchDraw();
    EndBatchDraw();
}

//void UI::ShowLevelTips(const int level)
//{
//    // 先清除原先的内容
//    setfillcolor(RGB(183, 145, 106));
//    setlinecolor(RGB(183, 145, 106));
//    fillrectangle(256, 0, 256 + 900, 40); // 256:菜单区域宽度; 900:游戏区域宽度;40:上方提示区域高度
//
//    // 利用drawtext输出提示信息
//    char tips[32] = { 0 };
//    sprintf_s(tips, "第%d关", level);
//    RECT rectangle = { 600, 0, 755, 40 }; // "第几关"占宽约80
//    settextcolor(RGB(6, 31, 62));
//    settextstyle(40, 0, "微软雅黑");
//    
//    drawtext(tips, &rectangle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//}

void UI::ShowTips(const int tips_area, char *tips, const int text_size, 
    const int tips_type /* = 0 */, const COLORREF text_color/* = RGB(6, 31, 62) */)
{
    // 关卡不为0表示是游戏区上方的关卡提示信息
    if (tips_area == TipsArea::kTipsUpper)
    {
        // 先清除原先的内容
        setfillcolor(RGB(183, 145, 106));
        setlinecolor(RGB(183, 145, 106));
        fillrectangle(257, 0, 256 + 900, 40); // 256:菜单区域宽度; 900:游戏区域宽度;40:上方提示区域高度

        // 再显示提示信息
        RECT rectangle;
        switch (tips_type)
        {
        case TipsType::kLevelOnly:        
            /*char tips_level[32] = { 0 };
            sprintf_s(tips_level, "第%d关", level);*/
            rectangle = { 600, 0, 755, 40 }; // "第几关"占宽约80
            settextcolor(text_color);
            
            break;
        case TipsType::kLevelSuccess:
            rectangle = { 500, 0, 900, 40 }; 
            //setlinecolor(RGB(255, 255, 255));
            settextcolor(text_color);
            /*settextstyle(40, 0, "微软雅黑");
            drawtext(tips, &rectangle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);*/
            break;
        }
        settextstyle(text_size, 0, "微软雅黑");
        drawtext(tips, &rectangle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    else if (tips_area == TipsArea::kTipsStatus)
    {
        // 先清除原先的内容
        setfillcolor(RGB(183, 145, 106)); 
        setlinecolor(RGB(183, 145, 106));
        fillrectangle(0, 40 + 900 + 2 , 256 + 900, 40 + 900 + 30); // 40:游戏区上方提示信息高度; 256:菜单区域宽度; 900:游戏区域宽度和高度;30:状态栏区域宽度

        // 再显示提示信息
        RECT rectangle = { 0, 40 + 900 , 256 + 900, 40 + 900 + 30 };
        settextcolor(text_color);
        settextstyle(text_size, 0, "微软雅黑");
        drawtext(tips, &rectangle, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }
}

//void UI::ShowTips(int dst_x, int dst_y, char *tips, const COLORREF text_color, const int text_size)
//{
//    // 先清空之前该区域的内容
//    //setfillcolor(text_color); // 设置为背景色
//    //fillrectangle(dst_x, dst_y, 900, 930);
//
//    settextcolor(text_color);
//    settextstyle(text_size, 0, "微软雅黑");
//    outtextxy(dst_x, dst_y, tips);
//}

} // namespace pushbox