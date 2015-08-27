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

#ifndef PLUSMATH_H
#define PLUSMATH_H

#include <QString>
#include<QLineEdit>

typedef struct{
	int enteros;
	int decimales;
}NUMBER;

class plusmath
{

	public:
		plusmath();
		~plusmath();
		static int cmpDecimal(QString val1, QString val2, int precision = 0);
		static NUMBER getCntDig(float number);
		static bool isAcceptValue(float);
		static int powXN(int base, int cnt, int nMult);
};

#endif // PLUSMATH_H
