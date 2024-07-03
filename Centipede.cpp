#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void initializingGridObjects(int centiX[]);
void movePlayer(float player[], sf::Clock& playerClock, bool& playerExist);
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveBullet(float bullet[], sf::Clock& bulletClock);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
void shootBullet(float bullet[], float player[],sf::RenderWindow& window,sf::Sprite& bulletSprite, sf::Clock& bulletClock, sf::Sound& fire);
void drawMushroom(sf::RenderWindow& window, int);
void drawgameGrid(sf::RenderWindow& window, sf::Sprite& mushSprite1, sf::Sprite& mushSprite2, sf::Sprite& centipedeHeadSprite, sf::Sprite& centipedeBodySprite, sf::Sprite& centipedeHeadRSprite, sf::Sprite& centipedeRBodySprite, sf::Sprite& poiMushSprite1, sf::Sprite& poiMushSprite2);
void detectCollision(float bullet[], int centiX[], int centiY[], int centiExist[], int centiType[], int centiDirect[], int& score, int extraX[], int extraY[], int extraDirect[], int extraExist[], sf::Sound& mushdeath, float player[], bool& playerExist, sf::Sound& playerdeath);
void moveCenti(sf::Clock& centiClock, int centiX[], int centiY[], int centiExist[], int centiType[], int centiDirect[], float bullet[]);
void printScore(int& score, sf::Text scoreBoard, sf::RenderWindow& window);
void extraHeadMove(int extraX[], int extraY[], int extraDirect[], int extraExist[], sf::Clock& extraClock, sf::Clock& gapClock);
void centiDeath(int centiExist[], sf::RenderWindow& window, int centiX[], int centiY[], int centiDirect[], int centiType[], sf::Sound& centideath, sf::Music& bgMusic);

