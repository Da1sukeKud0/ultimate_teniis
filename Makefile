CXX=g++
LD=g++
#CXXFLAGS=-std=c++0x `pkg-config --cflags gtkmm-3.0`
#LDFLAGS=`pkg-config --libs gtkmm-3.0`
CXXFLAGS=-std=c++0x `pkg-config --cflags gtkmm-2.4 gtkglext-1.0`
LDFLAGS=`pkg-config --libs gtkmm-2.4 gtkglext-1.0`
PROGRAM=sample
SRC_DIR:=./src
SRCS:=$(wildcard src/*.cpp)
OBJ_DIR:=./src
OBJS:=$(addprefix src/,$(notdir $(SRCS:.cpp=.o)))
.SUFFIXES: .o .cpp
.PHONY: all clean

all: $(PROGRAM)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(PROGRAM): $(OBJS)
	$(LD) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(TARGET) $(OBJS)
