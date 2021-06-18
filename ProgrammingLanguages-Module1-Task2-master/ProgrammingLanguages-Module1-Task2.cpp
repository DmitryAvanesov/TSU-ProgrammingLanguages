#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// getting a number from a user
int inputNumber();
// leaving only odd numerals in the number
int removeEvenNumerals(int n, int res = 0, int deg = 0);
// printing the resulting number
void print(int newNum);

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	print(
		removeEvenNumerals(
			inputNumber()
		)
	);

	return 0;
}

int inputNumber() {
	int n;
	cin >> n;

	return n;
}

int removeEvenNumerals(int n, int res, int deg) {
	if (n > 0) {
		int lastNum = n % 10;

		if (lastNum % 2 != 0) {
			res += lastNum * pow(10, deg++);
		}

		return removeEvenNumerals(n / 10, res, deg);
	}
	else {
		return res;
	}
}

void print(int newNum) {
	cout << newNum;
}