int main()
{
	srand(time(0));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);
	
	// Initializing Shooting sounds.
	sf::SoundBuffer fireSound;
	fireSound.loadFromFile("Sound Effects/fire1.wav");
	sf::Sound fire;
	fire.setBuffer(fireSound);
	fire.setVolume(20);
	
	// Initializing Mushroom death and Centi\Bullet collision sounds.
	sf::SoundBuffer mushSound;
	mushSound.loadFromFile("Sound Effects/kill.wav");
	sf::Sound mushdeath;
	mushdeath.setBuffer(mushSound);
	mushdeath.setVolume(20);
	
	// Initializing Player death sound.
	sf::SoundBuffer GOsound;
	GOsound.loadFromFile("Sound Effects/death.wav");
	sf::Sound playerdeath;
	playerdeath.setBuffer(GOsound);
	playerdeath.setVolume(20);
	
	// Initializing Centipede death sound.
	sf::SoundBuffer SOsound;
	SOsound.loadFromFile("Sound Effects/1up.wav");
	sf::Sound centideath;
	centideath.setBuffer(SOsound);
	centideath.setVolume(20);
	
	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/5455961c4e5b185bcb3a98fd23c2f25265aaf2f5.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	player[y] = (gameRows * 29/30) * boxPixelsY;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	sf::Clock playerClock;
	bool playerExist = true;

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] - boxPixelsY;
	bullet[exists] = false;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	
	// Initializing Mushrooms and Mushroom Sprites.
	sf::Texture mushTexture1;
	sf::Sprite mushSprite1;
	mushTexture1.loadFromFile("Textures/mushroom.png");
	mushSprite1.setTexture(mushTexture1);
	mushSprite1.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	sf::Texture mushTexture2;
	sf::Sprite mushSprite2;
	mushTexture2.loadFromFile("Textures/mushroom.png");
	mushSprite2.setTexture(mushTexture2);
	mushSprite2.setTextureRect(sf::IntRect(64, 0, boxPixelsX, boxPixelsY));
	sf::Texture poiMushTexture1;
	sf::Sprite poiMushSprite1;
	poiMushTexture1.loadFromFile("Textures/mushroom.png");
	poiMushSprite1.setTexture(poiMushTexture1);
	poiMushSprite1.setTextureRect(sf::IntRect(0, 32, boxPixelsX, boxPixelsY));
	sf::Texture poiMushTexture2;
	sf::Sprite poiMushSprite2;
	poiMushTexture2.loadFromFile("Textures/mushroom.png");
	poiMushSprite2.setTexture(poiMushTexture2);
	poiMushSprite2.setTextureRect(sf::IntRect(64, 32, boxPixelsX, boxPixelsY));
	
	// Initializing arrays to control various traits of the centipede.
	int centiX[12] = {rand()%18};
	int centiY[12] = {};
	int centiDirect[12] = {};
	int centiExist[12] = {};
	int centiType[12] = {1};
	
	
	for (int i=0; i<12; i++) { 
		centiDirect[i] = 'l';
		centiExist[i] = 1;
		centiY[i] = 0;
	}
	
	// Initializing Centipede head and Centipede body sprites.	
	sf::Texture centipedeHeadRTexture;
	sf::Sprite centipedeHeadRSprite;
	centipedeHeadRTexture.loadFromFile("Textures/c_head_right_walk.png");
	centipedeHeadRSprite.setTexture(centipedeHeadRTexture);
	centipedeHeadRSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	sf::Texture centipedeHeadLTexture;
	sf::Sprite centipedeHeadLSprite;
	centipedeHeadLTexture.loadFromFile("Textures/c_head_left_walk.png");
	centipedeHeadLSprite.setTexture(centipedeHeadLTexture);
	centipedeHeadLSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	sf::Texture centipedeBodyLTexture;
	sf::Sprite centipedeBodyLSprite;
	centipedeBodyLTexture.loadFromFile("Textures/c_body_left_walk.png");
	centipedeBodyLSprite.setTexture(centipedeBodyLTexture);
	centipedeBodyLSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	sf::Texture centipedeBodyRTexture;
	sf::Sprite centipedeBodyRSprite;
	centipedeBodyRTexture.loadFromFile("Textures/c_body_right_walk.png");
	centipedeBodyRSprite.setTexture(centipedeBodyRTexture);
	centipedeBodyRSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	sf::Clock centiClock;
	
	// Initializing arrays to control the traits of extra centipede heads.
	int extraX[10] = {};
	int extraY[10] = {};
	int extraDirect[10] = {};
	int extraExist[10] = {};
	
	// Initializing clock objects for controllling the rate of random centipede head updation.
	sf::Clock extraClock;
	sf::Clock gapClock;
					
	// Initilizing extra centipede heads, their locations and directions.
	for (int c=0; c<10; c++) {
		if (c%2==0) {
			extraX[c] = 29;
			extraDirect[c] = 'l';
		}
		else {
			extraX[c] = 0;
			extraDirect[c] = 'r'; 
		}
		extraY[c] = (rand()%4)+25;
	}
	
	// Initializing Score and ScoreBoard.
	static int score = 0;
	sf::Font scorefont;
	scorefont.loadFromFile("ErbosDraco1StOpenNbpRegular-l5wX.ttf");
	sf::Text scoreBoard("score", scorefont);
	scoreBoard.setPosition(10, 10);
	scoreBoard.setCharacterSize(30);
	scoreBoard.setStyle(sf::Text::Bold);
	scoreBoard.setFillColor(sf::Color::Red);
	
	// Initializing Game Over font and text.
	sf::Font GameOverfont;
	GameOverfont.loadFromFile("ErbosDraco1StOpenNbpRegular-l5wX.ttf");
	sf::Text GameOver("GAME OVER", GameOverfont);
	GameOver.setPosition(100, 260);
	GameOver.setCharacterSize(100);
	GameOver.setStyle(sf::Text::Bold);
	GameOver.setFillColor(sf::Color::Red);
	
	initializingGridObjects(centiX);
	
	while(window.isOpen()) {

		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////
	
		window.draw(backgroundSprite);
		movePlayer(player, playerClock, playerExist);
		drawPlayer(window, player, playerSprite);
		shootBullet(bullet, player, window, bulletSprite, bulletClock, fire);
		drawgameGrid(window, mushSprite1, mushSprite2, centipedeHeadLSprite, centipedeBodyLSprite, centipedeHeadRSprite, centipedeBodyRSprite, poiMushSprite1, poiMushSprite2);
		detectCollision(bullet, centiX, centiY, centiExist, centiType, centiDirect, score, extraX, extraY, extraDirect, extraExist, mushdeath, player, playerExist, playerdeath);
		centiDeath(centiExist, window, centiX, centiY, centiDirect, centiType, centideath, bgMusic);
		moveCenti(centiClock, centiX, centiY, centiExist, centiType, centiDirect, bullet);
		extraHeadMove(extraX, extraY, extraDirect, extraExist, extraClock, gapClock);
		printScore(score, scoreBoard, window);
		
		// Once player stops existing, music stops & gameover screen prints indefinitely.
		if (!playerExist) {
			bgMusic.stop();
			window.clear();
			window.draw(GameOver);
		}
		
		sf::Event e;
		while (window.pollEvent(e)) {
		
			if (e.type == sf::Event::Closed) {
				return 0;
			}
		}	
		window.display();
		window.clear();
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

// Initializing Mushrooms and Centipede's starting place.
void initializingGridObjects(int centiX[]) {
	int m=0;
	while (m<30) {
		gameGrid[(rand()%29)+1][(rand()%29)+1]=2;
		m+=1;
	}
	
	for (int i=1; i<12; i++) {
		centiX[i] = centiX[0]+i; 				// Every subsequent piece of centipede is one unit
	}											// to the right of the previous.
	
	gameGrid[0][centiX[0]] = 'L';
	
	for (int i=1; i<12; i++) {
		gameGrid[0][centiX[i]] = 'l';			// All parts have been initialized to move rightwards.
		break;									
	}
}

// Moves the player in the player area.
void movePlayer(float player[], sf::Clock& playerClock, bool& playerExist) {
	
	if (playerClock.getElapsedTime().asMilliseconds() < 40)
		return;
		
	playerClock.restart();
	
	// Use of casting to avoid getting zero in return for integer division.	
	// Condition detects imminent collision with normal mushrooms and stops.
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player[x] > (0.0/(gameColumns) * gameColumns) * boxPixelsX && gameGrid[static_cast<int>(player[y])/32][(static_cast<int>(player[x])/32)-1] != 2)
		player[x]-=32;	
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player[x] < (29.0/(gameColumns) * gameColumns) * boxPixelsX && gameGrid[static_cast<int>(player[y])/32][(static_cast<int>(player[x])/32)+1] != 2)  
		player[x]+=32;
		
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player[y] < (29.0/(gameRows) * gameRows) * boxPixelsY && gameGrid[(static_cast<int>(player[y])/32)+1][static_cast<int>(player[x])/32] != 2)       
		player[y]+=32;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player[y] > (24.0/(gameRows) * gameRows) * boxPixelsY && gameGrid[(static_cast<int>(player[y])/32)-1][static_cast<int>(player[x])/32] != 2) 
		player[y]-=32;
		
	// Detects player collision with the poisonous mushrooms at full health.
	if (gameGrid[static_cast<int>(player[y])/32][static_cast<int>(player[x])/32] == 4) {
		playerExist = false;
	}
			
	// Detects player collision with the poisonous mushrooms at half health.	
	if (gameGrid[static_cast<int>(player[y])/32][static_cast<int>(player[x])/32] == 3) {
	 	playerExist = false;
	 }
}
	/*for (int i=0; i<gameRows; i++)
		for (int j=0; j<gameColumns; j++)
			if (gameGrid[i][j] == 'c')
				gameGrid[i][j] = 0;
				
	gameGrid[static_cast<int>(player[y])/32][static_cast<int>(player[x])/32] = 'c';*/	


