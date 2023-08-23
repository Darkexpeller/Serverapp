#pragma once
#include <iostream>
#include <winsock2.h>
#include<string.h>
#include<vector>
using namespace std;

class socket_info
{
public:
	socket_info(SOCKET* socket, char User_ID[10]);
	SOCKET* ClientSocket;
	char User_ID[10];
	//vector<string>Friendlist;//好友队列
};
struct FriMessage
{
	char TYPE;
	char ID_from[10];
	char ID_aim[10];
	char MessageMain[1024];
	int Size = sizeof(FriMessage);
};

