#include "QTsnake.h"
#include "defines.h"
#include <QtWidgets>

int main(int argc, char *argv[])
{
	QStringList paths = QCoreApplication::libraryPaths();
	paths.append(".");
	paths.append("imageformats");
	paths.append("mediaservice");
	paths.append("platforms");
	QCoreApplication::setLibraryPaths(paths);

	QApplication a(argc, argv);
	qRegisterMetaType<COORDINATES>("COORDINATES");
	qRegisterMetaType<objects>("BORDERS");
	qRegisterMetaType<objects>("objects");

	QTsnake w(Qt::Window | Qt::WindowCloseButtonHint);

	w.show();
	return a.exec();
}
