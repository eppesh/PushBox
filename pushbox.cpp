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
    if (_kbhit() /*&& is_move_enable*/)		// 如果有按键按下，则_kbhit()返回值
    {
        //PrintCurrentMap(map_vector_); // 测试，查看移动前地图

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
            // TODO(Sean): 当前关卡通过后,弹出提示框(给出本关耗时,并询问是否开启下一关)
            // 通关后小人就不能再移动了
            map_object.DrawMap(offset, map_object.GetChartletWidth(), map_object.GetChartletHeight());
            //MessageBox(NULL, "已成功通关!", "Tip", MB_OK);

            //// 状态栏提示,TODO(Sean):须先清除之前的显示内容
            //setfillcolor(RGB(183, 145, 106)); // 设置为背景色
            //setlinecolor(RGB(183, 145, 106));
            //fillrectangle(0, 900, offset + 900, 930);

            int current_level = map_object.GetLevel();

            //TCHAR tips[64] = { 0 };
            //swprintf_s(tips, "恭喜你顺利闯过第%d关！", current_level);
            //settextcolor(RGB(6, 31, 62));
            //settextstyle(40, 0, "微软雅黑");
            ////outtextxy(20, 903, tips);
            //outtextxy(500, 30, tips);

            char tips[128] = { 0 };
            sprintf_s(tips, "恭喜你顺利闯过第%d关！", current_level);
            //UI::ShowTips(500, 0, tips, RGB(6, 31, 62), 40);
            UI::ShowTips(TipsArea::kTipsUpper, tips, 40, TipsType::kLevelSuccess);
            
            Sleep(3000);
            //map_object.SetIsNeedRepaint(false);      // 使地图刷新
            //int msgbox_id = MessageBox(nullptr, "是否继续下一关？", "通关提示", MB_ICONQUESTION | MB_OKCANCEL);
            //if (msgbox_id == IDOK)
            //{                
            //    current_level++;
            //    map_object.SetLevel(current_level);
            //    map_object.LoadMap(current_level);
            //    map_object.SetIsNeedRepaint(true);      // 使地图刷新
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
            map_vector_ = map_object.GetMapVector(); // 将新地图数据更新到当前内存中
            UpdateReferenceMap(map_vector_);         // 同时更新参考地图
            //map_object.SetIsNeedRepaint(true);      // 使地图刷新
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
            // 遍历地图上各元素,若地图上还存在至少一个MapElementTmp::MapElement::kDestination,说明还未结束
            //if (map_data_[i][j] == MapElement::kDestination)
            /*if (map_vector[i][j] == MapElement::kDestination)
            {
                return false;
            }*/

            // 针对第3关情况，若最后一个9被小人占据，会误认为通关的bug
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
    // 如果当前位置(即移动前的位置,(x,y))是目标点位MapElement::kDestination或MapElement::kReady,
    // 则移动后当前位置要恢复成目标点位MapElement::kDestination;否则恢复成空地MapElement::kGrass
    // (为何会是MapElement::kReady？ 因为箱子即使暂时到了目标点位,也有可能继续移动,移动后就要恢复成目标点位)
    if (reference_map_[hero_y][hero_x] == MapElement::kDestination || reference_map_[hero_y][hero_x] == MapElement::kReady)
    {
        map_vector[hero_y][hero_x] = MapElement::kDestination;
    }
    else
    {
        map_vector[hero_y][hero_x] = MapElement::kGrass;
    }
}

// 思路：向上移动时只考虑"上"方向一步即可。
// 先判断Hero当前位置(x,y)的上一个位置(x,y-1)处是什么地图元素;是否可以行走,是否有箱子等;先更新当前位置,再向上移动;
void PushBox::MoveUp(Map &map_object, Map2DVector &map_vector)
{
    int hero_x = map_object.GetHeroX();
    int hero_y = map_object.GetHeroY();

    // 上方向是MapElement::kGrass或MapElement::kReady时，说明可以向上移动; 依次更新移动后当前位置地图元素,更新"上方向"地图元素,更新Hero的位置坐标
    if (map_vector[hero_y - 1][hero_x] == MapElement::kGrass || map_vector[hero_y - 1][hero_x] == MapElement::kDestination)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector);
        map_vector[hero_y - 1][hero_x] = MapElement::kUp;
        hero_y--;
    }
    else if (map_vector[hero_y - 1][hero_x] == MapElement::kBox || map_vector[hero_y - 1][hero_x] == MapElement::kReady)  // "上方向"是MapElement::kBox或MapElement::kReady时,要看上方向的"上方向"是否可以移动(箱子能否推得动)
    {
        // 如果箱子的上面是目标点位MapElement::kDestination
        if (map_vector[hero_y - 2][hero_x] == MapElement::kDestination)
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y - 1][hero_x] = MapElement::kUp;
            map_vector[hero_y - 2][hero_x] = MapElement::kReady;
            hero_y--;
        }
        else if (map_vector[hero_y - 2][hero_x] == MapElement::kGrass)   // 如果箱子的上面是空地MapElement::kGrass
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y - 1][hero_x] = MapElement::kUp;
            map_vector[hero_y - 2][hero_x] = MapElement::kBox;
            hero_y--;
        }
        else    // 若箱子的上面是其他障碍物(MapElement::kWall/MapElement::kBox/MapElement::kReady)则无法移动
        {
            map_vector[hero_y][hero_x] = MapElement::kUp;
        }
    }
    else if (map_vector[hero_y - 1][hero_x] == MapElement::kWall)    // 上方向是MapElement::kWall时，不可移动
    {
        map_vector[hero_y][hero_x] = MapElement::kUp;
    }
    map_object.SetHeroY(hero_y);            // 更新小人当前坐标
    map_object.UpdateMap(map_vector);       // 更新地图数据
    map_object.SetIsNeedRepaint(true);      // 使地图刷新
}

