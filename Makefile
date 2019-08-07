SERVER_NAME = server

CLIENT_NAME = client

SERVER_DIR = server_src

CLIENT_DIR = client_src

.PHONY: all server client clean fclean re

all: server client

server:
	make -C $(SERVER_DIR)
	ln -sf $(SERVER_DIR)/$(SERVER_NAME) .

client:
	make -C $(CLIENT_DIR)
	ln -sf $(CLIENT_DIR)/$(CLIENT_NAME) .

clean:
	make -C $(SERVER_DIR) clean
	make -C $(CLIENT_DIR) clean

fclean: clean
	rm -f $(SERVER_NAME) $(CLIENT_NAME)
	make -C $(SERVER_DIR) fclean
	make -C $(CLIENT_DIR) fclean

re: fclean all