#pragma once
#include "stdafx.h"

int UnicodeToUTF8(CString & strSrc, char * pBuff, int nBuffLen);
int UTF8ToUnicode(CString & strSrc, char * pBuff, int nBuffLen);