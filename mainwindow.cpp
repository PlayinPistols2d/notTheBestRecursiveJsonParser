#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString json = "{\"error\":{\"errorcode\":1,\"errorstring\":\"\",\"smth\":{\"value\":{\"chips\":[\"qxc\",\"sys\",3]}}},\"login\":\"qwerty\",\"logintoken\":{\"token\":\"4daaf6b3dd5a26a2ad2c436e564bfa4d6c439ce4d0d6cd66705a8bdadddddaa0\"}}";

    temp = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject obj  = temp.object();

    QStandardItemModel *model = new QStandardItemModel();

    qDebug() << endl << endl;
    parseJsonToTreeView(obj, model);

    ui->treeView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parseJsonToTreeView(QJsonObject convertedJsonString, QStandardItemModel *model, QStandardItem *parent )
{
    QJsonObject::iterator it;
    qDebug() << "Json string: " << convertedJsonString << endl;
    isTopLevel = false;
    for(it=convertedJsonString.begin();it!=convertedJsonString.end();it++) {
        if(it.value().isObject()) {
            qDebug() << " Object : " << it.key() << " Value : "<< it.value() << "Proceed calculations..." << endl;

            QStandardItem *item = new QStandardItem(it.key());
            if((parent == NULL) || isTopLevel) {
                model->appendRow(item);
            }
            else {
                parent->appendRow(item);
            }
            parent = item;
            qDebug() << " ... sending parent: " << parent << "made from item: " << item << endl;
            parseJsonToTreeView(it.value().toObject(), model, parent);
        }
        else {
            qDebug() << " Finished Object: " << it.key() << "Value : "<< it.value() << " Parent: " << parent << endl;

            QStandardItem *item = new QStandardItem(it.key() + " - " + it.value().toString());
            if (it.value().isDouble()){
                item = new QStandardItem(QString::number(it.value().toInt()));
                qDebug() << "Int" << endl;
            } else if (it.value().isBool()) {
                item = new QStandardItem(it.value().toBool());
                qDebug() << "Bool" << endl;
            } else if (it.value().isArray()) {
                qDebug() << "Array" << endl;
                QJsonArray array = it.value().toArray();
                QJsonArray::iterator arrayIterator;
                QString convertedToStringArray;
                for(arrayIterator = array.begin();arrayIterator != array.end(); arrayIterator++) {
                    if(arrayIterator->isDouble()) {
                        convertedToStringArray.append(QString::number(it.value().toInt()));
                    } else{
                        convertedToStringArray.append(arrayIterator->toString());
                    }
                    convertedToStringArray.append(", ");
                }
                convertedToStringArray.chop(2);
                item = new QStandardItem(convertedToStringArray);
            }
            if(isTopLevel) {
                model->appendRow(item);
            }else {
                parent->appendRow(item);
            }
            if(convertedJsonString.length()==1) {
                isTopLevel = true;
            }
        }
    }
}

