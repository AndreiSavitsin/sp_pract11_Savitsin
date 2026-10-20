#include <Windows.h>
#include <iostream>
#include <fstream>

struct Robot
{
private:
	unsigned int energyReserve; //Запас энергии
	unsigned int countPoints; //Количество очков
	std::string state; //Состояние робота
	unsigned int id;
public:
	Robot()
	{
		energyReserve = 60;
		countPoints = 0;
	}

	int GetEnergyReserve()
	{
		return energyReserve;
	}
	void SetEnergyReserve(int _energyReserve)
	{
		energyReserve = _energyReserve;
	}

	int GetCountPoints()
	{
		return countPoints;
	}
	void SetCountPoints(int _countPoints)
	{
		countPoints = _countPoints;
	}

	std::string GetState()
	{
		return state;
	}
	void SetState(int _state)
	{
		state = _state;
	}

	int GetID()
	{
		return id;
	}
	void SetID(int _id)
	{
		id = _id;
	}
};

void Obstacle(Robot robot) //Испытание 1. Препятствие
{
	if (robot.GetEnergyReserve() - 10 <= 0) return;


	robot.SetEnergyReserve(robot.GetEnergyReserve() - 10);	
	robot.SetCountPoints(robot.GetCountPoints() + 15);

	Sleep(3000);

	std::cout << "Робот с id: " << robot.GetID() << " преодолел препятствие\n";
}

void Sprint(Robot robot) //Испытание 2. Спринт
{
	if (robot.GetEnergyReserve() - 20 <= 0) return;
	
	robot.SetEnergyReserve(robot.GetEnergyReserve() - 20);
	robot.SetCountPoints(robot.GetCountPoints() + 30);

	Sleep(4000);

	std::cout << "Робот с id: " << robot.GetID() << " выполнил спринт\n";
}

void Recharging(Robot robot) //Испытание 3. Подзарядка
{
	int time = rand() % 5 * 1000;

	robot.SetEnergyReserve(robot.GetEnergyReserve() + time / 100);

	if (robot.GetEnergyReserve() > 100)
	{
		robot.SetEnergyReserve(100);
	}

	Sleep(time);

	std::cout << "Робот с id: " << robot.GetID() << " подзарядился\n";
}

void Marathon(Robot robot) //Испытание 4. Марафон
{
	if (robot.GetEnergyReserve() - 60 <= 0) return;

	robot.SetEnergyReserve(robot.GetEnergyReserve() - 60);
	robot.SetCountPoints(robot.GetCountPoints() + 55);	

	Sleep(10000);

	std::cout << "Робот с id: " << robot.GetID() << " выполнил марафон\n";
}

DWORD WINAPI Obstacle(LPVOID robot)
{	

	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	std::ifstream file("arena.dat", std::ios::binary);

	file.seekg(0, std::ios::end);
	std::streamsize fileSize = file.tellg();

	file.seekg(0, std::ios::beg);

	int count = fileSize / 4;
	
	Robot* robots = new Robot[count];

	HANDLE hThread;
	DWORD IDThread;

	hThread = CreateThread(NULL, 0, Obstacle, (void*)robots, 0, &IDThread);
	if (hThread == NULL) return GetLastError();

	WaitForSingleObject(hThread, INFINITE);
}