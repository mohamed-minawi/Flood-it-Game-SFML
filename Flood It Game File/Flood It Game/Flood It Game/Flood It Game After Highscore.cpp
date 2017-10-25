#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace sf;
using namespace std;

struct id
{
	string name;
	float ratio;
	int win;
	int loss;

};


vector<id> highscore;
id use;
string e1 = "", str = "";
float e2;
int e3, e4;
int vectorpos;


// Declare Game Constants
#define gameheight 600 
#define gamewidth 600
#define tileheight 20
#define tilewidth 20
#define buttonwidth gameheight/3
#define buttonheight gameheight/12
#define maxcolor 8

const int playheight = 2 * gamewidth / 3;
const int playwidth = playheight;
int starty = 19;
int startx = 0;
int startposx = gamewidth / 15;
int startposy = gameheight / 15;
const int height = (playheight) / tileheight;
const int width = (playwidth) / tilewidth;
int colorposx = gamewidth / 15;
const int colorposy = gameheight / 1.2;
//counts declare
int moves = 30;
int wincount = 0, losecount = 0;
int numofcolor = 5;
int mariocount = 0;
//Texts declare
string t1, t2, t3, t4;
Font font, font2, font3, font4;
Text numofmoves, start, title, settingtxt, restart, difficulty, soundsetting, help, back, instructions, easy, medium, hard, txthelp, entername, inputname, ratio, wins, losses, statstext, name, on, off, declare;;
//Grid declare
RectangleShape grid[tileheight][tilewidth];
RectangleShape color[maxcolor];
//Sprite Declare
Sprite sprite, startbut, settingbut, movebut, newbut, helpbut, backbut, difficibut[3], mario, statsbut, soundbut[2];

//Bool functions for pages
bool gamestart = false, settings = false, home = true, instructionpg = false, namepg = false, statspage = false, gameend = false, soundplay = true;
//Declarartion of textures
IntRect blue(32, 0, 30, 30);
IntRect red(192, 0, 30, 30);
IntRect green(320, 32, 30, 30);
IntRect yellow(256, 64, 30, 30);
IntRect orange(192, 32, 30, 30);
IntRect pink(160, 64, 30, 30);
IntRect violet(128, 0, 30, 30);
IntRect grey(0, 0, 30, 30);


IntRect col[8] = { blue, red, green, yellow, orange, pink, violet, grey };

IntRect Butt(32, 191, buttonwidth, buttonheight);
IntRect DiffBut(32, 191, buttonheight, buttonheight);
IntRect old;
IntRect update;

Event event;
Vector2i pos;
//Prototypes 
void fillcolor(int, int);
IntRect rand_col(int);
bool endgame(RectangleShape[][20], int, int, IntRect);
void gridcreator(Texture&, int);
void txtdeclare();
void spritedeclare(Texture&, Texture&, Texture &);
int loader(Texture&, Texture&, Texture&, Texture&, Sound&, SoundBuffer&, Music&);
void undergamecolorgenerator(Texture&);
void newmap(Texture&);
void setanimation();

void copy();
void sort();
int find(string);
void copytofile();
float roundvalue(float, int);
void display();


