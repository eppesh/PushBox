#include "UI.h"

namespace pushbox
{
UI::UI(const int menu_area_width, const int tips_area_height, 
    const int game_area_width, const int game_area_height, const int status_area_height)
{
    initgraph(menu_area_width + game_area_width, tips_area_height + game_area_height + status_area_height/*, EW_SHOWCONSOLE*/);
    setbkmode(TRANSPARENT);
    setbkcolor(RGB(183, 145, 106));
    cleardevice();

    // ����������ָ������͡���ϸ����ɫ��λ��
    setlinestyle(PS_SOLID, 2);
    setlinecolor(LIGHTGRAY);
    line(menu_area_width, 0, menu_area_width, tips_area_height + game_area_height);
    line(0, tips_area_height + game_area_height, menu_area_width + game_area_width, tips_area_height + game_area_height);

    DrawMenu();

    char tips[128] = { 0 };
    sprintf_s(tips, "ף��˳��!");
    ShowTips(kTipsStatus, tips, 24, false);
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

    // �˴���ť��ͼ�ĳߴ���163*60
    SetTransparent(img_btn_replay, 163, 60, 47, 200);
    SetTransparent(img_btn_undo, 163, 60, 47, 300);
    SetTransparent(img_btn_previous, 163, 60, 47, 400);
    SetTransparent(img_btn_next, 163, 60, 47, 500);
    SetTransparent(img_btn_choose, 163, 60, 47, 600);

    // �����ļ�Ҫ�ŵ�SetTransparent֮��
    settextcolor(RGB(50, 50, 50));
    settextstyle(40, 15, "΢���ź�");
    outtextxy(68, 210, "���¿�ʼ");
    outtextxy(84, 310, "��һ��");
    outtextxy(84, 410, "��һ��");
    outtextxy(84, 510, "��һ��");
    outtextxy(98, 610, "ѡ��");
}

void UI::SetTransparent(IMAGE image, const int img_width, const int img_height, const int dst_x, const int dst_y)
{
    IMAGE mask(img_width, img_height);
    DWORD *p_memory_close = GetImageBuffer(&image);
    DWORD *p_memory_mask = GetImageBuffer(&mask);
    COLORREF mask_background = p_memory_close[0];
    for (int i = 0; i < img_width * img_height; ++i)
    {
        if (p_memory_close[i] <= 0x555555 && p_memory_close[i] >= 0)  // 0x555555 �����ɫ
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


void UI::ShowTips(const int tips_area, char *tips, const int text_size, const bool is_disappear, 
    const int tips_type /* = 0 */, const COLORREF text_color /* = RGB(6, 31, 62) */)
{
    // �ؿ���Ϊ0��ʾ����Ϸ���Ϸ��Ĺؿ���ʾ��Ϣ
    if (tips_area == TipsArea::kTipsUpper)
    {
        // �����ԭ�ȵ�����
        setfillcolor(RGB(183, 145, 106));
        setlinecolor(RGB(183, 145, 106));
        fillrectangle(257, 0, 256 + 900, 40); // 256:�˵�������; 900:��Ϸ������;40:�Ϸ���ʾ����߶�

        // ����ʾ��ʾ��Ϣ
        RECT rectangle;
        switch (tips_type)
        {
        case TipsType::kLevelOnly:
            rectangle = { 600, 0, 755, 40 }; // "�ڼ���"ռ��Լ80
            settextcolor(text_color);
            
            break;
        case TipsType::kLevelSuccess:
            rectangle = { 500, 0, 900, 40 }; 
            settextcolor(text_color);
            break;
        }
        settextstyle(text_size, 0, "΢���ź�");
        drawtext(tips, &rectangle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        if (is_disappear)
        {
            Sleep(2000);
            // ����ղ�չʾ������
            setfillcolor(RGB(183, 145, 106));
            setlinecolor(RGB(183, 145, 106));
            fillrectangle(257, 0, 256 + 900, 40); // 256:�˵�������; 900:��Ϸ������;40:�Ϸ���ʾ����߶�
        }
    }
    else if (tips_area == TipsArea::kTipsStatus)
    {
        // �����ԭ�ȵ�����
        setfillcolor(RGB(183, 145, 106)); 
        setlinecolor(RGB(183, 145, 106));
        fillrectangle(0, 40 + 900 + 2 , 256 + 900, 40 + 900 + 30); // 40:��Ϸ���Ϸ���ʾ��Ϣ�߶�; 256:�˵�������; 900:��Ϸ�����Ⱥ͸߶�;30:״̬��������

        // ����ʾ��ʾ��Ϣ
        RECT rectangle = { 0, 40 + 900 , 256 + 900, 40 + 900 + 30 };
        settextcolor(text_color);
        settextstyle(text_size, 0, "΢���ź�");
        drawtext(tips, &rectangle, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        if (is_disappear)
        {
            Sleep(2000);
            setfillcolor(RGB(183, 145, 106));
            setlinecolor(RGB(183, 145, 106));
            fillrectangle(0, 40 + 900 + 2, 256 + 900, 40 + 900 + 30); // 40:��Ϸ���Ϸ���ʾ��Ϣ�߶�; 256:�˵�������; 900:��Ϸ�����Ⱥ͸߶�;30:״̬��������
        }
    }
}

} // namespace pushbox