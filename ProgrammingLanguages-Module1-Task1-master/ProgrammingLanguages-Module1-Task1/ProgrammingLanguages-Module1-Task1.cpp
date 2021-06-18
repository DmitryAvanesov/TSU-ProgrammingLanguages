#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// creating an array of given length
vector <int> createArray();
// recursively filling an array with numbers
vector <int> fillArray(vector <int> arr, int curElem);
// accumulating the sum of numbers in a given array
int arraySum(vector <int> arr);
// printing the sum out
void print(int number);

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	print(
		arraySum(
			fillArray(
				createArray(), 0
			)
		)
	);

	return 0;
}

vector <int> createArray() {
	int length;
	cin >> length;
	vector <int> array(length);

	return array;
}

vector <int> fillArray(vector <int> arr, int curElem) {
	if (curElem < arr.size()) {
		cin >> arr[curElem];
		return fillArray(arr, ++curElem);
	}
	else {
		return arr;
	}
}

int arraySum(vector <int> arr) {
	return accumulate(arr.begin(), arr.end(), 0);
}

void print(int number) {
	cout << number;
}