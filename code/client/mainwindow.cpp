#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tableview.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

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

    tcpClient->sendRequest(Request::SetMasterFileRequest, path);
    tcpClient->sendRequest(Request::ReadMasterFileRequest, "");
}

void MainWindow::onSaveButtonClicked() {
    QString setupFilePaths = m_model->getSetupFilePaths();

    tcpClient->sendRequest(Request::WriteMasterFileRequest, setupFilePaths );
}

void MainWindow::onSaveAsButtonClicked() {
    QString path = QFileDialog::getSaveFileName(this,
                                                "Save Master File",
                                                "",
                                                "Master File (*.mas)");

    if (path.isEmpty())
        return;

    QString setupFilePaths = m_model->getSetupFilePaths();

    m_model->setMasterFile(path);
    m_model->checkAllFilesStatus();
    ui->lineEdit->setText(path);

    tcpClient->sendRequest(Request::SetMasterFileRequest, path);
    tcpClient->sendRequest(Request::WriteMasterFileRequest, setupFilePaths);
}

void MainWindow::onChangeDirectoryButtonClicked() {
    QString path = QFileDialog::getExistingDirectory(this,
                                                     "Select folder of Setup files");

    if (path.isEmpty())
        return;

    m_model->changeDirectory(path);
}

void MainWindow::buttonSignalSlotSetup() {
    connect(ui->openButton, &QAbstractButton::clicked, this, &MainWindow::onOpenButtonClicked);
    connect(ui->saveButton, &QAbstractButton::clicked, this, &MainWindow::onSaveButtonClicked);
    connect(ui->saveAsButton, &QAbstractButton::clicked, this, &MainWindow::onSaveAsButtonClicked);
    connect(ui->changeDirectoryButton, &QAbstractButton::clicked, this, &MainWindow::onChangeDirectoryButtonClicked);
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
    case Request::SetMasterFileRequest:
        if (replyData == "OK") {
            statusBar()->showMessage("Set master file successfully");
        }
        break;
    case Request::ReadMasterFileRequest:
        if (!replyData.isEmpty()) {
            m_model->setSetupFilePaths(QString(replyData));
            statusBar()->showMessage("Read master file successfully");
        } else {
            statusBar()->showMessage("Read master file not successfully");
        }
        break;
    case Request::WriteMasterFileRequest:
        if (replyData == "OK") {
            statusBar()->showMessage("Write master file successfully");
        }
        break;
    }
}

void MainWindow::errorMessage(QString errorString) {
    statusBar()->showMessage(errorString);
    QMessageBox::critical(this,
                          "Error",
                          errorString
                          );
}

MainWindow::~MainWindow()
{
    delete ui;
}

