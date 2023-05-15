#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <cstdlib>
#include <string>
#include <sstream>
#include <ctime>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#pragma comment(linker, "/STACK:268435456")
#pragma warning(disable : 4996)

using namespace sf;
using namespace std;

RenderWindow screen(VideoMode(1920, 1080), L"Casual", Style::Fullscreen);

class spr
{
public:
    Sprite selftexture;
    Image I;
    Texture T;

    void rebulid()
    {
        T.loadFromImage(I);
        selftexture.setTexture(T);
    }
    void set_alpha(int a)
    {
        selftexture.setColor(sf::Color(255, 255, 255, a));
    }
    void set_texture(string name, sf::Color key)
    {
        I.loadFromFile(name+".png");
        I.createMaskFromColor(key);
        T.loadFromImage(I);
        selftexture.setTexture(T);
    }
    void set_texture(string name)
    {
        I.loadFromFile(name + ".png");
        T.loadFromImage(I);
        selftexture.setTexture(T);
    }
    void set_origin(float x, float y)
    {
        selftexture.setOrigin(x, y);
    }
    void set_rotation(float a)
    {
        selftexture.setRotation(a);
    }
    void set_scale(float sx, float sy)
    {
        selftexture.setScale(sx, sy);
    }
    void draw(float x, float y)
    {
        selftexture.setPosition(x, y);
        screen.draw(selftexture);
    }
};
class borderC
{
public:
    float pos_x;
    float pos_y;
    float radius;
    void define(float x, float y, float r)
    {
        pos_x = x;
        pos_y = y;
        radius = r;
    }
    bool inborders(float px, float py)
    {
        return sqrt(pow(px - pos_x, 2) + pow(py - pos_y, 2)) <= radius;
    }
    bool inborders(borderC circul)
    {
        float b_pos_x, b_pos_y, b_radius;
        circul.get_pos(b_pos_x, b_pos_y, b_radius);
        return sqrt(pow(b_pos_x - pos_x, 2) + pow(b_pos_y - pos_y, 2)) <= (radius + b_radius);
    }
    borderC move(borderC C, float addx, float addy)
    {
        float b_pos_x, b_pos_y, b_radius;
        C.get_pos(b_pos_x, b_pos_y, b_radius);
        C.define(b_pos_x + addx, b_pos_y + addy, b_radius);
        return C;
    }
    void get_pos(float& x, float& y, float& r)
    {
        x = pos_x;
        y = pos_y;
        r = radius;
    }
};
class borderN
{
public:
    float sx;
    float sy;
    float ex;
    float ey;

    void define(float isx, float isy, float iex, float iey)
    {
        sx = isx;
        sy = isy;
        ex = iex;
        ey = iey;
    }
    bool inborders(float px, float py)
    {
        return (px >= sx && px <= ex && py >= sy && py <= ey);
    }
    bool inborders(borderN rect)
    {
        float b_sx, b_sy, b_ex, b_ey;
        rect.get_pos(b_sx, b_sy, b_ex, b_ey);

        return ((inborders(b_sx, b_sy) || inborders(b_sx, b_ey) || inborders(b_ex, b_sy) || inborders(b_ex, b_ey)) ||
            (rect.inborders(sx, sy) || rect.inborders(sx, ey) || rect.inborders(ex, sy) || rect.inborders(ex, ey)));
    }
    bool inborders(borderC circul)
    {
        float b_pos_x, b_pos_y, b_radius;
        circul.get_pos(b_pos_x, b_pos_y, b_radius);
        return (circul.inborders(sx, sy) || circul.inborders(sx, ey) || circul.inborders(ex, sy) || circul.inborders(ex, ey) ||
            inborders(b_pos_x - b_radius, b_pos_y) || inborders(b_pos_x + b_radius, b_pos_y) || inborders(b_pos_x, b_pos_y - b_radius) || inborders(b_pos_x, b_pos_y + b_radius));
    }
    borderN move(borderN C, float addx, float addy)
    {
        float b_sx, b_sy, b_ex, b_ey;
        C.get_pos(b_sx, b_sy, b_ex, b_ey);
        C.define(b_sx + addx, b_sy + addy, b_ex + addx, b_ey + addx);
        return C;
    }
    void get_pos(float& vsx, float& vsy, float& vex, float& vey)
    {
        vsx = sx;
        vsy = sy;
        vex = ex;
        vey = ey;
    }
};

class animB
{
public:
    int flames = 0;
    spr* anim;
    int sub_flames = 0;

