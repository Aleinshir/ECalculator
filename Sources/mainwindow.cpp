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
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	//--------------- Resistencias ---------------//
	// Divisor de voltaje //
	antOP = 0;
	antantOP = 0;
	makeColor();
	// Resistencias P/S //
	rowColumn[ROW] = 3;
	rowColumn[COLUMN] = 3;
	res = new Resistor(ui->tableWidget);
	resetButton();
	res->makeResBox(rowColumn[COLUMN]);
	// Resistencias comerciales //

}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::delay()
{
	QTime dieTime= QTime::currentTime().addSecs(1);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void MainWindow::refresh(int i)
{
	//------------ Declaracion de Variables ------------//		//------------------------------------------//
	float vVin = ui->vin->text().toFloat();						//	-Voltage de entrada						//
	QString tVout = ui->vout->text();
	QString tmp;
	tVout == "" ? tVout = "0" : 0;
	float vR1 = 0, vR2 = 0;										//	-Resistencia top y bottom sus totales	//
	float val = 0;												//	-Variable temporal						//
	int cntRes[2] = {0,0};										//	-Cantidad de resistencias en paralelo	//
	float vVout = ui->vout->text().toFloat();					//	-Voltage de salida						//
	float tempr = 0, tempr2 = 0, tempr3 = 0;					//	-Variables temporales					//
	int posR1[3][2] = {0}, posR2[3][2] = {0};					//	-Resistencia a probar					//
	//-------------- Fin de declaracion ----------------//		//------------------------------------------//

	tempr = res->getValResistor(ui->r1->currentIndex(), ui->r1m->currentIndex());		//---------------------------//
	tempr2 = res->getValResistor(ui->r1_2->currentIndex(), ui->r1_2m->currentIndex());	//	Valores temporales r1    //
	tempr3 = res->getValResistor(ui->r1_3->currentIndex(), ui->r1_3m->currentIndex());	//---------------------------//


	if(!ui->r1_2->isEnabled() && !ui->r1_3->isEnabled()){							//---------------------------//
		vR1 = tempr;																//	Comprueba si en R1 todas //
	} else if(ui->r1_2->isEnabled() && ui->r1_3->isEnabled()){						//	estan desmarcadas de     //
		vR1 = res->getResistorInPar(tempr, tempr2, tempr3);								//	manera que solo calcula  //
	} else if(!ui->r1_2->isEnabled() && ui->r1_3->isEnabled()){						//	las marcadas por el      //
		vR1 = res->getResistorInPar(tempr, tempr3);										//	usuario                  //
	} else {																		//---------------------------//
		vR1 = res->getResistorInPar(tempr, tempr2);
	}


	tempr = res->getValResistor(ui->r2->currentIndex(), ui->r2m->currentIndex());		//---------------------------//
	tempr2 = res->getValResistor(ui->r2_2->currentIndex(), ui->r2_2m->currentIndex());	//	Valores temporales r2    //
	tempr3 = res->getValResistor(ui->r2_3->currentIndex(), ui->r2_3m->currentIndex());	//---------------------------//


	if(!ui->r2_2->isEnabled() && !ui->r2_3->isEnabled()){
		vR2 = tempr;																//---------------------------//
	} else if(ui->r2_2->isEnabled() && ui->r2_3->isEnabled()){						//	Comprueba si en R2 todas //
		vR2 = res->getResistorInPar(tempr, tempr2, tempr3);								//	estan desmarcadas de     //
	} else if(!ui->r1_2->isEnabled() && ui->r1_3->isEnabled()){						//	manera que solo calcula  //
		vR2 = res->getResistorInPar(tempr, tempr3);										//	las marcadas por el      //
	} else {																		//	usuario                  //
		vR2 = res->getResistorInPar(tempr, tempr2);										//---------------------------//
	}


	if((i == VIN || i == RES) && (antOP == VIN || antOP == RES)){							//	Si lo ultimo en pulsar   //
		val = (vR2 / ( vR1 + vR2 )) * vVin;													//	fue vVin y R			 //
		ui->vout->setText(QString::number(val));
	} else if ((i == VIN || i == VOUT) && (antOP == VOUT || antOP == VIN) && !tVout.endsWith('.')){
		if(plusmath::cmpDecimal(tVout, QString::number(vVin)) == 2 || !plusmath::isAcceptValue(vVout)){			// Si vVout no es aceptable
			return;																								// o vout es mayor que vin
		}
		ui->checkBox->setChecked(false);
		ui->checkBox_2->setChecked(false);
		ui->checkBox_3->setChecked(false);
		ui->checkBox_4->setChecked(false);

		if(plusmath::getCntDig(vVout).decimales > 2 + ui->standar->currentIndex()){
			tmp = ui->vout->text();
			tmp.remove(ui->vout->text().length() - 1, 1);
			ui->vout->setText(tmp);

			return;
		}

		res->getResistorDiv(posR1, posR2, cntRes, vVin, tVout, ui->standar->currentIndex());

		setIndex(posR1, posR2, cntRes, vR1, vR2);

		val = (vR2 / ( vR1 + vR2 )) * vVin;
	} else if ((i == RES || i == VOUT) && (antOP == RES || antOP == VOUT)){ // Si lo ultimo fue una resistencia y vout calcular vin

		val = (vVout * ( vR1 + vR2 )) / vR2;
		ui->vin->setText(QString::number(val));
		vVin = val;
		val = (vR2 / ( vR1 + vR2 )) * val;
	}
	QString numberAmperios;
	ui->comboBox_2->clear();
	float escal = val / vR2;
	if(vVin > 0.0 && val > 0.0)
	for (int i = 1, escala = 0; i > 0 ; i *= 10){

		if(escal < 1){
			escal *= 1000;
			escala < 5 ? escala++ : escala;
		} else if (escal > 999){
			escal = numberAmperios.toFloat();
			escal /= 1000;
			escala > -1 ? escala-- : escala;
		}
		numberAmperios = QString::number(escal, 'f', 2);

		switch (escala) {
		case 0:												// Coloca la letra a corriente
			numberAmperios.append(" A");
			break;
		case 1:
			numberAmperios.append(" mA");
			break;
		case 2:
			numberAmperios.append(" uA");
			break;
		case 3:
			numberAmperios.append(" nA");
			break;
		case 4:
			numberAmperios.append(" pA");
			break;
		case -1:
			numberAmperios.append(" KA");
			break;
		case -2:
			numberAmperios.append(" MA");
			break;
		}

		ui->comboBox_2->addItem( numberAmperios );
		if(vR1 * i < 10000000 && vR2 * i < 10000000){
			vR1 *= 10;
			vR2 *= 10;
			escal /= 10;
		} else {
			i = -1;
		}
	} else {
		ui->comboBox_2->addItem("0");
	}

	resetButton();

}

void MainWindow::setIndex(int posR1[3][2], int posR2[3][2], int cntRes[2], float &vR1, float &vR2)
{
	ui->r1->setCurrentIndex(posR1[0][BASE]);
	ui->r1m->setCurrentIndex(posR1[0][MULTIPLICADOR]);
	ui->r2->setCurrentIndex(posR2[0][BASE]);
	ui->r2m->setCurrentIndex(posR2[0][MULTIPLICADOR]);

	switch(cntRes[0]){
	case 0:
		vR1 = res->getValResistor(posR1[0][BASE], posR1[0][MULTIPLICADOR]);
		break;
		case 1:
			ui->r1_2->setEnabled(true);
			ui->r1_2m->setEnabled(true);
			ui->checkBox->setChecked(true);
			ui->r1_2->setCurrentIndex(posR1[1][BASE]);
			ui->r1_2m->setCurrentIndex(posR1[1][MULTIPLICADOR]);
			vR1 = res->getResistorInPar(res->getValResistor(posR1[0][BASE], posR1[0][MULTIPLICADOR]),
					res->getValResistor(posR1[1][BASE], posR1[1][MULTIPLICADOR]));
			break;
		case 2:
			ui->r1_2->setEnabled(true);
			ui->r1_2m->setEnabled(true);
			ui->r1_3->setEnabled(true);
			ui->r1_3m->setEnabled(true);
			ui->checkBox->setChecked(true);
			ui->checkBox_2->setChecked(true);
			ui->r1_2->setCurrentIndex(posR1[1][BASE]);
			ui->r1_2m->setCurrentIndex(posR1[1][MULTIPLICADOR]);
			ui->r1_3->setCurrentIndex(posR2[2][BASE]);
			ui->r1_3m->setCurrentIndex(posR2[2][MULTIPLICADOR]);
			vR1 = res->getResistorInPar(res->getValResistor(posR1[0][BASE], posR1[0][MULTIPLICADOR]),
					res->getValResistor(posR1[1][BASE], posR1[1][MULTIPLICADOR]),
					res->getValResistor(posR1[2][BASE], posR1[2][MULTIPLICADOR]));
			break;
	}


	switch(cntRes[1]){
	case 0:
		vR2 = res->getValResistor(posR2[0][BASE], posR2[0][MULTIPLICADOR]);
		break;
		case 1:
			ui->r2_2->setEnabled(true);
			ui->r2_2m->setEnabled(true);
			ui->checkBox_3->setChecked(true);
			ui->r2_2->setCurrentIndex(posR1[1][BASE]);
			ui->r2_2m->setCurrentIndex(posR1[1][MULTIPLICADOR]);
			vR2 = res->getResistorInPar(res->getValResistor(posR2[0][BASE], posR2[0][MULTIPLICADOR]),
					res->getValResistor(posR2[1][BASE], posR2[1][MULTIPLICADOR]));
			break;
		case 2:
			ui->r2_2->setEnabled(true);
			ui->r2_2m->setEnabled(true);
			ui->r2_3->setEnabled(true);
			ui->r2_3m->setEnabled(true);
			ui->checkBox_3->setChecked(true);
			ui->checkBox_4->setChecked(true);
			ui->r2_2->setCurrentIndex(posR1[1][BASE]);
			ui->r2_2m->setCurrentIndex(posR1[1][MULTIPLICADOR]);
			ui->r2_3->setCurrentIndex(posR2[2][BASE]);
			ui->r2_3m->setCurrentIndex(posR2[2][MULTIPLICADOR]);
			vR2 = res->getResistorInPar(res->getValResistor(posR2[0][BASE], posR2[0][MULTIPLICADOR]),
					res->getValResistor(posR2[1][BASE], posR2[1][MULTIPLICADOR]),
					res->getValResistor(posR2[2][BASE], posR2[2][MULTIPLICADOR]));
			break;
	}
}



void MainWindow::on_vin_textEdited(const QString &arg1)
{

	antOP == 1 ? antOP = antantOP : arg1.isNull();
	refresh(1);
	antOP == 1 ? antOP = antantOP : antantOP = antOP, antOP = 1;
}

void MainWindow::on_vout_textEdited(const QString &arg1)
{
	antOP == 2 ? antOP = antantOP : arg1.isNull();
	refresh(2);
	antOP == 2 ? antOP = antantOP : antantOP = antOP, antOP = 2;
}

void MainWindow::on_r1_activated(int index)
{
	antOP == 3 ? antOP = antantOP : index;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_r1m_activated(int index)
{
	antOP == 3 ? antOP = antantOP : index;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_r2_activated(int index)
{
	antOP == 3 ? antOP = antantOP : index;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_r2m_activated(int index)
{
	antOP == 3 ? antOP = antantOP : index;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_r1_2_activated(int index)
{
	antOP == 3 ? antOP = antantOP : index;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_r1_2m_activated(int index)
{
	antOP == 3 ? antOP = antantOP : index;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_r1_3_activated(int index)
{
	antOP == 3 ? antOP = antantOP : index;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_r1_3m_activated(int index)
{
	antOP == 3 ? antOP = antantOP : index;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_r2_2_activated(int index)
{
	antOP == 3 ? antOP = antantOP : index;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_r2_2m_activated(int index)
{
	antOP == 3 ? antOP = antantOP : index;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_r2_3_activated(int index)
{
	antOP == 3 ? antOP = antantOP : index;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_r2_3m_activated(int index)
{
	antOP == 3 ? antOP = antantOP : index;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}


void MainWindow::on_checkBox_clicked()
{
	antOP == 3 ? antOP = antantOP : 0;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_checkBox_3_clicked()
{
	antOP == 3 ? antOP = antantOP : 0;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_checkBox_2_clicked()
{
	antOP == 3 ? antOP = antantOP : 0;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}

void MainWindow::on_checkBox_4_clicked()
{
	antOP == 3 ? antOP = antantOP : 0;
	refresh(3);
	antOP == 3 ? antOP = antantOP : antantOP = antOP, antOP = 3;
}


void MainWindow::on_pushButton_3_clicked()
{
	if(rowColumn[COLUMN] > 3){

		rowColumn[COLUMN]--;
		rowColumn[COLUMN] > 4  ? setMinimumWidth(887 - ((15 - rowColumn[COLUMN]) * 50)) : 0;
		setMaximumWidth(887 - ((15 - rowColumn[COLUMN]) * 50));
		ui->tableWidget->setColumnCount(rowColumn[COLUMN]);
	}
}

void MainWindow::on_pushButton_clicked()
{
	if(rowColumn[COLUMN] < 24){

		rowColumn[COLUMN]++;
		rowColumn[COLUMN] > 4 ? setMinimumWidth(387 + ((rowColumn[COLUMN] - 5) * 50)) : 0;
		ui->tableWidget->setColumnCount(rowColumn[COLUMN]);
	}
	ui->tableWidget->clear();
	res->setActCmBox(COLUMN, rowColumn[COLUMN]);
	res->makeResBox(rowColumn[COLUMN]);

}


void MainWindow::on_pushButton_4_clicked()
{
	if(rowColumn[ROW] < 20){

		rowColumn[ROW]++;
		rowColumn[ROW] > 13 ? setMinimumHeight(maximumHeight() + ((rowColumn[ROW] - 13) * 25)) : 0;
		ui->tableWidget->setRowCount(rowColumn[ROW]);

	}
}

void MainWindow::on_pushButton_2_clicked()
{
	if(rowColumn[ROW] > 3){

		rowColumn[ROW]--;
		rowColumn[ROW] > 13 ? setMinimumHeight((maximumHeight() + 300) - ((13 - rowColumn[ROW]) * 25)) : 0;
		setMaximumHeight(827 - ((13 - rowColumn[ROW]) * 25));
		ui->tableWidget->setRowCount(rowColumn[ROW]);
	}
}

void MainWindow::on_tabW_currentChanged(int index)
{
	switch (index){
	case 1:
		setMinimumSize(387, 527);
		setMaximumSize(387, 527);
		ui->tableWidget->clear();
		ui->tableWidget->setColumnCount(3);
		ui->tableWidget->setRowCount(3);
		rowColumn[ROW] = 3;
		rowColumn[COLUMN] = 3;
		break;
	case 2:
		makeStandarBox();
		ui->boxStandar->setCurrentIndex(0);
		break;
	}


}

void MainWindow::on_standar_currentIndexChanged(int index)
{
	ui->r1->clear();
	ui->r1_2->clear();
	ui->r1_3->clear();
	ui->r2->clear();
	ui->r2_2->clear();
	ui->r2_3->clear();

	ui->r1->addItems(res->fillResistor(index));
	ui->r1_2->addItems(res->fillResistor(index));
	ui->r1_3->addItems(res->fillResistor(index));

	ui->r2->addItems(res->fillResistor(index));
	ui->r2_2->addItems(res->fillResistor(index));
	ui->r2_3->addItems(res->fillResistor(index));

	refresh(antOP);
}

void MainWindow::resetButton()
{
	switch(ui->standar->currentIndex()){
	case 0:
		ui->r1_c5->setStyleSheet(colores.at(NINGUNO));
		ui->r2_c5->setStyleSheet(colores.at(NINGUNO));
		ui->r1_2_c5->setStyleSheet(colores.at(NINGUNO));
		ui->r2_2_c5->setStyleSheet(colores.at(NINGUNO));
		ui->r1_3_c5->setStyleSheet(colores.at(NINGUNO));
		ui->r2_3_c5->setStyleSheet(colores.at(NINGUNO));
		break;
	case 1:
		ui->r1_c5->setStyleSheet(colores.at(PLATA));
		ui->r2_c5->setStyleSheet(colores.at(PLATA));
		if(ui->checkBox->isChecked())
		ui->r1_2_c5->setStyleSheet(colores.at(PLATA));
		else ui->r1_2_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_3->isChecked())
		ui->r2_2_c5->setStyleSheet(colores.at(PLATA));
		else ui->r2_2_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_2->isChecked())
		ui->r1_3_c5->setStyleSheet(colores.at(PLATA));
		else ui->r1_3_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_4->isChecked())
		ui->r2_3_c5->setStyleSheet(colores.at(PLATA));
		else ui->r2_3_c5->setStyleSheet(colores.at(NINGUNO));
		break;
	case 2:
		ui->r1_c5->setStyleSheet(colores.at(ORO));
		ui->r2_c5->setStyleSheet(colores.at(ORO));
		if(ui->checkBox->isChecked())
		ui->r1_2_c5->setStyleSheet(colores.at(ORO));
		else ui->r1_2_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_3->isChecked())
		ui->r2_2_c5->setStyleSheet(colores.at(ORO));
		else ui->r2_2_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_2->isChecked())
		ui->r1_3_c5->setStyleSheet(colores.at(ORO));
		else ui->r1_3_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_4->isChecked())
		ui->r2_3_c5->setStyleSheet(colores.at(ORO));
		else ui->r2_3_c5->setStyleSheet(colores.at(NINGUNO));
		break;
	case 3:
		ui->r1_c5->setStyleSheet(colores.at(ROJO));
		ui->r2_c5->setStyleSheet(colores.at(ROJO));
		if(ui->checkBox->isChecked())
		ui->r1_2_c5->setStyleSheet(colores.at(ROJO));
		else ui->r1_2_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_3->isChecked())
		ui->r2_2_c5->setStyleSheet(colores.at(ROJO));
		else ui->r2_2_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_2->isChecked())
		ui->r1_3_c5->setStyleSheet(colores.at(ROJO));
		else ui->r1_3_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_4->isChecked())
		ui->r2_3_c5->setStyleSheet(colores.at(ROJO));
		else ui->r2_3_c5->setStyleSheet(colores.at(NINGUNO));
		break;
	case 4:
		ui->r1_c5->setStyleSheet(colores.at(MARRON));
		ui->r2_c5->setStyleSheet(colores.at(MARRON));
		if(ui->checkBox->isChecked())
		ui->r1_2_c5->setStyleSheet(colores.at(MARRON));
		else ui->r1_2_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_3->isChecked())
		ui->r2_2_c5->setStyleSheet(colores.at(MARRON));
		else ui->r2_2_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_2->isChecked())
		ui->r1_3_c5->setStyleSheet(colores.at(MARRON));
		else ui->r1_3_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_4->isChecked())
		ui->r2_3_c5->setStyleSheet(colores.at(MARRON));
		else ui->r2_3_c5->setStyleSheet(colores.at(NINGUNO));
		break;
	case 5:
		ui->r1_c5->setStyleSheet(colores.at(VERDE));
		ui->r2_c5->setStyleSheet(colores.at(VERDE));
		if(ui->checkBox->isChecked())
		ui->r1_2_c5->setStyleSheet(colores.at(VERDE));
		else ui->r1_2_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_3->isChecked())
		ui->r2_2_c5->setStyleSheet(colores.at(VERDE));
		else ui->r2_2_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_2->isChecked())
		ui->r1_3_c5->setStyleSheet(colores.at(VERDE));
		else ui->r1_3_c5->setStyleSheet(colores.at(NINGUNO));
		if(ui->checkBox_4->isChecked())
		ui->r2_3_c5->setStyleSheet(colores.at(VERDE));
		else ui->r2_3_c5->setStyleSheet(colores.at(NINGUNO));
		break;
	}
	ui->r1_c1->setStyleSheet(colores.at(NINGUNO));
	ui->r1_c2->setStyleSheet(colores.at(NEGRO));
	ui->r1_c3->setStyleSheet(colores.at(MARRON));
	ui->r1_c4->setStyleSheet(colores.at(NEGRO));

	ui->r2_c1->setStyleSheet(colores.at(NINGUNO));
	ui->r2_c2->setStyleSheet(colores.at(NEGRO));
	ui->r2_c3->setStyleSheet(colores.at(MARRON));
	ui->r2_c4->setStyleSheet(colores.at(NEGRO));


	ui->r1_2_c1->setStyleSheet(colores.at(NINGUNO));
	ui->r1_2_c2->setStyleSheet(colores.at(NINGUNO));
	ui->r1_2_c3->setStyleSheet(colores.at(NINGUNO));
	ui->r1_2_c4->setStyleSheet(colores.at(NINGUNO));


	ui->r1_3_c1->setStyleSheet(colores.at(NINGUNO));
	ui->r1_3_c2->setStyleSheet(colores.at(NINGUNO));
	ui->r1_3_c3->setStyleSheet(colores.at(NINGUNO));
	ui->r1_3_c4->setStyleSheet(colores.at(NINGUNO));


	ui->r2_3_c1->setStyleSheet(colores.at(NINGUNO));
	ui->r2_3_c2->setStyleSheet(colores.at(NINGUNO));
	ui->r2_3_c3->setStyleSheet(colores.at(NINGUNO));
	ui->r2_3_c4->setStyleSheet(colores.at(NINGUNO));

	ui->r2_2_c1->setStyleSheet(colores.at(NINGUNO));
	ui->r2_2_c2->setStyleSheet(colores.at(NINGUNO));
	ui->r2_2_c3->setStyleSheet(colores.at(NINGUNO));
	ui->r2_2_c4->setStyleSheet(colores.at(NINGUNO));

	/*
	 * Pinta los Cuadros para darle colores a las resistencias de manera que obtenga el usuario mejor reconocimiento
	*/
	QString te2 = ui->r1->currentText();
	QString temp = colores.at(ui->r1->currentText().at(0).unicode() - 47);
	if(ui->standar->currentIndex() > 2){
		ui->r1_c1->setStyleSheet(temp);			//	Color del Primer Digito		//
		ui->r1_c2->setStyleSheet(colores.at(ui->r1->currentText().at(2).unicode() - 47));			//	Color del Segundo Digito	//
		ui->r1_c3->setStyleSheet(colores.at(ui->r1->currentText().at(3).unicode() - 47));			//	Color del Tercer Digito		//
		ui->r1_c4->setStyleSheet(colores.at(ui->r1m->currentIndex()));								// 	Color del Multiplicador 	//

		ui->r2_c1->setStyleSheet(colores.at(ui->r2->currentText().at(0).unicode() - 47));			//	Color del Primer Digito		//
		ui->r2_c2->setStyleSheet(colores.at(ui->r2->currentText().at(2).unicode() - 47));			//	Color del Segundo Digito	//
		ui->r2_c3->setStyleSheet(colores.at(ui->r2->currentText().at(3).unicode() - 47));			//	Color del Tercer Digito		//
		ui->r2_c4->setStyleSheet(colores.at(ui->r2m->currentIndex()));								// 	Color del Multiplicador 	//

		if(ui->checkBox->isChecked()){
			ui->r1_2_c1->setStyleSheet(colores.at(ui->r1_2->currentText().at(0).unicode() - 47));	//	Color del Primer Digito		//
			ui->r1_2_c2->setStyleSheet(colores.at(ui->r1_2->currentText().at(2).unicode() - 47));	//	Color del Segundo Digito	//
			ui->r1_2_c3->setStyleSheet(colores.at(ui->r1_2->currentText().at(3).unicode() - 47));	//	Color del Tercer Digito		//
			ui->r1_2_c4->setStyleSheet(colores.at(ui->r1_2m->currentIndex()));						// 	Color del Multiplicador 	//
		}

		if(ui->checkBox_3->isChecked()){
			ui->r2_2_c1->setStyleSheet(colores.at(ui->r2_2->currentText().at(0).unicode() - 47));	//	Color del Primer Digito		//
			ui->r2_2_c2->setStyleSheet(colores.at(ui->r2_2->currentText().at(2).unicode() - 47));	//	Color del Segundo Digito	//
			ui->r2_2_c3->setStyleSheet(colores.at(ui->r2_2->currentText().at(3).unicode() - 47));	//	Color del Tercer Digito		//
			ui->r2_2_c4->setStyleSheet(colores.at(ui->r2_2m->currentIndex()));						// 	Color del Multiplicador		//
		}

		if(ui->checkBox_2->isChecked()){
			ui->r1_3_c1->setStyleSheet(colores.at(ui->r1_3->currentText().at(0).unicode() - 47));	//	Color del Primer Digito		//
			ui->r1_3_c2->setStyleSheet(colores.at(ui->r1_3->currentText().at(2).unicode() - 47));	//	Color del Segundo Digito	//
			ui->r1_3_c3->setStyleSheet(colores.at(ui->r1_3->currentText().at(3).unicode() - 47));	//	Color del Tercer Digito		//
			ui->r1_3_c4->setStyleSheet(colores.at(ui->r1_3m->currentIndex()));						// 	Color del Multiplicador		//
		}
		if(ui->checkBox_4->isChecked()){
			ui->r2_3_c1->setStyleSheet(colores.at(ui->r2_3->currentText().at(0).unicode() - 47));	//	Color del Primer Digito		//
			ui->r2_3_c2->setStyleSheet(colores.at(ui->r2_3->currentText().at(2).unicode() - 47));	//	Color del Segundo Digito	//
			ui->r2_3_c3->setStyleSheet(colores.at(ui->r2_3->currentText().at(3).unicode() - 47));	//	Color del Tercer Digito		//
			ui->r2_3_c4->setStyleSheet(colores.at(ui->r2_3m->currentIndex()));						// 	Color del Multiplicador		//
		}
	} else {
		
		ui->r1_c2->setStyleSheet(colores.at(ui->r1->currentText().at(0).unicode() - 47));			//	Color del Primer Digito		//
		ui->r1_c3->setStyleSheet(colores.at(ui->r1->currentText().at(2).unicode() - 47));			//	Color del Segundo Digito	//
		ui->r1_c4->setStyleSheet(colores.at(ui->r1m->currentIndex()));								//	Color del Multiplicador		//
		
		ui->r2_c2->setStyleSheet(colores.at(ui->r2->currentText().at(0).unicode() - 47));			//	Color del Primer Digito		//
		ui->r2_c3->setStyleSheet(colores.at(ui->r2->currentText().at(2).unicode() - 47));			//	Color del Segundo Digito	//
		ui->r2_c4->setStyleSheet(colores.at(ui->r2m->currentIndex()));								//	Color del Multiplicador		//
	
		if(ui->checkBox->isChecked()){
		ui->r1_2_c2->setStyleSheet(colores.at(ui->r1_2->currentText().at(0).unicode() - 47));		//	Color del Primer Digito		//
		ui->r1_2_c3->setStyleSheet(colores.at(ui->r1_2->currentText().at(2).unicode() - 47));		//	Color del Segundo Digito	//
		ui->r1_2_c4->setStyleSheet(colores.at(ui->r1_2m->currentIndex()));							//	Color del Multiplicador		//
		}

		if(ui->checkBox_3->isChecked()){
		ui->r2_2_c2->setStyleSheet(colores.at(ui->r2_2->currentText().at(0).unicode() - 47));		//	Color del Primer Digito		//
		ui->r2_2_c3->setStyleSheet(colores.at(ui->r2_2->currentText().at(2).unicode() - 47));		//	Color del Segundo Digito	//
		ui->r2_2_c4->setStyleSheet(colores.at(ui->r2_2m->currentIndex()));							//	Color del Multiplicador		//
		}

		if(ui->checkBox_2->isChecked()){
		ui->r1_3_c2->setStyleSheet(colores.at(ui->r1_3->currentText().at(0).unicode() - 47));		//	Color del Primer Digito		//
		ui->r1_3_c3->setStyleSheet(colores.at(ui->r1_3->currentText().at(2).unicode() - 47));		//	Color del Segundo Digito	//
		ui->r1_3_c4->setStyleSheet(colores.at(ui->r1_3m->currentIndex()));							//	Color del Multiplicador		//
		}

		if(ui->checkBox_4->isChecked()){
		ui->r2_3_c2->setStyleSheet(colores.at(ui->r2_3->currentText().at(0).unicode() - 47));		// 	Color del Primer Digito		//
		ui->r2_3_c3->setStyleSheet(colores.at(ui->r2_3->currentText().at(2).unicode() - 47));		// 	Color del Segundo Digito	//
		ui->r2_3_c4->setStyleSheet(colores.at(ui->r2_3m->currentIndex()));							// 	Color del Multiplicador		//
		}
	}
}


