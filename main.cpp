#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include "PopulationMember.h"

#define MEMBERS 200
#define ITERATIONS_MAX 100

using namespace std;

void select_next_gen_miBest(populationMember* members_tab[], populationMember* next_generation[]);
void select_next_gen_roulette(populationMember* member_tab[], populationMember* next_generation[]);
void printStatistics(int iteration);

bool sort_by_factor(populationMember* x, populationMember* y) {
	if (x->getOptimizedFactor() == INT_MAX && y->getOptimizedFactor() == INT_MAX)
		return (x->getCameraQuantity() > y->getCameraQuantity());
	else
		return (x->getOptimizedFactor() < y->getOptimizedFactor());
}

populationMember* members_tab[MEMBERS];
populationMember* next_generation[2 * MEMBERS];

int main()
{
	cout << K << endl;
	auto t1 = std::chrono::high_resolution_clock::now();
	Room* room1 = new Room;         //tworze obiekt klasy pokoj - w konstruktorze wczytywane sa dane o pokoju z pliku;
	room1->showRoom();
	int i;
	int n = room1->getRoomsize1();
	int m = room1->getRoomsize2();
	int max_cameras = room1->getPixelNO();      //maksymalna liczba kamer potrzebna do ogladania calego pokoju

	CameraPattern* pattern = new CameraPattern(room1->getRadius());
	//pattern->showTable();
	srand(time(NULL));

	for (i = 0; i < MEMBERS; i++)
	{
		populationMember* member1 = new populationMember;
		member1->randomParams(n, m, max_cameras, room1);
		members_tab[i] = member1;
		members_tab[i]->deleteRepeats(room1);
		members_tab[i]->checkMember(room1, pattern);
		member1 = nullptr;
	}
	for (int iterations = 0; iterations < ITERATIONS_MAX; iterations++)
	{
		printStatistics(iterations); //member_tab[0] musi byc wskazaniem na najlepszego osobnika w tej populacji
		for (i = 0; i < MEMBERS; i++)
		{
			if (i < MEMBERS - 1)
			{
				populationMember* next_gen_member = new populationMember;
				if (iterations < 9 * ITERATIONS_MAX / 10) {
					next_gen_member->crossMembers(*members_tab[i], *members_tab[i + 1], room1);
				}
				else {
					next_gen_member->partialCrossMembers(*members_tab[i], *members_tab[i + 1], room1);
				}
				next_generation[i] = next_gen_member;
				next_generation[i]->checkMember(room1, pattern);
			}
			else {
				populationMember* next_gen_member = new populationMember;
				if (iterations < 9 * ITERATIONS_MAX / 10) {
					next_gen_member->crossMembers(*members_tab[i], *members_tab[0], room1);
				}
				else {
					next_gen_member->partialCrossMembers(*members_tab[i], *members_tab[0], room1);
				}
				next_generation[i] = next_gen_member;
				next_generation[i]->checkMember(room1, pattern);
			}
		}
		//mutowanie
		for (i = 0; i < MEMBERS; ++i)
		{
			next_generation[i]->mutateMember(room1, max(max_cameras / 10, 2));
			next_generation[i]->deleteRepeats(room1);
			next_generation[i]->checkMember(room1, pattern);
		}
		//dodawanie do next_generation osobnikow z poprzedniego pokolenia
		for (i; i < 2 * MEMBERS; i++)
		{
			next_generation[i] = members_tab[i - MEMBERS];
		}
		sort(next_generation, next_generation + 2 * MEMBERS, sort_by_factor);

		//wybor nowego pokolenia- wybierz algorytm:
		//select_next_gen_miBest(members_tab, next_generation);
		select_next_gen_roulette(members_tab, next_generation);
	}

	cout << endl << "NAJLEPSZY POTOMEK: " << endl;
	members_tab[0]->printMemberDetails();
	members_tab[0]->printCameraPositions();
	members_tab[0]->checkMember(room1, pattern);
	room1->showCameras();

	ofstream results;
	results.open("times.txt", ios_base::app);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto dt1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	results << "TESTOWY: " << dt1 << " liczba kamer: " << members_tab[0]->getCameraQuantity() << endl;

	for (i = 0; i < MEMBERS; ++i)
	{
		delete members_tab[i];
	}

	delete pattern;
	delete room1;

	return 0;
}

void printStatistics(int iteration)
{
	if (iteration == 0)
	{
		cout << "---------------------------------------------------------" << endl;
		cout << "STATISTICS-THE BEST MEMBER'S PARAMETERS IN EACH ITERATION" << endl;
		cout.width(STAT_WIDTH); //zeby statystyki ladnie wygladaly
		cout << "Iteration";
		cout.width(STAT_WIDTH);
		cout << "Cameras";
		cout.width(STAT_WIDTH);
		cout << "Optimized" << endl;
	}
	cout.width(STAT_WIDTH);
	cout << iteration; //nr iteracji
	members_tab[0]->printMemberDetails(); // osobniki w member_details musza byc posortowane wdg rosnacego paranetru optimized_factor
}

void select_next_gen_miBest(populationMember* members_tab[], populationMember* next_generation[])
{
	int i;
	int los;
	for (i = 0; i < MEMBERS / 2; i++)
	{
		members_tab[i] = new populationMember(*next_generation[i]);
	}
	for (i; i < MEMBERS; ++i)
	{
		los = rand() % (2 * MEMBERS);
		members_tab[i] = new populationMember(*next_generation[los]);
	}
	//usuwanie niepotrzebnych osobnikow
	for (i = 0; i < 2 * MEMBERS; i++)
	{
		delete next_generation[i];
		next_generation[i] = nullptr;
	}
}

void select_next_gen_roulette(populationMember* member_tab[], populationMember* next_generation[])
{
	int pressure[4] = { 50, 85, 95, 100 };
	int i = 1;
	int group;
	int los;

	member_tab[0] = new populationMember(*next_generation[0]);
	for (i; i < MEMBERS; i++)
	{
		group = rand() % 100;
		los = rand() % (MEMBERS / 2);
		if (group < pressure[0]) {
			members_tab[i] = new populationMember(*next_generation[los]);
		}
		else if (group < pressure[1]) {
			members_tab[i] = new populationMember(*next_generation[MEMBERS / 4 + los]);
		}
		else if (group < pressure[2]) {
			members_tab[i] = new populationMember(*next_generation[2 * MEMBERS / 4 + los]);
		}
		else if (group < pressure[3]) {
			members_tab[i] = new populationMember(*next_generation[3 * MEMBERS / 4 + los]);
		}
	}

	for (i = 0; i < 2 * MEMBERS; i++)
	{
		delete next_generation[i];
		next_generation[i] = nullptr;
	}
}
