#pragma once
#include <windows.h>
#include <cmath>

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}




// This function checks if any of the 4 cursor keys are pressed. 
// If any cursor key is pressed, then the function returns true, and whichKey identifies which of the 4 cursor keys is pressed.
// whichkey is assigned following values if any cursor key is pressed. 1 for left, 2 for up, 3 for right and 4 for left.
bool isKeyPressed(int& whichKey)   // whichKey passed as reference
{
	char key;

	key = GetAsyncKeyState(37);
	if (key == 1)
	{
		whichKey = 1;   // 1 if left key is pressed
		return true;
	}

	key = GetAsyncKeyState(38);
	if (key == 1)
	{
		whichKey = 2;   // 2 if up key is pressed
		return true;
	}

	key = GetAsyncKeyState(39);
	if (key == 1)
	{
		whichKey = 3;   // 3 if right key is pressed
		return true;
	}

	key = GetAsyncKeyState(40);
	if (key == 1)
	{
		whichKey = 4;   // 4 if down key is pressed
		return true;
	}

	key = GetAsyncKeyState(13);
	if (key == 1)
	{
		whichKey = 5;   // 5 if enter key is pressed
		return true;
	}

	key = GetAsyncKeyState(16);
	if (key == 1)
	{
		whichKey = 6;   // 6 if shift key is pressed
		return true;
	}

	key = GetAsyncKeyState(27);
	if (key == 1)
	{
		whichKey = 7;   // 7 if escape key is pressed
		return true;
	}

	key = GetAsyncKeyState(VK_SPACE);
	if (key == 1)
	{
		whichKey = 8;   // 9 if space key is pressed
		return true;
	}

	key = GetAsyncKeyState(0x51); // Q key
	if (key == 1)
	{
		whichKey = 10;  // 10 if Q key is pressed
		return true;
	}

	key = GetAsyncKeyState(0x50); // P key
	if (key == 1)
	{
		whichKey = 11;  // 11 if P key is pressed
		return true;
	}

	return false;
}
void myLine(int x1, int y1, int x2, int y2, int R, int G = 255, int B = 255) //use three 3 integers if you want colored lines.
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255) to get shades of gray. For other colors use 3 integers for colors.
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B)); //2 is the width of the pen
	SelectObject(device_context, pen);
	MoveToEx(device_context, x1, y1, NULL);
	LineTo(device_context, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);

}
void myRect(int x1, int y1, int x2, int y2, int R, int G, int B, int fillR = 0, int fillG = 0, int fillB = 0)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);
	HBRUSH brush = ::CreateSolidBrush(RGB(fillR, fillG, fillB)); //Fill color can also be passed as parameter to the function!!!

	SelectObject(device_context, brush);

	Rectangle(device_context, x1, y1, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);
}
// Function to draw an equilateral triangle with optional fill color
void myEquilateralTriangle(int x1, int y1, int size, int R, int G, int B, int fillR = 0, int fillG = 0, int fillB = 0) {
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	// Set up the pen and brush
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);
	HBRUSH brush = ::CreateSolidBrush(RGB(fillR, fillG, fillB));
	SelectObject(device_context, brush);

	// Calculate the vertices of the upward-pointing equilateral triangle
	POINT vertices[3];
	vertices[0].x = x1;                         // First vertex (top)
	vertices[0].y = y1;
	vertices[1].x = x1 + (size / 2);            // Second vertex (bottom right)
	vertices[1].y = y1 + static_cast<int>(size * sqrt(3) / 2);  // Height below the top vertex
	vertices[2].x = x1 - (size / 2);            // Third vertex (bottom left)
	vertices[2].y = y1 + static_cast<int>(size * sqrt(3) / 2);  // Height below the top vertex

	// Draw the triangle using the Polygon function
	Polygon(device_context, vertices, 3);

	// Clean up
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);
}
void drawTriangle(int x1, int y1, int L, int R, int G, int B, int fillR = 0, int fillG = 0, int fillB = 0)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	// Calculate the height of the triangle
	int height = static_cast<int>(sqrt(3) / 2 * L);

	// Define the vertices for a vertical body with a horizontal nose
	int x2 = x1;               // x-coordinate for base's bottom
	int y2 = y1 - L / 2;       // y-coordinate for base's bottom

	int x3 = x1;               // x-coordinate for base's top
	int y3 = y1 + L / 2;       // y-coordinate for base's top

	int xNose = x1 + height;   // x-coordinate for the nose
	int yNose = y1;            // y-coordinate for the nose (centered vertically)

	// Change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);

	// Fill color can also be passed as a parameter to the function
	HBRUSH brush = ::CreateSolidBrush(RGB(fillR, fillG, fillB));
	SelectObject(device_context, brush);

	// Define the vertices of the triangle with a vertical body and a horizontal nose
	POINT vertices[] = { {x2, y2}, {x3, y3}, {xNose, yNose} };

	// Draw the triangle using the Polygon function
	Polygon(device_context, vertices, 3);

	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);
}