// Draws the player sprite on the screen.
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}

// Decrements the bullet's Y axis position, moving it upwards.
void moveBullet(float bullet[], sf::Clock& bulletClock) {
	if (bulletClock.getElapsedTime().asMilliseconds() < 5)
		return;
	
	bulletClock.restart();
	
	bullet[y] -= 10;	
	if (bullet[y] < -32)
		bullet[exists] = false;
}

// Draws the bullet sprite on the screen.
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}

// Introduces conditions that make the bullet come into existence when spacebar is pressed.
void shootBullet(float bullet[], float player[], sf::RenderWindow& window, sf::Sprite& bulletSprite, sf::Clock& bulletClock, sf::Sound& fire) {	
					
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && bullet[exists] == false) {
			bullet[exists]=true;
			bullet[x]=player[x];
			bullet[y]=player[y];
			fire.play();
		}
		
		// Once bullet sprite is in place, the bullet is shot (Y-decrement) and continues to move till collision/border (till bullet un-exists).
		if (bullet[exists] == true) {	
			moveBullet(bullet, bulletClock);
			drawBullet(window, bullet, bulletSprite);
		}	
}

// Prints everything that is introduced into the game grid, whether mushrooms or centipede.
void drawgameGrid(sf::RenderWindow& window, sf::Sprite& mushSprite1, sf::Sprite& mushSprite2, sf::Sprite& centipedeHeadLSprite, sf::Sprite& centipedeBodyLSprite, sf::Sprite& centipedeHeadRSprite, sf::Sprite& centipedeBodyRSprite, sf::Sprite& poiMushSprite1, sf::Sprite& poiMushSprite2) {
	
	for (int r=0; r<gameRows; r++) {
		for (int c=0; c<gameColumns; c++) {
			
			// Normal mushroom at full health.
			if (gameGrid[r][c]==2) {
				mushSprite1.setPosition(c*32, r*32);
				window.draw(mushSprite1);
			}
			
			// Normal mushroom at half health.	
			else if (gameGrid[r][c]==1) {
				mushSprite2.setPosition(c*32, r*32);
				window.draw(mushSprite2);
			}
			
			// Poisonous mushroom at full health.
			else if (gameGrid[r][c]==4) {
				poiMushSprite1.setPosition(c*32, r*32);
				window.draw(poiMushSprite1);
			}
			
			// Poisonous mushroom at half health.
			else if (gameGrid[r][c]==3) {
				poiMushSprite2.setPosition(c*32, r*32);
				window.draw(poiMushSprite2);
			}
			
			// Centipede head moving towards left.
			else if (gameGrid[r][c]=='L') {
				centipedeHeadLSprite.setPosition(c*32, r*32);
				window.draw(centipedeHeadLSprite);
			}
			
			// Centipede body moving towards left.
			else if (gameGrid[r][c]=='l') {
				centipedeBodyLSprite.setPosition(c*32, r*32);
				window.draw(centipedeBodyLSprite);
			}
			
			// Centipede head moving towards right.
			else if (gameGrid[r][c]=='R') {
				centipedeHeadRSprite.setPosition(c*32, r*32);
				window.draw(centipedeHeadRSprite);
			}
			
			// Centipede body moving towards right.
			else if (gameGrid[r][c]=='r') {
				centipedeBodyRSprite.setPosition(c*32, r*32);
				window.draw(centipedeBodyRSprite);
			}
		}
	}
}

