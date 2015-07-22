#ifndef MONGOITEMS_H
#define MONGOITEMS_H

#include "QDateTime"
#include "QVariant"
#include "mongo/bson/bson.h"
#include "qstring.h"
using mongo::BSONObj;

using namespace std;
using namespace mongo;

class MongoItems
{
private:
    BSONObj _object;
public:
    MongoItems (BSONObj object);
    QVariant getItem(QString fieldName);
};


#endif // MONGOITEMS_H
