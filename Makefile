OBJS = Main.c Screen.c GameManager.c Map.c Player.c
CC = gcc
INCLUDE_PATHS = -IC:\mingw_dev_lib\include
LIBRARY_PATHS = -LC:\mingw_dev_lib\lib
COMPILER_FLAGS = -w 
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2_image -lSDL2
OBJ_NAME = game
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)