#pragma once
#include "stdafx.h"
#include <string>
#include <map>
#include <vector>
#include <set>
#include <iterator>
using namespace std;

struct Node {
	int name; //Имя вершины
	map<int, int> edges_weights; //Name - weight
	map<int, Node*> pntr_next_nodes; //Name - pointer to next nodes
	//TODO: map<int, Node*> pntr_prev_nodes; //Name - pointer to previous nodes
	map<int, set<int>> weight_sort; //Weight - set of names

	int initial_weight;/*Исходная метка вершины. В алгоритме Дейкстры она изначально равна 
	бесконечности.*/
	int best_way[2]; /*Best weight - source node. For Dijkstra's algorithm. После завершения
	алгоритма значение метки восстанавливается из initial_weight, а данные о вершинах обнуляются.
	Это нужно для того, чтобы алгоритм учитываел изначальные данные при повторном запуске*/

	int p_flag;//Флаг пройденной вершины. For Dijkstra's algorithm.
	
	//Записываем стоимость перехода к одной из ближайших вершин
	void Set_edges(const int& to, const int& weight, Node* pnext_node);
	//Меняем стоимость перехода к указанной вершине
	void Change_weight(const int& to, const int& weight);
	//Возвращаем название вершины с наименьшей стоимостью перехода
	int Get_cheapest_edge();
};