// Detects collisions of the bullet with the centipede as well as the mushrooms and employs conditions accordingly.
void detectCollision(float bullet[], int centiX[], int centiY[], int centiExist[], int centiType[], int centiDirect[], int& score, int extraX[], int extraY[], int extraDirect[], int extraExist[], sf::Sound& mushdeath, float player[], bool& playerExist, sf::Sound& playerdeath) {
	
	for (int r=0; r<gameRows; r++) {
		for (int c=0; c<gameColumns; c++) {
			
			// Detects bullet collision with mushroom of full health.
			if (gameGrid[r][c]==2 && bullet[y] >= r*32-16 && bullet[y] <= r*32 && bullet[x] == c*32 && bullet[exists] == true) {
				bullet[exists]=false;
				gameGrid[r][c]=1;
				mushdeath.play();
			}
				
			// Detects bullet collision with mushroom of half health.
			if (gameGrid[r][c]==1 && bullet[y] >= r*32-16 && bullet[y] <= r*32 && bullet[x] == c*32 && bullet[exists] == true) {
				bullet[exists]=false;
				gameGrid[r][c]=0;
				score+=1;
				mushdeath.play();
			}
			
			// Detects bullet collision with poisonous mushrooms at full health.
			if (gameGrid[r][c]==4 && bullet[y] >= r*32-16 && bullet[y] <= r*32 && bullet[x] == c*32 && bullet[exists] == true) {
				bullet[exists]=false;
				gameGrid[r][c]=3;
				mushdeath.play();
			}
			
			// Detects bullet collision with poisonous mushrooms at half health.
			if (gameGrid[r][c]==3 && bullet[y] >= r*32-16 && bullet[y] <= r*32 && bullet[x] == c*32 && bullet[exists] == true) {
				bullet[exists]=false;
				gameGrid[r][c]=0;
				score+=1;
				mushdeath.play();
			}
		}
	}
	
	// Detecting cetipede collision with bullets.
	for (int i=0; i<12; i++) {
				
				// Detects collisions when centipede parts are moving in player area.
				if (centiY[i] >= 24 && centiY[i] <= 29) {
				
					// If centipede part is moving towards right.
					if (centiDirect[i] == 'r' && centiExist[i] && bullet[y] >= centiY[i]*32-16 && bullet[y] <= centiY[i]*32+16 && bullet[x] >= centiX[i]*32-16 && bullet[x] <= centiX[i]*32+16 && bullet[exists]) {
						mushdeath.play();
						bullet[exists] = false;
						centiExist[i] = false;
						gameGrid[centiY[i]][centiX[i]] = 4;
						if (i>0)
						centiType[i-1] = 1;
						
						if (centiType[i] == 1)
							score+=20;
						else
							score+=10;
							
						if (centiType[i] == 1) {
							for (int i=0; i<12; i++) 
								centiExist[i] = 0;
						}
					}	
					
					// If Centipede part is moving towards left.
					else if (centiDirect[i] == 'l' && centiExist[i] && bullet[y] >= centiY[i]*32-16 && bullet[y] <= centiY[i]*32+16 && bullet[x] >= centiX[i]*32-16 && bullet[x] <= centiX[i]*32+16 && bullet[exists]) {		
						mushdeath.play();
						bullet[exists] = false;
						centiExist[i] = false;
						gameGrid[centiY[i]][centiX[i]] = 4;
						if (i<29)
						centiType[i+1] = 1;
						
						if (centiType[i] == 1)
							score+=20;
						else
							score+=10;
						
						if (centiType[i] == 1) {
							for (int i=0; i<12; i++) 
								centiExist[i] = 0;
						}
					}
					
					// Detects centipede collision with the player.
					if (centiDirect[i] == 'r' && centiExist[i] && player[y] >= centiY[i]*32-16 && player[y] <= centiY[i]*32+16 && player[x] == centiX[i]*32) {
						playerExist = false;
						playerdeath.play();
					}
					
					// Detects centipede collision with the player.
					if (centiDirect[i] == 'l' && centiExist[i] && player[y] >= centiY[i]*32-16 && player[y] <= centiY[i]*32+16 && player[x] == centiX[i]*32) {
						playerExist = false;
						playerdeath.play();
					}
				}
				
				// Detects when centipede isn't moving in player area.
				else {
					
					// If centipede part is moving towards right.
					if (centiDirect[i]=='r' && centiExist[i] && bullet[y] <= centiY[i]*32+16 && bullet[y] >= centiY[i]*32-16 && bullet[x] >= centiX[i]*32-16 && bullet[x] <= centiX[i]*32+16 && bullet[exists]) {	
						mushdeath.play();
						bullet[exists] = false;
						centiExist[i] = false;
						gameGrid[centiY[i]][centiX[i]] = 2;
						if (i>0)
						centiType[i+1] = 1;
						
						if (centiType[i] == 1)
							score+=20;
						else
							score+=10;
							
						if (centiType[i] == 1) {
							for (int i=0; i<12; i++) 
								centiExist[i] = 0;
						}
					}
					
					// If centipede part is moving towards left.
					else if (centiDirect[i]=='l' && centiExist[i] && bullet[y] <= centiY[i]*32+16 && bullet[y] >= centiY[i]*32-16 && bullet[x] >= centiX[i]*32-16 && bullet[x] <= centiX[i]*32+16 && bullet[exists]) {
						mushdeath.play();
						bullet[exists] = false;
						centiExist[i] = false;
						gameGrid[centiY[i]][centiX[i]] = 2;
						if (i<29)
						centiType[i+1] = 1;
						
						if (centiType[i] == 1)
							score+=20;
						else
							score+=10;
							
						if (centiType[i] == 1) {
							for (int i=0; i<12; i++) 
								centiExist[i] = 0;
						}
					}
				}
	}
	
	for (int i=0; i<10; i++) {
		// If centipede part is moving towards right.
		if (extraDirect[i] == 'r' && extraExist[i] && bullet[y] >= extraY[i]*32-16 && bullet[y] <= extraY[i]*32+16 && bullet[x] >= extraX[i]*32-16 && bullet[x] <= extraX[i]*32+16 && bullet[exists]) {
			mushdeath.play();
			bullet[exists] = false;
			extraExist[i] = false;
			if (extraY[i] >= 0 && extraY[i] < 30 && extraX[i] >=0 && extraY[i] <30)
				gameGrid[extraY[i]][extraX[i]] = 4;
			score+=20;
		}	
				
		// If Centipede part is moving towards left.
		if (extraDirect[i] == 'l' && extraExist[i] && bullet[y] >= extraY[i]*32-16 && bullet[y] <= extraY[i]*32+16 && bullet[x] >= extraX[i]*32-16 && bullet[x] <= extraX[i]*32+16 && bullet[exists]) {
			mushdeath.play();
			bullet[exists] = false;
			extraExist[i] = false;
			if (extraY[i] >= 0 && extraY[i] < 30 && extraX[i] >=0 && extraY[i] <30)
				gameGrid[extraY[i]][extraX[i]] = 4;
			score+=20;
		}
		
		// Detects right moving extra centipede head collision with the player. 
		if (extraDirect[i] == 'r' && extraExist[i] && player[y] >= extraY[i]*32-16 && player[y] <= extraY[i]*32+16 && player[x] >= extraX[i]*32-16 && player[x] <= extraX[i]*32+16 && player[exists]) {
			playerExist = false;
			playerdeath.play();
		}
		
		// Detects left moving extra centipede head collision with the player.
		if (extraDirect[i] == 'l' && extraExist[i] && player[y] >= extraY[i]*32-16 && player[y] <= extraY[i]*32+16 && player[x] >= extraX[i]*32-16 && player[x] <= extraX[i]*32+16 && player[exists]) {
			playerExist = false;
			playerdeath.play();
		}
	}
}

