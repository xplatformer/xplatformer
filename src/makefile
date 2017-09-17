CC = g++

FILES = XPlatformer.cpp
TARGET = XPlatformer

all: build clean

clean:
	rm -f $(OBJS) *.o

purge:
	rm -f $(OBJS) *.o $(TARGET)

build:
	g++ --std=c++11 $(TARGET).cpp -o $(TARGET) -L/usr/X11R6/lib -lX11 -lstdc++ -L./lib -lxgame

run:
	@echo "Running..."
	./$(TARGET)




