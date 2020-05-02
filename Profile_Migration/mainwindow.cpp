#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include "tableview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_model(new DataModel(this))
{
    ui->setupUi(this);
    ui->tableView->setModel(m_model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnWidth(2, 600);
    ui->tableView->setColumnHidden(3, true); //status column

    tcpClient = new TCPClient(this);

    buttonSignalSlotSetup();
    signalSlotSetup();

    tcpClient->connectToHost();
}

void MainWindow::onOpenButtonClicked() {
    QString path = QFileDialog::getOpenFileName(this,
                                                "Open Master File",
                                                "",
                                                "Master File (*.mas)");

    if (path.isEmpty())
        return;

    m_model->setMasterFile(path);
    ui->lineEdit->setText(path);

    tcpClient->sendRequest(Request::Read, path);
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

    m_model->setMasterFile(path);
    ui->lineEdit->setText(path);
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
    connect(ui->tableView, &TableView::rightMouseClicked, m_model, &DataModel::clearSetupFilePath);
    connect(ui->tableView, &TableView::doubleClicked, m_model, &DataModel::selectSetupFile);

    connect(tcpClient, &TCPClient::replyReceived, this, &MainWindow::onReplyReceived);
    connect(tcpClient, &TCPClient::errorMessage, this, &MainWindow::errorMessage);
}

void MainWindow::onReplyReceived(const QByteArray data) {
    uint8_t replyType = data.at(0);
    QByteArray replyData = data.right(data.size() - 1);

    switch (replyType) {
    case 1:
        m_model->setSetupFilePaths(QString(replyData));
        break;
    case 2:
        qDebug() << "Write reply";
        qDebug() << replyData;
        break;
    }
}

void MainWindow::errorMessage(QString errorString) {
    qDebug() << errorString;
    statusBar()->showMessage(errorString);
}

MainWindow::~MainWindow()
{
    delete ui;
}

