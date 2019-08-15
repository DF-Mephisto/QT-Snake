#include "QTsnake.h"

QTsnake::QTsnake(Qt::WindowFlags f, QWidget *parent) : QWidget(parent)
{
	setWindowFlags(f);
	setFixedSize(500, 600);

	QPixmap back(":/QTsnake/sp");
	background = new QLabel(this);
	background->setPixmap(back);

	hints = new QLabel(this);
	hints->setGeometry(0, 500, 500, 100);
	hints->setText(
		"<style>"
		"TEX{"
		"font-family: Broadway;"
		"font-size: 35px;"
		"font-weight: bold;"
		"color: rgba(255, 255, 255, 100);"
		"}"
		"</style>"
		"<hr>"
		"<TEX>Esc = Stop    Sp = Pause</TEX>"    
		"<br>"
		"<TEX>R = Restart</TEX>"
	);
	hints->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

	QPixmap over(":/QTsnake/go");
	endgame = new QLabel(this);
	endgame->setPixmap(over);
	endgame->move((this->width() - over.width()) / 2, (this->height() - over.height()) / 2 - 50);
	endgame->hide();

	field = new QLabel**[10];
	for (int i = 0; i < 10; i++) field[i] = new QLabel*[10];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			field[i][j] = new QLabel(this);
			field[i][j]->setGeometry(j * 50, i * 50, 50, 50);
			field[i][j]->hide();
		}
	}

	mainThread = new QTlogic(this);
	QObject::connect(mainThread, SIGNAL(drawBlock(COORDINATES, objects, BORDERS)), this, SLOT(draw(COORDINATES, objects, BORDERS)));
	QObject::connect(this, SIGNAL(setKey(int)), mainThread, SLOT(getKey(int)));
	QObject::connect(mainThread, SIGNAL(Crash()), this, SLOT(GameOver()));
	QObject::connect(this, SIGNAL(setPause()), mainThread, SLOT(setPause()));

	mainThread->start();
	LoadMusic();
}

void QTsnake::LoadMusic()
{
	playlist = new QMediaPlaylist();
	playlist->addMedia(QUrl("qrc:/QTsnake/audio"));
	playlist->setPlaybackMode(QMediaPlaylist::Loop);

	player = new QMediaPlayer;
	player->setPlaylist(playlist);
	player->play();
}

void QTsnake::draw(COORDINATES c, objects obj, BORDERS b)
{
	QPalette pal;

	switch (obj)
	{
	case emptyBlock:
	{
		field[c.y][c.x]->hide();
		field[c.y][c.x]->setStyleSheet(makeBlockStyle(b));
		return;
	}

	case snakeBlock:
	{
		pal.setColor(this->backgroundRole(), Qt::green);
		field[c.y][c.x]->setStyleSheet(makeBlockStyle(b));
		break;
	}

	case foodBlock:
	{
		pal.setColor(this->backgroundRole(), Qt::blue);
		field[c.y][c.x]->setStyleSheet(makeBlockStyle(b));
		break;
	}

	case wallBlock:
	{
		pal.setColor(this->backgroundRole(), Qt::gray);
		field[c.y][c.x]->setStyleSheet(makeBlockStyle(b));
		break;
	}
	}

	field[c.y][c.x]->setPalette(pal);
	field[c.y][c.x]->setAutoFillBackground(true);
	field[c.y][c.x]->show();
}

QString QTsnake::makeBlockStyle(BORDERS b)
{
	QString borders = "border-width: ";
	if (b.top) borders += "3px "; else borders += "0px ";
	if (b.right) borders += "3px "; else borders += "0px ";
	if (b.bottom) borders += "3px "; else borders += "0px ";
	if (b.left) borders += "3px;"; else borders += "0px;";

	borders += " border-style: solid; border-color: black;";
	return borders;
}

void QTsnake::GameOver()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			field[i][j]->hide();
		}
	}
	endgame->show();
}

void QTsnake::keyPressEvent(QKeyEvent *event)
{
	switch (int k = event->key())
	{
	case Qt::Key_Escape:
	{
		GameOver();
	}
	case Qt::Key_Up:
	case Qt::Key_Down:
	case Qt::Key_Right:
	case Qt::Key_Left:
	{
		emit setKey(k);
		break;
	}
	case Qt::Key_R:
	case 1050:
	{
		endgame->hide();
		mainThread->start();
		break;
	}
	case Qt::Key_Space:
	{
		emit setPause();
		break;
	}

	default:
		QWidget::keyPressEvent(event);
		break;
	}
}