void MainWindow::makeColor()
{
	colores << "*{ background-color: rgb(231,174,24); border-radius: 5px; border-style: outset; border-width: 1px 1px 1px 1px; border-color: rgb(185, 124, 0); }";
	colores << "*{ background-color: rgb(30,30,30); border-radius: 5px; border-style: outset; border-width: 1px 1px 1px 1px; border-color: rgb(0, 0, 0); }";
	colores << "*{ background-color: rgb(128,0,0); border-radius: 5px; border-style: outset; border-width: 1px 1px 1px 1px; border-color: rgb(78, 0, 0); }";
	colores << "*{ background-color: rgb(255,0,0); border-radius: 5px; border-style: outset; border-width: 1px 1px 1px 1px; border-color: rgb(205, 0, 0); }";

	colores << "*{ background-color: rgb(255,102,0);border-radius: 5px; border-style: outset; border-width: 1px 1px 1px 1px; border-color: rgb(205, 52, 0); }";
	colores << "*{ background-color: rgb(255,255,0); border-radius: 5px; border-style: outset; border-width: 1px 1px 1px 1px; border-color: rgb(205, 205, 0);  }";
	colores << "*{ background-color: rgb(0,255,0); border-radius: 5px; border-style: outset; border-width: 1px 1px 1px 1px; border-color: rgb(0, 205, 0); }";

	colores << "*{ background-color: rgb(0,0,255); border-radius: 5px; border-style: outset; border-width: 1px 1px 1px 1px; border-color: rgb(0, 0, 205); }";
	colores << "*{ background-color: rgb(153,0,204); border-radius: 5px; border-style: outset; border-width: 1px 1px 1px 1px; border-color: rgb(103, 0, 154); }";
	colores << "*{ background-color: rgb(128,128,128); border-radius: 5px; border-style: outset; border-width: 1px 1px 1px 1px; border-color: rgb(78, 78, 78); }";

	colores << "*{ background-color: rgb(255,255,255); border-radius: 5px; border-style: outset; border-width: 1px 1px 1px 1px; border-color: rgb(205, 205, 205); }";
	colores << "*{ background-color: rgb(215,215,215); border-radius: 5px; border-style: outset; border-width: 1px 1px 1px 1px; border-color: rgb(124, 124, 124); }";
	colores << "border-style: inset; border-width: 1px; border-color: rgb(120, 120, 120); background-color: rgb(170, 170, 170); border-radius: 5px;";


}


