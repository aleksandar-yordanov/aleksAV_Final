#pragma once
#include "includes.h"

namespace Ui {
    class loginWindow;
}

class loginTransfer;

class loginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit loginWindow(loginTransfer* manager, QWidget *parent = nullptr);
    ~loginWindow();

public:
    std::string getUsername();
    std::string getPassword();
    void setLoginMessage(std::string message);
    void readDetails();
    void saveStateWriteDetails(bool state);

private slots:
    void on_gui_exit_clicked();

    void on_gui_continue_clicked();

    void on_gui_username_textChanged(const QString &arg1);

    void on_gui_password_textEdited(const QString &arg1);

    void on_gui_password_returnPressed();

    void on_gui_username_returnPressed();

    void on_gui_resetPassword_clicked();

    void on_gui_rememberDetails_toggled(bool checked);

private:
    Ui::loginWindow* ui;
    loginTransfer* manager;
    std::string m_username;
    std::string m_password;
    bool m_saveState;
    std::string detailsFilePath = utils::getHomeDir().append("/AleksAvData/cache.txt");

};
