#pragma once
#include <iostream>
#include <winsock2.h>
#include<string.h>
#include<fstream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

int encode(char* code)
{
	for (int i = 0; *(code + i) != '\0'; i++)
	{
		*(code + i) ^= 0x18;
	}
	return 1;
}
struct AccountMessage
{
	char account[10];
	char pas[20];
};
int RegAccount(SOCKET serConn)
{
	
	char recvbuf[sizeof(AccountMessage)];
	memset(recvbuf, 0, sizeof(recvbuf));
	recv(serConn, recvbuf, sizeof(recvbuf), 0);
	AccountMessage Accountinfo;
	memset(Accountinfo.account, 0, sizeof(Accountinfo.account));
	memset(Accountinfo.pas, 0, sizeof(Accountinfo.pas));
	memcpy(&Accountinfo, recvbuf, sizeof(Accountinfo));
	string dir1 = "userdata";
	string dir2 = Accountinfo.account;
	string Suffix = ".acinfo";
	string UserDir = "./userdata/" + dir2 + "/" + dir2 + Suffix;
	fstream fin;
	fin.open(UserDir, ios::in);
	if(fin.is_open() == false)
	{
		fs::create_directory(("./"+dir1 +"/"+ dir2));
		fstream fuser(UserDir, ios::out);
		fstream fuserfriend(("./userdata/" + dir2 + "/" + dir2 + ".firend"), ios::out);
		fuser.close();
		fuserfriend.close();
		fstream fuser_2(UserDir, ios::out | ios::in);
		encode(Accountinfo.pas);
		fuser << Accountinfo.pas<<endl;
		fuser.close();
	}
	return 0;
}