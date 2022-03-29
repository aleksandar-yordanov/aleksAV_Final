#pragma once
#include "includes.h"

class ErrorBox : public QMessageBox
{
    Q_OBJECT

public:
    explicit ErrorBox(std::string errMsg);
};


