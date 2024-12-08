#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include "help.h"
#include <thread>
#include <chrono>
#include <windows.h>
#include <mmsystem.h>
using namespace std;
using namespace std::chrono;


#pragma comment(lib,"winmm.lib")

// Global Variables 

int window_height = 690, window_width = 1340;

int selectedMenuOption = 0;
int selectedGameOption =0 ;
int selectedDiffultyOption=0 ;
int selectedPauseMenuOption =0;
int selectedGameOverOption =0;

bool clear_screen ;
bool player_clear ;

bool easyGame ;
bool mediumGame ;
bool hardGame ;


bool gameloaded ;

bool easyGameLoaded ;
bool mediumGameLoaded ;
bool hardGameLoaded ;

int highscore_easy = 550;
int highscore_medium = 700;
int highscore_hard = 850;

bool firstWave ;
bool secondWave ;
bool thirdWave ;

//Bullets
const int max_bullet_count = 999;
int bullet_count ;
const float bullet_Speed = 30;
float bullets_x[max_bullet_count] = { -1 };
float bullets_y[max_bullet_count] ={ -1 };
bool bullets_active[max_bullet_count] = { false };
//Score

int scorenum ;
const int max_scores = 5;
int high_scores[max_scores] = { 0 };

//Aliens
int alien_width = 40, alien_height = 40;

int xSpacing = 10;
int ySpacing = 15;

const int rows = 10;

float alien_Speed = 3;
float base_speed = 3;

const int colsfeasy = 5;
const int colsfmedium = 6;
const int colsfhard = 7;

const int alien_count_easy = 50;
const int alien_count_medium = 60;
const int alien_count_hard = 70;

float easy_x[alien_count_easy];
float easy_y[alien_count_easy];
bool easy_active[alien_count_easy];

float medium_x[alien_count_medium];
float medium_y[alien_count_medium];
bool medium_active[alien_count_medium];

float hard_x[alien_count_hard];
float hard_y[alien_count_hard];
bool hard_active[alien_count_hard];

int killed_alien_count ;
int alien_activated ;

//PLayer
int player_lives ;

const int blockheight = 5;
const int blockwidth = 5;

float player_x ;
float player_y ;

int player_width = blockwidth*10 ;
int player_height = blockheight*12;



int boss1_hits;
int boss2_hits;
int boss3_hits;

float boss1_x ;
float boss1_y ;
float boss1_y_range ;
bool boss1_active ;
float boss2_x ;
float boss2_y ;
bool boss2_active ;

float boss3_x ;
float boss3_y;
bool boss3_active;

struct PlayerBullet {
	float x = -1;
	float y = -1;
	bool active = false;

	void drawBullet() {
		myRect(x, y, x + 20, y + 5, 0, 0, 255, 0, 0, 255);
	}

	void eraseBullet() {
		myRect(x, y, x + 20, y + 5, 0, 0, 0, 0, 0, 0);
	}

	void moveBullet() {
		if (active) {
			eraseBullet();
			x += 10;
			if (x > 1360) {
				active = false;
			}
			else {
				drawBullet();
			}
		}
	}
};
PlayerBullet player_bullets[max_bullet_count];

struct BossBullet {
	float x = -1;
	float y = -1;
	bool active = false;

	void drawBullet() {
		myRect(x, y, x + 20, y + 5, 255, 0, 0, 255, 0, 0);
	}

	void eraseBullet() {
		myRect(x, y, x + 20, y + 5, 0, 0, 0, 0, 0, 0);
	}

	void moveBullet() {
		if (active) {
			eraseBullet();
			x -= 15;
			if (x < 0) {
				active = false;
			}
			else {
				drawBullet();
			}
		}
	}
};



BossBullet boss1_bullet1, boss1_bullet2;
BossBullet boss2_bullet1, boss2_bullet2;
BossBullet boss3_bullet1, boss3_bullet2;

int boss1_y_direction = 1;
int boss2_y_direction = 1;
int boss3_y_direction = 1;

//Functions Prototype
void clearScreen();
void initializeGlobalVariables();
void drawLives();

void drawAlien(int x, int y);

void drawAliens(float alien_x[], float alien_y[], bool alien_active[], int alien_count);

void moveBullets(float alien_x[], float alien_y[], bool alien_active[], int alien_count);

void checkPlayerCollision(float alien_x[], float alien_y[], bool alien_active[], int alien_count);
void eraseAlien(int x, int y);
void resetGame();
void resetAliens();
void moveAliens();
void playBossArrivalSound();
void drawPauseMenu(int selected);
void handlePauseMenuOptions();
void restoreGame();
void drawScorePage();
void handlescorePage();
void saveGame(const string& filename, int alien_count, float alien_x[], float alien_y[], bool alien_active[]);
void loadGame(const string& filename, int alien_count, float alien_x[], float alien_y[], bool alien_active[]);
void detectAndLoadGame(const string& filename);
void initializeAliens();
void displayTimer(int seconds, int previousSeconds);
void drawMenu(int selected);
void playExplosionSound();
void handleMenuInput();
void drawGameMenu(int selected);
void handleGameOptions();
void drawDifficultyMenu(int selected);
void handleDiffultyOptions();
void drawGameOverPage(int selected);
void handleGameOverOptions();
void playBackgroundMusic();
void initializeAliens();
void loadHighScores(const string& filename);
void saveHighScores(const string& filename);
void updateHighScores(int current_score);
void  bosslevel();
void playerSpaceshipBulletBossCollision();
void bossBulletcollision();
void moveBoss3();
void moveBoss2();
void moveBoss1();
void drawBossship(float x, float y);
void eraseBossship(float x, float y);
void drawPlayerShip();

void erasePlayerShip();
int main() {
	initializeGlobalVariables();
	hideCursor();
	loadHighScores("highscores.txt");
	drawMenu(selectedMenuOption);
	handleMenuInput();

	return 0;
}

// Functions Declaration

void clearScreen() {
	myRect(0, 0, window_width, window_height, 0, 0, 0, 0, 0, 0);
}
void initializeGlobalVariables() {
	playBackgroundMusic();
	selectedMenuOption = 0;
	selectedGameOption = 0;
	selectedDiffultyOption = 0;
	selectedPauseMenuOption = 0;
	selectedGameOverOption = 0;

	clear_screen = true;
	player_clear = true;

	easyGame = false;
	mediumGame = false;
	hardGame = false;

	gameloaded = false;

	easyGameLoaded = false;
	mediumGameLoaded = false;
	hardGameLoaded = false;

	firstWave = true;
	secondWave = false;
	thirdWave = false;
	bullet_count = 0;
	

	killed_alien_count = -1;
	alien_activated = -1;
	player_lives = 3;


	player_x = 50;
	player_y = (window_height - 100) / 2;


	boss1_hits = 0;
	boss2_hits = 0;
	boss3_hits = 0;

	boss1_x = 1200;
	boss1_y = 150;
	boss1_y_range = 600;
	boss1_active = true;
	boss2_x = 1200;
	boss2_y = 350;
	boss2_active = false;
	boss3_x = 1100;
	boss3_y = 500;
	boss3_active = false;
	scorenum = 0;
}
void loadHighScores(const string& filename) {
	ifstream file(filename);
	if (!file) {
		for (int i = 0; i < max_scores; i++) {
			high_scores[i] = 0;
		}
		return;
	}
	for (int i = 0; i < max_scores && file >> high_scores[i]; i++);
	file.close();
}

void saveHighScores(const string& filename) {
	ofstream file(filename);
	for (int i = 0; i < max_scores; i++) {
		file << high_scores[i] << endl;
	}
	file.close();
}
void updateHighScores(int current_score) {
	for (int i = 0; i < max_scores; i++) {
		if (current_score > high_scores[i]) {
			for (int j = max_scores - 1; j > i; j--) {
				high_scores[j] = high_scores[j - 1];
			}
			high_scores[i] = current_score;
			break;
		}
	}
}

