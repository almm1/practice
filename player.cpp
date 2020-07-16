#include <iostream>
#include <cmath>
#include "player.h"

#define ushort unsigned short
#define uint unsigned int
 
Player::Player(Map* ma, Menu* me) : display_flash(false), health(100), key_count(0), turkey_destruct(false), wall_destruct(false),
    hurt_sound(false), key_sound(false), x(3), y(3), angle(0), turn(0), walk_x(0), walk_y(0), pressed_keys(NULL), map(ma), menu(me)
{
    pressed_keys = new bool[7];
    for(int i = 0; i < 7; i++)
    {
        pressed_keys[i] = false;
    }
}

void Player::handle_events(float dt)
{
    SDL_Event event;
    if(health < 1)
        menu->current = GameOver;

    menu->mouse_down = false; //меню должно видеть нажатие кнопки мыши только для одного кадра
    display_flash = false;
    turkey_destruct = false;
    wall_destruct = false;

    while(SDL_PollEvent(&event))
    {
        //нажата кнопка мыши, поэтому мы обновляем меню
        if(event.type == SDL_MOUSEBUTTONDOWN)
            menu->mouse_down = true;
        // нажата клавиша
        else if(event.type == SDL_KEYDOWN)
        {
            update_key(event.key.keysym.sym, true);

            //стрельба из оружия
            if(event.key.keysym.sym == SDLK_SPACE)
            {
                if(menu->current == None)
                    Fire();
            }
        }
        //нажата клавиша
        else if(event.type == SDL_KEYUP)
        {
            update_key(event.key.keysym.sym, false);

            //приостанавливает игру, если игрок нажимает Escape
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                if(menu->current == None)
                    menu->current = Pause;
                else if(menu->current == Pause)
                    menu->current = None;
            }

            //выходит из игры, если игрок мертв и нажимает Space или Escape
            if(menu->current == GameOver)
            {
                if(event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_ESCAPE)
                    menu->wants_to_quit = true;
            }
        }
        //закрыть игру, если пользователь нажимает кнопку закрытия окна
        else if(!menu->wants_to_quit)
            menu->wants_to_quit = event.type == SDL_QUIT;
    }

    if(menu->current == Main || menu->current == Help)
    {
        angle += dt * 0.1;
        return;
    }

    //Движения вперед - назад
    if(pressed_keys[0])
        walk_y = speed * dt;
    else if(pressed_keys[2])
        walk_y = -speed * dt;
    else
        walk_y = 0;

    //Движения влево - вправо
    if(pressed_keys[3])
        walk_x = speed * dt * 0.5;
    else if(pressed_keys[1])
        walk_x = -speed * dt * 0.5;
    else
        walk_x = 0;

    //обзор
    if(fabs(turn) < turn_max)
    {
        if(pressed_keys[5])
            turn += turn_accel * dt;
        else if(pressed_keys[4])
            turn -= turn_accel * dt;
    }
    if((!pressed_keys[5] && turn > 0) || (!pressed_keys[4] && turn < 0))
        turn = 0;

    angle += turn;
    if(angle > M_PI)
        angle -= 2 * M_PI;
    if(angle < -M_PI)   
        angle += 2 * M_PI;

    float nx = x + (walk_y * cos(angle) + walk_x * cos(angle + M_PI / 2.0)) * 0.1;
    float ny = y + (walk_y * sin(angle) + walk_x * sin(angle + M_PI / 2.0)) * 0.1;

    if(int(nx) >= 0 && int(nx) < map->w && int(ny) >= 0 && int(ny) < map->h)
    {
        if(map->get_tile(ushort(nx), ushort(y)) == ' ')
            x = nx;
        if(map->get_tile(ushort(x), ushort(ny)) == ' ')
            y = ny;
    }
}

void Player::update_key(SDL_Keycode key, bool state)
{
    if (key == 'w' && menu->current != Pause) pressed_keys[0] = state;
    else if (key == 'a' && menu->current != Pause) pressed_keys[1] = state;
    else if (key == 's' && menu->current != Pause) pressed_keys[2] = state;
    else if (key == 'd' && menu->current != Pause) pressed_keys[3] = state;
    else if (key == SDLK_LEFT && menu->current != Pause) pressed_keys[4] = state;
    else if (key == SDLK_RIGHT && menu->current != Pause) pressed_keys[5] = state;
    else if (key == SDLK_SPACE && menu->current != Pause) pressed_keys[6] = state;
}

void Player::Fire()
{
    std::vector<Sprite> sprites = map->get_sprites();

    float dist = 0; //расстояние
    bool hit_wall = false; //удар в стенку

    //нам не нужно вычислять sin и cos для каждого шага
    float x_offset = cos(angle);
    float y_offset = sin(angle);

    while(!hit_wall && dist < 10) //заставляет луч двигаться вперед шаг за шагом
    {
        dist += 0.1;

        //позиция кончика луча для этой итерации
        float ray_x = x + x_offset * dist;
        float ray_y = y + y_offset * dist;

        if(map->get_tile(ushort(ray_x), ushort(ray_y)) != ' ') //текущая плитка не пуста, мы врезались в стену
        {
            hit_wall = true;
            if(map->get_tile(ushort(ray_x), ushort(ray_y)) == '2')
            {
                wall_destruct = true;
                map->set_tile(ushort(ray_x), ushort(ray_y), ' ');
                map->add_temp_sprite(7, ushort(ray_x) + 0.5, ushort(ray_y) + 0.5, 1000);
                map->sort_sprites(x, y);
                map->update_dist_map(x, y);
            }
        }
        else
        {
            for(unsigned int i = 0; i < sprites.size(); i++)
            {
                if(sprites.at(i).type == Enemy)
                {
                   float sqr_dist = pow(ray_x - sprites.at(i).x, 2) + pow(ray_y - sprites.at(i).y, 2);
                    if(sqr_dist < 0.016)
                    {
                        turkey_destruct = true;
                        map->delete_sprite(i);
                        hit_wall = true;
                        map->add_temp_sprite(6, sprites.at(i).x, sprites.at(i).y, 400);
                        map->sort_sprites(x, y);

                        map->enemy_count--;
                        if(map->enemy_count < 1)
                        {
                            menu->timer.stop();
                            menu->current = Win;
                            menu->leaderboard.add_score(menu->timer.get_time());
                        }
                        break;
                    } 
                }
            }
        }
    }

    display_flash = true;
}

float Player::get_x()
{
    return x;
}

float Player::get_y()
{
    return y; 
}

float Player::get_angle()
{
    return angle;
}

Player::~Player()
{
    delete pressed_keys;
    std::cout<<"Player deleted"<<std::endl;
}