    int curr_flame = 0;
    int curr_sub_flame = 0;
    bool opening = false;
    
    void set(string way, int f, int Sf)
    {
        flames = f;
        anim = new spr[flames];
        for (int i = 0; i < f; i++)
        {
            anim[i].set_texture(way+"_"+to_string(i+1));
        }
        sub_flames = Sf;
    }
    void update()
    {
        if (opening)
        {
            if (curr_flame <= flames - 2)
            {
                curr_sub_flame++;
                if (curr_sub_flame >= sub_flames)
                {
                    curr_sub_flame = 0;
                    curr_flame++;
                }
            }
        }
        else
        {
            if (curr_flame > 0)
            {
                curr_sub_flame++;
                if (curr_sub_flame >= sub_flames)
                {
                    curr_sub_flame = 0;
                    curr_flame--;
                }
            }
        }
    }
    void draw(int x, int y)
    {
        anim[curr_flame].draw(x, y);
    }
    void draw_flame(int x, int y, int f)
    {
        anim[f].draw(x, y);
    }
};
class animL
{
public:
    int flames = 0;
    spr* anim;
    int sub_flames = 0;

    int curr_flame = 0;
    int curr_sub_flame = 0;

    void set(string way, int f, int Sf)
    {
        flames = f;
        anim = new spr[flames];
        for (int i = 0; i < f; i++)
        {
            anim[i].set_texture(way + "_F" + to_string(i + 1));
        }
        sub_flames = Sf;
    }
    void update()
    {
            if (curr_flame <= flames - 2)
            {
                curr_sub_flame++;
                if (curr_sub_flame >= sub_flames)
                {
                    curr_sub_flame = 0;
                    curr_flame++;
                    if (curr_flame >= flames) curr_flame = 0;
                }
            }
    }
    void draw(int x, int y)
    {
        anim[curr_flame].draw(x, y);
    }
    void draw_flame(int x, int y, int f)
    {
        anim[f].draw(x, y);
    }
};
class animO
{
public:
    int flames = 0;
    spr* anim;
    int sub_flames = 0;

    int curr_flame = 0;
    int curr_sub_flame = 0;
    bool playing = false;

    void set(string way, int f, int Sf)
    {
        flames = f;
        anim = new spr[flames];
        for (int i = 0; i < f; i++)
        {
            anim[i].set_texture(way + "_" + to_string((i)/100) + to_string(((i)/10)%10) + to_string((i)%10%10));
        }
        sub_flames = Sf;
    }
    void update()
    {
        if (playing)
        if (curr_flame <= flames - 2)
        {
            curr_sub_flame++;
            if (curr_sub_flame >= sub_flames)
            {
                curr_sub_flame = 0;
                curr_flame++;
            }
        }
        else
        {
            playing = false;
        }
    }
    void draw(int x, int y)
    {
        anim[curr_flame].draw(x, y);
    }
    void play()
    {
        playing = true;
        curr_flame = 0;
        curr_sub_flame = 0;
    }
};

const float Pi = 3.14159;
const float G = 6.67 * pow(10, -2);

int ct = 64;

float* point_psx_1 = new float[ct];
float* point_psy_1 = new float[ct];
float* point_spx_1 = new float[ct];
float* point_spy_1 = new float[ct];

float* save_point_psx_1 = new float[ct];
float* save_point_psy_1 = new float[ct];
float* save_point_spx_1 = new float[ct];
float* save_point_spy_1 = new float[ct];

float* point_psx_2 = new float[ct];
float* point_psy_2 = new float[ct];
float* point_spx_2 = new float[ct];
float* point_spy_2 = new float[ct];

float* save_point_psx_2 = new float[ct];
float* save_point_psy_2 = new float[ct];
float* save_point_spx_2 = new float[ct];
float* save_point_spy_2 = new float[ct];

float* point_psx_3 = new float[ct];
float* point_psy_3 = new float[ct];
float* point_spx_3 = new float[ct];
float* point_spy_3 = new float[ct];

float* save_point_psx_3 = new float[ct];
float* save_point_psy_3 = new float[ct];
float* save_point_spx_3 = new float[ct];
float* save_point_spy_3 = new float[ct];

float* point_psx_4 = new float[ct];
float* point_psy_4 = new float[ct];
float* point_spx_4 = new float[ct];
float* point_spy_4 = new float[ct];

