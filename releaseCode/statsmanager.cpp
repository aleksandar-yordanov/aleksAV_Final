#include "statsmanager.h"

qreal CustomBarSet::getMaxValue()
{
    qreal curMax = 0;
    for (int i = 0; i < this->count();i++)
    {
        qreal tempVal = this->at(i);
        if(tempVal > curMax)
        {
            curMax = tempVal;
        }
    }
    return curMax;
}

qreal CustomBarSet::getMinValue()
{
    qreal curMin = 0xFFFF;
    for (int i = 0; i < this->count();i++)
    {
        qreal tempVal = this->at(i);
        if(tempVal < curMin)
        {
            curMin = tempVal;
        }
    }
    return curMin;
}

ScannedFrequency::ScannedFrequency()
{
    this->Chart = new QChart;
    this->mainSeries = new QBarSeries;
    this->yAxis = new QValueAxis;
}

ThreatLevelSum::ThreatLevelSum()
{
    this->Chart = new QChart;
    this->mainSeries = new QBarSeries;
    this->yAxis = new QValueAxis;
    for(int x = 0; x < TYPE_COUNT; x++)
    {
        threats::threatType currentThreat = static_cast<threats::threatType>(x);
        QPointer<CustomBarSet> set = new CustomBarSet(QString::fromStdString(threats::threat::mapThreatTypeToShortStr(currentThreat)));
        this->setVector.push_back(set);
    }
}

void ThreatLevelSum::loadData()
{

    std::ifstream file;
    file.open(this->typeRetrieveLocation);
    char convertMe[256];
    try {
        if(!file.fail())
        {
            file.getline(convertMe,256);
            file.close();
        }
        else
        {
            throw std::runtime_error("file::open: no file found");
        }
    }
    catch(std::runtime_error& e)
    {
        ErrorBox(std::string("Exception caught!\n") + e.what());
        exit(1);
    }
    std::string parseStr = std::string(convertMe);
    if(parseStr.length() == 0)
    {
        ErrorBox("Empty stats file, repopulate");
    }
    this->deserialiseData(parseStr);
    //this->mainSeries->append(this->fileSet);
}

void ThreatLevelSum::setData()
{
    this->Chart->addSeries(this->mainSeries);
    this->Chart->setTitle("Threats per type");
    this->Chart->setAnimationOptions(QChart::SeriesAnimations);
    this->Chart->addAxis(this->yAxis,Qt::AlignLeft);
    this->mainSeries->attachAxis(this->yAxis);
}

void ThreatLevelSum::addData(int num)
{
    pass;
}

void ThreatLevelSum::addDataToCategory(std::vector<threats::threatType>& type)
{
    for(const auto& threat : type)
    {
        if(threat == threats::threatType::Type_PUP)
        {
            qreal temp = this->setVector[0]->at(0);
            temp+=1;
            this->setVector[0]->remove(0);
            *(this->setVector[0]) << temp;
        }
        else if(threat == threats::threatType::Type_Unknown)
        {
            qreal temp = this->setVector[1]->at(0);
            temp+=1;
            this->setVector[1]->remove(0);
            *(this->setVector[1]) << temp;
        }
        else if(threat == threats::threatType::Type_Malicious)
        {
            qreal temp = this->setVector[2]->at(0);
            temp+=1;
            this->setVector[2]->remove(0);
            *(this->setVector[2]) << temp;
        }
        else if(threat == threats::threatType::Type_None)
        {
            qreal temp = this->setVector[3]->at(0);
            temp+=1;
            this->setVector[3]->remove(0);
            *(this->setVector[3]) << temp;
        }
    }
}

void ThreatLevelSum::saveData()
{
    std::ofstream file;
    file.open(this->typeRetrieveLocation);
    try
    {
        if(!file.fail())
        {
            file.clear();
            file << this->serialiseData();
            file.close();
        }
        else
        {
            throw std::runtime_error("file::open: file write error");
        }
    }
    catch (std::runtime_error& e)
    {
        ErrorBox(std::string("Exception Caught!\n") + e.what());
        exit(2);
    }

}

void ScannedFrequency::loadData() //load then set
{
    std::ifstream file;
    file.open(this->statsRetrieveLocation);
    char convertMe[256];
    try {
        if(!file.fail())
        {
            file.getline(convertMe,256);
            file.close();
        }
        else
        {
            throw std::runtime_error("file::open: no file found");
        }
    }
    catch(std::runtime_error& e)
    {
        ErrorBox(std::string("Exception caught!\n") + e.what());
        exit(1);
    }
    std::string parseStr = std::string(convertMe);
    if(parseStr.length() == 0)
    {
        ErrorBox("Empty stats file, repopulate");
        this->fileSet = new CustomBarSet("Threats");
        this->mainSeries->append(this->fileSet);
    }
    this->fileSet = this->deserialiseData(parseStr,"Threats");
    this->mainSeries->append(this->fileSet);
}

