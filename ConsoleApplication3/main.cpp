#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include "conio2.h"
#include "define.h"
#include "functions.h"

int main() {
	srand(time(NULL));
	int zn = 0, x = START_POS_CUR_X, y = START_POS_CUR_Y, attr = 7, back = 0, zero = 0;
	char txt[50];
	int mapSize = 12;
	int automaticDetection = 0;
	
	char**map = new char*[mapSize]; //alokacja pamiêci na planszê
	for (int i = 0; i < mapSize; i++)
	{
		map[i] = new char[mapSize + 1];
		for (int j = 0; j < mapSize; j++)
			map[i][j] = '.';
		map[i][mapSize] = '\0';
	}

	int bledy[10][2];
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 2; j++)
			bledy[i][j] = 0;


	fillFromAFile(map, mapSize, x, y, bledy);

	settitle("Piotr Adamczyk 151466"); // settitle ustawia tytu³ okienka

	do {
		textbackground(BLACK);
		clrscr(); // czyœcimy ekran: wype³niamy spacjami z ustawionym kolrem t³a
		textcolor(7); // ustawiamy kolor tekstu na jasnoszary (7 == LIGHTGRAY)

		printLegend(x, y, txt);

		if (automaticDetection == 1)
			startAutoDetection(map, mapSize, bledy);
		else stopAutoDetection(map, mapSize, bledy);

		printMap(map, mapSize);
		
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 2; j++)
				bledy[i][j] = 0;

		gotoxy(x, y);
		textcolor(attr);
		textbackground(back);
		putch('*');

		zero = 0;
		zn = getch();

		if (zn == 0) 
		{
			zero = 1;		// je¿eli tak, to czytamy
			zn = getch();		// kolejny znak wiedz¹c ju¿,
			if (zn == GORA && y > START_POS_MAP_Y + 1) y--;	// ¿e bêdzie to znak specjalny
			else if (zn == DOL && y < START_POS_MAP_Y + mapSize) y++;
			else if (zn == LEWO && x > START_POS_MAP_X + 1) x--;
			else if (zn == PRAWO && x < START_POS_MAP_X + mapSize) x++;
		}
		else if (zn == SPACJA) attr = (attr + 1) % 16;
		else if (zn == ENTER) back = (back + 1) % 16;
		else if (zn == ZERO) changeValue(0, map, x, y, mapSize, bledy); //wypisywanie zero
		else if (zn == JEDEN) changeValue(1, map, x, y, mapSize, bledy); //wypisywanie jedynki
		else if (zn == CLEAN) changeValue(5, map, x, y, mapSize, bledy); //kasowanie znaku za pomoc¹ klawisza 5
		else if (zn == 'n')
		{
			clrscr();
			fillFromAFile(map, mapSize, x, y, bledy);
			printMap(map, mapSize);
			x = START_POS_CUR_X, y = START_POS_CUR_Y;
		}
		else if (zn == 'o') // losowa mapa
		{
			x = START_POS_CUR_X, y = START_POS_CUR_Y;
			gotoxy(x, y);
			makeRandomMap(map, mapSize, x, y, bledy);
		}
		else if (zn == 'p') // prosta podpowiedz
		{
			checkFails(map, x - START_POS_MAP_X - 1, y - START_POS_MAP_Y - 1, '0', mapSize, bledy); //sprawdzamy kody b³êdów dla zera
			checkFails(map, x - START_POS_MAP_X - 1, y - START_POS_MAP_Y - 1, '1', mapSize, bledy); //sprawdzamy kody b³êdów dla jedynki
			printHint(map, x, y, bledy);
		}

		else if (zn == 'r') // zmiana rozmiaru planszy
		{
			gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 19);
			cputs("Podaj rozmiar planszy i kliknij ENTER:");
			int liczba = 0, i;
			//pobiera w pêtli dwa znaki, które okreœl¹ nam rozmiar nowo tworzonej tablicy
			for (i = 0; i < 2; i++) 
			{
				char znak = getch();

				if (znak < '0' || znak > '9') //gdy wprowadzony znak nie jest cyfr¹, przerwij pêtlê
					break;

				liczba *= 10;
				liczba += znak - '0';
				gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 20);
				sprintf(txt, "%d", liczba);
				cputs(txt);
			}
			if (i == 2) getch();

			if (liczba < 2)
				liczba = 2;
			if (liczba % 2 == 1)
				liczba -= 1;
			if (liczba > 16)
				liczba = 16;

			mapSize = liczba;
			clrscr();
			map = new char*[mapSize]; //alokacja pamiêci na planszê
			for (int i = 0; i < mapSize; i++)
			{
				map[i] = new char[mapSize + 1];
				for (int j = 0; j < mapSize; j++)
					map[i][j] = '.';
				map[i][mapSize] = '\0';
			}

			fillFromAFile(map, mapSize, x, y, bledy);
			printMap(map, mapSize);
			x = START_POS_CUR_X, y = START_POS_CUR_Y;
		}

		else if (zn == 'k') // detekcja pól niemozliwych do wypelnienia
		{
			checkPossibleEnding(map, mapSize, x, y, bledy);
		}

		else if (zn == 'd') // sprawdzenie reguly 2, wypisanie ilosc 0 & 1 w kolumnach i wierszach
		{
			printNumberOfDigitsInRow(map, mapSize); // wyœwietl ilosc zer i jedynek w wierszu
			printNumberOfDigitsInCol(map, mapSize); // wyœwietl ilosc zer i jedynek w kolumnie
			getch();
		}

		else if (zn == 'a') //automatyczna detekcja koñca gry
			automaticDetection = (automaticDetection + 1) % 2; //dla wartoœci 1 w³¹czenie autodetekcji, dla 0 wy³¹czenie

		else if (zn == 'j') //podœwietlenie pól jednoznacznych
		{
			checkUnambiguousFields(map, mapSize, x, y, bledy);
		}

	} while (zn != ESC);

	for (int i = 0; i < mapSize; i++) //zwalnianie pamiêci
		delete[] map[i];
	delete[] map;

	return 0;
}

