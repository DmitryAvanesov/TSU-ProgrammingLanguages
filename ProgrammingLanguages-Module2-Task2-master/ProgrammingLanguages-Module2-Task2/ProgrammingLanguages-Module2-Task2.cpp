#include <iostream>
#include <future>
#include <chrono>
#include <algorithm>
#include <utility>
#include <vector>
#include <random>
using namespace std;

// functions' declaration
pair<int, int> extremaElemIndexes(vector<int> subArray);
int inputArraySize();
vector<int> createArray(int arraySize);
vector<future<pair<int, int>>> createAsyncArray(vector<int> array, int numberOfTasks);
pair<pair<int, int>, pair<int, int>> getAsyncResults(
	vector<int> array, vector<future<pair<int, int>>> &asyncArray, int numberOfTasks
);
void printResults(pair<pair<int, int>, pair<int, int>> valuesAndIndexes);

// main
int main()
{
	vector<int> array = createArray(inputArraySize());

	pair<int, int> values = make_pair(INT32_MAX, INT32_MIN);
	pair<int, int> indexes;
	pair<pair<int, int>, pair<int, int>> valuesAndIndexes;

	for (int numberOfTasks = 2; numberOfTasks <= 16; numberOfTasks *= 2) {
		chrono::high_resolution_clock::time_point timeStart = chrono::high_resolution_clock::now();

		vector<future<pair<int, int>>> asyncArray = createAsyncArray(array, numberOfTasks);
		valuesAndIndexes = getAsyncResults(array, asyncArray, numberOfTasks);

		chrono::high_resolution_clock::time_point timeFinish = chrono::high_resolution_clock::now();
		cout << "time spent for " << numberOfTasks << " tasks: " <<
			chrono::duration_cast<chrono::milliseconds>(timeFinish - timeStart).count() << "ms\n\n";
	}

	printResults(valuesAndIndexes);

	return 0;
}

// other functions

pair<int, int> extremaElemIndexes(vector<int> subArray) {
	return make_pair(distance(subArray.begin(), min_element(subArray.begin(), subArray.end())),
		distance(subArray.begin(), max_element(subArray.begin(), subArray.end())));
}

int inputArraySize() {
	int size;
	cin >> size;

	if (size > 100000) {
		cout << "\n";
		return size;
	}

	return inputArraySize();
}

vector<int> createArray(int arraySize) {
	random_device rd;
	default_random_engine generator(rd());
	uniform_int_distribution<int> distribution(INT32_MIN, INT32_MAX);

	vector<int> array(arraySize);

	for (int i = 0; i < array.size(); i++) {
		array[i] = distribution(generator);
	}
	cout << "\n";

	return array;
}

vector<future<pair<int, int>>> createAsyncArray(vector<int> array, int numberOfTasks) {
	vector<future<pair<int, int>>> asyncArray(numberOfTasks);

	for (int i = 0; i < asyncArray.size(); i++) {
		vector<int>::iterator sliceBegin = array.begin() + array.size() / numberOfTasks * i;
		vector<int>::iterator sliceEnd = i != asyncArray.size() - 1 ?
			array.begin() + array.size() / numberOfTasks * (i + 1) :
			array.end();

		asyncArray[i] = async(
			launch::async,
			extremaElemIndexes,
			vector<int>(sliceBegin, sliceEnd)
		);
	}

	return asyncArray;
}

pair<pair<int, int>, pair<int, int>> getAsyncResults(
	vector<int> array, vector<future<pair<int, int>>> &asyncArray, int numberOfTasks
) {
	int valueToCompare;
	pair<int, int> currentIndexes;
	pair<pair<int, int>, pair<int, int>> valuesAndIndexes;

	for (int i = 0; i < asyncArray.size(); i++) {
		currentIndexes = asyncArray[i].get();

		valueToCompare = *(array.begin() + array.size() / numberOfTasks * i + currentIndexes.first);
		if (valueToCompare < valuesAndIndexes.first.first) {
			valuesAndIndexes.first.first = valueToCompare;
			valuesAndIndexes.second.first = array.size() / numberOfTasks * i + currentIndexes.first;
		}

		valueToCompare = *(array.begin() + array.size() / numberOfTasks * i + currentIndexes.second);
		if (valueToCompare > valuesAndIndexes.first.second) {
			valuesAndIndexes.first.second = valueToCompare;
			valuesAndIndexes.second.second = array.size() / numberOfTasks * i + currentIndexes.second;
		}
	}

	return valuesAndIndexes;
}

void printResults(pair<pair<int, int>, pair<int, int>> valuesAndIndexes) {
	cout << "min value: " << valuesAndIndexes.first.first <<
		" at index " << valuesAndIndexes.second.first << "\n";
	cout << "max value: " << valuesAndIndexes.first.second <<
		" at index " << valuesAndIndexes.second.second << "\n";
}