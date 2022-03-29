#include "errorbox.h"

ErrorBox::ErrorBox(std::string errMsg)
{
    this->critical(0,"Error",QString::fromStdString(errMsg));
}