int main()
{
	RenderWindow window(VideoMode(gamewidth, gameheight), "FloodIt Minawi Youssef Yasser");
	Texture texture, background, button, marioimg;
	//sound declare
	Sound intro;
	SoundBuffer beginsound;
	Music gamesound;
	//functions and passing parameters
	srand(time(NULL));
	loader(texture, background, button, marioimg, intro, beginsound, gamesound);
	spritedeclare(background, button, marioimg);
	txtdeclare();
	gridcreator(texture, numofcolor);

	old = grid[starty][startx].getTextureRect();
	update = old;
	Clock clock, gameclock;

	copy();
	int count = 0;
	while (window.isOpen())
	{
		if (soundplay & count == 0)
		{
			intro.play();
			count++;
		}


		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed || event.key.code == Keyboard::Escape)
				window.close();
			if (!gamestart &&!namepg&& !settings&&!instructionpg&&!statspage && event.key.code == Keyboard::Space)
			{
				namepg = true;
				newmap(texture);
			}
			if (!namepg &&!instructionpg)
				if (event.key.code == Keyboard::S)
				{
					settings = true;

					/*if (gamestart && soundplay)
					gamesound.pause();*/
				}
			if (!namepg  &&!settings)
				if (event.key.code == Keyboard::H)
				{
					instructionpg = true;
				}

			//back 
			if ((settings || instructionpg || statspage) && event.key.code == Keyboard::B)
			{
				if (settings&!statspage)
					settings = false;
				else if (statspage)
				{
					statspage = false;
				}
				else instructionpg = false;

			}

			//namepage settings
			if (namepg)
			{
				//Input name code
				if (event.type == sf::Event::TextEntered)
				{
					if (event.text.unicode == 8 && str.size() != 0)
					{
						str.erase(str.size() - 1);
						inputname.setString(str);
					}
					else if (event.text.unicode < 128 && event.text.unicode>65)
					{
						str += static_cast<char>(event.text.unicode);
						inputname.setString(str);

						if (str.length() > 0)
							inputname.move(-6.5, 0);
					}


					if (Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && str.length() > 0)
						inputname.move(6.5, 0);

					if (Keyboard::isKeyPressed(sf::Keyboard::Return))
					{
						namepg = false;
						gamestart = true;
						newmap(texture);
						vectorpos = find(str);
						//addjusting stats
						if (vectorpos <= highscore.size() - 1)
						{
							wincount = highscore[vectorpos].win;
							losecount = highscore[vectorpos].loss;

						}
						else
						{
							wincount = 0;
							losecount = 0;
							use.name = str;
							use.ratio = 0;
							use.win = 0;
							use.loss = 0;
							highscore.push_back(use);

						}

						//Sound when playing
						intro.stop();
						gamesound.play();


					}


				}

			}
		}


		//Mouse press for buttons
		if (event.type == Event::MouseButtonPressed)
		{
			pos = Mouse::getPosition(window);

			if (home &&!gamestart &&!namepg&& !settings&&!instructionpg&&!statspage)
			{
				if ((pos.x >= startbut.getPosition().x - buttonwidth / 2) && (pos.x <= startbut.getPosition().x + buttonwidth / 2)
					&& (pos.y >= startbut.getPosition().y - buttonheight / 2) && (pos.y <= startbut.getPosition().y + buttonheight / 2))
				{
					namepg = true;

					newmap(texture);
				}

				if ((pos.x >= settingbut.getPosition().x - buttonwidth / 2) && (pos.x <= settingbut.getPosition().x + buttonwidth / 2)
					&& (pos.y >= settingbut.getPosition().y - buttonheight / 2) && (pos.y <= settingbut.getPosition().y + buttonheight / 2))
				{
					settings = true;

				}

				if ((pos.x >= helpbut.getPosition().x - buttonwidth / 2) && (pos.x <= helpbut.getPosition().x + buttonwidth / 2)
					&& (pos.y >= helpbut.getPosition().y - buttonheight / 2) && (pos.y <= helpbut.getPosition().y + buttonheight / 2))
				{
					instructionpg = true;
				}

			}
			if (settings)
			{
				if (pos.x >= backbut.getPosition().x - gamewidth / 5 && pos.x <= backbut.getPosition().x + gamewidth / 5
					&& pos.y >= backbut.getPosition().y - buttonheight && pos.y <= backbut.getPosition().y + buttonheight)

				{
					settings = false;

					pos = Vector2i(600, 600);
					gameclock.restart().asSeconds();
				}
				if (pos.x >= difficibut[0].getPosition().x &&pos.x <= difficibut[0].getPosition().x + buttonheight && pos.y >= difficibut[0].getPosition().y && pos.y <= difficibut[0].getPosition().y + buttonheight)
				{
					numofcolor = 3;
					moves = 22;
					easy.setColor(Color::Black);
					medium.setColor(Color::Red);
					hard.setColor(Color::Red);

					newmap(texture);
				}
				if (pos.x >= difficibut[1].getPosition().x &&pos.x <= difficibut[1].getPosition().x + buttonheight && pos.y >= difficibut[1].getPosition().y && pos.y <= difficibut[1].getPosition().y + buttonheight)
				{
					numofcolor = 5;
					moves = 30;
					easy.setColor(Color::Red);
					medium.setColor(Color::Black);
					hard.setColor(Color::Red);
					newmap(texture);
				}
				if (pos.x >= difficibut[2].getPosition().x &&pos.x <= difficibut[2].getPosition().x + buttonheight && pos.y >= difficibut[2].getPosition().y && pos.y <= difficibut[2].getPosition().y + buttonheight)
				{
					numofcolor = 8;
					moves = 40;
					easy.setColor(Color::Red);
					medium.setColor(Color::Red);
					hard.setColor(Color::Black);
					newmap(texture);
				}
				if (pos.x >= statsbut.getPosition().x - buttonwidth / 2 && pos.x <= statsbut.getPosition().x + buttonwidth / 2
					&& pos.y >= statsbut.getPosition().y - buttonheight / 2 && pos.y <= statsbut.getPosition().y + buttonheight / 2)
				{
					statspage = true;
				}
				if (pos.x >= soundbut[0].getPosition().x &&pos.x <= soundbut[0].getPosition().x + buttonheight && pos.y >= soundbut[0].getPosition().y && pos.y <= soundbut[0].getPosition().y + buttonheight)
				{
					soundplay = true;
					on.setColor(Color::Black);
					off.setColor(Color::Red);
					if (gamestart)
						gamesound.play();
					else intro.play();
				}
				if (pos.x >= soundbut[1].getPosition().x &&pos.x <= soundbut[1].getPosition().x + buttonheight && pos.y >= soundbut[1].getPosition().y && pos.y <= soundbut[1].getPosition().y + buttonheight)
				{
					soundplay = false;
					on.setColor(Color::Red);
					off.setColor(Color::Black);
					if (gamestart)
						gamesound.stop();
					else intro.stop();
				}
			}
			if (instructionpg)
				if (pos.x >= backbut.getPosition().x - gamewidth / 5 && pos.x <= backbut.getPosition().x + gamewidth / 5
					&& pos.y >= backbut.getPosition().y - buttonheight && pos.y <= backbut.getPosition().y + buttonheight)
				{
					instructionpg = false;

					pos = Vector2i(600, 600);
					gameclock.restart().asSeconds();

				}
			if (statspage)
				if (pos.x >= backbut.getPosition().x - gamewidth / 5 && pos.x <= backbut.getPosition().x + gamewidth / 5
					&& pos.y >= backbut.getPosition().y - buttonheight && pos.y <= backbut.getPosition().y + buttonheight)
				{
					statspage = false;
					pos = Vector2i(600, 600);
					gameclock.restart().asSeconds();
				}

		}
		//after game ends win/loss
		if (endgame(grid, startx, starty, old) && !gameend)
		{

			declare.setString("You Win\n\n Press newgame to \n play again");
			declare.Bold;
			declare.Underlined;
			wincount++;
			gameend = true;

			e3 = wincount;
			e4 = losecount;
			highscore[vectorpos].win = e3;
			highscore[vectorpos].loss = e4;

			if (losecount == 0)
				losecount = 1;
			e2 = roundvalue((float(wincount) / losecount), 2);
			highscore[vectorpos].ratio = e2;

		}
		if (moves == 0 && !gameend)
		{
			declare.setString("You lost\n \nPress newgame to \n play again");
			declare.Bold;
			declare.Underlined;

			numofcolor = 1;
			newmap(texture);
			losecount++;

			gameend = true;

			e3 = wincount;
			e4 = losecount;
			highscore[vectorpos].win = e3;
			highscore[vectorpos].loss = e4;
			if (losecount == 0)
				losecount = 1;
			e2 = roundvalue((float(wincount) / losecount), 2);
			highscore[vectorpos].ratio = e2;

		}

		if (settings == false && gamestart == true)
		{
			if (clock.getElapsedTime() > seconds(.1))
			{



				clock.restart().asSeconds();

				if (gameclock.getElapsedTime() > seconds(.15))
				{
					pos = Vector2i(600, 600);

					if (event.type == Event::MouseButtonPressed)
					{
						pos = Mouse::getPosition(window);
					}
					gameclock.restart().asSeconds();
					//new game procedure
					if (pos.x >= newbut.getPosition().x && pos.x <= newbut.getPosition().x + buttonwidth
						&& pos.y >= newbut.getPosition().y && pos.y <= newbut.getPosition().y + buttonheight)
					{
						if (numofcolor < 4) //easy
							moves = 22;
						else if (numofcolor < 8)//hard
							moves = 30;
						else moves = 40;
						//blackening of preseed difficulty
						if (medium.getColor() == Color::Black)
							numofcolor = 5;
						else if (easy.getColor() == Color::Black)
							numofcolor = 3;
						else if (hard.getColor() == Color::Black)
							numofcolor = 8;
						newmap(texture);
						gameend = false;
						declare.setString("");
					}
					//scoreboard
					if (!settings && !instructionpg && !home && pos.x >= backbut.getPosition().x - gamewidth / 5 && pos.x <= backbut.getPosition().x + gamewidth / 5
						&& pos.y >= backbut.getPosition().y - buttonheight && pos.y <= backbut.getPosition().y + buttonheight)
					{
						e3 = wincount;
						e4 = losecount;
						highscore[vectorpos].win = e3;
						highscore[vectorpos].loss = e4;

						if (losecount == 0)
							losecount = 1;
						e2 = roundvalue((float(wincount) / losecount), 2);
						highscore[vectorpos].ratio = e2;


						home = true;
						gamestart = false;

						count = 0;
						gamesound.stop();
						declare.setString("");

					}
				}
				if (!gameend)
				{
					//mouse position 
					if (event.type == Event::MouseButtonPressed)
					{
						pos = Mouse::getPosition(window);
					}
					//Game cheat 
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
						moves += 5;
					//Colors and Updating the Grid using the fill color function
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) || pos.x >= color[0].getPosition().x&& pos.x <= color[0].getPosition().x + 2 * width && pos.y >= color[0].getPosition().y && pos.y <= color[0].getPosition().y + 2 * height)
					{
						update = blue;
						fillcolor(starty, startx);
						if (update != old)
							moves--;
						old = update;

					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) || pos.x >= color[1].getPosition().x&& pos.x <= color[1].getPosition().x + 2 * width && pos.y >= color[1].getPosition().y && pos.y <= color[1].getPosition().y + 2 * height)
					{
						update = red;
						fillcolor(starty, startx);
						if (update != old)
							moves--;
						old = update;

					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::G) || pos.x >= color[2].getPosition().x&& pos.x <= color[2].getPosition().x + 2 * width && pos.y >= color[2].getPosition().y && pos.y <= color[2].getPosition().y + 2 * height)
					{
						update = green;
						fillcolor(starty, startx);
						if (update != old)
							moves--;
						old = update;
					}
					if (numofcolor > 3 && sf::Keyboard::isKeyPressed(sf::Keyboard::Y) || pos.x >= color[3].getPosition().x&& pos.x <= color[3].getPosition().x + 2 * width && pos.y >= color[3].getPosition().y && pos.y <= color[3].getPosition().y + 2 * height)
					{
						update = yellow;
						fillcolor(starty, startx);
						if (update != old)
							moves--;
						old = update;
					}
					if (numofcolor > 3 && sf::Keyboard::isKeyPressed(sf::Keyboard::O) || pos.x >= color[4].getPosition().x&& pos.x <= color[4].getPosition().x + 2 * width && pos.y >= color[4].getPosition().y && pos.y <= color[4].getPosition().y + 2 * height)
					{
						update = orange;
						fillcolor(starty, startx);
						if (update != old)
							moves--;
						old = update;
					}
					if (numofcolor > 5 && sf::Keyboard::isKeyPressed(sf::Keyboard::P) || pos.x >= color[5].getPosition().x&& pos.x <= color[5].getPosition().x + 2 * width && pos.y >= color[5].getPosition().y && pos.y <= color[5].getPosition().y + 2 * height)
					{
						update = pink;
						fillcolor(starty, startx);
						if (update != old)
							moves--;
						old = update;
					}

					if (numofcolor > 5 && sf::Keyboard::isKeyPressed(sf::Keyboard::V) || pos.x >= color[6].getPosition().x&& pos.x <= color[6].getPosition().x + 2 * width && pos.y >= color[6].getPosition().y && pos.y <= color[6].getPosition().y + 2 * height)
					{
						update = violet;
						fillcolor(starty, startx);
						if (update != old)
							moves--;
						old = update;
					}
					if (numofcolor > 5 && sf::Keyboard::isKeyPressed(sf::Keyboard::A) || pos.x >= color[7].getPosition().x&& pos.x <= color[7].getPosition().x + 2 * width && pos.y >= color[7].getPosition().y && pos.y <= color[7].getPosition().y + 2 * height)
					{
						update = grey;
						fillcolor(starty, startx);
						if (update != old)
							moves--;
						old = update;
					}
					//Repositioning the mouse
					pos = Vector2i(600, 600);
				}

			}
		}

		numofmoves.setString(to_string(moves) + " moves");

		window.clear();

		window.draw(sprite);

		//Playing window
		if (home == false && settings == false && gamestart == true && instructionpg == false && namepg == false)
		{
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++)
					window.draw(grid[i][j]);

			for (int i = 0; i < numofcolor; i++)
				window.draw(color[i]);

			window.draw(movebut);
			window.draw(newbut);
			window.draw(backbut);
			window.draw(back);
			window.draw(numofmoves);
			window.draw(restart);
			window.draw(declare);
		}
		//namepage items
		if (namepg)
		{
			window.draw(entername);
			window.draw(inputname);


			home = false;
		}
		//Home window items
		if (home == true && settings == false && gamestart == false && instructionpg == false)
		{

			window.draw(startbut);
			window.draw(settingbut);
			window.draw(helpbut);
			window.draw(settingtxt);
			window.draw(title);
			window.draw(start);
			window.draw(help);

			window.draw(mario);

			if (clock.getElapsedTime() > seconds(.15))
			{
				setanimation();
				clock.restart().asSeconds();
			}
		}
		//Settings window items
		if (settings == true && !statspage)
		{
			window.draw(title);
			window.draw(difficulty);
			window.draw(soundsetting);
			for (int i = 0; i < 3; i++)
			{
				window.draw(difficibut[i]);
			}
			window.draw(backbut);
			window.draw(back);
			window.draw(easy);
			window.draw(medium);
			window.draw(hard);
			window.draw(statsbut);
			window.draw(statstext);
			for (int i = 0; i < 2; i++)
			{
				window.draw(soundbut[i]);
			}
			window.draw(on);
			window.draw(off);
		}
		//Help window items
		if (instructionpg)
		{
			window.draw(title);
			window.draw(txthelp);
			window.draw(backbut);
			window.draw(back);
		}
		//Scoreboard items
		if (statspage)
		{
			window.draw(losses);
			window.draw(name);
			window.draw(wins);
			window.draw(ratio);
			window.draw(backbut);
			window.draw(back);
			display();

		}
		window.display();
	}


	copytofile();

	return EXIT_SUCCESS;
}
//Recursive function of flood fill
void fillcolor(int x, int y)

