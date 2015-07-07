#include "mongoitems.h"

MongoItems::MongoItems(BSONObj object)
{
    this->_object = object.copy();
}

QString MongoItems::getItem(QString fieldName, int type)
{
    switch (type) {
    case 8:
        return this->_object.getBoolField(fieldName.toStdString()) ? "yes" : "no";
        break;
    case 16:
        return QString::number(this->_object.getField(fieldName.toStdString()).Int());
        break;
    case 1:
        return QString::number(this->_object.getField(fieldName.toStdString()).Double());
    case 9: {
        BSONElement element = this->_object.getField(fieldName.toStdString());
        time_t elapsedTime = element.date().toTimeT();
        return QDateTime::fromTime_t(elapsedTime).date().toString("dd-MM-yyyy");
        } break;
    default:
        return QString::fromStdString(this->_object.getStringField(fieldName.toStdString()));
        break;
    }

}
