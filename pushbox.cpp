#include "PushBox.h"

namespace pushbox
{


// Test Function: print current map
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
    this->map_object_ = &map_object;
    this->map_vector_ = map_object.GetMapVector();
    UpdateReferenceMap(this->map_vector_);
}

PushBox::~PushBox()
{
}

void PushBox::Play(const int offset, Map &map_object, Map2DVector &map_vector)
{
    if (_kbhit())		// 如果有按键按下，则_kbhit()返回值
    {
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
            map_object.DrawMap(offset, map_object.GetChartletWidth(), map_object.GetChartletHeight());
            
            int current_level = map_object.GetLevel();

            char tips[128] = { 0 };
            sprintf_s(tips, "恭喜你顺利闯过第%d关！", current_level);            
            UI::ShowTips(TipsArea::kTipsUpper, tips, 40, false, TipsType::kLevelSuccess);
            
            int msgbox_id = MessageBox(nullptr, "是否继续下一关？", "通关提示", MB_ICONQUESTION | MB_OKCANCEL);
            if (msgbox_id == IDOK)
            {                
                current_level++;
                map_object.LoadMap(current_level);          // 加载下一关地图并刷新地图
                map_vector_ = map_object.GetMapVector();    // 将新地图数据更新到当前内存中
                UpdateReferenceMap(map_vector_);            // 同时更新参考地图
                ClearStack();

                // TODO(Sean): 通关后通过回调函数进行其他操作（如：记录耗时、刷新已通关地图等功能）
            }
            else
            {
                closegraph();
                exit(0);
            }
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
            // 通关条件不能用“遍历地图上各元素,若地图上还存在至少一个kDestination(9),说明还未结束”的方式，因为           
            // 针对第3关情况，若最后一个kDestination(9)被小人占据，会误认为通关的bug
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
        move_stack_.push(kUpMovable);
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
            move_stack_.push(kUpUnMovable);
        }
        else if (map_vector[hero_y - 2][hero_x] == MapElement::kGrass)   // 如果箱子的上面是空地MapElement::kGrass
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y - 1][hero_x] = MapElement::kUp;
            map_vector[hero_y - 2][hero_x] = MapElement::kBox;
            hero_y--;
            move_stack_.push(kUpUnMovable);
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
        move_stack_.push(kDownMovable);
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
            move_stack_.push(kDownUnMovable);
        }
        else if (map_vector[hero_y + 2][hero_x] == MapElement::kDestination)		// 如果箱子的下面是目标点位,则箱子可以向下移动
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y + 1][hero_x] = MapElement::kDown;
            map_vector[hero_y + 2][hero_x] = MapElement::kReady;
            hero_y++;
            move_stack_.push(kDownUnMovable);
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
        move_stack_.push(kLeftMovable);
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
            move_stack_.push(kLeftUnMovable);
        }
        else if (map_vector[hero_y][hero_x - 2] == MapElement::kDestination)		// 如果箱子的左边是目标点位,则箱子可以向左移动
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y][hero_x - 1] = MapElement::kLeft;
            map_vector[hero_y][hero_x - 2] = MapElement::kReady;
            hero_x--;
            move_stack_.push(kLeftUnMovable);
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
        move_stack_.push(kRightMovable);
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
            move_stack_.push(kRightUnMovable);
        }
        else if (map_vector[hero_y][hero_x + 2] == MapElement::kDestination)		// 如果箱子的右边是目标点位,则箱子可以向右移动
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y][hero_x + 1] = MapElement::kRight;
            map_vector[hero_y][hero_x + 2] = MapElement::kReady;
            hero_x++;
            move_stack_.push(kRightUnMovable);
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

// 思路:向上移动后，退一步即为由上往下恢复; 
// 若向上可移动,则Hero位于上方位置,先恢复成Grass或Destination,再恢复Hero即可;
// 若向上不可移动,即上方向是箱子(Box或Ready),则Hero位于中间位置,上面是推动箱子,下面是之前位置;此时
// 可先将上面位置恢复成Destination或Grass,再将中间位置恢复成Box或Ready,最后恢复下面位置的Hero.
void PushBox::UndoMoveUp(int move_type)
{
    int hero_x = map_object_->GetHeroX();
    int hero_y = map_object_->GetHeroY();

    // 上方向可移动,即上方向为Grass或Destination时
    if (move_type == kUpMovable)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector_);
    }
    else if(move_type == kUpUnMovable)
    {
        // 先将最上面位置恢复成Destination或Grass
        if (reference_map_[hero_y - 1][hero_x] == MapElement::kDestination || reference_map_[hero_y - 1][hero_x] == MapElement::kReady)
        {
            map_vector_[hero_y - 1][hero_x] = MapElement::kDestination;
        }
        else
        {
            map_vector_[hero_y - 1][hero_x] = MapElement::kGrass;
        }

        // 再将中间位置恢复成Box或Ready
        // 思考：会有MapElement::kDestination的情况吗?
        if (/*reference_map_[hero_y][hero_x] == MapElement::kDestination ||*/ reference_map_[hero_y][hero_x] == MapElement::kReady)
        {
            map_vector_[hero_y][hero_x] = MapElement::kReady;
        }
        else
        {
            map_vector_[hero_y][hero_x] = MapElement::kBox;
        }
    }
    // 最后将最下面位置恢复成原来的Hero状态
    // TODO(Sean): 如果恢复成Up,第一步移动前若为Down,则恢复成Up不合适;如第1关和第10关第1步向上走时;
    // 但如果不是第一步还是得恢复成Up(比如连续向上走几步,退一步后Hero会消失)
    if (move_stack_.size() == 1) // 如果只走了一步,则恢复成Down(正面朝下),否则恢复成Up(正面朝上)
    {
        map_vector_[hero_y + 1][hero_x] = reference_map_[hero_y + 1][hero_x]; 
    }
    else
    {
        map_vector_[hero_y + 1][hero_x] = MapElement::kUp;
    }
    
    hero_y++;

    map_object_->SetHeroY(hero_y);            // 更新小人当前坐标
    map_object_->UpdateMap(map_vector_);      // 更新地图数据
    map_object_->SetIsNeedRepaint(true);      // 使地图刷新
}

