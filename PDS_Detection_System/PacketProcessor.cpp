/**
 * Copyright (c) 2019, HypnoProject
 * PACKET PROCESSOR
 *
 * Bonafede Giacomo
 * Ditano Dario
 * Poppa Emanuel
 *
 */

 /****************************
 * INCLUDE, DEFINE, MACRO   *
 ****************************/
 /* _______________________________________________  INCLUDE  ______________________________________________________ */
#include "stdafx.h"
#include "PacketProcessor.h"
#include <iostream>
#include <iomanip>
#include <fstream>

/*****************************************************
* CONSTANTS, STRUCTS, GLOBAL VARIABLES AND FUNCTIONS *
******************************************************/
/* ____________________________________________  GLOBAL VARIABLE  ___________________________________________________ */
std::string fileUrl = "rssi_div.txt";
std::vector<int> x;                                                                                                      /* vector of x coordinates */
std::vector<int> y;                                                                                                      /* vector of y coordinates */
std::vector<double> d;                                                                                                   /* vector of distances */

/* ______________________________________________  FUNCTIONS  _______________________________________________________ */

PacketProcessor::PacketProcessor(int count)
{
	esp_number = count;
	rssiAtOneMeter = -59;
	rssiDiv = 20;
}

void PacketProcessor::readRssiParam() {
	std::ifstream inFile;
	int count = 1;
	double x;

	inFile.open(fileUrl);
	if (!inFile) {
		std::cout << "Unable to open file";
		exit(1); // terminate with error
	}

	while (inFile >> x) {
		switch (count)
		{
		case 1:
			rssiAtOneMeter = x;
			break;
		case 2:
			rssiDiv = x;
			break;
		default:
			break;
		}

		count++;
	}

	inFile.close();
}

//Method that estimates the distance (in meters) starting from the RSSI
double PacketProcessor::getDistanceFromRSSI(double rssi) {
	double d = pow(10, (rssiAtOneMeter - rssi) / rssiDiv);
	return d;
}