// Controls everything related to the centipede movement.
void moveCenti(sf::Clock& centiClock, int centiX[], int centiY[], int centiExist[], int centiType[], int centiDirect[], float bullet[]) {
	if (centiClock.getElapsedTime().asMilliseconds() > 50) {
			centiClock.restart();
			
			// Clears all instances of the centipede on the game grid.
			for (int r=0; r<gameRows; r++) {
				for (int c=0; c<gameColumns; c++) {
					
					if (gameGrid[r][c] == 'l' || gameGrid[r][c] == 'r' || gameGrid[r][c] == 'L' || gameGrid[r][c] == 'R' || gameGrid[r][c] == 'E')
						gameGrid[r][c] = 0;
	
				}
			}
		
			// Prints all instances of the centipede on the game grid.
			for (int i=0; i<12; i++) {
				
				if (centiExist[i] == 1) {
					if (centiDirect[i] == 'l' && centiType[i] == 1)
						gameGrid[centiY[i]][centiX[i]] = 'L';
					
					if (centiDirect[i] == 'l' && centiType[i] == 0)
						gameGrid[centiY[i]][centiX[i]] = 'l';
					
					if (centiDirect[i] == 'r' && centiType[i] == 1)
						gameGrid[centiY[i]][centiX[i]] = 'R';
					
					if (centiDirect[i] == 'r' && centiType[i] == 0)
						gameGrid[centiY[i]][centiX[i]] = 'r';
				}
				
				///////////////////////////////////////////////////////////////	
				//			            	  		     //
				// Controls movement of the centipede parts in player area.  //
				//							     //
				///////////////////////////////////////////////////////////////
				
				if (centiY[i] >= 25 && centiY[i] <= 29) {
					
					// Initializing array to control up and down direction of centipede in player area.
					static int centiUoD[12] = {'d','d','d','d','d','d','d','d','d','d','d','d'};
					
					// Controls movement of centipede parts if on the bottom row of player area.
					if (centiY[i] == 29) {
						if (centiDirect[i] == 'r' && (centiX[i] == 29 || gameGrid[centiY[i]][centiX[i]+1] == 2 || gameGrid[centiY[i]][centiX[i]+1] == 1 || gameGrid[centiY[i]][centiX[i]+1] == 4 || gameGrid[centiY[i]][centiX[i]+1] == 3)) {
							centiUoD[i]='u';
							centiY[i]-=1;
							centiX[i]+=1;
							centiDirect[i] = 'l';
						}
					
						else if (centiDirect[i] == 'l' && (centiX[i] == 0 || gameGrid[centiY[i]][centiX[i]-1] == 2 || gameGrid[centiY[i]][centiX[i]-1] == 1 || gameGrid[centiY[i]][centiX[i]-1] == 4 || gameGrid[centiY[i]][centiX[i]-1] == 3)) {
							centiUoD[i]='u';
							centiY[i]-=1;
							centiX[i]-=1;
							centiDirect[i] = 'r';
						}
					}
				
					// Controls movement of centipede parts if on the top row of the player area.
					if (centiY[i] == 25 && centiUoD[i] == 'u') {
						if (centiDirect[i] == 'r' && (centiX[i] == 29 || gameGrid[centiY[i]][centiX[i]+1] == 2 || gameGrid[centiY[i]][centiX[i]+1] == 1 || gameGrid[centiY[i]][centiX[i]+1] == 4 || gameGrid[centiY[i]][centiX[i]+1] == 3)) {
							centiUoD[i]='d';
							centiY[i]-=1;
							centiX[i]+=1;
							centiDirect[i] = 'l';
						}
					
						else if (centiDirect[i] == 'l' && (centiX[i] == 0 || gameGrid[centiY[i]][centiX[i]-1] == 2 || gameGrid[centiY[i]][centiX[i]-1] == 1 || gameGrid[centiY[i]][centiX[i]-1] == 4 || gameGrid[centiY[i]][centiX[i]-1] == 3)) {
							centiUoD[i]='d';
							centiY[i]-=1;
							centiX[i]-=1;
							centiDirect[i] = 'r';
						}
					}
					
					// Controls mvement of centipede parts between the top and bottom row of the player area if part moving to the left.
					if (centiDirect[i] == 'l' && (centiX[i] == 0 || gameGrid[centiY[i]][centiX[i]-1] == 2 || gameGrid[centiY[i]][centiX[i]-1] == 1 || gameGrid[centiY[i]][centiX[i]-1] == 4 || gameGrid[centiY[i]][centiX[i]-1] == 3)) {
						centiX[i]-=1;
						if (centiUoD[i] == 'd')
							centiY[i]+=1;
						else
							centiY[i]-=1;
						centiDirect[i] = 'r';
					}
						
					// Controls mvement of centipede parts between the top and bottom row of the player area if part moving to the right.
					if (centiDirect[i] == 'r' && (centiX[i] == 29 || gameGrid[centiY[i]][centiX[i]+1] == 2 || gameGrid[centiY[i]][centiX[i]+1] == 1 || gameGrid[centiY[i]][centiX[i]+1] == 4 || gameGrid[centiY[i]][centiX[i]+1] == 3)) {
						centiX[i]+=1;
						if (centiUoD[i] == 'd')
							centiY[i]+=1;
						else
							centiY[i]-=1;
						centiDirect[i] = 'l';
					} 
				}
				
				////////////////////////////////////////////////////////////////////////
				//								      //	
				// Controls the movement of the centipede parts in non-player area.   //
				//								      //	
				////////////////////////////////////////////////////////////////////////
				
				else {
					
					if (centiDirect[i] == 'l' && (gameGrid[centiY[i]][centiX[i]-1] == 2 || gameGrid[centiY[i]][centiX[i]-1] == 1 || centiX[i] == 0)) {
						centiX[i]-=1;
						centiY[i]+=1;
						centiDirect[i] = 'r';
					}
						
					else if (centiDirect[i] == 'r' && (gameGrid[centiY[i]][centiX[i]+1] == 2 || gameGrid[centiY[i]][centiX[i]+1] == 1 || centiX[i] == 29)) {
						centiX[i]+=1;
						centiY[i]+=1;
						centiDirect[i] = 'l';
					}
				}
				
				// Increments/decrements the X-coordinates of the centipede parts based on their direction with each iteration of the function. 
				if (centiDirect[i] == 'l' || centiDirect[i] == 'L')
					centiX[i]-=1;
				
				if (centiDirect[i] == 'r' || centiDirect[i] == 'R')
					centiX[i]+=1;
			}	
		}
}

