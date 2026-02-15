#include "stdafx.h"
#include <string>
#include <iostream>
#include "table.h"
#include "C:\Program Files (x86)\ACS Motion Control\SPiiPlus ADK Suite v3.02\ACSC\C_CPP\ACSC.h"
#include <conio.h>
using namespace std;

Table::Table(string value) : TABLE_IP(value) {
	// конструктор инициализирует поле data
	connect(true);
	
	



}

string Table::getIP() const {
	return TABLE_IP;
}

void Table::setIP(string new_value) {
	TABLE_IP = new_value;
}

void Table::ErrorsHandler(const char* ErrorMessage, BOOL fCloseComm)
{
	printf(ErrorMessage);
	printf("press any key to exit.\n");
	if (fCloseComm) acsc_CloseComm(hComm);
	_getch();
};

bool Table::IS_CONNECTED()
{
	return connected;
}

int Table::connect(bool SIMULATOR)
{
	if (SIMULATOR)
	{
		// Open communication with the simulator
		printf("Application opens communication with the simulator and\n");
		printf("sends some commands to the simulator using SPiiPlus C Library functions\n\n");
		printf("Wait for opening of communication with the simulator...\n");
		hComm = acsc_OpenCommSimulator();
		if (hComm == ACSC_INVALID)
		{
			ErrorsHandler("error while opening communication.\n", FALSE);
			return-1;
		}
		else
		{
			connected = true;
			printf("Communication with the simulator was established successfully!\n");
			return 0;
		}
		
	}
	else
	{
		// Opening communication with real controller via Ethernet
		printf("Application opens communication with the controller via ethernet and\n");
		printf("sends some commands to the controller using SPiiPlus C Library functions\n\n");
		printf("Wait for opening of communication with the controller...\n");
		//10.0.0.100- default IP address of the controller
		//for the point to point connection to the controller
	
		char IP_ADRESS[256];
		strncpy_s(IP_ADRESS, TABLE_IP.c_str(), sizeof(IP_ADRESS) - 1);
		IP_ADRESS[sizeof(IP_ADRESS) - 1] = '\0';
	
		hComm = acsc_OpenCommEthernet(IP_ADRESS, ACSC_SOCKET_DGRAM_PORT);
		if (hComm == ACSC_INVALID)
		{
			ErrorsHandler("error while opening communication.\n", FALSE);
			return-1;
		}
		else
		{
			printf("Communication with the REAL CONTROLLER was established successfully! Be aware of stage jumps and moving!!!\n");
			connected = true;
			return 0;
		}
		
	}

}



int Table::enable_x_motor()
{
	int status = acsc_Enable(hComm, ACSC_AXIS_0, NULL);
	if(status)
	{ 
		setXMotorOn(true);
		printf("ATTENTION!!!	X AXIS   MOTOR  HAS BEEN SWITCHED ON!!! \n");
		return 0;
	}
	else
	{
		cout << "x axis motor enabling error! Status: " << status << endl;
		return -1;
	}
}

int Table::disable_x_motor()
{
	int status = acsc_Disable(hComm, ACSC_AXIS_0, NULL);
	if(status)
	{ 
		setXMotorOn(false);
		printf("X AXIS   MOTOR  disabled \n");
		return 0;
	}
	else
	{
		cout << "ATTENTION!!! DISABLING ERROR!!!	X AXIS   MOTOR  still WORKS!! " << status << endl;
		return -1;
	}
}

double Table::get_x_position()
{
	double XPOS;
	if (acsc_GetFPosition(hComm, ACSC_AXIS_0, &XPOS, NULL))
	{
		return XPOS;
	}
	else
	{
		printf("Getting x position error");
		return 0;
	}
}

double Table::get_y_position()
{
	double YPOS;
	if (acsc_GetFPosition(hComm, ACSC_AXIS_1, &YPOS, NULL))
	{
		return YPOS;
	}
	else
	{
		printf("Getting y position error");
		return 0;
	}
}

bool Table::x_moving()
{
	int State;
	if (!acsc_GetMotorState(hComm, ACSC_AXIS_0, &State, NULL))
	{
		ErrorsHandler("get motor state error.\n", TRUE);
		return false;
	}
	return State & ACSC_MST_MOVE;
}

bool Table::y_moving()
{
	int State;
	if (!acsc_GetMotorState(hComm, ACSC_AXIS_1, &State, NULL))
	{
		ErrorsHandler("get motor state error.\n", TRUE);
		return false;
	}
	return State & ACSC_MST_MOVE;
}

