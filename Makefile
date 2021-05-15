# Variable declarations
CC := g++

# Compile flags
CFLAGS := -std=c++17 -Wall -DFMT_HEADER_ONLY -DGLEW_STATIC -no-pie

# Additional include directories
CINC_DIR := -I/home/pi/other_src/fmt/include/

# Additional library directories
CLIB_DIR := -L/home/pi/other_src/fmt/ #-L/usr/lib/arm-linux-gnueabihf

# Add all necessery libs that will be used in the project
CLIBS := -lpthread -lboost_filesystem -lboost_program_options -lX11 -lGL -lGLEW -lEGL -lGLESv2

all: main_prog

main_prog:
	clear
	@echo "Compiling the main ..."
	$(CC) $(CINC_DIR) $(CLIB_DIR) $(CFLAGS) $(CLIBS) -o main.out main.cpp GLXDevice.cpp WindowX11.cpp

clean:
	rm -rf *.bin main.out
