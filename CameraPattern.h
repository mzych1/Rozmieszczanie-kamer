#ifndef CAMERA_PATTERN
#define CAMERA_PATTERN

#include <iostream>
using namespace std;

struct patternPixel
{
	bool isObserved;
	bool isHidden;
};

class CameraPattern
{
private:
	double radius;
	int cameraPosition;
	int dim; //wymiar tablicy
	patternPixel** tab;
	void calculateRange();
public:
	CameraPattern(double R);
	~CameraPattern();
	void showTable();
	int getDim();
	int getCameraPosition();
	bool getPixel(int i, int j);
	void resetHidden();
	void setHiddenPixels(int i, int j);
	bool isPixelHidden(int i, int j);
};

#endif // !CAMERA_PATTERN
