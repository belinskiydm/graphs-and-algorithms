/* Функция main */
#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <exception>
#include <sstream>
#include <iomanip>
#include "Node.h"
#include "Graph.h"
using namespace std;

void Print_way(const vector<int>&, Graph&, int);

int main()
{
	try {
		//Создаем граф, указывая начальную вершину
		Graph test(1);
		int weight_sum(0); //Стоимость всех переходов на пути

		//Добавляем связи между вершинами и стоимость перехода
		test.Set_edges(1, 2, 7);
		test.Set_edges(1, 3, 9);
		test.Set_edges(1, 6, 14);
		test.Set_edges(2, 3, 10);
		test.Set_edges(2, 4, 15);
		test.Set_edges(3, 6, 2);
		test.Set_edges(3, 4, 11);
		test.Set_edges(6, 5, 9);
		test.Set_edges(4, 5, 6);

		//Выводим оптимальный путь на основе жадного алгоритма между вершинами 1 и 5
		Print_way(test.Find_greedy_way(1, 5, &weight_sum), test, weight_sum);
		cout << endl;

		//Меняем стоимость перехода между вершинами 1 и 5 на 19
		test.Change_weight(3, 6, 15);

		//Снова расчитываем жадный алгоритм
		Print_way(test.Find_greedy_way(1, 5, &weight_sum), test, weight_sum);
		cout << endl;

		//Добавляем новую связь между вершинами 2 и 5. Стоимость 9
		test.Set_edges(2, 5, 9);

		//Снова расчитываем жадный алгоритм
		Print_way(test.Find_greedy_way(1, 5, &weight_sum), test, weight_sum);
		cout << endl;

		//Выводим оптимальный путь на основе алгоритма Дейкстры между вершинами 1 и 5
		Print_way(test.Find_Dijkstras_way(1, 5, &weight_sum), test, weight_sum);
		cout << endl;

		//Выводим оптимальный путь на основе алгоритма Дейкстры между вершинами 1 и 6
		Print_way(test.Find_Dijkstras_way(1, 6, &weight_sum), test, weight_sum);
		cout << endl;

	}
	catch (const logic_error& er) {
		cout << er.what() << endl;
	}

	return 0;
}

//Выводим вектор пути на экран
void Print_way(const vector<int>& output, Graph& current, int weight_sum) {
	for (const auto& node : output)
		cout << node << " - ";
	cout << "Total weight sum: " << weight_sum << endl;
}