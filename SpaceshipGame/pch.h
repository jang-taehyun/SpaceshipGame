// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#pragma comment(lib, "winmm.lib")

#include "targetver.h"

// 외부 MFC 오버헤더 제외 //
#define WIN32_LEAN_AND_MEAN

// Windows header //
#include <windows.h>

// C 런타임 헤더 //
#include <stdlib.h>				// C 언어 표준 라이브러리
#include <malloc.h>				// dynamic allocate
#include <memory.h>				// 메모리 관련

// C++ header //
#include <string>

// direct sound 관련 //
#include <stdio.h>
#include <mmsystem.h>

#define UNICODE
typedef std::basic_string<TCHAR> tstring;
#include <tchar.h>				// windows header에 있는 자료형 확장

// DirectX 11 관련 //
#include "DxDefine.h"

// typedef, 데이터 관련 //
#include "config.h"

#endif //PCH_H
