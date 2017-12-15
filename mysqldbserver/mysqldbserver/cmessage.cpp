#include "cmessage.h"
#include "server_socket.h"

namespace af
{
	void CMessageManger::OnRecvCharMessage(CNetSocket * pSocket, char * pBuff, int nLen)
	{
		if (pSocket == NULL || pBuff == NULL || nLen <= 0)
		{
			return;
		}


	}
}