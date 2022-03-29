#pragma once
#include "threadsafequeue.h"
#include "includes.h"
constexpr auto BUFSIZE = 512;

#define RELOC_FLAG32(RelInfo) ((RelInfo >> 0xC) == IMAGE_REL_BASED_HIGHLOW)

using f_LoadLibraryA = HINSTANCE(WINAPI*)(const char* lpLibFilename);
using f_GetProcAddress = UINT_PTR(WINAPI*)(HINSTANCE hModule, const char* lpProcName);
using f_DLL_ENTRY_POINT = BOOL(WINAPI*)(void* hDll, DWORD dwReason, void* pReserved);

struct exeDetails
{
	std::string path;
	bool maliciousImports;
	bool highEntropy;
	bool sigMatch;
    std::string fileHash;

	friend std::ostream& operator<<(std::ostream& os, const exeDetails& dt);
};

class PEDumper
{
public:
	PEDumper(std::string inputString);
	PEDumper(std::vector<std::string> execList);
	PEDumper(DWORD procID);
	PEDumper(std::vector<DWORD> procIDs);
	virtual ~PEDumper();
	std::vector<std::string> returnResults();
	static BOOL fileExists(LPCTSTR szPath);
	static HMODULE getHandleToModule(const char* ModuleName, DWORD procID);
	static DWORD getProcId(const char* ProcessName);
private:
	/*
	typedef struct MANUAL_MAPPING_DATA
	{
		f_LoadLibraryA pLoadLibraryA;
		f_GetProcAddress pGetProcAddress;
		HINSTANCE hMod;
	}manualMappingData;
	*/
	//void WINAPI shellcode(manualMappingData* pData);

	bool InjectDLL(const char* exeName);
	bool InjectDLL(DWORD procID);
	//bool manualMap(const char* exeName);
    static DWORD WINAPI pipeInstance();

	HANDLE* Events;
	inline static ThreadSafeQueue<std::string> mainQueue;
	std::vector<std::string> executableList;
	std::vector<DWORD> procIDList;
	std::vector<std::string> resultList;
    inline static std::atomic<int> numThreads = 0;
	inline static std::string currentPipe = "";
    const char* dllPath;
};
