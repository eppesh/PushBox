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
    if (_kbhit())		// ����а������£���_kbhit()����ֵ
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
            sprintf_s(tips, "��ϲ��˳��������%d�أ�", current_level);            
            UI::ShowTips(TipsArea::kTipsUpper, tips, 40, false, TipsType::kLevelSuccess);
            
            int msgbox_id = MessageBox(nullptr, "�Ƿ������һ�أ�", "ͨ����ʾ", MB_ICONQUESTION | MB_OKCANCEL);
            if (msgbox_id == IDOK)
            {                
                current_level++;
                map_object.LoadMap(current_level);          // ������һ�ص�ͼ��ˢ�µ�ͼ
                map_vector_ = map_object.GetMapVector();    // ���µ�ͼ���ݸ��µ���ǰ�ڴ���
                UpdateReferenceMap(map_vector_);            // ͬʱ���²ο���ͼ
                ClearStack();

                // TODO(Sean): ͨ�غ�ͨ���ص��������������������磺��¼��ʱ��ˢ����ͨ�ص�ͼ�ȹ��ܣ�
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
            // ͨ�����������á�������ͼ�ϸ�Ԫ��,����ͼ�ϻ���������һ��kDestination(9),˵����δ�������ķ�ʽ����Ϊ           
            // ��Ե�3������������һ��kDestination(9)��С��ռ�ݣ�������Ϊͨ�ص�bug
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
        move_stack_.push(kUpMovable);
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
            move_stack_.push(kUpUnMovable);
        }
        else if (map_vector[hero_y - 2][hero_x] == MapElement::kGrass)   // ������ӵ������ǿյ�MapElement::kGrass
        {
            UpdateCurrentPosition(hero_x, hero_y, map_vector);
            map_vector[hero_y - 1][hero_x] = MapElement::kUp;
            map_vector[hero_y - 2][hero_x] = MapElement::kBox;
            hero_y--;
            move_stack_.push(kUpUnMovable);
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
        move_stack_.push(kDownMovable);
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
            move_stack_.push(kDownUnMovable);
        }
        else if (map_vector[hero_y + 2][hero_x] == MapElement::kDestination)		// ������ӵ�������Ŀ���λ,�����ӿ��������ƶ�
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
        move_stack_.push(kLeftMovable);
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
            move_stack_.push(kLeftUnMovable);
        }
        else if (map_vector[hero_y][hero_x - 2] == MapElement::kDestination)		// ������ӵ������Ŀ���λ,�����ӿ��������ƶ�
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
        move_stack_.push(kRightMovable);
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
            move_stack_.push(kRightUnMovable);
        }
        else if (map_vector[hero_y][hero_x + 2] == MapElement::kDestination)		// ������ӵ��ұ���Ŀ���λ,�����ӿ��������ƶ�
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
    else if (map_vector[hero_y][hero_x + 1] == MapElement::kWall)		// ����ҷ�����MapElement::kWall,���޷��ƶ�,��ͣ��ԭ��
    {
        map_vector[hero_y][hero_x] = MapElement::kRight;
    }
    map_object.SetHeroX(hero_x);           // ����С�˵�ǰ����
    map_object.UpdateMap(map_vector);       // ���µ�ͼ����
    map_object.SetIsNeedRepaint(true);     // ʹ��ͼˢ��
}

