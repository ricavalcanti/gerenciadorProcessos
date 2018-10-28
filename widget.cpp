#include "widget.h"
#include "ui_widget.h"
#include <QListWidget>
#include <process.cpp>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QListWidget *listWidget = new QListWidget(this);
    QString* x = new QString("Meu texto");
    Process* p = new Process(x);
    ui->listWidget->addItem(p->getText());
}

Widget::~Widget()
{
    delete ui;
}