float* save_point_psx_4 = new float[ct];
float* save_point_psy_4 = new float[ct];
float* save_point_spx_4 = new float[ct];
float* save_point_spy_4 = new float[ct];

bool ct_drop = false;
bool ct_gt = false;
float ct_rize = 0.0;

int load_tick = 0;
int load_subtick = 0;
int record_time = 0;
int blackscreen_tick = 0;
int blackscreen_subtick = 0;

bool mainmenu_bool = true;
bool mainmenu_bool_r = false;
bool gamemenu_bool = false;
bool saper_gamebool = false;
bool saper_gamebool_r = false;

spr blackscreen;
spr menu_base;
spr menu_m_select_1;
spr menu_m_select_2;
borderN menu_m_1_border;
borderN menu_m_2_border;
spr menu_g_select_1;
spr menu_g_select_2;
spr menu_g_select_3;
spr menu_g_select_4;
spr menu_g_select_5;
spr menu_g_select_6;
borderN menu_g_1_border;
borderN menu_g_2_border;
borderN menu_g_3_border;
borderN menu_g_4_border;
borderN menu_g_5_border;
borderN menu_g_6_border;
spr saper_base;
spr saper_cells[13];
spr saper_button_exit[2];
spr saper_button_reset[2];
spr fone;
borderC saper_be;
borderC saper_br;
spr saper_number[10];
spr saper_mines_base;
animB saper_mines_32;
animB saper_mines_48;
animB saper_mines_64;
borderN saper_mines_32_b;
borderN saper_mines_48_b;
borderN saper_mines_64_b;
spr logo;
animO saper_expl;

int saper_time_record_32 = 59599;
int saper_time_record_48 = 59599;
int saper_time_record_64 = 59599;

void load_records()
{
    string buff, sm, var;
    ifstream fin("data/save/records.bin");
    while (true)
    {
        fin >> buff;
        if (buff == "end") break;
        fin >> sm >> var;
        if (buff == "record_32") saper_time_record_32 = stoi(var);
        if (buff == "record_48") saper_time_record_48 = stoi(var);
        if (buff == "record_64") saper_time_record_64 = stoi(var);
    }
}
void save_records()
{
    ofstream fout("data/save/records.bin", ios_base::trunc);
    fout << "record_32 = " + to_string(saper_time_record_32) << "\n";
    fout << "record_48 = " + to_string(saper_time_record_48) << "\n";
    fout << "record_64 = " + to_string(saper_time_record_64) << "\n";
    fout << "end\n";
    fout.close();
}

class saper_segm
{
public:
    bool open = false;
    bool mine = false;
    bool flag = false;
    int neight = 0;
};
saper_segm saper_map[24][16];

int bx, by;
bool saper_gameover_bool = false;
bool saper_win_bool = false;
bool saper_mapgen = false;

int saper_time = 0;
int saper_subtime = 0;
int saper_size = 48;

bool cheak_win()
{
    bool F = true;
    for (int i = 0; i < 24; i++)
        for (int j = 0; j < 16; j++)
        {
            F = F && ((!saper_map[i][j].mine && saper_map[i][j].open) || saper_map[i][j].mine);
    }
    return F;
}
int count_neight(int x, int y)
{
    int count = 0;
    if (y > 0 && saper_map[x][y - 1].mine) count += 1;
    if (x < 23 && y > 0 && saper_map[x+1][y - 1].mine) count += 1;
    if (x < 23 && saper_map[x + 1][y].mine) count += 1;
    if (x < 23 && y < 15 && saper_map[x + 1][y + 1].mine) count += 1;
    if (y < 15 && saper_map[x][y + 1].mine) count += 1;
    if (x > 0 && y < 15 && saper_map[x - 1][y + 1].mine) count += 1;
    if (x > 0 && saper_map[x - 1][y].mine) count += 1;
    if (x > 0 && y > 0 && saper_map[x - 1][y - 1].mine) count += 1;
    return count;
}
void saper_genmap(int px, int py, int size)
{
    int rx, ry;
    for (int k = 0; k < size; k++)
    {
        while (true)
        {
            rx = rand() % 24;
            ry = rand() % 16;
            bool F = false;
            if (!(rx == px && ry == py) && !saper_map[rx][ry].mine)
            {
                saper_map[rx][ry].mine = true;
                F = true;
            }
            if (F) break;
        }
    }
}
void open_cell(int px, int py)
{
    if (!saper_map[px][py].flag)
    {
        if (saper_map[px][py].mine)
        {
            bx = px;
            by = py;
            saper_gameover_bool = true;
            saper_expl.play();
        }
        else
        {
            saper_map[px][py].open = true;
            if (count_neight(px, py) == 0)
            {
                if (py > 0 && !saper_map[px][py - 1].open) open_cell(px, py - 1);
                if (px < 23 && py > 0 && !saper_map[px + 1][py - 1].open) open_cell(px + 1, py - 1);
                if (px < 23 && !saper_map[px + 1][py].open) open_cell(px + 1, py);
                if (px < 23 && py < 15 && !saper_map[px + 1][py + 1].open) open_cell(px + 1, py + 1);
                if (py < 15 && !saper_map[px][py + 1].open) open_cell(px, py + 1);
                if (px > 0 && py < 15 && !saper_map[px - 1][py + 1].open) open_cell(px - 1, py + 1);
                if (px > 0 && !saper_map[px - 1][py].mine) open_cell(px - 1, py);
                if (px > 0 && py > 0 && !saper_map[px - 1][py - 1].open) open_cell(px - 1, py - 1);
            }
        }
    }
}
void set_flag(int px, int py)
{
    if (!saper_map[px][py].open)  saper_map[px][py].flag = !(saper_map[px][py].flag);
}

