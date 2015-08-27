/*
_______ ______      _____                 ______  _____
___    |___  /_____ ___(_)_______ ___________  /_ ___(_)________
__  /| |__  / _  _ __  / __  __ __  ___/__  __ __  / __  ___/
_  ___ |_  /  /  __/_  /  _  / / /_(__  ) _  / / /_  /  _  /
/_/  |_|/_/   ___/ /_/   /_/ /_/ /____/  /_/ /_/ /_/   /_/

Este software contiene información de uso educativo propiedad que pertenece
a Michel Betancourt (Aleinshir). Toda la informacion aqui presente puede ser utilizada, modificada, y redistribuida
mientras se mantengan todas las referencias hacia el autor.
Copyright 2015 por Michel Betancourt.
*/

#ifndef RESISTOR_H
#define RESISTOR_H


#include "qmath.h"
#include "plusmath.h"
#include <qtablewidget.h>
#include <QMessageBox>
#include <qcombobox.h>

class Resistor
{

	public:
		Resistor(QTableWidget* table);
		float getValResistor(int res, int mul);
		float getResistorInPar(float v1, float v2 = 0, float v3 = 0);
		void getResistorDiv(int posR1[3][2], int posR2[3][2], int cntRes[2], float vVin, QString tVout, int stdar);
		void getResistor(float resis[6][192]);
		void makeResBox(int maxc);
		void setActCmBox(int,int);
		QStringList fillResistor(int index);
	private:
		const short int BASE = 0, MULTIPLICADOR = 1;
		float resistorSTD[6][192];
		void makeResistorStd();
		QStringList allResistorCommercial;
		int stdar = 0;
		QTableWidget *tb;
		int actCmBox[2];
		QComboBox *resCmBox[480];

};

#endif // RESISTOR_H
