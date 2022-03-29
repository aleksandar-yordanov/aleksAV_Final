// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "framework.h"
#include "IATdump.h"
#include "PipeManagement.h"

namespace malfuncs
{
    commonMalicious mainFuncs{ {"InitiateShutdownA","InitiateShutdownW","VirtualProtect","VirtualProtectEx"} }; //Common examples of malicious imports
}

DWORD WINAPI mainThread(HMODULE hModule) {

    //Stuff Here
    //AllocConsole();
    //freopen("CONOUT$", "w", stdout);
    bool Malicious = false;
    DWORD procID = GetCurrentProcessId();
    std::string pipeName = "\\\\.\\pipe\\avPipe-" + std::to_string(procID);
    std::vector<std::string> imports = testGetImports();
    char szExeFileName[MAX_PATH];
    GetModuleFileName(NULL, szExeFileName, MAX_PATH);
    std::string xt(szExeFileName);
    for (auto x : imports)
    {
        //std::cout << x << std::endl;
        for (auto func : malfuncs::mainFuncs.mList)
        {
            if (x == func)
            {
                Malicious = true;
            }
        }
    }
    if (Malicious) xt += "?true";
    else xt += "?false";
    SendMessageToPipe(pipeName.c_str(),xt.c_str());

    //Clean up after
    //FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)(mainThread), hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