void printLegend(int x, int y, char*txt)
{
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y);
	cputs("Piotr Adamczyk 151466");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 1);
	cputs("Zaimplementowane funkcjonalnosci:");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 2);
	cputs("a - m");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 3);
	cputs("LEGENDA:");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 4);
	cputs("strzalki = poruszanie sie po planszy");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 5);
	cputs("ESC = wyjscie");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 6);
	cputs("n   = nowa gra");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 7);
	cputs("01  = wpisanie cyfry na plansze");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 8);
	cputs("5   = kasowanie znaku");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 9);
	cputs("o   = losowe wypelnienie planszy");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 10);
	cputs("p   = prosta podpowiedz");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 11);
	cputs("r   = zmiana rozmiaru planszy");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 12);
	cputs("k   = pokaz pola niemozliwe do wypelnienia");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 13);
	cputs("d   = sprawdzenie reguly (2)");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 14);
	cputs("a   = automatyczna detekcja konca gry");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 15);
	cputs("j   = podswietlenie jednoznacznych pol");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 16);
	cputs("w   = wypelnienie podswietlonych");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 17);
	cputs("      jednoznacznych pol");
	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 18);
	sprintf(txt, "obecna pozycja kursora: x=%d y=%d", x - START_POS_CUR_X + 1, y - START_POS_CUR_Y + 1);
	cputs(txt);
}

void fillFromAFile(char **map, int mapSize, int x, int y, int bledy[][2])
{
	if (mapSize == 12)
	{
		FILE*board = fopen("plansza12x12.txt", "r");

		for (int i = 0; i < mapSize; i++)
		{
			for (int j = 0; j < mapSize; j++)
			{
				map[i][j] = fgetc(board);
			}
			fgetc(board);
		}
	}
	else if (mapSize == 8)
	{
		FILE*board = fopen("plansza8x8.txt", "r");

		for (int i = 0; i < mapSize; i++)
		{
			for (int j = 0; j < mapSize; j++)
			{
				map[i][j] = fgetc(board);
			}
			fgetc(board);
		}
	}

	else if (mapSize == 4)
	{
		FILE*board = fopen("plansza4x4.txt", "r");

		for (int i = 0; i < mapSize; i++)
		{
			for (int j = 0; j < mapSize; j++)
			{
				map[i][j] = fgetc(board);
			}
			fgetc(board);
		}
	}
	else makeRandomMap(map, mapSize, x, y, bledy);
}

