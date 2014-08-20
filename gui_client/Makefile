CXX=g++
WX_VERSION=3.0
CXXFLAGS=`wx-config --version=$(WX_VERSION) --cxxflags` -std=c++11 -I./headers -I../headers -L../lib/Debug -DDEBUG
LIBS=-ltamandua `wx-config --version=$(WX_VERSION) --libs` -lboost_system
TARGET=bin/Debug/TamanduaGUIClient
OBJS=src/gui_client.o src/main_frame.o src/tamandua_textctrl.o main.o

$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -pthread -o $(TARGET)

clean:
	rm -f $(OBJS)