void MainWindow::makeStandarBox()
{
	float valores[6][192];
	float valoresConvertidos[12][16];
	int valor = ui->boxStandar->currentIndex();
	int maxc = valor == 0 ? 1 : plusmath::powXN(6, valor, 2) / 12;
	int maxr = valor == 0 ? 6 : 12;
	ui->boxValoresCom->clear();
	ui->boxValoresCom->setColumnCount(maxc);
	ui->boxValoresCom->setRowCount(maxr);
	res->getResistor(valores);

	QTableWidgetItem *tmpText[12][16];
	for (int i = 0; i < maxr; i++){
		for(int e = 0; e < maxc; e++){
			tmpText[i][e] = new QTableWidgetItem;
		}
	}
	for(int i = 0; i < maxr; i++){
		for(int e = 0; e < maxc; e++){
			valoresConvertidos[i][e] = valores[valor][e+(i*maxc)];
		}
	}
	for(int i = 0; i < maxr; i++){
		for(int e = 0; e < maxc; e++){
			tmpText[i][e]->setText(QString::number(valoresConvertidos[i][e], 'f', valor < 3 ? 1 : 2));
			ui->boxValoresCom->setItem(i, e, tmpText[i][e]);
			//ui->boxValoresCom->setCellWidget(i, e, );
		}
	}
	int valmultiplicador = 80+(60*plusmath::powXN(1,valor-1,2));
	setMinimumSize(valmultiplicador, 550);
	setMaximumSize(valor > 3 ? valmultiplicador : 420, 550);

}

void MainWindow::on_boxStandar_currentIndexChanged(int index)
{
	makeStandarBox();
}