void drawScorePage() {
	myRect(0, 0, window_width, window_height, 0, 0, 0, 0, 0, 0);
	drawText(30, 200, 50, 255, 0, 0, "HIGH SCORES");

	for (int i = 0; i < max_scores; i++) {
		int var = i + 1;
		drawText(30, 200, 150 + (i * 70), 255, 255, 255, to_string(var).c_str());

		drawText(30, 275, 150 + (i * 70), 255, 255, 255, to_string(high_scores[i]).c_str());
	}
	myRect(200, 530, 400, 630, 0, 255, 215);
	drawText(30, 250, 550, 255, 255, 255, "BACK");
}
void handlescorePage() {
	int key;
	while (true) {
		if (isKeyPressed(key)) {
			if (key == 5) {
				clearScreen();
				drawMenu(selectedMenuOption);
				handleMenuInput();
				return;
			}
		}
	}
}
void drawLives() {
	int startX = 1180;
	int topY = 15;
	int bottomY = 50;
	int r, g, b;
	int fillR, fillG, fillB;

	for (int i = 0; i < 3; ++i) {
		if (i < player_lives) {
			fillR = 0, fillB = 0;  fillG = 255;
			r = 0; g = 255; b = 0;
		}
		else {
			fillR = 0, fillB = 0;  fillG = 0;
			r = 0; g = 0; b = 0;
		}

		int leftX = startX + (i * 50);
		int rightX = leftX + 40;

		myEllipse(leftX, topY, rightX, bottomY, r, g, b, fillR, fillG, fillB);
	}
}
void initializeAliens() {
	if (easyGame) {
		int startX = window_width - alien_width - xSpacing;
		int startY = 90;


		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < colsfeasy; ++col) {
				int x = startX - col * (alien_width + xSpacing);
				int y = startY + row * (alien_height + ySpacing);


				easy_x[row * colsfeasy + col] = x;
				easy_y[row * colsfeasy + col] = y;
				easy_active[row * colsfeasy + col] = true;
			}
		}
	}
	else if (mediumGame) {
		int startX = window_width;
		int startY = 90;


		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < colsfmedium; ++col) {
				int x = startX - col * (alien_width + xSpacing);
				int y = startY + row * (alien_height + ySpacing);


				medium_x[row * colsfmedium + col] = x;
				medium_y[row * colsfmedium + col] = y;
				medium_active[row * colsfmedium + col] = true;
			}
		}
	}
	else if (hardGame) {
		int startX = window_width + alien_width + xSpacing;
		int startY = 90;


		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < colsfhard; ++col) {
				int x = startX - col * (alien_width + xSpacing);
				int y = startY + row * (alien_height + ySpacing);


				hard_x[row * colsfhard + col] = x;
				hard_y[row * colsfhard + col] = y;
				hard_active[row * colsfhard + col] = true;
			}
		}
	}
}

void drawAlien(int x, int y) {
	if (firstWave)
	{
		myRect(x + 15, y, x + 25, y + 5, 103, 108, 155, 103, 108, 155);
		myRect(x + 10, y + 5, x + 30, y + 9, 103, 108, 155, 103, 108, 155);
		myRect(x + 5, y + 9, x + 35, y + 14, 103, 108, 155, 103, 108, 155);
		myRect(x, y + 14, x + alien_width, y + 25, 103, 108, 155, 103, 108, 155);
		myRect(x + 10, y + 25, x + 15, y + 30, 103, 108, 155, 103, 108, 155);
		myRect(x + 5, y + 30, x + 10, y + 35, 103, 108, 155, 103, 108, 155);
		myRect(x, y + 35, x + 5, y + alien_height, 103, 108, 155, 103, 108, 155);
		myRect(x + 25, y + 25, x + 30, y + 30, 103, 108, 155, 103, 108, 155);
		myRect(x + 30, y + 30, x + 35, y + 35, 103, 108, 155, 103, 108, 155);
		myRect(x + 35, y + 35, x + alien_width, y + alien_height, 103, 108, 155, 103, 108, 155);
		myRect(x + 10, y + 15, x + 15, y + 20, 0, 0, 0, 0, 0, 0);
		myRect(x + 25, y + 15, x + 30, y + 20, 0, 0, 0, 0, 0, 0);
		myRect(x + 15, y + 30, x + 25, y + 35, 103, 108, 155, 103, 108, 155);
		myRect(x + 10, y + 35, x + 15, y + alien_height, 103, 108, 155, 103, 108, 155);
		myRect(x + 25, y + 35, x + 30, y + alien_height, 103, 108, 155, 103, 108, 155);
	}
	else if (secondWave)
	{
		myRect(x + 10, y, x + 30, y + 5, 0, 255, 255, 0, 255, 255);
		myRect(x + 5, y + 5, x + 35, y + 10, 0, 255, 255, 0, 255, 255);
		myRect(x, y + 10, x + alien_width, y + 25, 0, 255, 255, 0, 255, 255);
		myRect(x + 10, y + 25, x + 15, y + 30, 0, 255, 255, 0, 255, 255);
		myRect(x + 5, y + 30, x + 10, y + 35, 0, 255, 255, 0, 255, 255);
		myRect(x, y + 35, x + 5, y + alien_height, 0, 255, 255, 0, 255, 255);
		myRect(x + 25, y + 25, x + 30, y + 30, 0, 255, 255, 0, 255, 255);
		myRect(x + 30, y + 30, x + 35, y + 35, 0, 255, 255, 0, 255, 255);
		myRect(x + 35, y + 35, x + alien_width, y + alien_height, 0, 255, 255, 0, 255, 255);
		myRect(x + 15, y + 30, x + 25, y + 35, 0, 255, 255, 0, 255, 255);
		myRect(x + 7, y + 12, x + 15, y + 17, 0, 0, 0, 0, 0, 0);
		myRect(x + 25, y + 12, x + 33, y + 17, 0, 0, 0, 0, 0, 0);
	}
	else if (thirdWave)
	{
		myRect(x, y, x + 5, y + 10, 255, 0, 0, 255, 0, 0);
		myRect(x, y + 12, x + alien_width, y + 18, 255, 0, 0, 255, 0, 0);
		myRect(x + 35, y, x + alien_width, y + 10, 255, 0, 0, 255, 0, 0);
		myRect(x + 5, y + 20, x + 5 + 30, y + 28, 255, 0, 0, 255, 0, 0);
		myRect(x, y + 30, x + 5, y + 40, 255, 0, 0, 255, 0, 0);
		myRect(x + 35, y + 30, x + 35 + 5, y + 40, 255, 0, 0, 255, 0, 0);
	}
}

void drawAliens(float alien_x[], float alien_y[], bool alien_active[], int alien_count) {
	for (int i = 0; i < alien_count; i++) {
		if (alien_active[i]) {
			drawAlien(alien_x[i], alien_y[i]);
		}
	}
}
void moveBullets(float alien_x[], float alien_y[], bool alien_active[], int alien_count) {
	for (int i = 0; i < bullet_count; i++) {
		if (bullets_x[i] != -1 && bullets_y[i] != -1) {
			myRect(bullets_x[i], bullets_y[i], bullets_x[i] + 15, bullets_y[i] + 5, 0, 0, 0);
			bullets_x[i] += bullet_Speed;

			if (bullets_x[i] > window_width - 21) {
				bullets_x[i] = -1;
				bullets_y[i] = -1;
			}
			else {
				myRect(bullets_x[i], bullets_y[i], bullets_x[i] + 15, bullets_y[i] + 5, 0, 0, 255, 0, 0, 255);
				for (int j = 0; j < alien_count; j++) {
					if (alien_active[j] && bullets_x[i] >= alien_x[j] && bullets_x[i] <= alien_x[j] + alien_width &&
						bullets_y[i] >= alien_y[j] && bullets_y[i] <= alien_y[j] + alien_height)
					{
						myRect(bullets_x[i], bullets_y[i], bullets_x[i] + 15, bullets_y[i] + 5, 0, 0, 0);
						alien_active[j] = false;
						killed_alien_count++;
						alien_activated--;
						eraseAlien(alien_x[j], alien_y[j]);
						bullets_x[i] = -1;
						bullets_y[i] = -1;
						drawText(20, 120, 15, 0, 0, 0, "", true, scorenum);
						scorenum += 10;
						drawText(20, 120, 15, 0, 255, 0, "", true, scorenum);
						break;
					}
				}
			}
		}
	}
}

