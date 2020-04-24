#Makefile
_START__: a.out
	./a.out

clean: 
	rm -f a.out main.o Room.o PopulationMember.o CameraPattern.o

main.o: main.cpp PopulationMember.h
	g++ -c main.cpp

populationMember.o: PopulationMember.cpp PopulationMember.h
	g++ -c PopulationMember.cpp

cameraPattern.o: CameraPattern.cpp CameraPattern.h
	g++ -c CameraPattern.cpp

room.o: Room.cpp Room.h
	g++ -c Room.cpp

a.out: main.o Room.o PopulationMember.o CameraPattern.o
	g++ main.o Room.o PopulationMember.o CameraPattern.o
