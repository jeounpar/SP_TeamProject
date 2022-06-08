# all	: server client_left client_right

# server	:
# 	g++ -std=c++11 -w -o server ./src/server_main.cpp \
# 	 ./src/gpio.cpp ./src/piezo.cpp ./src/pwm.cpp \
# 	 ./src/error.cpp ./src/ft_sock.cpp \
# 	 `pkg-config --cflags --libs opencv4` -lpthread

# client_left		:
# 	g++ -std=c++11 -w -o client_left ./src/client_left_main.cpp \
# 	./src/gpio.cpp ./src/error.cpp ./src/ft_sock.cpp

# client_right	:
# 	g++ -std=c++11 -w -o client_right ./src/client_right_main.cpp \
# 	./src/gpio.cpp ./src/error.cpp ./src/ft_sock.cpp

# client	:	client_left client_right

# clean	:
# 	rm -rf server client_left client_right

# re	: all clean

all : main

main :
	g++ -std=c++11 -w -o main ./src/*.cpp

clean	:
	rm -rf main

re	: all clean