void myEllipse(int x1, int y1, int x2, int y2, int R, int G, int B, int fillR = 0, int fillG = 0, int fillB = 0)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);
	HBRUSH brush = ::CreateSolidBrush(RGB(fillR, fillG, fillB));  //Fill color is black by default
	SelectObject(device_context, brush);
	Ellipse(device_context, x1, y1, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);


}


void lettersnNumbers(char chr, int x1, int x2, int y1, int y2, int size, int R = 0, int G = 0, int B = 0) {
	x1 += 5;
	x2 -= 5;
	y1 += 5;
	y2 -= 5;
	int mid_x = (x1 + x2) / 2;
	int mid_y = (y1 + y2) / 2;

	if (chr == 'A') {
		myLine(mid_x, y1, x1, y2, R, G, B);
		myLine(mid_x, y1, x2, y2, R, G, B);
		myLine(x1 + 2, mid_y + size / 2, x2 - 3, mid_y + size / 2, R, G, B);
	}
	else if (chr == 'B') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y1, mid_x + size / 4, y1 + size / 8, R, G, B);
		myLine(mid_x + size / 4, y1 + size / 8, mid_x, mid_y, R, G, B);
		myLine(x1, mid_y, mid_x, mid_y, R, G, B);
		myLine(mid_x, mid_y, x2, mid_y + size / 4, R, G, B);
		myLine(x2, mid_y + size / 4, mid_x + size / 8, y2, R, G, B);
		myLine(mid_x + size / 8, y2, x1, y2, R, G, B);
	}
	else if (chr == 'C') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x2, y1, x2, y1 + size / 4, R, G, B);
		myLine(x2, y2, x2, y2 - size / 4, R, G, B);
		myLine(x1, y2, x2, y2, R, G, B);
	}
	else if (chr == 'D') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y1, mid_x + size / 4, y1 + size / 4, R, G, B);
		myLine(mid_x + size / 4, y1 + size / 4, mid_x + size / 4, y2 - size / 4, R, G, B);
		myLine(mid_x + size / 4, y2 - size / 4, x1, y2, R, G, B);
	}
	else if (chr == 'E') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x1, mid_y, x2, mid_y, R, G, B);
		myLine(x1, y2, x2, y2, R, G, B);
	}
	else if (chr == 'F') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x1, mid_y, x2, mid_y, R, G, B);
	}
	else if (chr == 'G') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x2, y1, x2, y1 + size / 4, R, G, B);
		myLine(x2, y2, x2, y2 - size / 2, R, G, B);
		myLine(x2, y2 - size / 2, mid_x, y2 - size / 2, R, G, B);
		myLine(x1, y2, x2, y2, R, G, B);
	}
	else if (chr == 'H') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x2, y1, x2, y2, R, G, B);
		myLine(x1, mid_y, x2, mid_y, R, G, B);
	}
	else if (chr == 'I') {
		myLine(mid_x, y1, mid_x, y2, R, G, B);
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x1, y2, x2, y2, R, G, B);
	}
	else if (chr == 'J') {
		myLine(x2, y1, x2, y2, R, G, B);
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x2, y2, x1, y2, R, G, B);
		myLine(x1, y2, x1, y2 - size / 2, R, G, B);
	}
	else if (chr == 'K') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, mid_y, x2, y1, R, G, B);
		myLine(x1, mid_y, x2, y2, R, G, B);
	}
	else if (chr == 'L') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y2, x2, y2, R, G, B);
	}
	else if (chr == 'M') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y1, mid_x, mid_y, R, G, B);
		myLine(mid_x, mid_y, x2, y1, R, G, B);
		myLine(x2, y1, x2, y2, R, G, B);
	}
	else if (chr == 'N') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y1, x2, y2, R, G, B);
		myLine(x2, y1, x2, y2, R, G, B);
	}
	else if (chr == 'O') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x1, y2, x2, y2, R, G, B);
		myLine(x2, y1, x2, y2, R, G, B);
	}
	else if (chr == 'P') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y1, mid_x + size / 4, y1 + size / 8, R, G, B);
		myLine(mid_x + size / 4, y1 + size / 8, mid_x, mid_y, R, G, B);
		myLine(x1, mid_y, mid_x, mid_y, R, G, B);

	}
	else if (chr == 'Q') {
		myLine(x1, y1, x1, y2 - 5, R, G, B);
		myLine(x1, y1, mid_x + 3, y1, R, G, B);
		myLine(mid_x + 3, y1, mid_x + 3, y2 - 5, R, G, B);
		myLine(x1, y2 - 5, mid_x + 3, y2 - 5, R, G, B);
		myLine(mid_x - 2, mid_y, mid_x + 3, y2 - 5, R, G, B);
		myLine(mid_x + 3, y2 - 5, x2, y2, R, G, B);
	}
	else if (chr == 'R') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y1, mid_x + size / 4, y1 + size / 8, R, G, B);
		myLine(mid_x + size / 4, y1 + size / 8, mid_x, mid_y, R, G, B);
		myLine(x1, mid_y, mid_x, mid_y, R, G, B);
		myLine(mid_x, mid_y, x2, y2, R, G, B);
	}
	else if (chr == 'S') {
		myLine(mid_x, y1, x2, y1 + size / 2, R, G, B);
		myLine(mid_x, y1, x1, y1 + size / 2, R, G, B);
		myLine(x1, y1 + size / 2, x2, y2 - size / 2, R, G, B);
		myLine(x2, y2 - size / 2, mid_x, y2, R, G, B);
		myLine(mid_x, y2, x1, y2 - size / 2, R, G, B);
	}
	else if (chr == 'T') {
		myLine(mid_x, y1, mid_x, y2, R, G, B);
		myLine(x1, y1, x2, y1, R, G, B);
	}
	else if (chr == 'U') {
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, y2, x2, y2, R, G, B);
		myLine(x2, y1, x2, y2, R, G, B);

	}
	else if (chr == 'V') {
		myLine(x1, y1, mid_x, y2, R, G, B);
		myLine(mid_x, y2, x2, y1, R, G, B);
	}
	else if (chr == 'W') {
		myLine(x1, y1, mid_x - size / 8, y2, R, G, B);
		myLine(mid_x - size / 8, y2, mid_x, mid_y, R, G, B);
		myLine(mid_x, mid_y, mid_x + size / 8, y2, R, G, B);
		myLine(mid_x + size / 8, y2, x2, y1, R, G, B);
	}
	else if (chr == 'X') {
		myLine(x1, y1, x2, y2, R, G, B);
		myLine(x2, y1, x1, y2, R, G, B);

	}
	else if (chr == 'Y') {
		myLine(x1, y1, mid_x, mid_y, R, G, B);
		myLine(x2, y1, x1, y2, R, G, B);
	}
	else if (chr == 'Z') {
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x2, y1, x1, y2, R, G, B);
		myLine(x1, y2, x2, y2, R, G, B);
	}
	else if (chr == '.') {
		myRect(x2, y2 - size / 8, mid_x + size / 4, y2, R, G, B);
	}
	else if (chr == ',') {
		myRect(x2, y2 - size / 3, mid_x + size / 4, y2 - size / 6, R, G, B);
		myLine(x2, y2 - size / 3, x2, y2, R);
		myLine(x2, y2, mid_x + size / 4, y2, R);
	}
	else if (chr == '!') {
		myRect(mid_x - size / 8, y1, mid_x - 1, y2 - size / 3, R, G, B);
		myRect(mid_x - size / 8, y2 - size / 8, mid_x, y2, R, G, B);
	}
	else if (chr == ':') {
		myRect(x2, y2 - 20, mid_x + size / 4, y2 - 16, R, G, B);
		myRect(x2, y2 - 4, mid_x + size / 4, y2, R, G, B);
	}
	else if (chr == '0') {
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x2, y1, x2, y2, R, G, B);
		myLine(x1, y2, x2, y2, R, G, B);
		myLine(x2, y1, x1, y2, R, G, B);
	}
	else if (chr == '1') {
		myLine(x1, mid_y - size / 2, mid_x, y1, R, G, B);
		myLine(mid_x, y1, mid_x, y2, R, G, B);
		myLine(x1, y2, x2, y2, R, G, B);
	}
	else if (chr == '2') {
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x2, y1, x2, mid_y, R, G, B);
		myLine(x2, mid_y, x1, mid_y, R, G, B);
		myLine(x1, mid_y, x1, y2, R, G, B);
		myLine(x1, y2, x2, y2, R, G, B);
	}
	else if (chr == '3') {
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x2, y1, x2, mid_y, R, G, B);
		myLine(x2, mid_y, x1, mid_y, R, G, B);
		myLine(x2, mid_y, x2, y2, R, G, B);
		myLine(x2, y2, x1, y2, R, G, B);
	}
	else if (chr == '4') {
		myLine(x1, y1, x1, mid_y, R, G, B);
		myLine(x1, mid_y, x2, mid_y, R, G, B);
		myLine(mid_x + size / 6, y1, mid_x + size / 6, y2, R, G, B);
	}
	else if (chr == '5') {
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x1, y1, x1, mid_y, R, G, B);
		myLine(x1, mid_y, x2, mid_y, R, G, B);
		myLine(x2, mid_y, x2, y2, R, G, B);
		myLine(x2, y2, x1, y2, R, G, B);
	}
	else if (chr == '6') {
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, mid_y, x2, mid_y, R, G, B);
		myLine(x2, mid_y, x2, y2, R, G, B);
		myLine(x2, y2, x1, y2, R, G, B);
	}
	else if (chr == '7') {
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x2, y1, x2, y2, R, G, B);
	}
	else if (chr == '8') {
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x1, y1, x1, y2, R, G, B);
		myLine(x1, mid_y, x2, mid_y, R, G, B);
		myLine(x2, y1, x2, y2, R, G, B);
		myLine(x2, y2, x1, y2, R, G, B);
	}
	else if (chr == '9') {
		myLine(x1, y1, x2, y1, R, G, B);
		myLine(x1, y1, x1, mid_y, R, G, B);
		myLine(x1, mid_y, x2, mid_y, R, G, B);
		myLine(x2, y1, x2, y2, R, G, B);
		myLine(x2, y2, x1, y2, R, G, B);
	}
	else if (chr == '-')
		myRect(x1 - 5, y1 - 5, x2 + 5, y2 + 5, 0, 0, 0);
}
//This Function Converts a Cstring to uppercase
void Capitalizer(char text[]) {
	for (int i = 0; i < strlen(text); i++)
		if (text[i] >= 'a' && text[i] <= 'z')
			text[i] -= 32;
}
void hideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // Get the console handle
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(consoleHandle, &cursorInfo); // Get current cursor settings
	cursorInfo.bVisible = false; // Set cursor visibility to false
	SetConsoleCursorInfo(consoleHandle, &cursorInfo); // Apply the new cursor settings
}
//This Function to print Text on Screen
void drawText(int size, int start_x, int start_y, int R, int G, int B, const char text[], bool isNum = false, int num = 0) {
	int idx = 0;
	int idx_x = 0;
	int idx_y = 0;
	int x1, x2, y1, y2;
	int digit_count = 0;
	int len = strlen(text);
	char temp[100];

	strcpy_s(temp, text);
	//If this function takes a number to print then he converts the number into a string using following loop
	if (isNum == true) {
		int reverse_num = 0;
		int i;

		for (; num > 0; num /= 10, digit_count++)
			reverse_num = (reverse_num * 10) + num % 10;

		for (i = len; i - len < digit_count; i++, reverse_num /= 10)
			temp[i] = (reverse_num % 10) + 48;

		temp[i] = '\0';
	}
	//Now the cstring is converted into upper case letters
	Capitalizer(temp);
	//This loop prints each individual letter on the screen
	for (idx = 0; idx < strlen(temp); idx++)
	{
		x1 = start_x + size * idx_x;
		x2 = start_x + size * (idx_x + 1);

		if (x2 > 1585 - size) {
			idx_y++;
			idx_x = 0;
			x1 = start_x + size * idx_x;
			x2 = start_x + size * (idx_x + 1);

		}

		y1 = start_y + 2 * size * idx_y;
		y2 = start_y + 2 * size * (idx_y + 1);
		lettersnNumbers(temp[idx], x1, x2, y1, y2, size, R, G, B);
		idx_x++;
	}

}
