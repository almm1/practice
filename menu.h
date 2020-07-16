#ifndef _MENU_H_
#define _MENU_H_

#include <SDL.h>
#include "map.h"
#include "button.h"
#include "timer.h"
#include "leaderboard.h"
#define ushort unsigned short
#define uint unsigned int
enum CurrentMenu {None, Main, Pause, GameOver, Win, Help};
enum Difficulty {Easy, Normal, Hard};
enum SoundVolume {NoSound, LowSound, NormalSound};

class Menu
{
	public:
        Menu();
        bool check_hover(ushort button_id); //возвращает true, если мышь находитс€ над кнопкой, и вызывает handme_click, если нажата кнопка мыши
        void handle_click(ushort button_id); //обрабатывать логику кнопок
        bool wants_to_quit = false; //true, если игрок хочет закрыть игру
        CurrentMenu current = Main; //текущее меню
        bool mouse_down = false; //true, если кнопка мыши нажата во врем€ этого кадра, это обрабатываетс€ классом Player
        Difficulty difficulty = Normal;
        SoundVolume sound = LowSound;
        Timer timer;
        Leaderboard leaderboard;
        Button buttons[9]; // нопки интерфейса

        ~Menu();

    private:
        
};

#endif