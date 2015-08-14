#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Node;
struct Edge;
struct Tree;

struct Node
{
	string name;
	int dist;
	bool visited;
	Edge *ancestor;
	vector <Edge *> edges;
};


struct Edge
{
	double weight;
	Node *node1;
	Node *node2;
};



struct Tree
{
	vector <Node *> nodes;
	int key;
	Edge *ancestor;
};


void shortestPath (string name1, string name2, vector <Node *> &nodes, vector <Edge *> &result, vector <Tree *> &trees)
{
	Node *node1, *node2 = NULL;
	for (int i = 0; i < nodes.size(); ++i)
	{
		if (nodes[i]->name == name1)
		{
			node1 = nodes[i];
			nodes[i]->dist = 0;
			Node *min = node1;
			while (min)
			{
				if (min->name == name2)
				{
					node2 = min;
				}
				for (int j = 0; j < min->edges.size(); ++j)
				{
					Edge *tempEdge = min->edges[j];
					if (tempEdge->node1 == min && !tempEdge->node2->visited)
					{
						if (tempEdge->node2->dist > min->dist + tempEdge->weight || tempEdge->node2->dist == -1)
						{
							tempEdge->node2->dist = min->dist + tempEdge->weight;
							tempEdge->node2->ancestor = tempEdge;
						}
					}
					else if (tempEdge->node2 == min && !tempEdge->node1->visited)
					{
						if (tempEdge->node1->dist > min->dist + tempEdge->weight || tempEdge->node1->dist == -1)
						{
							tempEdge->node1->dist = min->dist + tempEdge->weight;
							tempEdge->node1->ancestor = tempEdge;
						}
					}
				}
				min->visited = true;
				if (node2)
				{
					break;
				}
				min = NULL;
				for (int j = 0; j < nodes.size(); ++j)
				{
					if (nodes[j]->dist != -1 && !nodes[j]->visited && (!min || min->dist > nodes[j]->dist))
					{
						min = nodes[j];
					}
				}
				
			}
			break;
		}
		else if (nodes[i]->name == name2)
		{
			node1 = nodes[i];
			nodes[i]->dist = 0;
			Node *min = node1;
			while (min)
			{
				if (min->name == name1)
				{
					node2 = min;
				}
				for (int j = 0; j < min->edges.size(); ++j)
				{
					Edge *tempEdge = min->edges[j];
					if (tempEdge->node1 == min && !tempEdge->node2->visited)
					{
						if (tempEdge->node2->dist > min->dist + tempEdge->weight || tempEdge->node2->dist == -1)
						{
							tempEdge->node2->dist = min->dist + tempEdge->weight;
							tempEdge->node2->ancestor = tempEdge;
						}
					}
					else if (tempEdge->node2 == min && !tempEdge->node1->visited)
					{
						if (tempEdge->node1->dist > min->dist + tempEdge->weight || tempEdge->node1->dist == -1)
						{
							tempEdge->node1->dist = min->dist + tempEdge->weight;
							tempEdge->node1->ancestor = tempEdge;
						}
					}
				}
				min->visited = true;
				if (node2)
				{
					break;
				}
				min = NULL;
				for (int j = 0; j < nodes.size(); ++j)
				{
					if (nodes[j]->dist != -1 && !nodes[j]->visited && (!min || min->dist > nodes[j]->dist))
					{
						min = nodes[j];
					}
				}
			}
			break;
		}
	}
	Tree *newTree = new Tree;
	newTree->key = -1;
	newTree->nodes.push_back (node2);
	while (node2 != node1)
	{
		result.push_back (node2->ancestor);
		if (node2->ancestor->node1 == node2)
		{
			newTree->nodes.push_back (node2->ancestor->node2);
			node2 = node2->ancestor->node2;
		}
		else
		{
			newTree->nodes.push_back (node2->ancestor->node1);
			node2 = node2->ancestor->node1;
		}
	}
	int merge = -1;
	for (int i = 0; i < trees.size() && merge == -1; ++i)
	{
		for (int j = 0; j < trees[i]->nodes.size() && merge == -1; ++j)
		{
			for (int k = 0; k < newTree->nodes.size(); ++k)
			{
				if (trees[i]->nodes[j] == newTree->nodes[k])
				{
					merge = i;
					break;
				}
			}
		}
	}
	if (merge != -1)
	{
		for (int i = 0; i < newTree->nodes.size(); ++i)
		{
			bool exists = false;
			for (int j = 0; j < trees[merge]->nodes.size(); ++j)
			{
				if (newTree->nodes[i] == trees[merge]->nodes[j])
				{
					exists = true;
					break;
				}
			}
			if (!exists)
			{
				trees[merge]->nodes.push_back (newTree->nodes[i]);
			}
		}
	}
	else
	{
		trees.push_back (newTree);
	}
	for (int i = 0; i < nodes.size(); ++i)
	{
		nodes[i]->dist = -1;
		nodes[i]->ancestor = NULL;
		nodes[i]->visited = false;
	}
}




