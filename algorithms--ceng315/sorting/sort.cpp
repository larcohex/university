#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <sys/time.h>


using namespace std;


int pow (int base, int exp)
{
	int result = 1;
	for (int i = 0; i < exp; ++i)
	{
		result *= base;
	}
	return result;
}


float calcTime (float speed)
{
	return 100 / speed;
}


vector <pair <int, float> > quickSort (vector <pair <int, float> > &a)
{
	int size = a.size();
	if (size == 1 || !size)
		return a;

	int median;
	vector <pair <int, float> > less;
	vector <pair <int, float> > equal;
	vector <pair <int, float> > greater;
	if (a[0].second < a[size - 1].second)
	{
		if (a[0].second >= a[size/2].second)
			median = 0;
		else
		{
			if (a[size/2].second < a[size - 1].second)
				median = size / 2;

			else
				median = size - 1;
		}
	}
	else
	{
		if (a[0].second < a[size/2].second)
			median = 0;
		else
		{
			if (a[size/2].second >= a[size - 1].second)
				median = size / 2;

			else
				median = size - 1;
		}
	}
	for (int i = 0; i < size; ++i)
	{
		if (a[i].second < a[median].second)
			less.push_back (a[i]);
		else if (a[i].second == a[median].second)
			equal.push_back (a[i]);
		else
			greater.push_back (a[i]);
	}
	less = quickSort (less);
	greater = quickSort (greater);
	less.insert (less.end(), equal.begin(), equal.end());
	less.insert (less.end(), greater.begin(), greater.end());
	return less;
}


void shellSort (vector <pair <int, float> > &a)
{
	int k = 0;
	while (pow (2, k) - 1 < a.size())
		++k;
	--k;
	while (k > 0)
	{
		int gap = pow (2, k) - 1;
		int pos = 0;
		while (pos < a.size())
		{
			for (int i = pos; i > 0 && a[i].second < a[i-gap].second; i -= gap)
			{
				pair <int, float> temp = a[i];
				a[i] = a[i-gap];
				a[i-gap] = temp;
			}
			pos += gap;
		}
		--k;
	}
}


void insertionSort (vector <pair <int, float> > &a)
{
	for (int i = 0; i < a.size(); ++i)
	{
		for (int j = i; j > 0 && a[j].second < a[j-1].second; --j)
		{
			pair <int, float> temp = a[j];
			a[j] = a[j-1];
			a[j-1] = temp;
		}
	}
}


vector <pair <int, float> > bucketSort (vector <pair <int, float> > &a)
{
	float max = 0;
	float range;
	vector <vector <pair <int, float> > > buckets;
	vector <pair <int, float> > result;
	for (int i = 0; i < 10; ++i)
	{
		vector <pair <int, float> > temp;
		buckets.push_back (temp);
	}
	for (int i = 0; i < a.size(); ++i)
	{
		if (a[i].second > max)
			max = a[i].second;
	}
	range = max / 10;
	for (int i = 0; i < a.size(); ++i)
	{
		if (a[i].second >= 0 && a[i].second < range)
			buckets[0].push_back (a[i]);
		else if (a[i].second >= range && a[i].second < range * 2)
			buckets[1].push_back (a[i]);
		else if (a[i].second >= range * 2 && a[i].second < range * 3)
			buckets[2].push_back (a[i]);
		else if (a[i].second >= range * 3 && a[i].second < range * 4)
			buckets[3].push_back (a[i]);
		else if (a[i].second >= range * 4 && a[i].second < range * 5)
			buckets[4].push_back (a[i]);
		else if (a[i].second >= range * 5 && a[i].second < range * 6)
			buckets[5].push_back (a[i]);
		else if (a[i].second >= range * 6 && a[i].second < range * 7)
			buckets[6].push_back (a[i]);
		else if (a[i].second >= range * 7 && a[i].second < range * 8)
			buckets[7].push_back (a[i]);
		else if (a[i].second >= range * 8 && a[i].second < range * 9)
			buckets[8].push_back (a[i]);
		else if (a[i].second >= range * 9 && a[i].second <= max)
			buckets[9].push_back (a[i]);
	}
	for (int i = 0; i < 10; ++i)
	{
		insertionSort (buckets[i]);
	}
	for (int i = 0; i < 10; ++i)
	{
		result.insert (result.end(), buckets[i].begin(), buckets[i].end());
	}
	return result;
}


