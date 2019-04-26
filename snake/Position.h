#ifndef POSITION_H
#define POSITION_H

enum Direction {
    UP = 0, DOWN, LEFT, RIGHT
};

struct Position
{
    int x;
    int y;

    Position(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}

    Position move(Direction direction) const {  // trả về pos ms
    	switch(direction) {
    		case UP: return Position(x, y - 1);
    		case DOWN: return Position(x, y + 1);
    		case LEFT: return Position(x - 1, y);
    		case RIGHT: return Position(x + 1, y);
		}
    }

    bool isInsideBox(int left, int top, int width, int height) const { // va tường chưa
        return x >= left && x < left+width && y >= top && y < top+height;
    }

    bool operator==(Position p) const {
        return x == p.x && y == p.y;
    }
};

#endif // POSITION_H
