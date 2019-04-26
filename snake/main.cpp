#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <SDL_ttf.h>
#include "SDL_utils.h"
#include "Game.h"
#include "Gallery.h"
#include "Position.h"

using namespace std;

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;
const char WINDOW_TITLE[] = "SnakeVsBlock";

const int BOARD_WIDTH = 30;
const int BOARD_HEIGHT = 20;
const int CELL_SIZE = 30;

const SDL_Color BOARD_COLOR = {255, 165, 0};
const SDL_Color LINE_COLOR = {255, 255, 255};

const double STEP_DELAY = 0.2;
const double WaitBlockTime = 0.2;
#define CLOCK_NOW chrono::system_clock::now
typedef chrono::duration<double> ElapsedTime;

const int caseOfBlock = 3;
const int firstForRandom = 1;

void renderSplashScreen();
void renderBackground(SDL_Renderer*);
void renderGamePlay(SDL_Renderer* renderer, const Game& game, TTF_Font* font, SDL_Surface* surface, SDL_Texture* texture, vector <int> number);
void insertText (SDL_Renderer* renderer,int left, int top, TTF_Font* font, SDL_Surface* surface, SDL_Texture* texture, const string text, Position pos);
void interpretEvent(SDL_Event e, Game& game);
void Menu(SDL_Renderer*, SDL_Window*,  TTF_Font* font, SDL_Surface* surface, SDL_Texture* texture);
void WonGame(SDL_Renderer*, SDL_Window*,  TTF_Font* font, SDL_Surface* surface, SDL_Texture* texture, bool inGame);
void RestartGame(SDL_Renderer*, SDL_Window*,  TTF_Font* font, SDL_Surface* surface, SDL_Texture* texture, bool inGame);

float randomKindOfBlock()
{
    return rand()%caseOfBlock+firstForRandom ;
}


Gallery* gallery = nullptr; // global picture manager

int main(int argc, char* argv[])
{
    srand(time(0));
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font = NULL;
    SDL_Surface* surface;
    SDL_Texture* texture;

    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    gallery = new Gallery(renderer);

    renderSplashScreen();

    Menu(renderer, window,  font,  surface,  texture);

    bool inGame=true;
    while (inGame)
    {
        Game game(BOARD_WIDTH, BOARD_HEIGHT);
        SDL_Event e;
        auto start = CLOCK_NOW();

        vector <int> arrayNumberOfBlock={};

        renderGamePlay(renderer, game, font, surface, texture, {0});

        auto timeForBlock=CLOCK_NOW();
        while (game.isGameRunning()) {
            while (SDL_PollEvent(&e)) {
                interpretEvent(e, game); // phân loại input và push vào queue
            }

            auto end = CLOCK_NOW();
            ElapsedTime elapsed = end-start;
            ElapsedTime elapsedBlock=end-timeForBlock;

            if(elapsedBlock.count()>WaitBlockTime)
            {
                if (randomKindOfBlock()==1)
                    game.addBlock();
                else
                    game.addBlockBall();

                timeForBlock=CLOCK_NOW();
            }
            if (elapsed.count() > STEP_DELAY) {
                arrayNumberOfBlock.push_back(game.randomNumberofBlock());
                cout<<arrayNumberOfBlock[0]<<" ";
                Position p=game.nextStep();

                for (int i=0; i<game.getBlockBallPosition().size(); i++)
                {
                    if (p==game.getBlockBallPosition()[i])
                    {
                        game.getBlockBallPosition()[i]=0;
                        arrayNumberOfBlock[i]=0;
                    }
                }

                renderGamePlay(renderer, game, font, surface, texture, arrayNumberOfBlock);
                start = end;
            }
            SDL_Delay(1);
        }

        if(game.isGameWon())
        {
            WonGame(renderer, window, font, surface, texture, inGame);
        }

        if (game.isGameOver())
        {
            RestartGame(renderer, window, font, surface, texture, inGame);
        }
    }
    delete gallery;
    quitSDL(window, renderer);
    return 0;
}


void renderSplashScreen()
{
    cout << "Press any key to start game" << endl;
    waitUntilKeyPressed();
}

