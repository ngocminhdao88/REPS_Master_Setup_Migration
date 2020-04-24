#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    buttonSignalSlotSetup();
}

void MainWindow::onOpenButtonClicked() {
    QString path = QFileDialog::getOpenFileName(this,
                                                "Open Master File",
                                                "",
                                                "Master File (*.mas)");

    if (path.isEmpty())
        return;

    masterFile = path;

}

void MainWindow::onSaveButtonClicked() {
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
    connect(ui->saveAsButton, &QAbstractButton::clicked, this, &MainWindow::onSaveAsButtonClicked);
    connect(ui->autoPopulateButton, &QAbstractButton::clicked, this, &MainWindow::onAutoPopulateButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