{
	if (x >= 0 && x < tileheight && y >= 0 && y<tilewidth && grid[x][y].getTextureRect() == old &&grid[x][y].getTextureRect() != update)
	{
		grid[x][y].setTextureRect(update);

		fillcolor(x + 1, y);
		fillcolor(x - 1, y);
		fillcolor(x, y + 1);
		fillcolor(x, y - 1);
	}

}
//function that returns a random position of color from the texture
IntRect rand_col(int num)
{
	IntRect c;
	int x = rand() % num;
	switch (x)
	{
	case 0: c = blue; break;
	case 1: c = red; break;
	case 2: c = green; break;
	case 3: c = yellow; break;
	case 4: c = orange; break;
	case 5: c = pink; break;
	case 6: c = violet; break;
	case 7: c = grey; break;

	}

	return c;
}
//end game function
bool endgame(RectangleShape A[][20], int row, int col, IntRect v)
{
	if (col<20 && row<20 && A[row][col].getTextureRect() != v)
		return false;
	else if (col < 20)
	{
		endgame(A, row, col + 1, v);
	}
	else if (row <= 19)
	{
		endgame(A, row + 1, 0, v);
	}
	else return true;
}
//Grid Creation
void gridcreator(Texture &texture, int num)
{
	IntRect o;
	for (int i = 0; i < tileheight; i++)
	{
		if (i>0)
			startposy += height;
		for (int j = 0; j < tilewidth; j++)
		{
			o = rand_col(num);
			grid[i][j].setPosition(startposx, startposy);
			grid[i][j].setSize(Vector2f(width - 5, height - 5));
			startposx += width;
			grid[i][j].setTexture(&texture);
			grid[i][j].setTextureRect(o);
		}
		startposx = gamewidth / 15;
	}
}
//Declaration and settings of Texts
void txtdeclare()
{

	numofmoves.setFont(font2);
	numofmoves.setCharacterSize(20);
	numofmoves.setPosition(movebut.getPosition().x + (gamewidth / 100), movebut.getPosition().y + gameheight / 50);
	numofmoves.setColor(Color::Red);
	numofmoves.setString("");

	restart.setFont(font2);
	restart.setCharacterSize(21);
	restart.setPosition(newbut.getPosition().x + gameheight / 100, newbut.getPosition().y + gameheight / 50);
	restart.setColor(Color::Red);
	restart.setString("New Game");

	start.setFont(font2);
	start.setCharacterSize(40);
	start.setPosition(startbut.getPosition().x - buttonwidth / 4, startbut.getPosition().y - 24 * buttonheight / 45);
	start.setColor(Color::Red);
	start.setString("Play");

	settingtxt.setFont(font2);
	settingtxt.setCharacterSize(35);
	settingtxt.setPosition(settingbut.getPosition().x - buttonwidth * 11 / 24, settingbut.getPosition().y - 24 * buttonheight / 50);
	settingtxt.setColor(Color::Red);
	settingtxt.setString("Settings");

	title.setFont(font2);
	title.setCharacterSize(80);
	title.setPosition(gamewidth / 7, gameheight / 30);
	title.setColor(Color::Red);
	title.setString("Flood It");

	difficulty.setFont(font2);
	difficulty.setCharacterSize(30);
	difficulty.setPosition(gamewidth / 20, gameheight / 3);
	difficulty.setColor(Color::Magenta);
	difficulty.setString("Difficulty");

	soundsetting.setFont(font2);
	soundsetting.setCharacterSize(30);
	soundsetting.setPosition(gamewidth / 20, gameheight / 1.7);
	soundsetting.setColor(Color::Magenta);
	soundsetting.setString("Sound");

	help.setFont(font2);
	help.setCharacterSize(35);
	help.setPosition(startbut.getPosition().x - buttonwidth * 5 / 24, startbut.getPosition().y + 49 * buttonheight / 50);
	help.setColor(Color::Red);
	help.setString("Help");

	back.setFont(font2);
	back.setCharacterSize(30);
	back.setPosition(backbut.getPosition().x + buttonwidth * 1 / 12, backbut.getPosition().y);
	back.setColor(Color::White);
	back.setString("Back");

	easy.setFont(font2);
	easy.setCharacterSize(35);
	easy.setPosition(difficibut[0].getPosition().x + buttonwidth / 16, difficibut[0].getPosition().y);
	easy.setColor(Color::Red);
	easy.setString("E");

	medium.setFont(font2);
	medium.setCharacterSize(35);
	medium.setPosition(difficibut[1].getPosition().x + buttonwidth / 16, difficibut[1].getPosition().y);
	medium.setColor(Color::Black);
	medium.setString("M");

	hard.setFont(font2);
	hard.setCharacterSize(35);
	hard.setPosition(difficibut[2].getPosition().x + buttonwidth / 16, difficibut[2].getPosition().y);
	hard.setColor(Color::Red);
	hard.setString("H");

	txthelp.setFont(font);
	txthelp.setColor(Color::Magenta);
	txthelp.setPosition(gamewidth / 20, gameheight / 4);
	txthelp.setString("Welcome to Flood It game \nPress B for Blue\nPress R for Red\nPress G for Green\nPress Y for Yellow\nPress O for Orange\nPress P for Pink\nPress V for Violet\nPress A for Ash(Grey)\nOr Click the color box\nPress S for settings\nPress B (except during game) \nfor back\nPress H for Help");
	txthelp.setScale(Vector2f(1, 1));

	entername.setFont(font2);
	entername.setCharacterSize(30);
	entername.setPosition(gamewidth / 6, gameheight / 30);
	entername.setColor(Color::Red);
	entername.setString("Enter your first name:");

	inputname.setFont(font2);
	inputname.setCharacterSize(20);
	inputname.setPosition(gamewidth / 2, gameheight / 2);
	inputname.setColor(Color::Black);

	statstext.setFont(font2);
	statstext.setCharacterSize(30);
	statstext.setPosition(statsbut.getPosition().x - buttonwidth / 2, statsbut.getPosition().y - 24 * buttonheight / 50);
	statstext.setColor(Color::Red);
	statstext.setString("Scoreboard");


	name.setFont(font2);
	name.setCharacterSize(30);
	name.setColor(sf::Color::Magenta);
	name.setPosition(gamewidth / 10, gameheight / 15);


	ratio.setFont(font2);
	ratio.setCharacterSize(30);
	ratio.setColor(sf::Color::Red);
	ratio.setPosition(name.getPosition().x + gamewidth / 3, name.getPosition().y);

	on.setFont(font2);
	on.setCharacterSize(22);
	on.setPosition(soundbut[0].getPosition().x + buttonwidth / 22, soundbut[0].getPosition().y + buttonheight * 2 / 16);
	on.setColor(Color::Black);
	on.setString("On");

	off.setFont(font2);
	off.setCharacterSize(22);
	off.setPosition(soundbut[1].getPosition().x + buttonwidth / 30, soundbut[1].getPosition().y + buttonheight * 2 / 16);
	off.setColor(Color::Red);
	off.setString("Off");

	wins.setFont(font2);
	wins.setCharacterSize(30);
	wins.setColor(sf::Color::Blue);
	wins.setPosition(ratio.getPosition().x + gamewidth / 6, name.getPosition().y);

	losses.setFont(font2);
	losses.setCharacterSize(30);
	losses.setColor(sf::Color::Green);
	losses.setPosition(wins.getPosition().x + gamewidth / 6, name.getPosition().y);

	declare.setFont(font2);
	declare.setCharacterSize(35);
	declare.setPosition(playwidth - 1.75*buttonwidth, playheight - 5 * buttonheight);
	declare.setString("");
	declare.setColor(Color::Black);
	declare.Bold;
	declare.Underlined;
}
//Declaration and settings of sprites
void spritedeclare(Texture &background, Texture &button, Texture& marioimg)
{
	sprite.setTexture(background);
	sprite.setTextureRect(IntRect(731, 193, gamewidth, gameheight));

	int x = 21 * gamewidth / 60;
	for (int i = 0; i < 3; i++)
	{
		difficibut[i].setTexture(button);
		difficibut[i].setTextureRect(DiffBut);
		difficibut[i].setPosition(x, gameheight / 2.25);
		x += gamewidth / 20 + buttonheight;
	}
	x = 21 * gamewidth / 60;
	for (int i = 0; i <2; i++)
	{
		soundbut[i].setTexture(button);
		soundbut[i].setTextureRect(DiffBut);
		soundbut[i].setPosition(x, gameheight / 1.5);
		x += gamewidth / 20 + buttonheight;
	}
	startbut.setTexture(button);
	startbut.setTextureRect(Butt);
	startbut.setOrigin(buttonwidth / 2, buttonheight / 2);
	startbut.setPosition(gamewidth / 2, (gameheight*3.8) / 8);


	movebut.setTexture(button);
	movebut.setTextureRect(IntRect(32, 191, gamewidth / 5, buttonheight));
	movebut.setPosition(grid[0][0].getPosition().x + playwidth + width + gamewidth / 15, gameheight / 10);

	newbut.setTexture(button);
	newbut.setTextureRect(IntRect(32, 191, gamewidth / 5, buttonheight));
	newbut.setPosition(movebut.getPosition().x, movebut.getPosition().y + 2 * buttonheight);

	helpbut.setTexture(button);
	helpbut.setTextureRect(Butt);
	helpbut.setOrigin(buttonwidth / 2, buttonheight / 2);
	helpbut.setPosition(startbut.getPosition().x, startbut.getPosition().y + 3 * buttonheight / 2);

	backbut.setTexture(button);
	backbut.setTextureRect(IntRect(32, 191, gamewidth / 5, buttonheight));
	backbut.setPosition(newbut.getPosition().x, gameheight - 4 * buttonheight);

	statsbut.setTexture(button);
	statsbut.setTextureRect(Butt);
	statsbut.setOrigin(buttonwidth / 2, buttonheight / 2);
	statsbut.setPosition(startbut.getPosition().x, gameheight - 2 * buttonheight);

	settingbut.setTexture(button);
	settingbut.setTextureRect(Butt);
	settingbut.setOrigin(buttonwidth / 2, buttonheight / 2);
	settingbut.setPosition(startbut.getPosition().x, startbut.getPosition().y + 3 * buttonheight);

	mario.setTexture(marioimg);
	mario.setPosition(0, gameheight - 175);
	mario.setTextureRect(IntRect(0, 0, 85, 169));
}
//Loader of files function
int loader(Texture &texture, Texture &background, Texture &button, Texture &marioimg, Sound &intro, SoundBuffer &beginsound, Music &gamesound)
{
	if (!texture.loadFromFile("nes-color-palette.gif"))
		return EXIT_FAILURE;
	if (!background.loadFromFile("download.jpg"))
		return EXIT_FAILURE;
	if (!button.loadFromFile("Buttons.jpg"))
		return EXIT_FAILURE;
	if (!font.loadFromFile("manaspc.ttf"))
		return EXIT_FAILURE;
	if (!font2.loadFromFile("manaspc.ttf"))
		return EXIT_FAILURE;
	if (!font3.loadFromFile("waltograph42.otf"))
		return EXIT_FAILURE;
	if (!font4.loadFromFile("waltographUI.ttf"))
		return EXIT_FAILURE;
	if (!marioimg.loadFromFile("mario.png"))
		return EXIT_FAILURE;
	if (!beginsound.loadFromFile("smb_world_clear.wav"))
		return EXIT_FAILURE;
	else intro.setBuffer(beginsound);

	if (!gamesound.openFromFile("Undertale-100-Megalovania.wav"))
		return EXIT_FAILURE;

}
//Color geneator function
void undergamecolorgenerator(Texture& texture)
{

	for (int i = 0; i < numofcolor; i++)
	{

		color[i].setPosition(colorposx, colorposy);
		color[i].setSize(Vector2f(2 * width, 2 * height));
		color[i].setTexture(&texture);
		color[i].setTextureRect(col[i]);
		colorposx += 3 * width;
	}
}

