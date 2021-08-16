#include "map.h"
#include "ui.h"

namespace pushbox
{
Map::Map() :level_(1), hero_x_(0), hero_y_(0), is_need_repaint_(true)
{
    map_vec_.resize(20, std::vector<int>(20)); // ָ�������vector��С,֮�����ʹ���±귽ʽ����    
}

Map::~Map()
{

}

void Map::DrawMap(const int offset, const int chartlet_width, const int chartlet_height)
{
    if (!is_need_repaint_)
    {
        return;
    }

    // ���ݵ�ͼԪ�ؽ���Ӧ��ͼ���Ƶ���Ļ��
    for (int row = 0; row < 20; ++row)
    {
        for (int column = 0; column < 20; ++column)
        {
            int dst_x = offset + chartlet_width * column;
            int dst_y = 40 + chartlet_height * row;
            putimage(dst_x, dst_y, &map_element_image_[map_vec_[row][column]]);
        }
    }
    is_need_repaint_ = false;    // ÿ�λ�������Ϊfalse; ��ͼ�и���ʱ����Ϊtrue
}

int Map::GetHeroX()
{
    return hero_x_;
}

int Map::GetHeroY()
{
    return hero_y_;
}

void Map::SetHeroX(const int hero_x)
{
    this->hero_x_ = hero_x;
}

void Map::SetHeroY(const int hero_y)
{
    this->hero_y_ = hero_y;
}

void Map::SetIsNeedRepaint(const bool flag)
{
    this->is_need_repaint_ = flag;
}

void Map::UpdateMap(Map2DVector map_vec)
{
    this->map_vec_ = map_vec;
}

void Map::LoadMap(int level)
{    
    std::ifstream in_file;
    char file_name[32] = { 0 };
    sprintf_s(file_name, "map/%d.map", level);
    in_file.open(file_name, std::ios::in);
    if (!in_file.is_open())
    {
        char tips[32] = { 0 };
        sprintf_s(tips, "�򿪵�ͼ�ļ�%sʧ��!", file_name);
        UI::ShowTips(TipsArea::kTipsStatus, tips, 24, false);
        return;
    }

    this->level_ = level;
    int tmp_map_element[20 * 20] = { 0 };
    std::string line_content;
    int index = 0;
    while (std::getline(in_file, line_content))
    {
        int width = line_content.size();
        for (int i = 0; i < line_content.size(); ++i)
        {
            tmp_map_element[index] = line_content[i] - '0';
            index++;
        }
    }

    index = 0;
    for (int row = 0; row < 20; ++row)
    {
        for (int column = 0; column < 20; ++column)
        {
            map_vec_[row][column] = tmp_map_element[index];
            // ��¼Hero��ǰ��λ��
            if (map_vec_[row][column] == MapElement::kDown)
            {
                hero_x_ = column;
                hero_y_ = row;
            }
            index++;
        }
    }

    // 20210718 LoadMap�� ˢ�µ�ͼ
    is_need_repaint_ = true;
    char tips[32] = { 0 };
    sprintf_s(tips, "��%d��", level);
    UI::ShowTips(TipsArea::kTipsUpper, tips, 40, false, TipsType::kLevelOnly);
}

void Map::LoadMapElement()
{
    loadimage(map_element_image_ + 0, _T("PNG"), MAKEINTRESOURCE(IDB_SOIL));
    loadimage(map_element_image_ + 1, _T("PNG"), MAKEINTRESOURCE(IDB_WALLBLACK));
    loadimage(map_element_image_ + 2, _T("PNG"), MAKEINTRESOURCE(IDB_GRASS));
    loadimage(map_element_image_ + 3, _T("PNG"), MAKEINTRESOURCE(IDB_BOXBLUE));
    loadimage(map_element_image_ + 4, _T("PNG"), MAKEINTRESOURCE(IDB_ENDBLUE));
    loadimage(map_element_image_ + 5, _T("PNG"), MAKEINTRESOURCE(IDB_HERODOWN));
    loadimage(map_element_image_ + 6, _T("PNG"), MAKEINTRESOURCE(IDB_HEROLEFT));
    loadimage(map_element_image_ + 7, _T("PNG"), MAKEINTRESOURCE(IDB_HERORIGHT));
    loadimage(map_element_image_ + 8, _T("PNG"), MAKEINTRESOURCE(IDB_HEROUP));
    loadimage(map_element_image_ + 9, _T("PNG"), MAKEINTRESOURCE(IDB_BOXRED));
}

void Map::Init()
{
    LoadMapElement();
    LoadMap(1); // ��ʼ��ʱĬ�ϼ��ص�1�ص�ͼ
}

// �뱣֤����ͼ�ĳߴ�һ��,��ȡһ����ͼ��ȡ���Ⱥ͸߶�,��:40*40
int Map::GetChartletWidth()
{
    return map_element_image_[0].getwidth();
}

int Map::GetChartletHeight()
{
    return map_element_image_[0].getheight();
}

Map2DVector Map::GetMapVector()
{
    return map_vec_;
}

int Map::GetLevel()
{
    return level_;
}

void Map::SetLevel(const int level)
{
    this->level_ = level;
}

} // namespace pushbox