#include <Windows.h>
#include <iostream>
#include <fstream>
#include <random>

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
		state = "работает";
		id = 0;
	}

	int GetEnergyReserve()
	{
		return energyReserve;
	}
	void SetEnergyReserve(int _energyReserve)
	{
		energyReserve = _energyReserve;
		if (energyReserve > 100) energyReserve = 100;
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
	void SetState(std::string _state)
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

	std::mt19937 Mt;

	int* trials;
	int trialCount;
};

void Obstacle(Robot* robot) //Испытание 1. Препятствие
{
	std::cout << "Робот с id: " << robot->GetID() << " начал проходить испытание Препятствие\n";

	if (robot->GetEnergyReserve() - 10 < 0)
	{
		std::cout << "Робот с id: " << robot->GetID() << " пропустил препятствие\n";
		return;
	}


	Sleep(3000);

	robot->SetEnergyReserve(robot->GetEnergyReserve() - 10);
	robot->SetCountPoints(robot->GetCountPoints() + 15);

	std::cout << "Робот с id: " << robot->GetID() << " преодолел препятствие. Энергия: " << robot->GetEnergyReserve() << 
		". Очки: " << robot->GetCountPoints() << std::endl;
}

void Sprint(Robot* robot) //Испытание 2. Спринт
{
	std::cout << "Робот с id: " << robot->GetID() << " начал проходить испытание Спринт\n";

	if (robot->GetEnergyReserve() - 20 < 0)
	{
		std::cout << "Робот с id: " << robot->GetID() << " пропустил спринт\n";
		return;
	}
	

	Sleep(4000);

	robot->SetEnergyReserve(robot->GetEnergyReserve() - 20);
	robot->SetCountPoints(robot->GetCountPoints() + 30);

	std::cout << "Робот с id: " << robot->GetID() << " выполнил спринт. Энергия: " << robot->GetEnergyReserve() <<
		". Очки: " << robot->GetCountPoints() << std::endl;;
}

void Recharging(Robot* robot) //Испытание 3. Подзарядка
{
	std::cout << "Робот с id: " << robot->GetID() << " начал проходить испытание Подзарядка\n";

	int time = (robot->Mt() % 5 + 1) * 1000;

	Sleep(time);

	robot->SetEnergyReserve(robot->GetEnergyReserve() + time / 100);

	if (robot->GetEnergyReserve() > 100)
	{
		robot->SetEnergyReserve(100);
	}

	std::cout << "Робот с id: " << robot->GetID() << " подзарядился. Энергия: " << robot->GetEnergyReserve() <<
		". Очки: " << robot->GetCountPoints() << std::endl;;
}

void Marathon(Robot* robot) //Испытание 4. Марафон
{
	std::cout << "Робот с id: " << robot->GetID() << " начал проходить испытание Марафон\n";

	if (robot->GetEnergyReserve() - 60 < 0)
	{
		std::cout << "Робот с id: " << robot->GetID() << " пропустил марафон\n";
		return;
	}
	

	Sleep(10000);

	robot->SetEnergyReserve(robot->GetEnergyReserve() - 60);
	robot->SetCountPoints(robot->GetCountPoints() + 55);

	std::cout << "Робот с id: " << robot->GetID() << " выполнил марафон. Энергия: " << robot->GetEnergyReserve() <<
		". Очки: " << robot->GetCountPoints() << std::endl;;
}

DWORD WINAPI RobotThread(LPVOID param)
{	
	Robot* robot = static_cast<Robot*>(param);			
	
	for (int i = 0; i < robot->trialCount; i++)
	{
		Sleep((robot->Mt() % 3 + 1) * 1000);

		switch (robot->trials[i])
		{
		case 1: 
			Obstacle(robot);
			break;
		case 2:
			Sprint(robot);
			break;
		case 3:
			Recharging(robot);
			break;
		case 4:
			Marathon(robot);
			break;
		default:
			break;
		}
	}

	robot->SetState("завершил испытания");

	std::cout << "Робот с id: " << robot->GetID() << " завершил испытания\n";

	return 0;
}

