#include "pch.h"
#include "framework.h"

typedef std::chrono::steady_clock Clock;

void SendMessageToPipe(const char* pipeName, const char* message)
{
    auto t1 = Clock::now();
    double timedif = 0;
	HANDLE pipe = INVALID_HANDLE_VALUE;
    while (pipe == INVALID_HANDLE_VALUE)
    {
        pipe = CreateFile(
            pipeName,
            GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        auto t2 = Clock::now();
        auto time_span = t2 - t1;
    	timedif = double(time_span.count()) * Clock::period::num / Clock::period::den;
        if(timedif > 10.0)
        {
            //std::cout << "here";
            break;
        }
    }

    if (pipe == INVALID_HANDLE_VALUE)
    {
        //std::cout << GetLastError();
        //std::cout << "Pipe not open" << std::endl;
        return;
    }
    std::string len = message;
    DWORD bytesWritten = 0;
    BOOL result = WriteFile(
        pipe,
        message,
        len.length(),
        &bytesWritten,
        NULL
    );
    if (result)
    {
        //std::cout << "Message sent" << std::endl;
    }
    else
    {
        //std::cout << "Troll gamer gaming" << std::endl;
    }
    CloseHandle(pipe);
}