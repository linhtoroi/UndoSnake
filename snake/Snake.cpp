#include "Snake.h"
#include "Game.h"
#include <iostream>
const int BW=30;
const int BH=20;
const int first_Body = 5;
//SnakeNode* taill(Position start,  follow = Position (start.x, start.y)
//    SnakeNode * nNode= new SnakeNode(follow, nullptr);
//    tail->next=nNode;
//    tail=nNode;

SnakeNode* Snake::firstBody(Position start, SnakeNode* head)
{
    for (int i=0; i < first_Body; i++)
    {
        SnakeNode * nNode= new SnakeNode(Position(head->position.x, head->position.y+1), nullptr);
        head->next=nNode;
        head=nNode;
    }
    return head;
}

Snake::Snake(Game& _game, Position start)
     :head(new SnakeNode(start)),
     tail(head),
     game(_game),
     scissors(0), blockNumber(0)


{
    head=firstBody(start,head);
    //game.snakeMoveTo(start);
    game.snakeMoveToTO(tail);
    game.setCellType(head->position, CELL_HEAD);////////////////////..........................
}


Snake::~Snake()
{

}

vector<Position> Snake::getPositions() const
{
    vector<Position> res;
    for (SnakeNode* p = tail; p != nullptr; p = p->next)
        res.push_back(p->position);
    return res;
}

void Snake::growAtFront(Position newPosition)
{
    game.setCellType(head->position, CELL_SNAKE);
	head->next = new SnakeNode(newPosition);
	head = head->next;
	game.setCellType(head->position, CELL_HEAD);
}
SnakeNode* Snake::cutTail()
{
    SnakeNode *oldTailNode = tail;
    tail=tail->next;
    delete oldTailNode;
    return tail;
}

SnakeNode* Snake::growAtLast(Position newPosition)
{
        SnakeNode* Node = new SnakeNode(newPosition, tail);
        tail = Node;
        return tail;
}

void Snake::slideTo(Position newPosition)
{
	if (tail->next == nullptr) {
		tail->position = newPosition;
	}
	else {
		SnakeNode *oldTailNode = tail;

		tail = tail->next;
		oldTailNode->next = nullptr;

        game.setCellType(head->position, CELL_SNAKE);

		oldTailNode->position = newPosition;
		head->next = oldTailNode;
		head = oldTailNode;
		game.setCellType(head->position, CELL_HEAD);
	}
}

void Snake::eatScissors()
{
	scissors++;
}

void Snake::eatBlockBall()
{
	blockNumber+= game.randomNumberofBlock();
}
Position Snake::move(Direction direction)
{
    Position newPosition = head->position.move(direction); // tạo pos ms
    game.snakeMoveTo(newPosition); // để xem vị trí tương quan của rắn so vs các ô -->> gán giá trị có rắn hay ko vào mảng squares
    if (tail->next==NULL)
    {
        game.status = GAME_WON;
        return Position(-1,-2);
    }
    if (game.isGameOver()) return Position(-1, -2); // soát xem status xem có va vào j hay ko

    if (scissors > 0)
    {
        scissors--;
        game.snakeLeave(tail->position); //chuyển pos của tail cũ thành empty trong squares
        slideTo(newPosition);
        cutTail();
        game.snakeLeave(tail->position);
        blockNumber=0;
    }
    else if (blockNumber>0)
    {
        growAtFront(newPosition);
        blockNumber--;

        if (tail->position.x == tail->next->position.x)
        {
            if (BW - tail->position.x>=blockNumber)
            {
                for (int i=0; i<blockNumber; i++)
                    tail=growAtLast(Position(tail->position.x+1, tail->position.y));
                //game.snakeMoveToTO(tail);
            } else {
                int sub = BW - tail->position.x;
                for (int i=0; i<sub; i++)
                    tail=growAtLast(Position(tail->position.x+1, tail->position.y));
                for (int i=sub; i<blockNumber; i++)
                    tail=growAtLast(Position(tail->position.x, tail->position.y+1));
                //game.snakeMoveToTO(tail);
            }
        } else {
            if (BH - tail->position.y>=blockNumber)
            {
                for (int i=0; i<blockNumber; i++)
                    tail=growAtLast(Position(tail->position.x, tail->position.y+1));
                //game.snakeMoveToTO(tail);
            } else {
                int sub = BH - tail->position.y;
                for (int i=0; i<sub; i++)
                    tail=growAtLast(Position(tail->position.x, tail->position.y+1));
                for (int i=sub; i<blockNumber; i++)
                    tail=growAtLast(Position(tail->position.x+1, tail->position.y));
                //game.snakeMoveToTO(tail);
                }
        }
        blockNumber=0;
        return newPosition;
    }
    else
    {
    	game.snakeLeave(tail->position); //chuyển pos của tail cũ thành empty trong squares
        slideTo(newPosition);
    }
    return Position(-1,-1);
}

//Position Snake::check(Direction direction)
//{
//    //return move(direction);
//}