void load()
{
    blackscreen.set_texture("data/texture/blackscreen");

    menu_base.set_texture("data/texture/menu_base", sf::Color(0, 0, 0, 255)); menu_base.set_alpha(128);
    menu_m_select_1.set_texture("data/texture/menu_m_select_1", sf::Color(0, 0, 0, 255));
    menu_m_select_2.set_texture("data/texture/menu_m_select_2", sf::Color(0, 0, 0, 255));

    menu_g_select_1.set_texture("data/texture/menu_g_select_1", sf::Color(0, 0, 0, 255));
    menu_g_select_2.set_texture("data/texture/menu_g_select_2", sf::Color(0, 0, 0, 255));
    menu_g_select_3.set_texture("data/texture/menu_g_select_3", sf::Color(0, 0, 0, 255));
    menu_g_select_4.set_texture("data/texture/menu_g_select_4", sf::Color(0, 0, 0, 255));
    menu_g_select_5.set_texture("data/texture/menu_g_select_5", sf::Color(0, 0, 0, 255));
    menu_g_select_6.set_texture("data/texture/menu_g_select_6", sf::Color(0, 0, 0, 255));

    saper_button_exit[0].set_texture("data/texture/saper_button_exit1");
    saper_button_exit[1].set_texture("data/texture/saper_button_exit2");

    saper_button_reset[0].set_texture("data/texture/saper_button_reset1");
    saper_button_reset[1].set_texture("data/texture/saper_button_reset2");

    saper_base.set_texture("data/texture/saper_base");

    for (int i = 0; i < 13; i++)
    {
        saper_cells[i].set_texture("data/texture/saper_segm_"+to_string(i+1));
    }

    fone.set_texture("data/texture/fone");

    for (int i = 0; i < 10; i++)
    {
        saper_number[i].set_texture("data/texture/number_" + to_string(i + 1));
    }

    saper_mines_32.set("data/texture/saper_button_32", 7, 2);
    saper_mines_48.set("data/texture/saper_button_48", 7, 2);
    saper_mines_64.set("data/texture/saper_button_64", 7, 2);

    saper_mines_base.set_texture("data/texture/saper_mines_base");

    logo.set_texture("data/texture/logo");

    saper_expl.set("data/texture/expl/expl_", 60, 1);
}
void predef()
{
    saper_be.define(111, 618, 67.5);
    saper_br.define(291, 618, 67.5);

    menu_m_1_border.define(782, 402, 1136, 518);
    menu_m_2_border.define(782, 561, 1136, 677);

    menu_g_1_border.define(782,  84, 1136, 200);
    menu_g_2_border.define(782, 243, 1136, 359);
    menu_g_3_border.define(782, 402, 1136, 518);
    menu_g_4_border.define(782, 561, 1136, 677);
    menu_g_5_border.define(782, 720, 1136, 836);
    menu_g_6_border.define(782, 879, 1136, 995);

    for (int i = 0; i < 24; i++)
        for (int j = 0; j < 16; j++)
        {
            saper_map[i][j] = saper_segm();
        }

    saper_mines_32_b.define( 30, 723, 135, 846);
    saper_mines_48_b.define(144, 723, 249, 846);
    saper_mines_64_b.define(258, 723, 363, 846);
}

