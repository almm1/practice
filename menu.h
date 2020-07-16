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
        bool check_hover(ushort button_id); //���������� true, ���� ���� ��������� ��� �������, � �������� handme_click, ���� ������ ������ ����
        void handle_click(ushort button_id); //������������ ������ ������
        bool wants_to_quit = false; //true, ���� ����� ����� ������� ����
        CurrentMenu current = Main; //������� ����
        bool mouse_down = false; //true, ���� ������ ���� ������ �� ����� ����� �����, ��� �������������� ������� Player
        Difficulty difficulty = Normal;
        SoundVolume sound = LowSound;
        Timer timer;
        Leaderboard leaderboard;
        Button buttons[9]; //������ ����������

        ~Menu();

    private:
        
};

#endif