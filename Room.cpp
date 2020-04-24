#include "Room.h"

Room::Room()
{
	int temp_flag;
	ifstream dataFile;
	dataFile.open("dane.txt");
	dataFile >> n >> m;
	pixelNO = 0;
	pixelMatrix = new roomMatrix * [m];
	for (int i = 0; i < m; i++)
	{
		pixelMatrix[i] = new roomMatrix[n];
		for (int j = 0; j < n; j++)
		{
			dataFile >> temp_flag;
			pixelMatrix[i][j].pixel = temp_flag;
			pixelMatrix[i][j].camerasQuantity = 0;
			if (temp_flag == 1)
				pixelNO++;
		}
	}
	dataFile >> cameraRadius;
	dataFile.close();
}

Room::~Room()
{
	for (int i = 0; i < m; ++i)
		delete[] pixelMatrix[i];

	delete[] pixelMatrix;
}


void Room::showRoom()
{
	int i, j;
	cout << "Piksele:" << endl;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			cout << pixelMatrix[i][j].pixel << " ";
		}
		cout << endl;
	}
}

void Room::showCameras()
{
	cout << "Liczby obserwujacych kamer:" << endl;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (pixelMatrix[i][j].pixel)
				cout << pixelMatrix[i][j].camerasQuantity << " ";
			else
				cout << "- ";
		}
		cout << endl;
	}
}

int Room::getRoomsize1()
{
	return n;
}
int Room::getRoomsize2()
{
	return m;
}

int Room::getPixelNO()
{
	return pixelNO;
}

bool Room::getPixelvalue(int i, int j)
{
	if (i < 0 || j < 0 || i >= n || j >= m || !pixelMatrix[j][i].pixel)
		return false;
	return true;
}

double Room::getRadius()
{
	return cameraRadius;
}

int Room::findMin()
{
	int min = INT16_MAX;

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (pixelMatrix[i][j].pixel && pixelMatrix[i][j].camerasQuantity < min)
				min = pixelMatrix[i][j].camerasQuantity;
		}
	}

	return min;
}


void Room::positionCamera(int x, int y, CameraPattern *pattern)
{
	int patternCameraPos = pattern->getCameraPosition();
	setObservation(pattern, x, y, patternCameraPos, patternCameraPos);//miejsce gdzie stoi kamera

	//przegladanie tablicy ze wzorcem kamery po obwodkach kwadratu
	for (int i = 1; i <= patternCameraPos; ++i)
	{
		for (int j = 0; j < 2 * i + 1; ++j)
		{
			setObservation(pattern, x, y, patternCameraPos + j - i, patternCameraPos + 0 - i);
			setObservation(pattern, x, y, patternCameraPos + j - i, patternCameraPos + 0 + i);
		}

		for (int j = 0; j < 2 * i - 1; ++j)
		{
			setObservation(pattern, x, y, patternCameraPos + 0 - i, patternCameraPos + j - i + 1);
			setObservation(pattern, x, y, patternCameraPos + 0 + i, patternCameraPos + j - i + 1);
		}
	}
	pattern->resetHidden();
}

void Room::setObservation(CameraPattern* pattern, int x, int y, int i2, int j2)
{
	int patternCameraPos = pattern->getCameraPosition();
	if (pattern->getPixel(j2, i2) && !pattern->isPixelHidden(j2, i2))
	{
		//xp, yp - wspolrzedne piksela  w pokoju
		int xp = x - patternCameraPos + i2;
		int yp = y - patternCameraPos + j2;
		if (xp >= 0 && yp >= 0 && xp < n && yp < m)
		{
			if (pixelMatrix[yp][xp].pixel)
				++pixelMatrix[yp][xp].camerasQuantity;
			else
			{
				pattern->setHiddenPixels(j2, i2);
			}
		}
	}
}

void Room::clearCameras()
{
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			pixelMatrix[i][j].camerasQuantity = 0;
		}
	}
}
