#include "debugwindow.h"
#include "ui_debugwindow.h"

debugWindow::debugWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::debugWindow)
{
    ui->setupUi(this);
}

void debugWindow::logMessage(const std::string &stringRef)
{
    std::lock_guard<std::mutex> l(this->scopedLock);
    auto oldMax = ui->plainTextEdit->verticalScrollBar()->maximum();
    std::string toAppend = "> " + stringRef;
    ui->plainTextEdit->appendPlainText(QString::fromStdString(toAppend));
    ui->plainTextEdit->verticalScrollBar()->setValue(oldMax);
}

debugWindow::~debugWindow()
{
    delete ui;
}

