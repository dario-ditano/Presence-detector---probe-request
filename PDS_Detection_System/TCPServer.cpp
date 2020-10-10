/**
 * Copyright (c) 2019, HypnoProject
 * TCP SERVER
 *
 * Bonafede Giacomo
 * Ditano Dario
 * Poppa Emanuel
 *
 */

 /****************************
 * INCLUDE, DEFINE, MACRO   *
 ****************************/
 /*************************************************  INCLUDE  **********************************************************/

#include "stdafx.h"
#include "TCPServer.h"

/*****************************************************
* CONSTANTS, STRUCTS, GLOBAL VARIABLES AND FUNCTIONS *
******************************************************/
/************************************************  FUNCTIONS  *********************************************************/
/* _______________________________________ CONSTRUCTOR __________________________________________ */
TCPServer::TCPServer(long int should_erase_db, long int espn, std::vector<long int> vec) {

	std::cout << "\t\t\t     ____________________________________________" << std::endl;
	std::cout << "\t\t\t    |                                            |" << std::endl;
	std::cout << "\t\t\t    |           TCPServer version 1.0            |" << std::endl;
	std::cout << "\t\t\t    |____________________________________________|\n";

	/* ----------- Connect to the GUI ------------ */
	try { TCPS_pipe_send("TCPServer"); }                                                                                    /* Create PIPE to the GUI */
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		throw;
	}

	/* ------------ Create DataBase -------------- */
	setupDB(should_erase_db);						  																	    /* Re-initialize DB if requested*/

	while (1) {
		try {
			esp_number = espn;
			if (esp_number > 0 && esp_number <= MAX_ESP32_NUM)                                                              /* From 0 to MAX_ESP32_NUM */
				break;
			else {
				std::string s = std::string("system allows from 1 to ") + std::to_string(MAX_ESP32_NUM)
					+ std::string(" ESP32.");
				throw std::exception(s.c_str());
			}
		}
		catch (std::exception& e) {
			std::cout << "+ Input number cannot be accepted for the following reason: " << e.what() << std::endl << std::endl;
		}
	}
	std::cout << std::endl;

	retry = MAX_RETRY_TIMES;                                                                                                /* Numero massimo di tentativi */
	threads_to_wait_for = esp_number;                                                                                       /* Numero di thread per la sync degli ESP */
	esp_to_wait = esp_number;                                                                                               /* Numero di ESP per triangolazione */

	/* ----------- Connect to ESPs ------------ */
	while (retry > 0) {
		try {
			if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) throw std::runtime_error("WSAStartup() failed with error ");     /* WSAStartup function initiates use of the Winsock DLL by a process */
																														    /* wsadata is a pointer to the WSADATA data structure that is to receive
																														     * details of the Windows Sockets implementation */
			/* ----------- Socket Protocol ------------ */
			TCPS_initialize();                                                                                              /* Fill the addrinfo *aresult variable */
			std::cout << "+ TCP Server is correctly initialized with " << esp_number << " ESP32." << std::endl;

			/* ------ 1° Phase - Initialization ------ */
			TCPS_socket();																									/* Passive socket creation */
			TCPS_bind();																									/* Socket Bind */
			TCPS_listen();																									/* Socket Listen */
			TCPS_ask_participation(vec, should_erase_db);																	/* Protocol INIT and READY */
			TCPS_close_listen_socket();																						/* Close listen Socket */

			/* ---- 2° Phase - Socket for packets ---- */
			TCPS_initialize();																								/* Fill the addrinfo *aresult variable */
			TCPS_socket();																									/* Passive socket creation */
			TCPS_bind();																									/* Socket Bind */
			TCPS_listen();																									/* Socket Listen */

			/* ------------- Triangulation ------------ */
			try {
				std::thread(&TCPServer::TCPS_process_packets, this).detach();												/* Thread that handle Packets and their triangulation Triangulation */
			}
			catch (std::exception& e) {
				std::cout << e.what() << std::endl;
				throw;																										/* throw exception to the outer handler */
			}
			std::cout << "\n_____________________________________________________________________________________________" << std::endl;
			std::cout << "\t\t\t\t\t TCP Server is running" << std::endl;
			std::cout << "_____________________________________________________________________________________________" << std::endl;

			/* ----------- Receive packets ------------ */
			TCPS_requests_loop();																							/* Listen for client request */
		}

		/* --------- Exception Handler --------- */
		catch (TCPServer_exception& e) {
			std::cout << e.what() << WSAGetLastError() << std::endl;
			freeaddrinfo(aresult);
			closesocket(listen_socket);
			WSACleanup();
			retry--;
			if (!retry) throw;
		}
		catch (std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			WSACleanup();
			retry--;
			if (!retry) throw;
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			retry--;
			if (!retry) throw;
		}
	}
}