void printMap(char** map, int mapSize)
{
	gotoxy(START_POS_MAP_X, START_POS_MAP_Y);
	for (int i = 0; i < mapSize + 2; i++)
		putch('*');

	for (int i = 0; i < mapSize; i++)
	{
		gotoxy(START_POS_MAP_X, START_POS_MAP_Y + i + 1);
		putch('*');

		for (int j = 0; j < mapSize; j++)
		{
			textcolor(7);

			if (map[i][j] == '2' || map[i][j] == '3') // niemodyfikowalne pola '0' i '1'
			{
				textcolor(10); //zmien kolor 

				putch(map[i][j] - 2);
				textcolor(7); //powrot do bazowego koloru
			}

			else if (map[i][j] == '7' || map[i][j] == '8') //podswietlanie pol niemozliwych do wypelnienia
			{
				textbackground(DARKGRAY);
				putch('.');
				textbackground(BLACK);
			}

			else if (map[i][j] == '9') //podswietlanie pol niemozliwych do wypelnienia
			{
				textbackground(RED);
				putch('.');
				textbackground(BLACK);
			}
			else putch(map[i][j]);
		}

		gotoxy(START_POS_MAP_X + mapSize + 1, START_POS_MAP_Y + i + 1);
		putch('*');
	}

	gotoxy(START_POS_MAP_X, START_POS_MAP_Y + mapSize + 1);
	for (int i = 0; i < mapSize + 2; i++)
		putch('*');
}

/*

0 - pole 0
1 - pole 1
2 - pole 0, ktorego nie mozna zmieniac
3 - pole 1,	ktorego nie mozna zmieniac
5 - puste pole

*/

void changeValue(int value, char **map, int x, int y, int mapSize, int bledy[][2])
{
	if (map[y - START_POS_MAP_Y - 1][x - START_POS_MAP_X - 1] == '2' || map[y - START_POS_MAP_Y - 1][x - START_POS_MAP_X - 1] == '3')
		return;

	if (value == 5)
		map[y - START_POS_MAP_Y - 1][x - START_POS_MAP_X - 1] = '.';

	if (!isPossibleToFill(map, x - START_POS_MAP_X - 1, y - START_POS_MAP_Y - 1, '0' + value, mapSize, bledy))
		return;

	if (value == 0)
		map[y - START_POS_MAP_Y - 1][x - START_POS_MAP_X - 1] = '0';

	if (value == 1)
		map[y - START_POS_MAP_Y - 1][x - START_POS_MAP_X - 1] = '1';
}

bool zeroColCounter(char**map, int mapSize, int x, int bledy[][2], int value)
{
	int colCounter = 0;
	for (int j = 0; j < mapSize; j++)
	{
		if (map[j][x] == value || map[j][x] == value + 2) colCounter++;
	}
	if (colCounter >(mapSize / 2) - 1)
	{
		bledy[1][0] = 1;
		return false;
	}
	return true;
}

bool zeroRowCounter(char**map, int mapSize, int y, int bledy[][2], int value)
{
	int rowCounter = 0;
	for (int j = 0; j < mapSize; j++)
	{
		if (map[y][j] == value || map[y][j] == value + 2) rowCounter++;
	}
	if (rowCounter >(mapSize / 2 - 1))
	{
		bledy[2][0] = 1;
		return false;
	}
	return true;
}

bool oneColCounter(char**map, int mapSize, int x, int bledy[][2], int value)
{
	int colCounter = 0;
	for (int j = 0; j < mapSize; j++)
	{
		if (map[j][x] == value || map[j][x] == value + 2) colCounter++;
	}
	if (colCounter >(mapSize / 2 - 1))
	{
		bledy[6][0] = 1;
		return false;
	}
	return true;
}

