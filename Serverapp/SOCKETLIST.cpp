#include"SOCKETLIST.h"

int SOCKETLIST::push_back(SOCKET* socket, char* ID)
{
	SOCKETLIST* now = next;
	SOCKETLIST* pr = now;
	if (next == NULL)
	{
		next=new SOCKETLIST(socket, ID);
		return 1;
	}
	while (now!=NULL)
	{
		if (now->next == NULL)
		{
			now->next = new SOCKETLIST(socket, ID);
			return 1;
		}
	}
	return 1;
}
int  SOCKETLIST::deuser(int val)
{
	SOCKETLIST* now = next;
	if (val >= size())
		return 0;
	if (val == 0)
	{

		next = next->next;
		delete now;
		return 1;
	}
	if (size() <= 2&& val == 1)
	{	
			delete next->next;
			next->next = NULL;
			return 1;
	}
	if (val == 1)
	{
		now = next->next;
		next->next = next->next->next;
		delete next->next;
		//next->next = NULL;
		return 1;
	}
	if (val == (size() - 1))
	{
		while (now != NULL)
		{
			if (now->next->next == NULL)
			{
				delete now->next;
				now->next = NULL;
			}
			now = now->next;
			return 1;
		}
		return 0;
	}
	for (int i = 0;i<val; i++)
	{
		if (i == val - 1)
		{
			SOCKETLIST* de = now->next;
			now->next = de->next;
			delete de;
		}
		now = now->next;
		return 1;
	}


}

SOCKETLIST* SOCKETLIST::operator[](int val)
{
	SOCKETLIST* now = next;
	for (int i = 0; i < val; i++)
	{
		now = now->next;
	}
	return now;
}
int SOCKETLIST::size()
{
	SOCKETLIST* now = next;
	int count = 0;
	while (now != NULL)
	{
		now = now->next;
		count++;
	}
	return count;
}