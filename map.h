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
	//Квадратное расстояние от игрока, оно используется для сортировки спрайтов, поэтому нет необходимости вычислять квадратный корень
	float sqr_dist = 0;
	Uint32 start_time = 0;

	bool operator < (const Sprite& s) const
	{
		//дальше спрайты сортируются перед близкими спрайтами
		return sqr_dist > s.sqr_dist;
	}

	Sprite() : x(0), y(0), size(600), itex(0), type(Decoration), sqr_dist(0), start_time(SDL_GetTicks()) {}
};

struct Door
{
	unsigned short x, y = 0;
	float animationState = 1; //0 = полностью открыта, 0.5 = частично открыта, 1 = полностью закрыта

	Door() : x(0), y(0), animationState(1) {}
};

class Map
{
	public:
        Map();
        unsigned short w = 0;
		unsigned short h = 0;

		float speed = 0.03; //скорость противника, изменения в зависимости от сложности
		int damage = 0; //урон, наносимый каждым врагом, изменяется в зависимости от сложности
		int enemy_count = 0;
		
		char get_tile(unsigned short x, unsigned short y);
		void set_tile(unsigned short x, unsigned short y, char tile);
		void sort_sprites(float player_x, float player_y); //сортирует спрайты в векторе по расстоянию от игрока
		std::vector<Sprite> const& get_sprites();
		void delete_sprite(unsigned short id); //удаляет спрайт по указанному индексу в векторе
		void update_sprites(float player_x, float player_y, float dt); //перемещает врагов, чтобы они следовали за игроком и изменяет размер временных спрайтов
		Door get_door(unsigned short x, unsigned short y); //пытается открыть дверь в указанных координатах, возвращает 0, если не найден
		bool update_doors(float player_x, float player_y, float dt); //обновляет состояние анимации, если игрок достаточно близко
		void animate_sprites(); //перестановка спрайтов для анимации врагов
		int damage_player(); //возвращает количество урона, которое игрок должен получить в зависимости от сложности и близости врагов
		bool pickup_keys(); //возвращает значение true и удаляет ключ с карты, если игрок находится достаточно близко, возвращает значение false, если ключ не подобран
		void add_temp_sprite(ushort itex, float x, float y, ushort size); //добавляет на карту спрайт, который удаляется через 500мс (для взрывов)
		void update_dist_map(unsigned short px, unsigned short py);
		~Map();

		//запрещаем копировать конструктор, чтобы избежать предупреждения
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