bool oneRowCounter(char**map, int mapSize, int y, int bledy[][2], int value)
{
	int rowCounter = 0;
	for (int j = 0; j < mapSize; j++)
	{
		if (map[y][j] == value || map[y][j] == value + 2) rowCounter++;
	}
	if (rowCounter >(mapSize / 2 - 1))
	{
		bledy[7][0] = 1;
		return false;
	}
	return true;
}

bool isRowFull(char**map, int mapSize, int y)
{
	int counter = 0;
	for (int j = 0; j < mapSize; j++)
	{
		if (map[y][j] != '.') counter++;
	}
	if (counter == mapSize - 1) return true;

	return false;
}

bool isTheSameRow(char**map, int mapSize, int x, int y, int value, int bledy[][2])
{
	int i, j;
	if (isRowFull(map, mapSize, y))
	{
		int counter = 0;
		for (i = 0; i < mapSize; i++)
		{
			counter = 0;
			if (y == i) continue;
			for (j = 0; j < mapSize; j++)
			{
				if (x == j)
				{
					if (value != map[i][j] && value + 2 != map[i][j])
						break;
				}

				else if (map[y][j] != map[i][j] && map[y][j] != map[i][j] + 2 && map[y][j] + 2 != map[i][j] && map[y][j] + 2 != map[i][j] + 2)
					break;

				counter++;
			}
			if (counter == mapSize)
			{
				if (value == '0')
				{
					bledy[4][0] = 1;
					bledy[4][1] = i + 1;
					return true;
				}
				else
				{
					bledy[9][0] = 1;
					bledy[9][1] = i + 1;
					return true;
				}
			}
		}
	}
	return false;
}

bool isColFull(char**map, int mapSize, int x)
{
	int counter = 0;
	for (int j = 0; j < mapSize; j++)
	{
		if (map[j][x] != '.') counter++;
	}
	if (counter == mapSize - 1) return true;

	return false;
}

bool isTheSameCol(char**map, int mapSize, int x, int y, int value, int bledy[][2])
{
	int i, j;
	if (isColFull(map, mapSize, x))
	{
		int counter = 0;
		for (j = 0; j < mapSize; j++)
		{
			counter = 0;
			if (x == j) continue;
			for (i = 0; i < mapSize; i++)
			{
				if (y == i)
				{
					if (value != map[i][j] && value + 2 != map[i][j])
						break;
				}

				else if (map[i][x] != map[i][j] && map[i][x] != map[i][j] + 2 && map[i][x] + 2 != map[i][j] && map[i][x] + 2 != map[i][j] + 2)
					break;

				counter++;
			}
			if (counter == mapSize)
			{
				if (value == '0')
				{
					bledy[3][0] = 1;
					bledy[3][1] = j + 1;
					return true;
				}
				else
				{
					bledy[8][0] = 1;
					bledy[8][1] = j + 1;
					return true;
				}
			}
		}
	}
	return false;
}

bool isNotThree(char**map, int x, int y, int value, int mapSize, int bledy[][2])
{
	if (value == '0')
		bledy[0][0] = 1;
	else if (value == '1')
		bledy[5][0] = 1;

	if (x > 0 && x < mapSize - 1 && (map[y][x - 1] == value || map[y][x - 1] - 2 == value) && (map[y][x + 1] == value || map[y][x + 1] - 2 == value))
		return false;

	if (y > 0 && y < mapSize - 1 && (map[y - 1][x] == value || map[y - 1][x] - 2 == value) && (map[y + 1][x] == value || map[y + 1][x] - 2 == value))
		return false;
	
	if (x > 1 && x < mapSize && (map[y][x - 1] == value || map[y][x - 1] - 2 == value) && (map[y][x - 2] == value || map[y][x - 2] - 2 == value))
		return false;

	if (x >= 0 && x < mapSize - 2 && (map[y][x + 1] == value || map[y][x + 1] - 2 == value) && (map[y][x + 2] == value || map[y][x + 2] - 2 == value))
		return false;
	
	if (y > 1 && y < mapSize && (map[y - 1][x] == value || map[y - 1][x] - 2 == value) && (map[y - 2][x] == value || map[y - 2][x] - 2 == value))
		return false;
	
	if (y >= 0 && y < mapSize - 2 && (map[y + 1][x] == value || map[y + 1][x] - 2 == value) && (map[y + 2][x] == value || map[y + 2][x] - 2 == value))
		return false;

	if (value == '0')
		bledy[0][0] = 0;
	else if (value == '1')
		bledy[5][0] = 0;
	
	return true;
}

