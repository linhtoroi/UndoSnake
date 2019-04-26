#include <vector>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <chrono>
#include <ctime>
#include <map>
#include "Game.h"
#define CLOCK_NOW chrono::system_clock::now

using namespace std;

Game::Game(int _width, int _height)
    : width(_width), height(_height),
	  squares(_height, vector<CellType>(_width, CELL_EMPTY)),
      snake(*this, Position(_width/2, _height/2)),
      currentDirection(Direction::RIGHT),
      status(GAME_RUNNING)
{
	addScissors();
    addBlock();
}

Game::~Game()
{
    //dtor
}

Position Game::eraseBall(bool fact, Position pos)
{
        return pos;
}

void Game::snakeMoveTo(Position pos) {
	switch(getCellType(pos)) {
		case CELL_OFF_BOARD: // va vào tường
        case CELL_BLOCK:
		case CELL_SNAKE: status = GAME_OVER; break; // đâm vào chính nó
		case CELL_SCISSORS: snake.eatScissors(); addScissors();
		case CELL_BLOCK_BALL:snake.eatBlockBall(); //eraseBall(pos); // vào ô có scissors, ăn scissors và tạo scissors mss
		default:setCellType(pos, CELL_SNAKE);  // để gán phần tử squares[pos.x][pos.y] (có tên là pos) có giá trị là ô có SNAKE: squares=cell_snake
    }
}



void Game::snakeMoveToTO(SnakeNode* tail) {
	for (SnakeNode* p=tail; p!=nullptr; p=p->next)
         setCellType(p->position, CELL_SNAKE); // để gán phần tử squares[pos.x][pos.y] (có tên là pos) có giá trị là ô có SNAKE: squares=cell_snake
	 // để gán phần tử squares[pos.x][pos.y] (có tên là pos) có giá trị là ô có SNAKE: squares=cell_snake
}

void Game::snakeLeave(Position position)
{
	setCellType(position,CELL_EMPTY);
} // set ô empty vào mảng squares

void Game::processUserInput(Direction direction)
{
    inputQueue.push(direction); // đẩy input của người chơi vào mảng
}

bool Game::canChange(Direction current, Direction next) const {
	if (current == UP || current == DOWN)
		return next == LEFT || next == RIGHT;// để chuyển hướng
	return next == UP || next == DOWN;
}

Position Game::nextStep()
{
	while (!inputQueue.empty()) {
        Direction next = inputQueue.front();
        inputQueue.pop();
        if (canChange(currentDirection, next)) {
        	currentDirection = next; // nếu next là chuyển hướng thì chuyển
        	break;
		}
    }
    Position p = snake.move(currentDirection);// di chuyển
    return p;
}

void Game::setCellType(Position pos, CellType cellType)
{
	if (pos.isInsideBox(0, 0, width, height)) squares[pos.y][pos.x] = cellType;
} // để gán phần tử squares[pos.x][pos.y] (có tên là pos) có giá trị là ô có SNAKE: squares=cell_snake
// hoặc để gán những ô ko có rắn là empty (squares[pos.x][pos.y] (có tên là pos)) : squares=cell_empty

CellType Game::getCellType(Position pos) const
{
	return pos.isInsideBox(0, 0, width, height) ? squares[pos.y][pos.x] : CELL_OFF_BOARD;
} // xem là cell ntn

void Game::addScissors()
{
    do {
        Position p(rand() % width, rand() % height);// random vị trí scissors
        if (getCellType(p) == CELL_EMPTY) { // xem ô có phải empty ko
        	setCellType(p, CELL_SCISSORS);// đúng thì cho scissors vào
        	scissorsPosition = p; //chuyển pos của scissors
       		break;
        }
    } while (true);
}

void Game::addBlock()
{
    do {
        Position p(rand() % width, rand() % height);// random vị trí scissors
        if (getCellType(p) == CELL_EMPTY && getCellType(Position(p.x+1,p.y)) != CELL_HEAD &&
                getCellType(Position(p.x-1,p.y)) != CELL_HEAD && getCellType(Position(p.x,p.y-1)) != CELL_HEAD &&
                    getCellType(Position(p.x,p.y+1)) != CELL_HEAD)
        {
        	setCellType(p, CELL_BLOCK);// đúng thì cho scissors vào
        	blockPosition.push_back(p); //chuyển pos của scissors
       		//SDL_Delay(1000);
       		break;
        }
    } while (true);
}

int Game::randomNumberofBlock()
{
    srand(time(0));
    int s = rand()%5+1;
    return s;
}



void Game::addBlockBall()
{
    do {
        Position p(rand() % width, rand() % height);// random vị trí scissors
        if (getCellType(p) == CELL_EMPTY)
        {
        	setCellType(p, CELL_BLOCK_BALL);// đúng thì cho scissors vào
        	blockBallPosition.push_back(p); //chuyển pos của scissors
       		//SDL_Delay(1000);
       		break;
        }
    } while (true);
}



vector<Position> Game::getSnakePositions() const // lấy pos của snake
{
    return snake.getPositions();
}
