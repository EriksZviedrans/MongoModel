#include "mongolistmodel.h"
#include <stdlib.h>

static const int bufferSize(200);

MongoListModel::MongoListModel(QObject *parent)
: QAbstractListModel(parent)
{
    host = "ds031591.mongolab.com";
    port = 31591;
    dbName = "mongolab_database";
    user = "mongo";
    passwd = "mongolab";
    connect();

    mongoQueryDown(0);
    mongoRowCount = bufferSize;
}

MongoListModel::~MongoListModel()
{
}

int MongoListModel::rowCount(const QModelIndex &) const
{
    return mongoRowCount;
}

QVariant MongoListModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    int row = index.row();

    if (row >= mongoDataList.size() || row < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
        return mongoDataList.at(row);

    return QVariant();
}

bool MongoListModel::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (mongoRowCount-1 < mongoDataList.size()){
        return true;
    }
    else {

        return false;
    }
}

void MongoListModel::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent);
    int remainder = mongoDataList.size() - mongoRowCount;
    int itemsToFetch = qMin(bufferSize, remainder);
    mongoQueryDown(mongoRowCount);
    beginInsertRows(QModelIndex(), mongoRowCount, mongoRowCount+itemsToFetch-1);

    mongoRowCount += itemsToFetch;

    endInsertRows();
}

void MongoListModel::connect()
{
    try {
        mongoDatabase = new DBClientConnection();
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

void MongoListModel::mongoQueryDown(int position,  QString value) const
{
    BSONObj b = BSON("item" << 1);
    BSONObj query;
    if (value != 0)
        query = BSONObjBuilder().appendRegex("item", value.toStdString()).obj();

    auto_ptr<DBClientCursor> cursor = mongoDatabase->query("mongolab_database.random_data",query, bufferSize, position, &b);
      while (cursor->more()) {
         BSONObj object  = cursor->next();
         mongoDataList.append(QString::fromStdString(object.getStringField("item")));
      }
}

void MongoListModel::searchBy(const QString &value)
{
    beginResetModel();
    mongoDataList.clear();
    mongoQueryDown(0, value);
    mongoRowCount = bufferSize;
    endResetModel();
}
