#include <iostream>
#include <mutex>
#include <vector>
#include <chrono>
#include "UserInterruptHandler.h"
using namespace std;

// some global variables

condition_variable waitCondition;
mutex mutexVar;
bool isWaiting = true;
bool notFinished = true;

// each thread prints its number
void threadFunction(int num) {
	unique_lock<mutex> lock(mutexVar);

	while (notFinished) {
		waitCondition.wait(lock, [&] { return !isWaiting; });

		if (notFinished) {
			isWaiting = true;
			cout << num << "\n";
		}
	}

	cout << num << " finished\n";
}

int inputAmountOfThreads() {
	int n;
	cin >> n;

	if (n <= 3) {
		return inputAmountOfThreads();
	}
	else {
		return n;
	}
}

void initializeThreads(int n, vector<thread>& threads) {
	for (int i = 0; i < n; i++) {
		threads[i] = thread(threadFunction, i + 1);
	}
}

void wakeUpThread(int n, vector<thread>& threads) {
	isWaiting = false;
	waitCondition.notify_one();
}

void joinThreads(int n, vector<thread>& threads) {
	waitCondition.notify_all();

	for (int i = 0; i < n; i++) {
		threads[i].join();
	}
}

int main()
{
	UserInterruptHandler handler;
	int n;
	n = inputAmountOfThreads();
	vector <thread> threads(n);

	initializeThreads(n, threads);

	chrono::high_resolution_clock::time_point lastTimePoint = chrono::high_resolution_clock::now();

	try {
		while (!handler.checkInterruption()) {
			if (chrono::duration_cast<chrono::milliseconds>
				(chrono::high_resolution_clock::now() - lastTimePoint).count() > 500) {

				wakeUpThread(n, threads);
				lastTimePoint = chrono::high_resolution_clock::now();
			}
		}
	}
	catch (...) { }

	isWaiting = false;
	notFinished = false;
	joinThreads(n, threads);

	return 0;
}