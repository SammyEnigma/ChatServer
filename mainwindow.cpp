#include "chatserver.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ChatServer *server = new ChatServer(QHostAddress::Any, 9000, this);
    connect(server, SIGNAL(systemMessage(QString)), ui->plainTextEdit, SLOT(appendPlainText(QString)));
    ui->plainTextEdit->setPlainText(tr("Server started at 0.0.0.0:9000"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
