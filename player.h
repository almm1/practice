#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SDL.h>
#include "map.h"
#include "button.h"
#include "menu.h"
#define ushort unsigned short
#define uint unsigned int
const float speed = 30;
const float  turn_accel = 0.18;
const float turn_max = 0.08;

class Player
{
	public:
        Player(Map* ma, Menu* me);
        void handle_events(float dt); //движение игрока
        bool display_flash = false; //скорость дисплея
        int health = 100; //здоровье
        ushort key_count = 0; 
        bool turkey_destruct, wall_destruct, hurt_sound, key_sound; //стамина

        float get_x();
        float get_y();
        float get_angle();

        ~Player();

        //запрещаем копировать конструктор, чтобы избежать предупреждения
        Player(const Player& p) = delete;
        Player& operator=(const Player& p) = delete;

    private:
        float x, y, angle; // положение и поворот игрока
        float turn, walk_x, walk_y; // летящей походкой
        bool* pressed_keys;

        void update_key(SDL_Keycode key, bool state); //управление
        void Fire(); //стрельба

        Map* map;
        Menu* menu;
};

#endif