void PushBox::MoveDown(Map &map_object, Map2DVector &map_vector)
{
    int hero_x = map_object.GetHeroX();
    int hero_y = map_object.GetHeroY();

    // 如果下方向是MapElement::kGrass或MapElement::kDestination,则可以向下移动;并更新相应地图元素
    if (map_vector[hero_y + 1][hero_x] == MapElement::kGrass || map_vector[hero_y + 1][hero_x] == MapElement::kDestination)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector);
        map_vector[hero_y + 1][hero_x] = MapElement::kDown;
        hero_y++;
    }
    else if (map_vector[hero_y + 1][hero_x] == MapElement::kBox || map_vector[hero_y + 1][hero_x] == MapElement::kReady)		// 如果下方向是箱子（MapElement::kBox或MapElement::kReady）,则要看箱子下面的位置是否可以移动
    {
        // 如果箱子的下面是MapElement::kGrass，则箱子可以向下移动
        if (map_vector[hero_y + 2][hero_x] == MapElement::kGrass)
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y + 1][hero_x] = MapElement::kDown;
            map_vector[hero_y + 2][hero_x] = MapElement::kBox;
            hero_y++;
        }
        else if (map_vector[hero_y + 2][hero_x] == MapElement::kDestination)		// 如果箱子的下面是目标点位,则箱子可以向下移动
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
    else if (map_vector[hero_y + 1][hero_x] == MapElement::kWall)		// 如果下方向是MapElement::kWall,则无法移动,需停在原地
    {
        map_vector[hero_y][hero_x] = MapElement::kDown;
    }
    map_object.SetHeroY(hero_y);           // 更新小人当前坐标
    map_object.UpdateMap(map_vector);       // 更新地图数据
    map_object.SetIsNeedRepaint(true);     // 使地图刷新
}

void PushBox::MoveLeft(Map &map_object, Map2DVector &map_vector)
{
    int hero_x = map_object.GetHeroX();
    int hero_y = map_object.GetHeroY();

    // 如果左方向是MapElement::kGrass或MapElement::kDestination,则可以向左移动;并更新相应地图元素
    if (map_vector[hero_y][hero_x - 1] == MapElement::kGrass || map_vector[hero_y][hero_x - 1] == MapElement::kDestination)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector);
        map_vector[hero_y][hero_x - 1] = MapElement::kLeft;
        hero_x--;
    }
    else if (map_vector[hero_y][hero_x - 1] == MapElement::kBox || map_vector[hero_y][hero_x - 1] == MapElement::kReady)		// 如果左方向是箱子（MapElement::kBox或MapElement::kReady）,则要再看箱子左边的位置是否可以移动
    {
        // 如果箱子的左边是MapElement::kGrass，则箱子可以向左移动
        if (map_vector[hero_y][hero_x - 2] == MapElement::kGrass)
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y][hero_x - 1] = MapElement::kLeft;
            map_vector[hero_y][hero_x - 2] = MapElement::kBox;
            hero_x--;
        }
        else if (map_vector[hero_y][hero_x - 2] == MapElement::kDestination)		// 如果箱子的左边是目标点位,则箱子可以向左移动
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
    else if (map_vector[hero_y][hero_x - 1] == MapElement::kWall)		// 如果左方向是MapElement::kWall,则无法移动,需停在原地
    {
        map_vector[hero_y][hero_x] = MapElement::kLeft;
    }
    map_object.SetHeroX(hero_x);           // 更新小人当前坐标
    map_object.UpdateMap(map_vector);       // 更新地图数据
    map_object.SetIsNeedRepaint(true);     // 使地图刷新
}

