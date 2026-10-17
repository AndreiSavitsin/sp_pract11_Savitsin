#include <iostream>
#include <fstream>

struct Robot
{
	unsigned int enregyReserve; //Запас энергии
	unsigned int countPoints; //Количество очков
	std::string state; //Состояние робота
	unsigned int id;

	Robot()
	{
		enregyReserve = 60;
		countPoints = 0;
	}	

	void Obstacle() //Испытание 1. Препятствие
	{
		if (this->enregyReserve - 10 <= 0) return;

		this->enregyReserve -= 10;
		this->countPoints += 15;
	}

	void Sprint() //Испытание 2. Спринт
	{
		if (this->enregyReserve - 20 <= 0) return;

		this->enregyReserve -= 20;
		this->countPoints += 30;
	}

	void Recharging() //Испытание 3. Подзарядка
	{

	}

	void Marathon() //Испытание 4. Марафон
	{
		if (this->enregyReserve - 60 <= 0) return;

		this->enregyReserve -= 60;
		this->countPoints += 55;
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	std::ifstream file("arena.dat", std::ios::binary);

	file.seekg(0, std::ios::end);
	std::streamsize fileSize = file.tellg();

	int count = fileSize / sizeof(Robot);

	Robot* robots = new Robot[count];
}