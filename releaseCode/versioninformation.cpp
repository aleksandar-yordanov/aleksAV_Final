#include "versioninformation.h"
#include "ui_versioninformation.h"

versionInformation::versionInformation(QWidget *parent, std::string version, std::string buildDate) :
    QWidget(parent),
    ui(new Ui::versionInformation)
{
    ui->setupUi(this);
    ui->gui_buildDate->setText("Last compiled on: " + QString::fromStdString(buildDate));
    ui->gui_currentVersion->setText("Current Version: " + QString::fromStdString(version));
}

versionInformation::~versionInformation()
{
    delete ui;
}

void versionInformation::on_pushButton_clicked()
{
    this->hide();
}

