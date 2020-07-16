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
        bool init_sdl(const char* title, ushort width, ushort height); //инициализирует SDL и необходимые данные дл€ рендеринга
        void draw(uint fps); //показатели игры и интерфейса и их отображение
        ~Renderer();

        //запрещает копировать конструктор, чтобы избежать предупреждени€
        Renderer(const Renderer& r) = delete;
        Renderer& operator=(const Renderer& r) = delete;
        
    private:
        void set_pixel(ushort x, ushort y, Uint32 pixel); //устанавливает пиксель на экране
        Uint32 get_pixel_tex(ushort itex, ushort x, ushort y, bool sprite = false); //получает пиксель из файла текстуры

        Uint32 rgb_to_int(unsigned char r, unsigned char g, unsigned char b); //преобразовать значение RGB в Uint32, используемый SDL
        Uint32 apply_light(Uint32 color, float factor); //затемн€ет цвет, использу€ коэффициент, 1 = без изменений, 0 = черный

        void draw_sprite(Sprite s); //рисует спрайты в игровом мире
        void draw_2d_sprite(ushort itex, ushort x, ushort y, float size); //рисует спрайт в виде плоской текстуры на экране
        void draw_text(ushort x, ushort y, std::string text, bool big_text, SDL_Color ttf_color); //рисует текст использу€ sdl_ttf

        //»нтерфейс пользовател€ (меню и результаты)
        void display_normal(int fps);
        void display_menu();
        void display_pause_menu();
        void display_game_over();
        void display_win();
        void display_help();

        SDL_Window* window;
        SDL_Renderer* sdl_renderer;
        SDL_Texture* render_texture;

        Uint32* pixels; //все пиксели отображаемые на экране
        ushort screen_w; //ширина экрана
        ushort screen_h; //высота экрана

        SDL_Surface* wall_textures;
        SDL_Surface* sprites_textures;
        TTF_Font *font_big; // шрифт размером 100
        TTF_Font *font_medium; //шрифт размером 60
        float* zbuffer; //массив, содержащий все рассто€ни€

        Player* player;
        Map* map;
        Menu* menu;
};

#endif

