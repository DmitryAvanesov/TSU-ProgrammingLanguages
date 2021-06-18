#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
using namespace std;

// getting input of the amount of triangles
int inputTrianglesAmount();
// creating arrays of KMs and prices
pair <vector <int>, vector <int>> createArrays(int amount);
// filling arrays with values and sorting them
pair <vector <int>, vector <int>> fillArrays(
	pair <vector <int>, vector <int>> arrays, int count = 0);
// sorting arrays using bubbleSort()
pair <vector <int>, vector <int>> sortArrays(
	pair <vector <int>, vector <int>> arrays, int count = 0);
// swapping elements of a vector
pair <vector <int>, vector <int>> bubbleSort(
	pair <vector <int>, vector <int>> arrays, int count = 0);
// printing matching KMs and prices
void printMatches(pair <vector <int>, vector <int>> arrays, int count = 0);

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	printMatches(
		sortArrays(
			fillArrays(
				createArrays(
					inputTrianglesAmount()
				)
			)
		)
	);

	return 0;
}

int inputTrianglesAmount() {
	int amount;
	cin >> amount;

	return amount;
}

pair <vector <int>, vector <int>> createArrays(int amount) {
	vector <int> km(amount), price(amount);
	return make_pair(km, price);
}

pair <vector <int>, vector <int>> fillArrays(
	pair <vector <int>, vector <int>> arrays, int count) {

	if (count < arrays.first.size() * 2) {
		if (count < arrays.first.size()) {
			cin >> arrays.first[count];
		}
		else {
			cin >> arrays.second[count - arrays.second.size()];
		}

		return fillArrays(arrays, ++count);
	}
	else {
		return arrays;
	}
}

pair <vector <int>, vector <int>> sortArrays(
	pair <vector <int>, vector <int>> arrays, int count) {

	if (count < arrays.first.size()) {
		return sortArrays(bubbleSort(arrays), ++count);
	}
	else {
		return arrays;
	}
}

pair <vector <int>, vector <int>> bubbleSort(
	pair <vector <int>, vector <int>> arrays, int count) {

	if (count < arrays.first.size() - 1) {
		if (arrays.first[count] > arrays.first[count + 1]) {
			swap(arrays.first[count], arrays.first[count + 1]);
		}
		if (arrays.second[count] < arrays.second[count + 1]) {
			swap(arrays.second[count], arrays.second[count + 1]);
		}

		return bubbleSort(arrays, ++count);
	}
	else {
		return arrays;
	}
}

void printMatches(pair <vector <int>, vector <int>> arrays, int count) {
	if (count < arrays.first.size()) {
		cout << arrays.first[count] << " " << arrays.second[count] << "\n";
		return printMatches(arrays, ++count);
	}
}