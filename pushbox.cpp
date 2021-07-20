#include "PushBox.h"

namespace pushbox
{


// Test: printf current map
void PrintCurrentMap(const Map2DVector &map)
{
    //printf("Start---------------------%#x\n", &map);
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            printf("%d", map[i][j]);
        }
        printf("\n");
    }
    printf("---------------------End\n\n");
}

PushBox::PushBox(Map &map_object) :is_move_enable(true)
{
    /*Map2DVector map_vec = map_object.GetMapVector();
    UpdateReferenceMap(map_vec);*/
    this->map_object_ = &map_object;
    //printf("PushBox::map_vector_ before%#x\n", &map_vector_);
    this->map_vector_ = map_object.GetMapVector();
    //printf("PushBox::map_vector_ after%#x\n", &map_vector_);
    UpdateReferenceMap(this->map_vector_);

    /*this->map_object_ = map;
    int *data = map->GetMap();
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            this->map_data_[i][j] = data[20 * i + j];
        }
    }
    delete[]data;
    data = nullptr;*/
}

PushBox::~PushBox()
{
    /*if (map_object_ != nullptr)
    {
        delete map_object_;
        map_object_ = nullptr;
    }*/
}

void PushBox::Play(const int offset, Map &map_object, Map2DVector &map_vector)
{
    if (_kbhit() /*&& is_move_enable*/)		// ����а������£���_kbhit()����ֵ
    {
        //PrintCurrentMap(map_vector_); // ���ԣ��鿴�ƶ�ǰ��ͼ

        int pressed_key = _getch();
        switch (pressed_key)
        {
        case 0:
        case kKeyArrowChar:
            switch (_getch())
            {
            case kKeyArrowUp:
                MoveUp(map_object, map_vector_);
                break;
            case kKeyArrowDown:
                MoveDown(map_object, map_vector_);
                break;
            case kKeyArrowLeft:
                MoveLeft(map_object, map_vector_);
                break;
            case kKeyArrowRight:
                MoveRight(map_object, map_vector_);
                break;
            }
        }

        if (IsWin(map_vector_))
        {
            /*printf("-------IsWin------\n");
            for (int i = 0; i < 20; ++i)
            {
                for (int j = 0; j < 20; ++j)
                {
                    printf("%d", map_vector_[i][j]);
                }
                printf("\n");
            }
            printf("-------------------\n\n");*/

            is_move_enable = false;
            // TODO(Sean): ��ǰ�ؿ�ͨ����,������ʾ��(�������غ�ʱ,��ѯ���Ƿ�����һ��)
            // ͨ�غ�С�˾Ͳ������ƶ���
            map_object.DrawMap(offset, map_object.GetChartletWidth(), map_object.GetChartletHeight());
            //MessageBox(NULL, "�ѳɹ�ͨ��!", "Tip", MB_OK);

            //// ״̬����ʾ,TODO(Sean):�������֮ǰ����ʾ����
            //setfillcolor(RGB(183, 145, 106)); // ����Ϊ����ɫ
            //setlinecolor(RGB(183, 145, 106));
            //fillrectangle(0, 900, offset + 900, 930);

            int current_level = map_object.GetLevel();

            //TCHAR tips[64] = { 0 };
            //swprintf_s(tips, "��ϲ��˳��������%d�أ�", current_level);
            //settextcolor(RGB(6, 31, 62));
            //settextstyle(40, 0, "΢���ź�");
            ////outtextxy(20, 903, tips);
            //outtextxy(500, 30, tips);

            char tips[128] = { 0 };
            sprintf_s(tips, "��ϲ��˳��������%d�أ�", current_level);
            //UI::ShowTips(500, 0, tips, RGB(6, 31, 62), 40);
            UI::ShowTips(TipsArea::kTipsUpper, tips, 40, TipsType::kLevelSuccess);
            
            Sleep(3000);
            //map_object.SetIsNeedRepaint(false);      // ʹ��ͼˢ��
            //int msgbox_id = MessageBox(nullptr, "�Ƿ������һ�أ�", "ͨ����ʾ", MB_ICONQUESTION | MB_OKCANCEL);
            //if (msgbox_id == IDOK)
            //{                
            //    current_level++;
            //    map_object.SetLevel(current_level);
            //    map_object.LoadMap(current_level);
            //    map_object.SetIsNeedRepaint(true);      // ʹ��ͼˢ��
            //    is_move_enable = true;
            //}
            //else
            //{
            //    closegraph();
            //    exit(0);
            //}

            current_level++;
            //map_object.SetLevel(current_level);
            map_object.LoadMap(current_level);
            map_vector_ = map_object.GetMapVector(); // ���µ�ͼ���ݸ��µ���ǰ�ڴ���
            UpdateReferenceMap(map_vector_);         // ͬʱ���²ο���ͼ
            //map_object.SetIsNeedRepaint(true);      // ʹ��ͼˢ��
            //printf("***level[%d] hero(%d,%d)\n", current_level, map_object.GetHeroX(), map_object.GetHeroY());
        }
    }
}

