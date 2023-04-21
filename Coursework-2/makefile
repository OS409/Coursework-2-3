CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wpedantic

.PHONY : all clean
all : musicplayer

musicplayer : musicplayer.cpp music.o
	$(CXX) $(CXXFLAGS) -o $@ $^

music.o : music.cpp music.h
	$(CXX) $(CXXFLAGS) -c $<

clean :
	$(RM) musicplayer *.o

