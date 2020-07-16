#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include "player.h"
#define ushort unsigned short
#define uint unsigned int
const float fov = 60 * M_PI / 180.0;

const SDL_Color ttf_color_white = {255, 255, 255, 255};
const SDL_Color ttf_color_banana = {209, 182, 6, 255};
const SDL_Color ttf_color_red = {255, 0, 0, 255};

class Renderer
{
    public:
        Renderer(Player* p, Map* ma, Menu* me);
        bool init_sdl(const char* title, ushort width, ushort height); //�������������� SDL � ����������� ������ ��� ����������
        void draw(uint fps); //���������� ���� � ���������� � �� �����������
        ~Renderer();

        //��������� ���������� �����������, ����� �������� ��������������
        Renderer(const Renderer& r) = delete;
        Renderer& operator=(const Renderer& r) = delete;
        
    private:
        void set_pixel(ushort x, ushort y, Uint32 pixel); //������������� ������� �� ������
        Uint32 get_pixel_tex(ushort itex, ushort x, ushort y, bool sprite = false); //�������� ������� �� ����� ��������

        Uint32 rgb_to_int(unsigned char r, unsigned char g, unsigned char b); //������������� �������� RGB � Uint32, ������������ SDL
        Uint32 apply_light(Uint32 color, float factor); //��������� ����, ��������� �����������, 1 = ��� ���������, 0 = ������

        void draw_sprite(Sprite s); //������ ������� � ������� ����
        void draw_2d_sprite(ushort itex, ushort x, ushort y, float size); //������ ������ � ���� ������� �������� �� ������
        void draw_text(ushort x, ushort y, std::string text, bool big_text, SDL_Color ttf_color); //������ ����� ��������� sdl_ttf

        //��������� ������������ (���� � ����������)
        void display_normal(int fps);
        void display_menu();
        void display_pause_menu();
        void display_game_over();
        void display_win();
        void display_help();

        SDL_Window* window;
        SDL_Renderer* sdl_renderer;
        SDL_Texture* render_texture;

        Uint32* pixels; //��� ������� ������������ �� ������
        ushort screen_w; //������ ������
        ushort screen_h; //������ ������

        SDL_Surface* wall_textures;
        SDL_Surface* sprites_textures;
        TTF_Font *font_big; // ����� �������� 100
        TTF_Font *font_medium; //����� �������� 60
        float* zbuffer; //������, ���������� ��� ����������

        Player* player;
        Map* map;
        Menu* menu;
};

#endif

