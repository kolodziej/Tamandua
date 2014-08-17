CXX=g++
CXXFLAGS=-std=c++11 -I../headers -L../lib/Debug -g -DDEBUG
LIBS=-lboost_system
SERVER_TARGET=tamandua_server
SERVER_OBJS=server.o
CLIENT_TARGET=tamandua_client
CLIENT_OBJS=client.o

$(SERVER_OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

server: $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) $(SERVER_OBJS) $(LIBS) -ltamandua -o $(SERVER_TARGET)

client: $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) $(CLIENT_OBJS) $(LIBS) -ltamandua -o $(CLIENT_TARGET)

clean:
	rm -f *.o
