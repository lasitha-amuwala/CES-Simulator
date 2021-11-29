#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->powerButton, SIGNAL(clicked()), this, SLOT(changePowerState()));

    ui->CESWidget->setVisible(false);
    powerState = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changePowerState(){
    bool isVisible = ui->CESWidget->isVisible();
    ui->CESWidget->setVisible(!isVisible);
    powerState = !powerState;
    qDebug() << bool(powerState);
}