void minSpanTree (vector <Tree *> &trees, vector <Edge *> &result)
{
	trees[0]->key = 0;
	vector <Tree *> visited;
	while (visited.size() != trees.size())
	{
		Tree *min = NULL;
		for (int i = 0; i < trees.size(); ++i)
		{
			bool finished = false;
			for (int j = 0; j < visited.size(); ++j)
			{
				if (trees[i] == visited[j])
				{
					finished = true;
				}
			}
			if (!finished && trees[i]->key != -1)
			{
				if (!min || min->key > trees[i]->key)
				{
					min = trees[i];
				}
			}
		}
		visited.push_back (min);
		for (int i = 0; i < min->nodes.size(); ++i)
		{
			Node *tempNode = min->nodes[i];
			for (int j = 0; j < tempNode->edges.size(); ++j)
			{
				Edge *tempEdge = tempNode->edges[j];
				bool inner = false;
				if (tempEdge->node1 == tempNode)
				{
					for (int k = 0; k < min->nodes.size(); ++k)
					{
						if (tempEdge->node2 == min->nodes[k])
						{
							inner = true;
							break;
						}
					}
					if (!inner)
					{
						for (int k = 0; k < trees.size(); ++k)
						{
							for (int l = 0; l < trees[k]->nodes.size(); ++l)
							{
								if (trees[k]->nodes[l] == tempEdge->node2)
								{
									if (trees[k]->key == -1 || trees[k]->key > tempEdge->weight)
									{
										trees[k]->key = tempEdge->weight;
										trees[k]->ancestor = tempEdge;
									}
								}
							}
						}
					}
				}
				else
				{
					for (int k = 0; k < min->nodes.size(); ++k)
					{
						if (tempEdge->node1 == min->nodes[k])
						{
							inner = true;
							break;
						}
					}
					if (!inner)
					{
						for (int k = 0; k < trees.size(); ++k)
						{
							for (int l = 0; l < trees[k]->nodes.size(); ++l)
							{
								if (trees[k]->nodes[l] == tempEdge->node1)
								{
									if (trees[k]->key == -1 || trees[k]->key > tempEdge->weight)
									{
										trees[k]->key = tempEdge->weight;
										trees[k]->ancestor = tempEdge;
									}
								}
							}
						}
					}
				}
			}
		}
		

	}
	for (int i = 0; i < trees.size(); ++i)
	{
		if (trees[i]->ancestor)
		{
			result.push_back (trees[i]->ancestor);
		}
	}
}

int main()
{
	int N, M, K;
	vector <Node *> nodes;
	vector <Edge *> edges, result;
	vector <Tree *> trees;
	ifstream input;
	ofstream output;
	input.open ("msp-sp.inp");
	output.open ("msp-sp.out");
	if (input.is_open() && output.is_open())
	{
		input >> N >> M >> K;
		for (int i = 0; i < N; ++i)
		{
			string name;
			input >> name;
			Node *newNode = new Node;
			newNode->name = name;
			newNode->dist = -1;
			newNode->visited = false;
			nodes.push_back (newNode);
		}



		for (int i = 0; i < M; ++i)
		{
			string name1, name2;
			double weight;
			input >> name1 >> name2 >> weight;
			for (int j = 0; j < nodes.size(); ++j)
			{
				if (nodes[j]->name == name1)
				{
					for (int k = 0; k < nodes.size(); ++k)
					{
						if (nodes[k]->name == name2)
						{
							Edge *newEdge = new Edge;
							newEdge->weight = weight;
							newEdge->node1 = nodes[j];
							newEdge->node2 = nodes[k];
							nodes[j]->edges.push_back (newEdge);
							nodes[k]->edges.push_back (newEdge);
							edges.push_back (newEdge);
							break;
						}
					}
					break;
				}
				else if (nodes[j]->name == name2)
				{
					for (int k = 0; k < nodes.size(); ++k)
					{
						if (nodes[k]->name == name1)
						{
							Edge *newEdge = new Edge;
							newEdge->weight = weight;
							newEdge->node1 = nodes[j];
							newEdge->node2 = nodes[k];
							nodes[j]->edges.push_back (newEdge);
							nodes[k]->edges.push_back (newEdge);
							edges.push_back (newEdge);
							break;
						}
					}
					break;
				}
			}
		}

		for (int i = 0; i < K; ++i)
		{
			string name1, name2;
			input >> name1 >> name2;
			shortestPath (name1, name2, nodes, result, trees);
		}
		for (int i = 0; i < nodes.size(); ++i)
		{
			bool inATree = false;
			for (int j = 0; j < trees.size() && !inATree; ++j)
			{
				for (int k = 0; k < trees[j]->nodes.size(); ++k)
				{
					if (nodes[i] == trees[j]->nodes[k])
					{
						inATree = true;
						break;
					}
				}
			}
			if (!inATree)
			{
				Tree *newTree = new Tree;
				newTree->key = -1;
				newTree->nodes.push_back (nodes[i]);
				trees.push_back (newTree);
			}
		}
		minSpanTree (trees, result);
		for (int i = 0; i < result.size(); ++i)
		{
			for (int k = 0; k < result.size(); ++k)
			{
				if (k != i)
				{
					if (result[i] == result[k])
					{
						result.erase (result.begin() + k);
					}
				}
			}
		}
					
		for (int i = 0; i < result.size(); ++i)
		{
			output << result[i]->node1->name << " " << result[i]->node2->name << endl;
		}
	}
	output.close();
}
