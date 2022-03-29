#include "includes.h"
#include "PEdump.h"

using namespace std::chrono_literals;
/*
void WINAPI PEDumper::shellcode(manualMappingData* pData)
{
	if(!pData)
	{
		return;
	}
	PBYTE pBase = reinterpret_cast<PBYTE>(pData);
	auto* pOpt = &reinterpret_cast<PIMAGE_NT_HEADERS>(pBase + reinterpret_cast<PIMAGE_DOS_HEADER>(pData)->e_lfanew)->OptionalHeader;
	auto pLoadLibraryA = pData->pLoadLibraryA;
	auto pGetProcAddress = pData->pGetProcAddress;
	auto pDllMain = reinterpret_cast<f_DLL_ENTRY_POINT>(pBase + pOpt->AddressOfEntryPoint);
	PBYTE imageDelta = pBase - pOpt->AddressOfEntryPoint;
	if(imageDelta)
	{
		if(pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size)
		{
			return;
		}
		auto* pRelocData = reinterpret_cast<PIMAGE_BASE_RELOCATION>(pBase + pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		while(pRelocData->VirtualAddress)
		{
			unsigned int iEntries = (pRelocData->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
			PWORD pRelativeInfo = reinterpret_cast<PWORD>(pRelocData + 1);
			for(unsigned int i=0; i != iEntries; ++i, ++pRelativeInfo)
			{
				if (true);
			}
		}
	}
}
*/

std::ostream& operator<<(std::ostream& os, const exeDetails& dt)
{
	os << dt.path << ' ' << dt.maliciousImports << ' ' << dt.highEntropy << ' ' << dt.sigMatch << std::endl;
	return os;
}

std::vector<std::string> PEDumper::returnResults()
{
	return this->resultList;
}

BOOL PEDumper::fileExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

HMODULE PEDumper::getHandleToModule(const char* ModuleName, DWORD procID)
{
	MODULEENTRY32 me32;

	HANDLE hModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	if (hModuleSnapshot == INVALID_HANDLE_VALUE)
	{
		return nullptr;
	}
	me32.dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(hModuleSnapshot, &me32)) {
		CloseHandle(hModuleSnapshot);
		return nullptr;
	}
	do {
		if (strcmp(ModuleName, me32.szModule) == 0)
		{
			CloseHandle(hModuleSnapshot);
			return (HMODULE)me32.modBaseAddr;
		}
	} while (Module32Next(hModuleSnapshot, &me32));
	CloseHandle(hModuleSnapshot);
	return nullptr;
}

