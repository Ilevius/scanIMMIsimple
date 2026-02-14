#include <string>
#include "C:\Program Files (x86)\ACS Motion Control\SPiiPlus ADK Suite v3.02\ACSC\C_CPP\ACSC.h"
#pragma once  // защита от повторного включения
using namespace std;

class Table {  // имя класса с большой буквы
public:          // публичный интерфейс
				 // Конструктор
	Table(string value);

	// Методы
	string getIP() const;
	void setIP(string new_value);
	bool IS_CONNECTED();
	int connect(bool SIMULATOR);
	int enable_x_motor();
	int disable_x_motor();
	int enable_y_motor();
	int disable_y_motor();
	double get_x_position();
	double get_y_position();



	// Геттеры (get)
	bool getXMotorOn() const { return X_MOTOR_ON; }
	bool getYMotorOn() const { return Y_MOTOR_ON; }
	double getX_HOME_POSITION() const { return X_HOME_POSITION; }
	double getY_HOME_POSITION() const { return Y_HOME_POSITION; }

	// Сеттеры (set)
	void setXMotorOn(bool state) { X_MOTOR_ON = state; }
	void setYMotorOn(bool state) { Y_MOTOR_ON = state; }
	void setX_HOME_POSITION(double state) { X_HOME_POSITION = state; }
	void setY_HOME_POSITION(double state) { Y_HOME_POSITION = state; }



	// Свойства
private:         // приватные данные
	string TABLE_IP;    // поле класса
	HANDLE hComm;
	bool X_MOTOR_ON;
	bool Y_MOTOR_ON;
	double X_HOME_POSITION;
	double Y_HOME_POSITION;
	bool connected = false;
	void ErrorsHandler(const char* ErrorMessage, BOOL fCloseComm);

};
