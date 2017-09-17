CC = g++
CFLAGS =-L/usr/X11R6/lib -lX11 -lstdc++ 

FILES = XPlatformer.cpp
TARGET = XPlatformer

all: build clean

clean:
	rm -f $(OBJS) *.o

purge:
	rm -f $(OBJS) *.o $(TARGET)

build:
	g++ --std=c++11 -o $(TARGET) $(TARGET).cpp $(CFLAGS)

run:
	@echo "Running..."
	./$(TARGET)




