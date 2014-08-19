CXX=g++
WX_VERSION=3.0
CXXFLAGS=`wx-config --version=$(WX_VERSION) --cxxflags` -std=c++11 -I./headers -I../headers -L../lib/Debug
LIBS=-ltamandua `wx-config --version=$(WX_VERSION) --libs` -lboost_system
TARGET=bin/Debug/TamanduaGUIClient
OBJS=src/gui_client.o src/main_frame.o src/tamandua_textctrl.o main.o

$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS)
