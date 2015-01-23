#include <iostream>
#include <vector>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <cstdlib>
#include <string>
#include "ConsoleGaming.h"
#include "Menu.h"
#include <iomanip>
#include<istream>
#include<fstream>
#include <algorithm>
#include <Windows.h>
#include <string>

using namespace std;

HANDLE consoleHandle;

typedef vector<GameObject>::iterator randomAccess_iterator;
typedef vector<GameObject>::const_iterator const_iterator;


// Window constants
const int WindowWidth = 15;
const int WindowHeight = 25;
// Shape
const int ShapeSpeed = 1;
const char ShapeSymbol = static_cast<char>(219);
const char FloorSymbol = '^';
const char WallSymbol = '|';
const char GhostSymbol = static_cast<char>(177);
// Game variables
unsigned long sleepDuration = 250;
// No need to reference the active shape, since it will always be the last shape in the list
vector<vector<GameObject>> shapes;
vector<vector<GameObject>> last2shapes;
bool fullrefresh = true;
bool soundplay;
int score = 0;
bool onepause = true;
bool quit = false;
string name;

void Draw();

int close(){
	exit(0);
}

void hidecursor()//kurzora e grozen
{
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void nobuttons(){

	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();//  Get a handle to the desktop window
	GetWindowRect(hDesktop, &desktop); // Get the size of screen to the variable desktop

	LONG style;
	HWND ConsoleWindow;
	ConsoleWindow = GetConsoleWindow();
	style = GetWindowLong(ConsoleWindow, GWL_STYLE);
	style &= ~(WS_MINIMIZEBOX);
	SetWindowLongPtr(ConsoleWindow, GWL_STYLE, style);

	SetWindowPos(ConsoleWindow, NULL, desktop.right / 2 - 128, desktop.bottom / 2 - 260, 0, 0, SWP_FRAMECHANGED |
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);

}

void adjust_window(){
	//hide cursor
	hidecursor();
	//no minimize and changes the window's postion
	nobuttons();
	//set up the window
	SMALL_RECT rect = { 0, 0, 30, 25 };
	SetConsoleWindowInfo(consoleHandle, TRUE, &rect);

	//remove scroll
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(consoleHandle, &info);
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(consoleHandle, new_size);
}


void core();//magiq koqto kazva na kompilotra 4e core sushtestvuva

void presspause(){//on LB Mouse up rele
	while (true){
		Sleep(1);//namalq cpu usage s 25% procenta lelz
		if (GetAsyncKeyState(1) == 0){
			break;
		}
	}
}

void ScoreBoardUpdate(string name, int score)
{
	string* lines = new string[10];
	string* newscoreboard = new string[10];
	string test;
	int line = 0;
	int newline = 0;
	bool added = false;
	ifstream read("Scoreboard.txt");
	if (read.is_open())
	{
		while (!read.eof())
		{
			getline(read, lines[line]);
			line++;
		}
		read.close();
	}
	if (line == 0)
	{
		ofstream write("Scoreboard.txt");
		write << name << ":" << score;
		write.close();
	}
	else
	{
		for (int i = 0; i < line; i++)
		{
			int pos = lines[i].find(":");
			if (pos >= 1)
			{
				if (!added)
				{
					if (score>stoi(lines[i].substr(pos + 1)))
					{
						newscoreboard[newline] = name + ":" + to_string(score);
						newline++;
						added = true;
					}
				}
				newscoreboard[newline] = lines[i];
				newline++;
			}
		}
		if (!added && line < 6)
		{
			newscoreboard[newline] = name + ":" + to_string(score);
			newline++;
			added = true;
		}
		int all = 5;
		if (line < 5)
		{
			all = line;
			if (added) all++;
		}
		ofstream write("Scoreboard.txt");
		for (int i = 0; i < all; i++)
		{
			write << newscoreboard[i] << endl;
		}
		write.close();
	}
}

void Menu(){//start menu
	quit = false;
	onepause = true;
	POINT pt;//za mouse coord
	HWND consoleWindow = GetConsoleWindow();
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);


	//4ertaq header


	//4ertaq start menu
	Rect start, instr, back, exit, hed, score, bk;
	bk.set_values(0, 0, 400, 400, L"");
	bk.drawblack();

	start.set_values(40, 70, 200, 100, L"Start");
	hed.set_values(0, 0, 250, 60, L"");//delete header
	start.draw();
	score.set_values(40, 130, 200, 160, L"High Score");
	score.draw();
	instr.set_values(40, 190, 200, 220, L"Instructions");
	instr.draw();
	exit.set_values(40, 250, 200, 280, L"Exit");
	exit.draw();
	HDC hdc = GetDC(GetConsoleWindow());//magiq koqto mi pravi teskta visible
	header2();//menu.h zalgavie

	//sledq za natiskane na buton
	while (true){
		Sleep(1);//namalq cpu usage s 25% procenta 
		GetCursorPos(&pt);//kazva na kursora da dava coordinati na mishakta 
		ScreenToClient(consoleWindow, &pt);//kazva da gi konveritra s an4alo 0,0 goren lqv ugul na kozolata

		if (GetAsyncKeyState(1) && pt.x > score.x1 && pt.x<score.x2&&pt.y<score.y2&&pt.y>score.y1){//lmb down  ...
			score.invert();
			presspause();
			instr.drawblack();
			start.drawblack();
			exit.drawblack();
			score.drawblack();
			SetConsoleCursorPosition(consoleHandle, { 9, 8 });//za pritnvane posredata
			ifstream myReadFile;
			myReadFile.open("Scoreboard.txt");
			string lines[10];
			int line = 0;
			int i = 0;
			bool haveScore = false;
			if (myReadFile.is_open()) {
				haveScore = true;
				while (!myReadFile.eof()) {
					i++;
					SetConsoleCursorPosition(consoleHandle, { 7, 6 + i });//za pritnvane posredata

					getline(myReadFile, lines[line]);
					int pos = lines[line].find(":");
					if (pos >= 1)
					{
						cout << i << ". " << lines[line].substr(0, pos) << " - " << lines[line].substr(pos + 1);
					}
					line++;
				}
			}
			myReadFile.close();
			if (!haveScore)
			{
				cout << "\n\n       No highscore yet...";
			}
			back.set_values(150, 170, 200, 190, L"Back");
			back.draw();
			HDC hdc = GetDC(GetConsoleWindow());//magiq

			while (true){
				Sleep(1);//namalq cpu usage s 25% procenta lelz
				GetCursorPos(&pt);
				ScreenToClient(consoleWindow, &pt);
				if (GetAsyncKeyState(1) && pt.x>back.x1 && pt.x < back.x2&&pt.y<back.y2&&pt.y>back.y1){//lmb down  ...
					back.invert();
					presspause();
					system("cls");
					back.drawblack();
					core();
					break;//shano magiq
				}
			}

		}

		if (GetAsyncKeyState(1) && pt.x>start.x1 && pt.x<start.x2&&pt.y<start.y2&&pt.y>start.y1){//lmb down  ...
			start.invert();
			presspause();
			start.drawblack();//triq butonite
			instr.drawblack();
			exit.drawblack();
			hed.drawblack();
			score.drawblack();
			cout << "Enter name:";
			cin >> setw(10) >> name;
			break;//sushtkod
		}

		if (GetAsyncKeyState(1) && pt.x>exit.x1 && pt.x<exit.x2&&pt.y<exit.y2&&pt.y>exit.y1){//lmb down  ...
			exit.invert();
			presspause();
			close();
		}

		if (GetAsyncKeyState(1) && pt.x>instr.x1 && pt.x<instr.x2&&pt.y<instr.y2&&pt.y>instr.y1){//lmb down  ...
			instr.invert();
			presspause();
			instr.drawblack();
			start.drawblack();
			exit.drawblack();
			score.drawblack();
			SetConsoleCursorPosition(consoleHandle, { 9, 5 });//za pritnvane posredata


			cout << "Controls:" << endl;
			SetConsoleCursorPosition(consoleHandle, { 5, 8 });
			cout << "Use A and D to move." << endl;
			SetConsoleCursorPosition(consoleHandle, { 5, 9 });
			cout << "Use S to drop." << endl;
			SetConsoleCursorPosition(consoleHandle, { 5, 10 });
			cout << "Use SPACEBAR to rotate." << endl;
			SetConsoleCursorPosition(consoleHandle, { 5, 12 });
			cout << " M - Toggle Sound" << endl;
			back.set_values(150, 170, 200, 190, L"Back");
			back.draw();
			HDC hdc = GetDC(GetConsoleWindow());//magiq

			while (true){
				Sleep(1);//namalq cpu usage s 25% procenta lelz
				GetCursorPos(&pt);
				ScreenToClient(consoleWindow, &pt);
				if (GetAsyncKeyState(1) && pt.x>back.x1 && pt.x < back.x2&&pt.y<back.y2&&pt.y>back.y1){//lmb down  ...
					back.invert();
					presspause();
					system("cls");
					back.drawblack();
					core();
					break;//shano magiq
				}
			}

		}
	}
}


