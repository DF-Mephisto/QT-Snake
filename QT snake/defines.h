#pragma once

struct COORDINATES
{
	int x;
	int y;
};
Q_DECLARE_METATYPE(COORDINATES)

struct BORDERS
{
	BORDERS() {};
	BORDERS(bool l, bool r, bool t, bool b) : left(l), right(r), top(t), bottom(b) {};

	bool left;
	bool right;
	bool top;
	bool bottom;
};
Q_DECLARE_METATYPE(BORDERS)

enum objects
{
	emptyBlock = ' ',
	snakeBlock = '1',
	foodBlock = '2',
	wallBlock = '3'
};
Q_DECLARE_METATYPE(objects)