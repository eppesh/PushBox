// ���ƣ���ϷUI��
// ��Ȩ������ѧϰ
// ���ߣ�Sean (eppesh@163.com)
// ������VS2019;EasyX 2021
// ʱ�䣺2021/07/17
// ˵������ϷUI��ص���,����������Ļ��Ƽ���������ؽ���Ԫ�صĻ������¼���Ӧ;

#ifndef _PUSH_BOX_UI_H_
#define _PUSH_BOX_UI_H_

#include "pushbox.h"

namespace pushbox
{

// ��ʾ��Ϣ��������
enum TipsArea
{
    kOtherArea = 0,         // �����������ʾ��Ϣ
    kTipsUpper,             // λ����Ϸ���Ϸ��������ʾ��Ϣ
    kTipsStatus             // λ��״̬��(�˵�������Ϸ���·�)����ʾ��Ϣ
};

// ��ʾ��Ϣ����
enum TipsType
{
    kOtherType = 0,         // ��������
    kLevelOnly,             // ����ʾ�ؿ���Ϣ,����"��8��"
    kLevelSuccess           // ��ʾͨ�سɹ���Ϣ,����"��ϲ��˳��������8��"
};

class UI
{
public:
    UI(const int menu_area_width, const int tips_area_height, const int game_area_width, 
        const int game_area_height, const int status_area_height);
    ~UI();

    // ����Ϸ������Ϸ���ʾ�ؿ���ʾ��Ϣ
    //static void ShowLevelTips(const int level);

    // tips_area ��ʾ��ʾ��Ϣ��������(Ŀǰ֧��״̬���������Ϸ�����Ϸ�����,���TipsArea)
    // level ��ʾ�ؿ���,��Ӧtips_areaΪkTipsUpper;Ĭ��Ϊ0,��ʾkTipsStatus����
    // tips_type ��ʾ��ʾ��Ϣ����(Ŀǰ֧�ֽ��ؿ���Ϣ��ͨ�سɹ���Ϣ,���TipsType)
    static void ShowTips(const int tips_area, char *tips, const int text_size, 
        const int tips_type = 0, const COLORREF text_color= RGB(6, 31, 62));

    // ��ָ��λ��(dst_x,dst_y)����ʾ��ʾ��Ϣ(tips),��������ɫ�����������
    //static void ShowTips(int dst_x, int dst_y, char *tips, const COLORREF text_color, const int text_size);
    
private:
    // ���Ʋ˵�����
    void DrawMenu();

    // ����ͼ��������Ϊ͸��,����ָ��λ��(dst_x,dst_y)����ʾͼƬ
    void SetTransparent(IMAGE image, const int img_width, const int img_height,
        const int dst_x, const int dst_y);
};

} // namespace pushbox
#endif