bool isPossibleToFill(char **map, int x, int y, int value, int mapSize, int bledy[][2]) //zamienione kolejnoœci¹ x i y!!
{

	if (!isNotThree(map, x, y, value, mapSize, bledy)) //sprawdza warunki s¹siedztwa
		return false;

	if (value == '1')
	{
		if (!oneColCounter(map, mapSize, x, bledy, value))
			return false;

		if (!oneRowCounter(map, mapSize, y, bledy, value))
			return false;
	}

	else if (value == '0')
	{
		if (!zeroColCounter(map, mapSize, x, bledy, value))
			return false;

		if (!zeroRowCounter(map, mapSize, y, bledy, value))
			return false;
	}

	if (isTheSameRow(map, mapSize, x, y, value, bledy))
		return false;

	if (isTheSameCol(map, mapSize, x, y, value, bledy))
		return false;

	return true;
}

void makeRandomMap(char**map, int mapSize, int x, int y, int bledy[][2])
{
	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
			map[i][j] = '.';
		map[i][mapSize] = '\0';
	}

	int counter = MAX_ELEMENTS;
	int errorr = 0;
	gotoxy(x, y);

	int randX, randY, randNum;
	while (counter > 0 && errorr < 20) //dopóki liczba pozostalych do wyswietlenia elementów nie spadnie do zera lub liczba pu³apek nie przekroczy 20
	{
		randX = rand() % mapSize; //losowanie pola na planszy o wsp. x
		randY = rand() % mapSize; //losowanie pola na planszy o wsp. y
		randNum = rand() % 2 + 2; //losujemy liczby 2 i 3
		if (isPossibleToFill(map, randX, randY, '0' + randNum, mapSize, bledy)) //jesli spelnia wszystkie regu³y
		{
			map[randY][randX] = '0' + randNum;
			counter--;
		}
		else errorr++;
	}
}

void printHint(char**map, int x, int y, int bledy[][2])
{
	char buffor[100];
	int i = 0, j = 0;

	gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 19);
	cputs("Podpowiedz:");

	if (map[y - START_POS_MAP_Y - 1][x - START_POS_MAP_X - 1] != '2' && map[y - START_POS_MAP_Y - 1][x - START_POS_MAP_X - 1] != '3')
	{
		gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 20);
		if (bledy[0][0] == 1) {
			cputs("Wpisanie 0 spowoduje wystapienie trzech \"0\" obok siebie");
			i++;
		}
		gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 20 + i);
		if (bledy[1][0] == 1) {
			cputs("Wpisanie 0 jest niemozliwe, gdyz limit \"0\" w tej kolumnie jest osiagniety");
			i++;
		}
		gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 20 + i);
		if (bledy[2][0] == 1) {
			cputs("Wpisanie 0 jest niemozliwe, gdyz limit \"0\" w tym wierszu zostal osiagniety");
			i++;
		}
		gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 20 + i);
		if (bledy[3][0] == 1) {
			sprintf(buffor, "Wpisanie 0 spowoduje, ze kolumna ta bedzie identyczna, jak kolumna %d", bledy[3][1]);
			cputs(buffor);
			i++;
		}

		gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 20 + i);
		if (bledy[4][0] == 1) {
			sprintf(buffor, "Wpisanie 0 spowoduje, ze wiersz ten bedzie identyczny, jak wiersz %d", bledy[4][1]);
			cputs(buffor);
			i++;
		}
		if (i == 0) {
			cputs("0 - OK!");
			i++;
		}

		j = 0;
		gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 20 + i + j);
		if (bledy[5][0] == 1) {
			cputs("Wpisanie 1 spowoduje wystapienie trzech \"1\" obok siebie");
			j++;
		}
		gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 20 + i + j);
		if (bledy[6][0] == 1) {
			cputs("Wpisanie 1 jest niemozliwe, gdyz limit \"1\" w tej kolumnie zostal osiagniety");
			j++;
		}
		gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 20 + i + j);
		if (bledy[7][0] == 1) {
			cputs("Wpisanie 1 jest niemozliwe, gdyz limit \"1\" w tym wierszu zostal osiagniety");
			j++;
		}
		gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 20 + i + j);
		if (bledy[8][0] == 1) {
			sprintf(buffor, "Wpisanie 1 spowoduje, ze kolumna ta bedzie identyczna, jak kolumna %d", bledy[8][1]);
			cputs(buffor);
			j++;
		}
		gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 20 + i + j);
		if (bledy[9][0] == 1) {
			sprintf(buffor, "Wpisanie 1 spowoduje, ze wiersz ten bedzie identyczny, jak wiersz %d", bledy[9][1]);
			cputs(buffor);
			j++;
		}
		if (j == 0) {
			cputs("1 - OK!");
			j++;
		}
	}
	else {
		gotoxy(START_POS_LEG_X, START_POS_LEG_Y + 20);
		cputs("Pole jest niemodyfikowalne");
		i++;
	}
	getch();
}

