#include"socket_info.h"

socket_info::socket_info(SOCKET* socket, char UserID[10])
{
	ClientSocket =socket;
	memcpy(User_ID, UserID, sizeof(User_ID));

}