// Prints the score on top left of the screen.
void printScore(int& score, sf::Text scoreBoard, sf::RenderWindow& window) {
	scoreBoard.setString(std::to_string(score));
	window.draw(scoreBoard);
}

// Controls the movement of the extra heads in player area.
void extraHeadMove(int extraX[], int extraY[], int extraDirect[], int extraExist[], sf::Clock& extraClock, sf::Clock& gapClock) {
	
	// For checking whether centipede is in player area.
	static bool centiInArea = false;
	
	for (int i=25; i<gameRows; i++) {
		for (int j=0; j<gameColumns; j++) {
			if (gameGrid[i][j] == 'l' || gameGrid[i][j] == 'L' || gameGrid[i][j] == 'r' || gameGrid[i][j] == 'R') {
				centiInArea = true;
				break;
			}
		}
	}
	
	// Once it is in position, condition runs.
	if (centiInArea == true) {
		
		// Brings a new head to existence every six seconds the centipede is in the player area.
		if (gapClock.getElapsedTime().asMilliseconds() > 6000) {
			
			static int count = 0;
			
			if (count<20) {
				extraExist[count] = 1;
				count++;
			}
			gapClock.restart();
		}
		
		// Centipede head position updation every 50 milliseconds.
		if (extraClock.getElapsedTime().asMilliseconds() > 50) {
		extraClock.restart();
		
		static int extraUoD[10] = {'d','d','d','d','d','d','d','d','d','d'};
		
		for (int i=0; i<10; i++) {
		
			if (extraExist[i] == 1) {
			
				if (extraDirect[i] == 'l')
					gameGrid[extraY[i]][extraX[i]] = 'L';
						
				else if (extraDirect[i] == 'r')
					gameGrid[extraY[i]][extraX[i]] = 'R';
			
				if (extraY[i] == 29) {
					if (extraDirect[i] == 'r' && (extraX[i] == 29 || gameGrid[extraY[i]][extraX[i]+1] == 2 || gameGrid[extraY[i]][extraX[i]+1] == 1 || gameGrid[extraY[i]][extraX[i]+1] == 4 || gameGrid[extraY[i]][extraX[i]+1] == 3)) {
						extraUoD[i]='u';
						extraY[i]-=1;
						if (extraX[i]<30)
							extraX[i]+=1;
						extraDirect[i] = 'l';
					}
						
					else if (extraDirect[i] == 'l' && (extraX[i] == 0 || gameGrid[extraY[i]][extraX[i]-1] == 2 || gameGrid[extraY[i]][extraX[i]-1] == 1 || gameGrid[extraY[i]][extraX[i]-1] == 4 || gameGrid[extraY[i]][extraX[i]-1] == 3)) {
						extraUoD[i]='u';
						extraY[i]-=1;
						if (extraX[i]>0)
							extraX[i]-=1;
						extraDirect[i] = 'r';
					}
				}	
					
				// Controls movement of centipede head segments if on the top row of the player area.
				if (extraY[i] == 25 && extraUoD[i] == 'u') {
					if (extraDirect[i] == 'r' && (extraX[i] == 29 || gameGrid[extraY[i]][extraX[i]+1] == 2 || gameGrid[extraY[i]][extraX[i]+1] == 1 || gameGrid[extraY[i]][extraX[i]+1] == 4 || gameGrid[extraY[i]][extraX[i]+1] == 3)) {
						extraUoD[i]='d';
						extraY[i]-=1;
						if (extraX[i]<30)
							extraX[i]+=1;
						extraDirect[i] = 'l';
					}
					
					else if (extraDirect[i] == 'l' && (extraX[i] == 0 || gameGrid[extraY[i]][extraX[i]-1] == 2 || gameGrid[extraY[i]][extraX[i]-1] == 1 || gameGrid[extraY[i]][extraX[i]-1] == 4 || gameGrid[extraY[i]][extraX[i]-1] == 3)) {
						extraUoD[i]='d';
						extraY[i]-=1;
						if (extraX[i]>0)
							extraX[i]-=1;
						extraDirect[i] = 'r';
					}
				}
						
				// Controls mvement of centipede head segments between the top and bottom row of the player area if part moving to the left.
				if (extraDirect[i] == 'l' && (extraX[i] == 0 || gameGrid[extraY[i]][extraX[i]-1] == 2 || gameGrid[extraY[i]][extraX[i]-1] == 1 || gameGrid[extraY[i]][extraX[i]-1] == 4 || gameGrid[extraY[i]][extraX[i]-1] == 3)) {
					if (extraX[i]>0)
						extraX[i]-=1;
					if (extraUoD[i] == 'd')
						extraY[i]+=1;
					else
						extraY[i]-=1;
					extraDirect[i] = 'r';
				}
						
				// Controls mvement of centipede head segments between the top and bottom row of the player area if part moving to the right.
				if (extraDirect[i] == 'r' && (extraX[i] == 29 || gameGrid[extraY[i]][extraX[i]+1] == 2 || gameGrid[extraY[i]][extraX[i]+1] == 1 || gameGrid[extraY[i]][extraX[i]+1] == 4 || gameGrid[extraY[i]][extraX[i]+1] == 3)) {
					if (extraX[i]<30)
						extraX[i]+=1;
					if (extraUoD[i] == 'd')
						extraY[i]+=1;
					else
						extraY[i]-=1;
					extraDirect[i] = 'l';
				}
				
				// X-updation accordingly with direction of movement.
				if (extraDirect[i] == 'l' && extraX[i]>0)
					extraX[i]-=1;
						
				else if (extraDirect[i] == 'r' && extraX[i]<30)
					extraX[i]+=1;
				}
			}
		}
	}
}

