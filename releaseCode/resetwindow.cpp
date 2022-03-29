#include "resetwindow.h"
#include "ui_resetwindow.h"
#include "stringrequest.h"
#include "httprequest.h"

using namespace std::chrono_literals;

resetWindow::resetWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::resetWindow)
{
    ui->setupUi(this);
}

resetWindow::~resetWindow()
{
    delete ui;
}

void resetWindow::sendCreateMessage()
{
    json createVars;
    createVars["Username"] = this->m_createUsername;
    createVars["Email"] = this->m_createUserEmail;
    if(this->m_createPassword != this->m_createConfirmPassword)
    {
        this->ui->gui_CreateWarningLabel->setText(QString::fromStdString("Passwords don't match"));
        return;
    }
    createVars["Password"] = this->m_createPassword;
    net::StringRequest sReq(net::method::METHOD_POST,"aleks/register.php","51.38.80.233",createVars.dump());
    net::Session s1;
    s1.initialiseEndpoint(sReq,"http");
    s1.connectAndSend(sReq);
    int registerState = net::parseResponse(s1.returnResponse());
    switch(net::createMapIntToEnum(registerState))
    {
    case net::HTTPRegisterResponses::registerSuccess:
        this->ui->gui_CreateWarningLabel->setText("Registration successful");
        break;
    case net::HTTPRegisterResponses::registerFailure:
        this->ui->gui_CreateWarningLabel->setText("Registration failed");
        break;
    case net::HTTPRegisterResponses::invalidPasswordLength:
        this->ui->gui_CreateWarningLabel->setText("Password length must be > 6 or < 64");
        break;
    case net::HTTPRegisterResponses::invalidEmail:
        this->ui->gui_CreateWarningLabel->setText("Email formatting is incorrect");
        break;
    case net::HTTPRegisterResponses::emailRegistered:
        this->ui->gui_CreateWarningLabel->setText("This email has already been registered");
        break;
    }
}

void resetWindow::sendResetMessage()
{
    json resetVars;
    if(this->m_resetNewPassword != this->m_resetConfirmPassword)
    {
        this->ui->gui_resetWarningLabel->setText(QString::fromStdString("Passwords don't match"));
        return;
    }
    resetVars["Username"] = this->m_resetUsername;
    resetVars["Password"] = this->m_resetPassword;
    resetVars["Updated"] = this->m_resetNewPassword;
    net::StringRequest sReq(net::method::METHOD_POST,"aleks/reset.php","51.38.80.233",resetVars.dump());
    net::Session s1;
    s1.initialiseEndpoint(sReq,"http");
    s1.connectAndSend(sReq);
    int resetState = net::parseResponse(s1.returnResponse());
    switch(net::resetMapIntToEnum(resetState))
    {
    case net::HTTPResetResponses::resetSuccess:
        this->ui->gui_resetWarningLabel->setText(QString::fromStdString("Reset Successful"));
        break;
    case net::HTTPResetResponses::invalidPasswordLength:
        this->ui->gui_resetWarningLabel->setText(QString::fromStdString("Password length must be > 6 or < 64"));
        break;
    case net::HTTPResetResponses::databaseDown:
        this->ui->gui_resetWarningLabel->setText(QString::fromStdString("Reset Failed"));
        break;
    case net::HTTPResetResponses::usernameNotFound:
        this->ui->gui_resetWarningLabel->setText(QString::fromStdString("Username not found"));
        break;
    case net::HTTPResetResponses::wrongPassword:
        this->ui->gui_resetWarningLabel->setText(QString::fromStdString("Account password is incorrect"));
        break;
    }
}

//mess of setters
void resetWindow::on_gui_resetUsernameInput_textChanged(const QString &arg1)
{
    this->m_resetUsername = this->ui->gui_resetUsernameInput->text().toStdString();
}

void resetWindow::on_gui_resetEmailInput_textChanged(const QString &arg1)
{
    this->m_resetNewPassword = this->ui->gui_resetEmailInput->text().toStdString();
}

void resetWindow::on_gui_resetPasswordInput_textChanged(const QString &arg1)
{
    this->m_resetPassword = this->ui->gui_resetPasswordInput->text().toStdString();
}

void resetWindow::on_gui_resetConfirmInput_textChanged(const QString &arg1)
{
    this->m_resetConfirmPassword = this->ui->gui_resetConfirmInput->text().toStdString();
}

void resetWindow::on_gui_createUsernameInput_textChanged(const QString &arg1)
{
    this->m_createUsername = this->ui->gui_createUsernameInput->text().toStdString();
}

void resetWindow::on_gui_createEmailInput_textChanged(const QString &arg1)
{
    this->m_createUserEmail = this->ui->gui_createEmailInput->text().toStdString();
}

void resetWindow::on_gui_createPasswordInput_textChanged(const QString &arg1)
{
    this->m_createPassword = this->ui->gui_createPasswordInput->text().toStdString();
}

void resetWindow::on_gui_createConfirmInput_textChanged(const QString &arg1)
{
    this->m_createConfirmPassword = this->ui->gui_createConfirmInput->text().toStdString();
}

void resetWindow::on_gui_sendDetailsCreate_clicked()
{
    this->sendCreateMessage();
}

void resetWindow::on_gui_sendDetailsReset_clicked()
{
    this->sendResetMessage();
}

void resetWindow::on_gui_resetResetPassword_clicked()
{
#ifdef _WIN32
    system("start chrome.exe http://51.38.80.233/aleks/sendEmailReset.html");
#elif _WIN64
    system("start chrome.exe http://51.38.80.233/aleks/sendEmailReset.html");
#else
    system("open http://51.38.80.233/aleks/sendEmailReset.html");
#endif
}
