#include <iostream>
#include <vector>
using namespace std;

// getting to know how many triangles we have
int inputAmountOfTriangles();
// creating an array of double given size
vector <vector <int>> createArrayOfCoordinates(int trianglesAmount);
// filling the first part of the array with coordinates of points
vector <vector <int>> fillArrayOfCoordinates(vector <vector <int>> triangles, int count = 0);
// the inner "loop" for coordinates' input
vector <vector <int>> inputTriangleCoordinates(vector <vector <int>> triangles,
	int currentTriangle, int count = 0);
// multiplying coordinates by the scale coefficient
vector <vector <int>> applyScaleCoef(vector <vector <int>> triangles);
// the outer "loop" for the scale coef's applying
vector <vector <int>> scaleCoefApplication(vector <vector <int>> triangles, int scaleCoef,
	int count = 0);
// the inner "loop" for the scale coef's applying
vector <vector <int>> scaleCoordinates(vector <vector <int>> triangles, int scaleCoef,
	int currentTriangle, int count = 0);
// filling the second part of the array with coordinates of vectors
vector <vector <int>> createArrayOfVectors(vector <vector <int>> triangles, int count = 0);
// printing vectors' coordinates
void printVectors(vector <vector <int>> triangles, int count = 0);
// the printing itself
void printVectorCoordinates(vector <vector <int>> triangles, int currentTriangle, int count = 0);

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	printVectors(
		createArrayOfVectors(
			applyScaleCoef(
				fillArrayOfCoordinates(
					createArrayOfCoordinates(
						inputAmountOfTriangles()
					)
				)
			)
		)
	);

	return 0;
}

int inputAmountOfTriangles() {
	int trianglesAmount;
	cin >> trianglesAmount;

	return trianglesAmount;
}

vector <vector <int>> createArrayOfCoordinates(int trianglesAmount) {
	const int coordinatesAmount = 6;
	vector <vector <int>> triangles(trianglesAmount * 2, vector <int>(coordinatesAmount));

	return triangles;
}

vector <vector <int>> fillArrayOfCoordinates(vector <vector <int>> triangles, int count) {
	if (count < triangles.size() / 2) {
		return fillArrayOfCoordinates(inputTriangleCoordinates(triangles, count), count + 1);
	}
	else {
		return triangles;
	}
}

vector <vector <int>> inputTriangleCoordinates(vector <vector <int>> triangles,
	int currentTriangle, int count) {

	if (count < triangles[currentTriangle].size()) {
		cin >> triangles[currentTriangle][count];
		return inputTriangleCoordinates(triangles, currentTriangle, ++count);
	}
	else {
		return triangles;
	}
}

vector <vector <int>> applyScaleCoef(vector <vector <int>> triangles) {
	int scaleCoef;
	cin >> scaleCoef;
	
	return scaleCoefApplication(triangles, scaleCoef);
}

vector <vector <int>> scaleCoefApplication(vector <vector <int>> triangles, int scaleCoef,
	int count) {

	if (count < triangles.size() / 2) {
		return scaleCoefApplication(scaleCoordinates(triangles, scaleCoef, count),
			scaleCoef, count + 1);
	}
	else {
		return triangles;
	}
}

vector <vector <int>> scaleCoordinates(vector <vector <int>> triangles, int scaleCoef,
	int currentTriangle, int count) {

	if (count < triangles[currentTriangle].size()) {
		triangles[currentTriangle][count] *= scaleCoef;
		return scaleCoordinates(triangles, scaleCoef, currentTriangle, ++count);
	}
	else {
		return triangles;
	}
}

vector <vector <int>> createArrayOfVectors(vector <vector <int>> triangles, int count) {
	if (count < triangles.size() / 2) {
		triangles[triangles.size() / 2 + count][0] = triangles[count][2] - triangles[count][0];
		triangles[triangles.size() / 2 + count][1] = triangles[count][3] - triangles[count][1];
		triangles[triangles.size() / 2 + count][2] = triangles[count][4] - triangles[count][2];
		triangles[triangles.size() / 2 + count][3] = triangles[count][5] - triangles[count][3];
		triangles[triangles.size() / 2 + count][4] = triangles[count][0] - triangles[count][4];
		triangles[triangles.size() / 2 + count][5] = triangles[count][1] - triangles[count][5];

		return createArrayOfVectors(triangles, ++count);
	}
	else {
		return triangles;
	}
}

void printVectors(vector <vector <int>> triangles, int count) {
	if (count < triangles.size() / 2) {
		printVectorCoordinates(triangles, triangles.size() / 2 + count);
		return printVectors(triangles, ++count);
	}
}

void printVectorCoordinates(vector <vector <int>> triangles, int currentTriangle, int count) {
	if (count < triangles[currentTriangle].size()) {
		cout << triangles[currentTriangle][count] << " ";
		return printVectorCoordinates(triangles, currentTriangle, ++count);
	}
	else {
		cout << "\n";
	}
}