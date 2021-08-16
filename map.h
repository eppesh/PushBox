// 名称：地图类
// 版权：仅供学习
// 作者：Sean (eppesh@163.com)
// 环境：VS2019;EasyX 2021
// 时间：2021/07/15
// 说明：地图操作相关的类,包括地图绘制与刷新等;窗体左上角为坐标原点(0,0);


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

const int kMaxLevel = 50;               // 最大关卡数

// 地图中各元素
enum MapElement
{
	kSoil = 0,							// “0”代表“地图围墙以外的空地”
	kWall,								// “1”代表“地图中的边缘围墙”
	kGrass,								// “2”代表“地图围墙以内的空地(非目标点位)”
	kBox,								// “3”代表“地图中的箱子”
	kDestination,						// “4”代表“地图中箱子的目标点位”
    kDown,								// “5”代表“小人面部朝下(即正面)”
    kLeft,								// “6”代表“小人面部朝左”
    kRight,								// “7”代表“小人面部朝右”
    kUp,								// “8”代表“小人面部朝上(即背着脸)”
    kReady								// “9”代表“箱子已经到达目标点位”	
};

class Map
{
public:
    Map();
    ~Map();
    void Init();                        // 初始化    
    int GetHeroX();                     // 获取Hero当前位置的x坐标
    int GetHeroY();                     // 获取Hero当前位置的y坐标
    void SetHeroX(const int hero_x);    // 设置Hero当前位置的x坐标
    void SetHeroY(const int hero_y);    // 设置Hero当前位置的y坐标
    void SetLevel(const int level);     // 设置关卡数
    int GetLevel();                     // 获取关卡数
    int GetChartletWidth();             // 获取贴图的宽度
    int GetChartletHeight();            // 获取贴图的高度
    Map2DVector GetMapVector();         // 获取地图数据vector
    void SetIsNeedRepaint(const bool flag);  // 设置是否需要刷新地图
    void UpdateMap(Map2DVector map_vec);// 更新地图数据

    // 绘制地图; offset表示地图向右的偏移量(即左侧菜单区域的宽度);clartlet_width表示贴图的宽度;chartlet_height表示贴图的高度
    void DrawMap(const int offset, const int chartlet_width, const int chartlet_height);
    void LoadMap(int level);            // 加载指定关卡的地图文件到内存

private:    
    void LoadMapElement();              // 加载地图元素资源到内存

private:
    int level_;                         // 关卡
    int hero_x_;                        // 小人(Hero)当前位置的x坐标(屏幕坐标)(即地图中元素为5时的位置)
    int hero_y_;                        // 小人(Hero)当前位置的y坐标(屏幕坐标)(即地图中元素为5时的位置)
    bool is_need_repaint_;              // 是否需要刷新地图
    IMAGE map_element_image_[10];       // 地图元素对应的素材图片
    Map2DVector map_vec_;               // 地图：用vector存放
};
}// namespace pushbox
#endif