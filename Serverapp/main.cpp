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
	char USer_ID[10];//��¼��ǰsocket��Ӧ��UserID�Զ�̬���λ��

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
	//���ȷ��ͺ����б�
	char FriID[10];
	memset(&FriID, 0, sizeof(FriID));
	char head = FRIENDLIST_BEGIN; //���ͺ����б��־
	send(*serConn, &head, sizeof(head),0 );
	head = socketlist->size();
	send(*serConn, &head, sizeof(head), 0);//���ͺ�������
	for (int i = 0; i < socketlist->size(); i++)
	{
		cout << "(*socketlist)["<<i<<"]->User_ID = " << (*socketlist)[i]->User_ID << endl;
		send(*serConn, (*socketlist)[i]->User_ID, sizeof(USer_ID), 0);
		//cout << (*socketlist)[i]->User_ID;
		
	}
	cout << "you" << (int)head << endl;;
	head = FRIENDLIST_END;
	send(*serConn, &head, 1, 0); //�������ߺ����б�ķ���
	*/
	//int i = 0;
	while(true)
	{ 
		memset(&Buf, 0, sizeof(Buf));
		if (recv(*serConn, Buf, sizeof(Buf), 0) != SOCKET_ERROR)
		{
			cout << "�յ���Ϣ" << endl;
			cout << (int)Buf[0] << endl;
			switch (Buf[0])
			{
			case FRIENDMESSAGE:
			{
				cout << "�յ�������Ϣ" << endl;
				FriMessage message;
				memset(&message, 0, sizeof(message));
				//memset(&message.TYPE, 0, sizeof(message));
				memcpy(&message, Buf, sizeof(message));
				cout << "����" << message.ID_from << "��" << message.ID_aim << "���͵�" << message.MessageMain << endl;
				for (int i = 0; i < socketlist->size(); i++)
				{
					cout << "ִ��forѭ��" << endl;
					if (strcmp(message.ID_aim, (*socketlist)[i]->User_ID)==0)
					{
						send(*((*socketlist)[i]->ClientSocket), Buf, sizeof(Buf), 0);
						cout << "����" << message.ID_from << "��" << message.ID_aim<< "���͵�" << message.MessageMain << endl;
						cout << "���߳���" << USer_ID << " " << "socketlist[i].User_ID)=" << (*socketlist)[i]->User_ID << endl;
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
						cout << "����" << USer_ID << "��Ŀ��" << endl;
					}
				}
				break;
			}
			default:
				cout << "�Ǻ�����Ϣ" << endl;
				break;
			}


			/*
			 cout << i++;
			 cout << "�յ���" << Buf << endl;
			*/
		}
		else
		{
			cout << USer_ID<<" �����ѶϿ�" << endl;
			for (int i = 0; i < socketlist->size(); i++)
			{
				cout << (*socketlist)[i]->User_ID << " " << socketlist->size() << endl;
			}
			for (int i = 0; i < socketlist->size(); i++)
			{
				if (strcmp((*socketlist)[i]->User_ID , USer_ID)==0)
				{
					socketlist->deuser(i);//ɾ��ָ��Ԫ��
				}
			}
			cout << "ɾ����" << endl;
			for (int i = 0; i < socketlist->size(); i++)
			{
				cout << (*socketlist)[i]->User_ID << " " << socketlist->size() << endl;
			}
			
			return 0;//����̺߳���return������ֵ����ʽ����ExitThread����������ʹ��GetExitCodeThread������ø��̺߳����ķ���ֵ��
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
		cout << "����:";
		gets_s(Buf, 1024);
		if (send(serConn, Buf, 1024, 0) == SOCKET_ERROR)
		{
			cout << "�����ѶϿ�" << endl;
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

		//�����׽��֣�socketǰ��һЩ��鹤��.
	//���������
		WSADATA wsadata;//wsa ��windows socket async �첽�׽���
		if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata))
		{
			cout << "�������׽���δ��" << endl;
			return 0;
		}
		else
		{
			cout << "�������Ѵ��׽���" << endl;
		}
		SOCKET serSocket = socket(AF_INET, SOCK_STREAM, 0);//������ʶ����׽���//parm1: af ��ַЭ���� ipv4 ipv6
		//parm2:type ����Э������ ��ʽ�׽��֣�SOCK_STREAM)�����ݰ��׽��֣�SOCK_DGRAM)
		//parm3:ptotoc1 ʹ�þ����ĳ������Э��

		SOCKADDR_IN addr;                                  //��Ҫ�󶨵Ĳ�������Ҫ�Ǳ��ص�socket��һЩ��Ϣ��
		addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);     //ip��ַ��htonl��host���� to:to  n:net l:unsigned long ��˴洢�����ֽ��ڸ�λ
		addr.sin_family = AF_INET;
		addr.sin_port = htons(6000);                       //�˿� htons���޷��Ŷ�����ת��Ϊ�����ֽ���

		bind(serSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));//�����
		listen(serSocket, 5);                               //��������
		SOCKADDR_IN clientsocket;
		int len = sizeof(SOCKADDR);



		while (true)
		{
			
			SOCKET* serConn =new SOCKET;//�ڿͻ��˽�������
			*serConn = accept(serSocket, (SOCKADDR*)&clientsocket, &len);
			if (*serConn == SOCKET_ERROR)
			{
				cout << "���ӽ���ʧ��" << endl;
			}
			else
			{
				Parm parm;//����������socketlist�����߳�
				parm.SerConn = serConn;
				parm.socketlist = &socketlist;
				cout << "�ɹ���������" << endl;
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
		


		//closesocket(serConn);//�ر�
		WSACleanup();//�ͷ���Դ
	return 0;
}
