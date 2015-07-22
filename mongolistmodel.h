#ifndef MONGOLISTMODEL_H
#define MONGOLISTMODEL_H

#include <QContiguousCache>
#include <QAbstractListModel>
#include <QDebug>

#include "mongoitems.h"

#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"

using mongo::BSONArray;
using mongo::BSONArrayBuilder;
using mongo::BSONObj;
using mongo::BSONObjBuilder;
using mongo::BSONElement;

using namespace std;
using namespace mongo;

QT_FORWARD_DECLARE_CLASS(QTimer)

class MongoListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MongoListModel(QObject *parent = 0);
    ~MongoListModel();

    int rowCount(const QModelIndex & = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &, int) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

    QString host;
    QString dbName;
    QString user;
    QString passwd;
    int port;

    QString errors;
    string errmsg;
protected:
    bool canFetchMore(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    void fetchMore(const QModelIndex &parent) Q_DECL_OVERRIDE;

private:
    DBClientConnection* mongoDatabase;
    void connect();

    mutable QList< MongoItems *> mongoDataList;
    mutable QStringList mongoHeaderDataList;
    int mongoRowCount;
    void mongoQueryDown(int, QString value = 0) const;
    void mongoHeader() const;


public slots:
    void searchBy(const QString &value);

};

#endif // MONGOLISTMODEL_H
