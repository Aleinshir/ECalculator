/*
_______ ______      _____                 ______  _____
___    |___  /_____ ___(_)_______ ___________  /_ ___(_)________
__  /| |__  / _  _ \__  / __  __ \__  ___/__  __ \__  / __  ___/
_  ___ |_  /  /  __/_  /  _  / / /_(__  ) _  / / /_  /  _  /
/_/  |_|/_/   \___/ /_/   /_/ /_/ /____/  /_/ /_/ /_/   /_/

Este software contiene información de uso educativo propiedad que pertenece
a Michel Betancourt (Aleinshir). Toda la informacion aqui presente puede ser utilizada, modificada, y redistribuida
mientras se mantengan todas las referencias hacia el autor.
Copyright 2015 por Michel Betancourt.
*/

#include "./Headers/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
