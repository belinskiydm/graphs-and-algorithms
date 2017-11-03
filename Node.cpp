/* Описываем методы структуры вершины (Node). */
#include "stdafx.h"
#include "Node.h"

//Записываем стоимость перехода к одной из ближайших вершин (запоминаем ее с помощью указателя)
void Node::Set_edges(const int& to, const int& weight, Node* pnext_node) {
	edges_weights[to] = weight;
	pntr_next_nodes[to] = pnext_node;
	weight_sort[weight].insert(to);

	//Инициализация полей для алгоритма Дейкстры
	pnext_node->initial_weight = 1000000; //Аналог бесконечности
	/*
	Переменная должны быть равна сумме всех стимостей граней + 1!!!
	Нужно создать цикл, который при добавлении нового ребра пересчитывал общую стоимость всех переходов
	и обновлял значения этого поля в каждой вершине! Пока пусть 1000000 будет.
	*/

	//Копируем значение метки во временный массив
	pnext_node->best_way[0] = pnext_node->initial_weight;
	pnext_node->best_way[1] = 0;
	
}

//Меняем стоимость перехода к указанной вершине
void Node::Change_weight(const int& to, const int& weight) {
	//Запоминаем старое значение
	int old_weight = edges_weights[to];
	edges_weights[to] = weight;	
	//Если старому весу соотвествовало несколько вершин, удаляем заданную
	if (weight_sort[old_weight].size() > 1)
		weight_sort[old_weight].erase(to);
	//В остальных случаях удалем весь ключ
	else
		weight_sort.erase(old_weight);
	//Записываем в словарь новую стоимость перехода и имя вершины графа
	weight_sort[weight].insert(to);
}

//Возвращаем название вершины с наименьшей стоимостью перехода
int Node::Get_cheapest_edge() {
	auto i = weight_sort.begin();
	auto j = i->second.begin();
	return *j;
}
