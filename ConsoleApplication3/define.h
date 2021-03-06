#pragma once
#define PRAWO 0x4d
#define LEWO 0x4b
#define GORA 0x48
#define DOL 0x50
#define ZERO 0x30 //znak 0
#define JEDEN 0x31 //znak 1
#define CLEAN 0x35 //czyszczenie znaku za pomocą klawisza 5
#define ESC 0x1b
#define ENTER 0x0d
#define SPACJA ' '

#define START_POS_LEG_X 2 // pozycja początkowa legendy na osi X 
#define START_POS_LEG_Y 2 // pozycja początkowa legendy na osi Y 
#define START_POS_MAP_X 50 // pozycja początkowa planszy na osi X 
#define START_POS_MAP_Y 2  // pozycja początkowa planszy na osi Y 
#define START_POS_CUR_X (START_POS_MAP_X + 1) // pozycja początkowa kursora na osi X 
#define START_POS_CUR_Y (START_POS_MAP_Y + 1) // pozycja początkowa kursora na osi Y 

#define MAX_ELEMENTS 0.2*mapSize*mapSize


