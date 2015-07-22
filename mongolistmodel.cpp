#include "mongolistmodel.h"
#include <stdlib.h>

static const int bufferSize(200);

MongoListModel::MongoListModel(QObject *parent)
: QAbstractListModel(parent)
{
    host = "ds049848.mongolab.com";
    port = 49848;
    dbName = "mongolab_database";
    user = "mongo";
    passwd = "mongolab";
    connect();
    mongoHeader();
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

int MongoListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mongoHeaderDataList.size();
}

QVariant MongoListModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    int row = index.row();

    if (row >= mongoDataList.size() || row < 0)
        return QVariant();

    if (role == Qt::DisplayRole){ 
        return mongoDataList.at(row)->getItem(mongoHeaderDataList.at(index.column()));
    }
    return QVariant();
}

QVariant MongoListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
            return QVariant();
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return mongoHeaderDataList.at(section);
    }else{
        return QString("%1").arg( section + 1 );
    }
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

    BSONObj query;
    if (value != 0)
        query = BSONObjBuilder().appendRegex("data", value.toStdString()).obj();

    auto_ptr<DBClientCursor> cursor = mongoDatabase->query("mongolab_database.random_data",query, bufferSize, position);
      while (cursor->more()) {
         BSONObj object = cursor->next();
         MongoItems * it = new MongoItems(object);
         mongoDataList.append(it);
      }
}

void MongoListModel::mongoHeader() const
{
    BSONObj objOne = mongoDatabase->findOne("mongolab_database.random_data",Query());
    mongoHeaderDataList.clear();
    for ( BSONObj::iterator iter = objOne.begin(); iter.more(); )
    {
        BSONElement element = iter.next();
        QString key = QString::fromStdString( element.fieldName() );
        if ( key == "_id" ) continue;
        mongoHeaderDataList.append(key);

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