/* ______________________________________ GUI CONNECTION ________________________________________ */
/* ---- connects to the pipe created by the GUI application ---- */
void TCPServer::TCPS_pipe_send(const char* message) {

	namedPipe = CreateFile(PIPENAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);						/* Open PIPE */
	if (namedPipe == INVALID_HANDLE_VALUE) {
		throw std::exception("couldn't open pipe");
	}

	char my_buffer[10];
	DWORD bytesWritten;
	strcpy(my_buffer, message);
	BOOL r = WriteFile(namedPipe, my_buffer, strlen(my_buffer), &bytesWritten, NULL);										/* Send message to the PIPE */
	if (bytesWritten != strlen(my_buffer)) {
		throw std::exception("failed writing on pipe");
	}
	CloseHandle(namedPipe);
}

/* ___________________________________________ DATABASE __________________________________________ */

/* -------- Create the table used to store the packets --------- */
void TCPServer::setupDB(long int should_erase_db)
{
	if (should_erase_db == 0)
		return;
	try {
		mysqlx::Session session("localhost", 33060, "pds_user", "password");

		/* ------------------ PACKET TABLE --------------------- */
		std::string quoted_name = std::string("`pds_db`.`Packet`");
		session.sql(std::string("DROP TABLE IF EXISTS") + quoted_name).execute();
		std::string create = "CREATE TABLE ";
		create += quoted_name;
		create += " (id INT NOT NULL PRIMARY KEY AUTO_INCREMENT, ";
		create += " esp_id INT UNSIGNED, ";
		create += " timestamp TIMESTAMP, ";
		create += " channel TINYINT UNSIGNED, ";
		create += " seq_ctl VARCHAR(4), ";
		create += " rssi TINYINT, ";
		create += " addr VARCHAR(32), ";
		create += " ssid VARCHAR(32), ";
		create += " crc VARCHAR(8), ";
		create += " hash INT UNSIGNED, ";
		create += " to_be_deleted INT )";
		session.sql(create).execute();

		/* -------------------- ESP TABLE ---------------------- */
		quoted_name = std::string("`pds_db`.`ESP`");
		session.sql(std::string("DROP TABLE IF EXISTS") + quoted_name).execute();
		create = "CREATE TABLE ";
		create += quoted_name;
		create += " (mac VARCHAR(32) NOT NULL PRIMARY KEY,";
		create += " esp_id INT NOT NULL,";
		create += " x INT NOT NULL,";
		create += " y INT NOT NULL)";
		session.sql(create).execute();

		/* ------------------ DEVICES TABLE --------------------- */
		quoted_name = std::string("`pds_db`.`Devices`");
		session.sql(std::string("DROP TABLE IF EXISTS") + quoted_name).execute();
		create = "CREATE TABLE ";
		create += quoted_name;
		create += " (dev_id INT NOT NULL PRIMARY KEY AUTO_INCREMENT, ";
		create += " mac VARCHAR(32) NOT NULL,";
		create += " x FLOAT NOT NULL,";
		create += " y FLOAT NOT NULL,";
		create += " timestamp TIMESTAMP)";
		session.sql(create).execute();

		/* --------------- LOCAL MAC DEVICES TABLE --------------- */
		quoted_name = std::string("`pds_db`.`Local_Macs`");
		session.sql(std::string("DROP TABLE IF EXISTS") + quoted_name).execute();
		create = "CREATE TABLE ";
		create += quoted_name;
		create += " (dev_id INT NOT NULL PRIMARY KEY AUTO_INCREMENT, ";
		create += " mac VARCHAR(32) NOT NULL,";
		create += " x FLOAT NOT NULL,";
		create += " y FLOAT NOT NULL,";
		create += " timestamp TIMESTAMP)";
		session.sql(create).execute();

		/* -------- PACKETS WITH LOCAL SOURCE MAC ADDRESS --------- */
		quoted_name = std::string("`pds_db`.`Local_Packets`");
		session.sql(std::string("DROP TABLE IF EXISTS") + quoted_name).execute();
		create = "CREATE TABLE ";
		create += quoted_name;
		create += " (id INT NOT NULL PRIMARY KEY AUTO_INCREMENT, ";
		create += " addr VARCHAR(32), ";
		create += " timestamp TIMESTAMP, ";
		create += " seq_ctl VARCHAR(4), ";
		create += " to_be_deleted INT )";
		session.sql(create).execute();

	}
	catch (std::exception& err) {
		std::cout << "The following error occurred: " << err.what() << std::endl;
	}
	std::cout << "\n+ Database correctly initialized." << std::endl;
}