int* find_close(int I, float* point_psx, float* point_psy)
{
    static int RT[2] = { 2048,2048 };
    for (int n = 0; n < 2; n++)
    {
        float min = 2048;
        int maxid = -1;
        for (int u = 0; u < ct; u++)
        {
            float r = sqrt(pow(point_psx[u] - point_psx[I], 2) + pow(point_psy[u] - point_psy[I], 2));
            if (r < min && u != I && (RT[0] != u || n == 0))
            {
                min = r;
                maxid = u;
            }
        }
        RT[n] = maxid;
    }
    return RT;
}
void fone_update(float* point_psx, float* point_psy, float* point_spx, float* point_spy)
{

    if (ct != 0) {
        for (int i = 0; i < ct; i++)
        {
            if (ct_drop) {
                point_spy[i] += abs(point_spy[i] * 0.05) + 0.01;
            }
            
            bool F = true;
            for (int i = 0; i < ct; i++) {
                if (point_psy[i] < 1080) F = false;
            }
            if (F) ct_drop = false;


            point_psx[i] += point_spx[i];
            if (point_psx[i] < 0 || point_psx[i] > 1920) point_spx[i] *= -1;
            point_psy[i] += point_spy[i];
            if (point_psy[i] < 0 || point_psy[i] > 1080) {
                if (!ct_drop) { point_spy[i] *= -1; }
                else { point_spy[i] = 0; }
            }
        }

        CircleShape shape(1);

        shape.setFillColor(Color(255, 255, 255));
        for (int i = 0; i < ct; i++)
        {
            int* close = find_close(i, point_psx, point_psy);
            if (0)
            {
                shape.setPosition(point_psx[i], point_psy[i]);
                screen.draw(shape);

                for (int j = 0; j < 2; j++)
                {
                    sf::Vertex line[] =
                    {
                        sf::Vertex(sf::Vector2f(point_psx[i], point_psy[i])),
                        sf::Vertex(sf::Vector2f(point_psx[close[j]], point_psy[close[j]]))
                    };
                    screen.draw(line, 2, sf::Lines);
                }
            }
            else {

                int ck1 = (((point_psy[i] + point_psy[close[0]] + point_psy[close[1]]) / 3) / (1080 / 100));
                int ck2 = (((point_psy[i] + point_psy[close[0]] + point_psy[close[1]]) / 3) / (1080 / 200));
                ConvexShape convex;
                convex.setFillColor(Color(100 - ck1 + 155 + 55, 200 - ck2 + 55, 200 - ck2 + 55, 70*ct_rize));
                convex.setPointCount(3);
                convex.setPoint(0, sf::Vector2f(point_psx[i], point_psy[i]));
                convex.setPoint(1, sf::Vector2f(point_psx[close[0]], point_psy[close[0]]));
                convex.setPoint(2, sf::Vector2f(point_psx[close[1]], point_psy[close[1]]));
                screen.draw(convex);
            }
        }
    }

}

