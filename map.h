#ifndef _MAP_H_
#define _MAP_H_

#include <SDL.h>
#include <vector>
#define ushort unsigned short
#define uint unsigned int
enum SpriteType {Decoration, Enemy, Key, Temporary};

struct Sprite
{
	float x, y = 0;
	unsigned short size = 600;
	unsigned short itex = 0;
	SpriteType type = Decoration;
	//���������� ���������� �� ������, ��� ������������ ��� ���������� ��������, ������� ��� ������������� ��������� ���������� ������
	float sqr_dist = 0;
	Uint32 start_time = 0;

	bool operator < (const Sprite& s) const
	{
		//������ ������� ����������� ����� �������� ���������
		return sqr_dist > s.sqr_dist;
	}

	Sprite() : x(0), y(0), size(600), itex(0), type(Decoration), sqr_dist(0), start_time(SDL_GetTicks()) {}
};

struct Door
{
	unsigned short x, y = 0;
	float animationState = 1; //0 = ��������� �������, 0.5 = �������� �������, 1 = ��������� �������

	Door() : x(0), y(0), animationState(1) {}
};

class Map
{
	public:
        Map();
        unsigned short w = 0;
		unsigned short h = 0;

		float speed = 0.03; //�������� ����������, ��������� � ����������� �� ���������
		int damage = 0; //����, ��������� ������ ������, ���������� � ����������� �� ���������
		int enemy_count = 0;
		
		char get_tile(unsigned short x, unsigned short y);
		void set_tile(unsigned short x, unsigned short y, char tile);
		void sort_sprites(float player_x, float player_y); //��������� ������� � ������� �� ���������� �� ������
		std::vector<Sprite> const& get_sprites();
		void delete_sprite(unsigned short id); //������� ������ �� ���������� ������� � �������
		void update_sprites(float player_x, float player_y, float dt); //���������� ������, ����� ��� ��������� �� ������� � �������� ������ ��������� ��������
		Door get_door(unsigned short x, unsigned short y); //�������� ������� ����� � ��������� �����������, ���������� 0, ���� �� ������
		bool update_doors(float player_x, float player_y, float dt); //��������� ��������� ��������, ���� ����� ���������� ������
		void animate_sprites(); //������������ �������� ��� �������� ������
		int damage_player(); //���������� ���������� �����, ������� ����� ������ �������� � ����������� �� ��������� � �������� ������
		bool pickup_keys(); //���������� �������� true � ������� ���� � �����, ���� ����� ��������� ���������� ������, ���������� �������� false, ���� ���� �� ��������
		void add_temp_sprite(ushort itex, float x, float y, ushort size); //��������� �� ����� ������, ������� ��������� ����� 500�� (��� �������)
		void update_dist_map(unsigned short px, unsigned short py);
		~Map();

		//��������� ���������� �����������, ����� �������� ��������������
        Map(const Map& m) = delete;
        Map& operator=(const Map& m) = delete;

	private:
		char* map;
		unsigned short* dist;
		std::vector<Sprite> sprites;
		std::vector<Door> doors;
		Uint32 get_pixel(SDL_Surface* source, unsigned short x, unsigned short y);
};

#endif