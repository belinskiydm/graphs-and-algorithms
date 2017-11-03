/* Описываем методы класса Graph. */
#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "Graph.h"
using namespace std;

//Освобождаем память
Graph::~Graph() {
	for (auto m : full_graph)
		delete m.second;
}
//В этом конструкторе задаем первую вершину
Graph::Graph(const int& name) {
	Node *p_node = new Node;
	p_node->name = name;
	full_graph[name] = p_node;
	/* Инициализируем поля оптимального пути нулями, так как в исходную вершину не ведут 
	никакие ребра. Необходимо для алгоритма Дейкстры */
	p_node->initial_weight = 0;
	//Копируем значение метки во временный массив
	p_node->best_way[0] = p_node->initial_weight;
	p_node->best_way[1] = 0;
	p_node->p_flag = 0;//Инициализируем флаг прохода вершины для алгоритма Дейкстры
}

//Находим указатель на вершину по названию
Node* Graph::Get_edge(const int& node_name) {
	return full_graph[node_name];
}

//Записываем стоимость перехода от исходной вершины к другой
void Graph::Set_edges(const int& from, const int& to, const int& weight) {
	//Если обе вершины существуют
	if (full_graph.count(from) != 0 && full_graph.count(to) != 0) {
		//Проверяем соединены ли они уже. Если нет, то соединяем
		if (full_graph[from]->edges_weights.count(to) == 0)
			full_graph[from]->Set_edges(to, weight, full_graph[to]);
		/* Если да - то возвращаем ошибку. Чтобы логически разделить соединение вершин
		и изменение стоимости перехода (эта операция описана в другом методе)*/
		else
			throw logic_error("Edge already exists");
	}
	//Если существует только исходная вершина
	else if (full_graph.count(from) != 0) {
		Node *p_next_node = new Node;
		p_next_node->name = to;
		p_next_node->p_flag = 0;//Инициализируем флаг прохода вершины для алгоритма Дейкстры
		full_graph[from]->Set_edges(to, weight, p_next_node);
		full_graph[to] = p_next_node;
	}
	//Если только исходной вершины нет
	else if (full_graph.count(to) != 0) {
		Node *p_current_node = new Node;
		p_current_node->name = from;
		/* Инициализируем поля оптимального пути нулями, так как в исходную вершину не ведут
		никакие ребра. Необходимо для алгоритма Дейкстры */
		p_current_node->initial_weight = 0;
		p_current_node->best_way[0] = p_current_node->initial_weight;
		p_current_node->best_way[1] = 0;
		p_current_node->p_flag = 0;//Инициализируем флаг прохода вершины для алгоритма Дейкстры

		full_graph[from]->Set_edges(to, weight, full_graph[to]);
	}
	//Если обеих вершин нет - ошибка. Чтобы не создавать несвязанные с текущим графом вершины
	else
		throw logic_error("Both nodes don't exist.");
}

//Меняем стоимость указанного ребра
void Graph::Change_weight(const int& from, const int& to, const int& weight) {
	//Стандартная проверка наличия вершин в графе
	if (full_graph.count(from) != 0 && full_graph.count(to) != 0) {
		//Проврка, связанны ли вершины
		if (full_graph[from]->edges_weights.count(to) != 0) {
			//Вызываем метод структуры Node
			full_graph[from]->Change_weight(to, weight);
		} else
			throw logic_error("Nodes are not connected.");
	} else
		throw logic_error("One of the nodes doesn't exist.");
}


//Greedy algorithm
vector<int> Graph::Find_greedy_way(int from, const int& to, int* weight_sum) {
	//Стандартная проверка наличия вершин в графе
	if (full_graph.count(from) != 0 && full_graph.count(to) != 0) {
		vector<int> greedy_way;
		greedy_way.push_back(from);
		*weight_sum = 0; //Обнуляем значение итоговой суммы переходов
		//int save_previous_node; Для хранения пердыдущей вершины
		while (from != to) {
			//Находим вершину с самым дешевым переходом
			int cheapest_node = full_graph[from]->Get_cheapest_edge();
			//Сохраняем ее в результирующем векторе
			greedy_way.push_back(cheapest_node);
			//Возвращаем итоговую стоимость всех переходов
			*weight_sum += full_graph[from]->edges_weights[cheapest_node];
			//Заменяем текущую вершина на новую (самую дешевую)
			//save_previous_node = from;
			from = cheapest_node;
		}

		return greedy_way;
	}
	else
		throw logic_error("Wrong nodes.");

	//если тупик, то просто присвоить from предыдущую вершину и continue!!!!!!
	//Улучшить алгоритм на случай тупиковых ветвей
}

//Dijkstra's algorithm
vector<int> Graph::Find_Dijkstras_way(const int& from, const int& to, int* weight_sum) {
	if (full_graph.count(from) != 0 && full_graph.count(to) != 0) {
		vector<int> dijkstras_way;
		int temp_from = from;
		//Обнуляем метку начальной вершины, так как с нее начинается расчет пути
		full_graph[temp_from]->best_way[0] = 0;
		full_graph[temp_from]->best_way[1] = 0;
		
		set<int> nodes_to_cover; //Массив вершин, которые нужно обработать
		nodes_to_cover.insert(temp_from);//Начинаем с первой
		
		//Пока вектор вершин больше 0
		while (nodes_to_cover.size() > 0) {
			//Если флаг = 1, значит вершина уже обработана и пропускаем ее
			if (full_graph[temp_from]->p_flag != 1) {
				//Пересчитываем вес меток ко всем соседним вершинам, к которым можно перейти
				for (auto m : full_graph[temp_from]->edges_weights) {
					//Складываем накопленный вес переходов и стоимость нового перехода
					int total_weight = full_graph[temp_from]->best_way[0] + m.second;
					//Если полученное значение меньше указанного в будущей вершине - заменяем его
					if (full_graph[m.first]->best_way[0] >= total_weight) {
						full_graph[m.first]->best_way[0] = total_weight;
						//Запоминаем вершину - источник перехода
						full_graph[m.first]->best_way[1] = temp_from;
					}
					//Добавляем новые вершины(к которым можно перейти от исходной) в вектор
					//при этом дуюликаты не будут учитываться
					nodes_to_cover.insert(m.first);
				}
				//Вершина обработана - флаг устанавливаем в 1
				full_graph[temp_from]->p_flag = 1;
			}
			nodes_to_cover.erase(temp_from);//удаляем обработанную вершину
			//Если вектор не пустой - берем первую вершину
			if (nodes_to_cover.size() != 0)
				temp_from = *nodes_to_cover.begin();
		}
	
		//Формируем оптимальный путь. С конца графа.
		int temp_to = to;
		dijkstras_way.push_back(temp_to);
		while (temp_to != from) {
			temp_to = full_graph[temp_to]->best_way[1];
			dijkstras_way.push_back(temp_to);
		}
		//Переворачиваем вектор
		reverse(dijkstras_way.begin(), dijkstras_way.end());
		//Возвращаем итоговую стоимость всех переходов
		*weight_sum = full_graph[to]->best_way[0];

		//Восстанавливаем исходные метки вершин, флагов и завершаем алгоритм
		for (auto &node : full_graph) {
			node.second->best_way[0] = node.second->initial_weight;
			node.second->best_way[1] = 0;
			node.second->p_flag = 0;
		}
		//Возвращаем путь
		return dijkstras_way;
	}
	else
		throw logic_error("One of the nodes doesn't exist.");
}