void ScannedFrequency::setData()
{
    this->yAxis->setRange(0,this->fileSet->getMaxValue());
    this->Chart->addAxis(this->yAxis,Qt::AlignLeft);
    this->Chart->addSeries(this->mainSeries);
    this->Chart->setTitle("Threats found per scan");
    this->Chart->setAnimationOptions(QChart::SeriesAnimations);
}

void ScannedFrequency::saveData()
{
    std::ofstream file;
    file.open(this->statsRetrieveLocation);
    try
    {
        if(!file.fail())
        {
            file.clear();
            file << this->serialiseData(*(this->fileSet));
            file.close();
        }
        else
        {
            throw std::runtime_error("file::open: file write error");
        }
    }
    catch (std::runtime_error& e)
    {
        ErrorBox(std::string("Exception Caught!\n") + e.what());
        exit(2);
    }
    //std::cout << this->serialiseData(*(this->fileSet)) << std::endl;
}


void ScannedFrequency::addData(int num)
{
    this->fileSet->remove(0); //fileSet is essentially a queue.
    *(this->fileSet) << num;
}

std::string ScannedFrequency::serialiseData(CustomBarSet &set) //Call this in saveData, Call saveData when the program finished execution
{

    std::string serialisedReturnString = "";
    if(set.count() == 0)
    {
        return "";
    }
    for(int i = 0; i < set.count(); i++)
    {
        if(i != set.count() - 1)
        {
            std::string curStr = QString::number(set.at(i)).toStdString()+',';
            serialisedReturnString.append(curStr);
        }
        else
        {
            std::string curStr = QString::number(set.at(i)).toStdString();
            serialisedReturnString.append(curStr);
        }
    }
    return serialisedReturnString;
}

std::string ThreatLevelSum::serialiseData()
{
    std::string serialisedReturnString = "";
    for(unsigned int i = 0; i < this->setVector.size(); i++)
    {
        if(i != this->setVector.size() - 1)
        {
            std::string curStr = QString::number(setVector[i]->at(0)).toStdString() + ',';
            serialisedReturnString.append(curStr);
        }
        else
        {
            std::string curStr = QString::number(setVector[i]->at(0)).toStdString();
            serialisedReturnString.append(curStr);
        }
    }
    return serialisedReturnString;
}

QPointer<CustomBarSet> ScannedFrequency::deserialiseData(std::string str,std::string setName)
{
    QPointer<CustomBarSet> set = new CustomBarSet(QString::fromStdString(setName));
    std::vector<int> intVec;
    while(str.find(',') != std::string::npos)
    {
        for(int x = 0; x < static_cast<int>(str.length()); x++)
        {
            if(str[x] == ',')
            {
                try {
                    intVec.push_back(std::stoi(str.substr(0,x)));
                }  catch (std::exception& e) {
                    ErrorBox(std::string("Exception Caught:\n") + e.what());
                    exit(0);
                }
                str.erase(0,x+1);
                break;
            }
        }
    }
    try {
        intVec.push_back(std::stoi(str.substr(0,str.length())));
    }  catch (std::exception& e) {
        ErrorBox(std::string("Exception Caught:\n") + e.what());
        exit(0);
    }
    for(auto x : intVec)
    {
        *(set) << x;
    }
    return set;
}

void ThreatLevelSum::deserialiseData(std::string str)
{
    std::vector<int> intVec;
    while(str.find(',') != std::string::npos)
    {
        for(int x = 0; x < static_cast<int>(str.length()); x++)
        {
            if(str[x] == ',')
            {
                try {
                    intVec.push_back(std::stoi(str.substr(0,x)));
                }  catch (std::exception& e) {
                    ErrorBox(std::string("Exception Caught:\n") + e.what());
                    exit(0);
                }
                str.erase(0,x+1);
                break;
            }
        }
    }
    try {
        intVec.push_back(std::stoi(str.substr(0,str.length())));
    }  catch (std::exception& e) {
        ErrorBox(std::string("Exception Caught:\n") + e.what());
        exit(0);
    }
    unsigned int c = 0;
    int max = 0;
    for(auto& set: this->setVector)
    {
        *(set) << intVec[c];
        this->mainSeries->append(set);
        if(set->at(0) > max)
        {
            max = set->at(0) + 1;
        }
        c++;
    }
    this->yAxis->setMax(max);
}