void PushBox::MoveRight(Map &map_object, Map2DVector &map_vector)
{
    int hero_x = map_object.GetHeroX();
    int hero_y = map_object.GetHeroY();

    // 如果右方向是MapElement::kGrass或MapElement::kDestination,则可以向右移动;并更新相应地图元素
    if (map_vector[hero_y][hero_x + 1] == MapElement::kGrass || map_vector[hero_y][hero_x + 1] == MapElement::kDestination)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector);
        map_vector[hero_y][hero_x + 1] = MapElement::kRight;
        hero_x++;
    }
    else if (map_vector[hero_y][hero_x + 1] == MapElement::kBox || map_vector[hero_y][hero_x + 1] == MapElement::kReady)		// 如果右方向是箱子（MapElement::kBox或MapElement::kReady）,则要再看箱子右边的位置是否可以移动
    {
        // 如果箱子的右边是MapElement::kGrass，则箱子可以向右移动
        if (map_vector[hero_y][hero_x + 2] == MapElement::kGrass)
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y][hero_x + 1] = MapElement::kRight;
            map_vector[hero_y][hero_x + 2] = MapElement::kBox;
            hero_x++;
        }
        else if (map_vector[hero_y][hero_x + 2] == MapElement::kDestination)		// 如果箱子的右边是目标点位,则箱子可以向右移动
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
    else if (map_vector[hero_y][hero_x + 1] == MapElement::kWall)		// 如果右方向是MapElement::kWall,则无法移动,需停在原地
    {
        map_vector[hero_y][hero_x] = MapElement::kRight;
    }
    map_object.SetHeroX(hero_x);           // 更新小人当前坐标
    map_object.UpdateMap(map_vector);       // 更新地图数据
    map_object.SetIsNeedRepaint(true);     // 使地图刷新
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
            //printf("重新开始 level:%d Click: %#x\n", current_level, map_object_);            
            printf("重新开始 level:%d\n", current_level);
            map_object_->LoadMap(current_level); // LoadMap中已经添加了刷新地图操作
            map_vector_ = map_object_->GetMapVector(); // 将新地图数据更新到当前内存中

            //// 利用outtextxy输出提示信息
            //char tips[128] = { 0 };
            //sprintf_s(tips, "第%d关", current_level);
            //UI::ShowTips(650, 0, tips, RGB(6, 31, 62), 40);

            //// 利用drawtext输出提示信息
            char tips[128] = { 0 };
            sprintf_s(tips, "第%d关", current_level);
            //RECT rectangle = { 650, 0, 730, 40 }; // "第几关"占宽约80
            //settextcolor(RGB(6, 31, 62));
            //drawtext(tips, &rectangle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            UI::ShowTips(TipsArea::kTipsUpper, tips, 40, TipsType::kLevelOnly);
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 300 && mouse_msg.y <= 300 + 60)
        {
            printf("退一步\n");
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 400 && mouse_msg.y <= 400 + 60)
        {
            printf("上一关\n");
            if (current_level == 1)
            {

            }
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 500 && mouse_msg.y <= 500 + 60)
        {
            printf("下一关\n");
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 600 && mouse_msg.y <= 600 + 60)
        {
            
            char input[16] = { 0 };
            InputBox(input, 16, "请输入关卡数");
            unsigned int level = atoi(input);
            if (level == 0)
            {
                level = 1;
            }
            else if (level > kMaxLevel)
            {
                level = kMaxLevel;
            }
            map_object_->LoadMap(level); // LoadMap中已经添加了刷新地图操作
            map_vector_ = map_object_->GetMapVector(); // 将新地图数据更新到当前内存中
            UpdateReferenceMap(map_vector_);
            
            //// 利用outtextxy输出提示信息
            //char tips[128] = { 0 };
            //sprintf_s(tips, "第%d关", level);
            //UI::ShowTips(650, 0, tips, RGB(6, 31, 62), 40);

            //// 利用drawtext输出提示信息
            char tips[128] = { 0 };
            sprintf_s(tips, "第%d关", level);
            //RECT rectangle = { 650, 0, 730, 40 }; // "第几关"占宽约80
            //settextcolor(RGB(6, 31, 62));
            //drawtext(tips, &rectangle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            UI::ShowTips(TipsArea::kTipsUpper, tips, 40, TipsType::kLevelOnly);

            printf("选关: 选择了第%d关\n", level);
        }
        break;
    case WM_MOUSEMOVE:
        if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 200 && mouse_msg.y <= 200 + 60)
        {
            settextcolor(RGB(88, 88, 88));
            settextstyle(40, 15, "微软雅黑");
            outtextxy(68, 210, "重新开始");
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 300 && mouse_msg.y <= 300 + 60)
        {
            settextcolor(RGB(88, 88, 88));
            settextstyle(40, 15, "微软雅黑");
            outtextxy(84, 310, "退一步");
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 400 && mouse_msg.y <= 400 + 60)
        {
            settextcolor(RGB(88, 88, 88));
            settextstyle(40, 15, "微软雅黑");
            outtextxy(84, 410, "上一关");
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 500 && mouse_msg.y <= 500 + 60)
        {
            settextcolor(RGB(88, 88, 88));
            settextstyle(40, 15, "微软雅黑");
            outtextxy(84, 510, "下一关");
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 600 && mouse_msg.y <= 600 + 60)
        {
            settextcolor(RGB(88, 88, 88));
            settextstyle(40, 15, "微软雅黑");
            outtextxy(98, 610, "选关");
        }
        else
        {
            settextcolor(RGB(50, 50, 50));
            settextstyle(40, 15, "微软雅黑");
            outtextxy(68, 210, "重新开始");
            outtextxy(84, 310, "退一步");
            outtextxy(84, 410, "上一关");
            outtextxy(84, 510, "下一关");
            outtextxy(98, 610, "选关");
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