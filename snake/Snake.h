#ifndef SNAKE_H
#define SNAKE_H

#include <queue>
#include <vector>
#include "Position.h"

using namespace std;

class Game;

struct SnakeNode
{
    Position position;
    SnakeNode *next;
    SnakeNode(Position p, SnakeNode* _next = nullptr) : position(p), next(_next) {}
};

class Snake
{

    SnakeNode *head,*tail;
    Game& game;
    int scissors;
    int blockNumber;
   // int score;
public:
    Snake(Game& _game, Position start);
    ~Snake();
    SnakeNode* firstBody(Position start,SnakeNode* head);
    Position move(Direction direction);
    bool checkPosition(Position pos);
    vector<Position> getPositions() const;
    void eatScissors();
    void eatBlockBall();
   // Position check(Direction direction);

private:
    void slideTo(Position newPosition);
    void growAtFront(Position newPosition);
    SnakeNode* cutTail();
    SnakeNode* growAtLast(Position newPosition);
};

#endif // SNAKE_H
