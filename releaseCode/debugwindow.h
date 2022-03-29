#pragma once
#include "includes.h"

namespace Ui {
class debugWindow;
}

class debugWindow : public QWidget
{
    Q_OBJECT

public:
    explicit debugWindow(QWidget *parent = nullptr);
    ~debugWindow();
    static inline std::mutex scopedLock;

public slots:
    void logMessage(const std::string& stringRef);

private:
    Ui::debugWindow *ui;
};

