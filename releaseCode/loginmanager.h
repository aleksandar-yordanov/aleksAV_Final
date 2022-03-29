#pragma once
#include "includes.h"
#include "mainwindow.h"
#include "loginwindow.h"
#include "resetwindow.h"
#include "httprequest.h"
#include "license.h"
//#include "debugwindow.h"
/*
namespace Ui {
    class MainWindow;
    class loginWindow;
}
*/
class loginTransfer
{
public:
    loginTransfer();
    //loginTransfer(MainWindow* mainref = nullptr, loginWindow* loginref = nullptr, resetWindow* resetref = nullptr, debugWindow* debugref = nullptr);
    //loginTransfer(QPointer<MainWindow> mainref, QPointer<loginWindow> loginref, QPointer<resetWindow> resetref, QPointer<debugWindow> debugref);
    //void setChildWindows(MainWindow* mainref, loginWindow* loginref, resetWindow* resetref, debugWindow* debugref);
    //void setChildWindows(QPointer<MainWindow> mainref, QPointer<loginWindow> loginref, QPointer<resetWindow> resetref, QPointer<debugWindow> debugref);
    //no defined destructor needed as Q_Objects have are deleted when the end of scope is reached.

private:
    //neccessary for program function
    QPointer<MainWindow> main;
    QPointer<loginWindow> login;
    //not neccessary for program function
    QPointer<resetWindow> reset;

public:
    void transfer();
    void resetWindowTransfer();
    net::HTTPResponses doLogin();
    std::string getPassword();
    std::string getUsername();
    void setUsername(std::string username);
    void setPassword(std::string password);
    void recieveEvent(int state);
    void dodgyLoginTextSetter(std::string message);

private:

    std::string m_username;
    std::string m_password;
};


