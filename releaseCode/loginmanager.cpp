#include "loginmanager.h"
#include "includes.h"
#include "httprequest.h"
#include "stringrequest.h"
#include "debugwindow.h"

loginTransfer::loginTransfer()
{
    this->login = new loginWindow(this);
    this->reset = new resetWindow();
    this->login->show();
}
net::HTTPResponses loginTransfer::doLogin()
{
    if(m_username == "bypass" && m_password == "bypass") return net::mapIntToEnum(1); //DEBUG, REMOVE IN FINAL BUILD
    json loginVars; //crazy gaming json
    loginVars["Username"] = m_username;
    loginVars["Password"] = m_password;
    net::StringRequest sReq(net::method::METHOD_POST,"aleks/login.php","51.38.80.233",loginVars.dump());
    net::Session s1;
    s1.initialiseEndpoint("51.38.80.233", "http");
    s1.connectAndSend(sReq);
    int loginState = net::parseResponse(s1.returnResponse());
    return net::mapIntToEnum(loginState);
}

void loginTransfer::transfer(){
    this->login->hide();
    userDetails details{this->m_username,this->m_password, license::getLicenseStatic(this->m_username)};
    this->main = new MainWindow(details);
    this->main->show();
    this->main->update();
}

void loginTransfer::resetWindowTransfer()
{
    //execution flow from this function onwards will remove main and login window, user must restart program after
    //setting password
    this->login->hide();
    if(this->main) delete this->main;
    delete this->login;
    this->reset->show();
    this->reset->update();
    //DO NOT TRY ACCESSING MAIN AND LOGIN AFTER FUNCTION HAS BEEN RUN, WILL RESULT IN SEG FAULT
    //this is defined behaviour. After any reset, program must be rerun/
}

void loginTransfer::recieveEvent(int state)//retarded code, doesnt respect encapsulation heirarchy
{
    if(state == 1)
    {
        net::HTTPResponses response = this->doLogin();
        switch (response) //enum comparison for login
        {
        case net::HTTPResponses::loginSuccess:
            this->dodgyLoginTextSetter("Login Successful");
            logging.debug->logMessage("Login: Login Successful");
            this->transfer();
            break;
        case net::HTTPResponses::loginFailure:
            this->dodgyLoginTextSetter("Invalid Details");
            logging.debug->logMessage("Login: Invalid details");
            break;
        case net::HTTPResponses::databaseDown:
            logging.debug->logMessage("Login: Server down");
            this->dodgyLoginTextSetter("Down for maintainance");
            break;
        case net::HTTPResponses::unknownResponse:
            logging.debug->logMessage("Login: Connection issue, see above");
            this->dodgyLoginTextSetter("Check Connection");
            break;
        }
    }
    else if(state == 0)
    {
        this->~loginTransfer();
    }
    else if(state == 2) //State to transfer to forgot password window.
    {
        this->resetWindowTransfer();
    }
}

std::string loginTransfer::getPassword()
{
    return this->m_password;
}
std::string loginTransfer::getUsername()
{
    return this->m_username;
}
void loginTransfer::setPassword(std::string password)
{
    this->m_password=password;
}
void loginTransfer::setUsername(std::string username)
{
    this->m_username=username;
}
void loginTransfer::dodgyLoginTextSetter(std::string message)
{
    this->login->setLoginMessage(message);
}
