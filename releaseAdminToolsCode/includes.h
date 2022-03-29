#pragma once
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QCryptographicHash>
#include <iostream>
#include <thread>
#include <string>
#define _WIN32_WINNT 0x0A00
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include "json.hpp"

using namespace std::chrono_literals;

using json = nlohmann::json;
