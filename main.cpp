// ���ƣ�������С��Ϸ
// ��Ȩ������ѧϰ
// ���ߣ�Sean (eppesh@163.com)
// ������VS2019;EasyX 2021
// ʱ�䣺2021/07/15
// ˵����ͨ�����������С�˽��������Ӷ��Ƶ�Ŀ���λ����ͨ��;


#include <graphics.h>
#include <conio.h>			// _getch()�õ�
#include <windows.h>        // MessageBox�õ�
#include <stdio.h>
#include <thread>

#include "resource.h"
#include "map.h"
#include "pushbox.h"
#include "ui.h"

using namespace pushbox;

int main()
{
    int menu_area_width = 256;			// �˵�������
    int game_area_width = 20 * 45;		// ��Ϸ������
    int game_area_height = 20 * 45;		// ��Ϸ����߶�
	int tips_area_height = 40;			// ��Ϸ�����Ϸ���ʾ��Ϣ����߶�
    int status_area_height = 30;	    // ״̬������߶�
	   
    UI main_ui(menu_area_width, tips_area_height, game_area_width, game_area_height, status_area_height);
    
	Map map_object;
	map_object.Init();
	Map2DVector map_vector = map_object.GetMapVector();
	PushBox pushbox(map_object);

    // ���̣߳���ӦMouseClick�¼������ڰ�ť��������
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