void eraseAlien(int x, int y) {
	if (firstWave) {
		myRect(x + 15, y, x + 25, y + 5, 0, 0, 0, 0, 0, 0);
		myRect(x + 10, y + 5, x + 30, y + 9, 0, 0, 0, 0, 0, 0);
		myRect(x + 5, y + 9, x + 35, y + 14, 0, 0, 0, 0, 0, 0);
		myRect(x, y + 14, x + alien_width, y + 25, 0, 0, 0, 0, 0, 0);
		myRect(x + 10, y + 25, x + 15, y + 30, 0, 0, 0, 0, 0, 0);
		myRect(x + 5, y + 30, x + 10, y + 35, 0, 0, 0, 0, 0, 0);
		myRect(x, y + 35, x + 5, y + alien_height, 0, 0, 0, 0, 0, 0);
		myRect(x + 25, y + 25, x + 30, y + 30, 0, 0, 0, 0, 0, 0);
		myRect(x + 30, y + 30, x + 35, y + 35, 0, 0, 0, 0, 0, 0);
		myRect(x + 35, y + 35, x + alien_width, y + alien_height, 0, 0, 0, 0, 0, 0);
		myRect(x + 10, y + 15, x + 15, y + 20, 0, 0, 0, 0, 0, 0);
		myRect(x + 25, y + 15, x + 30, y + 20, 0, 0, 0, 0, 0, 0);
		myRect(x + 15, y + 30, x + 25, y + 35, 0, 0, 0, 0, 0, 0);
		myRect(x + 10, y + 35, x + 15, y + alien_height, 0, 0, 0, 0, 0, 0);
		myRect(x + 25, y + 35, x + 30, y + alien_height, 0, 0, 0, 0, 0, 0);
	}
	else if (secondWave) {
		myRect(x + 10, y, x + 30, y + 5, 0, 0, 0, 0, 0, 0);
		myRect(x + 5, y + 5, x + 35, y + 10, 0, 0, 0, 0, 0, 0);
		myRect(x, y + 10, x + alien_width, y + 25, 0, 0, 0, 0, 0, 0);
		myRect(x + 10, y + 25, x + 15, y + 30, 0, 0, 0, 0, 0, 0);
		myRect(x + 5, y + 30, x + 10, y + 35, 0, 0, 0, 0, 0, 0);
		myRect(x, y + 35, x + 5, y + alien_height, 0, 0, 0, 0, 0, 0);
		myRect(x + 25, y + 25, x + 30, y + 30, 0, 0, 0, 0, 0, 0);
		myRect(x + 30, y + 30, x + 35, y + 35, 0, 0, 0, 0, 0, 0);
		myRect(x + 35, y + 35, x + alien_width, y + alien_height, 0, 0, 0, 0, 0, 0);
		myRect(x + 15, y + 30, x + 25, y + 35, 0, 0, 0, 0, 0, 0);
		myRect(x + 7, y + 12, x + 15, y + 17, 0, 0, 0, 0, 0, 0);
		myRect(x + 25, y + 12, x + 33, y + 17, 0, 0, 0, 0, 0, 0);
	}
	else if (thirdWave) {
		myRect(x, y, x + 5, y + 10, 0, 0, 0, 0, 0, 0);
		myRect(x, y + 12, x + alien_width, y + 18, 0, 0, 0, 0, 0, 0);
		myRect(x + 35, y, x + alien_width, y + 10, 0, 0, 0, 0, 0, 0);
		myRect(x + 5, y + 20, x + 5 + 30, y + 28, 0, 0, 0, 0, 0, 0);
		myRect(x, y + 30, x + 5, y + 40, 0, 0, 0, 0, 0, 0);
		myRect(x + 35, y + 30, x + 35 + 5, y + 40, 0, 0, 0, 0, 0, 0);
	}

}

void checkPlayerCollision(float alien_x[], float alien_y[], bool alien_active[], int alien_count) {
	for (int i = 0; i < alien_count; i++) {
		if (alien_active[i] && player_x < alien_x[i] + alien_width &&
			player_x + player_width > alien_x[i] && player_y < alien_y[i] + alien_height &&
			player_y + player_height > alien_y[i]) {

			player_lives--;
			if (player_lives == 2) {
				secondWave = true;
				firstWave = false;
				thirdWave = false;
				resetGame();
			}
			else if (player_lives == 1) {
				thirdWave = true;
				firstWave = false;
				secondWave = false;
				resetGame();
			}
			else {
				updateHighScores(scorenum);
				saveHighScores("highscores.txt");
			}
			return;
		}
	}
}

void resetAliens() {
	if (easyGame) {

		for (int i = 0; i < alien_count_easy; i++) {
			if (easy_active[i]) {
				easy_x[i] = (window_width - alien_width - xSpacing) - (i % colsfeasy) * (alien_width + xSpacing);
				easy_y[i] = 90 + (i / colsfeasy) * (alien_height + ySpacing);
			}
		}
	}
	else if (mediumGame) {

		for (int i = 0; i < alien_count_medium; i++) {
			if (medium_active[i]) {

				medium_x[i] = (window_width + (2 * (alien_width + xSpacing))) - (i % colsfmedium) * (alien_width + xSpacing);
				medium_y[i] = 90 + (i / colsfmedium) * (alien_height + ySpacing);
			}
		}
	}
	else if (hardGame) {


		for (int i = 0; i < alien_count_hard; i++) {
			if (hard_active[i]) {

				hard_x[i] = (window_width + (4 * (alien_width + xSpacing))) - (i % colsfhard) * (alien_width + xSpacing);
				hard_y[i] = 0 + (i / colsfhard) * (alien_height + ySpacing);
			}
		}
	}
}

void resetGame() {
	myRect(2, 2, 1340, 690, 0, 0, 0);

	player_x = 50;
	player_y = (window_height - 100) / 2;


	myRect(2, 2, 1340, 690, 0, 255, 0);
	myLine(2, 60, 1340, 60, 0, 255, 0);



	if (easyGame) {
		drawText(20, 5, 15, 255, 255, 255, "Score : ");
		drawText(20, 120, 15, 0, 255, 0, "",true,scorenum);
		drawText(1080, 5, 15, 255, 255, 255, "Lives: ");
		drawLives();
		drawText(20, 400, 15, 255, 255, 255, "Easy Mode");
	}
	else if (mediumGame) {
		drawText(20, 5, 15, 255, 255, 255, "Score : ");
		drawText(20, 120, 15, 0, 255, 0, "", true, scorenum);
		drawText(1080, 5, 15, 255, 255, 255, "Lives: ");
		drawLives();
		drawText(20, 400, 15, 255, 255, 255, "Medium Mode");
	}
	else if (hardGame) {
		drawText(20, 5, 15, 255, 255, 255, "Score : ");
		drawText(20, 120, 15, 0, 255, 0, "", true, scorenum);
		drawText(1080, 5, 15, 255, 255, 255, "Lives: ");
		drawLives();
		drawText(20, 400, 15, 255, 255, 255, "Hard Mode");
	}

	resetAliens();
	if (easyGame) {
		drawAliens(easy_x, easy_y, easy_active, alien_count_easy);
	}
	if (mediumGame) {
		drawAliens(medium_x, medium_y, medium_active, alien_count_medium);
	}
	if (hardGame) {
		drawAliens(hard_x, hard_y, hard_active, alien_count_hard);
	}
	clear_screen = true;
	// Redraw Score and Lives

}

void moveAliens() {
	if (easyGame) {
		for (int i = 0; i < alien_count_easy; i++) {

			if (easy_active[i]) {
				if (alien_activated < 10 ) {
					std::this_thread::sleep_for(std::chrono::microseconds(50));
				}
				eraseAlien(easy_x[i], easy_y[i]);
				easy_x[i] -= alien_Speed;
				drawAlien(easy_x[i], easy_y[i]);
			}
			if (easy_active[i] && easy_x[i] < 0) {
				player_lives--;
				if (player_lives == 2) {
					secondWave = true;
					firstWave = false;
					thirdWave = false;
					resetGame();
				}
				else if (player_lives == 1) {
					thirdWave = true;
					firstWave = false;
					secondWave = false;
					resetGame();
				}
				else {
					clearScreen();
					drawGameOverPage(selectedGameOverOption);
					handleGameOverOptions();
					updateHighScores(scorenum);
					saveHighScores("highscores.txt");
				}
				return;
			}
		}
	}
	else if (mediumGame) {
		for (int i = 0; i < alien_count_medium; i++) {
			if (medium_active[i]) {
				if (alien_activated < 10) {
					std::this_thread::sleep_for(std::chrono::microseconds(50));
				}
				eraseAlien(medium_x[i], medium_y[i]);
				medium_x[i] -= alien_Speed;
				drawAlien(medium_x[i], medium_y[i]);
			}
			if (medium_active[i] && medium_x[i] < 0) {
				player_lives--;
				if (player_lives == 2) {
					secondWave = true;
					firstWave = false;
					thirdWave = false;
					resetGame();
				}
				else if (player_lives == 1) {
					thirdWave = true;
					firstWave = false;
					secondWave = false;
					resetGame();
				}
				else {
					clearScreen();
					drawGameOverPage(selectedGameOverOption);
					handleGameOverOptions();
					updateHighScores(scorenum);
					saveHighScores("highscores.txt");
				}
				return;
			}
		}
	}
	else if (hardGame) {
		for (int i = 0; i < alien_count_hard; i++) {
			if (hard_active[i]) {
				if (alien_activated < 10) {
					std::this_thread::sleep_for(std::chrono::microseconds(50));
				}
				eraseAlien(hard_x[i], hard_y[i]);
				hard_x[i] -= alien_Speed;
				drawAlien(hard_x[i], hard_y[i]);
			}
			if (hard_active[i] && hard_x[i] < 0) {
				player_lives--;
				if (player_lives == 2) {
					secondWave = true;
					firstWave = false;
					thirdWave = false;
					resetGame();
				}
				else if (player_lives == 1) {
					thirdWave = true;
					firstWave = false;
					secondWave = false;
					resetGame();
				}
				else {
					clearScreen();
					drawGameOverPage(selectedGameOverOption);
					handleGameOverOptions();
					updateHighScores(scorenum);
					saveHighScores("highscores.txt");
				}
				return;
			}
		}
	}
}