int Table::x_move_from_home_to(double point)
{
	double newPosition = getX_HOME_POSITION() + point;
	if (acsc_ToPoint(hComm, 0, ACSC_AXIS_0, point, NULL))
	{
		printf("ATTENTION!!!	X AXIS   MOTOR  IS  SET TO MOVE TO POSITION %f\r \n", newPosition);
		return 0;
	}
	else
	{
		printf("Some error occured while using acsc_ToPoint command \n");
		return -1;
	}
}














////#include "../stdafx.h"
//#include <conio.h>
//#include <stdio.h>
//#include "windows.h"
//#include <iostream>
//#include "../settings module/settings.h"
//
//
//
//
//
//
//
//
//
//
//using namespace std;
//
//HANDLE hComm;
//
//void ErrorsHandler(const char* ErrorMessage, BOOL fCloseComm)
//{
//	printf(ErrorMessage);
//	printf("press any key to exit.\n");
//	if (fCloseComm) acsc_CloseComm(hComm);
//	_getch();
//};
//
//
//
//
//
//int table()
//{
//
//	double FPOS;
//	int State;
//	printf("ACS Motion Control. Copyright (C) 2011. All Rights Reserved.\n");
//	printf("Application executes reciprocated point-to-point motion\n");
//
//	if (SETTINGS.SIMULATOR)
//	{
//		// Open communication with the simulator
//		printf("Application opens communication with the simulator and\n");
//		printf("sends some commands to the simulator using SPiiPlus C Library functions\n\n");
//		printf("Wait for opening of communication with the simulator...\n");
//		hComm = acsc_OpenCommSimulator();
//		if (hComm == ACSC_INVALID)
//		{
//			ErrorsHandler("error while opening communication.\n", FALSE);
//			return-1;
//		}
//		printf("Communication with the simulator was established successfully!\n");
//	}
//	else
//	{
//		// Opening communication with real controller via Ethernet
//		printf("Application opens communication with the controller via ethernet and\n");
//		printf("sends some commands to the controller using SPiiPlus C Library functions\n\n");
//		printf("Wait for opening of communication with the \controller...\n");
//		//10.0.0.100- default IP address of the controller
//		//for the point to point connection to the controller
//
//		char IP_ADRESS[256];
//		strncpy_s(IP_ADRESS, SETTINGS.IP_ADRESS.c_str(), sizeof(IP_ADRESS) - 1);
//		IP_ADRESS[sizeof(IP_ADRESS) - 1] = '\0';
//
//		hComm = acsc_OpenCommEthernet(IP_ADRESS, ACSC_SOCKET_DGRAM_PORT);
//		if (hComm == ACSC_INVALID)
//		{
//			ErrorsHandler("error while opening communication.\n", FALSE);
//			return-1;
//		}
//		printf("Communication with the REAL CONTROLLER was established successfully! Be aware of stage jumps and moving!!!\n");
//	}
//
//	printf("Press any key to run motion.\n");
//	printf("Then press any key to exit.\n");
//	_getch();
//	// Enable the motor 0
//	if (!acsc_Enable(hComm, ACSC_AXIS_0, NULL))
//	{
//		ErrorsHandler("transaction error.\n", TRUE);
//		return-1;
//	}
//	printf("Motor enabled\n");
//	Sleep(600);
//	while (!_kbhit())
//	{
//
//		// execute point-to-point motion to position 4000
//		if (!acsc_ToPoint(hComm, 0, ACSC_AXIS_0, 140, NULL))
//		{
//			ErrorsHandler("PTP motion error.\n", TRUE);
//			return-1;
//		}
//		printf("Moving to the position 4000...\n");
//		// execute backward point-to-point motion to position 0
//		if (!acsc_ToPoint(hComm, 0, ACSC_AXIS_0, 0, NULL))
//		{
//			ErrorsHandler("PTP motion error.\n", TRUE);
//			return-1;
//		}
//		printf("Moving back to the position 0...\n");
//		// Check if both of motions finished
//		do
//		{
//			if (acsc_GetFPosition(hComm, ACSC_AXIS_0, &FPOS, NULL))
//			{
//				printf("%f\r", FPOS);
//			}
//			// Read the motor 0 state. Fifth bit shows motion
//			// process
//			if (!acsc_GetMotorState(hComm, ACSC_AXIS_0, &State, NULL))
//			{
//				ErrorsHandler("get motor state error.\n", TRUE);
//				return-1;
//			}
//			Sleep(500);
//		} while (State & ACSC_MST_MOVE);
//	}
//	acsc_CloseComm(hComm);
//	return 0;
//
//
//}