void PushBox::UndoMoveDown(int move_type)
{
    int hero_x = map_object_->GetHeroX();
    int hero_y = map_object_->GetHeroY();

    // 下方向可移动,即下方向为Grass或Destination时
    if (move_type == kDownMovable)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector_);
    }
    else if (move_type == kDownUnMovable)
    {
        // 先将最下面位置恢复成Destination或Grass
        if (reference_map_[hero_y + 1][hero_x] == MapElement::kDestination || reference_map_[hero_y + 1][hero_x] == MapElement::kReady)
        {
            map_vector_[hero_y + 1][hero_x] = MapElement::kDestination;
        }
        else
        {
            map_vector_[hero_y + 1][hero_x] = MapElement::kGrass;
        }

        // 再将中间位置恢复成Box或Ready
        // 思考：会有MapElement::kDestination的情况吗?
        if (/*reference_map_[hero_y][hero_x] == MapElement::kDestination ||*/ reference_map_[hero_y][hero_x] == MapElement::kReady)
        {
            map_vector_[hero_y][hero_x] = MapElement::kReady;
        }
        else
        {
            map_vector_[hero_y][hero_x] = MapElement::kBox;
        }
    }
    // 最后将最上面位置恢复成恢复成Down(正面朝下)
    map_vector_[hero_y - 1][hero_x] = MapElement::kDown;
    
    hero_y--;

    map_object_->SetHeroY(hero_y);            // 更新小人当前坐标
    map_object_->UpdateMap(map_vector_);      // 更新地图数据
    map_object_->SetIsNeedRepaint(true);      // 使地图刷新
}

void PushBox::UndoMoveLeft(int move_type)
{
    int hero_x = map_object_->GetHeroX();
    int hero_y = map_object_->GetHeroY();

    // 左方向可移动,即左方向为Grass或Destination时
    if (move_type == kLeftMovable)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector_);
    }
    else if (move_type == kLeftUnMovable)
    {
        // 先将最左面位置恢复成Destination或Grass
        if (reference_map_[hero_y][hero_x - 1] == MapElement::kDestination || reference_map_[hero_y][hero_x - 1] == MapElement::kReady)
        {
            map_vector_[hero_y][hero_x - 1] = MapElement::kDestination;
        }
        else
        {
            map_vector_[hero_y][hero_x - 1] = MapElement::kGrass;
        }

        // 再将中间位置恢复成Box或Ready
        // 思考：会有MapElement::kDestination的情况吗?
        if (/*reference_map_[hero_y][hero_x] == MapElement::kDestination ||*/ reference_map_[hero_y][hero_x] == MapElement::kReady)
        {
            map_vector_[hero_y][hero_x] = MapElement::kReady;
        }
        else
        {
            map_vector_[hero_y][hero_x] = MapElement::kBox;
        }
    }
    // 最后将最右面位置恢复成原来的Hero状态;如果只走了一步,则恢复成Down(正面朝下),否则恢复成Left
    if (move_stack_.size() == 1)
    {
        map_vector_[hero_y][hero_x + 1] = reference_map_[hero_y][hero_x + 1];
    }
    else
    {
        map_vector_[hero_y][hero_x + 1] = MapElement::kLeft;
    }

    hero_x++;

    map_object_->SetHeroX(hero_x);            // 更新小人当前坐标
    map_object_->UpdateMap(map_vector_);      // 更新地图数据
    map_object_->SetIsNeedRepaint(true);      // 使地图刷新
}