// ˼·:�����ƶ�����һ����Ϊ�������»ָ�; 
// �����Ͽ��ƶ�,��Heroλ���Ϸ�λ��,�Ȼָ���Grass��Destination,�ٻָ�Hero����;
// �����ϲ����ƶ�,���Ϸ���������(Box��Ready),��Heroλ���м�λ��,�������ƶ�����,������֮ǰλ��;��ʱ
// ���Ƚ�����λ�ûָ���Destination��Grass,�ٽ��м�λ�ûָ���Box��Ready,���ָ�����λ�õ�Hero.
void PushBox::UndoMoveUp(int move_type)
{
    int hero_x = map_object_->GetHeroX();
    int hero_y = map_object_->GetHeroY();

    // �Ϸ�����ƶ�,���Ϸ���ΪGrass��Destinationʱ
    if (move_type == kUpMovable)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector_);
    }
    else if(move_type == kUpUnMovable)
    {
        // �Ƚ�������λ�ûָ���Destination��Grass
        if (reference_map_[hero_y - 1][hero_x] == MapElement::kDestination || reference_map_[hero_y - 1][hero_x] == MapElement::kReady)
        {
            map_vector_[hero_y - 1][hero_x] = MapElement::kDestination;
        }
        else
        {
            map_vector_[hero_y - 1][hero_x] = MapElement::kGrass;
        }

        // �ٽ��м�λ�ûָ���Box��Ready
        // ˼��������MapElement::kDestination�������?
        if (/*reference_map_[hero_y][hero_x] == MapElement::kDestination ||*/ reference_map_[hero_y][hero_x] == MapElement::kReady)
        {
            map_vector_[hero_y][hero_x] = MapElement::kReady;
        }
        else
        {
            map_vector_[hero_y][hero_x] = MapElement::kBox;
        }
    }
    // ���������λ�ûָ���ԭ����Hero״̬
    // TODO(Sean): ����ָ���Up,��һ���ƶ�ǰ��ΪDown,��ָ���Up������;���1�غ͵�10�ص�1��������ʱ;
    // ��������ǵ�һ�����ǵûָ���Up(�������������߼���,��һ����Hero����ʧ)
    if (move_stack_.size() == 1) // ���ֻ����һ��,��ָ���Down(���泯��),����ָ���Up(���泯��)
    {
        map_vector_[hero_y + 1][hero_x] = reference_map_[hero_y + 1][hero_x]; 
    }
    else
    {
        map_vector_[hero_y + 1][hero_x] = MapElement::kUp;
    }
    
    hero_y++;

    map_object_->SetHeroY(hero_y);            // ����С�˵�ǰ����
    map_object_->UpdateMap(map_vector_);      // ���µ�ͼ����
    map_object_->SetIsNeedRepaint(true);      // ʹ��ͼˢ��
}

void PushBox::UndoMoveDown(int move_type)
{
    int hero_x = map_object_->GetHeroX();
    int hero_y = map_object_->GetHeroY();

    // �·�����ƶ�,���·���ΪGrass��Destinationʱ
    if (move_type == kDownMovable)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector_);
    }
    else if (move_type == kDownUnMovable)
    {
        // �Ƚ�������λ�ûָ���Destination��Grass
        if (reference_map_[hero_y + 1][hero_x] == MapElement::kDestination || reference_map_[hero_y + 1][hero_x] == MapElement::kReady)
        {
            map_vector_[hero_y + 1][hero_x] = MapElement::kDestination;
        }
        else
        {
            map_vector_[hero_y + 1][hero_x] = MapElement::kGrass;
        }

        // �ٽ��м�λ�ûָ���Box��Ready
        // ˼��������MapElement::kDestination�������?
        if (/*reference_map_[hero_y][hero_x] == MapElement::kDestination ||*/ reference_map_[hero_y][hero_x] == MapElement::kReady)
        {
            map_vector_[hero_y][hero_x] = MapElement::kReady;
        }
        else
        {
            map_vector_[hero_y][hero_x] = MapElement::kBox;
        }
    }
    // ���������λ�ûָ��ɻָ���Down(���泯��)
    map_vector_[hero_y - 1][hero_x] = MapElement::kDown;
    
    hero_y--;

    map_object_->SetHeroY(hero_y);            // ����С�˵�ǰ����
    map_object_->UpdateMap(map_vector_);      // ���µ�ͼ����
    map_object_->SetIsNeedRepaint(true);      // ʹ��ͼˢ��
}

void PushBox::UndoMoveLeft(int move_type)
{
    int hero_x = map_object_->GetHeroX();
    int hero_y = map_object_->GetHeroY();

    // ������ƶ�,������ΪGrass��Destinationʱ
    if (move_type == kLeftMovable)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector_);
    }
    else if (move_type == kLeftUnMovable)
    {
        // �Ƚ�������λ�ûָ���Destination��Grass
        if (reference_map_[hero_y][hero_x - 1] == MapElement::kDestination || reference_map_[hero_y][hero_x - 1] == MapElement::kReady)
        {
            map_vector_[hero_y][hero_x - 1] = MapElement::kDestination;
        }
        else
        {
            map_vector_[hero_y][hero_x - 1] = MapElement::kGrass;
        }

        // �ٽ��м�λ�ûָ���Box��Ready
        // ˼��������MapElement::kDestination�������?
        if (/*reference_map_[hero_y][hero_x] == MapElement::kDestination ||*/ reference_map_[hero_y][hero_x] == MapElement::kReady)
        {
            map_vector_[hero_y][hero_x] = MapElement::kReady;
        }
        else
        {
            map_vector_[hero_y][hero_x] = MapElement::kBox;
        }
    }
    // ���������λ�ûָ���ԭ����Hero״̬;���ֻ����һ��,��ָ���Down(���泯��),����ָ���Left
    if (move_stack_.size() == 1)
    {
        map_vector_[hero_y][hero_x + 1] = reference_map_[hero_y][hero_x + 1];
    }
    else
    {
        map_vector_[hero_y][hero_x + 1] = MapElement::kLeft;
    }

    hero_x++;

    map_object_->SetHeroX(hero_x);            // ����С�˵�ǰ����
    map_object_->UpdateMap(map_vector_);      // ���µ�ͼ����
    map_object_->SetIsNeedRepaint(true);      // ʹ��ͼˢ��
}