/* ____________________________________ CONNECTION PROTOCOL _____________________________________ */
/* --------- initialize winsock, may throw exception ----------- */
void TCPServer::TCPS_initialize() {

	if (aresult) freeaddrinfo(aresult);																						/* Check TCP Server variable aresult */
	ZeroMemory(&hints, sizeof(hints));																						/* Fills a block of memory with zeros */
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int result = getaddrinfo(NULL, DEFAULT_PORT, &hints, &aresult);															/* getaddrinfo function provides protocol-independent translation
																															 * from an ANSI host name to an address */
	if (result != 0) {
		throw std::runtime_error("getaddrinfo() failed with error ");
	}
}

/* ----------- calls socket(), may throw exception ------------- */
void TCPServer::TCPS_socket() {
	listen_socket = socket(aresult->ai_family, aresult->ai_socktype, aresult->ai_protocol);									/* The socket function creates a socket that is bound to a specific
																															 * transport service provider */
	if (listen_socket == INVALID_SOCKET) {
		freeaddrinfo(aresult);
		throw std::runtime_error("socket() failed with error ");
	}
}

/* ------------ calls bind(), may throw exception -------------- */
void TCPServer::TCPS_bind() {
	int result = bind(listen_socket, aresult->ai_addr, (int)aresult->ai_addrlen);											/* The bind function associates a local address with a socket */
	if (result == SOCKET_ERROR) {
		throw TCPServer_exception("bind() failed");
	}
}

/* ----------- calls listen(), may throw exception --------------*/
void TCPServer::TCPS_listen() {
	int result = listen(listen_socket, SOMAXCONN);																			/* The listen function places a socket in a state in which it is
 *																															 * listening for an incoming connection */
	if (result == SOCKET_ERROR) {
		throw TCPServer_exception("listen() failed with error ");
	}
}

