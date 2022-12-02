#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<stdio.h>
#include<time.h>
#include<Windows.h>
const int row = 20;
const int line = 10;
using namespace sf;

int blocks[7][4]
{
	10,20,30,40,    		//0   I
	30,11,21,31, 			//1   L
	10,11,21,31,			//2   J
	20,11,21,31,			//3   T
	20,30,21,31,			//4   O
	10,20,21,31,			//5   Z
	20,30,11,21,			//6   S
};
int table[row][line] = { 0 }, dropBlocks[15] = { 0 } ,score=0;
RenderWindow window(VideoMode(1129, 768), "Tetris");
Clock clocks;
Sound clrline;
Font font;
Text textScore;

struct Point
{
	int x;
	int y;
}drop[4],view[4],next[4];

void initGame(double *delay, int *maxscore)
{
	/*printf("你想要......\n");
	printf("x：更改游戏设置\n");
	printf("任意键：正常地游玩\n");
	char key = _getch();
	if (key == 'x' || key == 'X') {
		printf("\n你想要......(回车键默认)\n");
		printf("z.更改分数上限（默认1000）\n");
		printf("x.更改落速（默认0.5，数字越低越快）\n");
		printf("c.返回...\n");
		char key = _getch();
		switch (key)
		{
		case 'Z':
		case 'z':
			printf("\n最大分数： ");
			scanf_s("%d", maxscore);
			break;
		case 'X':
		case 'x':
			printf("\n落速： ");
			scanf_s("%lf", delay);
			break;
		default:
			break;
		}
	}*/

}

void pauseGame() 
{
	Event keys;
	RenderWindow pause(VideoMode(200, 150), "Pause");
	while (1) {
		pause.pollEvent(keys);
		if (keys.key.code == Keyboard::Enter)
			break;
	}
	pause.close();
}

int GameOver(int *maxscore) 
{
	if (score >= *maxscore) {
		return 2;
	}
	for (int j = 0; j < line; j++)
		if (table[0][j])
			return 1;
	return 0;
}

bool checkBlock(int checknum)
{
	switch (checknum)
	{
	case line:
		for (int i = 0; i < 4;i++) {
			if (drop[i].x >= line || drop[i].x <= -1)
				return false;
		}
	case row:
		for (int i = 0; i < 4; i++) {
			if (drop[i].y >= row || table[drop[i].y][drop[i].x])
				return false;
		}
	default:
		break;
	}
	return true;
}

void dropBlock()
{
	for (int i = 0; i < 4; i++)
		drop[i].y++;
}

void moveBlock(int offest) 
{
	int backBlock[4];
	for (int i = 0; i < 4; i++) {
		backBlock[i] = drop[i].x;
		drop[i].x += offest;
	}
	if (checkBlock(line) == false) {
		for (int i = 0; i < 4; i++) {
			drop[i].x = backBlock[i];
		}
	}
}

void clockWise()
{
	Point bakdrop[4];
	Point center = drop[2];
	if (dropBlocks[0] == 5) {
		return;
	}
	for (int i = 0; i < 4; i++) {
		bakdrop[i] = drop[i];
	}
	for (int j = 0; j < 4; j++) {
		Point tmp = drop[j];
		drop[j].x = center.x - tmp.y + center.y;
		drop[j].y = center.y + tmp.x - center.x;
	}
	if (!checkBlock(line)) {
		for (int i = 0; i < 4; i++) {
			drop[i] = bakdrop[i];
		}
	}
}

void counterClockWise() 
{


}

void rotateBlock(Window *window)
{
	Event rots;
	while (window->pollEvent(rots)) {
		if (rots.key.code == Keyboard::Up)
			clockWise();
		if (rots.key.code == Keyboard::Z)
			counterClockWise();
	}
}

void viewBlock() 
{
	Point bakdrop[4];
	bool check=1;
	for (int i = 0; i < 4; i++) {
		view[i] = drop[i];
	}
	while (check) {
		for (int i = 0; i < 4; i++) {
			bakdrop[i] = view[i];
			view[i].y++;
			if (view[i].y >= row || table[view[i].y][view[i].x])
				check = 0;
		}
	}
	for (int i = 0; i < 4; i++) {
		view[i] = bakdrop[i];
	}
}

void translateBlock(RenderWindow *window, double *delay)
{
	int offest = 0;
	Event keys,rots;
	while (window->pollEvent(keys)) {
		if (keys.type == Event::Closed) {
			window->close();
		}
		if (keys.type == Event::KeyPressed)
		{
			switch (keys.key.code) {
			case Keyboard::Down:
				for (int i = 0; i < 4; i++)
					drop[i].y++;
				break;
			case Keyboard::Left:
				offest -= 1;
				break;
			case Keyboard::Right:
				offest += 1;
				break;
			case Keyboard::Space:
				for (int i = 0; i < 4; i++) {
					drop[i].x = view[i].x;
					drop[i].y = view[i].y;
				}
				break;
			case Keyboard::Escape:
			default:
				break;
			}
		}
	}
	if (offest)
		moveBlock(offest);
}