void radixSort (vector <pair <int, float> > &a)
{
	for (int i = 0; i < 32; ++i)
	{
		unsigned int radix = (1 << i);
		pair <int, float> temp[a.size()];
		int temp0 = 0;
		int temp1 = 0;
		for (int j = 0; j < a.size(); ++j)
		{
			if (!((*(int*)(&(a[j].second))) & radix))
				++temp1;
		}
		for (int j = 0; j < a.size(); ++j)
		{
			if ((*(int*)(&(a[j].second))) & radix)
			{
				temp[temp1] = a[j];
				++temp1;
			}
			else
			{
				temp[temp0] = a[j];
				++temp0;
			}
		}
		for (int j = 0; j < a.size(); ++j)
		{
			a[j] = temp[j];
		}
	}
}


void sort (vector <pair <int, float> > &a, int type)
{
	switch (type)
	{
		case (1):
		{
			a = quickSort (a);
			break;
		}
		case (2):
		{
			shellSort (a);
			break;
		}
		case (3):
		{
			a = bucketSort (a);
			break;
		}
		case (4):
		{
			radixSort (a);
			break;
		}
	}
}


vector <pair <int, int> > quickSort2 (vector <pair <int, int> > &a)
{
	int size = a.size();
	if (size == 1 || !size)
		return a;

	int median;
	vector <pair <int, int> > less;
	vector <pair <int, int> > equal;
	vector <pair <int, int> > greater;
	if (a[0].second < a[size - 1].second)
	{
		if (a[0].second >= a[size/2].second)
			median = 0;
		else
		{
			if (a[size/2].second < a[size - 1].second)
				median = size / 2;

			else
				median = size - 1;
		}
	}
	else
	{
		if (a[0].second < a[size/2].second)
			median = 0;
		else
		{
			if (a[size/2].second >= a[size - 1].second)
				median = size / 2;

			else
				median = size - 1;
		}
	}
	for (int i = 0; i < size; ++i)
	{
		if (a[i].second < a[median].second)
			less.push_back (a[i]);
		else if (a[i].second == a[median].second)
			equal.push_back (a[i]);
		else
			greater.push_back (a[i]);
	}
	less = quickSort2 (less);
	greater = quickSort2 (greater);
	less.insert (less.end(), equal.begin(), equal.end());
	less.insert (less.end(), greater.begin(), greater.end());
	return less;
}


void shellSort2 (vector <pair <int, int> > &a)
{
	int k = 0;
	while (pow (2, k) - 1 < a.size())
		++k;
	--k;
	while (k > 0)
	{
		int gap = pow (2, k) - 1;
		int pos = 0;
		while (pos < a.size())
		{
			for (int i = pos; i > 0 && a[i].second < a[i-gap].second; i -= gap)
			{
				pair <int, int> temp = a[i];
				a[i] = a[i-gap];
				a[i-gap] = temp;
			}
			pos += gap;
		}
		--k;
	}
}


void insertionSort2 (vector <pair <int, int> > &a)
{
	for (int i = 0; i < a.size(); ++i)
	{
		for (int j = i; j > 0 && a[j].second < a[j-1].second; --j)
		{
			pair <int, int> temp = a[j];
			a[j] = a[j-1];
			a[j-1] = temp;
		}
	}
}


vector <pair <int, int> > bucketSort2 (vector <pair <int, int> > &a)
{
	int max = 0;
	float range;
	vector <vector <pair <int, int> > > buckets;
	vector <pair <int, int> > result;
	for (int i = 0; i < 10; ++i)
	{
		vector <pair <int, int> > temp;
		buckets.push_back (temp);
	}
	for (int i = 0; i < a.size(); ++i)
	{
		if (a[i].second > max)
			max = a[i].second;
	}
	range = max / 10.0;
	for (int i = 0; i < a.size(); ++i)
	{
		if (a[i].second >= 0 && a[i].second < range)
			buckets[0].push_back (a[i]);
		else if (a[i].second >= range && a[i].second < range * 2)
			buckets[1].push_back (a[i]);
		else if (a[i].second >= range * 2 && a[i].second < range * 3)
			buckets[2].push_back (a[i]);
		else if (a[i].second >= range * 3 && a[i].second < range * 4)
			buckets[3].push_back (a[i]);
		else if (a[i].second >= range * 4 && a[i].second < range * 5)
			buckets[4].push_back (a[i]);
		else if (a[i].second >= range * 5 && a[i].second < range * 6)
			buckets[5].push_back (a[i]);
		else if (a[i].second >= range * 6 && a[i].second < range * 7)
			buckets[6].push_back (a[i]);
		else if (a[i].second >= range * 7 && a[i].second < range * 8)
			buckets[7].push_back (a[i]);
		else if (a[i].second >= range * 8 && a[i].second < range * 9)
			buckets[8].push_back (a[i]);
		else if (a[i].second >= range * 9 && a[i].second <= max)
			buckets[9].push_back (a[i]);
	}
	for (int i = 0; i < 10; ++i)
	{
		insertionSort2 (buckets[i]);
	}
	for (int i = 0; i < 10; ++i)
	{
		result.insert (result.end(), buckets[i].begin(), buckets[i].end());
	}
	return result;
}