void drawPause(){

	Rect  pause, line1, line2, exit;//golqmo zeleno kvadr s 2 4erni ivici
	pause.set_values(195, 10, 218, 40, L"");

	line1.set_values(200, 17, 204, 33, L"");

	line2.set_values(208, 17, 212, 33, L"");
	if (onepause){
		pause.draw();
		line1.drawblackish();
		line2.drawblackish();

	}

	POINT pt;//za mouse coord
	HWND consoleWindow = GetConsoleWindow();
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetCursorPos(&pt);
	ScreenToClient(consoleWindow, &pt);

	if (GetAsyncKeyState(1) && pt.x > pause.x1 && pt.x < pause.x2&&pt.y<pause.y2&&pt.y>pause.y1){
		pause.invert();

		presspause();
		Rect retr, menu;
		menu.set_values(60, 100, 170, 130, L"Main Menu");
		retr.set_values(60, 160, 170, 190, L"Return");
		if (quit == false){
			retr.draw();
			menu.draw();
		}
		else{
			exit.set_values(40, 250, 200, 280, L"Exit");
			exit.draw();
			menu.set_values(40, 190, 200, 220, L"Main Menu");
			menu.draw();
		}
		while (true){
			Sleep(1);//namalq cpu usage s 25% procenta lelz
			GetCursorPos(&pt);
			ScreenToClient(consoleWindow, &pt);

			if (GetAsyncKeyState(1) && pt.x > menu.x1 && pt.x < menu.x2&&pt.y<menu.y2&&pt.y>menu.y1){
				menu.invert();
				presspause();
				menu.drawblack();
				retr.drawblack();
				pause.drawblack();
				system("cls");
				shapes.erase(shapes.begin(), shapes.end());//4istim
				core();
				break;


			}
			if (GetAsyncKeyState(1) && pt.x > retr.x1 && pt.x < retr.x2&&pt.y<retr.y2&&pt.y>retr.y1&&quit == false){
				retr.invert();
				presspause();
				menu.drawblack();
				retr.drawblack();
				Draw();
				onepause = true;
				pause.draw();
				line1.drawblackish();
				line2.drawblackish();
				break;
			}
			if (quit == true){//nameri


				if (GetAsyncKeyState(1) && pt.x>exit.x1 && pt.x<exit.x2&&pt.y<exit.y2&&pt.y>exit.y1){//lmb down  ...
					exit.invert();
					presspause();
					close();
				}

			}

		}
	}




}

