// ���ƣ���ͼ��
// ��Ȩ������ѧϰ
// ���ߣ�Sean (eppesh@163.com)
// ������VS2019;EasyX 2021
// ʱ�䣺2021/07/15
// ˵������ͼ������ص���,������ͼ������ˢ�µ�;�������Ͻ�Ϊ����ԭ��(0,0);


#ifndef _PUSH_BOX_MAP_H_
#define _PUSH_BOX_MAP_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <graphics.h>


#include "resource.h"


namespace pushbox
{
typedef std::vector<std::vector<int>> Map2DVector;

const int kMaxLevel = 50;               // ���ؿ���

// ��ͼ�и�Ԫ��
enum MapElement
{
	kSoil = 0,							// ��0��������ͼΧǽ����Ŀյء�
	kWall,								// ��1��������ͼ�еı�ԵΧǽ��
	kGrass,								// ��2��������ͼΧǽ���ڵĿյ�(��Ŀ���λ)��
	kBox,								// ��3��������ͼ�е����ӡ�
	kDestination,						// ��4��������ͼ�����ӵ�Ŀ���λ��
    kDown,								// ��5������С���沿����(������)��
    kLeft,								// ��6������С���沿����
    kRight,								// ��7������С���沿���ҡ�
    kUp,								// ��8������С���沿����(��������)��
    kReady								// ��9�����������Ѿ�����Ŀ���λ��	
};

class Map
{
public:
    Map();
    ~Map();
    void Init();                        // ��ʼ��    
    int GetHeroX();                     // ��ȡHero��ǰλ�õ�x����
    int GetHeroY();                     // ��ȡHero��ǰλ�õ�y����
    void SetHeroX(const int hero_x);    // ����Hero��ǰλ�õ�x����
    void SetHeroY(const int hero_y);    // ����Hero��ǰλ�õ�y����
    void SetLevel(const int level);     // ���ùؿ���
    int GetLevel();                     // ��ȡ�ؿ���
    int GetChartletWidth();             // ��ȡ��ͼ�Ŀ��
    int GetChartletHeight();            // ��ȡ��ͼ�ĸ߶�
    Map2DVector GetMapVector();         // ��ȡ��ͼ����vector
    void SetIsNeedRepaint(const bool flag);  // �����Ƿ���Ҫˢ�µ�ͼ
    void UpdateMap(Map2DVector map_vec);// ���µ�ͼ����

    // ���Ƶ�ͼ; offset��ʾ��ͼ���ҵ�ƫ����(�����˵�����Ŀ��);clartlet_width��ʾ��ͼ�Ŀ��;chartlet_height��ʾ��ͼ�ĸ߶�
    void DrawMap(const int offset, const int chartlet_width, const int chartlet_height);
    void LoadMap(int level);            // ����ָ���ؿ��ĵ�ͼ�ļ����ڴ�

private:    
    void LoadMapElement();              // ���ص�ͼԪ����Դ���ڴ�

private:
    int level_;                         // �ؿ�
    int hero_x_;                        // С��(Hero)��ǰλ�õ�x����(��Ļ����)(����ͼ��Ԫ��Ϊ5ʱ��λ��)
    int hero_y_;                        // С��(Hero)��ǰλ�õ�y����(��Ļ����)(����ͼ��Ԫ��Ϊ5ʱ��λ��)
    bool is_need_repaint_;              // �Ƿ���Ҫˢ�µ�ͼ
    IMAGE map_element_image_[10];       // ��ͼԪ�ض�Ӧ���ز�ͼƬ
    Map2DVector map_vec_;               // ��ͼ����vector���
};
}// namespace pushbox
#endif