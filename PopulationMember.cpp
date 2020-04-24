#include "PopulationMember.h"

populationMember::populationMember()
{
	cameraQuantity = 0;
	pixelWithMinCameras = 0;
	optimized_factor = 0;
	sigmaX = 0;
	sigmaY = 0;
}

populationMember::populationMember(populationMember& member)
{
	cameraQuantity = member.cameraQuantity;
	pixelWithMinCameras = member.pixelWithMinCameras;
	optimized_factor = member.optimized_factor;
	sigmaX = member.sigmaX;
	sigmaY = member.sigmaY;

	cameraPosition pos;
	for (int i = 0; i < cameraQuantity; ++i)
	{
		pos.x = member.cameraLocations[i].x;
		pos.y = member.cameraLocations[i].y;
		cameraLocations.push_back(pos);
	}
}

void populationMember::randomParams(int n, int m, int camera_max, Room* room1)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	cameraQuantity = generator() % camera_max + 1;
	for (int i = 0; i < cameraQuantity; i++)
	{
		cameraPosition pom1;
		seed = std::chrono::system_clock::now().time_since_epoch().count();         //losuję pierwszą współrzędną danej kamery;
		pom1.x = generator() % n;
		seed = std::chrono::system_clock::now().time_since_epoch().count();
		pom1.y = generator() % m;                                                      //losuję drugą wspólrzędną danej kamery;
		if (room1->getPixelvalue(pom1.x, pom1.y))        //sprawdzam czy współrzędne kamery mieszczą się w stworzonym pokoju;
		{
			cameraLocations.push_back(pom1);
		}
		else i--;
	}
	setSigmaX();
	setSigmaY();
}

void populationMember::printMemberDetails()
{
	cout.width(STAT_WIDTH);
	cout << cameraQuantity;
	cout.width(STAT_WIDTH);
	cout << optimized_factor << endl;
}

void populationMember::printCameraPositions()
{
	cout << "Pozycje kamer: " << endl;
	for (int i = 0; i < cameraQuantity; i++)
	{
		cout << cameraLocations[i].x << " " << cameraLocations[i].y << endl;
	}
}

int populationMember::checkMember(Room* room, CameraPattern* pattern)
{
	room->clearCameras();
	for (int i = 0; i < cameraQuantity; ++i)
	{
		//"nakladanie" obszaru obserwowanego przez kazda z kamer na pokój
		room->positionCamera(cameraLocations[i].x, cameraLocations[i].y, pattern);
	}

	pixelWithMinCameras = room->findMin();
	if (pixelWithMinCameras >= K)
	{
		optimized_factor = cameraQuantity;
	}
	else {
		optimized_factor = INT_MAX;
	}

	return pixelWithMinCameras; //przejrzenie calego pokoju w poszukiwaniu najgorszego piksela
}

void populationMember::setSigmaX() {
	double sum = 0;
	vector<cameraPosition>::iterator ite;
	for (ite = cameraLocations.begin(); ite != cameraLocations.end(); ++ite)
	{
		sum = sum + ite->x;
	}
	double avg = static_cast<double>(sum) / static_cast<double>(cameraQuantity);
	sum = 0;

	for (ite = cameraLocations.begin(); ite != cameraLocations.end(); ++ite)
	{
		sum = sum + (static_cast<double>(ite->x) - avg)* (static_cast<double>(ite->x) - avg);
	}
	sigmaX = sqrt(sum / static_cast<double>(cameraQuantity));
}

void populationMember::setSigmaY() {
	double sum = 0;
	vector<cameraPosition>::iterator ite;
	for (ite = cameraLocations.begin(); ite != cameraLocations.end(); ++ite)
	{
		sum = sum + ite->y;
	}
	double avg = static_cast<double>(sum) / static_cast<double>(cameraQuantity);
	sum = 0;

	for (ite = cameraLocations.begin(); ite != cameraLocations.end(); ++ite)
	{
		sum = sum + (static_cast<double>(ite->y) - avg)* (static_cast<double>(ite->y) - avg);
	}
	sigmaY = sqrt(sum / static_cast<double>(cameraQuantity));
}

int populationMember::getCameraQuantity()
{
	return cameraQuantity;
}

