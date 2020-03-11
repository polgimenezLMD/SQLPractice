#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QString>
#include <QJsonArray>


class JSONParser
{
public:
    JSONParser();
    QString JSONExport(QString sqlquery);
    QJsonArray JSONImport(QString fileJSON);

};

#endif // JSONPARSER_H
