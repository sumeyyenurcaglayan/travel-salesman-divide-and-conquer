#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <math.h>
#include <algorithm> 
#include <chrono>
using namespace std;

class city {

public:
	int x_coord;
	int y_coord;
	int c_no;

	city(int x, int y, int no) {
		x_coord = x;
		y_coord = y;
		c_no = no;
	}

	bool operator < (city c1)
	{
		return this->x_coord < c1.x_coord;
	}
};

float Distance(city a, city b) {
	return sqrt(pow(a.x_coord - b.x_coord, 2) + pow(a.y_coord - b.y_coord, 2));
}

float routeCost(vector<city> a){

	float total = 0;

	for (int i = 0; i < a.size() - 1; i++) {
		total += Distance(a[i], a[i + 1]);
	}
	return total;
}
vector<city> shortest_tour(vector<vector<city>> total) {
	float min = routeCost(total[0]);
	vector<city> res = total[0];
	for (int i = 1; i < total.size(); i++)
	{
		float temp = routeCost(total[i]);
		if (temp < min) {
			min = temp;
			res = total[i];
		}
	}

	/*if (res.size() == 48) {      //for tour
		for (auto c : res) {
			cout << c.c_no << endl;
		}
	}*/
	return res;
}


vector<city> exhaustive_tsp(vector<city> a) {
	vector<vector<city>> total;

	do {
		total.push_back(a);

	} while (next_permutation(a.begin(), a.end()));

	return shortest_tour(total);
}

vector<vector<city>> split_cities(vector<city> a) {
	int min_x = INT_MAX, min_y = INT_MAX, max_x = INT_MIN, max_y = INT_MIN;

	for (int i = 0; i < a.size(); i++)
	{
		city temp = a[i];
		if (temp.x_coord < min_x)
		{
			min_x = temp.x_coord;
		}
		if (temp.y_coord < min_y)
		{
			min_y = temp.y_coord;
		}
		if (temp.x_coord > max_x)
		{
			max_x = temp.x_coord;
		}
		if (temp.y_coord > max_y)
		{
			max_y = temp.y_coord;
		}
	}

	float distance_x = max_x - min_x;
	float distance_y = max_y - min_y;

	if (distance_x > distance_y)
	{

		sort(a.begin(), a.end(), [](const city& city1, const city& city2) -> bool { return city1.x_coord > city2.x_coord; });

	}
	else {

		sort(a.begin(), a.end(), [](const city& city1, const city& city2) -> bool { return city1.y_coord > city2.y_coord; });

	}

	int half = a.size() / 2;

	vector<vector<city>> halfways;
	halfways.push_back(vector<city>(a.begin(), a.begin() + half));

	halfways.push_back(vector<city>(a.begin() + half, a.end()));

	return halfways;

}
vector<vector<city>> rotation(vector<city> sequence) {
	vector<vector<city>> res;

	for (int i = 0; i < sequence.size(); i++) {
		vector<city> temp(sequence.begin() + i, sequence.end());
		temp.insert(temp.end(), sequence.begin(), sequence.begin() + i);
		res.push_back(temp);
	}

	
	return res;
}


vector<city> join_tours(vector<city> half1, vector<city> half2) {
	vector<vector<city>> half1_rot = rotation(half1), half2_rot = rotation(half2);	vector<vector<city>> res;		for (int i = 0; i < half1_rot.size(); i++) {		for (int j = 0; j < half2_rot.size(); j++) {			vector<city> t(half2_rot[j]);			vector<vector<city>> s2;			s2.push_back(t);			reverse(t.begin(), t.end());			s2.push_back(t);			for (int a = 0; a < s2.size(); a++) {				vector<city> total;				total.insert(total.end(), half1_rot[i].begin(), half1_rot[i].end());				total.insert(total.end(), s2[a].begin(), s2[a].end());				res.push_back(total);			}		}	}	return shortest_tour(res);
}

vector<city> divide_tsp(vector<city> a, int n = 6) {
	if (a.size() <= n) {
		return exhaustive_tsp(a);
	}

	else
	{
		vector<vector<city>> half = split_cities(a);
		return join_tours(divide_tsp(half[0]), divide_tsp(half[1]));
	}
}

int main() {

	auto t1 = chrono::high_resolution_clock::now();

	vector<city> coords;

	ifstream reader("att48_xy.txt");

	int a, b;
	int i = 0;

	while (reader >> a >> b)
	{
		coords.push_back(city(a, b, i++));
	}


	vector<city> res = divide_tsp(coords);
	cout << routeCost(res) << endl;

	auto t2 = chrono::high_resolution_clock::now();

	auto duration = chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	cout << "Execution Time: " << (float)duration / 1000000<<endl;
	
	system("pause");

	return 0;
}