void drawBackground(SDL_Renderer* renderer, int left, int top, SDL_Texture* texture)
{
	SDL_Rect Background;
	Background.x = left;
	Background.y = top;
	Background.w = SCREEN_WIDTH;
	Background.h = SCREEN_HEIGHT;
	SDL_RenderCopy(renderer, texture, NULL, &Background);
}

void renderBackground(SDL_Renderer* renderer)
{
    int top = 0, left = 0;
    drawBackground(renderer, left, top, gallery->getImage(PIC_BACKGROUND));
    SDL_RenderPresent(renderer);
}


//float generateRandomNumber()
//{
//    return (float) rand() / RAND_MAX;
//}

void drawCell(SDL_Renderer* renderer, int left, int top, Position pos, SDL_Texture* texture)
{
	SDL_Rect cell;
	cell.x = left + pos.x * CELL_SIZE + 2;
	cell.y = top + pos.y * CELL_SIZE + 2;
	cell.w = CELL_SIZE - 4;
	cell.h = CELL_SIZE - 4;
	SDL_RenderCopy(renderer, texture, NULL, &cell);
}

void drawScissors(SDL_Renderer* renderer, int left, int top, Position pos)
{
    drawCell(renderer, left, top, pos, gallery->getImage(PIC_SCISSORS));
}

void drawBlock(SDL_Renderer* renderer, int left, int top, Position pos)
{
    drawCell(renderer, left, top, pos, gallery->getImage(PIC_BLOCK_GAMEOVER));
}

void drawBlockBall(SDL_Renderer* renderer, int left, int top, Position pos, int number)
{
    switch(number)
    {
        case 1: drawCell(renderer, left, top, pos, gallery->getImage(PIC_BLOCK_ADDHEAD1)); break;
        case 2: drawCell(renderer, left, top, pos, gallery->getImage(PIC_BLOCK_ADDHEAD2)); break;
        case 3: drawCell(renderer, left, top, pos, gallery->getImage(PIC_BLOCK_ADDHEAD3)); break;
        case 4: drawCell(renderer, left, top, pos, gallery->getImage(PIC_BLOCK_ADDHEAD4)); break;
        case 5: drawCell(renderer, left, top, pos, gallery->getImage(PIC_BLOCK_ADDHEAD5)); break;
    }

}

void drawSnake(SDL_Renderer* renderer, int left, int top, vector<Position> pos)
{
	drawCell(renderer, left, top, pos[pos.size()-1], gallery->getImage(PIC_SNAKE_HEAD));
    for (int i = pos.size() - 2; i >= 0; i--)
        drawCell(renderer, left, top, pos[i], gallery->getImage(PIC_SNAKE_BODY));
}

void drawVerticalLine(SDL_Renderer* renderer, int left, int top, int cells)
{
    SDL_SetRenderDrawColor(renderer, LINE_COLOR.r, LINE_COLOR.g, LINE_COLOR.b, 0);
    SDL_RenderDrawLine(renderer, left, top, left, top + cells * CELL_SIZE);
}

void drawHorizontalLine(SDL_Renderer* renderer, int left, int top, int cells)
{
    SDL_SetRenderDrawColor(renderer, LINE_COLOR.r, LINE_COLOR.g, LINE_COLOR.b, 0);
    SDL_RenderDrawLine(renderer, left, top, left + cells * CELL_SIZE, top);
}

void insertText (SDL_Renderer* renderer,int left, int top, TTF_Font* font, SDL_Surface* surface, SDL_Texture* texture, const string text, Position pos)
{
    font = TTF_OpenFont("VeraMoBd.ttf", 100);

	SDL_Color fg = { 255, 255, 255 };

	surface = TTF_RenderText_Solid(font, text.c_str(), fg);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect srcRest;
	SDL_Rect desRect;
	TTF_SizeText(font, text.c_str(), &srcRest.w, &srcRest.h);

	srcRest.x = 0;
	srcRest.y = 0;

	desRect.x = left + pos.x;
	desRect.y = top + pos.y;
	desRect.w = srcRest.w;
	desRect.h = srcRest.h;

	SDL_RenderCopy(renderer, texture, &srcRest, &desRect);
}


