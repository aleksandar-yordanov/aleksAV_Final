#include "includes.h"

namespace utils
{

    template<class T> void safeDelete(T* ptr)
    {
        if(*ptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }
    template<class T> T safeMalloc(T* ptr)
    {
        if(ptr == nullptr)
        {
            ptr = new T;
            return ptr;
        }
    }

    bool checkStrToInt(std::string str)
    {
        try {
            std::stoi(str);
        }  catch (const std::exception &e) {
            logging.debug->logMessage(e.what());
            return false;
        }
        return true;
    }
    std::string boolToStr(bool state)
    {
        if(state)
        {
            return "true";
        }
        else if(!state)
        {
            return "false";
        }
        else {
            return "err";
        }
    }

    bool strToBool(std::string state)
    {
        if(state == "true") return true;
        else if(state == "false") return false;
        else return false;
    }
#ifdef __APPLE__
    std::string getHomeDir(){
        const char* homedir;
        if ((homedir = getenv("HOME")) == NULL) {
            homedir = getpwuid(getuid())->pw_dir;
        }
        return std::string(homedir);
    }
#elif _WIN32
    std::string getHomeDir()
    {
        char path[MAX_PATH];
        if (SHGetFolderPathA( NULL, CSIDL_PROFILE, NULL, 0, path ) != S_OK)
        {
            //std::cout << path << std::endl;
            return static_cast<std::string>(path);
        }
        else
        {
#ifdef _APPLE
            std::cout << "/" + static_cast<std::string>(path)+ "/" << std::endl;
            return "/" + static_cast<std::string>(path)+ "/";
#elif _WIN32
            //std::cout << path << std::endl;
            return static_cast<std::string>(path);
#endif
        }
    }
#endif

    bool is_empty(std::ifstream& pFile)
    {
        return pFile.peek() == std::ifstream::traits_type::eof();
    }
    std::string intToStr(int x)
    {
        std::ostringstream stringStream;//inefficient, uses really roundabout method to convert
        stringStream << x;
        return stringStream.str();
    }
}