void radixSort2 (vector <pair <int, int> > &a)
{
	for (int i = 0; i < 32; ++i)
	{
		unsigned int radix = (1 << i);
		pair <int, int> temp[a.size()];
		int temp0 = 0;
		int temp1 = 0;
		for (int j = 0; j < a.size(); ++j)
		{
			if (!((*(int*)(&(a[j].second))) & radix))
				++temp1;
		}
		for (int j = 0; j < a.size(); ++j)
		{
			if ((*(int*)(&(a[j].second))) & radix)
			{
				temp[temp1] = a[j];
				++temp1;
			}
			else
			{
				temp[temp0] = a[j];
				++temp0;
			}
		}
		for (int j = 0; j < a.size(); ++j)
		{
			a[j] = temp[j];
		}
	}
}


void sort2 (vector <pair <int, int> > &a, int type)
{
	switch (type)
	{
		case (1):
		{
			a = quickSort2 (a);
			break;
		}
		case (2):
		{
			shellSort2 (a);
			break;
		}
		case (3):
		{
			a = bucketSort2 (a);
			break;
		}
		case (4):
		{
			radixSort2 (a);
			break;
		}
	}
}


int main()
{
	int sortType;
	int n;
	vector <vector <float> > speeds;
	vector <vector <float> > rests;


	string line;
	ifstream input;
	input.open("sort.inp");
	ofstream output;
	output.open ("sort.out");	// added
	if (input.is_open() && output.is_open())	// changed
	{
		input >> sortType;
		input >> n;
		for (int i = 0; i < n; ++i)
		{
			vector <float> tempSpeeds;
			vector <float> tempRests;
			for (int j = 0; j < 10; ++j)
			{
				if (j < 5)
				{
					float temp;
					input >> temp;
					tempSpeeds.push_back (temp);
				}
				else
				{
					float temp;
					input >> temp;
					tempRests.push_back (temp);
				}
			}
			speeds.push_back (tempSpeeds);
			rests.push_back (tempRests);
		}
	}

	vector <pair <int, int> > points;
	for (int i = 0; i < n; ++i)
	{
		pair <int, int> temp (i + 1, 0);
		points.push_back (temp);
	}
	for (int i = 0; i < 5; ++i)
	{
		vector <pair <int, float> > result;
		for (int j = 0; j < n; ++j)
		{
			pair <int, float> temp (points[j].first, calcTime (speeds[points[j].first-1][i]) + rests[points[j].first-1][i]);
			result.push_back (temp);
		}


		struct timeval tv_begin;
		struct timeval tv_end;
		int passed_milliseconds;
		gettimeofday (&tv_begin, NULL);
		sort (result, sortType);
		gettimeofday (&tv_end, NULL);
		passed_milliseconds = (tv_end.tv_sec - tv_begin.tv_sec) * 1000 + (tv_end.tv_usec -
tv_begin.tv_usec) / 1000;
		output << passed_milliseconds << endl;	// changed


		int newPoint = n;
		for (int j = 0; j < n; ++j)
		{
			if (j)
			{
				if (result[j].second != result[j-1].second)
					--newPoint;
				points[result[j].first-1].second += newPoint;
			}
			else
				points[result[j].first-1].second += newPoint;	
		}
	}
	struct timeval tv_begin;
	struct timeval tv_end;
	int passed_milliseconds;
	gettimeofday (&tv_begin, NULL);
	sort2 (points, sortType);
	gettimeofday (&tv_end, NULL);
	passed_milliseconds = (tv_end.tv_sec - tv_begin.tv_sec) * 1000 + (tv_end.tv_usec -
tv_begin.tv_usec) / 1000;
	output << passed_milliseconds << endl;	// changed
	
	for (int i = n - 1; i >= 0; --i)
	{
		output << points[i].second << " " << points[i].first << endl;	// changed
	}
	return 0;
}
