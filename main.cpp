//подключения стандартных библиотек
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
//отключение экстренных оповещений

#pragma warning(disable: 4996)
//создаения сокета тоесть тонеля для общения с сервером
SOCKET Connection;
//

//функция для контроля данного клиента 
void ClientHandler() {
	int msg_size;
	//цикл вайл и переменная для получения сообщения
	while (true) {
		//сначала получаем размер
		recv(Connection, (char*)&msg_size, sizeof(int), NULL);
		//высчитываем его и создаем символьный массив для полученного сообщения ставим по индексу терминатор для предотовращения утечки памяти
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		//получаем само сообщения записываем его в переменную которую мы проиницыализировали
		//и получаем по тому же размеру
		recv(Connection, msg, msg_size, NULL);
		std::cout << msg << std::endl;
		//
		delete[] msg;
	}
}

int main(int argc, char* argv[]) {
	//WSAStartup
	//
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	//
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
		//
	}
	//
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	//
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;
	//
	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		for (int i = 0;i < 5;i++) {
			//
			//std::cout << "Error: failed connect to server.\n";
			//std::cout << "We remove connection for 2 seconds";
			//if (i >= 5) {
			std::cout << "Server closed connection.\n";
			return 1;
			//}
		}
		//
	}
	std::cout << "Connected!\n";
	//
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
	//
	int gg;
	std::string msg1;
	while (true) {
		std::getline(std::cin, msg1);
		//
		int msg_size = msg1.size();
		if (msg1 == "PRmsg") {
			send(Connection, (char*)&msg_size, sizeof(int), NULL);
			send(Connection, msg1.c_str(), msg_size, NULL);
			//
			std::getline(std::cin,msg1);
			send(Connection, msg1.c_str(), sizeof(int), NULL);
		}
		//
		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		send(Connection, msg1.c_str(), msg_size, NULL);
		//
		Sleep(10);
	}

	//
	system("pause");
	return 0;
}