DWORD PEDumper::getProcId(const char* ProcessName)
{
	HANDLE hProcSnap;
	PROCESSENTRY32 pr32;

	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hProcSnap == INVALID_HANDLE_VALUE)
	{
        logging.debug->logMessage("invalid handle");
		return 0;
	}
	pr32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcSnap, &pr32))
	{
		CloseHandle(hProcSnap);
        logging.debug->logMessage("proc32iter failed");
		return 1;
	}
	do {
		if (strcmp(ProcessName, pr32.szExeFile) == 0)
		{
			return pr32.th32ProcessID;
		}
	} while (Process32Next(hProcSnap, &pr32));
	CloseHandle(hProcSnap);
	return 2;
}
/*
bool PEDumper::manualMap(const char* exeName) //in case of blocked or x64 app
{
	DWORD procID = this->getProcId(exeName);
	HANDLE hProc = OpenProcess(NULL, FALSE, procID);

	char fullDllName[260];
	if (!procID)
	{
		std::cout << "Invalid Process ID";
		CloseHandle(hProc);
		return false;
	}
	DWORD len = GetFullPathName(this->dllPath, MAX_PATH, fullDllName, nullptr);
	if (!len)
	{
		CloseHandle(hProc);
		return false;
	}

	if(!this->fileExists(fullDllName))
	{
		std::cout << "No valid dll at this file location" << std::endl;
		CloseHandle(hProc);
		return false;
	}

	std::ifstream is(fullDllName, std::ios::binary | std::ios::ate);
	if(is.fail())
	{
		std::cout << "File failed to open" << static_cast<DWORD>(is.rdstate()) << std::endl;
		CloseHandle(hProc);
		return false;
	}
	int fileSize = is.tellg();
	if(fileSize < 4096)
	{
		std::cout << "Invalid fileSize" << std::endl;
		is.close();
		CloseHandle(hProc);
		return false;
	}
	auto pSourceData = std::unique_ptr<BYTE[]>(new BYTE[static_cast<UINT_PTR>(fileSize)]);
	PBYTE imageBase = &pSourceData[0];
	if(!pSourceData)
	{
		std::cout << "malloc failed" << std::endl;
		is.close();
		CloseHandle(hProc);
		return false;
	}
	is.seekg(0,std::ios::beg);
	is.read(reinterpret_cast<char*>(imageBase), fileSize);
	is.close();
	if(reinterpret_cast<PIMAGE_DOS_HEADER>(imageBase)->e_magic != 0x5A4D)
	{
		std::cout << "Invalid File" << std::endl;
		CloseHandle(hProc);
		return false;
	}
	PIMAGE_NT_HEADERS pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(imageBase + reinterpret_cast<PIMAGE_DOS_HEADER>(imageBase)->e_lfanew);
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pNtHeaders->OptionalHeader;
	PIMAGE_FILE_HEADER pFileHeader = &pNtHeaders->FileHeader;
	//for now we don't need to worry about writing as a 64 bit process
	if(pFileHeader->Machine != IMAGE_FILE_MACHINE_I386)
	{
		std::cout << "Invalid Machine" << std::endl;
		return false;
	}
	PBYTE pTargetBase = static_cast<PBYTE>(VirtualAllocEx(hProc, reinterpret_cast<void*>(pOptionalHeader->ImageBase), pOptionalHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
	if(!pTargetBase)
	{
		pTargetBase = static_cast<PBYTE>(VirtualAllocEx(hProc,nullptr, pOptionalHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
		if(!pTargetBase)
		{
			std::cout << "Cannot Allocate memory in the target process, Last error: "<< GetLastError() << std::endl;
			return false;
		}
	}
	manualMappingData data{ NULL };
	data.pLoadLibraryA = LoadLibraryA;
	data.pGetProcAddress = reinterpret_cast<f_GetProcAddress>(GetProcAddress);
	auto* pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);

	for (unsigned int i = 0; i != pFileHeader->NumberOfSections; ++i, ++pSectionHeader)
	{
		if (pSectionHeader->SizeOfRawData)
		{
			if(!WriteProcessMemory(hProc,pTargetBase+pSectionHeader->VirtualAddress,imageBase+pSectionHeader->PointerToRawData,pSectionHeader->SizeOfRawData,nullptr))
			{
				std::cout << "Section mapping failed, Last Error: " << GetLastError() << std::endl;
				VirtualFreeEx(hProc, pTargetBase,pOptionalHeader->SizeOfImage, MEM_RELEASE);
				return false;
			}
		}
	}
	CloseHandle(hProc);
}
*/
bool PEDumper::InjectDLL(const char* exeName)
{
	//32 bit dlls into 32 bit processes only
	char fullDllName[260];
	DWORD procID = this->getProcId(exeName);
	if (!procID)
	{
        logging.debug->logMessage("Invalid Process ID");
		return false;
	}
	DWORD len = GetFullPathName(this->dllPath, MAX_PATH, fullDllName, nullptr);
	if (!len)
	{
		return false; 
	}
	if (len > MAX_PATH)
	{
		return false;
	}
	if (!this->fileExists(fullDllName))
	{
        logging.debug->logMessage("No valid dll at this file location");
		return false;
	}
	//Dll Injection below
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
	BOOL x;
	IsWow64Process(hProc, &x);
	if(x == FALSE)
	{
        logging.debug->logMessage("64 bit process" + std::to_string(procID));
		return false;
	}
	LPVOID pMemRegion = LPVOID(VirtualAllocEx(hProc, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)); //Alloc mem region
	if (!pMemRegion)
	{
        logging.debug->logMessage("Invalid memory allocation, Error code:" + std::to_string(GetLastError()));
		CloseHandle(hProc);
		return false;
	} //Memset validation
	if (!WriteProcessMemory(hProc, pMemRegion, fullDllName, MAX_PATH, nullptr))
	{
        logging.debug->logMessage("WriteProcessMemory failed, run as administrator, Error code:" +std::to_string(GetLastError()));
		CloseHandle(hProc);
		return false;
	}//WPM validation
	HANDLE hThread = CreateRemoteThread(hProc, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), pMemRegion, NULL, nullptr);
	//Above creates a thread in the target process and loads the dll into the target with LoadLibraryA in the mem region
	if (!hThread)
	{
        logging.debug->logMessage("Failed to create remote thread in target process, Error code:"+ std::to_string(GetLastError()));
		CloseHandle(hProc);
		return false;
	}
	//std::cout << GetLastError() << std::endl;
	CloseHandle(hProc);
	VirtualFreeEx(hProc, LPVOID(pMemRegion), 0, MEM_RELEASE);
	return true;
}

