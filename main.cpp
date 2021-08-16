// 名称：推箱子小游戏
// 版权：仅供学习
// 作者：Sean (eppesh@163.com)
// 环境：VS2019;EasyX 2021
// 时间：2021/07/15
// 说明：通过方向键操作小人将所有箱子都推到目标点位即可通关;


#include <graphics.h>
#include <conio.h>			// _getch()用到
#include <windows.h>        // MessageBox用到
#include <stdio.h>
#include <thread>

#include "resource.h"
#include "map.h"
#include "pushbox.h"
#include "ui.h"

using namespace pushbox;

int main()
{
    int menu_area_width = 256;			// 菜单区域宽度
    int game_area_width = 20 * 45;		// 游戏区域宽度
    int game_area_height = 20 * 45;		// 游戏区域高度
	int tips_area_height = 40;			// 游戏区域上方提示信息区域高度
    int status_area_height = 30;	    // 状态栏区域高度
	   
    UI main_ui(menu_area_width, tips_area_height, game_area_width, game_area_height, status_area_height);
    
	Map map_object;
	map_object.Init();
	Map2DVector map_vector = map_object.GetMapVector();
	PushBox pushbox(map_object);

    // 子线程：响应MouseClick事件，用于按钮单击功能
    std::thread thread_pushbox_mouseclick(&PushBox::Run, &pushbox);
    thread_pushbox_mouseclick.detach();
    
	while (true)
	{
		map_object.DrawMap(menu_area_width, map_object.GetChartletWidth(), map_object.GetChartletHeight());
		pushbox.Play(menu_area_width, map_object, map_vector);       
	}

	_getch();
	closegraph();    
	return 0;
}