#include <iostream>
#include <vector>
#include <iostream>
#include <conio.h>
#include <time.h>

#include "ConsoleGaming.h"

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
 
void Menu(){//start menu
 
 
        POINT pt;//za mouse coord
        HWND consoleWindow = GetConsoleWindow();
        consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
 
 
        //4ertaq header
 
 
        //4ertaq start menu
        Rect start, instr, back, exit, hed;
        start.set_values(30, 70, 200, 100, L"Start");
        hed.set_values(0, 0, 250, 60, L"");//delete header
        start.draw();
        instr.set_values(30, 130, 200, 160, L"Instructions");
        instr.draw();
        exit.set_values(30, 190, 200, 220, L"Exit");
        exit.draw();
        HDC hdc = GetDC(GetConsoleWindow());//magiq koqto mi pravi teskta visible
        header2();//menu.h zalgavie
 
        //sledq za natiskane na buton
        while (true){
                Sleep(1);//namalq cpu usage s 25% procenta
                GetCursorPos(&pt);//kazva na kursora da dava coordinati na mishakta
                ScreenToClient(consoleWindow, &pt);//kazva da gi konveritra s an4alo 0,0 goren lqv ugul na kozolata
 
                if (GetAsyncKeyState(1) && pt.x>start.x1 && pt.x<start.x2&&pt.y<start.y2&&pt.y>start.y1){//lmb down  ...
                        start.invert();
                        presspause();
                        start.drawblack();//triq butonite
                        instr.drawblack();
                        exit.drawblack();
                        hed.drawblack();
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
                        SetConsoleCursorPosition(consoleHandle, { 9, 8 });//za pritnvane posredata
 
 
                        cout << "To win:" << endl;
                        SetConsoleCursorPosition(consoleHandle, { 9, 9 });
                        cout << "1.JAJAJAJAJAJJAAJ" << endl;
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
        Rect pause, line1, line2;//golqmo zeleno kvadr s 2 4erni ivici
        pause.set_values(202, 10, 218, 30, L"");
        pause.draw();
        line1.set_values(206, 14, 208, 26, L"");
        line1.drawblack();
        line2.set_values(212, 14, 214, 26, L"");
        line2.drawblack();
 
 
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
                menu.draw();
                retr.set_values(60, 160, 170, 190, L"Return");
                retr.draw();
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
                        if (GetAsyncKeyState(1) && pt.x > retr.x1 && pt.x < retr.x2&&pt.y<retr.y2&&pt.y>retr.y1){
                                retr.invert();
                                presspause();
                                menu.drawblack();
                                retr.drawblack();
                                Draw();
                                break;
                        }
 
                }
        }
 
 
 
 
}



typedef vector<GameObject>::iterator randomAccess_iterator;
typedef vector<GameObject>::const_iterator const_iterator;

// Window constants
const int WindowWidth = 70;
const int WindowHeight = 30;
// Shape
const int ShapeSpeed = 1;
const char ShapeSymbol = '*';
// Game variables
unsigned long sleepDuration = 200;
// No need to reference the active shape, since it will always be the last shape in the list
vector<vector<GameObject>> shapes;


void Update()
{
	COORD direction = { 0, 0 };

	if (kbhit())
	{
		char key = getch();
		switch (key)
		{
		case 'a':
			direction.X = -1;
			break;
		case 'd':
			direction.X = 1;
			break;
		};
	}

	bool hasReachedFloor = false;
	vector<vector<GameObject>>::iterator activeShape = shapes.end() - 1;
	for (randomAccess_iterator shapeNode = activeShape->begin(); shapeNode != activeShape->end(); ++shapeNode)
	{
		shapeNode->Coordinates.X += direction.X;
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
			shapeNode->Coordinates.X -= direction.X;
			shapeNode->Coordinates.Y -= ShapeSpeed;
		}
		
		vector<GameObject> shape;
		int x = rand() % WindowWidth;
		shape.push_back(GameObject(x, 0, ShapeSymbol));
		shape.push_back(GameObject(x, 1, ShapeSymbol));
		shape.push_back(GameObject(x + 1, 0, ShapeSymbol));
		shape.push_back(GameObject(x + 1, 1, ShapeSymbol));
		// Add it to the list, set it as active
		shapes.push_back(shape);
	}
}

void Draw()
{
	ClearScreen(consoleHandle);

	typedef vector<vector<GameObject>>::const_iterator it;
	for (it shape = shapes.begin(); shape != shapes.end(); ++shape)
	{
		for (const_iterator shapeNode = shape->begin(); shapeNode != shape->end(); ++shapeNode)
		{
			shapeNode->Draw(consoleHandle);
		}
	}
}

int main()
{
	consoleHandle = GetStdHandle( STD_OUTPUT_HANDLE );
	// Prepare rand
	srand(time(NULL));
	
	// Create the floor
	vector<GameObject> floor;
	for (int i = 0; i < WindowWidth - 1; i++)
	{
		floor.push_back(GameObject(i, WindowHeight - 1, ShapeSymbol));
	}
	shapes.push_back(floor);


	// Create the first active shape
	vector<GameObject> shape;
	int x = rand() % WindowWidth;
	shape.push_back(GameObject(x, 0, ShapeSymbol));
	shape.push_back(GameObject(x, 1, ShapeSymbol));
	shape.push_back(GameObject(x + 1, 0, ShapeSymbol));
	shape.push_back(GameObject(x + 1, 1, ShapeSymbol));
	// Add it to the list, setting it as active
	shapes.push_back(shape);

	while (true)
	{
		Update();
		Draw();
		Sleep(sleepDuration);
	}

	return 0;
}
