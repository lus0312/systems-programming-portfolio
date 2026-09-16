COMPOSE = docker compose -f srcs/docker-compose.yml
DATA_DIR = /home/eulee/data

all:
	mkdir -p $(DATA_DIR)/mariadb
	mkdir -p $(DATA_DIR)/wordpress
	$(COMPOSE) up -d --build

down:
	$(COMPOSE) down

clean:
	$(COMPOSE) down

re: down all

.PHONY: all down clean re


