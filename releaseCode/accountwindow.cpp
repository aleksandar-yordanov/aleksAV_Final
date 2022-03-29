#include "accountwindow.h"
#include "ui_accountwindow.h"
#include "includes.h"

accountWindow::accountWindow(QWidget *parent, std::string username) :
    QWidget(parent),
    ui(new Ui::accountWindow)
{
    ui->setupUi(this);
    this->username = username;
    ui->gui_username->setText(QString::fromStdString("Username: " + username));
    this->license = getLicense();
    ui->gui_licenseState->setText(QString::fromStdString("License Type: " + license::mapLicenseToStringFormatted(this->license)));
}

accountWindow::~accountWindow()
{
    delete ui;
}

license::t_licenses accountWindow::getLicense()
{
    json licenseJs;
    licenseJs["Username"] = this->username;
    //std::cout << testJs.dump();
    net::StringRequest sReq(net::method::METHOD_POST, "aleks/license.php","51.38.80.233", licenseJs.dump());
    net::Session s1;
    s1.initialiseEndpoint(sReq, "http");
    s1.connectAndSend(sReq);
    int licenseState = net::parseResponse(s1.returnResponse());
    return license::mapLicenseResponseToLicense(net::licenseMapIntToEnum(licenseState));
}

void accountWindow::on_gui_quit_clicked()
{
    this->hide();
}

void accountWindow::update()
{
    ui->gui_licenseState->setText(QString::fromStdString("License Type: " + license::mapLicenseToStringFormatted(this->license)));
}

void accountWindow::on_gui_upgradeLicense_clicked()
{
    std::string licenseText = ui->gui_licenseSelector->currentText().toStdString();
    json licenseUpgradeJs;
    licenseUpgradeJs["Username"] = this->username;
    if(licenseText == "No License")
    {
        licenseUpgradeJs["LicenseType"] = "license_none";
        this->license = license::t_licenses::license_none;
    }
    else if(licenseText == "Standard License")
    {
        licenseUpgradeJs["LicenseType"] = "license_standard";
        this->license = license::t_licenses::license_standard;

    }
    else if(licenseText == "Premium License")
    {
        licenseUpgradeJs["LicenseType"] = "license_premium";
        this->license = license::t_licenses::license_premium;

    }
    else
    {
        return;
    }
    net::StringRequest sReq(net::method::METHOD_POST,"aleks/upgradeLicense.php","51.38.80.233",licenseUpgradeJs.dump());
    net::Session s1;
    s1.initialiseEndpoint(sReq,"http");
    s1.connectAndSend(sReq);
    this->update();
}