bool FalseCollision(COORD dir)
{
	bool falseCol = false;

	vector<vector<GameObject>>::iterator activeShape = shapes.end() - 1;

	for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
	{
		shapeNode->Coordinates.X += dir.X;

		// Loop trough all shapes, check if we are colliding. If that's the case, the shape has reached its destination, leave it there and generate a new shape
		typedef vector<vector<GameObject>>::const_iterator it;
		// Since the active shape is always the last, loop until before the active shape, otherwise we'll detect false collision
		for (it shape = shapes.begin(); shape != shapes.end() - 1; ++shape)
		{
			for (const_iterator shapePoint = shape->begin(); shapePoint != shape->end(); ++shapePoint)
			{
				if (shapePoint->Coordinates.X == shapeNode->Coordinates.X && shapePoint->Coordinates.Y == shapeNode->Coordinates.Y)
				{
					falseCol = true;
					break;
				}
			}
			// This might go in the loop condition, but I believe it's much clear here
			if (falseCol)
				break;
		}
	}

	// Return the coordinates to how they were
	for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
	{
		shapeNode->Coordinates.X -= dir.X;
	}

	return falseCol;
}

void newShape(bool &gameover)
{
	vector<GameObject> shape;
	int x = (WindowWidth - 4) / 2;
	int type = rand() % 7;

	int c = 15;
	c = rand() % 3 + 4;
	if (c == 6) c = 7;


	switch (type)
	{
	case 0:
	{
		//#
		//###
		shape.push_back(GameObject(x, 0, ShapeSymbol, c));
		shape.push_back(GameObject(x, 1, ShapeSymbol, c));
		shape.push_back(GameObject(x + 1, 1, ShapeSymbol, c));
		shape.push_back(GameObject(x + 2, 1, ShapeSymbol, c));
	}break;
	case 1:
	{
		//  #
		//###
		shape.push_back(GameObject(x, 1, ShapeSymbol, c));
		shape.push_back(GameObject(x + 1, 1, ShapeSymbol, c));
		shape.push_back(GameObject(x + 2, 0, ShapeSymbol, c));
		shape.push_back(GameObject(x + 2, 1, ShapeSymbol, c));
	}break;
	case 2:
	{
		//##
		//##
		shape.push_back(GameObject(x, 0, ShapeSymbol, c));
		shape.push_back(GameObject(x, 1, ShapeSymbol, c));
		shape.push_back(GameObject(x + 1, 0, ShapeSymbol, c));
		shape.push_back(GameObject(x + 1, 1, ShapeSymbol, c));
	}break;
	case 3:
	{
		//##
		// ##
		shape.push_back(GameObject(x, 0, ShapeSymbol, c));
		shape.push_back(GameObject(x + 1, 0, ShapeSymbol, c));
		shape.push_back(GameObject(x + 1, 1, ShapeSymbol, c));
		shape.push_back(GameObject(x + 2, 1, ShapeSymbol, c));
	}break;
	case 4:
	{
		// ##
		//##
		shape.push_back(GameObject(x, 1, ShapeSymbol, c));
		shape.push_back(GameObject(x + 1, 1, ShapeSymbol, c));
		shape.push_back(GameObject(x + 1, 0, ShapeSymbol, c));
		shape.push_back(GameObject(x + 2, 0, ShapeSymbol, c));
	}break;
	case 5:
	{
		//####
		shape.push_back(GameObject(x, 0, ShapeSymbol, c));
		shape.push_back(GameObject(x + 1, 0, ShapeSymbol, c));
		shape.push_back(GameObject(x + 2, 0, ShapeSymbol, c));
		shape.push_back(GameObject(x + 3, 0, ShapeSymbol, c));
	}break;
	case 6:
	{
		// #
		//###
		shape.push_back(GameObject(x, 1, ShapeSymbol, c));
		shape.push_back(GameObject(x + 1, 0, ShapeSymbol, c));
		shape.push_back(GameObject(x + 1, 1, ShapeSymbol, c));
		shape.push_back(GameObject(x + 2, 1, ShapeSymbol, c));
	}break;
	}
	// Add it to the list, set it as active
	shapes.push_back(shape);
	COORD direction = { 0, 0 };
	direction.X = 0;
	if (FalseCollision(direction))
	{
		gameover = true;
	}
}

