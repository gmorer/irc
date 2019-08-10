SERVER_NAME = server

CLIENT_NAME = client

LIB_NAME = libft.a

SERVER_DIR = server_src

CLIENT_DIR = client_src

LIB_DIR = libft

.PHONY: all server client clean fclean re

all: lib server client

lib:
		make -C $(LIB_DIR)
		ln -sf ../$(LIB_DIR)/$(LIB_NAME) $(SERVER_DIR)
		ln -sf ../$(LIB_DIR)/$(LIB_NAME) $(CLIENT_DIR)

server: lib
	make -C $(SERVER_DIR)
	ln -sf $(SERVER_DIR)/$(SERVER_NAME) .

client: lib
	make -C $(CLIENT_DIR)
	ln -sf $(CLIENT_DIR)/$(CLIENT_NAME) .

clean:
	make -C $(LIB_DIR) clean
	make -C $(SERVER_DIR) clean
	make -C $(CLIENT_DIR) clean

fclean: clean
	rm -f $(SERVER_NAME) $(CLIENT_NAME)
	make -C $(LIB_DIR) fclean
	make -C $(SERVER_DIR) fclean
	make -C $(CLIENT_DIR) fclean
	rm -f $(SERVER_DIR)/$(LIB_NAME)
	rm -f $(CLIENT_DIR)/$(LIB_NAME)

re: fclean all