int populationMember::getCameraLocationX(int i) {
	if (cameraQuantity > i)
		return cameraLocations[i].x;
	else return -1;
}
int populationMember::getCameraLocationY(int i) {
	if (cameraQuantity > i)
		return cameraLocations[i].y;
	else return -1;
}

double populationMember::getSigmaX() {
	return sigmaX;
}
double populationMember::getSigmaY() {
	return sigmaY;
}

int populationMember::getOptimizedFactor() const {
	return optimized_factor;
}

void populationMember::crossMembers(populationMember& P1, populationMember& P2, Room* room1)
{
	if (P1.getOptimizedFactor() == INT_MAX || P2.getOptimizedFactor() == INT_MAX)
	{
		cameraQuantity = (P1.getCameraQuantity() + P2.getCameraQuantity()) / 2;
	}
	else {
		cameraQuantity = min(P1.getCameraQuantity(), P2.getCameraQuantity());
		cameraQuantity--;
	}
	sigmaX = (P1.getSigmaX() + P2.getSigmaX()) / 2;
	sigmaY = (P1.getSigmaY() + P2.getSigmaY()) / 2;
	int avgX, avgY, divider;
	for (int i = 0; i < cameraQuantity; i++)
	{
		avgX = 0; avgY = 0; divider = 0;
		if (P1.getCameraLocationX(i) != -1)
		{
			avgX = avgX + P1.getCameraLocationX(i);
			avgY = avgY + P1.getCameraLocationY(i);
			divider++;
		}
		if (P2.getCameraLocationX(i) != -1)
		{
			avgX = avgX + P2.getCameraLocationX(i);
			avgY = avgY + P2.getCameraLocationY(i);
			divider++;
		}
		if (divider == 0)
			return;

		avgX = avgX / divider;
		avgY = avgY / divider;
		cameraPosition pom;
		pom.x = avgX;
		pom.y = avgY;
		findExistingPixel(room1, pom); //znalezienie wspolrzednych mieszczacych sie w pokoju
		cameraLocations.push_back(pom);
	}
}

void populationMember::partialCrossMembers(populationMember& P1, populationMember& P2, Room* room1)
{
	cameraQuantity = min(P1.getCameraQuantity(), P2.getCameraQuantity());
	if (cameraQuantity != 1)
		cameraQuantity--;
	sigmaX = (P1.getSigmaX() + P2.getSigmaX()) / 2;
	sigmaY = (P1.getSigmaY() + P2.getSigmaY()) / 2;
	int pom_zam;
	if (P1.getCameraQuantity() < P2.getCameraQuantity())
	{
		for (int i = 0; i < cameraQuantity; i++)
		{
			cameraPosition pom;
			pom_zam = rand() % 10;
			if (pom_zam == 0) {
				pom.x = P2.getCameraLocationX(i);
				pom.y = P2.getCameraLocationY(i);
			}
			else {
				pom.x = P1.getCameraLocationX(i);
				pom.y = P1.getCameraLocationY(i);
			}
			findExistingPixel(room1, pom); //znalezienie wspolrzednych mieszczacych sie w pokoju
			cameraLocations.push_back(pom);
		}
	}
	else {
		for (int i = 0; i < cameraQuantity; i++)
		{
			cameraPosition pom;
			pom_zam = rand() % 10;
			if (pom_zam == 0) {
				pom.x = P1.getCameraLocationX(i);
				pom.y = P1.getCameraLocationY(i);
			}
			else {
				pom.x = P2.getCameraLocationX(i);
				pom.y = P2.getCameraLocationY(i);
			}
			findExistingPixel(room1, pom); //znalezienie wspolrzednych mieszczacych sie w pokoju
			cameraLocations.push_back(pom);
		}
	}
}

//wykorzystana Transformacja Boxa-Mullera
double populationMember::normal(const double& mean, const double& std)
{
	static const double pii = 3.1415927;
	static const double r_max = static_cast<double>(RAND_MAX) + 1;
	return std * sqrt(-2 * log((static_cast<double>(rand()) + 1) / r_max))* sin(2 * pii * rand() / r_max) + mean;
}

