#include"account.hpp"

#pragma comment(lib,"ws2_32.lib")
using namespace std;


struct Parm
{
	SOCKETLIST* socketlist;
	SOCKET* SerConn;
};
DWORD WINAPI ThreadProc(LPVOID lp)
{
	char USer_ID[10];//记录当前socket对应的UserID以动态标记位置

	Parm* parm = (Parm*)lp;
	SOCKETLIST* socketlist = parm->socketlist;
	cout << 1 << endl;
	cout << "socketlist->size()=" << socketlist->size() << endl;
	strcpy_s(USer_ID, (*socketlist)[socketlist->size() - 1]->User_ID);
	char Buf[sizeof(FriMessage)];
	

	SOCKET* serConn = parm->SerConn;
	cout << 2<<endl;
	memset(&Buf, 0, sizeof(Buf));
	/*
	//首先发送好友列表
	char FriID[10];
	memset(&FriID, 0, sizeof(FriID));
	char head = FRIENDLIST_BEGIN; //发送好友列表标志
	send(*serConn, &head, sizeof(head),0 );
	head = socketlist->size();
	send(*serConn, &head, sizeof(head), 0);//发送好友数量
	for (int i = 0; i < socketlist->size(); i++)
	{
		cout << "(*socketlist)["<<i<<"]->User_ID = " << (*socketlist)[i]->User_ID << endl;
		send(*serConn, (*socketlist)[i]->User_ID, sizeof(USer_ID), 0);
		//cout << (*socketlist)[i]->User_ID;
		
	}
	cout << "you" << (int)head << endl;;
	head = FRIENDLIST_END;
	send(*serConn, &head, 1, 0); //结束在线好友列表的发送
	*/
	//int i = 0;
	while(true)
	{ 
		memset(&Buf, 0, sizeof(Buf));
		if (recv(*serConn, Buf, sizeof(Buf), 0) != SOCKET_ERROR)
		{
			cout << "收到消息" << endl;
			cout << (int)Buf[0] << endl;
			switch (Buf[0])
			{
			case FRIENDMESSAGE:
			{
				cout << "收到好友消息" << endl;
				FriMessage message;
				memset(&message, 0, sizeof(message));
				//memset(&message.TYPE, 0, sizeof(message));
				memcpy(&message, Buf, sizeof(message));
				cout << "来自" << message.ID_from << "向" << message.ID_aim << "发送的" << message.MessageMain << endl;
				for (int i = 0; i < socketlist->size(); i++)
				{
					cout << "执行for循环" << endl;
					if (strcmp(message.ID_aim, (*socketlist)[i]->User_ID)==0)
					{
						send(*((*socketlist)[i]->ClientSocket), Buf, sizeof(Buf), 0);
						cout << "来自" << message.ID_from << "向" << message.ID_aim<< "发送的" << message.MessageMain << endl;
						cout << "该线程是" << USer_ID << " " << "socketlist[i].User_ID)=" << (*socketlist)[i]->User_ID << endl;
						for (int k = 0; k < 30; k++)
						{
							cout << " " <<(int) Buf[k];
						}
						cout << endl;
						for (int i = 0; i < socketlist->size(); i++)
						{
							cout << (*socketlist)[i]->User_ID << " " << socketlist->size()<< endl;
						}
						break;
					}
					if (i ==( socketlist->size()-1))
					{
						cout << "来自" << USer_ID << "无目标" << endl;
					}
				}
				break;
			}
			default:
				cout << "非好友消息" << endl;
				break;
			}


			/*
			 cout << i++;
			 cout << "收到：" << Buf << endl;
			*/
		}
		else
		{
			cout << USer_ID<<" 连接已断开" << endl;
			for (int i = 0; i < socketlist->size(); i++)
			{
				cout << (*socketlist)[i]->User_ID << " " << socketlist->size() << endl;
			}
			for (int i = 0; i < socketlist->size(); i++)
			{
				if (strcmp((*socketlist)[i]->User_ID , USer_ID)==0)
				{
					socketlist->deuser(i);//删除指定元素
				}
			}
			cout << "删除后" << endl;
			for (int i = 0; i < socketlist->size(); i++)
			{
				cout << (*socketlist)[i]->User_ID << " " << socketlist->size() << endl;
			}
			
			return 0;//如果线程函数return，返回值会隐式调用ExitThread函数，可以使用GetExitCodeThread函数获得该线程函数的返回值。
		}
		
	}
	
	return 0;
}/*
DWORD WINAPI Threadsend(LPVOID lp)
{
	char Buf[1024];
	SOCKET serConn = *(SOCKET*)lp;
	while (1)
	{
		cout << "发出:";
		gets_s(Buf, 1024);
		if (send(serConn, Buf, 1024, 0) == SOCKET_ERROR)
		{
			cout << "连接已断开" << endl;
			return 0;
		}
		
	}
	
	return 0;
}
*/
int main()
{
	SOCKETLIST socketlist;
	char sendBuf[1024];
	char receiveBuf[1024];

		//创建套接字，socket前的一些检查工作.
	//服务的启动
		WSADATA wsadata;//wsa 即windows socket async 异步套接字
		if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata))
		{
			cout << "服务器套接字未打开" << endl;
			return 0;
		}
		else
		{
			cout << "服务器已打开套接字" << endl;
		}
		SOCKET serSocket = socket(AF_INET, SOCK_STREAM, 0);//创建可识别的套接字//parm1: af 地址协议族 ipv4 ipv6
		//parm2:type 传输协议类型 流式套接字（SOCK_STREAM)，数据包套接字（SOCK_DGRAM)
		//parm3:ptotoc1 使用具体的某个传输协议

		SOCKADDR_IN addr;                                  //需要绑定的参数，主要是本地的socket的一些信息。
		addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);     //ip地址，htonl即host本机 to:to  n:net l:unsigned long 大端存储，低字节在高位
		addr.sin_family = AF_INET;
		addr.sin_port = htons(6000);                       //端口 htons将无符号短整型转化为网络字节序

		bind(serSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));//绑定完成
		listen(serSocket, 5);                               //监听窗口
		SOCKADDR_IN clientsocket;
		int len = sizeof(SOCKADDR);



		while (true)
		{
			
			SOCKET* serConn =new SOCKET;//于客户端建立链接
			*serConn = accept(serSocket, (SOCKADDR*)&clientsocket, &len);
			if (*serConn == SOCKET_ERROR)
			{
				cout << "连接建立失败" << endl;
			}
			else
			{
				Parm parm;//传递容器和socketlist进入线程
				parm.SerConn = serConn;
				parm.socketlist = &socketlist;
				cout << "成功建立连接" << endl;
				if (login_verify(serConn,&socketlist) == PASS)
				{
					CreateThread(NULL, NULL, ThreadProc, &parm, NULL, NULL);//(LPVOID)
					//CreateThread(NULL, NULL, Threadsend, (LPVOID)&serConn, NULL, NULL);
				}
				else
				{
					closesocket(*serConn);
				}
			}
		}
		


		//closesocket(serConn);//关闭
		WSACleanup();//释放资源
	return 0;
}
