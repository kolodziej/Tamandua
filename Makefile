CXX=g++
CXXFLAGS=-std=c++11 -I./headers
LIBS=-lboost_system
OBJS=src/group.o src/participant.o src/hidden_participant.o src/message_buffer.o src/private_room.o src/room.o src/root.o src/server.o src/user.o src/client.o src/user_message_interpreter.o src/message_composer.o
LIB_TARGET=libtamandua.a

$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS+= -g -DDEBUG
debug: LIB_PATH=lib/Debug/$(LIB_TARGET)
debug: all

release: LIB_PATH=lib/Release/$(LIB_TARGET)
release: all

all: $(OBJS)
	ar rcvs $(LIB_PATH) $(OBJS)

cleanup:
	rm -f $(OBJS)