bool PushBox::IsWin(Map2DVector &map_vector)
{
    bool flag = false;
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            // ������ͼ�ϸ�Ԫ��,����ͼ�ϻ���������һ��MapElementTmp::MapElement::kDestination,˵����δ����
            //if (map_data_[i][j] == MapElement::kDestination)
            /*if (map_vector[i][j] == MapElement::kDestination)
            {
                return false;
            }*/

            // ��Ե�3������������һ��9��С��ռ�ݣ�������Ϊͨ�ص�bug
            if (reference_map_[i][j] == 4 || reference_map_[i][j] == 9)
            {
                if (map_vector[i][j] == MapElement::kReady)
                {
                    flag = true;
                }
                else
                {
                    flag = false;
                    return flag;
                }
            }
        }
    }
    return flag;
}

void PushBox::UpdateCurrentPosition(const int hero_x, const int hero_y, Map2DVector &map_vector)
{
    // �����ǰλ��(���ƶ�ǰ��λ��,(x,y))��Ŀ���λMapElement::kDestination��MapElement::kReady,
    // ���ƶ���ǰλ��Ҫ�ָ���Ŀ���λMapElement::kDestination;����ָ��ɿյ�MapElement::kGrass
    // (Ϊ�λ���MapElement::kReady�� ��Ϊ���Ӽ�ʹ��ʱ����Ŀ���λ,Ҳ�п��ܼ����ƶ�,�ƶ����Ҫ�ָ���Ŀ���λ)
    if (reference_map_[hero_y][hero_x] == MapElement::kDestination || reference_map_[hero_y][hero_x] == MapElement::kReady)
    {
        map_vector[hero_y][hero_x] = MapElement::kDestination;
    }
    else
    {
        map_vector[hero_y][hero_x] = MapElement::kGrass;
    }
}

// ˼·�������ƶ�ʱֻ����"��"����һ�����ɡ�
// ���ж�Hero��ǰλ��(x,y)����һ��λ��(x,y-1)����ʲô��ͼԪ��;�Ƿ��������,�Ƿ������ӵ�;�ȸ��µ�ǰλ��,�������ƶ�;
void PushBox::MoveUp(Map &map_object, Map2DVector &map_vector)
{
    int hero_x = map_object.GetHeroX();
    int hero_y = map_object.GetHeroY();

    // �Ϸ�����MapElement::kGrass��MapElement::kReadyʱ��˵�����������ƶ�; ���θ����ƶ���ǰλ�õ�ͼԪ��,����"�Ϸ���"��ͼԪ��,����Hero��λ������
    if (map_vector[hero_y - 1][hero_x] == MapElement::kGrass || map_vector[hero_y - 1][hero_x] == MapElement::kDestination)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector);
        map_vector[hero_y - 1][hero_x] = MapElement::kUp;
        hero_y--;
    }
    else if (map_vector[hero_y - 1][hero_x] == MapElement::kBox || map_vector[hero_y - 1][hero_x] == MapElement::kReady)  // "�Ϸ���"��MapElement::kBox��MapElement::kReadyʱ,Ҫ���Ϸ����"�Ϸ���"�Ƿ�����ƶ�(�����ܷ��Ƶö�)
    {
        // ������ӵ�������Ŀ���λMapElement::kDestination
        if (map_vector[hero_y - 2][hero_x] == MapElement::kDestination)
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y - 1][hero_x] = MapElement::kUp;
            map_vector[hero_y - 2][hero_x] = MapElement::kReady;
            hero_y--;
        }
        else if (map_vector[hero_y - 2][hero_x] == MapElement::kGrass)   // ������ӵ������ǿյ�MapElement::kGrass
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y - 1][hero_x] = MapElement::kUp;
            map_vector[hero_y - 2][hero_x] = MapElement::kBox;
            hero_y--;
        }
        else    // �����ӵ������������ϰ���(MapElement::kWall/MapElement::kBox/MapElement::kReady)���޷��ƶ�
        {
            map_vector[hero_y][hero_x] = MapElement::kUp;
        }
    }
    else if (map_vector[hero_y - 1][hero_x] == MapElement::kWall)    // �Ϸ�����MapElement::kWallʱ�������ƶ�
    {
        map_vector[hero_y][hero_x] = MapElement::kUp;
    }
    map_object.SetHeroY(hero_y);            // ����С�˵�ǰ����
    map_object.UpdateMap(map_vector);       // ���µ�ͼ����
    map_object.SetIsNeedRepaint(true);      // ʹ��ͼˢ��
}