struct OperatorData
{
	HANDLE* threads;
	int count;
	Robot* robots;
};

DWORD WINAPI OperatorThread(LPVOID param)
{
	OperatorData* data = static_cast<OperatorData*>(param);

	while (true)
	{
		std::cout << "Введите p для паузы или r для возобновления или q для выхода: ";
		char key = 0;
		std::cin >> key;

		if (key == 'q') break;

		std::cout << "Введите id для робота: ";
		int id = 0;
		std::cin >> id;

		if (id < 1 || id > data->count)
		{
			std::cout << "Неверный id\n";
			continue;
		}

		if (key == 'p')
		{
			SuspendThread(data->threads[id - 1]);
			std::cout << "Робот с id: " << id << " приостановлен\n";
			data->robots[id - 1].SetState("приостановлен");
		}
		else if (key == 'r')
		{
			ResumeThread(data->threads[id - 1]);
			std::cout << "Робот с id: " << id << " работает\n";
			data->robots[id - 1].SetState("работает");
		}
		else
		{
			std::cout << "Неизвестная команда\n";
		}
	}

	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	std::random_device rd;

	std::ifstream file("arena.dat", std::ios::binary);

	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		std::streamsize fileSize = file.tellg();

		file.seekg(0, std::ios::beg);

		int trialCount = fileSize / sizeof(int);

		if (trialCount > 0)
		{
			int* trials = new int[trialCount];
			file.seekg(0, std::ios::beg);
			file.read(reinterpret_cast<char*>(trials), trialCount * sizeof(int));

			int count = 0;
			count = 3 + rand() % 8;
			Robot* robots = new Robot[count];
			for (int i = 0; i < count; i++)
			{			
				robots[i].SetID(i + 1);
				robots[i].trials = trials;
				robots[i].trialCount = trialCount;
				robots[i].Mt.seed(rd());
			}

			HANDLE* threads = new HANDLE[count];

			for (int i = 0; i < count; i++)
			{				
				threads[i] = CreateThread(NULL, 0, RobotThread, &robots[i], 0, NULL);
				if (threads[i] == NULL) return GetLastError();
			}

			OperatorData opData;
			opData.threads = threads;
			opData.count = count;
			opData.robots = robots;
			HANDLE hOperator = CreateThread(NULL, 0, OperatorThread, &opData, 0, NULL);

			WaitForMultipleObjects(count, threads, TRUE, INFINITE);

			if (hOperator != NULL) CloseHandle(hOperator);

			for (int i = 0; i < count - 1; i++)
			{
				for (int j = 0; j < count - 1 - i; j++)
				{
					if (robots[j].GetCountPoints() < robots[j + 1].GetCountPoints())
					{
						Robot temp = robots[j];
						robots[j] = robots[j + 1];
						robots[j + 1] = temp;
					}
				}
			}

			std::cout << "\nРезультаты:\n";
			for (int i = 0; i < count; i++)
			{
				std::cout << "Робот с id: " << robots[i].GetID() <<
					". Очки: " << robots[i].GetCountPoints() <<
					". Энергия: " << robots[i].GetEnergyReserve() <<
					". Состояние: " << robots[i].GetState() << std::endl;
			}

			int best = robots[0].GetCountPoints();
			int bestCount = 0;
			for (int i = 0; i < count; i++)
				if (robots[i].GetCountPoints() == best) bestCount++;

			if (bestCount >= 2)
				std::cout << "\nНичья\n";
			else
				std::cout << "\nПобедитель: робот с id: " << robots[0].GetID() << "\n";

			for (int i = 0; i < count; i++)
			{
				CloseHandle(threads[i]);
			}

			delete[] threads;
			delete[] robots;
			delete[] trials;			
		}
		else
		{
			std::cout << "Файл заполнен некорректно\n";
		}
	}
	else
	{
		std::cout << "Файл не найден\n";
	}

	return 0;
}