void saper_update()
{
    Vector2f pos = screen.mapPixelToCoords(Mouse::getPosition(screen));

    saper_base.draw(0, 0);

    saper_button_exit[saper_be.inborders(pos.x,pos.y)].draw(45, 555);
    saper_button_reset[saper_br.inborders(pos.x, pos.y)].draw(225, 555);

    saper_mines_base.draw(24,735);
    if (saper_size == 32) saper_mines_32.opening = true; else saper_mines_32.opening = false;
    if (saper_size == 48) saper_mines_48.opening = true; else saper_mines_48.opening = false;
    if (saper_size == 64) saper_mines_64.opening = true; else saper_mines_64.opening = false;
    saper_mines_32.update(); saper_mines_32.draw(30, 723);
    saper_mines_48.update(); saper_mines_48.draw(144, 723);
    saper_mines_64.update(); saper_mines_64.draw(258, 723);
    
        for (int i = 0; i < 24; i++)
            for (int j = 0; j < 16; j++)
            {
                if (saper_map[i][j].open)
                {
                    if (count_neight(i, j) != 0) saper_cells[count_neight(i, j) - 1].draw(i * 60 + 420, j * 60 + 60);
                }
                else
                {
                    if (saper_map[i][j].flag) saper_cells[12].draw(i * 60 + 420, j * 60 + 60);
                    else saper_cells[11].draw(i * 60 + 420, j * 60 + 60);
                }
                if (saper_gameover_bool)
                {
                    if (i == bx && j == by)
                    {
                        saper_cells[9].draw(i * 60 + 420, j * 60 + 60);
                    }
                    else if (saper_map[i][j].mine) saper_cells[10].draw(i * 60 + 420, j * 60 + 60);
                }
            }

    saper_expl.update();
    saper_expl.draw(bx * 60 + 30 + 420 - 210, by * 60 + 30 + 60 - 198);
}
void update()
{
    Vector2f pos = screen.mapPixelToCoords(Mouse::getPosition(screen));

    fone.draw(0, 0);

    fone_update(point_psx_1, point_psy_1, point_spx_1, point_spy_1);
    fone_update(point_psx_2, point_psy_2, point_spx_2, point_spy_2);
    fone_update(point_psx_3, point_psy_3, point_spx_3, point_spy_3);
    fone_update(point_psx_4, point_psy_4, point_spx_4, point_spy_4);
    
    if (load_tick < 300)
    {
        load_subtick += 1;
        if (load_subtick >= 1)
        {
            load_tick += 1;
            load_subtick = 0;
        }
    }

    ct_rize = load_tick / 300.0;

    menu_base.draw(0, 0);
    logo.draw(681,66);

    if (saper_gamebool && saper_mapgen && !(saper_gameover_bool || saper_win_bool))
    {
        saper_subtime += 1;
        if (saper_subtime >= 6)
        {
            saper_time += 1;
            saper_subtime = 0;
        }
    }

    int b = saper_time % 600;
    int f1 = b / 100;
    int f2 = (b / 10) % 10;
    int f3 = b % 10;
    int c = saper_time / 600;
    int f4 = c / 10;
    int f5 = c % 10;

    int br, f1r, f2r, f3r, f4r, f5r, cr;
    if (saper_size == 32)
    {
        br = saper_time_record_32 % 600;
        f1r = br / 100;
        f2r = (br / 10) % 10;
        f3r = br % 10;
        cr = saper_time_record_32 / 600;
        f4r = cr / 10;
        f5r = cr % 10;
    }
    if (saper_size == 48)
    {
        br = saper_time_record_48 % 600;
        f1r = br / 100;
        f2r = (br / 10) % 10;
        f3r = br % 10;
        cr = saper_time_record_48 / 600;
        f4r = cr / 10;
        f5r = cr % 10;
    }
    if (saper_size == 64)
    {
        br = saper_time_record_64 % 600;
        f1r = br / 100;
        f2r = (br / 10) % 10;
        f3r = br % 10;
        cr = saper_time_record_64 / 600;
        f4r = cr / 10;
        f5r = cr % 10;
    }

    if (mainmenu_bool)
    {
        if (menu_m_1_border.inborders(pos.x, pos.y)) menu_m_select_1.set_alpha(255);
        else menu_m_select_1.set_alpha(128);
        menu_m_select_1.draw(0, 0);
        if (menu_m_2_border.inborders(pos.x, pos.y)) menu_m_select_2.set_alpha(255);
        else menu_m_select_2.set_alpha(128);
        menu_m_select_2.draw(0, 0);
    }
    if (gamemenu_bool)
    {
        if (menu_g_1_border.inborders(pos.x, pos.y)) menu_g_select_1.set_alpha(255);
        else menu_g_select_1.set_alpha(128);
        menu_g_select_1.draw(0, 0);
        if (menu_g_2_border.inborders(pos.x, pos.y)) menu_g_select_2.set_alpha(255);
        else menu_g_select_2.set_alpha(128);
        menu_g_select_2.draw(0, 0);
        if (menu_g_3_border.inborders(pos.x, pos.y)) menu_g_select_3.set_alpha(255);
        else menu_g_select_3.set_alpha(128);
        menu_g_select_3.draw(0, 0);
        if (menu_g_4_border.inborders(pos.x, pos.y)) menu_g_select_4.set_alpha(255);
        else menu_g_select_4.set_alpha(128);
        menu_g_select_4.draw(0, 0);
        if (menu_g_5_border.inborders(pos.x, pos.y)) menu_g_select_5.set_alpha(255);
        else menu_g_select_5.set_alpha(128);
        menu_g_select_5.draw(0, 0);
        if (menu_g_6_border.inborders(pos.x, pos.y)) menu_g_select_6.set_alpha(255);
        else menu_g_select_6.set_alpha(128);
        menu_g_select_6.draw(0, 0);
    }
    if (saper_gamebool)
    {
        saper_update();

        saper_number[f4].draw( 54, 405);
        saper_number[f5].draw(108, 405);
        saper_number[f1].draw(180, 405);
        saper_number[f2].draw(234, 405);
        saper_number[f3].draw(306, 405);

        saper_number[f4r].draw( 54, 165);
        saper_number[f5r].draw(108, 165);
        saper_number[f1r].draw(180, 165);
        saper_number[f2r].draw(234, 165);
        saper_number[f3r].draw(306, 165);
    }

    if (blackscreen_tick > 0)
    {
        blackscreen.set_alpha(255.0 - 255.0 * (abs(blackscreen_tick - 150) / 150.0));
        blackscreen.draw(0, 0);

        blackscreen_subtick += 1;
        if (blackscreen_subtick >= 1)
        {
            if (blackscreen_tick == 150)
            {
                if (saper_gamebool_r) {saper_gamebool = true; saper_gamebool_r = false;}
                if (mainmenu_bool_r) {
                    mainmenu_bool = true; mainmenu_bool_r = false; saper_gamebool = false;
                }
            }
            blackscreen_tick -= 1;
            blackscreen_subtick = 0;
        }

    }

}

