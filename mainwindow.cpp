#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "menu.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->CESWidget->setVisible(false);



    powerState = false;

    connect(ui->powerButton, SIGNAL(clicked()), this, SLOT(changePowerState()));
    connect(ui->downButton, SIGNAL(pressed()), this, SLOT(navigateDown()));
    connect(ui->upButton, SIGNAL(pressed()), this, SLOT(navigateUp()));
    connect(ui->okButton, SIGNAL(pressed()), this, SLOT(okButton()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changePowerState(){
    bool isVisible = ui->CESWidget->isVisible();
    ui->CESWidget->setVisible(!isVisible);
    powerState = !powerState;

    if(powerState){
        Menu mainMenu("Main Menu", {"Frequency", "Wave Form", "Countdown Cycles"}, 0);
        ui->MainMenu->setCurrentRow(0);
        currentMenu = mainMenu;
        drawMenu(mainMenu);
    }
}

void MainWindow::navigateDown(){
    int currRow = ui->MainMenu->currentRow();
    int nextRow = (currRow + 1 == ui->MainMenu->count())? 0 : currRow + 1;
    ui->MainMenu->setCurrentRow(nextRow);
    currentMenu.setSelectedRow(nextRow);
    qDebug() << QString::number(currentMenu.getSelectedRow());
}

void MainWindow::navigateUp(){
    int currRow = ui->MainMenu->currentRow();
    int nextRow = (currRow == 0)? ui->MainMenu->count() - 1 : currRow - 1;
    ui->MainMenu->setCurrentRow(nextRow);
}

void MainWindow::drawMenu(Menu &m){
    ui->MainMenu->clear();
    ui->MainMenu->addItems(m.getMenuItems());
    ui->MainMenu->setCurrentRow(m.getSelectedRow());
    ui->menuLabel->setText(m.getName());
}

void MainWindow::okButton(){
    Menu frequencyMenu("Frequency", {"0.5hz", "77hz", "100hz"}, 0);
    Menu waveFormMenu("Wave Form", {"Alpha", "Beta", "Gamma"}, 0);
    Menu countdownCycles("Countdown Cycles", {"20 mins", "40 mins", "60 mins"}, 0);

    QString menu = currentMenu.getName();
    int selectedRow = currentMenu.getSelectedRow();

    qDebug() << QString::number(currentMenu.getSelectedRow());
    if(menu == "Main Menu"){
        qDebug() << "nossss";
        switch(selectedRow){
            case 0:
                currentMenu = frequencyMenu;
                drawMenu(frequencyMenu);
                break;
            case 1:
                currentMenu = waveFormMenu;
                drawMenu(waveFormMenu);
                break;
            case 2:
                currentMenu = countdownCycles;
                drawMenu(countdownCycles);
                break;
        }
    }else{
        qDebug() << "no";
    }
    qDebug() << currentMenu.getName();
}
/*
 *         QListWidgetItem *item = new QListWidgetItem();
        item->setText(menuItems.at(i))
        */