void drawPauseMenu(int selected) {

	if (clear_screen) {
		clearScreen();
		clear_screen = false;
	}

	drawText(30, 220, 65, 255, 215, 0, "Pause Menu");

	int yPosition = 150;

	if (selected == 0) {
		myRect(200, yPosition, 600, yPosition + 75, 0, 255, 255);
		drawText(25, 300, yPosition + 15, 0, 255, 255, "Save");
	}
	else {
		myRect(200, yPosition, 600, yPosition + 75, 255, 255, 255);
		drawText(25, 300, yPosition + 15, 255, 255, 255, "Save");
	}

	yPosition += 100;
	if (selected == 1) {
		myRect(200, yPosition, 600, yPosition + 75, 0, 255, 255);
		drawText(25, 250, yPosition + 15, 0, 255, 255, "Load Game");
	}
	else {
		myRect(200, yPosition, 600, yPosition + 75, 255, 255, 255);
		drawText(25, 250, yPosition + 15, 255, 255, 255, "Load Game");
	}

	yPosition += 100;
	if (selected == 2) {
		myRect(200, yPosition, 600, yPosition + 75, 0, 255, 255);
		drawText(25, 250, yPosition + 15, 0, 255, 255, "Main Menu");
	}
	else {
		myRect(200, yPosition, 600, yPosition + 75, 255, 255, 255);
		drawText(25, 250, yPosition + 15, 255, 255, 255, "Main Menu");
	}

	yPosition += 100;
	if (selected == 3) {
		myRect(200, yPosition, 600, yPosition + 75, 0, 255, 255);
		drawText(25, 250, yPosition + 15, 0, 255, 255, "Resume");
	}
	else {
		myRect(200, yPosition, 600, yPosition + 75, 255, 255, 255);
		drawText(25, 250, yPosition + 15, 255, 255, 255, "Resume");
	}


}
void handlePauseMenuOptions() {
	drawPauseMenu(selectedPauseMenuOption);

	while (true) {
		int key = -1;
		if (isKeyPressed(key)) {
			if (key == 2) {
				selectedPauseMenuOption = (selectedPauseMenuOption - 1 + 4) % 4;
				drawPauseMenu(selectedPauseMenuOption);
			}
			else if (key == 4) {
				selectedPauseMenuOption = (selectedPauseMenuOption + 1) % 4;
				drawPauseMenu(selectedPauseMenuOption);
			}
			else if (key == 5) {
				switch (selectedPauseMenuOption) {
				case 0:
					cout << "Saved..." << endl;
					if (easyGame)
					{
						saveGame("savedgame.txt", alien_count_easy, easy_x, easy_y, easy_active);
					}
					if (mediumGame) {
						saveGame("savedgame.txt", alien_count_medium, medium_x, medium_y, medium_active);
					}
					if (hardGame) {
						saveGame("savedgame.txt", alien_count_hard, hard_x, hard_y, hard_active);
					}
					drawPauseMenu(selectedPauseMenuOption);
					handlePauseMenuOptions();
					return;
				case 1:
					clearScreen();
					if (easyGame)
					{
						loadGame("savedgame.txt", alien_count_easy, easy_x, easy_y, easy_active);
					}
					if (mediumGame) {
						loadGame("savedgame.txt", alien_count_medium, medium_x, medium_y, medium_active);
					}
					if (hardGame) {
						loadGame("savedgame.txt", alien_count_hard, hard_x, hard_y, hard_active);
					}
					restoreGame();
					return;
				case 2:
					clearScreen();
					drawMenu(selectedMenuOption);
					handleMenuInput();
					break;

				case 3:
					clearScreen();
					restoreGame();
					return;
				}
				break;
			}
		}
	}
}

