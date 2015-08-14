#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <stdlib.h>


using namespace std;


struct Node
{
	string program;
	string arguments;
	vector <int> links;
	vector <Node *> neighbours;
	bool hasParent;
};


void process (vector <int> &links, string neighbours)
{
	vector <string> numbers;
	for (int i = 0; i < neighbours.size(); ++i)
	{
		string temp;
		while (neighbours[i] != ' ')
		{
			temp.push_back (neighbours[i]);
			++i;
		}
		numbers.push_back (temp);
	}
	for (int i = 0; i < numbers.size(); ++i)
	{
		istringstream iss (numbers[i], istringstream::in);
		int val;
		iss >> val;
		links.push_back (val);
	}
}


void link (vector <Node *> &graph)
{
	for (int i = 0; i < graph.size(); ++i)
	{
		for (int j = 0; j < graph[i]->links.size(); ++j)
		{
			graph[i]->neighbours.push_back (graph[graph[i]->links[j]]);
			graph[graph[i]->links[j]]->hasParent = true;
		}
	}
}


void run (Node *node)
{
	string temp;
	temp.append (node->program);
	temp.push_back (' ');
	temp.append(node->arguments);
	char * cmd = new char [temp.length()+1];
	strcpy (cmd, temp.c_str());
	char *sptr, *parse;
	char *args[30];
	int i, n = 0;
	parse = strtok_r (cmd, " ", &sptr);
	args[n] = parse;
	while (parse != NULL)
	{
		parse = strtok_r (NULL, " 	", &sptr);
		++n;
		args[n] = parse;
	}

	if (execvp (args[0],args))
	{
		printf ("error");
	}
}


void proceed (Node *node, int cpipe[])
{
	if (node->neighbours.size())
	{
		int npipe[2];
		pipe(npipe);
		if (!fork)
		{
			dup2 (cpipe[0], 0);
			close (cpipe[0]);
			close (cpipe[1]);
			
			dup2 (npipe[1], 1);
			close (npipe[0]);
			close (npipe[1]);
			
			run (node);
			return;
		}
		char buffer[10000];
		int readed = read (npipe[0], buffer, 10000);
	
		close (npipe[0]);
		close (npipe[1]);
	
		for (int i = 0; i < node->neighbours.size(); ++i)
		{
			int testpipe[2];
			pipe (testpipe);
			write (testpipe[1], buffer,readed);
			proceed (node->neighbours[i], testpipe);
		}
	}
	else
	{
		if (!fork())
		{
			dup2 (cpipe[0], 0);
			close (cpipe[0]);
			close (cpipe[1]);
			
			run (node);
			return;
		}
		close (cpipe[0]);
		close (cpipe[1]);
		
	}
}



void start (Node *node)
{
	if (node->neighbours.size())
	{
		int cpipe[2];
		pipe (cpipe);
		if (!fork())
		{
			dup2 (cpipe[1], 1);
			close (cpipe[1]);
			close (cpipe[0]);
			run (node);
			return;
		}
		char buffer[10000];
		int readed = read (cpipe[0], buffer, 10000);
		close (cpipe[0]);
		close (cpipe[1]);

		for (int i = 0; i < node->neighbours.size(); ++i)
		{
			int npipe[2];
			pipe (npipe);
			write (npipe[1], buffer,readed);
			proceed (node->neighbours[i], npipe);
		}
	}
	else
	{
		if (!fork())
		{
			run (node);
			return;
		}
	}
}


void execute (vector <Node *> &graph)
{
	for (int i = 0; i < graph.size(); ++i)
	{
		if (!graph[i]->hasParent)
		{
			start (graph[i]);
		}
	}
	for(int i = 0; i < graph.size(); ++i)
	{
		int w;
		wait (&w);
		if (WIFEXITED(w) && WEXITSTATUS(w))
		{
			cout << "child " << i << " exitted with error: " << WEXITSTATUS(w) << endl;
		}
		else if (WIFSIGNALED(w))
		{
			cout << "child " << i << " signalled: " << WTERMSIG(w) << endl;
		}
	}
}




int main()
{
	int n;
	vector <string> lines;
	cin >> n;
	cin.get();
	for (int i = 0; i < n; ++i)
	{
		string temp;
		getline (cin, temp);
		lines.push_back (temp);
	}
	vector <Node *> graph;
	for (int i = 0; i < n; ++i)
	{
		string program;
		int j = 0;
		while (lines[i][j] != ' ')
		{
		
			program.push_back (lines[i][j]);
			++j;
		}
		string arguments;
		while (lines[i][j] != ':')
		{
			arguments.push_back (lines[i][j]);
			++j;
		}

		arguments = arguments.substr(1, arguments.size()-2);
		j += 2;
		string neighbours;
		while (j < lines[i].size())
		{
			neighbours.push_back (lines[i][j]);
			++j;
		}
		Node *temp = new Node;
		temp->program = program;
		temp->arguments = arguments;
		vector <int> links;
		if (neighbours.size())
		{
			process (links, neighbours);
		}
		temp->links = links;
		graph.push_back (temp);
	}
	link (graph);
	execute (graph);
	
	return 0;
}
