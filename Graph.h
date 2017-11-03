/*Заголовочный файл. Объявляем класс графа, методы и поля.*/
#pragma once
#include "Node.h"
#include <set>
class Graph {
	//Все вершины записываем в словарь
	map<int, Node*> full_graph; //Name and pntr to one node
public:
	//Конструкторы
	Graph() {}
	~Graph();
	//В этом конструкторе задаем первую вершину
	Graph(const int& name); 
	//Находим указатель на вершину по названию
	Node* Get_edge(const int&);
	//Записываем стоимость перехода от одной вершины к другой
	void Set_edges(const int& from, const int& to, const int& weight); 
	//Меняем стоимость указанного ребра
	void Change_weight(const int& from, const int& to, const int& weight);
	//Жадный алгоритм. Возвращаем путь и стоимость всех переходов (по указателю)
	vector<int> Find_greedy_way(int from, const int& to, int*);
	//Dijkstra's algorithm. Возвращаем путь и стоимость всех переходов (по указателю)
	vector<int> Find_Dijkstras_way(const int& from, const int& to, int*);
};
