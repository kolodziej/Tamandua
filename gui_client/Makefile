CXX=g++
CXXFLAGS=`wx-config --cxxflags` -std=c++11 -I./headers -I../headers -L../lib/Debug
LIBS=`wx-config --libs`
TARGET=bin/Debug/TamanduaGUIClient
OBJS=src/gui_client.o src/main_frame.o main.o

$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $(TARGET) $(OBJS)