void checkFails(char**map, int x, int y, int value, int mapSize, int bledy[][2])
{
	isNotThree(map, x, y, value, mapSize, bledy);

	if (value == '0')
	{
		zeroColCounter(map, mapSize, x, bledy, value);
		zeroRowCounter(map, mapSize, y, bledy, value);
	}
	else if (value == '1')
	{
		oneColCounter(map, mapSize, x, bledy, value);
		oneRowCounter(map, mapSize, y, bledy, value);
	}

	isTheSameCol(map, mapSize, x, y, value, bledy);
	isTheSameRow(map, mapSize, x, y, value, bledy);
}

/*
TABELA BLEDOW:
bledy[0][0] = 1 -> trzy zera obok siebie
bledy[1][0] = 1 -> za du¿o 0 w kolumnie
bledy[2][0] = 1 -> za du¿o 0 w wierszu
bledy[3][0] = 1 -> wpisanie 0 spowoduje istnienie drugiej takiej samej kolumny
bledy[3][1] = ? -> nr w.w. kolumny
bledy[4][0] = 1 -> wpisanie 0 spowoduje istnienie drugiego takiego samego wiersza
bledy[4][1] = 1 -> nr w.w. wiersza
bledy[5][0] = 1 -> trzy jedynki obok siebie
bledy[6][0] = 1 -> za du¿o 1 w kolumnie
bledy[7][0] = 1 -> za du¿o 1 w wierszu
bledy[8][0] = 1 -> wpisanie 1 spowoduje istnienie drugiej takiej samej kolumny
bledy[8][1] = 1 -> nr w.w. kolumny
bledy[9][0] = 1 -> wpisanie 1 spowoduje istnienie drugiego takiego samego wiersza
bledy[9][1] = 1 -> nr w.w. wiersza
*/

void checkPossibleEnding(char**map, int mapSize, int x, int y, int bledy[][2])
{
	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
		{
			if (!isPossibleToFill(map, j, i, '0', mapSize, bledy) && !isPossibleToFill(map, j, i, '1', mapSize, bledy) && map[i][j] != '0' && map[i][j] != '1' && map[i][j] != '2' && map[i][j] != '3')
			{
				map[i][j] = '9';
			}
		}
	}
	printMap(map, mapSize);
	getch();
	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
		{
			if (map[i][j] == '9')
			{
				map[i][j] = '.';
			}
		}
	}
}

