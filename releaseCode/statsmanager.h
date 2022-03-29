#pragma once
#include "includes.h"
#include "threattype.h"
#include "robin_hood.h"

class CustomBarSet : public QBarSet //Extending base QBarSet Class with custom methods.
{
public:
    CustomBarSet(QString in) : QBarSet(in){}; //in would be the name of the barset
    qreal getMaxValue();
    qreal getMinValue();
};

class StatsManager
{
public: //Stats interface, C++ doesn't have interfaces as a keyword, therefore a pure virtual class must be used.
    virtual void loadData() = 0;
    virtual void setData() = 0;
    virtual void saveData() = 0;
    virtual void addData(int num) = 0;
protected:
    std::string statsRetrieveLocation = utils::getHomeDir().append("/AleksAvData/StatsDump.txt");
    std::string typeRetrieveLocation = utils::getHomeDir().append("/AleksAvData/CountDump.txt");
};

class ScannedFrequency : StatsManager
{
public:
    explicit ScannedFrequency();
    void loadData();
    void setData();
    void saveData();
    void addData(int num);
    static std::string serialiseData(CustomBarSet& set);
    static QPointer<CustomBarSet> deserialiseData(std::string str,std::string setName);
    QPointer<QChart> Chart;
private:

    QPointer<QValueAxis> yAxis;
    QPointer<CustomBarSet> fileSet;
    QPointer<QBarSeries> mainSeries;
    //Threats detected per scan
};

class ThreatLevelSum : StatsManager
{
public:
    explicit ThreatLevelSum();
    void loadData();
    void setData();
    void saveData();
    void addData(int num);
    void addDataToCategory(std::vector<threats::threatType>& type);
    void deserialiseData(std::string parseStr);
    std::string serialiseData();
    QPointer<QChart> Chart;
private:
    QPointer<QValueAxis> yAxis;
    robin_hood::unordered_map<threats::threatType,int> threatMap;
    std::vector<QPointer<CustomBarSet>> setVector;
    QPointer<QBarSeries> mainSeries;
    //Summation of the number of different threat classes over install lifetime
};
