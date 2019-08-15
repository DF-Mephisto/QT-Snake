#include "QTlogic.h"

QTlogic::QTlogic(QObject* pobj) : QThread(pobj)
{
	levelType = 0;

	QObject::connect(this, SIGNAL(Crash()), SLOT(Exit()));
}

void QTlogic::CreateLevel()
{
	Game = true;
	CanMove = true;
	Pause = false;
	direct = Qt::Key_Right;
	delay = 300;

	FillLevelVectors(levelPlan[levelType]);
}

void  QTlogic::FillLevelVectors(const char levelPlan[10][11])
{
	snake.clear();
	wall.clear();

	COORDINATES c;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			level[i][j] = (objects)levelPlan[i][j];

			if ((objects)levelPlan[i][j] == wallBlock)
			{
				c.x = j;
				c.y = i;
				wall.push_back(c);
			}
			else if ((objects)levelPlan[i][j] == snakeBlock)
			{
				c.x = j;
				c.y = i;
				snake.push_back(c);
			}
		}
	}

	srand(time(NULL));
	food = CreateFood();
	level[food.y][food.x] = foodBlock;
}

COORDINATES QTlogic::defineCurBlock()
{
	COORDINATES c = snake[snake.size() - 1];

	switch (direct)
	{
	case Qt::Key_Up:
	{
		c.y -= 1;
		break;
	}
	case Qt::Key_Down:
	{
		c.y += 1;
		break;
	}
	case Qt::Key_Right:
	{
		c.x += 1;
		break;
	}
	case Qt::Key_Left:
	{
		c.x -= 1;
		break;
	}
	}

	return c;
}

bool QTlogic::CheckEdges(COORDINATES c)
{
	if (c.x >= 0 && c.x < 10 && c.y >= 0 && c.y < 10) return true;
	else return false;
}

COORDINATES QTlogic::CreateFood()
{
	vector<COORDINATES> emptyBlocks;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (level[i][j] == emptyBlock)
			{
				COORDINATES c = { j, i };
				emptyBlocks.push_back(c);
			}
		}
	}

	return emptyBlocks[rand() % emptyBlocks.size()];
}

void QTlogic::Move(COORDINATES c)
{
	if (!CheckEdges(c))
	{
		Game = false;

		return;
	}

	switch (level[c.y][c.x])
	{
	case emptyBlock:
	{
		emit drawBlock(snake[0], emptyBlock, BORDERS(false, false, false, false));

		level[snake[0].y][snake[0].x] = emptyBlock;
		level[c.y][c.x] = snakeBlock;

		for (int i = 0; i < snake.size() - 1; i++)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}

		snake[snake.size() - 1].x = c.x;
		snake[snake.size() - 1].y = c.y;

		RedrawSnake();

		break;
	}

	case snakeBlock:
	case wallBlock:
	{
		Game = false;

		break;
	}

	case foodBlock:
	{
		snake.insert(snake.end(), food);
		level[food.y][food.x] = snakeBlock;
		RedrawSnake();

		if (snake.size() < 100)
		{
			food = CreateFood();
			level[food.y][food.x] = foodBlock;
			emit drawBlock(food, foodBlock, BORDERS(false, false, false, false));
		}

		break;
	}
	}
}

void QTlogic::RedrawSnake()
{
	if (snake.size() > 1)
	{
		BORDERS b(true, true, true, true);
		COORDINATES c1 = { snake[snake.size() - 1].x, snake[snake.size() - 1].y };
		COORDINATES c2 = { snake[snake.size() - 2].x, snake[snake.size() - 2].y };
		if (c1.x > c2.x) b.left = false;
		if (c1.x < c2.x) b.right = false;
		if (c1.y > c2.y) b.top = false;
		if (c1.y < c2.y) b.bottom = false;

		emit drawBlock(snake[snake.size() - 1], snakeBlock, b);
	}
	else
	{
		emit drawBlock(snake[snake.size() - 1], snakeBlock, BORDERS(true, true, true, true));
	}

	if (snake.size() > 1)
	{
		BORDERS b(true, true, true, true);
		COORDINATES c1 = { snake[0].x, snake[0].y };
		COORDINATES c2 = { snake[1].x, snake[1].y };
		if (c1.x > c2.x) b.left = false;
		if (c1.x < c2.x) b.right = false;
		if (c1.y > c2.y) b.top = false;
		if (c1.y < c2.y) b.bottom = false;

		emit drawBlock(snake[0], snakeBlock, b);
	}

	if (snake.size() > 2)
	{
		BORDERS b(true, true, true, true);
		COORDINATES c1 = { snake[snake.size() - 2].x, snake[snake.size() - 2].y };
		COORDINATES c2 = { snake[snake.size() - 1].x, snake[snake.size() - 1].y };
		COORDINATES c3 = { snake[snake.size() - 3].x, snake[snake.size() - 3].y };

		if (c1.x > c2.x) b.left = false;
		if (c1.x < c2.x) b.right = false;
		if (c1.y > c2.y) b.top = false;
		if (c1.y < c2.y) b.bottom = false;

		if (c1.x > c3.x) b.left = false;
		if (c1.x < c3.x) b.right = false;
		if (c1.y > c3.y) b.top = false;
		if (c1.y < c3.y) b.bottom = false;

		emit drawBlock(snake[snake.size() - 2], snakeBlock, b);
	}
}

void QTlogic::Exit()
{
	levelType++;
	if (levelType == levelCount) levelType = 0;

	this->terminate();
}

void QTlogic::run()
{
	CreateLevel();

	emit drawBlock(snake[snake.size() - 1], snakeBlock, BORDERS(true, true, true, true));
	emit drawBlock(food, foodBlock, BORDERS(false, false, false, false));
	for (int i = 0; i < wall.size(); i++)
	{
		emit drawBlock(wall[i], wallBlock, BORDERS(false, false, false, false));
	}

	COORDINATES curStep = { 0, 0 };

	while (Game)
	{
		QThread::msleep(delay);
		while(Pause) QThread::msleep(100);
		
		curStep = defineCurBlock();
		Move(curStep);
		CanMove = true;
	}

	emit Crash();
}

void QTlogic::getKey(int k)
{
	if (k == Qt::Key_Escape)
	{
		emit Crash();
		return;
	}

	if (CanMove && !Pause)
	{
		switch (k)
		{
		case Qt::Key_Up:
		{
			if (direct != Qt::Key_Down)
				direct = k;
			break;
		}
		case Qt::Key_Down:
		{
			if (direct != Qt::Key_Up)
				direct = k;
			break;
		}
		case Qt::Key_Right:
		{
			if (direct != Qt::Key_Left)
				direct = k;
			break;
		}
		case Qt::Key_Left:
		{
			if (direct != Qt::Key_Right)
				direct = k;
			break;
		}
		}

		CanMove = false;
	}
}

void QTlogic::setPause()
{
	Pause = !Pause;
}