void printNumberOfDigitsInRow(char**map, int mapSize)
{
	int i, j, zeroCounter, oneCounter;
	int x;
	char buffor[10];

	if (START_POS_MAP_X < 6)
		x = START_POS_MAP_X + mapSize + 3;
	else x = START_POS_MAP_X - 4;

	for (i = 0; i < mapSize; i++)
	{
		zeroCounter = 0, oneCounter = 0;
		for (j = 0; j < mapSize; j++)
		{
			if (map[i][j] == '0' || map[i][j] == '2')
				zeroCounter++;
			if (map[i][j] == '1' || map[i][j] == '3')
				oneCounter++;
		}

		if (zeroCounter > mapSize / 2)
		{
			gotoxy(x, START_POS_MAP_Y + 1 + i);
			textbackground(RED);
			sprintf(buffor, "%d", zeroCounter);
			cputs(buffor);
			textbackground(BLACK);
			sprintf(buffor, " %d", oneCounter);
			cputs(buffor);
		}

		if (oneCounter > mapSize / 2)
		{
			gotoxy(x, START_POS_MAP_Y + 1 + i);
			sprintf(buffor, "%d ", zeroCounter);
			cputs(buffor);
			textbackground(RED);
			sprintf(buffor, "%d", oneCounter);
			cputs(buffor);
			textbackground(BLACK);
		}

		if (oneCounter <= mapSize / 2 && zeroCounter <= mapSize / 2)
		{
			gotoxy(x, START_POS_MAP_Y + 1 + i);
			sprintf(buffor, "%d %d", zeroCounter, oneCounter);
			cputs(buffor);
		}
	}
	gotoxy(x, START_POS_MAP_Y);
	textcolor(2);
	cputs("0 1");
	textcolor(7);
}

void printNumberOfDigitsInCol(char**map, int mapSize)
{
	int i, j, zeroCounter, oneCounter;
	char buffor[10];
	for (i = 0; i < mapSize; i++)
	{
		zeroCounter = 0, oneCounter = 0;
		for (j = 0; j < mapSize; j++)
		{
			if (map[j][i] == '0' || map[j][i] == '2')
				zeroCounter++;
			if (map[j][i] == '1' || map[j][i] == '3')
				oneCounter++;
		}


		if (zeroCounter > mapSize / 2)
		{
			gotoxy(START_POS_MAP_X + 1 + i, START_POS_MAP_Y + mapSize + 2);
			if (zeroCounter>9)
				gotoxy(START_POS_MAP_X + 1 + i, START_POS_MAP_Y + mapSize + 3);
			textbackground(RED);
			sprintf(buffor, "%d", zeroCounter);
			cputs(buffor);
			textbackground(BLACK);
			gotoxy(START_POS_MAP_X + 1 + i, START_POS_MAP_Y + mapSize + 4);
			sprintf(buffor, "%d", oneCounter);
			cputs(buffor);
		}

		if (oneCounter > mapSize / 2)
		{
			gotoxy(START_POS_MAP_X + 1 + i, START_POS_MAP_Y + mapSize + 2);
			sprintf(buffor, "%d", zeroCounter);
			cputs(buffor);
			gotoxy(START_POS_MAP_X + 1 + i, START_POS_MAP_Y + mapSize + 4);
			if (oneCounter>9)
				gotoxy(START_POS_MAP_X + 1 + i, START_POS_MAP_Y + mapSize + 5);
			textbackground(RED);
			sprintf(buffor, "%d", oneCounter);
			cputs(buffor);
			textbackground(BLACK);
		}

		if (oneCounter <= mapSize / 2 && zeroCounter <= mapSize / 2)
		{
			gotoxy(START_POS_MAP_X + 1 + i, START_POS_MAP_Y + mapSize + 2);
			if (zeroCounter>9)
				gotoxy(START_POS_MAP_X + 1 + i, START_POS_MAP_Y + mapSize + 3);
			sprintf(buffor, "%d", zeroCounter);
			cputs(buffor);
			gotoxy(START_POS_MAP_X + 1 + i, START_POS_MAP_Y + mapSize + 4);
			if (oneCounter>9)
				gotoxy(START_POS_MAP_X + 1 + i, START_POS_MAP_Y + mapSize + 5);
			sprintf(buffor, "%d", oneCounter);
			cputs(buffor);
		}
	}
	gotoxy(START_POS_MAP_X, START_POS_MAP_Y + mapSize + 2);
	textcolor(2);
	cputs("0");
	textcolor(7);
	gotoxy(START_POS_MAP_X, START_POS_MAP_Y + mapSize + 4);
	textcolor(2);
	cputs("1");
	textcolor(7);
}

