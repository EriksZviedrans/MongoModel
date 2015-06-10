#include "mongotablemodel.h"

MongoTableModel::MongoTableModel(QObject *parent):
    QAbstractTableModel(parent)
{
    this->host = "localhost";
    this->port = 27017;
    this->dbName = "dellper";
    this->user = "deff";
    this->passwd = "qwerty";

    this->mongoDatabase = new DBClientConnection();
    this->connect();
    qDebug() << this->mongoDatabase->isStillConnected();
}

MongoTableModel::~MongoTableModel()
{

}

QVariant MongoTableModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

bool MongoTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return true;
}

int MongoTableModel::rowCount(const QModelIndex &parent) const
{
    return rowsCount;
}

int MongoTableModel::columnCount(const QModelIndex &parent) const
{
    return 10;
}

QModelIndex MongoTableModel::setRowsSize(int count)
{
    this->rowsCount =  this->rowsCount + count;
    return this->index(this->rowsCount,10);//QModelIndex(10,this->rowsCount);
}

void MongoTableModel::connect()
{
    try {
        mongo::client::initialize();
        this->mongoDatabase->connect(host.toStdString()+":"+QString::number(port).toStdString(),errmsg);
        errors = QString::fromStdString(errmsg);
        if (errors == "") {
                   try
                   {
                       this->mongoDatabase->auth(dbName.toStdString(),user.toStdString(),passwd.toStdString(), errmsg);
                       errors = QString::fromStdString(errmsg);
                   }
                   catch ( const DBException &e )
                   {
                       errors = e.what();
                   }
         }
    }
    catch (const DBException &e )
    {
       errors = e.what();
    }

}