void populationMember::mutateMember(Room* room, int maxCamChange)
{
	//okreslenie nowych sigm
	double k = normal(0.0, 1.0); //jedna wartosc wylosowana dla wszstkich sigm danego potomka
	mutateSigma(sigmaX, k);
	mutateSigma(sigmaY, k);

	//modyfikowanie parametrow potomka
	int prevCamQuantity = cameraQuantity;
	do
	{
		if (optimized_factor < INT_MAX)
		{
			//jesli kazdy piksel jest odpowiednio obserwowany to juz tylko usuwamy kamery
			cameraQuantity = prevCamQuantity + ((rand() % maxCamChange) - maxCamChange + 1);
		}
		else
		{
			cameraQuantity = prevCamQuantity + ((rand() % (2 * maxCamChange + 1)) - maxCamChange);
		}
	} while (cameraQuantity <= 0);

	int iterations = min(prevCamQuantity, cameraQuantity);
	cameraPosition pom;
	for (int i = 0; i < iterations; ++i) //nowe wspolrzedne kamer
	{
		mutatePosition(room->getRoomsize1(), pom.x, cameraLocations[i].x, sigmaX);
		mutatePosition(room->getRoomsize2(), pom.y, cameraLocations[i].y, sigmaY);
		findExistingPixel(room, pom);
		cameraLocations[i] = pom;
	}
	for (int i = 0; i < prevCamQuantity - cameraQuantity; ++i)//jesli teraz jest mniej kamer niz bylo wczesniej to usuwamy z wektora ostatnie z nich
	{
		cameraLocations.pop_back();
	}
	int randLocation;
	for (int i = 0; i < cameraQuantity - prevCamQuantity; ++i)//jesli teraz jest wiecej kamer niz poprzednio to losujemy polozenie z juz istniejacych i je mutujemy
	{
		randLocation = rand() % cameraLocations.size();
		mutatePosition(room->getRoomsize1(), pom.x, cameraLocations[randLocation].x, sigmaX);
		mutatePosition(room->getRoomsize2(), pom.y, cameraLocations[randLocation].y, sigmaY);
		findExistingPixel(room, pom);
		cameraLocations.push_back(pom);
	}
}

void populationMember::mutateSigma(double& sigma, double ksi)
{
	double T = 1 / sqrt(2 * sqrt(nT));
	double Tprim = 1 / sqrt(2 * nT);
	double ksi2 = normal(0.0, 1.0);
	sigma *= exp(Tprim * ksi + T * ksi2);
}

void populationMember::findExistingPixel(Room* room1, cameraPosition& pom)
{
	int j = 1;
	while (!room1->getPixelvalue(pom.x, pom.y)) //sprawdzam czy współrzędne kamery mieszczą się w stworzonym pokoju;
	{
		if (room1->getPixelvalue(pom.x + j, pom.y + j))
		{
			pom.x += j;
			pom.y += j;
			break;
		}
		if (room1->getPixelvalue(pom.x + j, pom.y - j))
		{
			pom.x += j;
			pom.y -= j;
			break;
		}
		if (room1->getPixelvalue(pom.x - j, pom.y + j))
		{
			pom.x -= j;
			pom.y += j;
			break;
		}
		if (room1->getPixelvalue(pom.x - j, pom.y - j))
		{
			pom.x -= j;
			pom.y -= j;
			break;
		}
		j++;
	}
}

void populationMember::mutatePosition(int range, int& position, int prevPosition, double sigma)
{
	do
	{
		position = prevPosition + sigma * normal(0.0, 1.0);
	} while (position < 0 || position >= range);
}

void populationMember::deleteRepeats(Room* room1)
{
	roomMatrix** camerasInPixel = new roomMatrix * [room1->getRoomsize2()];
	for (int i = 0; i < room1->getRoomsize2(); ++i)
	{
		camerasInPixel[i] = new roomMatrix[room1->getRoomsize1()];
		for (int j = 0; j < room1->getRoomsize1(); ++j)
		{
			camerasInPixel[i][j].camerasQuantity = 0;
		}
	}

	for (int i = 0; i < cameraQuantity; i++)
	{
		if (camerasInPixel[cameraLocations[i].y][cameraLocations[i].x].camerasQuantity == 0)
			camerasInPixel[cameraLocations[i].y][cameraLocations[i].x].camerasQuantity++;
		else {
			cameraLocations.erase(cameraLocations.begin() + i);
			cameraQuantity--;
		}
	}

	for (int i = 0; i < room1->getRoomsize2(); ++i)
		delete[] camerasInPixel[i];

	delete[] camerasInPixel;

	return;
}