/* ------------ listen for ESP32 joining requests -------------- */
void TCPServer::TCPS_ask_participation(std::vector<long int> vec, long int should_erase_db) {								/* Interact with the -----> void esp_initialization() function of the ESP32 Code */

	std::cout << "----------------------------------------------------------------------------------------------" << std::endl << std::endl;
	std::cout << "\n\n\t\t\t  Please unplug all the ESP32 from the energy source " << std::endl;
	std::cout << "\t\t\t  --------------------------------------------------" << std::endl << std::endl;
	std::thread threads[MAX_ESP32_NUM];																						/* create threads with no execution flow */

	for (int i = 0; i < esp_number * 2; i += 2) {                                                                           /* for each ESP32 ask for its position in the space and its INIT packet */

		int posx, posy;																										/* Coordinates of the current ESP32 */
		uint8_t mac[6];																										/* MAC address of the current ESP32 */

		try {
			posx = vec[i], posy = vec[i + 1];                                                                               /* Save the current coordinates */
			std::cout << "__________________ ";
			std::cout << "\nESP32 number " << i / 2 << std::endl;
			std::cout << "X coordinate: " << posx;

			std::cout << std::endl << "Y coordinate: " << posy;;
			std::cout << "\n__________________ ";
			std::cout << std::endl;
		}
		catch (std::exception& e) {
			std::cout << "Input number cannot be accepted for the following reason: " << e.what() << std::endl << std::endl;
			i--;
			continue;
		}
		std::cout << "You can now plug this ESP32. Waiting for ESP32 detection..." << std::endl;							/* Request of plug the current ESP32 */

		/* ------------ SOCKET TCP Protocol ------------- */
		while (1) {																											/* waiting for a INIT packet coming from the i-th ESP32.
																															   If something else is sent, just ignore it. */

			SOCKET client_socket = accept(listen_socket, NULL, NULL);														/* Accept request on the active socket */
			if (client_socket == INVALID_SOCKET) {
				throw TCPServer_exception("accept() failed with error ");
			}

			std::cout << "+ Connected to the client" << std::endl;

			int result = 0;
			char* recvbuf = (char*)malloc(10);

			for (int i = 0; i < 10; i = i + result)																			/* Listen fir the INIT_MSG + MAC address*/
				result = recv(client_socket, recvbuf + i, 10 - i, 0);

			if (result > 0) {
				if (memcmp(recvbuf, INIT_MSG_H, 4) == 0) {																	/* The ESP32 sent an init message */
					std::cout << "+ A new ESP32 has been detected." << std::endl;
					std::cout << "\t ------------------------------------" << std::endl;
					std::cout << "\t| Its MAC address: ";

					memcpy(mac, recvbuf + 4, 6);																			/* Retrieve the MAC address */
					printf("%02x:%02x:%02x:%02x:%02x:%02x |\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
					std::cout << "\t ------------------------------------" << std::endl;
					char s[8];
					memcpy(s, INIT_MSG_H, 4);
					int port = FIRST_READY_PORT + i;
					std::string str = std::to_string(port);
					memcpy(s + 4, str.c_str(), 4);																			/* INIT_MSG + PORT */

					int send_result = send(client_socket, s, 8, 0);															/* Sending joining confirm */
					if (send_result == SOCKET_ERROR) {
						if (WSAGetLastError() == 10054) {																	/* Connection reset by peer.
																															   An existing connection was forcibly closed by the remote host.
																															   This normally results if the peer application on the remote host is suddenly stopped,
																															   the host is rebooted, the host or remote network interface is disabled,
																															   or the remote host uses a hard close */
							continue;
						}
						else {
							free(recvbuf);
							throw TCPServer_exception("send() failed with error ");
						}
					}
					std::cout << "+ Confirmation sent." << std::endl;
					free(recvbuf);

					ESP32 espdata(i / 2, mac, posx, posy, port);															/* arguments: id, mac address, x pos, y pos, ready port for socket creation */
					if (should_erase_db)
						espdata.store_esp();																				/* Insert new ESP32 in the ESP table in the DB */
					esp_list.push_back(espdata);																			/* Insert the ESP32 in the list of ESP */

					break;																									/* Return to the outer for loop */
				}
				else {
					free(recvbuf);																							/* Incorrect formatting of the request; ignore it. */
					continue;
				}
			}
			else if (result < 0) {
				free(recvbuf);
				throw TCPServer_exception("recv() failed with error ");
			}
		}

		/* -------------- THREAD Creation -------------- */
		std::cout << "---------------------------------------------------" << std::endl;
		try {
			threads[i / 2] = std::thread(&TCPServer::TCPS_ready_channel, this, i / 2);                                       /* Assign to a thread the job to handle a new socket for ready packets. */
		}
		
		/* -------- Exception Handler ---------- */
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			throw;
		}
	}
	for (int j = 0; j < MAX_ESP32_NUM; j++) {																				/* Before going to the next phase, the main thread will wait all the other threads
																															   (that have an execution flow) to terminate their tasks on the ready socket. */
		if (threads[j].joinable())
			threads[j].join();																								/* Join thread */
	}
}

