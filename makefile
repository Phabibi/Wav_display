all: clean Node Wav Canvas q2

Node: node.h
	g++ -Wreturn-type -Wc++11-extensions -std=c++1y -o node node.h 

Wav: wav.h node.h	
	g++ -Wreturn-type -Wc++11-extensions -std=c++1y -o wav wav.h 
Canvas: node.h wav.h canvas.cpp Canvas.h
	g++ -std=c++11 main.cpp  `wx-config --libs` `wx-config --cxxflags ` -o Canvas	

q2: node.h base.h main.cpp 
	g++ -std=c++11 main.cpp `wx-config --libs` `wx-config --cxxflags ` -o q2	
clean:
	rm -f q2 canvas wav test *.o
