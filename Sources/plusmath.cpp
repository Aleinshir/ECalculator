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

#include "./Headers/plusmath.h"

plusmath::plusmath()
{

}

plusmath::~plusmath()
{

}

int plusmath::cmpDecimal(QString val1, QString val2, int precision)
{
	int cnt = 0;
	int cnt2 = 0;
	int enteroA = 0, enteroB = 0;
	for(int i = 0, b = 0; i < val2.length(); i ++){

		b == 1 ? cnt++ : 0;
		val2.at(i) == '.' || b == 1 ? b = 1 : enteroA++;
	}
	for(int i = 0, b = 0; i < val1.length(); i ++){
		b == 1 ? cnt2++ : 0;
		val1.at(i) == '.' || b == 1 ? b = 1 : enteroB++;
	}
	if(cnt2 < cnt){
		return 2;
	}
	if (cnt == 0){
		val2.append(".0");
		cnt++;
	}
	for(int i = 0, b = 0, antcnt = 0; i < val1.length() && antcnt < cnt; i ++){
		if(b == 0){
			if(val1.at(i) != val2.at(i)){
				if(enteroA != enteroB){
					if (enteroB < enteroA){
						return 0;
					} else {
						return 2;
					}
				} else if(val1.at(i) < val2.at(i)){
					return 0;
				} else{
					return 2;
				}
			}
			if(val1.at(i) == '.'){b = 1;}
		} else {
			if(val1.at(i) != val2.at(i)){
				if(val1.at(i) < val2.at(i)){
					return 0;
				} else{
					return 2;
				}
			}
			antcnt ++;
		}
	}
	return 1;
}

NUMBER plusmath::getCntDig(float number)
{
	int decimal = 0;
	int enteros = 0;
	NUMBER valreturn;
	QString val1 = QString::number(number);
	for(int i = 0, b = 0; i < val1.length(); i ++){
		b == 1 ? decimal++ : 0;
		val1.at(i) == '.' || b == 1 ? b = 1 : enteros++;
	}
	valreturn.decimales = decimal;
	valreturn.enteros = enteros;
	return valreturn;
}

bool plusmath::isAcceptValue(float prueba){

	QString val1 = QString::number(prueba);

	for(int i = 0; i < val1.length(); i ++){
		if(val1.at(i) != '.' && val1.at(i) > 0){
			return true;
		}
	}
	return false;

}

int plusmath::powXN(int base, int cnt, int nMult)
{
	for(int i = 0; i < cnt; i++){
		base *= nMult;
	}
	return base;
}

