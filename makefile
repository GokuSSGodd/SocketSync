#--------------------=-----------------------------
# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g

# Targets
TARGET_MOM = mom
TARGET_KID = kid

# Source files
MOM_SRCS = main.cpp Mom.cpp Printer.cpp Kid.cpp Job.cpp tools.cpp
KID_SRCS = kidmain.cpp Kid.cpp Job.cpp Printer.cpp tools.cpp

# Object files
MOM_OBJS = $(MOM_SRCS:.cpp=.o)
KID_OBJS = $(KID_SRCS:.cpp=.o)

# Default target: build both executables
all: $(TARGET_MOM) $(TARGET_KID)

# Build mom executable
$(TARGET_MOM): $(MOM_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(MOM_OBJS)

# Build kid executable
$(TARGET_KID): $(KID_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(KID_OBJS)

# Compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object and binary files
clean:
	rm -f $(MOM_OBJS) $(KID_OBJS) $(TARGET_MOM) $(TARGET_KID)

# Optional run commands
run-mom: $(TARGET_MOM)
	./$(TARGET_MOM)

run-kid-1: $(TARGET_KID)
	./$(TARGET_KID)
run-kid-2: $(TARGET_KID)
	./$(TARGET_KID)
run-kid-3: $(TARGET_KID)
	./$(TARGET_KID)
run-kid-4: $(TARGET_KID)
	./$(TARGET_KID)