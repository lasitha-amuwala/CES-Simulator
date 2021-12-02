#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "menu.h"

const QStringList MENUS = {"Main Menu", "Frequencies", "Wave Forms", "Countdown Cycles", "Start Therapy"};
const QStringList FREQUENCIES = {"0.5 Hz", "77 Hz", "100 Hz"};
const QStringList WAVEFORMS = {"Alpha", "Beta", "Gamma"};
const QStringList COUNTDOWN_CYCLES = {"20 mins", "40 mins", "60 mins"};

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->CESWidget->setVisible(false);

    frequency = 0.5;
    waveform = "Alpha";
    countdownCycle = 20;
    powerState = false;

    mainMenu = Menu(MENUS[0], {MENUS[1], MENUS[2], MENUS[3], MENUS[4]});
    frequencyMenu = Menu(MENUS[1], FREQUENCIES);
    waveformMenu = Menu(MENUS[2], WAVEFORMS);
    countdownCycleMenu = Menu(MENUS[3], COUNTDOWN_CYCLES);

    connect(ui->powerButton, SIGNAL(clicked()), this, SLOT(changePowerState()));
    connect(ui->downButton, SIGNAL(pressed()), this, SLOT(navigateDown()));
    connect(ui->upButton, SIGNAL(pressed()), this, SLOT(navigateUp()));
    connect(ui->homeButton, SIGNAL(pressed()), this, SLOT(goHome()));
    connect(ui->okButton, SIGNAL(pressed()), this, SLOT(okButton()));

    displayOptions();
    disableButtons(true);
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
        drawMenu(mainMenu);
        disableButtons(false);
    }else{
        disableButtons(true);
    }
}

void MainWindow::disableButtons(bool x){
    ui->upButton->setDisabled(x);
    ui->downButton->setDisabled(x);
    ui->homeButton->setDisabled(x);
    ui->okButton->setDisabled(x);
    ui->minusButton->setDisabled(x);
    ui->plusButton->setDisabled(x);
    ui->lockButtton->setDisabled(x);
}

void MainWindow::navigateDown(){
    int currRow = ui->MainMenu->currentRow();
    int nextRow = (currRow + 1 == ui->MainMenu->count())? 0 : currRow + 1;
    ui->MainMenu->setCurrentRow(nextRow);
    currentMenu.setSelectedRow(nextRow);
}

void MainWindow::navigateUp(){
    int currRow = ui->MainMenu->currentRow();
    int nextRow = (currRow == 0)? ui->MainMenu->count() - 1 : currRow - 1;
    currentMenu.setSelectedRow(nextRow);
    ui->MainMenu->setCurrentRow(nextRow);
}

void MainWindow::drawMenu(Menu &menu){
    ui->MainMenu->clear();
    ui->MainMenu->addItems(menu.getMenuItems());
    ui->MainMenu->setCurrentRow(menu.getSelectedRow());
    ui->menuLabel->setText(menu.getName());
    currentMenu = menu;
}

void MainWindow::displayOptions(){
    ui->frequencyLabel->setText(QString::number(frequency) + " Hz");
    ui->waveformLabel->setText(waveform);
    ui->countdownLabel->setText(QString::number(countdownCycle) + " mins");
}

void MainWindow::goHome(){
    drawMenu(mainMenu);
}

void MainWindow::okButton(){
    QString menu = currentMenu.getName();
    int selectedRow = currentMenu.getSelectedRow();

    if(menu == MENUS[0]){
        if(selectedRow == 0){ drawMenu(frequencyMenu); }
        else if (selectedRow == 1){ drawMenu(waveformMenu);}
        else if (selectedRow == 2){ drawMenu(countdownCycleMenu);}
        else if (selectedRow == 3){}

    } else if (menu == MENUS[1]){

        if(selectedRow == 0){ frequency = 0.5; }
        else if(selectedRow == 1){ frequency = 77; }
        else { frequency = 100; }
        displayOptions();

    } else if (menu == MENUS[2]){

        if(selectedRow == 0){ waveform = WAVEFORMS[0]; }
        else if(selectedRow == 1){ waveform = WAVEFORMS[1]; }
        else { waveform = WAVEFORMS[2]; }
        displayOptions();

    } else if (menu == MENUS[3]){

        if(selectedRow == 0){ countdownCycle = 20; }
        else if(selectedRow == 1){ countdownCycle = 40; }
        else { countdownCycle = 60; }
        displayOptions();

    } else {
        qDebug() << "Start Therapy";
    }

    qDebug() << frequency;
    qDebug() << waveform;
    qDebug() << countdownCycle;
}