void newmap(Texture &texture)
{

	startposx = gamewidth / 15;
	startposy = gameheight / 15;
	gridcreator(texture, numofcolor);
	old = grid[starty][startx].getTextureRect();
	update = old;
	colorposx = gamewidth / 15;
	undergamecolorgenerator(texture);

}
//Mario animation function
void setanimation()
{
	//depending on animation, animation changes picture
	switch (mariocount % 5)
	{
	case 0:
	{
		mario.setTextureRect(IntRect(0, 0, 85, 169));
		if (mariocount == 0)
			mario.setPosition(0, gameheight - 175);
		mario.move(2 * 9.9423754576095572469935295871468, 0);
		mariocount++;
	}
	break;
	case 1:
	{
		mario.setTextureRect(IntRect(91, 0, 90, 169));
		mario.move(2.4*9.9423754576095572469935295871468, -2.4*10.052737957609557246993529587147);
		mariocount++;
	}
	break;
	case 2:
	{
		mario.setTextureRect(IntRect(187, 0, 97, 169));
		mario.move(2.4*9.9423754576095572469935295871468, -2.4*10.273462957609557246993529587147);
		mariocount++;
	}
	break;
	case 3:
	{
		mario.setTextureRect(IntRect(290, 0, 108, 169));
		mario.move(2.4*9.9423754576095572469935295871468, 2.4*10.273462957609557246993529587147);
		mariocount++;
	}
	break;
	case 4:
	{
		mario.setTextureRect(IntRect(404, 0, 96, 175));
		mario.move(2.4*9.9423754576095572469935295871468, 2.4*10.052737957609557246993529587147);
		mariocount++;
	}
	break;
	}

	int y = (gamewidth / 95) * 5;
	if (mariocount > y)
	{
		mariocount = 0;
	}
}
//Stats Functions
void copy()
{
	ifstream output;
	id x;
	output.open("Text.txt");
	int count = 0;
	output >> e1;
	while (!output.eof())
	{

		x.name = e1;
		output >> e2;
		x.ratio = e2;
		output >> e3;
		x.win = e3;
		output >> e4;
		x.loss = e4;
		output >> e1;

		highscore.push_back(x);
	}
	output.close();


}

