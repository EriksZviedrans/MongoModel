#include "mongoitems.h"

MongoItems::MongoItems(BSONObj object)
{
    this->_object = object.copy();
}

QVariant MongoItems::getItem(QString fieldName, int type)
{
    switch (type) {
    case 8:
        return this->_object.getBoolField(fieldName.toStdString());
        break;
    case 16:
        return this->_object.getField(fieldName.toStdString()).Int();
        break;
    case 1:
        return this->_object.getField(fieldName.toStdString()).Double();
    case 9: {
        BSONElement element = this->_object.getField(fieldName.toStdString());
        time_t elapsedTime = element.date().toTimeT();
        return QDateTime::fromTime_t(elapsedTime).date();
        } break;
    default:
        return this->_object.getStringField(fieldName.toStdString());
        break;
    }
}
