#pragma once
//SOCKETLIST.h
#include<winsock.h>
#include<iostream>

//socket∂”¡–
class SOCKETLIST
{
public:
	SOCKETLIST(SOCKET* socket, char* ID)
	{
		ClientSocket = socket;
		strcpy(User_ID, ID);
		next = NULL;
	}
	SOCKETLIST()
	{
		ClientSocket =NULL;
		memset(User_ID, 0, sizeof(User_ID));
		next = NULL;
	}

public:
	SOCKETLIST* next;
	SOCKET* ClientSocket;
	char User_ID[10];
	int push_back(SOCKET* socket, char* ID);
	SOCKETLIST* operator[](int i);
	int deuser(int val);
	int size();

};