bool PEDumper::InjectDLL(DWORD procID)
{
	char fullDllName[260];
	if (!procID)
	{
        logging.debug->logMessage("Invalid Process ID");
		return false;
	}
	DWORD len = GetFullPathName(this->dllPath, MAX_PATH, fullDllName, nullptr);
	if (!len)
	{
		return false;
	}
	if (len > MAX_PATH)
	{
		return false;
	}
	if (!this->fileExists(fullDllName))
	{
        logging.debug->logMessage("No valid dll at this file location");
		return false;
	}
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
	BOOL x;
	IsWow64Process(hProc, &x);
	if(x == FALSE)
	{
        logging.debug->logMessage("64 bit process");
		return false;
	}
	LPVOID pMemRegion = LPVOID(VirtualAllocEx(hProc, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)); //Alloc mem region
	if (!pMemRegion)
	{
        logging.debug->logMessage("Invalid memory allocation, Error code:" +std::to_string(GetLastError()));
		CloseHandle(hProc);
		return false;
	} //Memset validation
	if (!WriteProcessMemory(hProc, pMemRegion, fullDllName, MAX_PATH, nullptr))
	{
        logging.debug->logMessage("WriteProcessMemory failed, run as administrator, Error code:" +std::to_string(GetLastError()));
		CloseHandle(hProc);
		return false;
	}//WPM validation
	HANDLE hThread = CreateRemoteThread(hProc, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), pMemRegion, NULL, nullptr);
	//Above creates a thread in the target process and loads the dll into the target with LoadLibraryA in the mem region
	if (!hThread)
	{
        logging.debug->logMessage("Failed to create remote thread in target process, Error code:" +std::to_string(GetLastError()));
		CloseHandle(hProc);
		return false;
	}
	//std::cout << GetLastError() << std::endl;
	CloseHandle(hProc);
	VirtualFreeEx(hProc, LPVOID(pMemRegion), 0, MEM_RELEASE);
	return true;
}


DWORD WINAPI PEDumper::pipeInstance()
{
	const char* pipeName = PEDumper::currentPipe.c_str();
    logging.debug->logMessage("Pipe created:"+ PEDumper::currentPipe);
	//std::this_thread::sleep_for(1100ms);
	SECURITY_DESCRIPTOR sd;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, 0, FALSE);
	SetSecurityDescriptorControl(&sd, SE_DACL_PROTECTED, SE_DACL_PROTECTED);
	SECURITY_ATTRIBUTES sa = { sizeof(sa), &sd, FALSE };
	HANDLE pipe = CreateNamedPipe(
		pipeName,
		PIPE_ACCESS_INBOUND,
		PIPE_TYPE_BYTE,
		1,
		0,
		0,
		0,
		&sa
	);
	if (pipe == NULL || pipe == INVALID_HANDLE_VALUE)
	{
		//std::cout << pipeName << std::endl;
        logging.debug->logMessage(PEDumper::currentPipe + "Pipe creation failed" +std::to_string(GetLastError()));
		return 1;
	}
	//std::cout << pipeName << std::endl;
	BOOL result = ConnectNamedPipe(pipe, nullptr);
	if (!result)
	{
		if (GetLastError() != 535)
		{
            logging.debug->logMessage("Failed" +std::to_string(GetLastError()));
			return 1;
		}
	}
	char buffer[BUFSIZE];
	DWORD bytesRead = 0;
	result = ReadFile(
		pipe,
		buffer,
		(BUFSIZE - 1) * sizeof(char),
		&bytesRead,
		NULL
	);
	DisconnectNamedPipe(pipe);
	if (result)
	{
		buffer[bytesRead / sizeof(char)] = '\0';
		PEDumper::mainQueue.pushBack(std::string(buffer));
	}
	else
	{
        logging.debug->logMessage("Failed");
	}
	//std::cout << GetLastError();
	CloseHandle(pipe);
	PEDumper::numThreads -= 1;
	return 0;
}