void Rotate()
{
	vector<GameObject> Shape = shapes.at(shapes.size() - 1);
	vector<GameObject>::iterator it;
	int CorX[4];
	int CorY[4];
	int c = 15;
	int arr[4][4] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int arrRot[4][4] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int i = 0;
	int minY = 1000, minXonY = 1000, minX = 100, minYpos;
	int minRotX = 1000;
	for (it = Shape.begin(); it < Shape.end(); it++, i++)
	{
		c = it->Color;
		CorX[i] = it->Coordinates.X;
		CorY[i] = it->Coordinates.Y;
		if (minY > CorY[i])
		{
			minY = CorY[i];
			minYpos = i;
		}
		if (minX > CorX[i])
		{
			minX = CorX[i];
		}
	}
	for (i = 0; i < 4; i++)
	{
		if (minY == CorY[i] && minXonY > CorX[i])
		{
			minXonY = CorX[i];
		}
	}
	for (int i = 0; i < 4; i++)
	{
		arr[CorY[i] - minY][CorX[i] - minX] = 1;
	}
	for (i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			arrRot[j][3 - i] = arr[i][j];
			if (arrRot[j][3 - i] == 1)
			{
				if (minRotX > 3 - i)
				{
					minRotX = 3 - i;
				}
			}
		}
	}
	int arrNewRot[4][4] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (j >= minRotX)
			{
				arrNewRot[i][j - minRotX] = arrRot[i][j];
			}
		}
	}

	vector<GameObject> newshape;

	for (i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (arrNewRot[i][j] == 1)
			{
				newshape.push_back(GameObject(minX + j, minY + i, ShapeSymbol, c));
			}
		}
	}

	bool Collides = false;

	for (randomAccess_iterator shapeNode = newshape.begin(); shapeNode != newshape.end(); ++shapeNode)
	{
		if (shapeNode->Coordinates.X < 0 || shapeNode->Coordinates.X >= WindowWidth)
		{
			Collides = true;
		}
		// Loop trough all shapes, check if we are colliding. If that's the case, the shape has reached its destination, leave it there and generate a new shape
		typedef vector<vector<GameObject>>::const_iterator it;
		// Since the active shape is always the last, loop until before the active shape, otherwise we'll detect false collision
		for (it shape = shapes.begin(); shape != shapes.end() - 1; ++shape)
		{
			for (const_iterator shapePoint = shape->begin(); shapePoint != shape->end(); ++shapePoint)
			{
				if (shapePoint->Coordinates.X == shapeNode->Coordinates.X && shapePoint->Coordinates.Y == shapeNode->Coordinates.Y)
				{
					Collides = true;
					break;
				}
			}
			// This might go in the loop condition, but I believe it's much clear here
			if (Collides)
				break;
		}
	}
	if (!Collides)
	{
		shapes.pop_back();
		shapes.push_back(newshape);
	}
}

