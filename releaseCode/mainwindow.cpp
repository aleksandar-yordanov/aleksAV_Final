#include "includes.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "accountwindow.h"

MainWindow::MainWindow(userDetails details, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
   this->details = details;
   ui->setupUi(this);
   QPixmap item(QPixmap(QString::fromStdString(utils::getHomeDir()+"/AleksAvData/ad0.jpg")));
   if(item.isNull())
   {
   logging.debug->logMessage("Advertising: Image not found");
   }
   ui->gui_ads->setPixmap(item);
   this->threatList.reserve(10);

   this->w_account = new accountWindow(nullptr,this->details.username);\
   this->w_info = new versionInformation(nullptr,this->versionString,std::string(__DATE__));

   model = new QFileSystemModel(this);
   model->setRootPath(QDir::rootPath());
   model->setReadOnly(true);
   model->setFilter(QDir::Drives|QDir::NoDotAndDotDot|QDir::AllDirs);
   ui->treeView->setModel(this->model);
   ui->treeView->setColumnWidth(0,265);
   ui->treeView->setRootIndex(model->index(QDir::rootPath()));

   threatListModel = new QStandardItemModel(0,2,this);
   threatListModel->setHorizontalHeaderLabels({"Name","Type","Hash"});
   quarantineListModel = new QStandardItemModel(0,1,this);
   quarantineListModel->setHorizontalHeaderLabels({"Original File Location","Type"});

   ui->gui_ThreatTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->gui_ThreatTable->setModel(threatListModel);
   ui->gui_ThreatTable->setColumnWidth(0,350);
   ui->gui_ThreatTable->setColumnWidth(1,110);
   ui->gui_ThreatTable->setColumnWidth(2,190);

   ui->gui_QuarantineTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->gui_QuarantineTable->setModel(quarantineListModel);
   ui->gui_QuarantineTable->setColumnWidth(0,450);
   ui->gui_QuarantineTable->setColumnWidth(1,200);

   this->scanFreq = new ScannedFrequency();
   this->scanFreq->loadData();
   this->scanFreq->setData();
   this->threatSum = new ThreatLevelSum();
   this->threatSum->loadData();
   this->threatSum->setData();
   ui->gui_scanDangerousAverage->setChart(this->threatSum->Chart);
   ui->gui_scanAverage->setChart(this->scanFreq->Chart);

   this->fileManager = new class fileManager(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string MainWindow::getUsername()
{
    return this->details.username;
}

std::string MainWindow::getPassword()
{
    return this->details.password;
}

license::t_licenses MainWindow::getLicenseType()
{
    return this->details.licenseType;
}

void MainWindow::update()
{
    if(this->details.licenseType == license::t_licenses::license_none || this->details.licenseType == license::t_licenses::license_standard)
    {
        ui->gui_ads->show();
    }
    else
    {
        ui->gui_ads->hide();
    }
}

void MainWindow::setScanText(QString inStr)
{
    this->ui->gui_currentScan->setText("Progress: " + inStr);
}

void MainWindow::on_treeView_activated(const QModelIndex &index)
{

    this->ui->gui_searchDir->setText("Current Search Directory: " + this->model->filePath(index));
    this->m_searchPath = this->model->filePath(index).toStdString();
    logging.debug->logMessage("Search Dir set to:" + this->model->filePath(index).toStdString());
    return;
}

std::string MainWindow::getSearchDir()
{
    return this->m_searchPath + "\\";
}

void MainWindow::scanWrapper(MainWindow* mainWnd)
{
    std::scoped_lock lock(mainWnd->threadLock);
    scanning::scanType type;
    if(mainWnd->ui->gui_bDirScan->isChecked())
    {
        type = scanning::scanType::static_scan;
    }
    else
    {
        type = scanning::scanType::memory_scan;
    }
    mainWnd->scanState = true;
    mainWnd->threatList.clear();
    scanning::scanManager mainManager(mainWnd, type);
    mainWnd->threatList = mainManager.returnResults();
    mainWnd->threatListModel->setRowCount(mainWnd->threatList.size());
    mainWnd->threatCount = mainWnd->threatList.size();
    for(unsigned int row = 0; row < mainWnd->threatList.size();row++)
    {
        if(mainWnd->threatList[row].returnThreatType() == threats::threatType::Type_None)
        {
            continue;
        }
        for(int col = 0; col < mainWnd->threatListModel->columnCount(); col++)
        {
            QModelIndex index = mainWnd->threatListModel->index(row,col,QModelIndex());
            if(col == 0)
            {
                mainWnd->threatListModel->setData(index, QString::fromStdString(mainWnd->threatList[row].returnFileLocation()));
            }
            else if(col == 1)
            {
                mainWnd->threatListModel->setData(index, QString::fromStdString(threats::threat::mapThreatTypeToStr(mainWnd->threatList[row].returnThreatType())));
            }
            else if(col == 2)
            {
                mainWnd->threatListModel->setData(index,QString::fromStdString(mainWnd->threatList[row].returnFileHash()));
            }
        }
    }
    mainWnd->scanState = false;
}

void MainWindow::appendQuarantinedItem(std::vector<quarantineInfo> info)
{
    this->quarantineListModel->setRowCount(info.size());
    for(unsigned int row = 0; row < info.size(); row++)
    {
        for(unsigned int col = 0; col < this->quarantineListModel->columnCount(); col++)
        {
            QModelIndex index = this->quarantineListModel->index(row,col,QModelIndex());
            if(col == 0)
            {
                this->quarantineListModel->setData(index, QString::fromStdString(info[row].originalFilePath));
            }
            else if(col == 1)
            {
                this->quarantineListModel->setData(index, QString::fromStdString(threats::threat::mapThreatTypeToStr(info[row].type)));
            }
        }
    }
}

void MainWindow::on_gui_startScan_clicked()
{
    if(this->scanState)
    {
        return;
    }
    else
    {
        this->scanFuture = std::async(std::launch::async,this->scanWrapper,this);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->scanFreq->addData(this->threatCount);
    this->scanFreq->saveData();
    std::vector<threats::threatType> threatCountThreatSum;
    for(auto threat : this->threatList)
    {
        threatCountThreatSum.emplace_back(threat.returnThreatType());
    }
    this->threatSum->addDataToCategory(threatCountThreatSum);
    this->threatSum->saveData();
}

void MainWindow::on_actionEdit_Details_triggered()
{
    if(!this->w_account->isVisible())
    {
        this->w_account->show();
    }
    else
    {
        this->w_account->raise();
    }
}


void MainWindow::on_actionVersion_Information_triggered()
{
    if(!this->w_info->isVisible())
    {
        this->w_info->show();
    }
    else
    {
        this->w_info->raise();
    }
}

void MainWindow::on_actionSpawnConsole_triggered()
{
    if(!logging.debug->isVisible())
    {
        logging.debug->show();
    }
    else
    {
        logging.debug->raise();
    }
}



void MainWindow::on_gui_ThreatTable_doubleClicked(const QModelIndex &index)
{
    this->currentScanIndex = index;
}


void MainWindow::on_gui_deleteThreat_clicked()
{
    QStandardItem* item = this->threatListModel->itemFromIndex(this->currentScanIndex);
    if(item->data(Qt::DisplayRole).isNull())
    {
        return;
    }
    QMessageBox confirm; //reimplement confirmation boxes as a polymorphic class
    confirm.setText("Are you sure you want to delete this file?");
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = confirm.exec();
    if(ret == QMessageBox::No)
    {
        return;
    }
    std::string path = item->data(Qt::DisplayRole).value<QString>().toStdString();
    if(QFileInfo(QString::fromStdString(path)).exists())
    {
        bool state = fileManager->deleteFile(path,false);
        if(state) this->threatListModel->removeRows(this->currentScanIndex.row(),1);
    }
    else
    {
        return;
    }
}


void MainWindow::on_gui_quarantineThreat_clicked()
{
    if(this->threatListModel->itemFromIndex(this->currentScanIndex)->data(Qt::DisplayRole).isNull())
    {
        return;
    }
    QMessageBox confirm; //reimplement confirmation boxes as a polymorphic class
    confirm.setText("Are you sure you want to quarantine this file?");
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = confirm.exec();
    if(ret == QMessageBox::No)
    {
        return;
    }
    int curRow = this->currentScanIndex.row();
    threats::threat curThreat{"",threats::threatType::Type_None};
    QList<QStandardItem*> passThroughItemList;
    for(unsigned int col = 0; col < this->threatListModel->columnCount(); col++)
    {
        QModelIndex index = this->threatListModel->index(curRow,col,QModelIndex());
        if(col == 0)
        {
            curThreat.setFileLocation(this->threatListModel->itemFromIndex(index)->data(Qt::DisplayRole).value<QString>().toStdString());
            passThroughItemList.append(this->threatListModel->itemFromIndex(index));
        }
        else if(col == 1)
        {
            curThreat.setThreatType(threats::threat::mapStrToThreatType(this->threatListModel->itemFromIndex(index)->data(Qt::DisplayRole).value<QString>().toStdString()));
            passThroughItemList.append(this->threatListModel->itemFromIndex(index));
        }
        else if(col == 2)
        {
            curThreat.setFileHash(this->threatListModel->itemFromIndex(index)->data(Qt::DisplayRole).value<QString>().toStdString());
        }
    }
    bool state = fileManager->quarantineFile(curThreat.returnFileLocation(),curThreat.returnThreatType());
    if(state)
    {
        this->threatListModel->removeRows(curRow,1);
        QStandardItem* fileLoc = new QStandardItem(QString::fromStdString(curThreat.returnFileLocation()));
        QStandardItem* threatType = new QStandardItem(QString::fromStdString(threats::threat::mapThreatTypeToStr(curThreat.returnThreatType())));
        QList<QStandardItem*> rowToAppend = {fileLoc,threatType};
        this->quarantineListModel->appendRow(rowToAppend);
    }
    //delete fileLoc; - Memory here cant be free'd as the list model holds a pointer to it,
    //delete threatType; - This will cause a memory leak, figure out how to move that data later
}


void MainWindow::on_gui_restoreThreat_clicked()
{
    QStandardItem* item = this->quarantineListModel->itemFromIndex(this->currentQuarantineIndex);
    if(item->data(Qt::DisplayRole).isNull())
    {
        return;
    }
    QMessageBox confirm; //reimplement confirmation boxes as a polymorphic class
    confirm.setText("Are you sure you want to restore this file?");
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = confirm.exec();
    if(ret == QMessageBox::No)
    {
        return;
    }
    std::string path = item->data(Qt::DisplayRole).value<QString>().toStdString();
    if(path != "")
    {
        bool state = fileManager->restoreFile(path);
        if(state)this->quarantineListModel->removeRows(this->currentQuarantineIndex.row(),1);
    }
    else
    {
        return;
    }
}


void MainWindow::on_gui_quarantineDeleteThreat_clicked()
{
    QStandardItem* item = this->quarantineListModel->itemFromIndex(this->currentQuarantineIndex);
    if(item->data(Qt::DisplayRole).isNull())
    {
        return;
    }
    QMessageBox confirm; //reimplement confirmation boxes as a polymorphic class
    confirm.setText("Are you sure you want to delete this file?");
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int ret = confirm.exec();
    if(ret == QMessageBox::No)
    {
        return;
    }
    std::string path = item->data(Qt::DisplayRole).value<QString>().toStdString();
    if(path != "")
    {
        bool state = fileManager->deleteFile(path,true);
        if(state) this->quarantineListModel->removeRows(this->currentQuarantineIndex.row(),1);
    }
    else
    {
        return;
    }
}


void MainWindow::on_gui_QuarantineTable_doubleClicked(const QModelIndex &index)
{
    this->currentQuarantineIndex = index;
}
