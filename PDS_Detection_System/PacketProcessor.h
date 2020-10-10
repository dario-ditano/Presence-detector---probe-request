#pragma once
#include <stdlib.h>
#include <stdio.h>

#include <mysqlx/xdevapi.h>
#include "dlib\optimization.h"
#include "dlib\global_optimization.h"

#include "CoverageArea.h"

typedef dlib::matrix<double, 0, 1> column_vector;

class PacketProcessor
{
private:

	/* --------- VARIABLES ----------- */
	int esp_number;                                                                                                      /* number of the ESP32 */
	double rssiAtOneMeter, rssiDiv;                                                                                      /* Value of rssi at one meter of distance and constant for the formula */

	CoverageArea ca;                                                                                                     /* Area of Coverage */

	/* --------- FUNCTIONS ----------- */
	/* Method that estimates the distance (in meters) starting from the RSSI */
	double getDistanceFromRSSI(double rssi);

	/* Method that calculates the distance among two points (x1,y1) , (x2,y2) */
	double static dist(double x1, double y1, double x2, double y2);

	/* Method that defines the Mean Square Error (MSE) function */
	double static meanSquareError(const column_vector& m);

	/* Method that finds the min (x,y) of the function meanSquareError ==> the (x,y) point will be the position of the device */
	void trilaterate(double* pos_x, double* pos_y);

	void readRssiParam();

public:

	/* Constructor */
	PacketProcessor(int count);

	/* Main Function */
	void process();
};

