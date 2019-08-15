#pragma once

#include <QtWidgets>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "defines.h"
#include "QTlogic.h"


class QTsnake : public QWidget
{
	Q_OBJECT

public:
	QTsnake(Qt::WindowFlags f = 0, QWidget *parent = Q_NULLPTR);

private:
	QLabel* background;
	QLabel* endgame;
	QLabel*** field;

	QLabel* hints;

	QMediaPlayer* player;
	QMediaPlaylist *playlist;

	QTlogic* mainThread;

	void LoadMusic();
	QString makeBlockStyle(BORDERS b);

public slots:
	virtual void keyPressEvent(QKeyEvent *event);
	void draw(COORDINATES c, objects obj, BORDERS b);
	void GameOver();

signals:
	void setKey(int);
	void setPause();

};
