SRC = \
	src/ftp_server_poll.c \
	src/poll_loop.c \
	src/handle_poll_events.c \
	src/handle_client_ready.c \
	src/ftp_server_poll_internals.c \
	src/handle_new_connection.c \
##
## EPITECH PROJECT, 2026
## my_ftp
## File description:
## Makefile
##


# Makefile to call CMake
BUILD_DIR = build

.PHONY: all clean fclean re run

all: $(BUILD_DIR)/Makefile
	$(MAKE) -C $(BUILD_DIR)

$(BUILD_DIR)/Makefile: CMakeLists.txt
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..

clean:
	@if [ -d $(BUILD_DIR) ]; then $(MAKE) -C $(BUILD_DIR) clean; fi

fclean: clean
	rm -rf $(BUILD_DIR)

re: fclean all

run: all
	$(BUILD_DIR)/server.out & $(BUILD_DIR)/client.out

SRC_DIR = src
SRC_FILES = $(shell find $(SRC_DIR) -name '*.c')
SRC = \
	src/ftp_server_poll.c \
	src/poll_loop.c \
	src/handle_poll_events.c \
	src/handle_client_ready.c \
	src/ftp_server_poll_internals.c \

style:
	epiclang -fsyntax-only $(SRC_FILES) -Iinclude -std=c23
