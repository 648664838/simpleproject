#include "cclienttool.h"

int UnicodeToUTF8(CString & strSrc, char * pBuff, int nBuffLen)
{
	if (pBuff == NULL || nBuffLen <= 0)
	{
		return -1;
	}
	int nLen = WideCharToMultiByte(CP_ACP, 0, strSrc, strSrc.GetLength(), NULL, 0, NULL, NULL);

	if (nLen < 0 || nLen + 1 > nBuffLen)
	{
		return -1;
	}

	WideCharToMultiByte(CP_ACP, 0, strSrc, strSrc.GetLength(), pBuff, nLen, NULL, NULL);

	pBuff[nLen + 1] = '\0';

	return 0;
}
int UTF8ToUnicode(CString & strSrc, char * pBuff, int nBuffLen)
{
	if (pBuff == NULL || nBuffLen <= 0)
	{
		return -1;
	}

	int nLen = MultiByteToWideChar(CP_UTF8, 0, pBuff, nBuffLen, NULL, 0);

	TCHAR *buf = new TCHAR[nLen + 1];

	buf[nLen] = '\0';

	MultiByteToWideChar(CP_UTF8, 0, pBuff, nBuffLen, buf, nLen);

	strSrc = buf;

	delete buf;

	return 0;
}