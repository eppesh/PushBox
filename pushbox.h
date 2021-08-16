// ���ƣ���Ϸ������
// ��Ȩ������ѧϰ
// ���ߣ�Sean (eppesh@163.com)
// ������VS2019;EasyX 2021
// ʱ�䣺2021/07/15
// ˵������Ϸ������ص���,����С��(Hero)���ƶ�������


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
    kUpMovable = 0,                 // �Ϸ�����ƶ�
    kUpUnMovable,                   // �Ϸ��򲻿��ƶ�
    kDownMovable,                   // �·�����ƶ�
    kDownUnMovable,                 // �·��򲻿��ƶ�
    kLeftMovable,                   // ������ƶ�
    kLeftUnMovable,                 // ���򲻿��ƶ�
    kRightMovable,                  // �ҷ�����ƶ�
    kRightUnMovable                 // �ҷ��򲻿��ƶ�
};

const int kKeyArrowChar = 224;		// 0xE0, ���������ֵ��һ����224���ڶ�������ʵ�ʵķ������ֵ
const int kKeyArrowUp = 72;         // �����-Up��
const int kKeyArrowLeft = 75;       // �����-Left��
const int kKeyArrowRight = 77;      // �����-Right��
const int kKeyArrowDown = 80;       // �����-Down��

class PushBox
{
public:
    PushBox(Map &map_object);
    ~PushBox();
    void Play(const int offset, Map &map_object, Map2DVector &map_vector);      // ����Ϸ
    bool IsWin(Map2DVector &map_vector);                                        // ��ǰ�ؿ��Ƿ�ͨ��    
    void Run();

private:
    void UpdateCurrentPosition(const int hero_x, const int hero_y, Map2DVector &map_vector);   // ����Hero�ƶ���ǰλ�õĵ�ͼԪ��
    void MoveUp(Map &map_object, Map2DVector &map_vector);                  // �����ƶ�
    void MoveDown(Map &map_object, Map2DVector &map_vector);                // �����ƶ�
    void MoveLeft(Map &map_object, Map2DVector &map_vector);                // �����ƶ�
    void MoveRight(Map &map_object, Map2DVector &map_vector);               // �����ƶ�

    void UndoMoveUp(int move_type);              // ���������ƶ�
    void UndoMoveDown(int move_type);            // ���������ƶ�
    void UndoMoveLeft(int move_type);            // ���������ƶ�
    void UndoMoveRight(int move_type);           // ���������ƶ�

    void UpdateReferenceMap(Map2DVector &map_vec);                           // ���²ο���ͼ����
    void OnMouseClick(const MOUSEMSG mouse_msg);

    void ClearStack();                           // ���ջԪ��

private:
    bool is_move_enable;            // Hero�Ƿ�����ƶ�;ͨ�غ�Hero�����ƶ�(����Ӧ����)
    Map *map_object_;               // Map����
    int reference_map_[20][20];     // �ο���ͼ,���ڲ鿴ĳλ���ƶ�ǰ��Ԫ��;�����޸�����Ԫ��
    Map2DVector map_vector_;        // real_map_in_memory ʵ�ʲ���ʱ���ڴ��еĵ�ͼ����Map::map_vec_��һ�������������޸ĺ�ҲҪ���µ�Map::map_vec_�У�
    std::stack<int> move_stack_;    // ��¼Hero�ƶ���ջ,����"��һ��"����
};
} // namespace pushbox
#endif