void startAutoDetection(char**map, int mapSize, int bledy[][2])
{
	printNumberOfDigitsInRow(map, mapSize);
	printNumberOfDigitsInCol(map, mapSize);

	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
		{
			if (!isPossibleToFill(map, j, i, '0', mapSize, bledy) && !isPossibleToFill(map, j, i, '1', mapSize, bledy) && map[i][j] != '0' && map[i][j] != '1' && map[i][j] != '2' && map[i][j] != '3')
			{
				map[i][j] = '9';
			}
		}
	}

	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
		{
			if ((isPossibleToFill(map, j, i, '0', mapSize, bledy) || isPossibleToFill(map, j, i, '1', mapSize, bledy)) && map[i][j] == '9')
			{
				map[i][j] = '.';
			}
		}
	}

	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
		{
			if (!isPossibleToFill(map, j, i, '0', mapSize, bledy) && !isPossibleToFill(map, j, i, '1', mapSize, bledy) && map[i][j] != '0' && map[i][j] != '1' && map[i][j] != '2' && map[i][j] != '3')
			{
				gotoxy(START_POS_MAP_X, START_POS_MAP_Y + mapSize + 5);
				cputs("Gra w obecnej konfiguracji ");
				gotoxy(START_POS_MAP_X, START_POS_MAP_Y + mapSize + 6);
				cputs("nie moze zostac ukonczona");
			}
		}
	}
	gotoxy(START_POS_MAP_X, START_POS_MAP_Y + mapSize + 7);
	cputs("Tryb autodetekcji: wlaczony ");
}

void stopAutoDetection(char**map, int mapSize, int bledy[][2])
{
	for (int i = 0; i < mapSize; i++)
		for (int j = 0; j < mapSize; j++)
			if (map[i][j] == '9')
				map[i][j] = '.';

	gotoxy(START_POS_MAP_X, START_POS_MAP_Y + mapSize + 7);
	cputs("Tryb autodetekcji: wylaczony");
}

void checkUnambiguousFields(char**map, int mapSize, int x, int y, int bledy[][2])
{
	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
		{
			if (isPossibleToFill(map, j, i, '0', mapSize, bledy) && !isPossibleToFill(map, j, i, '1', mapSize, bledy) && map[i][j] != '0' && map[i][j] != '1' && map[i][j] != '2' && map[i][j] != '3')
				map[i][j] = '8';

			if (!isPossibleToFill(map, j, i, '0', mapSize, bledy) && isPossibleToFill(map, j, i, '1', mapSize, bledy) && map[i][j] != '0' && map[i][j] != '1' && map[i][j] != '2' && map[i][j] != '3')
				map[i][j] = '7';
		}
	}
	printMap(map, mapSize);

	if (getch() == 'w')
	{
		for (int i = 0; i < mapSize; i++)
		{
			for (int j = 0; j < mapSize; j++)
			{
				if (map[i][j] == '8' && isPossibleToFill(map, j, i, '0', mapSize, bledy))
					map[i][j] = '0';

				else if (map[i][j] == '8' && !isPossibleToFill(map, j, i, '0', mapSize, bledy)) map[i][j] = '.';

				if (map[i][j] == '7' && isPossibleToFill(map, j, i, '1', mapSize, bledy))
					map[i][j] = '1';

				else if (map[i][j] == '7' && !isPossibleToFill(map, j, i, '1', mapSize, bledy)) map[i][j] = '.';
			}
		}
	}
	else
	{
		for (int i = 0; i < mapSize; i++)
			for (int j = 0; j < mapSize; j++)
				if (map[i][j] == '7' || map[i][j] == '8')
					map[i][j] = '.';
	}
}