void PushBox::MoveDown(Map &map_object, Map2DVector &map_vector)
{
    int hero_x = map_object.GetHeroX();
    int hero_y = map_object.GetHeroY();

    // ����·�����MapElement::kGrass��MapElement::kDestination,����������ƶ�;��������Ӧ��ͼԪ��
    if (map_vector[hero_y + 1][hero_x] == MapElement::kGrass || map_vector[hero_y + 1][hero_x] == MapElement::kDestination)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector);
        map_vector[hero_y + 1][hero_x] = MapElement::kDown;
        hero_y++;
    }
    else if (map_vector[hero_y + 1][hero_x] == MapElement::kBox || map_vector[hero_y + 1][hero_x] == MapElement::kReady)		// ����·��������ӣ�MapElement::kBox��MapElement::kReady��,��Ҫ�����������λ���Ƿ�����ƶ�
    {
        // ������ӵ�������MapElement::kGrass�������ӿ��������ƶ�
        if (map_vector[hero_y + 2][hero_x] == MapElement::kGrass)
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y + 1][hero_x] = MapElement::kDown;
            map_vector[hero_y + 2][hero_x] = MapElement::kBox;
            hero_y++;
        }
        else if (map_vector[hero_y + 2][hero_x] == MapElement::kDestination)		// ������ӵ�������Ŀ���λ,�����ӿ��������ƶ�
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y + 1][hero_x] = MapElement::kDown;
            map_vector[hero_y + 2][hero_x] = MapElement::kReady;
            hero_y++;
        }
        else
        {
            map_vector[hero_y][hero_x] = MapElement::kDown;
        }
    }
    else if (map_vector[hero_y + 1][hero_x] == MapElement::kWall)		// ����·�����MapElement::kWall,���޷��ƶ�,��ͣ��ԭ��
    {
        map_vector[hero_y][hero_x] = MapElement::kDown;
    }
    map_object.SetHeroY(hero_y);           // ����С�˵�ǰ����
    map_object.UpdateMap(map_vector);       // ���µ�ͼ����
    map_object.SetIsNeedRepaint(true);     // ʹ��ͼˢ��
}

void PushBox::MoveLeft(Map &map_object, Map2DVector &map_vector)
{
    int hero_x = map_object.GetHeroX();
    int hero_y = map_object.GetHeroY();

    // ���������MapElement::kGrass��MapElement::kDestination,����������ƶ�;��������Ӧ��ͼԪ��
    if (map_vector[hero_y][hero_x - 1] == MapElement::kGrass || map_vector[hero_y][hero_x - 1] == MapElement::kDestination)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector);
        map_vector[hero_y][hero_x - 1] = MapElement::kLeft;
        hero_x--;
    }
    else if (map_vector[hero_y][hero_x - 1] == MapElement::kBox || map_vector[hero_y][hero_x - 1] == MapElement::kReady)		// ������������ӣ�MapElement::kBox��MapElement::kReady��,��Ҫ�ٿ�������ߵ�λ���Ƿ�����ƶ�
    {
        // ������ӵ������MapElement::kGrass�������ӿ��������ƶ�
        if (map_vector[hero_y][hero_x - 2] == MapElement::kGrass)
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y][hero_x - 1] = MapElement::kLeft;
            map_vector[hero_y][hero_x - 2] = MapElement::kBox;
            hero_x--;
        }
        else if (map_vector[hero_y][hero_x - 2] == MapElement::kDestination)		// ������ӵ������Ŀ���λ,�����ӿ��������ƶ�
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y][hero_x - 1] = MapElement::kLeft;
            map_vector[hero_y][hero_x - 2] = MapElement::kReady;
            hero_x--;
        }
        else
        {
            map_vector[hero_y][hero_x] = MapElement::kLeft;
        }
    }
    else if (map_vector[hero_y][hero_x - 1] == MapElement::kWall)		// ���������MapElement::kWall,���޷��ƶ�,��ͣ��ԭ��
    {
        map_vector[hero_y][hero_x] = MapElement::kLeft;
    }
    map_object.SetHeroX(hero_x);           // ����С�˵�ǰ����
    map_object.UpdateMap(map_vector);       // ���µ�ͼ����
    map_object.SetIsNeedRepaint(true);     // ʹ��ͼˢ��
}

