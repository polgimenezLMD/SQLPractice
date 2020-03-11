#ifndef SQLPRACTICE_H
#define SQLPRACTICE_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>

#include <ui_sqlpractice.h>


QT_BEGIN_NAMESPACE
namespace Ui { class SQLPractice; }
QT_END_NAMESPACE

class SQLPractice : public QMainWindow
{
    Q_OBJECT

public:
    SQLPractice();
    ~SQLPractice();
    QSqlDatabase db;
    QString file_path_;
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void FileSelection();
    void SettingSelection();
    void ExportJSON();
    void ImportJSON();
    void DeleteRows();
    void OpenAbout();

private:
    Ui::SQLPractice *ui;
    QString id_;
    QString file_;
};

#endif // SQLPRACTICE_H