void renderGamePlay(SDL_Renderer* renderer, const Game& game, TTF_Font* font, SDL_Surface* surface, SDL_Texture* texture, vector <int> number)
{
    int top = 0, left = 0;

    SDL_Rect Background;
	Background.x = left;
	Background.y = top;
	Background.w = SCREEN_WIDTH;
	Background.h = SCREEN_HEIGHT;
	SDL_RenderCopy(renderer, gallery->getImage(PIC_BACKGROUND), NULL, &Background);

    for (int x = 0; x <= BOARD_WIDTH; x++)
        drawVerticalLine(renderer, left + x*CELL_SIZE, top, BOARD_HEIGHT);
    for (int y = 0; y <= BOARD_HEIGHT; y++)
        drawHorizontalLine(renderer, left, top+y * CELL_SIZE, BOARD_WIDTH);

    drawScissors(renderer, left, top, game.getScissorsPosition());

    for (int i=0; i<game.getBlockPosition().size(); i++)
        drawBlock(renderer, left, top, game.getBlockPosition()[i]);

    if (number[0]>0)
    {
        for (int i=0; i<game.getBlockBallPosition().size(); i++)
            drawBlockBall(renderer, left, top, game.getBlockBallPosition()[i], number[i]);
    }

    drawSnake(renderer, left, top, game.getSnakePositions());

    SDL_RenderPresent(renderer);
}


void interpretEvent(SDL_Event e, Game& game) //xét even để nhét vào inputqueue
{
    if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
        	case SDLK_UP: game.processUserInput(UP); break;
        	case SDLK_DOWN: game.processUserInput(DOWN); break;
        	case SDLK_LEFT: game.processUserInput(LEFT); break;
        	case SDLK_RIGHT: game.processUserInput(RIGHT); break;
        }
    }
}


void Menu(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, SDL_Surface* surface, SDL_Texture* texture)
{
    renderBackground(renderer);

    int Mx=0;
    int My=0;
    bool menu=true;
    int left = 0, top = 0;
    SDL_Event eMenu;

    SDL_Rect newgame;
    newgame.x=200;
    newgame.y=400;
    newgame.w=100;
    newgame.h=100;

    SDL_Rect Exit;
    Exit.x=600;
    Exit.y=400;
    Exit.w=100;
    Exit.h=100;
    SDL_RenderCopy(renderer, gallery->getImage(PIC_PLAYBUTTON) , NULL, &newgame);
    SDL_RenderCopy(renderer, gallery->getImage(PIC_EXITBUTTON) , NULL, &Exit);
    insertText (renderer, left, top, font, surface, texture, "SNAKEVSBLOCK", Position(90, 200));

	SDL_RenderPresent(renderer);

    while (menu)
    {
        SDL_Delay(10);
        if ( SDL_WaitEvent(&eMenu) == 0)
        {
            delete gallery;
            quitSDL(window, renderer);
        }
        if (eMenu.type == SDL_QUIT)
        {
            delete gallery;
            quitSDL(window, renderer);
        }
        if (eMenu.type == SDL_KEYDOWN && eMenu.key.keysym.sym == SDLK_ESCAPE)
        {
            delete gallery;
            quitSDL(window, renderer);
        }

        Mx=eMenu.button.x;
        My=eMenu.button.y;

        if (Mx >= newgame.x && Mx <= (newgame.x + newgame.w) && My >= newgame.y && My <= (newgame.y + newgame.h))
        {
            if (eMenu.type == SDL_MOUSEBUTTONDOWN) {
                if (eMenu.button.button == SDL_BUTTON_LEFT) {
                    menu = false;
                }
            }
        }
        if (Mx >= Exit.x && Mx <= (Exit.x + Exit.w) && My >= Exit.y && My <= (Exit.y + Exit.h))
        {
            if (eMenu.type == SDL_MOUSEBUTTONDOWN) {
                if (eMenu.button.button == SDL_BUTTON_LEFT) {
                    delete gallery;
                    quitSDL(window, renderer);
                }
            }
        }
    }
}


