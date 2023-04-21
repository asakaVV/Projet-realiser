all : prog

prog : towerDefense.c
	$(CC) -std=c17 -Wall -Wfatal-errors towerDefense.c -o towerDefense -lMLV
