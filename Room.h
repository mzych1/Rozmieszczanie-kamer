#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <fstream>
#include "CameraPattern.h"
using namespace std;

struct roomMatrix {
	bool pixel;
	int camerasQuantity;
};

class Room {
private:
	int n, m, pixelNO;
	roomMatrix** pixelMatrix;
	double cameraRadius;

	void setObservation(CameraPattern* pattern, int x, int y, int i2, int j2);
public:
	Room();         //konstruktor wczytujący z pliku macierz pikseli
	~Room();
	void showRoom();     //funkcja drukująca na ekran macierz pikseli symbolizującą pokój
	int getRoomsize1();
	int getRoomsize2();
	int getPixelNO();
	bool getPixelvalue(int i, int j);
	double getRadius();
	int findMin();
	void showCameras();
	void positionCamera(int x, int y, CameraPattern *pattern);
	void clearCameras();
};

#endif