void PushBox::UndoMoveRight(int move_type)
{
    int hero_x = map_object_->GetHeroX();
    int hero_y = map_object_->GetHeroY();

    // 右方向可移动,即右方向为Grass或Destination时
    if (move_type == kRightMovable)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector_);
    }
    else if (move_type == kRightUnMovable)
    {
        // 先将最右面位置恢复成Destination或Grass
        if (reference_map_[hero_y][hero_x + 1] == MapElement::kDestination || reference_map_[hero_y][hero_x + 1] == MapElement::kReady)
        {
            map_vector_[hero_y][hero_x + 1] = MapElement::kDestination;
        }
        else
        {
            map_vector_[hero_y][hero_x + 1] = MapElement::kGrass;
        }

        // 再将中间位置恢复成Box或Ready
        // 思考：会有MapElement::kDestination的情况吗?
        if (/*reference_map_[hero_y][hero_x] == MapElement::kDestination ||*/ reference_map_[hero_y][hero_x] == MapElement::kReady)
        {
            map_vector_[hero_y][hero_x] = MapElement::kReady;
        }
        else
        {
            map_vector_[hero_y][hero_x] = MapElement::kBox;
        }
    }
    // 最后将最左面位置恢复成原来的Hero状态;如果只走了一步,则恢复成Down(正面朝下),否则恢复成Right
    if (move_stack_.size() == 1)
    {
        map_vector_[hero_y][hero_x - 1] = reference_map_[hero_y][hero_x - 1];
    }
    else
    {
        map_vector_[hero_y][hero_x - 1] = MapElement::kRight;
    }

    hero_x--;

    map_object_->SetHeroX(hero_x);            // 更新小人当前坐标
    map_object_->UpdateMap(map_vector_);      // 更新地图数据
    map_object_->SetIsNeedRepaint(true);      // 使地图刷新
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
        // TODO(Sean): 需要将涉及的按钮坐标更换（不要用绝对值）
    case WM_LBUTTONDOWN:
        if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 200 && mouse_msg.y <= 200 + 60)
        {
            map_object_->LoadMap(current_level);            // LoadMap中已经添加了刷新地图操作
            map_vector_ = map_object_->GetMapVector();      // 将新地图数据更新到当前内存中
            ClearStack();

            char tips[128] = { 0 };
            sprintf_s(tips, "第%d关", current_level);
            UI::ShowTips(TipsArea::kTipsUpper, tips, 40, false, TipsType::kLevelOnly);
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 300 && mouse_msg.y <= 300 + 60)
        {
            if (move_stack_.empty())
            {
                char tips[32] = { 0 };
                sprintf_s(tips, "您还未进行移动,无法返回上一步!");
                UI::ShowTips(TipsArea::kTipsStatus, tips, 24, true);
            }
            else
            {
                int result = move_stack_.top();
                switch (result)
                {
                case kUpMovable:
                    UndoMoveUp(result);
                    break;
                case kUpUnMovable:
                    UndoMoveUp(result);
                    break;
                case kDownMovable:
                    UndoMoveDown(result);
                    break;
                case kDownUnMovable:
                    UndoMoveDown(result);
                    break; 
                case kLeftMovable:
                    UndoMoveLeft(result);
                    break;
                case kLeftUnMovable:
                    UndoMoveLeft(result);
                    break; 
                case kRightMovable:
                    UndoMoveRight(result);
                    break;
                case kRightUnMovable:
                    UndoMoveRight(result);
                    break;
                }
                move_stack_.pop(); // 调整为回退之后再删除栈顶元素
            }
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 400 && mouse_msg.y <= 400 + 60)
        {
            if (current_level == 1)
            {
                char tips[16] = { 0 };
                sprintf_s(tips, "已经是第1关!");
                UI::ShowTips(TipsArea::kTipsStatus, tips, 24, true);
            }
            else
            {
                current_level--;
                map_object_->LoadMap(current_level);
                map_vector_ = map_object_->GetMapVector();
                UpdateReferenceMap(map_vector_);
                ClearStack();
            }
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 500 && mouse_msg.y <= 500 + 60)
        {
            if (current_level == kMaxLevel)
            {
                char tips[16] = { 0 };
                sprintf_s(tips, "已经是最后一关!");
                UI::ShowTips(TipsArea::kTipsStatus, tips, 24, true);
            }
            else
            {
                current_level++;
                map_object_->LoadMap(current_level);
                map_vector_ = map_object_->GetMapVector();
                UpdateReferenceMap(map_vector_);
                ClearStack();
            }
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 600 && mouse_msg.y <= 600 + 60)
        {
            // 选择关卡功能
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
            map_object_->LoadMap(level);                    // LoadMap中已经添加了刷新地图操作
            map_vector_ = map_object_->GetMapVector();      // 将新地图数据更新到当前内存中
            UpdateReferenceMap(map_vector_);
            ClearStack();
            
            char tips[128] = { 0 };
            sprintf_s(tips, "第%d关", level);
            UI::ShowTips(TipsArea::kTipsUpper, tips, 40, false, TipsType::kLevelOnly);
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

void PushBox::ClearStack()
{
    while (move_stack_.size() > 0)
    {
        move_stack_.pop();
    }
}

} // namespace pushbox