#ifndef POPULATIONMEMBER_H
#define POPULATIONMEMBER_H
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <random>
#include <climits>
#include <math.h>
#include <algorithm>
#include "Room.h"
#include "CameraPattern.h"

#define STAT_WIDTH 12
#define K 3 //liczba okreslajaca minimalna liczbe kamer obserwujacych kazdy punkt
#define nT 3 

using namespace std;

struct cameraPosition {
	int x;
	int y;
};

class populationMember {
private:
	int cameraQuantity;
	vector<cameraPosition> cameraLocations;
	int pixelWithMinCameras;
	int optimized_factor;
	double sigmaX, sigmaY;
public:
	populationMember(); //konstruktor domyślny
	populationMember(populationMember &member); //konstruktor kopiujacy
    //tworzenie losowego parametru początkowego:
	void randomParams(int n, int m, int camera_max, Room *room1);
	//krzyżowanie dwóch parametrów:
	void crossMembers(populationMember &P1, populationMember &P2, Room *room1);
    //niepełne krzyżowanie dwóch parametrów:
	void partialCrossMembers(populationMember &P1, populationMember &P2, Room *room1);
	//mutowanie
	void mutateMember(Room *room, int maxCamChange);
	static void mutateSigma(double &sigma, double ksi);
	void findExistingPixel(Room *room, cameraPosition &pom);
	void mutatePosition(int range, int &position, int prevPosition, double sigma);

	void printMemberDetails();
	void printCameraPositions();
	// sprawdzanie jak obserowany jest pokój
	int checkMember(Room *r, CameraPattern *pattern);
	// obliczanie sigm
	void setSigmaX();
	void setSigmaY();
	//funkcje wyłuskania pól klasy
	int getCameraQuantity();
	int getCameraLocationX(int i);
	int getCameraLocationY(int i);
	double getSigmaX();
	double getSigmaY();
	int getOptimizedFactor() const;
	//statyczna metoda do generowania liczb z rozkładu normalnego
	static double normal(const double&, const double&);

	void deleteRepeats(Room *room1);
};

#endif