void DelFullRow(int row)
{
	vector<vector<GameObject>> shapescopy = shapes;

	int max = shapes.size();

	int br = 0;

	vector<GameObject> *vshapes = new vector<GameObject>[shapes.size()];

	for (int i = 0; i < max; i++)
	{
		vshapes[i] = shapes.back();
		shapes.pop_back();
	}

	for (int j = 0; j < WindowWidth; j++)
	{
		for (int k = 0; k < max; k++)
		{
			if (find(vshapes[k].begin(), vshapes[k].end(), GameObject(j, row, ShapeSymbol)) != vshapes[k].end())
			{
				vshapes[k].erase(find(vshapes[k].begin(), vshapes[k].end(), GameObject(j, row, ShapeSymbol)));
			}
		}
	}

	/*for (int i = row - 1; i > 0; i--)
	{
	for (int j = 0; j < WindowWidth; j++)
	{
	for (int k = 0; k < max; k++)
	{

	if (find(vshapes[k].begin(), vshapes[k].end(), GameObject(j, i, ShapeSymbol)) != vshapes[k].end())
	{
	replace(vshapes[k].begin(), vshapes[k].end(), GameObject(j, i, ShapeSymbol), GameObject(j, i + 1, ShapeSymbol));
	}
	}
	}
	}*/

	for (int i = max - 1; i >= 0; i--)
	{
		shapes.push_back(vshapes[i]);
	}

	typedef vector<vector<GameObject>>::const_iterator it;
	// Since the active shape is always the last, loop until before the active shape, otherwise we'll detect false collision
	for (vector<vector<GameObject>>::iterator shape = shapes.begin(); shape != shapes.end(); ++shape)
	{
		for (randomAccess_iterator shapePoint = shape->begin(); shapePoint != shape->end(); ++shapePoint)
		{
			if (shapePoint->Coordinates.Y < row && shapePoint->Symbol == ShapeSymbol)
			{
				shapePoint->Coordinates.Y += 1;
			}
		}
	}
}