void PushBox::UndoMoveRight(int move_type)
{
    int hero_x = map_object_->GetHeroX();
    int hero_y = map_object_->GetHeroY();

    // �ҷ�����ƶ�,���ҷ���ΪGrass��Destinationʱ
    if (move_type == kRightMovable)
    {
        UpdateCurrentPosition(hero_x, hero_y, map_vector_);
    }
    else if (move_type == kRightUnMovable)
    {
        // �Ƚ�������λ�ûָ���Destination��Grass
        if (reference_map_[hero_y][hero_x + 1] == MapElement::kDestination || reference_map_[hero_y][hero_x + 1] == MapElement::kReady)
        {
            map_vector_[hero_y][hero_x + 1] = MapElement::kDestination;
        }
        else
        {
            map_vector_[hero_y][hero_x + 1] = MapElement::kGrass;
        }

        // �ٽ��м�λ�ûָ���Box��Ready
        // ˼��������MapElement::kDestination�������?
        if (/*reference_map_[hero_y][hero_x] == MapElement::kDestination ||*/ reference_map_[hero_y][hero_x] == MapElement::kReady)
        {
            map_vector_[hero_y][hero_x] = MapElement::kReady;
        }
        else
        {
            map_vector_[hero_y][hero_x] = MapElement::kBox;
        }
    }
    // ���������λ�ûָ���ԭ����Hero״̬;���ֻ����һ��,��ָ���Down(���泯��),����ָ���Right
    if (move_stack_.size() == 1)
    {
        map_vector_[hero_y][hero_x - 1] = reference_map_[hero_y][hero_x - 1];
    }
    else
    {
        map_vector_[hero_y][hero_x - 1] = MapElement::kRight;
    }

    hero_x--;

    map_object_->SetHeroX(hero_x);            // ����С�˵�ǰ����
    map_object_->UpdateMap(map_vector_);      // ���µ�ͼ����
    map_object_->SetIsNeedRepaint(true);      // ʹ��ͼˢ��
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
        // TODO(Sean): ��Ҫ���漰�İ�ť�����������Ҫ�þ���ֵ��
    case WM_LBUTTONDOWN:
        if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 200 && mouse_msg.y <= 200 + 60)
        {
            map_object_->LoadMap(current_level);            // LoadMap���Ѿ������ˢ�µ�ͼ����
            map_vector_ = map_object_->GetMapVector();      // ���µ�ͼ���ݸ��µ���ǰ�ڴ���
            ClearStack();

            char tips[128] = { 0 };
            sprintf_s(tips, "��%d��", current_level);
            UI::ShowTips(TipsArea::kTipsUpper, tips, 40, false, TipsType::kLevelOnly);
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 300 && mouse_msg.y <= 300 + 60)
        {
            if (move_stack_.empty())
            {
                char tips[32] = { 0 };
                sprintf_s(tips, "����δ�����ƶ�,�޷�������һ��!");
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
                move_stack_.pop(); // ����Ϊ����֮����ɾ��ջ��Ԫ��
            }
        }
        else if (mouse_msg.x >= 47 && mouse_msg.x <= 47 + 163 && mouse_msg.y >= 400 && mouse_msg.y <= 400 + 60)
        {
            if (current_level == 1)
            {
                char tips[16] = { 0 };
                sprintf_s(tips, "�Ѿ��ǵ�1��!");
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
                sprintf_s(tips, "�Ѿ������һ��!");
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
            // ѡ��ؿ�����
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
            map_object_->LoadMap(level);                    // LoadMap���Ѿ������ˢ�µ�ͼ����
            map_vector_ = map_object_->GetMapVector();      // ���µ�ͼ���ݸ��µ���ǰ�ڴ���
            UpdateReferenceMap(map_vector_);
            ClearStack();
            
            char tips[128] = { 0 };
            sprintf_s(tips, "��%d��", level);
            UI::ShowTips(TipsArea::kTipsUpper, tips, 40, false, TipsType::kLevelOnly);
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

void PushBox::ClearStack()
{
    while (move_stack_.size() > 0)
    {
        move_stack_.pop();
    }
}

} // namespace pushbox