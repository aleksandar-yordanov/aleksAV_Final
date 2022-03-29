#pragma once
#include "includes.h"
#include "loginwindow.h"
#include "threattype.h"
#include "license.h"
#include "statsmanager.h"
#include "scanmanager.h"
#include "filemanager.h"
#include "versioninformation.h"

class accountWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; class loginWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(userDetails details, QWidget *parent = nullptr);
    ~MainWindow();

public:
    std::string getUsername();
    std::string getPassword();
    std::string getSearchDir();
    license::t_licenses getLicenseType();
    void appendQuarantinedItem(std::vector<quarantineInfo> info);
    void update();

public slots:
    void setScanText(QString inStr);

private slots:
    void on_treeView_activated(const QModelIndex &index);

    void on_gui_startScan_clicked();

    void on_actionEdit_Details_triggered();

    void closeEvent(QCloseEvent *event);

    void on_actionSpawnConsole_triggered();

    void on_gui_ThreatTable_doubleClicked(const QModelIndex &index);

    void on_gui_deleteThreat_clicked();

    void on_gui_quarantineThreat_clicked();

    void on_gui_restoreThreat_clicked();

    void on_gui_quarantineDeleteThreat_clicked();

    void on_gui_QuarantineTable_doubleClicked(const QModelIndex &index);

    void on_actionVersion_Information_triggered();

private:
    Ui::MainWindow *ui;
    userDetails details; //Details need to be passed into the class at construction
    QFileSystemModel* model;
    QPointer<accountWindow> w_account;
    QPointer<versionInformation> w_info;
    static void scanWrapper(MainWindow* mainWnd);
    std::string m_searchPath = "";
    std::vector<threats::threat> threatList;
    QStandardItemModel* threatListModel;
    QStandardItemModel* quarantineListModel;
    int threatCount = 0;
    ScannedFrequency* scanFreq;
    ThreatLevelSum* threatSum;
    fileManager* fileManager;
    std::mutex threadLock;
    std::future<void> scanFuture;
    std::atomic<bool> scanState;
    QModelIndex currentScanIndex;
    QModelIndex currentQuarantineIndex;
    std::string versionString = "Beta 0.57"; //based of number of commits
};

