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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ORO 0
#define NEGRO 1
#define MARRON 2
#define ROJO 3
#define NARANJA 4
#define AMARILLO 5
#define VERDE 6
#define AZUL 7
#define VIOLETA 8
#define GRIS 9
#define BLANCO 10
#define PLATA 11
#define NINGUNO 12

#include <QMainWindow>
#include <QMessageBox>
#include <QTime>
#include "qstring.h"
#include "resistor.h"
#include <qpushbutton.h>
#include "plusmath.h"
#include <qtablewidget.h>
#include <qcombobox.h>
#include <qstringlist.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
		void delay();
	private slots:

		void on_vin_textEdited(const QString &arg1);

		void on_vout_textEdited(const QString &arg1);

		void on_r1_activated(int index);

		void on_r1m_activated(int index);

		void on_r2_activated(int index);

		void on_r2m_activated(int index);

		void on_r1_2_activated(int index);

		void on_r1_2m_activated(int index);

		void on_r1_3_activated(int index);

		void on_r1_3m_activated(int index);

		void on_r2_2_activated(int index);

		void on_r2_2m_activated(int index);

		void on_r2_3_activated(int index);

		void on_r2_3m_activated(int index);


		void on_checkBox_clicked();

		void on_checkBox_3_clicked();

		void on_checkBox_2_clicked();

		void on_checkBox_4_clicked();

		void on_pushButton_2_clicked();

		void on_pushButton_3_clicked();

		void on_pushButton_clicked();

		void on_pushButton_4_clicked();

		void on_tabW_currentChanged(int index);

		void on_standar_currentIndexChanged(int index);

		void on_boxStandar_currentIndexChanged(int index);

	private:
		Ui::MainWindow *ui;

		// Funciones //
		void resetButton();
		void refresh(int i);
		void makeColor();
		void makeStandarBox();



		// Variables //
		int antOP, antantOP;
		const int VOUT = 2, VIN = 1, RES = 3, BASE = 0, MULTIPLICADOR = 1, ROW = 0, COLUMN = 1;
		void setIndex(int posR1[3][2], int posR2[3][2], int cntRes[2], float& vR1, float& vR2);
		int rowColumn[2];
		Resistor *res;
		QStringList colores;


};

#endif // MAINWINDOW_H
