##
## EPITECH PROJECT, 2026
## my_ftp
## File description:
## Makefile
##


# Makefile to call CMake
BUILD_DIR = build

.PHONY: all clean fclean re run bonus

all: $(BUILD_DIR)/Makefile
	$(MAKE) -C $(BUILD_DIR)

$(BUILD_DIR)/Makefile: CMakeLists.txt
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..

clean:
	@echo "Cleaning build artifacts..."

fclean: clean
	rm -rf $(BUILD_DIR)
	rm -rf bonus/target

re: fclean all bonus

bonus:
	cd bonus && cargo build --release