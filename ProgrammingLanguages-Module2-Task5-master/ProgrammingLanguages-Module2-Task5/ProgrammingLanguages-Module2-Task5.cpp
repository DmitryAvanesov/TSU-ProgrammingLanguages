#include <iostream>
#include <queue>
#include <utility>
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>
#include <string>
#include "UserInterruptHandler.h"
using namespace std;

int AMOUNT_OF_TYPES = 3;
int MAX_WAIT_DEVICE = 60;
int MAX_WAIT_GENERATOR = 5;

mutex thisMutex;

auto cmp = [](pair<int, int> left, pair<int, int> right) { return left.second < right.second; };
priority_queue < pair<int, int>, vector<pair<int, int>>, decltype(cmp) > requestQueue(cmp);

vector<vector<pair<int, string>>> devicesStatus;
bool isFinished = false;

// input a number obviously
int inputNumber();
// creating a 2d vector of threads
void createDeviceArray(vector<vector<thread>>& deviceGroups,
	int amountOfGroups, int amountOfDevices);
// separate thread function for the generator
void addRequest(int amountOfGroups, int storageCapacity);
// process a request for random amount of time
void handleRequest(int group, int device);
// the current system state
void printState(int amountOfGroups, int amountOfDevices);
// wait for all the threads to execute
void joinThreads(vector<vector<thread>>& deviceGroups,
	int amountOfGroups, int storageCapacity);


int main()
{
	UserInterruptHandler handler;

	cout << "Amount of groups: ";
	int amountOfGroups = inputNumber();
	cout << "Amount of devices: ";
	int amountOfDevices = inputNumber();
	cout << "Storage capacity: ";
	int storageCapacity = inputNumber();

	devicesStatus = vector<vector<pair<int, string>>>
		(amountOfGroups, vector<pair<int, string>>(amountOfDevices, make_pair(0, "")));

	thread generator(addRequest, amountOfGroups, storageCapacity);
	vector<vector<thread>> deviceGroups(amountOfGroups);
	createDeviceArray(deviceGroups, amountOfGroups, amountOfDevices);

	chrono::high_resolution_clock::time_point lastTimePoint = chrono::high_resolution_clock::now();

	while (!handler.checkInterruption())
	{
		if (chrono::duration_cast<chrono::seconds>
			(chrono::high_resolution_clock::now() - lastTimePoint).count() > 1)
		{
			printState(amountOfGroups, amountOfDevices);
			lastTimePoint = chrono::high_resolution_clock::now();
		}
	}

	isFinished = true;
	generator.join();
	joinThreads(deviceGroups, amountOfGroups, amountOfDevices);

	return 0;
}


int inputNumber()
{
	int number;
	cin >> number;

	if (number > 2)
	{
		return number;
	}

	return inputNumber();
}

void createDeviceArray(vector<vector<thread>>& deviceGroups,
	int amountOfGroups, int amountOfDevices)
{
	for (int i = 0; i < amountOfGroups; i++)
	{
		for (int j = 0; j < amountOfDevices; j++)
		{
			deviceGroups[i].push_back(thread(handleRequest, i, j));
		}
	}
}

void addRequest(int amountOfGroups, int storageCapacity)
{
	srand(time(NULL));

	while (!isFinished) {
		this_thread::sleep_for(chrono::seconds(rand() % (MAX_WAIT_GENERATOR - 1) + 1));

		if (requestQueue.size() < storageCapacity) {
			thisMutex.lock();
			requestQueue.push(make_pair(rand() % amountOfGroups, rand() % AMOUNT_OF_TYPES));
			thisMutex.unlock();
		}
	}
}

void handleRequest(int group, int device)
{
	int sleepDuration;
	bool isSleeping = false;

	while (!isFinished) {
		isSleeping = false;
		thisMutex.lock();

		if (!requestQueue.empty() && requestQueue.top().first == group && devicesStatus[group][device].first == 0) {
			isSleeping = true;
			devicesStatus[group][device].second = to_string(requestQueue.top().second);
			requestQueue.pop();
		}

		thisMutex.unlock();
		srand(time(NULL));

		if (isSleeping) {
			sleepDuration = rand() % (MAX_WAIT_DEVICE - 1) + 1;

			thisMutex.lock();
			devicesStatus[group][device].first = sleepDuration;
			thisMutex.unlock();

			this_thread::sleep_for(chrono::seconds(sleepDuration));

			while (devicesStatus[group][device].first != 0) {
				this_thread::sleep_for(chrono::seconds(1));
			}

			devicesStatus[group][device].second = "";
			isSleeping = false;
		}
	}
}

void printState(int amountOfGroups, int amountOfDevices)
{
	cout << "\nRequests in the queue: " << requestQueue.size() << "\n";
	for (int i = 0; i < amountOfGroups; i++) {
		for (int j = 0; j < amountOfDevices; j++) {
			thisMutex.lock();

			cout << "Group #" << i << "\t| Device #" << j <<
				"\t| Type #" << devicesStatus[i][j].second <<
				"\t| Sec left: " << devicesStatus[i][j].first;

			if (devicesStatus[i][j].first > 0) {
				cout << "\t||  BUSY\n";
				devicesStatus[i][j].first--;
			}
			else {
				cout << "\t||  FREE\n";
			}

			thisMutex.unlock();
		}
	}
}

void joinThreads(vector<vector<thread>>& deviceGroups,
	int amountOfGroups, int amountOfDevices)
{
	cout << "\nWait for all the devices to finish execution";

	for (int i = 0; i < amountOfGroups; i++)
	{
		for (int j = 0; j < amountOfDevices; j++)
		{
			devicesStatus[i][j].first = 0;
			deviceGroups[i][j].join();
		}
	}

	cout << "\nAll devices finished execution";
}