//Method that calculates the distance among two points (x1,y1) , (x2,y2)
double PacketProcessor::dist(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

//Method that defines the Mean Square Error (MSE) function
double PacketProcessor::meanSquareError(const column_vector& m) {
	const double pos_x = m(0);
	const double pos_y = m(1);

	double mse = 0;
	int N = d.size();

	for (int i = 0; i < N; i++)
		mse = mse + pow(d[i] - dist(pos_x, pos_y, x[i], y[i]), 2);

	mse = mse / N;

	return mse;
}

/* Method that finds the min (x,y) of the function meanSquareError ==> the (x,y) point will be the position of the device */
void PacketProcessor::trilaterate(double* pos_x, double* pos_y) {
	try {
		column_vector starting_point = { 0, 0 };

		/* Performs an unconstrained minimization of a nonlinear function using some search strategy. */
		/* This version doesn't take a gradient function but instead numerically approximates the gradient. */
		/* Requires
				- search_strategy == an object that defines a search strategy such as one of the objects from dlib/optimization/optimization_search_strategies_abstract.h
				- stop_strategy == an object that defines a stop strategy such as one of the objects from dlib/optimization/optimization_stop_strategies_abstract.h
				- f(x) must be a valid expression that evaluates to a double
				- is_col_vector(x) == true
				- derivative_eps > 0
			Ensures
				- Performs an unconstrained minimization of the function f() using the given search_strategy and starting from the initial point x.
				- The function is optimized until stop_strategy decides that an acceptable point has been found or f(#x) < min_f.
				- #x == the value of x that was found to minimize f()
				- returns f(#x).
				- Uses the dlib::derivative(f,derivative_eps) function to compute gradient information.
	*/
		dlib::find_min_using_approximate_derivatives(dlib::bfgs_search_strategy(),                                       /* Use BFGS search algorithm */
			dlib::objective_delta_stop_strategy(1e-7),                          /* Stop when the change in Mean square error is less than 1e-7 */
			&meanSquareError,                                                   /* Error Function */
			starting_point,                                                     /* Starting point */
			-1);
		*pos_x = starting_point(0);
		*pos_y = starting_point(1);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

/**
 * + -------------------------------------------------------------- process() Workflow ------------------------------------------------------------------------ +
   | + For each packet:                                                                                                                                         |
   | - Count how many ESP have received the packet (at least 3) and retrieve the rssi distance from each of them                                                |
   | - + for each ESP which received the packet:                                                                                                                |
   | --- Get coordinates of the ESP and compute the distance from the packet                                                                                    |
   | - Trilaterate the device with the current MAC address getting its coordinates pos_x and pos_y                                                              |
   +------------------------------------------------------------------------------------------------------------------------------------------------------------+
*/
/* Main function, process all the received packets */
void PacketProcessor::process() {

	/* --------- Initialization ---------- */
	double pos_x = -1;                                                                                                   /* Reset pos_x */
	double pos_y = -1;
	int n = 0;

	x.clear();                                                                                                           /* Reset of x coordinates vector */
	y.clear();                                                                                                           /* Reset of y coordinates vector */
	d.clear();                                                                                                           /* Reset of distances vector */

	try {
		mysqlx::Session session("localhost", 33060, "pds_user", "password");                                             /* Open the session with the DB */
		try {
			/* ---------- Load DB Table ----------- */
			mysqlx::Schema myDb = session.getSchema("pds_db");                                                           /* Get DB schema */
			mysqlx::Table packetTable = myDb.getTable("Packet");                                                         /* Get Packet table */
			mysqlx::Table espTable = myDb.getTable("ESP");                                                               /* Get ESP table */
			mysqlx::Table devicesTable = myDb.getTable("Devices");                                                       /* Get Devices table */
			mysqlx::Table localMacsTable = myDb.getTable("Local_Macs");                                                  /* Get Local Macs table */

			mysqlx::RowResult retrievedPackets;                                                                          /* Allows traversing the Row objects returned by a Table.select operation. */
																														 /* "List f packets" */
			mysqlx::Row row;                                                                                             /* Represents a row element returned from a SELECT query. */

			retrievedPackets = packetTable.select("hash", "addr").execute();                                             /* Get Hash and MAC address of the current packet */
			std::cout << "_________________________________________________" << std::endl;
			std::cout << "RETRIEVED PACKETS: " << std::endl;
			std::cout << "+------------------------------------------------------------------------+" << std::endl;
			/* ---------- Process Packets ---------- */
			for (mysqlx::Row packet : retrievedPackets.fetchAll()) {                                                     /* Iterate all over the packets */
				uint32_t current_hash = (uint32_t)packet[0];                                                             /* Save Hash of the current packet */
				std::string current_address = static_cast<std::string> (packet[1]);				                         /* Save MAC of th current packet */

				/* Count how many ESPs have received this packet (this hash) */
				mysqlx::RowResult hashCount = packetTable.select("count(DISTINCT(esp_id))").where("hash=:current_hash AND to_be_deleted = 0").bind("current_hash", current_hash).execute();

				row = hashCount.fetchOne();
				int counter = row[0];                                                                                    /* Save the number of ESP which have received the current packet */


				/* floor(esp_number / 2) + 1) { //the packet has been received by at least 3 ESPs (Note: change this value in debug/testing)*/
				if (((esp_number < 4) && (counter == esp_number)) || ((esp_number >= 4) && (counter >= floor(esp_number / 2) + 1))) {

					uint64_t average_timestamp = 0;
					int count = 0;

					/* Get the ESP-ID and the RSSI from *ALL* the ESPs which have received the packet N.B.: this query gives multiple rows --> one row for each ESP which has received the packet */
					mysqlx::RowResult multiple_query_result = packetTable.select("esp_id", "rssi", "unix_timestamp(timestamp)").where("hash=:current_hash").bind("current_hash", current_hash).execute();

					/* --------- Process ESP which have received the current packet -------- */
					for (mysqlx::Row rows : multiple_query_result.fetchAll()) {                                          /* Iterate all over the ESP which have received the packet */
						uint32_t current_esp_id = (uint32_t)rows[0];                                                     /* Save Id of the current ESP */
						int current_rssi = (int)rows[1];                                                                 /* Save rssi of the packet for current ESP */
						average_timestamp += (uint64_t)rows[2];                                                          /* Add the timestamp to the average timestamp */

						/* Get the coordinates of the ESP who has received the current packet */
						mysqlx::RowResult esp_coordinates = espTable.select("x", "y").where("esp_id=:current_esp_id").bind("current_esp_id", current_esp_id).execute();
						row = esp_coordinates.fetchOne();
						int current_esp_x = (int)row[0];                                                                 /* Save x coordinate of the current ESP */
						int current_esp_y = (int)row[1];                                                                 /* Save y coordinate of the current ESP */

						double current_distance = getDistanceFromRSSI(current_rssi);                                     /* Estimate the distance from the RSSI using */

						x.push_back(current_esp_x);                                                                      /* Add the value of x coordinate in the vector */
						y.push_back(current_esp_y);                                                                      /* Add the value of y coordinate in the vector */
						d.push_back(current_distance);                                                                   /* Add the value of distance in the vector */

						count++;                                                                                         /* Increment the number of ESP which have received the packet */
					}

					/* ------------- Device Position -------------- */
					trilaterate(&pos_x, &pos_y);                                                                         /* Trilaterate the device with the current MAC address getting its coordinates pos_x and pos_y */

					/* ----------- Computing timestamp ------------ */
					average_timestamp = floor(average_timestamp / count);                                                /* The floor() function in C++ returns the largest possible integer value which is less than
																														  * or equal to the given argument. */
					time_t rawtime = average_timestamp;
					struct tm timeinfo;                                                                                  /* Structure containing a calendar date and time broken down into its components. */
					char average_time[20];
					localtime_s(&timeinfo, &rawtime);                                                                    /* Converts given time since epoch (a time_t value pointed to by time) into calendar time, */
																														 /* expressed in local time, in the struct tm format. */
																														 /* The result is stored in static storage and a pointer to that static storage is returned. */
																														 /* With error handler */
					strftime(average_time, 20, "%F %T", &timeinfo);                                                      /* The strftime() function in C++ converts the given date and time from a given calendar time */
																														 /* time to a null-terminated multibyte character string according to a format string. */
					/* ------------ MAC address check ------------- */
					int local = 0;
					int firstByteMAC = std::stol(current_address.substr(0, 2), nullptr, 16);                             /* Parses str interpreting its content as an integral number of the specified base, */
																														 /* which is returned as a value of type long int. */
					int mask1 = 0b00000010;                                                                              /* global/local bit */
					int mask2 = 0b00000001;                                                                              /* unicast/multicast bit */

					if ((firstByteMAC & mask1) && !(firstByteMAC & mask2))
						local = 1;                                                                                       /* local MAC */

					/* -------------- Position check -------------- */
					if (ca.isInside(pos_x, pos_y)) {                                                                     /* Device within the coverage area. */
						if (local) {
							localMacsTable.insert("mac", "x", "y", "timestamp").values(current_address, pos_x, pos_y, average_time).execute();
							std::cout << "LOCAL "<< current_hash << ":" << current_address << " (" << pos_x << ", " << pos_y << ") " << average_time << std::endl;
						}
						else {
							devicesTable.insert("mac", "x", "y", "timestamp").values(current_address, pos_x, pos_y, average_time).execute();
							std::cout << "| "<< current_hash << "\t" << current_address << "\t (" << pos_x << "," << pos_y << ")\t " << average_time << "|" << std::endl;
							std::cout << "+------------------------------------------------------------------------+" << std::endl;
							n++;
						}
					}
					else {
						//std::cout << current_address << " - The device is outside the coverage area." << std::endl;
					}

					/* ------- Remove trilaterated packets -------- */
					packetTable.update().set("to_be_deleted", 1).where("hash=:current_hash").bind("current_hash", current_hash).execute();
				}
				else if (counter != 0) {                                                                                 /* Less then 3 ESP32 */

					mysqlx::RowResult timestamp_result = packetTable.select("unix_timestamp(timestamp)").where("hash=:current_hash").bind("current_hash", current_hash).execute();
					row = timestamp_result.fetchOne();
					uint64_t ts = row[0];

					if (static_cast<uint64_t> (time(NULL)) - ts > 30) {                                                 /* If the packet has been received more than 2 minutes ago then delete it */
						packetTable.update().set("to_be_deleted", 1).where("hash=:current_hash").bind("current_hash", current_hash).execute();
					}
				}
			}
			packetTable.remove().where("to_be_deleted = 1").execute();
			
			std::cout << "+ Number of packets sent inside the area: " << n << std::endl;
			std::cout << "_________________________________________________" << std::endl;
		}
		catch (std::exception& err) {
			std::cout << "The following error occurred: " << err.what() << std::endl;
			exit(1);
		}
	}
	catch (std::exception& err) {
		std::cout << "The database session could not be opened: " << err.what() << std::endl;
		exit(1);
	}
}



