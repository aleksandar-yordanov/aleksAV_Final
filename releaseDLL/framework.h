#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <winternl.h>
#include <iostream>
#include <Psapi.h>
#include <math.h>
#include <cstdint>
#include <iterator>
#include <WinBase.h>
#include <conio.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <map>
#include <thread>
#include <chrono>

typedef struct commonMalicious
{
	std::vector<std::string> mList;

}commonMalicious;

#define BUFSIZE 512