void FullRows()
{
	bool NoMoreFullRows = false;
	int brscore = 0;
	while (!NoMoreFullRows)
	{
		vector<vector<GameObject>> shapescopy = shapes;

		int max = shapes.size();

		int br = 0;

		vector<GameObject> *vshapes = new vector<GameObject>[shapes.size()];

		for (int i = 0; i < max; i++)
		{
			vshapes[i] = shapescopy.back();
			shapescopy.pop_back();
		}

		int row = WindowHeight - 2;
		for (row = WindowHeight - 2; row > 0; row--)
		{
			br = 0;
			for (int j = 0; j < WindowWidth; j++)
			{
				for (int k = 0; k < max; k++)
				{
					if (find(vshapes[k].begin(), vshapes[k].end(), GameObject(j, row, ShapeSymbol)) != vshapes[k].end())
					{
						br++;
					}
				}
			}
			if (br == WindowWidth) break;
		}
		if (br == WindowWidth)
		{
			brscore++;
			DelFullRow(row);
		}
		else
		{
			NoMoreFullRows = true;
		}
	}

	switch (brscore)
	{
	case 1: score += 200; break;
	case 2: score += 500; break;
	case 3: score += 800; break;
	case 4: score += 1200; break;
	}
}

void GhostShape()
{
	bool hasReachedFloor = false;
	vector<GameObject> savevector = shapes.back();
	vector<GameObject> ghost = shapes.back();
	shapes.pop_back();
	shapes.push_back(ghost);

	while (!hasReachedFloor)
	{
		vector<vector<GameObject>>::iterator activeShape = shapes.end() - 1;

		for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
		{
			shapeNode->Coordinates.Y += ShapeSpeed;
			shapeNode->Symbol = GhostSymbol;

			// Loop trough all shapes, check if we are colliding. If that's the case, the shape has reached its destination, leave it there and generate a new shape
			typedef vector<vector<GameObject>>::const_iterator it;
			// Since the active shape is always the last, loop until before the active shape, otherwise we'll detect false collision
			for (it shape = shapes.begin(); shape != shapes.end() - 1; ++shape)
			{
				for (const_iterator shapePoint = shape->begin(); shapePoint != shape->end(); ++shapePoint)
				{
					if (shapePoint->Coordinates.X == shapeNode->Coordinates.X && shapePoint->Coordinates.Y == shapeNode->Coordinates.Y)
					{
						hasReachedFloor = true;
						//break;
					}
				}
				// This might go in the loop condition, but I believe it's much clear here
				//if (hasReachedFloor)
				//break;
			}
		}

	}

	vector<vector<GameObject>>::iterator activeShape = shapes.end() - 1;

	if (hasReachedFloor)
	{
		// Return the coordinates to how they were
		for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
		{
			shapeNode->Coordinates.Y -= ShapeSpeed;
		}
	}
	ghost = shapes.back();
	shapes.pop_back();
	shapes.push_back(ghost);
	shapes.push_back(savevector);
	if (last2shapes.empty())
	{
		last2shapes.push_back(ghost);
		last2shapes.push_back(savevector);
	}
}

void ToGround(bool &gameover)
{
	GhostShape();
	shapes.pop_back();

	vector<vector<GameObject>>::iterator activeShape = shapes.end() - 1;

	for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
	{
		shapeNode->Symbol = ShapeSymbol;
	}
	FullRows();
	newShape(gameover);
	GhostShape();
	vector<GameObject> last = shapes.back();
	shapes.pop_back();
	shapes.pop_back();
	shapes.push_back(last);
	fullrefresh = true;

}

