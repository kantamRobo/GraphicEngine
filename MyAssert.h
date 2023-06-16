/*!
 *@brief	�A�T�[�g
 */

#pragma once

#include <stdarg.h>
#include <Windows.h>
#include <stdio.h>

 /*!
	 *@brief	�A�T�[�g
	 *@param[in]	flag	�U�̂Ƃ��A�T�[�g���������܂��B
	 */
static inline void MyAssert(bool flag, const char* format, const char* file, long line, ...)
{
	if (!flag) {
		va_list va;
		va_start(va, flag);
		vprintf(format, va);
		char fileLineInfo[256];
		sprintf_s(fileLineInfo, "%s, %d�s��", file, line);
		char assertMessage[256];
		vsprintf_s(assertMessage, format, va);
		strcat_s(assertMessage, fileLineInfo);
		MessageBoxA(nullptr, assertMessage, "�A�T�[�g", MB_OK);
		va_end(va);
		std::abort();
	}
}

#ifdef _DEBUG
#define 	MY_ASSERT( flg, format, ... )	MyAssert( flg, format, __FILE__, __LINE__, __VA_ARGS__)
#else
#define 	MY_ASSERT( flg, format, ... )
#endif