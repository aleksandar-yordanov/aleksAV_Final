#pragma once
#include "includes.h"

namespace Ui {
class resetWindow;
}

class resetWindow : public QWidget
{
    Q_OBJECT

public:
    explicit resetWindow(QWidget *parent = nullptr);
    ~resetWindow();
    void sendCreateMessage();
    void sendResetMessage();

private slots:
    void on_gui_resetUsernameInput_textChanged(const QString &arg1);

    void on_gui_resetEmailInput_textChanged(const QString &arg1);

    void on_gui_resetPasswordInput_textChanged(const QString &arg1);

    void on_gui_resetConfirmInput_textChanged(const QString &arg1);

    void on_gui_createUsernameInput_textChanged(const QString &arg1);

    void on_gui_createEmailInput_textChanged(const QString &arg1);

    void on_gui_createPasswordInput_textChanged(const QString &arg1);

    void on_gui_createConfirmInput_textChanged(const QString &arg1);

    void on_gui_sendDetailsCreate_clicked();

    void on_gui_sendDetailsReset_clicked();

    void on_gui_resetResetPassword_clicked();

private:
    Ui::resetWindow *ui;
    //resetTab
    std::string m_resetUsername;
    std::string m_resetNewPassword;
    std::string m_resetPassword;
    std::string m_resetConfirmPassword;
    //createTab
    std::string m_createUsername;
    std::string m_createUserEmail;
    std::string m_createPassword;
    std::string m_createConfirmPassword;
};