void setBlock()
{
	static int random[7] = { 1,2,3,4,5,6,7 };		
	int tmp=0;
	srand(time(0));
	for (int i = 0; i < 4; i++) {
		table[drop[i].y-1][drop[i].x] = dropBlocks[0];
	}
	dropBlocks[0] = 0;
	if (dropBlocks[7] <= 0 || dropBlocks[7]>=8) {
		for (int i = 0; i < 30; i++) {
			int rana = rand() % 7;
			int ranb = rand() % 7;
			tmp = random[rana];
			random[rana] = random[ranb];
			random[ranb] = tmp;
		}
		for (int i = 7; i < 14; i++) {
			dropBlocks[i] = random[i - 7];
		}
	}
	while (!dropBlocks[0]) {
		for (int i = 0; i < 15; i++) {
			dropBlocks[i] = dropBlocks[i + 1];
		}
	}
	for (int i = 0; i < 4; i++)
	{
		drop[i].x = (blocks[dropBlocks[0]-1][i] / 10)+2;
		drop[i].y = blocks[dropBlocks[0]-1][i] % 10;
		next[i].x = (blocks[dropBlocks[1] - 1][i] / 10) + 10;
		next[i].y = blocks[dropBlocks[1] - 1][i] % 10;
	}
}   

void drawBlock(Sprite *spriteBlock,RenderWindow *window)
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < line; j++) {
			if (table[i][j]) {
				spriteBlock->setTextureRect(IntRect((table[i][j] - 1) * 38, 0, 38, 38));
				spriteBlock->setPosition(j * 38, i * 38);
				spriteBlock->move(48, 8);
				window->draw(*spriteBlock);
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		spriteBlock->setTextureRect(IntRect(266, 0, 38, 38));
		spriteBlock->setPosition(view[i].x * 38, view[i].y * 38);
		spriteBlock->move(48, 8);
		window->draw(*spriteBlock);
		spriteBlock->setTextureRect(IntRect((dropBlocks[0] - 1) * 38, 0, 38, 38));
		spriteBlock->setPosition(drop[i].x * 38, drop[i].y * 38);
		spriteBlock->move(48, 8);
		window->draw(*spriteBlock);
		spriteBlock->setTextureRect(IntRect((dropBlocks[1] - 1) * 38, 0, 38, 38));
		spriteBlock->setPosition(next[i].x * 38, next[i].y * 38);
		spriteBlock->move(40, 50);
		window->draw(*spriteBlock);
	}
}

void checkLine() 
{
	int k = row - 1,line=0;
	for (int i = row - 1; i > 0; i--) {
		int count = 0;
		for (int j = 0; j < line; j++)
		{
			if (table[i][j]) count++;

			table[k][j] = table[i][j];
		}
		if (count < line) k--;
		else {
			line++;
			score += line*10;
			clrline.play();
		}
	}
	char tmp[10];
	sprintf_s(tmp,"%d", score);
	textScore.setString(tmp);
}

void Status() 
{
	font.loadFromFile("Sansation.ttf");
	textScore.setFont(font); 
	textScore.setCharacterSize(60);
	textScore.setFillColor(Color::White);
	textScore.setStyle(Text::Bold); 
	textScore.setPosition(50, 3);
	textScore.setString("0");
}

int playGame(int *maxscore,double *delay,Sprite *spriteBg)
{
	Music bgm;
	bgm.openFromFile("sound/bgm01.wav");
	bgm.setVolume(50);
	bgm.setLoop(true);
	bgm.play();

	SoundBuffer clr;
	clr.loadFromFile("sound/clearline.wav");
	clrline.setBuffer(clr);

	Texture tile;
	tile.loadFromFile("images/blocks.jpg");
	Sprite spriteBlock(tile);
	bool check = false;
	double timer = 0, delaytime = 0;
	while (window.isOpen())
	{
		float time = clocks.getElapsedTime().asSeconds();
		timer += time;
		delaytime += time;
		clocks.restart();
		window.draw(*spriteBg);
		window.draw(textScore);
		Status();
		if (check == false) {
			setBlock();
		}
		drawBlock(&spriteBlock, &window);
		viewBlock();
		check = checkBlock(row);
		translateBlock(&window, delay);
		rotateBlock(&window);
		if (timer > *delay && check) {
			dropBlock();
			timer = 0;
		}
		if (delaytime > 20) {
			*delay *= 0.98;
			delaytime = 0;
		}
		checkLine();
		switch (GameOver(maxscore)) 
		{
		case 1:
		case 2:
			bgm.stop();
			sleep(seconds(3));
			return GameOver(maxscore);
		default:
			break ;
		}
		window.display();
	}
}

bool retry() 
{
	Event retry;
	sleep(seconds(1));
	while (window.pollEvent(retry)) {
		if (retry.key.code == Keyboard::Enter) {
			table[row][line] = { 0 };
			dropBlocks[15] = { 0 };
			score = 0;
			window.display();
		}
			return true;
	}
	return false;
}

int main()
{
	double delay = 0.5;
	int maxscore = 1000;

	Texture bg;
	bg.loadFromFile("images/bg.jpg");
	Sprite spriteBg(bg);
	do {
		playGame(&maxscore, &delay, &spriteBg);
	} while (1);
	window.close();
	return 0;
}