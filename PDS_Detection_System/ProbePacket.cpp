/**
 * Copyright (c) 2019, HypnoProject
 * PROBE PACKET
 *
 * Bonafede Giacomo
 * Ditano Dario
 * Poppa Emanuel
 *
 */

 /****************************
 * INCLUDE, DEFINE, MACRO   *
 ****************************/
 /* ______________________________________________  INCLUDE  ________________________________________________________ */

#include "stdafx.h"
#include "ProbePacket.h"

/*****************************************************
* CONSTANTS, STRUCTS, GLOBAL VARIABLES AND FUNCTIONS *
******************************************************/
/* ______________________________________________  FUNCTIONS  _______________________________________________________ */

ProbePacket::ProbePacket() {
}

/* Connect to the database and store the content of this instance of a packet in it.
   long int last_update: The last time when the local computer timestamp has been properly recorded. */
void ProbePacket::storeInDB(mysqlx::Table packetTable, mysqlx::Table localPacketsTable, long int last_update, uint8_t espid) {

	/* ----------- Computing timestamp ------------ */
	time_t rawtime = last_update + timestamp / 1000000;                                                                  /* Computing the timestamp */
	struct tm timeinfo;                                                                                                  /* Structure containing a calendar date and time broken down into its components. */
	char receive_time[20];
	localtime_s(&timeinfo, &rawtime);                                                                                    /* Converts given time since epoch (a time_t value pointed to by time) into calendar time, */
																														 /* expressed in local time, in the struct tm format. */
																														 /* The result is stored in static storage and a pointer to that static storage is returned. */
	strftime(receive_time, 20, "%F %T", &timeinfo);                                                                      /* The strftime() function in C++ converts the given date and time from a given calendar time */
																														 /* time to a null-terminated multibyte character string according to a format string. */
	/* ------ Computing the control sequence ------- */
	std::ostringstream ctlStream;                                                                                        /* Objects of this class use a string buffer that contains a sequence of characters.
																															This sequence of characters can be accessed directly as a string object, using member str. */
	ctlStream << std::setfill('0') << std::setw(4) << std::hex << seq_ctl << std::endl;
	std::string ctl_to_store = ctlStream.str();
	//std::cout << "string to store " << ctl_to_store;

	/* ----------- Computing the address ------------ */
	char buff[50];
	snprintf(buff, sizeof(buff), "%02x:%02x:%02x:%02x:%02x:%02x", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]); /* Store the MAC in buff */
	std::string address = buff;

	/* ------------- Computing the SSID ------------- */
	std::ostringstream ssidStream;
	for (int i = 0; i < ssid_length; i++) {
		ssidStream << (char)ssid[i];
	}
	std::string ssid_to_store = ssidStream.str();                                                                        /* Store the ssid in a string */

	if (!checkSSID(ssid_to_store)) {                                                                                     /* Check if the SSID is a valid utf8 string */
		return;
	}

	/* -------------- Computing the CRC ------------- */
	snprintf(buff, sizeof(buff), "%02x%02x%02x%02x", crc[0], crc[1], crc[2], crc[3]);
	std::string crc_to_store = buff;

	/* ------------ MAC address check ------------- */
	int local = 0;
	int firstByteMAC = std::stol(address.substr(0, 2), nullptr, 16);                                                     /* Parses str interpreting its content as an integral number of the specified base, */
	int mask1 = 0b00000010;                                                                                              /* global/local bit */
	int mask2 = 0b00000001;                                                                                              /* unicast/multicast bit */
	if ((firstByteMAC & mask1) && !(firstByteMAC & mask2))
		local = 1;                                                                                                       /* local MAC */

	try {

		packetTable.insert("esp_id", "timestamp", "channel", "seq_ctl", "rssi", "addr", "ssid", "crc", "hash", "to_be_deleted")
			.values(espid, receive_time, channel, ctl_to_store, rssi, address, ssid_to_store, crc_to_store, hash, 0).execute();

		if (local)
			localPacketsTable.insert("addr", "timestamp", "seq_ctl", "to_be_deleted")
			.values(address, receive_time, ctl_to_store, 0).execute();

	}
	catch (...) {
		return;
	}
}

/* http://www.zedwood.com/article/cpp-is-valid-utf8-string-function
   Check if the SSID is a valid utf8 string */
bool ProbePacket::checkSSID(const std::string& string) {
	int c, i, ix, n, j;

	for (i = 0, ix = string.length(); i < ix; i++)
	{
		c = (unsigned char)string[i];

		if (0x00 <= c && c <= 0x7f) n = 0;                                                                               /* 0bbbbbbb */
		else if ((c & 0xE0) == 0xC0) n = 1;                                                                              /* 110bbbbb */
		else if (c == 0xed && i < (ix - 1) && ((unsigned char)string[i + 1] & 0xa0) == 0xa0) return false;                 /* U+d800 to U+dfff */
		else if ((c & 0xF0) == 0xE0) n = 2;                                                                              /* 1110bbbb */
		else if ((c & 0xF8) == 0xF0) n = 3;                                                                              /* 11110bbb */
		else return false;

		for (j = 0; j < n && i < ix; j++) {                                                                                  /* n bytes matching 10bbbbbb follow ? */
			if ((++i == ix) || (((unsigned char)string[i] & 0xC0) != 0x80))
				return false;
		}
	}
	return true;
}

/* Print information about the packet */
void ProbePacket::print() {
	printf("%08d  PROBE  CHAN=%02d,  SEQ=%04x,  RSSI=%02d, "
		" ADDR=%02x:%02x:%02x:%02x:%02x:%02x, HASH:%lu ",
		timestamp,
		channel,
		seq_ctl/*[0], seq_ctl[1]*/,
		rssi,
		addr[0], addr[1], addr[2],
		addr[3], addr[4], addr[5],
		hash
	);

	printf("SSID=");
	for (int i = 0; i < ssid_length; i++)
		printf("%c", (char)ssid[i]);
	printf("  CRC=");
	for (int i = 0; i < 4; i++)
		printf("%02x", crc[i]);
	printf("\n");
}

/* Print information about the packet on the last update */
void ProbePacket::print(long int last_update) {
	last_update += timestamp / 1000000;

	printf("%08d  PROBE  CHAN=%02d,  SEQ=%04x,  RSSI=%02d, "
		" ADDR=%02x:%02x:%02x:%02x:%02x:%02x, timestamp=%ld, HASH:%lu ",
		timestamp,
		channel,
		seq_ctl/*[0], seq_ctl[1]*/,
		rssi,
		addr[0], addr[1], addr[2],
		addr[3], addr[4], addr[5],
		last_update,
		hash
	);
	printf("SSID=");
	for (int i = 0; i < ssid_length; i++)
		printf("%c", (char)ssid[i]);
	printf("  CRC=");
	for (int i = 0; i < 4; i++)
		printf("%02x", crc[i]);
	printf("\n");
}