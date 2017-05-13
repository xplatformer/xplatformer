CC = g++
CFLAGS = -L/usr/X11R6/lib -lX11 -lstdc++  -std=c++11

FILES = XPlatformer.cpp
TARGET = XPlatformer

all: build clean

clean:
	rm -f $(OBJS) *.o

purge:
	rm -f $(OBJS) *.o $(TARGET)

build:
	$(CC) -o $(TARGET) $(TARGET).cpp $(CFLAGS)

run:
	@echo "Running..."
	./$(TARGET)