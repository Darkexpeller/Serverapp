#pragma once
#include <iostream>
#include <winsock2.h>
#include<string.h>
#include<fstream>
#include"socket_info.h"
#include"registere.hpp"
#include"SOCKETLIST.h"
#define PASSWORD_ERROR  0x0
#define PASSWPRD_TRUE   0x1
#define ACCOUNT_NULL    0X2
#define _WINSOCK_DEPRECATED_NOWARNINGS
#define  FRIENDMESSAGE 0X20
#define FRIENDLIST_BEGIN 0x40
#define FRIENDLIST_END 0x79
#define PASS            0X4
#define ACCOUNT_Reg 0x3
using namespace std;


void checkvector(vector<socket_info>& S)
{
	for (int i = 0; i <S.size(); i++)
	{
		if ( *(S[i].ClientSocket) == SOCKET_ERROR)
		{
			cout << "刪除" << S[i].User_ID << endl;
			S.erase(S.begin() + i);//删除指定元素
			i--;
		}
	}
}
struct Acinfo
{
	char pr[4];
	char psd[20];
};
struct FriNode  //好友链表
{
	char* ID;
	FriNode* next;
};
int friendlist(fstream fin)       //读取好友队列
{
	return 1;
}


int login_verify(SOCKET* serConn,SOCKETLIST* socketlist)
{
	char recvbuf[sizeof(AccountMessage)];
	memset(recvbuf, 0, sizeof(recvbuf));
	recv(*serConn, recvbuf, sizeof(recvbuf), 0);
	AccountMessage Accountinfo;
	memset(Accountinfo.account, 0, sizeof(Accountinfo.account));
	memset(Accountinfo.pas, 0, sizeof(Accountinfo.pas));
	memcpy(&Accountinfo,recvbuf, sizeof(Accountinfo));



	Acinfo acinfo;
	string dir1="userdata";
	string dir2 = Accountinfo.account;
	string Suffix = ".acinfo";
	string filename = "./userdata/" + dir2 + "/" + dir2 + Suffix;
	fstream fin;
	fin.open(filename, ios::in);
	cout << filename << endl;
	cout << Accountinfo.account << " " << Accountinfo.pas << endl;
	if (fin.is_open() == false)
	{
		char b = ACCOUNT_NULL;
		send(*serConn, &b, sizeof(b), 0);
		char recv_ack = 0;                                        //接受确认码，如果为1自动注册账号；
		//recv(*serConn, &recv_ack, sizeof(recv_ack), 0);
		if (recv_ack == 1)
		{
			fs::create_directory(("./" + dir1 + "/" + dir2));
			fstream fuser(filename, ios::out);
			fstream fuserfriend(("./userdata/" + dir2 + "/" + dir2 + ".firend"), ios::out);
			fuser.close();
			fuserfriend.close();
			fstream fuser_2(filename, ios::out | ios::in);
			encode(Accountinfo.pas);
			fuser << Accountinfo.pas << endl;
			fuser.close();
		}
		char c = ACCOUNT_Reg;
		send(*serConn, &c, sizeof(b), 0);
		return ACCOUNT_NULL;
	}
	char pas[20];
	fin.getline(pas, 20, '\n');
	//encode(pas);                                      //对储存的密码进行解密
	encode(Accountinfo.pas);                         //接受的密码进行解密
	if (strcmp(pas, Accountinfo.pas) == 0)
	{
		char b = PASS;
		send(*serConn, &b, sizeof(b), 0);
		socket_info* newuser=new socket_info(serConn, Accountinfo.account);
		FriMessage Friend;
		memset(&Friend, 0, sizeof(Friend));
		Friend.TYPE = FRIENDLIST_BEGIN;
		char buf[sizeof(Friend)];
		memset(buf, 0, sizeof(Friend));
		memcpy(buf, &Friend, sizeof(Friend));
		char head = FRIENDLIST_BEGIN; //发送好友列表标志
		for (int i=0; i < socketlist->size(); i++)
		{
			if (strcmp(Accountinfo.account, (*socketlist)[i]->User_ID)==0)
			{

				goto no;
			}
		}
		socketlist->push_back(serConn, Accountinfo.account);     //将新用户压入vector
		for (int j = 0; j < socketlist->size(); j++)
		{
			send(*(*socketlist)[j]->ClientSocket, buf, sizeof(Friend), 0);
			head = FRIENDLIST_BEGIN;
			send(*(*socketlist)[j]->ClientSocket, &head, sizeof(head), 0);
			head = socketlist->size();
			send(*(*socketlist)[j]->ClientSocket, &head, sizeof(head), 0);//发送好友数量
			for (int i = 0; i < socketlist->size(); i++)
			{
				cout << "(*socketlist)[" << i << "]->User_ID = " << (*socketlist)[i]->User_ID << endl;
				send(*(*socketlist)[j]->ClientSocket, (*socketlist)[i]->User_ID, sizeof((*socketlist)[i]->User_ID), 0);
				//cout << (*socketlist)[i]->User_ID;

			}
			cout << "you" << (int)head << endl;;
			head = FRIENDLIST_END;
			send(*(*socketlist)[j]->ClientSocket, &head, 1, 0); //结束在线好友列表的发送
		}
	no:

		cout << Accountinfo.account << "成功登录" << endl;
		return PASS;
	}
	else
	{
		char b = PASSWORD_ERROR;
		send(*serConn, &b, sizeof(b), 0);
		return PASSWORD_ERROR;
	}
}