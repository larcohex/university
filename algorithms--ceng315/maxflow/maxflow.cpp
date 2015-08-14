#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <utility>
#include <cmath>

using namespace std;


double bfs (vector <vector <int> > &graph, vector <vector <double> > &capacities, vector <vector <double> > &flows, vector <int> &parents, int source, int sink)
{
	parents.clear();
	vector <double> caps;
	for (int i = 0; i < graph.size(); ++i)
	{
		parents.push_back (-1);
		caps.push_back (0);
	}
	parents[source] = -2;
	caps[source] = numeric_limits <double> :: max();
	vector <int> queue;
	queue.push_back (source);
	while (!queue.empty())
	{
		int current = queue[0];
		queue.erase (queue.begin());
		for (int i = 0; i < graph[current].size(); ++i)
		{
			int temp = graph[current][i];
			if (parents[temp] == -1)
			{
				if (capacities[current][temp] - flows[current][temp] > 0)
				{
					parents[temp] = current;
					if (caps[current] < capacities[current][temp] - flows[current][temp])
					{
						caps[temp] = caps[current];
					}
					else
					{
						caps[temp] = capacities[current][temp] - flows[current][temp];
					}
					if (temp == sink)
					{
						return caps[sink];
					}
					queue.push_back (temp);
				}
			}
		}
	}
	return 0;
}


double maxFlow (vector <vector <int> > &graph, vector <vector <double> > &capacities, vector <vector <double> > &flows, vector <int> &parents, int source, int sink)
{
	double maxFlow = 0;
	while (true)
	{
		double tempFlow = bfs (graph, capacities, flows, parents, source, sink);
		if (!tempFlow)
		{
			break;
		}
		maxFlow += tempFlow;
		int current = sink;
		while (current != source)
		{
			int prev = parents[current];
			flows[prev][current] += tempFlow;
			flows[current][prev] -= tempFlow;
			current = prev;
		}
	}
	return maxFlow;
} 


int main()
{
	ifstream input;
	input.open ("maxflow.inp");
	ofstream output;
	if (input.is_open())
	{
		int N, M;
		input >> N >> M;
		vector <vector <int> > graph;
		vector <vector <double> > capacities;
		vector <vector <double> > flows;
		vector <pair <int, int> > order;
		for (int i = 0; i < N + 4; ++i)
		{
			vector <int> temp1;
			graph.push_back (temp1);
			vector <double> temp2;
			for (int j = 0; j < N + 4; ++j)
			{
				temp2.push_back (0);
			}
			flows.push_back (temp2);
			capacities.push_back (temp2);
		}
		for (int i = 0; i < M; ++i)
		{
			int i, j;
			double c;
			input >> i >> j >> c;
			capacities[i][j] = c;
			capacities[j][i] = c;
			graph[i].push_back (j);
			graph[j].push_back (i);
			pair <int, int> temp (i, j);
			order.push_back (temp);
		}
		graph[1].push_back (N + 3);
		graph[2].push_back (N + 3);
		graph[N + 3].push_back (1);
		graph[N + 3].push_back (2);
		capacities[1][N + 3] = numeric_limits <double> :: max();
		capacities[N + 3][1] = numeric_limits <double> :: max();
		capacities[2][N + 3] = numeric_limits <double> :: max();
		capacities[N + 3][2] = numeric_limits <double> :: max();
		vector <int> parents;
		maxFlow (graph, capacities, flows, parents, 0, N + 3);
		if (flows[1][N+3] != flows[2][N+3])
		{
			if (flows[1][N+3] > flows[2][N+3])
			{
				graph[N+3].pop_back();
				graph[2].pop_back();
				capacities[1][N+3] = 0;
				capacities[2][N+3] = 0;
				capacities[N+3][1] = (flows[1][N+3] - flows[2][N+3]) / 2;
				capacities[1][N+3] = (flows[1][N+3] - flows[2][N+3]) / 2;
				flows[1][N+3] = 0;
				flows[2][N+3] = 0;
				flows[N+3][1] = 0;
				flows[N+3][2] = 0;
				int tempFlow = maxFlow (graph, capacities, flows, parents, N + 3, 2);
				if (tempFlow != capacities[N+3][1])
				{
					int temp1 = 0;
					int temp2 = 0;
					for (int i = 0; i < N + 3; ++i)
					{
						temp1 -= flows[1][i];
						temp2 -= flows[2][i];
					}
					capacities[N+3][1] = temp1 - temp2;
					capacities[1][N+3] = temp1 - temp2;
					flows[1][N+3] = 0;
					flows[N+3][1] = 0;
					tempFlow = maxFlow (graph, capacities, flows, parents, N + 3, 0);
				}
			}
			else
			{
				graph[N+3].erase (graph[N+3].begin());
				graph[1].pop_back();
				capacities[1][N+3] = 0;
				capacities[2][N+3] = 0;
				capacities[N+3][2] = (flows[2][N+3] - flows[1][N+3]) / 2;
				capacities[2][N+3] = (flows[2][N+3] - flows[1][N+3]) / 2;
				flows[1][N+3] = 0;
				flows[2][N+3] = 0;
				flows[N+3][1] = 0;
				flows[N+3][2] = 0;
				int tempFlow = maxFlow (graph, capacities, flows, parents, N + 3, 1);		
				if (tempFlow != capacities[N+3][1])
				{
					int temp1 = 0;
					int temp2 = 0;
					for (int i = 0; i < N + 3; ++i)
					{
						temp1 -= flows[1][i];
						temp2 -= flows[2][i];
					}
					capacities[N+3][2] = temp2 - temp1;
					capacities[2][N+3] = temp2 - temp1;
					flows[2][N+3] = 0;
					flows[N+3][2] = 0;
					maxFlow (graph, capacities, flows, parents, N + 3, 0);
				}
			}
		}
		ofstream output;
		output.open ("maxflow.out");
		for (int i = 0; i < M; ++i)
		{
			output << order[i].first << " " << order[i].second << " " << abs (flows[order[i].first][order[i].second]) << endl;
		}
	}
}
