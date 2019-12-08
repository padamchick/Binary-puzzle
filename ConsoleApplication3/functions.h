#pragma once
void printLegend(int x, int y, char*txt);
void fillFromAFile(char**map, int mapSize, int x, int y, int bledy[][2]);
void printMap(char ** map, int mapSize);
void changeValue(int value, char **map, int x, int y, int mapSize, int bledy[][2]);
bool zeroColCounter(char**map, int mapSize, int x, int bledy[][2], int value); // liczy, czy liczba zer nie przekracza N/2
bool zeroRowCounter(char**map, int mapSize, int y, int bledy[][2], int value); // liczy, czy liczba zer nie przekracza N/2
bool oneColCounter(char**map, int mapSize, int x, int bledy[][2], int value); // liczy, czy liczba jedynek nie przekracza N/2
bool oneRowCounter(char**map, int mapSize, int y, int bledy[][2], int value); // liczy, czy liczba jedynek nie przekracza N/2
bool isRowFull(char**map, int mapSize, int y); // sprawdza, czy chcemy wpisaæ do wiersza ostatni¹ pozycjê
bool isTheSameRow(char**map, int mapSize, int x, int y, int value, int bledy[][2]); // sprawdza, czy istnieje juz taki wiersz
bool isColFull(char**map, int mapSize, int x); // sprawdza, czy chcemy wpisaæ do kolumny ostatni¹ pozycjê
bool isTheSameCol(char**map, int mapSize, int x, int y, int value, int bledy[][2]); // sprawdza czy istnieje juz taka kolumna
bool isNotThree(char**map, int x, int y, int value, int mapSize, int bledy[][2]); // sprawdzenie regu³y 1
bool isPossibleToFill(char **map, int x, int y, int value, int mapSize, int bledy[][2]); // sprawdzenie wszystkich regu³
void makeRandomMap(char**map, int mapSize, int x, int y, int bledy[][2]); // tworzenie losowej mapy
void printHint(char **map, int x, int y, int bledy[][2]); // wyswietlanie podpowiedzi
void checkFails(char**map, int x, int y, int value, int mapSize, int bledy[][2]); // wywolywanie wszystkich regul, w celu wypisania bledow w tablicy bledow
void checkPossibleEnding(char**map, int mapSize, int x, int y, int bledy[][2]); // podswietlanie pol niemozliwych do wypelnienia
void printNumberOfDigitsInRow(char**map, int mapSize); // wyswietlanie liczby 0 i 1 w wierszu
void printNumberOfDigitsInCol(char**map, int mapSize); // wyswietlanie liczby 0 i 1 w kolumnie
void checkUnambiguousFields(char**map, int mapSize, int x, int y, int bledy[][2]); // wyswietlanie pol jednoznacznych i ich ew. wypelnianie
void startAutoDetection(char**map, int mapSize, int bledy[][2]); // wlacz tryb autodetekcji konca gry
void stopAutoDetection(char**map, int mapSize, int bledy[][2]); // wylacz w.w. tryb