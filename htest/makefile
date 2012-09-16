
#Program directory
BINDIR := 

#Program and objects
PROGRAM := $(BINDIR)Test

SYS_INCLUDE := -I/usr/local/mysql/include
SELF_INCLUDE := -Ihtest/  -Imain/  -I./

SRCS := $(shell find . -name *.cpp) 
OBJS := $(patsubst %.cpp, %.o, $(wildcard $(SRCS)))

SYS_LIB := -lpthread -lssl -luuid -lm -lz
#Compile options
CXXFLAGS := -g -O2 -Wall -D_REENTRANT -DLOGGER -D_DEBUG $(SELF_INCLUDE) $(FRAME_INCLUDE) $(COMMON_INCLUDE) $(SYS_INCLUDE)

all : $(PROGRAM)
	./$(PROGRAM)

%.o:%.cpp $(HEADS)
	$(CXX) $< -o $@ -c $(CXXFLAGS)

$(PROGRAM) : $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)
clean :
	-@$(RM) $(GCOVS) $(MAPFILE)
	-$(RM) $(OBJS)
	-$(RM) $(PROGRAM)

rebuild : clean all
