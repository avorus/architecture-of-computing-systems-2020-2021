#include <iostream>
#include <omp.h>
#include <queue>
#include <Windows.h>

using namespace std;

/// <summary>
/// Осуществляет корректный ввод целого положительного числа.
/// </summary>
/// <returns>корректное целое число</returns>
int correct_input()
{
	int input;
	while (!(cin >> input && 1 <= input && input <= INT32_MAX))
	{
		cout << "Incorrect input! Please, try again: ";
	}
	return input;
}

int const num_treads = 3; //количество потоков
int tread_num; //номер потока
queue<int> stock; //склад
omp_lock_t stock_lock;
queue<int> lorry; //грузовик
omp_lock_t lorry_lock;
unsigned long sum; //сумма награбленного

/// <summary>
/// Симулирует работу прапорщика Иванова
/// </summary>
/// <param name="num_items"></param>
void Ivanov(int &num_items)
{
	int count = 0;
	while (count < num_items)
	{
		int temp = rand();

		omp_set_lock(&stock_lock);
		stock.push(temp);

		#pragma omp critical (output)
		{
			cout << "Ivanov took box " << temp << " from the stock." << endl;
		}
		omp_unset_lock(&stock_lock);

		count++;

		Sleep(num_items % 131);
	}
}

/// <summary>
/// Симулирует работу прапорщика Петрова
/// </summary>
/// <param name="num_items"></param>
void Petrov(int &num_items)
{
	int count = 0;
	while (count < num_items)
	{
		if (!stock.empty())
		{
			omp_set_lock(&stock_lock);
			int temp = stock.front();
			stock.pop();
			omp_unset_lock(&stock_lock);

			omp_set_lock(&lorry_lock);
			lorry.push(temp);

			#pragma omp critical (output)
			{
				cout << "Petrov put box " << temp << " in the lorry." << endl;
			}
			omp_unset_lock(&lorry_lock);

			count++;

			Sleep(num_items % 131);
		}
	}
}

/// <summary>
/// Симулирует работу прапорщика Нечепочрук
/// </summary>
/// <param name="num_items"></param>
void Nechepochruk(int &num_items)
{
	int count = 0;
	while (count < num_items)
	{
		if (!lorry.empty())
		{
			omp_set_lock(&lorry_lock);
			int temp = lorry.front();
			lorry.pop();

			sum += temp;

			#pragma omp critical (output)
			{
				cout << "Necheporchuk calculated box " << temp << " and got sum " << sum << "." << endl;
			}
			omp_unset_lock(&lorry_lock);

			count++;

			Sleep(num_items % 131);
		}
	}
}

int main()
{
	//Установка значения для генератора рандомных чисел
	srand(static_cast<unsigned int>(time(0)));

	//Инициализация переменных
	omp_set_num_threads(num_treads);

	stock = queue<int>();
	omp_init_lock(&stock_lock);
	lorry = queue<int>();
	omp_init_lock(&lorry_lock);

	sum = 0;

	//Ввод количества ящиков
	cout << "Enter the number of items: ";
	int num_items = correct_input(); //количество ящиков

	//Начало работы прапорщиков
	#pragma omp parallel private (tread_num)
	{
		tread_num = omp_get_thread_num();
		if (tread_num == 0) //Иванов
		{
			Ivanov(num_items);
		}
		else if (tread_num == 1) //Петров
		{
			Petrov(num_items);
		}
		else //Нечепорчук
		{
			Nechepochruk(num_items);
		}
		#pragma omp barrier 
	}

	//Вывод итога выполнения работы
	cout << "Total sum of loot is equal to " << sum << "." << endl;

	omp_destroy_lock(&stock_lock);
	omp_destroy_lock(&lorry_lock);
}