void WonGame(SDL_Renderer*renderer, SDL_Window*window,  TTF_Font* font, SDL_Surface* surface, SDL_Texture* texture, bool inGame)
{
    renderBackground(renderer);

    int Mx=0;
    int My=0;
    bool menu=true;
    int left = 0, top = 0;

    SDL_Event eMenu;

    SDL_Rect newgame;
    newgame.x=200;
    newgame.y=400;
    newgame.w=100;
    newgame.h=100;

    SDL_Rect Exit;
    Exit.x=600;
    Exit.y=400;
    Exit.w=100;
    Exit.h=100;

    SDL_Rect O;
    O.x=350;
    O.y=200;
    O.w=200;
    O.h=220;

    SDL_RenderCopy(renderer, gallery->getImage(PIC_RESTARTBUTTONWON) , NULL, &newgame);
    SDL_RenderCopy(renderer, gallery->getImage(PIC_EXITBUTTONWON) , NULL, &Exit);

    insertText (renderer, left, top, font, surface, texture, "YOU WIN", Position(250, 200));
    //SDL_RenderCopy(renderer, gallery->getImage(PIC_O) , NULL, &O);
	SDL_RenderPresent(renderer);

    while (menu)
    {
        SDL_Delay(10);
        if ( SDL_WaitEvent(&eMenu) == 0)
        {
            delete gallery;
            quitSDL(window, renderer);
        }
        if (eMenu.type == SDL_QUIT)
        {
            delete gallery;
            quitSDL(window, renderer);
        }
        if (eMenu.type == SDL_KEYDOWN && eMenu.key.keysym.sym == SDLK_ESCAPE)
        {
            delete gallery;
            quitSDL(window, renderer);
        }

        Mx=eMenu.button.x;
        My=eMenu.button.y;

        if (Mx >= newgame.x && Mx <= (newgame.x + newgame.w) && My >= newgame.y && My <= (newgame.y + newgame.h))
        {
            if (eMenu.type == SDL_MOUSEBUTTONDOWN) {
                if (eMenu.button.button == SDL_BUTTON_LEFT) {
                    menu = false;
                }
            }
        }
        if (Mx >= Exit.x && Mx <= (Exit.x + Exit.w) && My >= Exit.y && My <= (Exit.y + Exit.h))
        {
            if (eMenu.type == SDL_MOUSEBUTTONDOWN) {
                if (eMenu.button.button == SDL_BUTTON_LEFT) {
                    delete gallery;
                    quitSDL(window, renderer);
                }
            }
        }
    }
}


void RestartGame(SDL_Renderer* renderer, SDL_Window* window,  TTF_Font* font, SDL_Surface* surface, SDL_Texture* texture, bool inGame)
{
    renderBackground(renderer);

    int Mx=0;
    int My=0;
    bool menu=true;
    int left = 0, top = 0;

    SDL_Event eMenu;

    SDL_Rect newgame;
    newgame.x=200;
    newgame.y=400;
    newgame.w=100;
    newgame.h=100;

    SDL_Rect Exit;
    Exit.x=600;
    Exit.y=400;
    Exit.w=100;
    Exit.h=100;

    SDL_RenderCopy(renderer, gallery->getImage(PIC_RESTARTBUTTONOVER) , NULL, &newgame);
    SDL_RenderCopy(renderer, gallery->getImage(PIC_EXITBUTTONOVER) , NULL, &Exit);
    insertText (renderer, left, top, font, surface, texture, "YOU LOSE", Position(210, 200));

	SDL_RenderPresent(renderer);

    while (menu)
    {
        SDL_Delay(10);
        if ( SDL_WaitEvent(&eMenu) == 0)
        {
            delete gallery;
            quitSDL(window, renderer);
        }
        if (eMenu.type == SDL_QUIT)
        {
            delete gallery;
            quitSDL(window, renderer);
        }
        if (eMenu.type == SDL_KEYDOWN && eMenu.key.keysym.sym == SDLK_ESCAPE)
        {
            delete gallery;
            quitSDL(window, renderer);
        }

        Mx=eMenu.button.x;
        My=eMenu.button.y;

        if (Mx >= newgame.x && Mx <= (newgame.x + newgame.w) && My >= newgame.y && My <= (newgame.y + newgame.h))
        {
            if (eMenu.type == SDL_MOUSEBUTTONDOWN) {
                if (eMenu.button.button == SDL_BUTTON_LEFT) {
                    menu = false;
                }
            }
        }
        if (Mx >= Exit.x && Mx <= (Exit.x + Exit.w) && My >= Exit.y && My <= (Exit.y + Exit.h))
        {
            if (eMenu.type == SDL_MOUSEBUTTONDOWN) {
                if (eMenu.button.button == SDL_BUTTON_LEFT) {
                    delete gallery;
                    quitSDL(window, renderer);
                }
            }
        }
    }
}