PEDumper::PEDumper(std::string inputString)
{
    std::string dllPath = (utils::getHomeDir() + "\\AleksAvData\\DLL-DumpPEHeaders.dll");
    this->dllPath = dllPath.c_str();
	this->executableList.emplace_back(inputString);
	this->Events = new HANDLE[this->procIDList.size()];
    for (unsigned int i = 0; i < this->procIDList.size(); i++)
	{
        std::this_thread::sleep_for(1ms);
		std::string pipeName = "\\\\.\\pipe\\avPipe-" + std::to_string(getProcId(this->executableList[i].c_str()));
		PEDumper::currentPipe = pipeName;
		PEDumper::numThreads += 1;
		HANDLE threadHandle = CreateThread(
			NULL,
			NULL,
			(LPTHREAD_START_ROUTINE)this->pipeInstance,
			NULL,
			NULL,
			NULL
		);
		if (threadHandle == INVALID_HANDLE_VALUE)
		{
            logging.debug->logMessage("invalid thread");
		}
		this->Events[i] = threadHandle;
		bool Injected = InjectDLL(procIDList[i]);
		if (!Injected)
		{
            logging.debug->logMessage("App not running or 64 bit app");
		}

		if (PEDumper::numThreads >= maxThreads)
		{
			WaitForMultipleObjects(this->procIDList.size(), this->Events, true, 500);
		}
	}
	WaitForMultipleObjects(this->procIDList.size(), this->Events, true, 5000);
    for (unsigned int i = 0; i < this->procIDList.size(); i++)
	{
        if (this->Events[i] != INVALID_HANDLE_VALUE && Events[i] != NULL)
		{
			try {
				CloseHandle(Events[i]);
			}
			catch (std::system_error& e)
			{
                logging.debug->logMessage(e.what());
			}
		}
	}
	while (PEDumper::mainQueue.checkEmpty() == false)
	{
		std::string x = PEDumper::mainQueue.popFront();
		this->resultList.emplace_back(x);
	}
}

PEDumper::PEDumper(std::vector<std::string> execList)
{
    std::string dllPath = (utils::getHomeDir() + "\\AleksAvData\\DLL-DumpPEHeaders.dll");
    this->dllPath = dllPath.c_str();
	this->executableList = execList;
	this->Events = new HANDLE[this->procIDList.size()];
    for (unsigned int i = 0; i < this->procIDList.size(); i++)
	{
		std::this_thread::sleep_for(1ms);
		std::string pipeName = "\\\\.\\pipe\\avPipe-" + std::to_string(getProcId(this->executableList[i].c_str()));
		PEDumper::currentPipe = pipeName;
		PEDumper::numThreads += 1;
		HANDLE threadHandle = CreateThread(
			NULL,
			NULL,
			(LPTHREAD_START_ROUTINE)this->pipeInstance,
			NULL,
			NULL,
			NULL
		);
		if (threadHandle == INVALID_HANDLE_VALUE)
		{
            logging.debug->logMessage("invalid thread");
		}
		this->Events[i] = threadHandle;
		bool Injected = InjectDLL(procIDList[i]);
		if (!Injected)
		{
            logging.debug->logMessage("App not running or 64 bit app");
		}

		if (PEDumper::numThreads >= maxThreads)
		{
			WaitForMultipleObjects(this->procIDList.size(), this->Events, true, 500);
		}
	}
	WaitForMultipleObjects(this->procIDList.size(), this->Events, true, 5000);
    for (unsigned int i = 0; i < this->procIDList.size(); i++)
	{
        if (this->Events[i] != INVALID_HANDLE_VALUE && Events[i] != NULL)
		{
			try {
				CloseHandle(Events[i]);
			}
			catch (std::system_error& e)
			{
                logging.debug->logMessage(e.what());
			}
		}
	}
	while (PEDumper::mainQueue.checkEmpty() == false)
	{
		std::string x = PEDumper::mainQueue.popFront();
		this->resultList.emplace_back(x);
	}
}

