#pragma once

#include <QtWidgets>
#include <vector>
#include <ctime>
#include "defines.h"
#include "levelInfo.h"


using namespace std;

class QTlogic : public QThread
{
	Q_OBJECT

private:
	objects level[10][10];
	vector<COORDINATES> snake;
	vector<COORDINATES> wall;
	COORDINATES food;

	bool Game;
	bool CanMove;
	bool Pause;
	int direct;
	int delay;
	int levelType;

	COORDINATES defineCurBlock();
	COORDINATES CreateFood();
	bool CheckEdges(COORDINATES c);
	void Move(COORDINATES c);
	void CreateLevel();
	void FillLevelVectors(const char levelPlan[10][11]);
	void RedrawSnake();

public:
	QTlogic(QObject* pobj = nullptr);

	void run();

public slots:
	void getKey(int k);
	void setPause();
	void Exit();

signals:
	void drawBlock(COORDINATES, objects, BORDERS);
	void Crash();
};