void PushBox::MoveRight(Map &map_object, Map2DVector &map_vector)
{
    int hero_x = map_object.GetHeroX();
    int hero_y = map_object.GetHeroY();

    // ����ҷ�����MapElement::kGrass��MapElement::kDestination,����������ƶ�;��������Ӧ��ͼԪ��
    if (map_vector[hero_y][hero_x + 1] == MapElement::kGrass || map_vector[hero_y][hero_x + 1] == MapElement::kDestination)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector);
        map_vector[hero_y][hero_x + 1] = MapElement::kRight;
        hero_x++;
    }
    else if (map_vector[hero_y][hero_x + 1] == MapElement::kBox || map_vector[hero_y][hero_x + 1] == MapElement::kReady)		// ����ҷ��������ӣ�MapElement::kBox��MapElement::kReady��,��Ҫ�ٿ������ұߵ�λ���Ƿ�����ƶ�
    {
        // ������ӵ��ұ���MapElement::kGrass�������ӿ��������ƶ�
        if (map_vector[hero_y][hero_x + 2] == MapElement::kGrass)
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y][hero_x + 1] = MapElement::kRight;
            map_vector[hero_y][hero_x + 2] = MapElement::kBox;
            hero_x++;
        }
        else if (map_vector[hero_y][hero_x + 2] == MapElement::kDestination)		// ������ӵ��ұ���Ŀ���λ,�����ӿ��������ƶ�
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y][hero_x + 1] = MapElement::kRight;
            map_vector[hero_y][hero_x + 2] = MapElement::kReady;
            hero_x++;
        }
        else
        {
            map_vector[hero_y][hero_x] = MapElement::kRight;
        }
    }
    else if (map_vector[hero_y][hero_x + 1] == MapElement::kWall)		// ����ҷ�����MapElement::kWall,���޷��ƶ�,��ͣ��ԭ��
    {
        map_vector[hero_y][hero_x] = MapElement::kRight;
    }
    map_object.SetHeroX(hero_x);           // ����С�˵�ǰ����
    map_object.UpdateMap(map_vector);       // ���µ�ͼ����
    map_object.SetIsNeedRepaint(true);     // ʹ��ͼˢ��
}

void PushBox::UndoMoveUp()
{

}

void PushBox::UndoMoveDown()
{

}

void PushBox::UndoMoveLeft()
{

}

void PushBox::UndoMoveRight()
{

}

void PushBox::UpdateReferenceMap(Map2DVector &map_vec)
{
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            this->reference_map_[i][j] = map_vec[i][j];
        }
    }
}

