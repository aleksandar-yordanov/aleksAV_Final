#pragma once
//Platform specific includes

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "User32.lib")

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#define _WIN32_WINNT 0x0A00
#ifdef __APPLE__
#include <unistd.h>
#include <sys/types.h>
#include <sys/kernel.h>
#include <pwd.h>
#elif _WIN32
#include <windows.h>
#include <shlobj.h>
#include <psapi.h>
#include <winsvc.h>
#include <winternl.h>
#include <WinBase.h>
#include <strsafe.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <libloaderapi.h>
#include <AclAPI.h>
#endif

//Standard Library includes
#include <fstream>
#include <tchar.h>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <regex>
#include <mutex>
#include <math.h>
#include <algorithm>
#include <thread>
#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <string>
#include <cstring>
#include <locale>
#include <optional>
#include <set>
#include <atomic>
#include <future>

//QT related includes
#include <QWidget>
#include <QGraphicsScene>
#include <QPixmap>
#include <QtCharts>
#include <QBitmap>
#include <QPointer>
#include <QMainWindow>
#include <QApplication>
#include <QString>
#include <QDialog>
#include <QStandardItemModel>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFileSystemModel>
#include <QTableView>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>

//Other includes
#define ASIO_STANDALONE
#include "common.h"
#include "debugwindow.h"
#include "json.hpp"
#include "errorbox.h"

//Non-include or import related expressions
using json = nlohmann::json;
using namespace std::chrono_literals;

const int maxThreads = std::thread::hardware_concurrency() /2;
