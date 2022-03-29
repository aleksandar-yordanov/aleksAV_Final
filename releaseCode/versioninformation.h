#pragma once
#include "includes.h"
namespace Ui {
class versionInformation;
}

class versionInformation : public QWidget
{
    Q_OBJECT

public:
    explicit versionInformation(QWidget *parent = nullptr, std::string version ="", std::string buildDate = "");
    ~versionInformation();

private slots:
    void on_pushButton_clicked();

private:
    Ui::versionInformation *ui;
};
