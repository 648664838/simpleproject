COMMONPATH	= common
CC		= gcc
CXX		= g++
CXXFLAGS 	= -g
INCPATH		= -I./inc -I./$(COMMONPATH)/tiny -I $(COMMONPATH)/mysql/inc/ -I $(COMMONPATH)/include/
LIBS     	= -L$(COMMONPATH)/mysql/lib/ -lmysqlclient  -lz 
LINK     	= g++	
####### Files

OBJECTS = ./debug/simplebasedata.o ./debug/main.o \
        ./debug/tinystr.o ./debug/tinyxml.o ./debug/tinyxmlerror.o ./debug/tinyxmlparser.o \
	./debug/server_socket.o ./debug/clienthandle.o ./debug/scenelogic.o

TARGET = ./mysimpledb

first: all
./debug/tinystr.o :  $(COMMONPATH)/tiny/tinystr.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
	
./debug/tinyxml.o : $(COMMONPATH)/tiny/tinyxml.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/tinyxmlerror.o : $(COMMONPATH)/tiny/tinyxmlerror.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/tinyxmlparser.o : $(COMMONPATH)/tiny/tinyxmlparser.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/simplebasedata.o :  ./simplebasedata.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/main.o :  ./main.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/server_socket.o :  ./server_socket.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/clienthandle.o  :  ./clienthandle.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/scenelogic.o  :  ./scenelogic.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

all : $(TARGET) 
$(TARGET): $(OBJECTS) 
	$(LINK) -o $(TARGET) $(OBJECTS) $(LIBS)
