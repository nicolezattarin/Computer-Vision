CFLAGS = `pkg-config --cflags opencv4`
LIBS = `pkg-config --libs opencv4`

main: main.o func.o filter.o
	g++ -std=c++11 main.o func.o filter.o ${CFLAGS} ${LIBS} -o main

main.o: main.cpp func.h filter.h
	g++ -c -std=c++11 main.cpp -o main.o ${CFLAGS} ${LIBS}

%.o: %.cpp %.h
	g++ -c -std=c++11 $< ${CFLAGS} ${LIBS} -o $@

clean:
	rm *.o
	rm main