int main()
{
    srand(time(NULL));

    bool F;
    for (int i = 0; i < ct; i++)
    {
        F = true;
        while (F)
        {
            point_psx_1[i] = rand() % 1920;
            point_psy_1[i] = rand() % 1080;
            point_spx_1[i] = (rand() % 1001 - 500) / 500.0;
            point_spy_1[i] = (rand() % 1001 - 500) / 500.0;
            if (point_spx_1[i] != 0 && point_spy_1[i] != 0)
            {
                F = false;
                save_point_psx_1[i] = point_psx_1[i];
                save_point_psy_1[i] = point_psy_1[i];
                save_point_spx_1[i] = point_spx_1[i];
                save_point_spy_1[i] = point_spy_1[i];
            }
        }
        F = true;
        while (F)
        {
            point_psx_2[i] = rand() % 1920;
            point_psy_2[i] = rand() % 1080;
            point_spx_2[i] = (rand() % 1001 - 500) / 500.0;
            point_spy_2[i] = (rand() % 1001 - 500) / 500.0;
            if (point_spx_2[i] != 0 && point_spy_2[i] != 0)
            {
                F = false;
                save_point_psx_2[i] = point_psx_2[i];
                save_point_psy_2[i] = point_psy_2[i];
                save_point_spx_2[i] = point_spx_2[i];
                save_point_spy_2[i] = point_spy_2[i];
            }
        }
        F = true;
        while (F)
        {
            point_psx_3[i] = rand() % 1920;
            point_psy_3[i] = rand() % 1080;
            point_spx_3[i] = (rand() % 1001 - 500) / 500.0;
            point_spy_3[i] = (rand() % 1001 - 500) / 500.0;
            if (point_spx_3[i] != 0 && point_spy_3[i] != 0)
            {
                F = false;
                save_point_psx_3[i] = point_psx_3[i];
                save_point_psy_3[i] = point_psy_3[i];
                save_point_spx_3[i] = point_spx_3[i];
                save_point_spy_3[i] = point_spy_3[i];
            }
        }
        F = true;
        while (F)
        {
            point_psx_4[i] = rand() % 1920;
            point_psy_4[i] = rand() % 1080;
            point_spx_4[i] = (rand() % 1001 - 500) / 500.0;
            point_spy_4[i] = (rand() % 1001 - 500) / 500.0;
            if (point_spx_4[i] != 0 && point_spy_4[i] != 0)
            {
                F = false;
                save_point_psx_4[i] = point_psx_4[i];
                save_point_psy_4[i] = point_psy_4[i];
                save_point_spx_4[i] = point_spx_4[i];
                save_point_spy_4[i] = point_spy_4[i];
            }
        }
    }

    load();
    predef();

    load_records();
    
    Clock clock;

    screen.setVerticalSyncEnabled(true);
    screen.setFramerateLimit(120);

    while (screen.isOpen())
    {
        Vector2f pos = screen.mapPixelToCoords(Mouse::getPosition(screen));

        int time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        Event event;
        while (screen.pollEvent(event))
        {
            if (event.type == Event::Closed)
                screen.close();
            if (event.key.code == Keyboard::LAlt)
                if (event.key.code == Keyboard::F4)
                    screen.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (mainmenu_bool)
                    {
                        if (menu_m_1_border.inborders(pos.x, pos.y))
                        {
                            saper_subtime = 0;
                            saper_time = 0;
                            mainmenu_bool = false;
                            ct_drop = true;
                            blackscreen_tick = 300;
                            saper_gamebool_r = true;
                            for (int i = 0; i < 24; i++)
                                for (int j = 0; j < 16; j++)
                                {
                                    saper_map[i][j].open = false;
                                    saper_map[i][j].flag = false;
                                    saper_map[i][j].mine = false;
                                    saper_map[i][j].neight = 0;
                                }
                            saper_mapgen = false;
                            saper_gameover_bool = false;
                        }
                        if (menu_m_2_border.inborders(pos.x, pos.y)) screen.close();
                    }
                    if (gamemenu_bool)
                    {
                        if (menu_g_1_border.inborders(pos.x, pos.y))
                        {
                            gamemenu_bool = false;
                            ct_drop = true;
                            blackscreen_tick = 300;
                            saper_gamebool_r = true;
                        }
                    }
                    if (saper_gamebool)
                    {
                        if (saper_mines_32_b.inborders(pos.x, pos.y))
                        {
                            saper_size = 32;
                        }
                        if (saper_mines_48_b.inborders(pos.x, pos.y))
                        {
                            saper_size = 48;
                        }
                        if (saper_mines_64_b.inborders(pos.x, pos.y))
                        {
                            saper_size = 64;
                        }
                        if (saper_be.inborders(pos.x, pos.y))
                        {
                            mainmenu_bool_r = true;
                            blackscreen_tick = 300;
                            load_tick = 0;
                            for (int i = 0; i < ct; i++)
                            {
                                point_psx_1[i] = save_point_psx_1[i];
                                point_psy_1[i] = save_point_psy_1[i];
                                point_spx_1[i] = save_point_spx_1[i];
                                point_spy_1[i] = save_point_spy_1[i];
                                point_psx_2[i] = save_point_psx_2[i];
                                point_psy_2[i] = save_point_psy_2[i];
                                point_spx_2[i] = save_point_spx_2[i];
                                point_spy_2[i] = save_point_spy_2[i];
                                point_psx_3[i] = save_point_psx_3[i];
                                point_psy_3[i] = save_point_psy_3[i];
                                point_spx_3[i] = save_point_spx_3[i];
                                point_spy_3[i] = save_point_spy_3[i];
                                point_psx_4[i] = save_point_psx_4[i];
                                point_psy_4[i] = save_point_psy_4[i];
                                point_spx_4[i] = save_point_spx_4[i];
                                point_spy_4[i] = save_point_spy_4[i];
                            }
                        }
                        if (saper_br.inborders(pos.x, pos.y))
                        {
                            saper_subtime = 0;
                            saper_time = 0;
                            for (int i = 0; i < 24; i++)
                            for (int j = 0; j < 16; j++)
                            {
                                saper_map[i][j].open = false;
                                saper_map[i][j].flag = false;
                                saper_map[i][j].mine = false;
                                saper_map[i][j].neight = 0;
                            }
                            saper_mapgen = false;
                            saper_gameover_bool = false;
                            saper_win_bool = false;
                        }
                        if (pos.x > 420 && pos.x < 1860 && pos.y > 60 && pos.y < 1020)
                        {
                            if (!saper_mapgen)
                            {
                                saper_genmap((pos.x - 420) / 60, (pos.y - 60) / 60, saper_size);
                                saper_mapgen = true;
                            }
                            if (!saper_gameover_bool && !saper_win_bool)
                            {
                                open_cell((pos.x - 420) / 60, (pos.y - 60) / 60);
                                saper_win_bool = cheak_win();
                                if (saper_win_bool)
                                {
                                    if (saper_size == 32 && saper_time < saper_time_record_32) saper_time_record_32 = saper_time;
                                    if (saper_size == 48 && saper_time < saper_time_record_48) saper_time_record_48 = saper_time;
                                    if (saper_size == 64 && saper_time < saper_time_record_64) saper_time_record_64 = saper_time;
                                    save_records();
                                }
                            }
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (saper_gamebool)
                    {
                        if (pos.x > 420 && pos.x < 1860 && pos.y > 60 && pos.y < 1020)
                            if (!saper_gameover_bool) set_flag((pos.x - 420) / 60, (pos.y - 60) / 60);
                    }
                }
            }
        }

        screen.clear();
        update();
        screen.display();

    }

    return 0;
}