void sort()
{
	id f;

	for (int i = 0; i < highscore.size(); i++)
		for (int j = i + 1; j < highscore.size(); j++)
		{
			if (highscore[j].ratio > highscore[i].ratio)
			{
				f = highscore[i];
				highscore[i] = highscore[j];
				highscore[j] = f;

			}
			else if (highscore[j].ratio == highscore[i].ratio)
			{
				if (highscore[j].win > highscore[i].win)
				{
					f = highscore[i];
					highscore[i] = highscore[j];
					highscore[j] = f;
				}
			}
		}
}

int find(string x)
{
	for (int i = 0; i < highscore.size(); i++)
		if (x == highscore[i].name)
			return i;
	return highscore.size();
}

void copytofile()
{
	int x = 10;
	ofstream input;
	input.open("Text.txt");
	sort();
	if (highscore.size() < x)
	{
		x = highscore.size();
	}
	for (int i = 0; i < x; i++)
	{
		string ratio;
		ratio = to_string(highscore[i].ratio);

		ratio.erase(4, ratio.size());

		input << highscore[i].name << "		" << ratio << "	" << highscore[i].win << "		" << highscore[i].loss << endl;

	}
	input.close();
}

float roundvalue(float value, int decimalplace)
{
	double k = std::pow(10, decimalplace);
	return int(round(value * k) / k);
}

void display()
{

	int x = 10;
	sort();
	if (highscore.size() <= x)
	{
		x = highscore.size();
	}
	t1 = "Name  \n";
	t2 = "Ratio \n";
	t3 = "Win \n";
	t4 = "Loss \n";
	for (int j = 0; j < x; j++)
	{
		t1 += highscore[j].name + "\n";
		t2 += to_string(int(highscore[j].ratio)) + "\n";
		t3 += to_string(highscore[j].win) + "\n";
		t4 += to_string(highscore[j].loss) + "\n";
	}
	if (gamestart)
	{
		t1 += "\n\n\n\n" + highscore[vectorpos].name;
		t2 += "\n\n\n\n" + to_string(int(roundvalue(highscore[vectorpos].ratio, 2)));
		t3 += "\n\n\n\n" + to_string(highscore[vectorpos].win);
		t4 += "\n\n\n\n" + to_string(highscore[vectorpos].loss);
	}

	name.setString(t1);
	ratio.setString(t2);
	wins.setString(t3);
	losses.setString(t4);


}