PEDumper::PEDumper(DWORD procID)
{
    std::string dllPath = (utils::getHomeDir() + "\\AleksAvData\\DLL-DumpPEHeaders.dll");
    this->dllPath = dllPath.c_str();
	this->procIDList.emplace_back(procID);
	this->Events = new HANDLE[this->procIDList.size()];
    for (unsigned int i = 0; i < this->procIDList.size(); i++)
	{
		std::this_thread::sleep_for(1ms);
		std::string pipeName = "\\\\.\\pipe\\avPipe-" + std::to_string(this->procIDList[i]);
		PEDumper::currentPipe = pipeName;
		PEDumper::numThreads += 1;
		HANDLE threadHandle = CreateThread(
			NULL,
			NULL,
			(LPTHREAD_START_ROUTINE)this->pipeInstance,
			NULL,
			NULL,
			NULL
		);
		if (threadHandle == INVALID_HANDLE_VALUE)
		{
            logging.debug->logMessage("invalid thread");
		}
		this->Events[i] = threadHandle;
		bool Injected = InjectDLL(procIDList[i]);
		if (!Injected)
		{
            logging.debug->logMessage("App not running or 64 bit app");
		}

		if (PEDumper::numThreads >= maxThreads)
		{
			WaitForMultipleObjects(this->procIDList.size(), this->Events, true, 500);
		}
	}
	WaitForMultipleObjects(this->procIDList.size(), this->Events, true, 5000);
    for (unsigned int i = 0; i < this->procIDList.size(); i++)
	{
        if (this->Events[i] != INVALID_HANDLE_VALUE && Events[i] != NULL)
		{
			try {
				CloseHandle(Events[i]);
			}
			catch (std::system_error& e)
			{
                logging.debug->logMessage(e.what());
			}
		}
	}
	while (PEDumper::mainQueue.checkEmpty() == false)
	{
		std::string x = PEDumper::mainQueue.popFront();
		this->resultList.emplace_back(x);
	}
}

PEDumper::PEDumper(std::vector<DWORD> procIDs)
{
    std::string dllPath = (utils::getHomeDir() + "\\AleksAvData\\DLL-DumpPEHeaders.dll");
    this->dllPath = dllPath.c_str();
	this->procIDList = procIDs;
    ThreadSafeQueue<DWORD> pQueue;
    for(const auto& pid : this->procIDList)
    {
        pQueue.pushBack(pid);
    }
	this->Events = new HANDLE[this->procIDList.size()];
    for (unsigned int i = 0; i < this->procIDList.size(); i++)
	{
        std::this_thread::sleep_for(1ms);
        DWORD cur = pQueue.popBack();
        std::string pipeName = "\\\\.\\pipe\\avPipe-" + std::to_string(cur);
        PEDumper::currentPipe = pipeName;
		PEDumper::numThreads += 1;
		HANDLE threadHandle = CreateThread(
			NULL,
			NULL,
			(LPTHREAD_START_ROUTINE)this->pipeInstance,
			NULL,
			NULL,
			NULL
		);
		if(threadHandle == INVALID_HANDLE_VALUE)
		{
            logging.debug->logMessage("invalid thread");
		}
		this->Events[i] = threadHandle;
        bool Injected = InjectDLL(cur);
		if (!Injected)
		{
            logging.debug->logMessage("App not running or 64 bit app");
		}
		
		if (PEDumper::numThreads >= maxThreads)
		{
			WaitForMultipleObjects(this->procIDList.size(), this->Events, true, 500);
		}
	}
    WaitForMultipleObjects(this->procIDList.size(), this->Events, true, 500);
    for (unsigned int i = 0; i < this->procIDList.size(); i++)
	{
        if (this->Events[i] != INVALID_HANDLE_VALUE && Events[i] != NULL)
		{
			try {
				CloseHandle(Events[i]);
			}
			catch (std::system_error& e)
			{
                logging.debug->logMessage(e.what());
			}
		}
	}
	while (PEDumper::mainQueue.checkEmpty() == false)
	{
		std::string x = PEDumper::mainQueue.popFront();
		this->resultList.emplace_back(x);
	}
}

PEDumper::~PEDumper()
{
	delete[] this->Events;
}
