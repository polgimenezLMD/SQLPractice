#include "jsonparser.h"
#include <QSqlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSqlRecord>
#include <QVariant>
#include <QFile>
#include <QDebug>
#include <QTableWidget>
#include <ui_sqlpractice.h>

JSONParser::JSONParser()
{

}


QString JSONParser::JSONExport(QString sqlquery){
    QSqlQuery query;
    query.setForwardOnly(true);
    query.exec(sqlquery);

    QJsonDocument  json;
    QJsonArray     recordsArray;

      while(query.next())
      {
         QJsonObject recordObject;
            for(int x=0; x < query.record().count(); x++)
            {
            recordObject.insert( query.record().fieldName(x),QJsonValue::fromVariant(query.value(x)) );
            }
         recordsArray.push_back(recordObject);
      }
      json.setArray(recordsArray);

      return json.toJson();


}


QJsonArray JSONParser::JSONImport(QString fileJSON){
    QString val;
    QFile file;
    file.setFileName(fileJSON);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonArray json_array = doc.array();
    return json_array;
}
