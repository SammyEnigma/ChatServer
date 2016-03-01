#include "chatserver.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    new ChatServer(QHostAddress::Any, 9000, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
