#include "sqlpractice.h"
#include "jsonparser.h"
#include <QListWidget>
#include <QDebug>
#include <QSqlError>
#include <QFile>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QJsonArray>
#include <QJsonObject>

SQLPractice::SQLPractice() : QMainWindow(), ui(new Ui::SQLPractice)
{
    ui->setupUi(this);
    ui->menuExport->setEnabled(false);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Id" << "SetKey" << "SetValue");
    connect(ui->actionOpen_File,SIGNAL(triggered()),this,SLOT(FileSelection()));
    connect(ui->actionJSON,SIGNAL(triggered()),this,SLOT(ExportJSON()));
    connect(ui->actionJSON_2,SIGNAL(triggered()),this,SLOT(ImportJSON()));
}

SQLPractice::~SQLPractice()
{

}

void SQLPractice::contextMenuEvent(QContextMenuEvent *)
{
    QMenu *menu = new QMenu();
    QAction *del = new QAction("Delete",this);
    connect(del,SIGNAL(triggered()),this,SLOT(DeleteRows()));
    menu->addAction(del);
    menu->exec(QCursor::pos());
}

void SQLPractice::FileSelection()
{
    file_=QFileDialog::getOpenFileName(this,QString(),QString(),"(*.db)");
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString filename = file_;
    if(QFile::exists(filename))
        db.setDatabaseName(filename);
    else
        qDebug() << "unable to open file" << filename ;
    if(!db.open()) {
        qDebug() << "unable to open";
        return;
    }
    QSqlQuery q;
    q.exec("SELECT * FROM Settings");
    while(q.next()){
        QString data = q.value(1).toString();
        QListWidgetItem *item = new QListWidgetItem(data, ui->listWidget);
        item->setData(Qt::UserRole, q.value(0));
    }
    q.clear();
    connect(ui->listWidget,SIGNAL(itemSelectionChanged()),this,SLOT(SettingSelection()));
}

void SQLPractice::SettingSelection()
{
    ui->menuExport->setEnabled(true);
    ui->tableWidget->clear();
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Id" << "SetKey" << "SetValue");
    QListWidgetItem* currentItem = ui->listWidget->currentItem();
    if (currentItem != 0) {
         QVariant data = currentItem->data(Qt::UserRole);
         id_ = data.toString();
    }
    QSqlQuery count;
    count.exec("SELECT * FROM SetDetail WHERE setId="+id_);
    int rows = 0;
    while(count.next()){
        rows++;
    }

    ui->tableWidget->setRowCount(rows);
    count.clear();
    QSqlQuery q;
    q.exec("SELECT * FROM SetDetail WHERE setId="+id_);
    int index = 0;
    while(q.next()){
        ui->tableWidget->setItem(index,0,new QTableWidgetItem(q.value(0).toString()));
        ui->tableWidget->setItem(index,1,new QTableWidgetItem(q.value(2).toString()));
        ui->tableWidget->setItem(index,2,new QTableWidgetItem(q.value(3).toString()));
        index++;
    }
    q.clear();
}


void SQLPractice::ExportJSON()
{
    file_path_ = QFileDialog::getSaveFileName( this,QString(),QString()) + ".JSON";
    QString sqlquery = "SELECT * FROM SetDetail WHERE setId="+id_;
    JSONParser * exp = new JSONParser();
    QString json_string = exp->JSONExport(sqlquery);
    QFile save_file(file_path_);
    save_file.open(QIODevice::WriteOnly);
    save_file.write(json_string.toLocal8Bit());
    save_file.close();
}


void SQLPractice::ImportJSON()
{
    file_=QFileDialog::getOpenFileName(this,QString(),QString(),"(*.JSON)");
    JSONParser * imp = new JSONParser();
    QJsonArray getjson = imp->JSONImport(file_);
    int rows = getjson.size();
    ui->tableWidget->setRowCount(rows);
    for(int i=0;i<getjson.size();i++){
        QJsonObject json_obj = getjson[i].toObject();
        QVariantMap json_map = json_obj.toVariantMap();
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(json_map["id"].toString()));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(json_map["setKey"].toString()));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(json_map["setValue"].toString()));
    }

}

void SQLPractice::DeleteRows()
{
    QItemSelection selection = ui->tableWidget->selectionModel()->selection();
    QAbstractItemModel *model = ui->tableWidget->model();
    QList<int> removeRows;
    foreach(QModelIndex index, selection.indexes()) {
        if(!removeRows.contains(index.row())) {
            removeRows.append(index.row());
        }
    }
    QStringList list;
    for(int i=0;i<removeRows.count();++i)
    {
        QModelIndex index = model->index(removeRows[i], 0); // The first column data.
        list.append(index.data().toString());
    }
    for(int i=0;i<list.count();++i)
    {
        QSqlQuery q;
        QString num = list[i];
        q.exec("DELETE FROM SetDetail WHERE Id="+num);
        q.exec("commit");
        q.clear();
    }
    SettingSelection();
}

