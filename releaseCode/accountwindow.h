#pragma once
#include "includes.h"
#include "stringrequest.h"
#include "httprequest.h"
#include "license.h"

namespace Ui {
class accountWindow;
}

class accountWindow : public QWidget
{
    Q_OBJECT

public:
    explicit accountWindow(QWidget *parent = nullptr, std::string username = "");
    ~accountWindow();

private slots:

    void on_gui_quit_clicked();

    void on_gui_upgradeLicense_clicked();

private:
    license::t_licenses getLicense();
    void update();
    Ui::accountWindow *ui;
    std::string username;
    license::t_licenses license;
};
