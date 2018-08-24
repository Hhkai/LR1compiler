CXX = g++

CXXFLAGS = -std=c++11

OBJS = main.o lexer.o keywords.o parser.o symbol.o reduce.o

LIBS =

TARGET = tst

$(TARGET):    $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:    $(TARGET)

clean:
ifeq ($(shell uname), Linux)
	rm $(OBJS) $(TARGET)
else
	del $(OBJS) $(TARGET).exe
endif