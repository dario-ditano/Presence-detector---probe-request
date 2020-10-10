#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <sstream> 
#include <iomanip>

#include <mysqlx/xdevapi.h>

/* Probe Packet received from ESP32 */
class ProbePacket {

public:

	/* --------- VARIABLES ----------- */
	unsigned timestamp : 32;	//4  bytes +
	unsigned channel : 8;		//1  bytes +
	unsigned seq_ctl : 16;		//2  bytes +
	signed rssi : 8;			//1  bytes +
	uint8_t addr[6];			//6  bytes +
	uint8_t ssid_length;		//1  bytes +
	uint8_t ssid[32];			//32 bytes +
	uint8_t crc[4];				//4  bytes +
	unsigned long hash;			//4  bytes =
								//_________
								//55 bytes

	/* --------- FUNCTIONS ----------- */
	ProbePacket();

	/* Print information about the packet */
	void print();

	/* Print information about the packet on the last update */
	void print(long int last_update);

	/* Connect to the database and store the content of this instance of a packet in it.
	   long int last_update: The last time when the local computer timestamp has been properly recorded. */
	void storeInDB(mysqlx::Table packetTable, mysqlx::Table localPacketsTable, long int last_update, uint8_t espid);

	/* Check if the SSID is a valid utf8 string */
	bool checkSSID(const std::string& string);
};