void PushBox::OnMouseClick(const MOUSEMSG mouse_msg)
{
    int current_level = map_object_->GetLevel();
    switch (mouse_msg.uMsg)
    {
    case WM_LBUTTONDOWN:
        if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 200 && mouse_msg.y <= 200 + 60)
        {
            //printf("���¿�ʼ level:%d Click: %#x\n", current_level, map_object_);            
            printf("���¿�ʼ level:%d\n", current_level);
            map_object_->LoadMap(current_level); // LoadMap���Ѿ������ˢ�µ�ͼ����
            map_vector_ = map_object_->GetMapVector(); // ���µ�ͼ���ݸ��µ���ǰ�ڴ���

            //// ����outtextxy�����ʾ��Ϣ
            //char tips[128] = { 0 };
            //sprintf_s(tips, "��%d��", current_level);
            //UI::ShowTips(650, 0, tips, RGB(6, 31, 62), 40);

            //// ����drawtext�����ʾ��Ϣ
            char tips[128] = { 0 };
            sprintf_s(tips, "��%d��", current_level);
            //RECT rectangle = { 650, 0, 730, 40 }; // "�ڼ���"ռ��Լ80
            //settextcolor(RGB(6, 31, 62));
            //drawtext(tips, &rectangle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            UI::ShowTips(TipsArea::kTipsUpper, tips, 40, TipsType::kLevelOnly);
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 300 && mouse_msg.y <= 300 + 60)
        {
            printf("��һ��\n");
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 400 && mouse_msg.y <= 400 + 60)
        {
            printf("��һ��\n");
            if (current_level == 1)
            {

            }
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 500 && mouse_msg.y <= 500 + 60)
        {
            printf("��һ��\n");
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 600 && mouse_msg.y <= 600 + 60)
        {
            
            char input[16] = { 0 };
            InputBox(input, 16, "������ؿ���");
            unsigned int level = atoi(input);
            if (level == 0)
            {
                level = 1;
            }
            else if (level > kMaxLevel)
            {
                level = kMaxLevel;
            }
            map_object_->LoadMap(level); // LoadMap���Ѿ������ˢ�µ�ͼ����
            map_vector_ = map_object_->GetMapVector(); // ���µ�ͼ���ݸ��µ���ǰ�ڴ���
            UpdateReferenceMap(map_vector_);
            
            //// ����outtextxy�����ʾ��Ϣ
            //char tips[128] = { 0 };
            //sprintf_s(tips, "��%d��", level);
            //UI::ShowTips(650, 0, tips, RGB(6, 31, 62), 40);

            //// ����drawtext�����ʾ��Ϣ
            char tips[128] = { 0 };
            sprintf_s(tips, "��%d��", level);
            //RECT rectangle = { 650, 0, 730, 40 }; // "�ڼ���"ռ��Լ80
            //settextcolor(RGB(6, 31, 62));
            //drawtext(tips, &rectangle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            UI::ShowTips(TipsArea::kTipsUpper, tips, 40, TipsType::kLevelOnly);

            printf("ѡ��: ѡ���˵�%d��\n", level);
        }
        break;
    case WM_MOUSEMOVE:
        if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 200 && mouse_msg.y <= 200 + 60)
        {
            settextcolor(RGB(88, 88, 88));
            settextstyle(40, 15, "΢���ź�");
            outtextxy(68, 210, "���¿�ʼ");
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 300 && mouse_msg.y <= 300 + 60)
        {
            settextcolor(RGB(88, 88, 88));
            settextstyle(40, 15, "΢���ź�");
            outtextxy(84, 310, "��һ��");
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 400 && mouse_msg.y <= 400 + 60)
        {
            settextcolor(RGB(88, 88, 88));
            settextstyle(40, 15, "΢���ź�");
            outtextxy(84, 410, "��һ��");
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 500 && mouse_msg.y <= 500 + 60)
        {
            settextcolor(RGB(88, 88, 88));
            settextstyle(40, 15, "΢���ź�");
            outtextxy(84, 510, "��һ��");
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 600 && mouse_msg.y <= 600 + 60)
        {
            settextcolor(RGB(88, 88, 88));
            settextstyle(40, 15, "΢���ź�");
            outtextxy(98, 610, "ѡ��");
        }
        else
        {
            settextcolor(RGB(50, 50, 50));
            settextstyle(40, 15, "΢���ź�");
            outtextxy(68, 210, "���¿�ʼ");
            outtextxy(84, 310, "��һ��");
            outtextxy(84, 410, "��һ��");
            outtextxy(84, 510, "��һ��");
            outtextxy(98, 610, "ѡ��");
        }
        break;
    }
}

void PushBox::Run()
{
    MOUSEMSG mouse_msg;

    while (true)
    {
        mouse_msg = GetMouseMsg();
        OnMouseClick(mouse_msg);
        Sleep(5);
    }
}

} // namespace pushbox