// Checks for the entire centipede unexisting and forms a new one.
void centiDeath(int centiExist[], sf::RenderWindow& window, int centiX[], int centiY[], int centiDirect[], int centiType[], sf::Sound& centideath, sf::Music& bgMusic) {
	
	bool centiAlive = false;
	
	for (int i=0; i<12; i++) {
		if (centiExist[i]) {
			centiAlive = true;
			break;
		}
	}
	
	if (!centiAlive) {
	
		bgMusic.stop();	
		centideath.play();
	
		for (int i=0; i<12; i++) {
			centiX[i] = 0;
			centiY[i] = 0;
			centiDirect[i] = 0;
			centiExist[i] = 0;
			centiType[i] = 0;
		}
		
		centiX[12] = {rand()%18};
		centiY[12] = {};
		centiDirect[12] = {};
		centiExist[12] = {};
		centiType[12] = {1};
	
	
		for (int i=0; i<12; i++) { 
			centiDirect[i] = 'l';
			centiExist[i] = 1;
			centiY[i] = 0;
		}
		
		for (int i=1; i<12; i++) {
			centiX[i] = centiX[0]+i; 
		}
	
		gameGrid[0][centiX[0]] = 'L';
	
		for (int i=1; i<12; i++) {
			gameGrid[0][centiX[i]] = 'l';
		}
	}
	
}