/* ------------ create a connection with the ESP32 ------------- */
void TCPServer::TCPS_ready_channel(int esp_id) {																			/* Interact with the -----> void esp_is_ready() function of the ESP32 code */

	SOCKET sock;

	try {																													/* SOCKET parameters */
		addrinfo h, * ainfo;
		ZeroMemory(&h, sizeof(h));
		h.ai_family = AF_INET;
		h.ai_socktype = SOCK_STREAM;
		h.ai_protocol = IPPROTO_TCP;
		h.ai_flags = AI_PASSIVE;

		std::string s = std::to_string(esp_list[esp_id].get_port());														/* converting the port number in a format getaddrinfo understands */

		/* ---------- TCP SOCKET Protocol ---------- */
		if (((getaddrinfo(NULL, s.c_str(), &h, &ainfo)) != 0))
			throw std::runtime_error("CHILD THREAD [READY] - getaddrinfo() failed with error ");

		sock = socket(ainfo->ai_family, ainfo->ai_socktype, ainfo->ai_protocol);											/* Socket creation */
		if (sock < 0) {
			freeaddrinfo(ainfo);
			throw std::runtime_error("CHILD THREAD [READY] - socket() failed with error ");
		}

		ready_sockets.push(sock);																							/* Insert sock in the BlockingQueue Thread safe structure */
		if (bind(sock, ainfo->ai_addr, (int)ainfo->ai_addrlen) == SOCKET_ERROR)
			throw TCPServer_exception("CHILD THREAD [READY] - bind() failed");

		if (listen(sock, 1) == SOCKET_ERROR)																				/* Socket listen for to the ESP32 */
			throw TCPServer_exception("CHILD THREAD [READY] - listen() failed with error ");

		while (1) {
			SOCKET c_sock = accept(sock, NULL, NULL);																		/* Active Socket to the ESP32 */
			if (c_sock == INVALID_SOCKET)
				throw TCPServer_exception("CHILD THREAD [READY] - accept() failed with error ");

			char recvbuf[5];
			int res = 0;

			for (int i = 0; i < 5; i = i + res) {
				res = recv(c_sock, recvbuf + i, 5 - i, 0);																	/* waiting to receive a READY message READY_MSG_H */
			}

			if (res > 0) {
				if (memcmp(recvbuf, READY_MSG_H, 5) == 0) {
					{
						std::unique_lock<std::mutex> ul(mtx);																/* Once the ready packet is arrived, notify all the threads waiting
																															   to answer READY to the clients. */
						threads_to_wait_for--;
						cvar.notify_all();
						//std::cout << "1";
					}

					std::unique_lock<std::mutex> ul(mtx);																	/* Once every thread has received the ready packet, the threads can tell the esp32
																															   to begin sniffing. */
					//std::cout << "2";
					cvar.wait(ul, [this]() { return threads_to_wait_for == 0; });
					//std::cout << "3";
					char sendbuf[5];
					strncpy_s(sendbuf, 6, READY_MSG_H, 5);																	/* Send the ready message */

					int send_result = send(c_sock, sendbuf, 5, 0);
					if (send_result == SOCKET_ERROR) {
						if (WSAGetLastError() == 10054) {																	/* Connection reset by peer */
							continue;
						}
						else {
							throw TCPServer_exception("CHILD THREAD [READY] - send() failed with error ");
						}
					}
					esp_list[esp_id].update_time();																			/* Record the time when the ESP starts */
					std::cout << "+ CONFIGURATION COMPLETE for ESP with id " << esp_id << " on port " << s << ".\n";
					break;
				}
				else {
					continue;
				}
			}
			else if (res < 0) {
				throw TCPServer_exception("CHILD THREAD [READY] - recv() failed with error ");
			}
		}
	}

	/* -------- Exception Handler ---------- */
	catch (TCPServer_exception& e) {
		std::cout << e.what() << WSAGetLastError() << std::endl;
		freeaddrinfo(aresult);
		closesocket(sock);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

/* --------- calls closesocket() on the listen socket ---------- */
void TCPServer::TCPS_close_listen_socket() {
	int result = closesocket(listen_socket);
	if (result == SOCKET_ERROR) {
		throw TCPServer_exception("closesocket() failed with error ");
	}
}

/* ________________________________________ TRIANGULATION ________________________________________ */
/* ------ Method that calls the triangulation when needed ------ */
//std::cout << "Coordinates of " << current_address << " : X=" << pos_x << ", Y=" << pos_y << std::endl << std::endl;
void TCPServer::TCPS_process_packets() {

	PacketProcessor pckt_rfn(esp_number);																					/* Packet Processor Constructor */
	while (1) {
		{
			std::unique_lock<std::mutex> ul(triang_mtx);
			triang_cvar.wait(ul, [this]() { return esp_to_wait == 0; });													/* Unlock when esp_to_wait == 0 */
			esp_to_wait = esp_number;																						/* Set the number of esp_to_wait */
		}
		pckt_rfn.process();																									/* Process the packets sniffed */
		std::cout << "===== Finished processing packets =====" << std::endl;
	}
}


/* ---------- loops to client's connection requests ------------ */
void TCPServer::TCPS_requests_loop() {
	while (1) {
		/* ------- Accepting Client request ------- */
		SOCKET client_socket = accept(listen_socket, NULL, NULL);															/* Creazione Socket connesso */
		if (client_socket == INVALID_SOCKET) {
			throw TCPServer_exception("accept() failed with error ");
		}

		/* ---- Receiving Packets from Client ----- */
		try {
			std::thread(&TCPServer::TCPS_service, this, client_socket).detach();
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			throw;
		}
	}
}

/* --- stores in the blocking queue client's sniffed packets --- */
void TCPServer::TCPS_service(SOCKET client_socket) {

	uint8_t retry_child = MAX_RETRY_TIMES;																					/* Number of attempts */
	char c;
	uint8_t count;

	// std::cout << "Running child thread" << std::endl;

	while (retry_child > 0) {
		try {
			int result = 0;

			/* -------- MAC address from client -------- */
			uint8_t mac[6];
			char rbuff[6];
			for (int i = 0; i < 6; i = i + result)
				result = recv(client_socket, rbuff + i, 6 - i, 0);															/* Receive MAC address */

			if (result < 0) throw TCPServer_exception("mac_addr recv() failed with error ");

			memcpy(mac, rbuff, 6);

			int esp_id = get_esp_instance(mac);																				/* Get the instance of the device from the list created during
																															   setup with the mac received from the socket */
			esp_list[esp_id].update_time();																					/* Update the time since last update */

			/* ---------- Packets from client ---------- */
			recv(client_socket, &c, 1, 0);																					/* Receiving packet counter from client */
			count = c - '0';
			// printf("Receiving %u packets from ESP with MAC address %02x:%02x:%02x:%02x:%02x:%02x\n", count, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
			//std::cout << "--- New request accepted from client ---" << std::endl;
			char* recvbuf = (char*)malloc(count * PACKET_SIZE);                                                             /* Create space for the incoming packets */
			int recvbuflen = count * PACKET_SIZE;																			/* Total dimension of the packets */

			for (int i = 0; i < recvbuflen; i = i + result)																	/* Receive the effective packets */
				result = recv(client_socket, recvbuf + i, recvbuflen - i, 0);

			if (result > 0) {
				// std::cout << "\t+ Bytes received: " << result << std::endl;
				// std::cout << "Buffer size: " << recvbuflen << std::endl;

				printf("---------------------\n");
				printf("| %02x:%02x:%02x:%02x:%02x:%02x |\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
				printf("---------------------\n");
				std::cout << "+ Current Time: " << std::time(NULL) << ", passed since last update: " << esp_list[esp_id].get_update_interval() << std::endl;

				storePackets(count, esp_id, recvbuf);																		/* Store and print them */

				// notify an esp has sent its data
				{
					std::unique_lock<std::mutex> ul(triang_mtx);
					esp_to_wait--;
					triang_cvar.notify_all();
				}

				// just send back a packet to the client as ack
				int send_result = send(client_socket, recvbuf, PACKET_SIZE, 0);
				if (send_result == SOCKET_ERROR) {
					// connection reset by peer
					if (WSAGetLastError() == 10054) {
						continue;
					}
					else {
						free(recvbuf);
						throw TCPServer_exception("send() failed with error ");
					}
				}
				//std::cout << "Bytes sent back: " << send_result << std::endl;

			}
			else if (result == 0) {
				std::cout << "Connection closing" << std::endl;
			}
			else {
				// connection reset by peer
				if (WSAGetLastError() == 10054) {
					continue;
				}
				else {
					free(recvbuf);
					throw TCPServer_exception("send() failed with error ");
				}
			}

			free(recvbuf);
			break;
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			retry_child--;
			if (!retry_child) {
				std::cout << "AN EXCEPTION OCCURRED ON CHILD THREAD, TERMINATE APPLICATION" << std::endl;
				break;
			}
		}
	}

	//std::cout << "--- Child thread correctly ended ---" << std::endl;
}

/* Store probe packets received from an ESP32 */
void TCPServer::storePackets(int count, int esp_id, char* recvbuf) {
	try {
		long int time_since_last_update = esp_list[esp_id].get_previous_update_time();

		mysqlx::Session session("localhost", 33060, "pds_user", "password");												/* Connect to server using a connection URL */

		try {
			mysqlx::Schema myDb = session.getSchema("pds_db");																/* Get DB schema */
			mysqlx::Table packetTable = myDb.getTable("Packet");															/* Accessing the packet table */
			mysqlx::Table localPacketsTable = myDb.getTable("Local_Packets");												/* Accessing the local packet table */

			/* --------- Store Probe Packets --------- */
			for (int i = 0; i < count; i++) {
				ProbePacket pp;
				memcpy(&pp, recvbuf + (i * PACKET_SIZE), PACKET_SIZE);                                                      /* Insert received info in the ProbePacket pp variable */
				pp.storeInDB(packetTable, localPacketsTable, time_since_last_update, esp_id);								/* Store the received packet in the DB */
			}
		}
		catch (std::exception& err) {
			std::cout << "The following error occurred: " << err.what() << std::endl;
			exit(1);																										/* Exit with error code */
		}
	}
	catch (std::exception& err) {
		std::cout << "The database session could not be opened: " << err.what() << std::endl;
		exit(1);																											/* Exit with error code */
	}
}

/* --------------------- calls shutdown() ---------------------- */
void TCPServer::TCPS_shutdown(SOCKET client_socket) {
	int result = shutdown(client_socket, SD_SEND);
	if (result == SOCKET_ERROR) {
		throw TCPServer_exception("shutdown() failed with error ");
	}
	freeaddrinfo(aresult);
	closesocket(listen_socket);
	WSACleanup();
}

/* Get ESP32 id given the MAC */
int TCPServer::get_esp_instance(uint8_t* mac) {
	for (int i = 0; i < esp_number; i++) {
		if (memcmp(mac, esp_list[i].get_mac_address_ptr(), 6) == 0) {
			return i;
		}
	}
	throw std::exception("No esp with such a MAC has been found.");
}



