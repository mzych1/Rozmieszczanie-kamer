#include "CameraPattern.h"

CameraPattern::CameraPattern(double R)
{
	radius = R;
	cameraPosition = (int)R;
	dim = 2 * cameraPosition + 1;
	tab = new patternPixel * [dim]; //przydzielenie pamieci na dim wierszy

	for (int i = 0; i < dim; ++i)
		tab[i] = new patternPixel[dim];
	calculateRange();
	resetHidden();
}

CameraPattern::~CameraPattern()
{
	for (int i = 0; i < dim; ++i)
		delete[] tab[i];

	delete[] tab;
}

void CameraPattern::showTable() //pokazuje zasieg kamery
{
	cout << "Kropkami sa zaznaczone piksele 'widziane' przez kamere umieszczona w srodku - oznaczone jako x" << endl;
	for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			if (j == cameraPosition && i == cameraPosition)
				cout << "x ";
			else
			{
				if (tab[i][j].isObserved)
					cout << ". ";
				else
					cout << "  ";
			}
		}
		cout << endl;
	}
}

void CameraPattern::calculateRange()
{
	//zakladamy ze piksel jest obserwowany przez kamere jesli punkt bedacy jego srodkiem nalezy do kola o promieniu zasiegu kamery i o srodku w 
	//punkcie w ktorym znajduje sie kamera

	double radius2 = radius * radius;
	int tmpValue;

	for (int i = 0; i < dim; ++i) //i jest na osi y-kow
	{
		for (int j = 0; j < dim; ++j) //j jest na osi x-ow
		{
			tmpValue = (i - cameraPosition) * (i - cameraPosition) + (j - cameraPosition) * (j - cameraPosition);
			tab[i][j].isObserved = (double)tmpValue <= radius2;
		}
	}
}

int CameraPattern::getDim()
{
	return dim;
}

int CameraPattern::getCameraPosition()
{
	return cameraPosition;
}

bool CameraPattern::getPixel(int i, int j)
{
	return tab[i][j].isObserved;
}

bool CameraPattern::isPixelHidden(int i, int j)
{
	return tab[i][j].isHidden;
}

void CameraPattern::resetHidden()
{
	for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			tab[i][j].isHidden = false;
		}
	}
}

void CameraPattern::setHiddenPixels(int i, int j) //i w pionie, j w poziomie
{
	double xa, ya, xb, yb, a1, b1, a2, b2;
	double camPosDouble = static_cast<double>(cameraPosition);
	//8 ronych sytuacji z prostymi
	if (j != cameraPosition) //sytuacje 1,3,4,5,7,8
	{
		if (j > cameraPosition&& i < cameraPosition) //sytuacja 1
		{
			xa = j - 0.5;
			ya = i - 0.5;
			xb = j + 0.5;
			yb = i + 0.5;
		}
		else if (j > cameraPosition&& i == cameraPosition) //sytuacja 8
		{
			xa = j - 0.5;
			ya = i - 0.5;
			xb = j - 0.5;
			yb = i + 0.5;
		}
		else if (j > cameraPosition&& i > cameraPosition) //sytuacja 7
		{
			xa = j + 0.5;
			ya = i - 0.5;
			xb = j - 0.5;
			yb = i + 0.5;
		}
		else if (j < cameraPosition && i > cameraPosition) //sytuacja 5
		{
			xa = j - 0.5;
			ya = i - 0.5;
			xb = j + 0.5;
			yb = i + 0.5;
		}
		else if (j < cameraPosition && i == cameraPosition) //sytuacja 4
		{
			xa = j + 0.5;
			ya = i - 0.5;
			xb = j + 0.5;
			yb = i + 0.5;
		}
		else if (j < cameraPosition && i < cameraPosition) //sytuacja 3
		{
			xa = j + 0.5;
			ya = i - 0.5;
			xb = j - 0.5;
			yb = i + 0.5;
		}

		a1 = (camPosDouble - ya) / (camPosDouble - xa);
		b1 = camPosDouble - a1 * camPosDouble;
		a2 = (camPosDouble - yb) / (camPosDouble - xb);
		b2 = camPosDouble - a2 * camPosDouble;

		for (int i1 = 0; i1 < dim; ++i1) //j1 w pionie, i1 w poziomie
		{
			for (int j1 = 0; j1 < dim; ++j1)
			{
				if (static_cast<double>(j1) > a1* static_cast<double>(i1) + b1 && static_cast<double>(j1) < a2* static_cast<double>(i1) + b2)
				{
					tab[j1][i1].isHidden = true;
				}
			}
		}
	}
	else if (i < cameraPosition) //sytuacja 2
	{
		xa = j - 0.5;
		ya = i + 0.5;
		xb = j + 0.5;
		yb = i + 0.5;

		a1 = (camPosDouble - ya) / (camPosDouble - xa);
		b1 = camPosDouble - a1 * camPosDouble;
		a2 = (camPosDouble - yb) / (camPosDouble - xb);
		b2 = camPosDouble - a2 * camPosDouble;

		for (int i1 = 0; i1 < dim; ++i1) //j1 w pionie, i1 w poziomie
		{
			for (int j1 = 0; j1 < dim; ++j1)
			{
				if (static_cast<double>(j1) < a1* static_cast<double>(i1) + b1 && static_cast<double>(j1) < a2* static_cast<double>(i1) + b2)
				{
					tab[j1][i1].isHidden = true;
				}
			}
		}
	}
	else //sytuacja 6
	{
		xa = j - 0.5;
		ya = i - 0.5;
		xb = j + 0.5;
		yb = i - 0.5;

		a1 = (camPosDouble - ya) / (camPosDouble - xa);
		b1 = camPosDouble - a1 * camPosDouble;
		a2 = (camPosDouble - yb) / (camPosDouble - xb);
		b2 = camPosDouble - a2 * camPosDouble;

		for (int i1 = 0; i1 < dim; ++i1) //j1 w pionie, i1 w poziomie
		{
			for (int j1 = 0; j1 < dim; ++j1)
			{
				if (static_cast<double>(j1) > a1* static_cast<double>(i1) + b1 && static_cast<double>(j1) > a2* static_cast<double>(i1) + b2)
				{
					tab[j1][i1].isHidden = true;
				}
			}
		}
	}

}