#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpClient = new TCPClient(this);

    buttonSignalSlotSetup();
    signalSlotSetup();

    tcpClient->connectToHost();
}

void MainWindow::onOpenButtonClicked() {
//    QString path = QFileDialog::getOpenFileName(this,
//                                                "Open Master File",
//                                                "",
//                                                "Master File (*.mas)");

//    if (path.isEmpty())
//        return;

//    masterFile = path;

    tcpClient->sendRequest(Request::Read, "Read Command");

}

void MainWindow::onSaveButtonClicked() {
    tcpClient->sendRequest(Request::Save, "Save Command");
}

void MainWindow::onSaveAsButtonClicked() {
    QString path = QFileDialog::getSaveFileName(this,
                                                "Save Master File",
                                                "",
                                                "Master File (*.mas)");

    if (path.isEmpty())
        return;

    masterFile = path;
}

void MainWindow::onAutoPopulateButtonClicked() {
    QString path = QFileDialog::getExistingDirectory(this,
                                                     "Select folder of Setup files");

    if (path.isEmpty())
        return;
}

void MainWindow::buttonSignalSlotSetup() {
    connect(ui->openButton, &QAbstractButton::clicked, this, &MainWindow::onOpenButtonClicked);
    connect(ui->saveButton, &QAbstractButton::clicked, this, &MainWindow::onSaveButtonClicked);
    //connect(ui->saveAsButton, &QAbstractButton::clicked, this, &MainWindow::onSaveAsButtonClicked);
    connect(ui->autoPopulateButton, &QAbstractButton::clicked, this, &MainWindow::onAutoPopulateButtonClicked);
}

void MainWindow::signalSlotSetup() {
    connect(tcpClient, &TCPClient::errorMessage, this, &MainWindow::errorMessage);

}

void MainWindow::errorMessage(QString errorString) {
    qDebug() << errorString;
    statusBar()->showMessage(errorString);
}

MainWindow::~MainWindow()
{
    delete ui;
}

