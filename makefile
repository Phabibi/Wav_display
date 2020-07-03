all: q2 Canvas Wav

Wav: wav.h	
	g++ -std=c++1y -o wav wav.h 
Canvas: wav.h canvas.cpp Canvas.h
	g++ main.cpp `wx-config --libs` `wx-config --cxxflags` -o Canvas	

q2: base.h main.cpp 
	g++ main.cpp `wx-config --libs` `wx-config --cxxflags` -o q2	
clean:
	rm -f q2 canvas wav test *.o
