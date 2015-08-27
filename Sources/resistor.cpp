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

#include "./Headers/resistor.h"

Resistor::Resistor(QTableWidget* table) :
	tb(table)
{
	actCmBox[0] = 3;
	actCmBox[1] = 3;
	for(int i = 0; i < 480; i++){
		resCmBox[i] = new QComboBox;
	}
	allResistorCommercial.clear();
	makeResistorStd();
	fillResistor(0);

}
void Resistor::setActCmBox(int index, int i){
	actCmBox[index] = i;
}


QStringList Resistor::fillResistor(int index)
{
	int valst = plusmath::powXN(6, index, 2);
	allResistorCommercial.clear();
	for(int i = 0; i < valst; i++){
		allResistorCommercial << QString::number(resistorSTD[index][i], 'f', index < 3 ? 1 : 2);
	}
	return allResistorCommercial;
}

float Resistor::getResistorInPar(float v1, float v2, float v3)
{
	return 1 / ((1.0/v1) + (1.0/v2) + (v3 != 0 ? 1.0/v3 : 0));
}

void Resistor::getResistorDiv(int posR1[3][2], int posR2[3][2], int cntRes[2], float vVin, QString tVout, int stdar)
{
	float tmpr = 0;											//	-Variable temporal				//
	this->stdar = stdar;
	float tmpr2 = 0;											//	-Variable temporal				//
	float result = 0;											//	-Variable temporal				//
	bool fail = false;											//	-Variable de escape de for		//
	int valst = plusmath::powXN(6, stdar, 2);
	unsigned int fal = 0;
	plusmath m;
	do{
		if(fal == 32767){
			fail = true;
		}
		tmpr = getValResistor(posR1[0][BASE], posR1[0][MULTIPLICADOR]);
		tmpr2 = getValResistor(posR2[0][BASE], posR2[0][MULTIPLICADOR]);

		switch (cntRes[0]){
		case 1:
			tmpr = getResistorInPar(tmpr, getValResistor(posR1[1][BASE], posR1[1][MULTIPLICADOR]));
			break;
		case 2:
			tmpr = getResistorInPar(tmpr,	getValResistor(posR1[1][BASE], posR1[1][MULTIPLICADOR]),
										getValResistor(posR1[2][BASE], posR1[2][MULTIPLICADOR]));
			break;
		}

		switch (cntRes[1]){
		case 1:
			tmpr2 = getResistorInPar(tmpr2, getValResistor(posR2[1][BASE], posR2[1][MULTIPLICADOR]));
			break;
		case 2:
			tmpr2 = getResistorInPar(tmpr2,	getValResistor(posR2[1][BASE], posR2[1][MULTIPLICADOR]),
										getValResistor(posR2[2][BASE], posR2[2][MULTIPLICADOR]));
			break;
		}

		result = (tmpr2 / (tmpr + tmpr2) * vVin);

		if(m.cmpDecimal(QString::number(result), tVout) == 2 && posR1[2][MULTIPLICADOR] < 6){
			/*if(m.cmpDecimal(QString::number(result), tVout, 1) == 2){
				cntRes[0]++;
				posR1[0][MULTIPLICADOR] = 0;
				posR1[0][BASE] = 0;

				posR2[0][BASE] = 0;
				posR2[0][MULTIPLICADOR] = 0;
			}*/
			posR1[0][BASE]++;
			posR1[0][BASE] == valst  ? posR1[0][BASE] = 0, posR1[0][MULTIPLICADOR]++ : 0;
			if (posR1[0][MULTIPLICADOR] > 6 && cntRes[0] == 0){
				cntRes[0]++;
				posR1[0][MULTIPLICADOR] = 0;
				posR1[0][BASE] = 0;

				posR2[0][BASE] = 0;
				posR2[0][MULTIPLICADOR] = 0;
			}
			if(cntRes[0] > 0 && posR1[0][MULTIPLICADOR] > 6){
				posR1[1][BASE]++;
				posR1[1][BASE] == valst ? posR1[1][BASE] = 0, posR1[1][MULTIPLICADOR]++ : 0;
				posR1[0][MULTIPLICADOR] = 0;
				posR1[0][BASE] = 0;

				posR2[0][BASE] = 0;
				posR2[0][MULTIPLICADOR] = 0;
			}
			if(posR1[1][MULTIPLICADOR] > 6 && cntRes[0] == 1){
				cntRes[0] = 2;
			}
			if(cntRes[0] > 1 && posR1[1][MULTIPLICADOR] > 6){
				posR1[2][BASE]++;
				posR1[2][BASE] == valst ? posR1[1][BASE] = 0, posR1[1][MULTIPLICADOR]++ : 0;
				posR1[1][MULTIPLICADOR] = 0;
				posR1[1][BASE] = 0;

				posR2[1][BASE] = 0;
				posR2[1][MULTIPLICADOR] = 0;
			}

		} else if (m.cmpDecimal(QString::number(result), tVout) == 0 && posR2[2][MULTIPLICADOR] < 6){
			/*if(m.cmpDecimal(QString::number(result), tVout, 1) == 0){
				cntRes[0]++;
				posR2[0][MULTIPLICADOR] = 0;
				posR2[0][BASE] = 0;

				posR1[0][BASE] = 0;
				posR1[0][MULTIPLICADOR] = 0;
			}*/
			posR2[0][BASE]++;
			posR2[0][BASE] == valst ? posR2[0][BASE] = 0, posR2[0][MULTIPLICADOR]++ : 0;
			if (posR2[0][MULTIPLICADOR] > 6 && cntRes[1] == 0){
				cntRes[1] = 1;
				posR2[0][MULTIPLICADOR] = 0;
				posR2[0][BASE] = 0;

				posR1[0][BASE] = 0;
				posR1[0][MULTIPLICADOR] = 0;
			}
			if(cntRes[1] > 0 && posR2[0][MULTIPLICADOR] > 6){
				posR2[1][BASE]++;
				posR2[1][BASE] == valst ? posR2[1][BASE] = 0, posR2[1][MULTIPLICADOR]++ : 0;
				posR2[0][MULTIPLICADOR] = 0;
				posR2[0][BASE] = 0;

				posR1[0][BASE] = 0;
				posR1[0][MULTIPLICADOR] = 0;
			}
			if(posR2[1][MULTIPLICADOR] > 6 && cntRes[1] == 1){
				cntRes[1] = 2;
			}
			if(cntRes[1] > 1 && posR2[1][MULTIPLICADOR] > 6){

				posR2[2][BASE]++;
				posR2[2][BASE] == valst ? posR2[2][BASE] = 0, posR2[2][MULTIPLICADOR]++ : 0;
				posR2[1][MULTIPLICADOR] = 0;
				posR2[1][BASE] = 0;

				posR1[1][BASE] = 0;
				posR1[1][MULTIPLICADOR] = 0;
			}

		}
		if (posR1[2][MULTIPLICADOR] > 6 || posR2[2][MULTIPLICADOR] > 6){
			fail = true;
		}
		fal++;
	}while(m.cmpDecimal(QString::number(result), tVout) != 1 && fail == false);

}

void Resistor::getResistor(float resis[6][192])
{
	for(int i = 0; i < 6; i++){
		for(int e = 0; e < 192; e++){
			resis[i][e] = resistorSTD[i][e];
		}
	}
}


void Resistor::makeResBox(int maxc)
{
	for(int i = 0; i < actCmBox[0]; i++){
		for(int z = 0; z < actCmBox[1]; z++){
			tb->setCellWidget(i,z,resCmBox[z+(i * maxc)]);
		}
	}


}


float Resistor::getValResistor(int base, int multiplicador){
	return allResistorCommercial.at(base).toFloat() * qPow(10, multiplicador);
}


void Resistor::makeResistorStd()
{
	int cnt = 6;
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < cnt; j++ ){
			resistorSTD[i][j] = qPow(10, static_cast<float>(j) / static_cast<float>(cnt));
		}
		cnt *= 2;
	}
}