void restoreGame() {
	myRect(2, 2, 1340, 690, 0, 255, 0);
	myLine(2, 60, 1340, 60, 0, 255, 0);
	drawText(20, 5, 15, 255, 255, 255, "Score : ");
	drawText(20, 120, 15, 0, 255, 0, "",true, scorenum);
	drawText(20, 1050, 15, 255, 255, 255, " Lives ");
	drawLives();
	if (easyGame) {
		drawText(20, 400, 15, 255, 255, 255, "Easy Mode");
	}
	else if (mediumGame) {
		drawText(20, 400, 15, 255, 255, 255, "Medium Mode");
	}
	else if (hardGame) {
		drawText(20, 400, 15, 255, 255, 255, "Hard Mode");
	}
	if (easyGame) {
		drawAliens(easy_x, easy_y, easy_active, alien_count_easy);
	}
	if (mediumGame) {
		drawAliens(medium_x, medium_y, medium_active, alien_count_medium);
	}
	if (hardGame) {
		drawAliens(hard_x, hard_y, hard_active, alien_count_hard);
	}

	drawPlayerShip();
}
void playBossArrivalSound() {
	PlaySound(TEXT("boss.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void saveGame(const string& filename, int alien_count, float alien_x[], float alien_y[], bool alien_active[]) {
	ofstream file(filename);
	if (easyGame) {
		file << "GameMode=" << "easyMode" << "\n";
	}
	if (mediumGame) {
		file << "GameMode=" << "mediumMode" << "\n";
	}
	if (hardGame) {
		file << "GameMode=" << "hardMode" << "\n";
	}
	file << "player_x " << fixed << setprecision(2) << player_x << "\n";
	file << "player_y " << fixed << setprecision(2) << player_y << "\n";

	for (int i = 0; i < alien_count; i++) {
		file << "Alien_x " << fixed << setprecision(2) << alien_x[i] << "\n";
		file << "Alien_y " << fixed << setprecision(2) << alien_y[i] << "\n";
		file << "State " << (alien_active[i] ? 1 : 0) << "\n";
	}

	file << "Score=" << scorenum << "\n";
	file << "Lives=" << player_lives << "\n";
}
void loadGame(const string& filename, int alien_count, float alien_x[], float alien_y[], bool alien_active[]) {
	ifstream file(filename);
	string line;
	int aliencount = 0;

	while (getline(file, line)) {
		if (line.find("player_x") != string::npos) {
			stringstream ss(line.substr(line.find(" ") + 1));
			ss >> player_x;
		}
		else if (line.find("player_y") != string::npos) {
			stringstream ss(line.substr(line.find(" ") + 1));
			ss >> player_y;
		}
		else if (line.find("Alien_x") != string::npos && aliencount < alien_count) {
			stringstream ss(line.substr(line.find(" ") + 1));
			ss >> alien_x[aliencount];

			getline(file, line);
			ss.clear();
			ss.str(line.substr(line.find(" ") + 1));
			ss >> alien_y[aliencount];

			getline(file, line);
			ss.clear();
			ss.str(line.substr(line.find(" ") + 1));
			int state;
			ss >> state;
			alien_active[aliencount] = (state == 1);

			aliencount++;
		}
		else if (line.find("Score") != string::npos) {
			stringstream ss(line.substr(line.find("=") + 1));
			ss >> scorenum;

		}
		else if (line.find("Lives") != string::npos) {
			stringstream ss(line.substr(line.find("=") + 1));
			ss >> player_lives;

		}
	}
}
void detectAndLoadGame(const string& filename) {
	ifstream file(filename);
	string line;

	if (getline(file, line) && line.find("GameMode") != string::npos) {
		if (line.find("easyMode") != string::npos) {
			easyGameLoaded = true;
			loadGame(filename, alien_count_easy, easy_x, easy_y, easy_active);
		}
		else if (line.find("mediumMode") != string::npos) {
			mediumGameLoaded = true;
			loadGame(filename, alien_count_medium, medium_x, medium_y, medium_active);
		}
		else if (line.find("hardMode") != string::npos) {
			hardGameLoaded = true;
			loadGame(filename, alien_count_hard, hard_x, hard_y, hard_active);
		}
	}
	else {
		cout << "Error: Game mode not found in save file." << endl;
	}
}

void displayTimer(int seconds, int previousSeconds) {

	if (seconds != previousSeconds) {

		drawText(20, 960, 15, 0, 0, 0, "", true, previousSeconds);

		drawText(20, 960, 15, 0, 255, 0, "", true, seconds);
	}
}
void drawGame(int alien_count, float alien_x[], float alien_y[], bool alien_active[]) {
	myRect(2, 2, 1340, 690, 0, 255, 0);
	myLine(2, 60, 1340, 60, 0, 255, 0);
	if (!gameloaded) {
		initializeAliens();
	}
	drawPlayerShip();
	drawAliens(alien_x, alien_y, alien_active, alien_count);
	drawText(20, 5, 15, 255, 255, 255, "Score : ");
	if (scorenum == 0) {
		drawText(20, 120, 15, 0, 255, 0, "", true, 0);
	}

	drawText(20, 1050, 15, 255, 255, 255, " Lives ");
	drawLives();

	if (easyGame) {
		drawText(20, 400, 15, 255, 255, 255, "Easy Mode");
		alien_activated = 50;
	}
	else if (mediumGame) {
		drawText(20, 400, 15, 255, 255, 255, "Medium Mode");
		alien_activated == 60;
	}
	else if (hardGame) {
		drawText(20, 400, 15, 255, 255, 255, "Hard Mode");
		alien_activated = 70;
	}
}
void playGame(int alien_count, float alien_x[], float alien_y[], bool alien_active[]) {
	bool isRunning = true;
	int framecount = 0;
	drawGame(alien_count, alien_x, alien_y, alien_active);

	while (isRunning && player_lives > 0) {
		int key = -1;
		if (isKeyPressed(key)) {
			if (key == 1 || key == 12) {
				if (player_x - 20 > 5) {
					erasePlayerShip();
					player_x += -20;
					drawPlayerShip();
				}
			}
			else if (key == 2 || key == 13) {
				if (player_y - 20 > 60) {
					erasePlayerShip();
					player_y += -20;
					drawPlayerShip();
				}
			}
			else if (key == 3 || key == 14) {
				if (player_x + 20 < window_width - 20) {
					erasePlayerShip();
					player_x += 20;
					drawPlayerShip();
				}
			}
			else if (key == 4 || key == 15) {
				if (player_y + 20 < window_height - 80) {
					erasePlayerShip();
					player_y += 20;
					drawPlayerShip();
				}
			}
			else if (key == 8 || key == 16) {
				bullets_active[bullet_count] = true;
				bullets_x[bullet_count] = player_x + player_width + 2;
				bullets_y[bullet_count] = player_y + player_height / 2 - 2;
				bullet_count++;
			}
			else if (key == 7 || key == 11) {
				clear_screen = true;
				drawPauseMenu(0);
				handlePauseMenuOptions();
			}
			else if (key == 10) {
				exit(1);
			}
		}
		checkPlayerCollision(alien_x, alien_y, alien_active, alien_count);
		
		if (easyGame && scorenum == 500) {
			clearScreen();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			bosslevel();

			return;
		}
		else if (mediumGame && scorenum == 600) {
			clearScreen();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			boss2_active = true;
			boss3_active = false;
			boss1_y_range = 350;
			bosslevel();

			return;
		}
		else if (hardGame && scorenum == 700) {
			clearScreen();
			boss3_active = true;
			boss3_active = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			bosslevel();

			return;
		}

		if (player_lives <= 0) {
			drawGameOverPage(selectedGameOverOption);
			handleGameOverOptions();
			isRunning = false;
		}
		if (framecount % 2 == 0)
		{
			moveBullets(alien_x, alien_y, alien_active, alien_count);
		}
		if (framecount % 10 == 0) {
			
			moveAliens();
		}

		framecount++;
	}
}


void drawMenu(int selected) {
	static bool firstttimemenuclear = true;
	if (firstttimemenuclear) {
		clearScreen();
		firstttimemenuclear = false;
	}
	hideCursor();

	drawText(40, 150, 50, 255, 215, 0, "Space Invaders");

	if (selected == 0) {
		myRect(250, 200, 700, 300, 0, 255, 255);
		drawText(30, 300, 215, 0, 255, 255, "Start");
		myRect(250, 350, 700, 450, 255, 255, 255);
		drawText(30, 300, 375, 255, 255, 255, "High Score ");
		myRect(250, 500, 700, 600, 255, 255, 255);
		drawText(30, 300, 535, 255, 255, 255, "Quit");
	}
	else if (selected == 1) {
		myRect(250, 200, 700, 300, 255, 255, 255);
		drawText(30, 300, 215, 255, 255, 255, "Start");
		myRect(250, 350, 700, 450, 0, 255, 255);
		drawText(30, 300, 375, 0, 255, 255, "High Score ");
		myRect(250, 500, 700, 600, 255, 255, 255);
		drawText(30, 300, 535, 255, 255, 255, "Quit");
	}
	else if (selected == 2) {
		myRect(250, 200, 700, 300, 255, 255, 255);
		drawText(30, 300, 215, 255, 255, 255, "Start");
		myRect(250, 350, 700, 450, 255, 255, 255);
		drawText(30, 300, 375, 255, 255, 255, "High Score ");
		myRect(250, 500, 700, 600, 0, 255, 255);
		drawText(30, 300, 535, 0, 255, 255, "Quit");
	}
}

void drawGameMenu(int selected) {

	if (clear_screen) {
		clearScreen();
		clear_screen = false;
	}

	drawText(40, 300, 110, 255, 215, 0, "Load Menu");

	if (selected == 0) {
		myRect(300, 250, 600, 350, 0, 255, 255);
		drawText(30, 300, 270, 0, 255, 255, " New Game");
		myRect(300, 375, 600, 475, 255, 255, 255);
		drawText(30, 310, 395, 255, 255, 255, "Load Game");
		myRect(300, 500, 600, 600, 255, 255, 255);
		drawText(30, 400, 520, 255, 255, 255, "Back");
	}
	else if (selected == 1) {
		myRect(300, 250, 600, 350, 255, 255, 255);
		drawText(30, 300, 270, 255, 255, 255, " New Game");
		myRect(300, 375, 600, 475, 0, 255, 255);
		drawText(30, 310, 395, 0, 255, 255, "Load Game");
		myRect(300, 500, 600, 600, 255, 255, 255);
		drawText(30, 400, 520, 255, 255, 255, "Back");
	}
	else if (selected == 2) {
		myRect(300, 250, 600, 350, 255, 255, 255);
		drawText(30, 300, 270, 255, 255, 255, " New Game");
		myRect(300, 375, 600, 475, 255, 255, 255);
		drawText(30, 310, 395, 255, 255, 255, "Load Game");
		myRect(300, 500, 600, 600, 0, 255, 255);
		drawText(30, 400, 520, 0, 255, 255, "Back");
	}
}

void handleMenuInput() {
	int key;
	while (true) {
		if (isKeyPressed(key)) {
			if (key == 2) {
				selectedMenuOption = (selectedMenuOption - 1 + 3) % 3;
				drawMenu(selectedMenuOption);
			}
			else if (key == 4) {
				selectedMenuOption = (selectedMenuOption + 1) % 3;
				drawMenu(selectedMenuOption);
			}
			else if (key == 5) {
				if (selectedMenuOption == 0) {
					clearScreen();
					drawGameMenu(selectedGameOption);
					handleGameOptions();
					break;
				}
				else if (selectedMenuOption == 1) {
					clearScreen();
					drawScorePage();
					handlescorePage();
					break;
				}
				else if (selectedMenuOption == 2) {
					exit(1);
					break;
				}
			}
		}

	}
}
void handleGameOptions() {

	drawGameMenu(selectedGameOption);

	int key;
	while (true) {
		if (isKeyPressed(key)) {
			if (key == 2) {
				selectedGameOption = (selectedGameOption - 1 + 3) % 3;
				drawGameMenu(selectedGameOption);
			}
			else if (key == 4) {
				selectedGameOption = (selectedGameOption + 1) % 3;
				drawGameMenu(selectedGameOption);
			}
			else if (key == 5) {
				if (selectedGameOption == 0) {
					clearScreen();
					drawDifficultyMenu(selectedDiffultyOption);
					handleDiffultyOptions();
					break;
				}
				else if (selectedGameOption == 1) {
					gameloaded = true;
					clearScreen();
					detectAndLoadGame("savedgame.txt");


					if (easyGameLoaded) {
						easyGame = true;
						mediumGame = false;
						hardGame = false;

						playGame(alien_count_easy, easy_x, easy_y, easy_active);
					}
					else if (mediumGameLoaded) {
						mediumGame = true;
						easyGame = false;
						hardGame = false;

						playGame(alien_count_medium, medium_x, medium_y, medium_active);
					}
					else if (hardGameLoaded) {
						hardGame = true;
						easyGame = false;
						mediumGame = false;

						playGame(alien_count_hard, hard_x, hard_y, hard_active);
					}

					break;
				}
				else if (selectedGameOption == 2) {
					cout << "Going back to main menu..." << endl;
					clearScreen();
					drawMenu(selectedMenuOption);
					handleMenuInput();
					break;
				}
			}
		}
	}
}


void drawDifficultyMenu(int selected) {
	static bool firstTimefDiff = true;
	if (firstTimefDiff) {
		clearScreen();
		firstTimefDiff = false;
	}
	myRect(300, 100, 800, 200, 255, 215, 0);
	drawText(30, 350, 110, 255, 215, 0, "Diffulty level ");

	if (selected == 0) {
		myRect(400, 250, 700, 325, 0, 255, 255);
		drawText(25, 450, 260, 0, 255, 255, " Easy");
		myRect(400, 350, 700, 425, 255, 255, 255);
		drawText(30, 450, 360, 255, 255, 255, "Medium");
		myRect(400, 450, 700, 525, 255, 255, 255);
		drawText(30, 450, 460, 255, 255, 255, "Hard");
		myRect(400, 550, 700, 625, 255, 255, 255);
		drawText(30, 450, 560, 255, 255, 255, "Back");
	}
	else if (selected == 1) {
		myRect(400, 250, 700, 325, 255, 255, 255);
		drawText(25, 450, 260, 255, 255, 255, " Easy");
		myRect(400, 350, 700, 425, 0, 255, 255);
		drawText(30, 450, 360, 0, 255, 255, "Medium");
		myRect(400, 450, 700, 525, 255, 255, 255);
		drawText(30, 450, 460, 255, 255, 255, "Hard");
		myRect(400, 550, 700, 625, 255, 255, 255);
		drawText(30, 450, 560, 255, 255, 255, "Back");
	}
	else if (selected == 2) {
		myRect(400, 250, 700, 325, 255, 255, 255);
		drawText(25, 450, 260, 255, 255, 255, " Easy");
		myRect(400, 350, 700, 425, 255, 255, 255);
		drawText(30, 450, 360, 255, 255, 255, "Medium");
		myRect(400, 450, 700, 525, 0, 255, 255);
		drawText(30, 450, 460, 0, 255, 255, "Hard");
		myRect(400, 550, 700, 625, 255, 255, 255);
		drawText(30, 450, 560, 255, 255, 255, "Back");
	}
	else if (selected == 3) {
		myRect(400, 250, 700, 325, 255, 255, 255);
		drawText(25, 450, 260, 255, 255, 255, " Easy");
		myRect(400, 350, 700, 425, 255, 255, 255);
		drawText(30, 450, 360, 255, 255, 255, "Medium");
		myRect(400, 450, 700, 525, 255, 255, 255);
		drawText(30, 450, 460, 255, 255, 255, "Hard");
		myRect(400, 550, 700, 625, 0, 255, 255);
		drawText(30, 450, 560, 0, 255, 255, "Back");
	}
}

void handleDiffultyOptions() {
	drawDifficultyMenu(selectedDiffultyOption);
	int key;
	while (true) {
		if (isKeyPressed(key)) {
			if (key == 2) {
				selectedDiffultyOption = (selectedDiffultyOption - 1 + 4) % 4;
				drawDifficultyMenu(selectedDiffultyOption);
			}
			else if (key == 4) {
				selectedDiffultyOption = (selectedDiffultyOption + 1) % 4;
				drawDifficultyMenu(selectedDiffultyOption);
			}
			else if (key == 5) {
				if (selectedDiffultyOption == 0) {
					clearScreen();
					easyGame = true;
					mediumGame = false;
					hardGame = false;
					drawGame(alien_count_easy, easy_x, easy_y, easy_active);
					playGame(alien_count_easy, easy_x, easy_y, easy_active);
					break;  //Game starts on Easy
				}
				else if (selectedDiffultyOption == 1) {
					mediumGame = true;
					easyGame = false;
					hardGame = false;
					drawGame(alien_count_medium, medium_x, medium_y, medium_active);
					playGame(alien_count_medium, medium_x, medium_y, medium_active);
					break;  // Game starts on Medium
				}
				else if (selectedDiffultyOption == 2) {
					hardGame = true;
					easyGame = false;
					mediumGame = false;
					drawGame(alien_count_hard, hard_x, hard_y, hard_active);
					playGame(alien_count_hard, hard_x, hard_y, hard_active);
					break;  // Game starts on Hard
				}
				else if (selectedDiffultyOption == 3) {
					cout << "Going back to loading menu..." << endl;
					clearScreen();
					drawGameMenu(selectedGameOption);
					handleGameOptions();
					break;
				}
			}
		}
	}

}
void playBackgroundMusic() {
	PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
void drawGameOverPage(int selected) {
	static bool firstTimefGameOver = true;
	if (firstTimefGameOver) {
		clearScreen();
		firstTimefGameOver = false;
	}
	if (easyGame && scorenum == 550 || mediumGame && scorenum == 700 || hardGame && scorenum == 850) {
		drawText(40, 300, 50, 255, 215, 0, "You Won!");
	}
	else {
		drawText(40, 300, 50, 255, 215, 0, "Game Over");
		drawText(40, 300, 150, 255, 215, 0, "You Loose!");
	}
	drawText(30, 250, 300, 0, 255, 0, "Your Score: ");

	drawText(30, 600, 300, 255, 255, 255, "", true, scorenum);
	if (selected == 0) {
		myRect(300, 450, 650, 525, 0, 255, 255);
		drawText(30, 320, 460, 0, 255, 255, "Main Menu");
		myRect(300, 550, 650, 625, 255, 255, 255);
		drawText(30, 400, 560, 255, 255, 255, "Quit");

	}
	else if (selected == 1) {

		myRect(300, 450, 650, 525, 255, 255, 255);
		drawText(30, 320, 460, 255, 255, 255, "Main Menu");
		myRect(300, 550, 650, 625, 0, 255, 255);
		drawText(30, 400, 560, 0, 255, 255, "Quit");
	}
}

void playExplosionSound() {
	PlaySound(TEXT("explosion.wav"), NULL, SND_FILENAME | SND_ASYNC);

}

void handleGameOverOptions() {

	drawGameOverPage(selectedGameOverOption);
	updateHighScores(scorenum);
	saveHighScores("highscores.txt");
	int key;
	while (true) {
		if (isKeyPressed(key)) {
			if (key == 2) {
				selectedGameOverOption = (selectedGameOverOption == 1) ? 0 : 1;
				drawGameOverPage(selectedGameOverOption);
			}
			else if (key == 4) {
				selectedGameOverOption = (selectedGameOverOption == 1) ? 0 : 1;
				drawGameOverPage(selectedGameOverOption);
			}
			else if (key == 5) {
				if (selectedGameOverOption == 0) {
					clearScreen();
					initializeGlobalVariables();
					drawMenu(selectedMenuOption);
					handleMenuInput();
					return;
				}
				else if (selectedGameOverOption == 1) {
					exit(1);
					break;
				}
			}
		}
	}
}
void drawBossship(float x, float y) {
	myRect(x, y, x + 10, y + 50, 255, 0, 255, 255, 0, 255);
	myRect(x - 10, y - 10, x, y + 60, 255, 0, 255, 255, 0, 255);
	myRect(x - 20, y - 20, x - 10, y + 70, 255, 0, 255, 255, 0, 255);
	myRect(x - 30, y - 30, x - 20, y + 80, 255, 0, 255, 255, 0, 255);
	myRect(x - 40, y - 40, x - 30, y + 90, 255, 0, 255, 255, 0, 255);
	myRect(x - 50, y - 15, x - 40, y, 255, 0, 255, 255, 0, 255);
	myRect(x - 30, y - 15, x - 20, y - 5, 0, 0, 0, 0, 0, 0);
	myRect(x - 50, y + 50, x - 40, y + 65, 255, 0, 255, 255, 0, 255);
	myRect(x - 50, y + 15, x - 40, y + 35, 255, 0, 255, 255, 0, 255);
	myRect(x - 30, y + 50, x - 20, y + 60, 0, 0, 0, 0, 0, 0);
	myRect(x - 60, y - 10, x - 50, y - 5, 255, 0, 255, 255, 0, 255);
	myRect(x - 60, y + 55, x - 50, y + 60, 255, 0, 255, 255, 0, 255);
	myRect(x - 30, y + 10, x - 20, y + 20, 0, 0, 0, 0, 0, 0);
	myRect(x - 30, y + 30, x - 20, y + 40, 0, 0, 0, 0, 0, 0);
}

void eraseBossship(float x, float y) {
	myRect(x, y, x + 10, y + 50, 0, 0, 0, 0, 0, 0);
	myRect(x - 10, y - 10, x, y + 60, 0, 0, 0, 0, 0, 0);
	myRect(x - 20, y - 20, x - 10, y + 70, 0, 0, 0, 0, 0, 0);
	myRect(x - 30, y - 30, x - 20, y + 80, 0, 0, 0, 0, 0, 0);
	myRect(x - 40, y - 40, x - 30, y + 90, 0, 0, 0, 0, 0, 0);
	myRect(x - 50, y - 15, x - 40, y, 0, 0, 0, 0, 0, 0);
	myRect(x - 30, y - 15, x - 20, y - 5, 0, 0, 0, 0, 0, 0);
	myRect(x - 50, y + 50, x - 40, y + 65, 0, 0, 0, 0, 0, 0);
	myRect(x - 50, y + 15, x - 40, y + 35, 0, 0, 0, 0, 0, 0);
	myRect(x - 30, y + 50, x - 20, y + 60, 0, 0, 0, 0, 0, 0);
	myRect(x - 60, y - 10, x - 50, y - 5, 0, 0, 0, 0, 0, 0);
	myRect(x - 60, y + 55, x - 50, y + 60, 0, 0, 0, 0, 0, 0);
	myRect(x - 30, y + 10, x - 20, y + 20, 0, 0, 0, 0, 0, 0);
	myRect(x - 30, y + 30, x - 20, y + 40, 0, 0, 0, 0, 0, 0);
}

void moveBoss1() {
	if (boss1_active) {
		eraseBossship(boss1_x, boss1_y);

		boss1_x -= 20;
		boss1_y += boss1_y_direction * 15;

		if (boss1_x < 600) {
			boss1_x = 1200;
		}

		if (boss1_y <= 100 || boss1_y >= boss1_y_range) {
			boss1_y_direction *= -1;
		}
		drawBossship(boss1_x, boss1_y);

		if (!boss1_bullet1.active) {
			boss1_bullet1 = { boss1_x - 80, boss1_y , true };
		}
		if (!boss1_bullet2.active) {
			boss1_bullet2 = { boss1_x - 80, boss1_y + 55, true };
		}
	}
}
void moveBoss2() {
	if (boss2_active) {
		eraseBossship(boss2_x, boss2_y);

		boss2_x -= 20;
		boss2_y += boss2_y_direction * 15;

		if (boss2_x < 400) {
			boss2_x = 1200;
		}

		if (boss2_y <= 351 || boss2_y >= 550) {
			boss2_y_direction *= -1;
		}

		drawBossship(boss2_x, boss2_y);

		if (!boss2_bullet1.active) {
			boss2_bullet1 = { boss2_x - 80, boss2_y , true };
		}
		if (!boss2_bullet2.active) {
			boss2_bullet2 = { boss2_x - 80, boss2_y + 55, true };
		}
	}
}

void moveBoss3() {
	if (boss3_active) {
		eraseBossship(boss3_x, boss3_y);

		boss3_x -= 20;
		boss3_y += boss3_y_direction * 15;

		if (boss3_x < 600) {
			boss3_x = 1000;
		}

		if (boss3_y <= 300 || boss3_y >= 550) {
			boss3_y_direction *= -1;
		}
		drawBossship(boss3_x, boss3_y);

		if (!boss3_bullet1.active) {
			boss3_bullet1 = { boss3_x - 80, boss3_y , true };
		}
		if (!boss3_bullet2.active) {
			boss3_bullet2 = { boss3_x - 80, boss3_y + 55, true };
		}
	}
}
void bossBulletcollision() {
	if (boss1_bullet1.active &&
		player_x < boss1_bullet1.x + 20 && player_x + player_width > boss1_bullet1.x &&
		player_y < boss1_bullet1.y + 5 && player_y + player_height > boss1_bullet1.y) {
		player_lives--;
		drawLives();
		boss1_bullet1.active = false;
		boss1_bullet1.eraseBullet();
		erasePlayerShip();
		player_x = 50;
		player_y = 300;
		drawPlayerShip();
	}
	if (boss1_bullet2.active &&
		player_x < boss1_bullet2.x + 20 && player_x + player_width > boss1_bullet2.x &&
		player_y < boss1_bullet2.y + 5 && player_y + player_height > boss1_bullet2.y) {
		player_lives--;
		drawLives();
		boss1_bullet2.active = false;
		boss1_bullet2.eraseBullet();
		erasePlayerShip();
		player_x = 50;
		player_y = 300;
		drawPlayerShip();
	}
	if (boss2_bullet1.active &&
		player_x < boss2_bullet1.x + 20 && player_x + player_width > boss2_bullet1.x &&
		player_y < boss2_bullet1.y + 5 && player_y + player_width > boss2_bullet1.y) {
		player_lives--;
		drawLives();
		boss2_bullet1.active = false;
		boss2_bullet1.eraseBullet();
		erasePlayerShip();
		player_x = 50;
		player_y = 300;
		drawPlayerShip();
	}
	if (boss2_bullet2.active &&
		player_x < boss2_bullet2.x + 20 && player_x + player_width > boss2_bullet2.x &&
		player_y < boss2_bullet2.y + 5 && player_y + player_height > boss2_bullet2.y) {
		player_lives--;
		drawLives();
		boss2_bullet2.active = false;
		boss2_bullet2.eraseBullet();
		erasePlayerShip();
		player_x = 50;
		player_y = 300;
		drawPlayerShip();
	}
	if (boss3_bullet1.active &&
		player_x < boss3_bullet1.x + 20 && player_x + player_width > boss3_bullet1.x &&
		player_y < boss3_bullet1.y + 5 && player_y + player_height > boss3_bullet1.y) {
		player_lives--;
		drawLives();
		boss3_bullet1.active = false;
		boss3_bullet1.eraseBullet();
		erasePlayerShip();
		player_x = 50;
		player_y = 300;
		drawPlayerShip();
	}
	if (boss3_bullet2.active &&
		player_x < boss3_bullet2.x + 20 && player_x + player_width > boss3_bullet2.x &&
		player_y < boss3_bullet2.y + 5 && player_y + player_height > boss3_bullet2.y) {
		player_lives--;
		drawLives();
		boss3_bullet2.active = false;
		boss3_bullet2.eraseBullet();
		erasePlayerShip();
		player_x = 50;
		player_y = 300;
		drawPlayerShip();
	}
}

void playerSpaceshipBulletBossCollision() {
	for (int i = 0; i < max_bullet_count; ++i) {
		if (player_bullets[i].active) {
			if (boss1_active &&
				player_bullets[i].x < boss1_x + 60 && player_bullets[i].x + 20 > boss1_x - 50 &&
				player_bullets[i].y < boss1_y + 90 && player_bullets[i].y + 5 > boss1_y - 40) {
				boss1_hits++;
				player_bullets[i].active = false;
				player_bullets[i].eraseBullet();

				if (boss1_hits == 5) {
					boss1_active = false;
					drawText(20, 120, 15, 0, 0, 0, "", true, scorenum);
					scorenum += 50;
					drawText(20, 120, 15, 0, 255, 0, "", true, scorenum);
					eraseBossship(boss1_x, boss1_y);
					playExplosionSound();
				}
			}

			if (boss2_active &&
				player_bullets[i].x < boss2_x + 60 && player_bullets[i].x + 20 > boss2_x - 50 &&
				player_bullets[i].y < boss2_y + 90 && player_bullets[i].y + 5 > boss2_y - 40) {
				boss2_hits++;
				player_bullets[i].active = false;
				player_bullets[i].eraseBullet();

				if (boss2_hits == 5) {
					boss2_active = false;
					drawText(20, 120, 15, 0, 0, 0, "", true, scorenum);
					scorenum += 50;
					drawText(20, 120, 15, 0, 255, 0, "", true, scorenum);
					eraseBossship(boss2_x, boss2_y);
					playExplosionSound();
					
				}
			}
			if (boss3_active &&
				player_bullets[i].x < boss3_x + 60 && player_bullets[i].x + 20 > boss3_x - 50 &&
				player_bullets[i].y < boss3_y + 90 && player_bullets[i].y + 5 > boss3_y - 40) {
				boss3_hits++;
				player_bullets[i].active = false;
				player_bullets[i].eraseBullet();

				if (boss3_hits == 5) {
					boss3_active = false;
					drawText(20, 120, 15, 0, 0, 0, "", true, scorenum);
					scorenum += 50;
					drawText(20, 120, 15, 0, 255, 0, "", true, scorenum);
					eraseBossship(boss3_x, boss3_y);
					playExplosionSound();
					
				}
			}

		}
	}
}
void bosslevel() {
	playBossArrivalSound();
	myRect(2, 2, 1340, 690, 0, 255, 0);
	myLine(2, 60, 1340, 60, 0, 255, 0);

	drawText(20, 5, 15, 255, 255, 255, "Score : ");

	drawText(20, 120, 15, 0, 255, 0, "", true, scorenum);

	drawText(20, 1050, 15, 255, 255, 255, " Lives ");
	drawLives();

	if (easyGame) {
		drawText(20, 400, 15, 255, 255, 255, "Easy Mode");
	}
	else if (mediumGame) {
		drawText(20, 400, 15, 255, 255, 255, "Medium Mode");
	}
	else if (hardGame) {
		drawText(20, 400, 15, 255, 255, 255, "Hard Mode");
	}
	if (boss1_active) {
		drawBossship(boss1_x, boss1_y);
	}
	if (boss2_active) {
		drawBossship(boss2_x, boss2_y);
	}
	if (boss3_active) {
		drawBossship(boss3_x, boss3_y);
	}
	drawPlayerShip();
	int framecount = 0;

	auto last_time = std::chrono::steady_clock::now();

	while (true) {

		auto current_time = std::chrono::steady_clock::now();
		auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time);

		if (elapsed_time.count() >= 16) {
			last_time = current_time;

			if (player_lives == 0) {
				clearScreen();
				drawGameOverPage(selectedGameOverOption);
				handleGameOverOptions();
				break;
			}
			int key = -1;
			if (isKeyPressed(key)) {
				if (key == 1) {
					erasePlayerShip();
					player_x += -20;
					drawPlayerShip();
				}
				else if (key == 2) {
					erasePlayerShip();
					player_y += -20;
					drawPlayerShip();
				}
				else if (key == 3) {
					erasePlayerShip();
					player_x += 20;
					drawPlayerShip();
				}
				else if (key == 4) {
					erasePlayerShip();
					player_y += 20;
					drawPlayerShip();
				}
				else if (key == 8) {

					player_bullets[bullet_count] = { player_x + player_width +2, player_y + player_height / 2, true };
					bullet_count++;
				}
			}

			for (int i = 0; i < bullet_count; i++) {
				player_bullets[i].moveBullet();
			}

			boss1_bullet1.moveBullet();
			boss1_bullet2.moveBullet();
			boss2_bullet1.moveBullet();
			boss2_bullet2.moveBullet();
			boss3_bullet1.moveBullet();
			boss3_bullet2.moveBullet();

			bossBulletcollision();
			playerSpaceshipBulletBossCollision();

			if (framecount % 40 == 0) {
				moveBoss1();
				moveBoss2();
				moveBoss3();
			}
			if (scorenum == 550 && easyGame) {
				std::this_thread::sleep_for(std::chrono::seconds(1));
				drawGameOverPage(selectedGameOverOption);
				handleGameOverOptions();
			}
			if (scorenum == 700 && mediumGame) {
				std::this_thread::sleep_for(std::chrono::seconds(1));
				drawGameOverPage(selectedGameOverOption);
				handleGameOverOptions();
			}
			if (scorenum == 850 && hardGame) {
				std::this_thread::sleep_for(std::chrono::seconds(1));
				drawGameOverPage(selectedGameOverOption);
				handleGameOverOptions();
			}
			framecount++;
		}
		std::this_thread::yield();
	}
}
void drawPlayerShip() {
	myRect(player_x, player_y, player_x + blockwidth * 6, player_y + blockheight, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 6, player_y, player_x + blockwidth * 7, player_y + blockheight, 255, 0, 0, 255, 0, 0);
	myRect(player_x + blockwidth * 1, player_y + blockheight, player_x + blockwidth * 4, player_y + blockheight * 2, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 3, player_y + blockheight * 4, player_x + blockwidth * 7, player_y + blockheight * 5, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 3, player_y + blockheight * 3, player_x + blockwidth * 6, player_y + blockheight * 4, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 3, player_y + blockheight * 5, player_x + blockwidth * 8, player_y + blockheight * 6, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 5, player_y + blockheight * 5, player_x + blockwidth * 6, player_y + blockheight * 6, 255, 0, 0, 255, 0, 0);
	myRect(player_x, player_y + blockheight * 6, player_x + blockwidth * 10, player_y + blockheight * 7, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 6, player_y + blockheight * 6, player_x + blockwidth * 7, player_y + blockheight * 7, 255, 0, 0, 255, 0, 0);
	myRect(player_x + blockwidth * 5, player_y + blockheight * 7, player_x + blockwidth * 6, player_y + blockheight * 8, 255, 0, 0, 255, 0, 0);
	myRect(player_x + blockwidth * 3, player_y + blockheight * 7, player_x + blockwidth * 8, player_y + blockheight * 8, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 5, player_y + blockheight * 7, player_x + blockwidth * 6, player_y + blockheight * 8, 255, 0, 0, 255, 0, 0);
	myRect(player_x + blockwidth * 5, player_y + blockheight * 2, player_x + blockwidth * 6, player_y + blockheight * 3, 0, 0, 255, 0, 0, 255);
	myRect(player_x + blockwidth * 6, player_y + blockheight * 2, player_x + blockwidth * 7, player_y + blockheight * 3, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 7, player_y + blockheight * 2, player_x + blockwidth * 8, player_y + blockheight * 3, 255, 0, 0, 255, 0, 0);
	myRect(player_x + blockwidth * 3, player_y + blockheight * 8, player_x + blockwidth * 7, player_y + blockheight * 9, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 3, player_y + blockheight * 9, player_x + blockwidth * 6, player_y + blockheight * 10, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 2, player_y + blockheight * 2, player_x + blockwidth * 5, player_y + blockheight * 10, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 2, player_y + blockheight * 10, player_x + blockwidth * 7, player_y + blockheight * 11, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 7, player_y + blockheight * 10, player_x + blockwidth * 8, player_y + blockheight * 11, 255, 0, 0, 255, 0, 0);
	myRect(player_x + blockwidth * 6, player_y + blockheight * 10, player_x + blockwidth * 7, player_y + blockheight * 11, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 5, player_y + blockheight * 10, player_x + blockwidth * 6, player_y + blockheight * 11, 0, 0, 255, 0, 0, 255);
	myRect(player_x + blockwidth * 1, player_y + blockheight * 11, player_x + blockwidth * 4, player_y + blockheight * 12, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 6, player_y + blockheight * 12, player_x + blockwidth * 7, player_y + blockheight * 13, 255, 0, 0, 255, 0, 0);
	myRect(player_x, player_y + blockheight * 12, player_x + blockwidth * 6, player_y + blockheight * 13, 255, 255, 255, 255, 255, 255);
	myRect(player_x + blockwidth * 1, player_y + blockheight * 4, player_x + blockwidth * 3, player_y + blockheight * 5, 255, 0, 0, 255, 0, 0);
	myRect(player_x + blockwidth * 1, player_y + blockheight * 3, player_x + blockwidth * 2, player_y + blockheight * 4, 255, 0, 0, 255, 0, 0);
	myRect(player_x + blockwidth * 1, player_y + blockheight * 8, player_x + blockheight * 3, player_y + blockheight * 9, 255, 0, 0, 255, 0, 0);
	myRect(player_x + blockwidth * 1, player_y + blockheight * 9, player_x + blockwidth * 2, player_y + blockheight * 10, 255, 0, 0, 255, 0, 0);
}

void erasePlayerShip() {
	myRect(player_x, player_y, player_x + blockwidth * 6, player_y + 8, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 6, player_y, player_x + blockwidth * 7, player_y + 8, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 1, player_y + 8, player_x + blockwidth * 4, player_y + blockheight * 2, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 3, player_y + blockheight * 4, player_x + blockwidth * 7, player_y + blockheight * 5, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 3, player_y + blockheight * 3, player_x + blockwidth * 6, player_y + blockheight * 4, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 3, player_y + blockheight * 5, player_x + blockwidth * 8, player_y + blockheight * 6, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 5, player_y + blockheight * 5, player_x + blockwidth * 6, player_y + blockheight * 6, 0, 0, 0, 0, 0, 0);
	myRect(player_x, player_y + blockheight * 6, player_x + blockwidth * 10, player_y + blockheight * 7, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 6, player_y + blockheight * 6, player_x + blockwidth * 7, player_y + blockheight * 7, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 5, player_y + blockheight * 7, player_x + blockwidth * 6, player_y + blockheight * 8, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 3, player_y + blockheight * 7, player_x + blockwidth * 8, player_y + blockheight * 8, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 5, player_y + blockheight * 7, player_x + blockwidth * 6, player_y + blockheight * 8, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 5, player_y + blockheight * 2, player_x + blockwidth * 6, player_y + blockheight * 3, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 6, player_y + blockheight * 2, player_x + blockwidth * 7, player_y + blockheight * 3, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 7, player_y + blockheight * 2, player_x + blockwidth * 8, player_y + blockheight * 3, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 3, player_y + blockheight * 8, player_x + blockwidth * 7, player_y + blockheight * 9, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 3, player_y + blockheight * 9, player_x + blockwidth * 6, player_y + blockheight * 10, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 2, player_y + blockheight * 2, player_x + blockwidth * 5, player_y + blockheight * 10, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 2, player_y + blockheight * 10, player_x + blockwidth * 7, player_y + blockheight * 11, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 7, player_y + blockheight * 10, player_x + blockwidth * 8, player_y + blockheight * 11, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 6, player_y + blockheight * 10, player_x + blockwidth * 7, player_y + blockheight * 11, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 5, player_y + blockheight * 10, player_x + blockwidth * 6, player_y + blockheight * 11, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 1, player_y + blockheight * 11, player_x + blockwidth * 4, player_y + blockheight * 12, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 6, player_y + blockheight * 12, player_x + blockwidth * 7, player_y + blockheight * 13, 0, 0, 0, 0, 0, 0);
	myRect(player_x, player_y + blockheight * 12, player_x + blockwidth * 6, player_y + blockheight * 13, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 1, player_y + blockheight * 4, player_x + blockheight * 3, player_y + blockheight * 5, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 1, player_y + blockheight * 3, player_x + blockwidth * 2, player_y + blockheight * 4, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 1, player_y + blockheight * 8, player_x + blockheight * 3, player_y + blockheight * 9, 0, 0, 0, 0, 0, 0);
	myRect(player_x + blockwidth * 1, player_y + blockheight * 9, player_x + blockwidth * 2, player_y + blockheight * 10, 0, 0, 0, 0, 0, 0);
}