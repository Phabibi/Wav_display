all: q2

q2: base.h main.cpp 
	g++ main.cpp `wx-config --libs` `wx-config --cxxflags` -o q2	
clean:
	rm -f q2 *.0