void Update(bool &gameover)
{
	COORD direction = { 0, 0 };

	char key = '1';

	while (kbhit())
	{
		key = getch();
	}

	switch (key)
	{
	case 'a':
		direction.X = -1;
		break;
	case 'd':
		direction.X = 1;
		break;
	case ' ':
		Rotate();
		break;
	case 's':
		ToGround(gameover);
		break;
	case 'm':
		if (soundplay)
		{
			PlaySound(NULL, 0, 0);
			soundplay = false;
		}
		else
		{
			PlaySound(TEXT("TetrisTheme.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
			soundplay = true;
		}
		break;
	};

	bool IsInside = true;
	vector<vector<GameObject>>::iterator activeShape = shapes.end() - 1;

	for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
	{
		shapeNode->Coordinates.X += direction.X;
		shapeNode->Coordinates.Y += ShapeSpeed;

		if (shapeNode->Coordinates.X < 0 || shapeNode->Coordinates.X >= WindowWidth)
		{
			IsInside = false;
		}
	}

	for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
	{
		shapeNode->Coordinates.X -= direction.X;
		shapeNode->Coordinates.Y -= ShapeSpeed;
	}

	if (!IsInside)
	{
		direction = { 0, 0 };
	}

	if (!FalseCollision(direction))
	{
		activeShape = shapes.end() - 1;

		for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
		{
			shapeNode->Coordinates.X += direction.X;
		}
	}

	bool hasReachedFloor = false;
	activeShape = shapes.end() - 1;

	for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
	{
		shapeNode->Coordinates.Y += ShapeSpeed;



		// Loop trough all shapes, check if we are colliding. If that's the case, the shape has reached its destination, leave it there and generate a new shape
		typedef vector<vector<GameObject>>::const_iterator it;
		// Since the active shape is always the last, loop until before the active shape, otherwise we'll detect false collision
		for (it shape = shapes.begin(); shape != shapes.end() - 1; ++shape)
		{
			for (const_iterator shapePoint = shape->begin(); shapePoint != shape->end(); ++shapePoint)
			{
				if (shapePoint->Coordinates.X == shapeNode->Coordinates.X && shapePoint->Coordinates.Y == shapeNode->Coordinates.Y)
				{
					hasReachedFloor = true;
					break;
				}
			}
			// This might go in the loop condition, but I believe it's much clear here
			if (hasReachedFloor)
				break;
		}
	}

	// Generate a new shape
	if (hasReachedFloor)
	{
		// Return the coordinates to how they were
		for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
		{
			shapeNode->Coordinates.Y -= ShapeSpeed;
		}
		FullRows();
		newShape(gameover);
		GhostShape();
		vector<GameObject> last = shapes.back();
		shapes.pop_back();
		shapes.pop_back();
		shapes.push_back(last);
		fullrefresh = true;
	}
}

void UpdateNoDown(bool &gameover)
{
	COORD direction = { 0, 0 };

	char key = '1';

	while (kbhit())
	{
		key = getch();
	}

	switch (key)
	{
	case 'a':
		direction.X = -1;
		break;
	case 'd':
		direction.X = 1;
		break;
	case ' ':
		Rotate();
		break;
	case 's':
		ToGround(gameover);
		break;
	case 'm':
		if (soundplay)
		{
			PlaySound(NULL, 0, 0);
			soundplay = false;
		}
		else
		{
			PlaySound(TEXT("TetrisTheme.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
			soundplay = true;
		}
		break;
	};

	bool IsInside = true;
	vector<vector<GameObject>>::iterator activeShape = shapes.end() - 1;

	for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
	{
		shapeNode->Coordinates.X += direction.X;
		shapeNode->Coordinates.Y += ShapeSpeed;

		if (shapeNode->Coordinates.X < 0 || shapeNode->Coordinates.X >= WindowWidth)
		{
			IsInside = false;
		}
	}

	for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
	{
		shapeNode->Coordinates.X -= direction.X;
		shapeNode->Coordinates.Y -= ShapeSpeed;
	}

	if (!IsInside)
	{
		direction = { 0, 0 };
	}

	if (!FalseCollision(direction))
	{
		activeShape = shapes.end() - 1;

		for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
		{
			shapeNode->Coordinates.X += direction.X;
		}
	}
}

void Draw()
{
	GhostShape();
	ClearScreen(consoleHandle);
	typedef vector<vector<GameObject>>::const_iterator it;
	for (it shape = shapes.begin(); shape != shapes.end(); ++shape)
	{
		for (const_iterator shapeNode = shape->begin(); shapeNode != shape->end(); ++shapeNode)
		{
			shapeNode->Draw(consoleHandle);
		}
	}
	vector<GameObject> last = shapes.back();
	shapes.pop_back();
	last2shapes.push_back(shapes.back());
	shapes.pop_back();
	shapes.push_back(last);
	last2shapes.push_back(last);
	fullrefresh = false;
}

void DrawNoDel()
{
	GhostShape();
	typedef vector<vector<GameObject>>::iterator it;
	for (it shape = last2shapes.end() - 2; shape != last2shapes.end(); ++shape)
	{
		for (randomAccess_iterator shapeNode = shape->begin(); shapeNode != shape->end(); ++shapeNode)
		{
			shapeNode->Symbol = ' ';
			shapeNode->Draw(consoleHandle);
		}
	}
	last2shapes.pop_back();
	last2shapes.pop_back();

	typedef vector<vector<GameObject>>::const_iterator it2;
	for (it2 shape = shapes.end() - 2; shape != shapes.end(); ++shape)
	{
		for (const_iterator shapeNode = shape->begin(); shapeNode != shape->end(); ++shapeNode)
		{
			shapeNode->Draw(consoleHandle);
		}
	}
	vector<GameObject> last = shapes.back();
	shapes.pop_back();
	last2shapes.push_back(shapes.back());
	shapes.pop_back();
	shapes.push_back(last);
	last2shapes.push_back(last);
}

void core(){//core izpulqnva funkciq na main koito shte bude izvikvan ot pause menu-to

	PlaySound(TEXT("TetrisTheme.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
	soundplay = true;

	Menu();
	drawPause();
	bool gameover = false;
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	// Prepare rand
	srand(time(NULL));


	vector<GameObject> floor;
	for (int i = 0; i < WindowWidth; i++)
	{
		floor.push_back(GameObject(i, WindowHeight - 1, FloorSymbol));
	}

	shapes.push_back(floor);

	vector<GameObject> wall;
	for (int i = 0; i < WindowHeight - 1; i++)
	{
		wall.push_back(GameObject(WindowWidth, i, '|'));
	}

	shapes.push_back(wall);

	newShape(gameover);

	Draw();
	Draw();

	clock_t lastTime = clock(); //Start timer
	clock_t newTime;
	clock_t timePassed;
	double secondsPassed;
	double timeForUpdate = sleepDuration / 1000.0;

	while (!gameover)
	{
		Sleep(1);//namalq cpu usage s 25% procenta lelz
		drawPause();
		/*Update(gameover);
		Draw();
		Sleep(sleepDuration);*/

		newTime = clock();
		timePassed = newTime - lastTime;
		secondsPassed = timePassed / (double)CLOCKS_PER_SEC;
		onepause = false;
		if (secondsPassed >= timeForUpdate)
		{
			lastTime = newTime;
			Update(gameover);

			if (fullrefresh)
			{
				Draw();
				onepause = true;
			}
			else
			{
				DrawNoDel();
			}
		}

		if (kbhit())
		{
			UpdateNoDown(gameover);
			if (fullrefresh)
			{
				Draw();
				onepause = true;
			}
			else
			{
				DrawNoDel();
			}
		}
	}

	if (gameover)
	{
		ClearScreen(consoleHandle);
		Sleep(200);
		sadface();
		ScoreBoardUpdate(name, score);
		gameovertxt();
		SetConsoleCursorPosition(consoleHandle, { 10, 13 });//za pritnvane posredata
		cout << name << ",";
		SetConsoleCursorPosition(consoleHandle, { 4, 14 });
		cout << " Your score is: " << score;
		quit = true;
		while (true){
			drawPause();
		}

	}
}

int main()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN);
	adjust_window();
	Sleep(200);//malko po4ivka za da narisuva grafikata
	core();


	return 0;
}
