#ifndef GAME_H
#define GAME_H
#include <cmath>
#include <chrono>
#include <ctime>
#include <map>
#include "Snake.h"
#include "Position.h"
#define CLOCK_NOW chrono::system_clock::now
enum GameStatus {
    GAME_RUNNING = 1,
    GAME_STOP = 2,
    GAME_WON = 4 | GAME_STOP,
    GAME_OVER = 8 | GAME_STOP,
};

enum CellType {
    CELL_EMPTY = 0, CELL_SNAKE, CELL_SCISSORS, CELL_OFF_BOARD, CELL_BLOCK, CELL_BLOCK_BALL, CELL_HEAD
};

class Game
{
    std::vector< std::vector<CellType> > squares;
    Snake snake;

    std::queue<Direction> inputQueue;
    Direction currentDirection;
    Position scissorsPosition;
    vector <Position> blockPosition;
    vector <Position> blockBallPosition;
    //map <Position, int> blockBallPositionAndNumber;
//    vector <int> numberOfBlock;
    int numberOfBlock;

public:
    GameStatus status;
	const int width;
	const int height;

    Game(int _width, int _height);
    ~Game();

    bool isGameRunning() const { return status == GAME_RUNNING; }
    bool isGameOver() const { return status == GAME_OVER; }
    bool isGameWon() const {return status == GAME_WON;}
    void processUserInput(Direction direction);
    Position nextStep();
    const std::vector< std::vector<CellType> >& getSquares() const { return squares; }
    CellType getCellType(Position p) const;
    void setGameStatus(GameStatus status);

    std::vector<Position> getSnakePositions() const;
    Position getScissorsPosition() const { return scissorsPosition; }
    vector <Position> getBlockPosition() const { return blockPosition; }
    vector <Position> getBlockBallPosition() const { return blockBallPosition; }


    //void changePos(map <Position, int> blockBallPosition, vector (Position) blockBallPosition);


    //map <Position, int> getBlockPositionAndNumber() const { return blockBallPositionAndNumber;}
    bool canChange(Direction current, Direction next) const;

    void snakeMoveTo(Position position);
    void snakeMoveToTO(SnakeNode* head);
    void snakeLeave(Position position);
    void addBlock();
    void addBlockBall();
    void setCellType(Position pos, CellType cellType);
    int  randomNumberofBlock();
    Position eraseBall(bool fact, Position pos);
//    vector <int>
//    vector <int> returnNumberOfBlock()const{return numberOfBlock;}
private:
	void addScissors();


};

#endif // GAMEGROUND_H
