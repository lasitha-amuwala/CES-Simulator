#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "menu.h"
#include <QLCDNumber>

const QStringList MENUS = {"Main Menu", "Frequencies", "Wave Forms", "Countdown Cycles", "Start Therapy","history"};
const QList<float> FREQUENCIES = {0.5, 77, 100};
const QStringList WAVEFORMS = {"Alpha", "Beta", "Gamma"};
const QList<int>  COUNTDOWN_CYCLES = {20, 40, 60};
const QStringList THERAPY_BUTTONS = {"Stop Therapy", "Save Therapy"};

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->displayWidget->setVisible(false);


    therapies.append(new Therapy());
    powerState = false;
    skinContact = false;
    saveTherapy = false;
    battery = 100;
    timer = new QTimer(this);
    power = 1;

    QStringList timeMenu;
    QStringList freqMenu;
    foreach (int time, COUNTDOWN_CYCLES) {timeMenu<< QString::number(time) + " mins";}
    foreach (float freq, FREQUENCIES) {freqMenu<< QString::number(freq) + " Hz";}

    mainMenu = Menu(MENUS[0], {MENUS[1], MENUS[2], MENUS[3], MENUS[4], MENUS[5]});
    waveformMenu = Menu(MENUS[2], WAVEFORMS);
    frequencyMenu = Menu(MENUS[1], freqMenu);
    countdownCycleMenu = Menu(MENUS[3], timeMenu);
    therapyMenu = Menu(MENUS[4],THERAPY_BUTTONS);

    connect(ui->powerButton, SIGNAL(clicked()), this, SLOT(changePowerState()));
    connect(ui->downButton, SIGNAL(pressed()), this, SLOT(navigateDown()));
    connect(ui->upButton, SIGNAL(pressed()), this, SLOT(navigateUp()));
    connect(ui->homeButton, SIGNAL(pressed()), this, SLOT(goHome()));
    connect(ui->okButton, SIGNAL(pressed()), this, SLOT(okButton()));
    connect(ui->plusButton, SIGNAL(pressed()), this, SLOT(increaseCurrent()));
    connect(ui->minusButton, SIGNAL(pressed()), this, SLOT(decreaseCurrent()));
    connect(ui->toggleElectrodes, SIGNAL(pressed()), this, SLOT(toggleElectrodes()));
    connect(ui->toggleElectrodes_2, SIGNAL(pressed()), this, SLOT(toggleElectrodes()));
    connect(ui->setBattery, SIGNAL(valueChanged(double)), this, SLOT(forceBattery(double)));
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);

    displayOptions();
    disableButtons(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changePowerState(){
    powerState = !powerState;
    if(powerState){
        if(battery==0){
            powerState = false;
        }else{
            drawMenu(mainMenu);
            disableButtons(false);
        }
    }else{
        disableButtons(true);
        shutdownTherapy();
    }
    ui->displayWidget->setVisible(powerState);
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
void MainWindow::increaseCurrent(){
    power += 1;
    if(power>10) power = 10;
    therapies[therapies.size()-1]->setPower(power);
    ui->powerLevelBar->setValue(power);
    displayOptions();
}

void MainWindow::decreaseCurrent(){
    power -= 2;
    if(power<1) power = 1;
    ui->powerLevelBar->setValue(power);
    therapies[therapies.size()-1]->setPower(power);
    displayOptions();
}

void MainWindow::toggleElectrodes(){
    skinContact = !skinContact;
    ui->electrodes->setChecked(skinContact);
}

void MainWindow::drawMenu(Menu &menu){
    ui->MainMenu->clear();
    ui->MainMenu->addItems(menu.getMenuItems());
    ui->MainMenu->setCurrentRow(menu.getSelectedRow());
    ui->menuLabel->setText(menu.getName());
    currentMenu = menu;
    displayOptions();
}

void MainWindow::displayOptions(){
    ui->battery->setValue(battery);
    ui->powerLevelBar->setValue(power);
    ui->frequencyLabel->setText(QString::number(therapies[therapies.size()-1]->getFrequency()) + " Hz");
    ui->waveformLabel->setText(therapies[therapies.size()-1]->getWaveform());
    ui->countdownLabel->setText(QString::number(therapies[therapies.size()-1]->getTime()) + " mins");
}

void MainWindow::goHome(){
    drawMenu(mainMenu);
}

void MainWindow::okButton(){
    QString menu = currentMenu.getName();
    int selectedRow = currentMenu.getSelectedRow();

    if(menu == MENUS[0]){
        if(      selectedRow == 0){ drawMenu(frequencyMenu); }
        else if (selectedRow == 1){ drawMenu(waveformMenu);}
        else if (selectedRow == 2){ drawMenu(countdownCycleMenu);}
        else if (selectedRow == 3){ drawMenu(therapyMenu);
            timeTraker = therapies[therapies.size()-1]->getTime()*60;
            timer->start(1000);
            updateTimer();
            therapies[therapies.size()-1]->setStart(QDateTime::currentDateTime());
        }  else if (selectedRow == 4){
            QStringList oldTherapy = {"Go Back"};
            foreach (Therapy* therapy,therapies){
                qDebug()<<therapy->getFrequency();
                qDebug()<<therapy->getWaveform();
                qDebug()<<therapy->getStart().date().toString();
                qDebug()<<therapy->getPower();
                qDebug()<<therapy->getTime();
                qDebug()<<"\n";
                oldTherapy<< therapy->getStart().date().toString() + "\n  " + QString::number(therapy->getPower()) + " power\n  " + QString::number(therapy->getTime()) + " mins";
            }
            oldTherapy.takeLast();
            Menu history = Menu(MENUS[5],oldTherapy);
            drawMenu(history);
        }

    } else if (menu == MENUS[1]){
         therapies[therapies.size()-1]->setFrequency(FREQUENCIES[selectedRow]);
         drawMenu(mainMenu);

        qDebug() << "[MainWindow]: Frequency set to " << FREQUENCIES[selectedRow] << "Hz";

    } else if (menu == MENUS[2]){
        therapies[therapies.size()-1]->setWaveform(WAVEFORMS[selectedRow]);
        drawMenu(mainMenu);

        qDebug() << "[MainWindow]: Wave Form set to " << WAVEFORMS[selectedRow];

    } else if (menu == MENUS[3]){
        therapies[therapies.size()-1]->setTime(COUNTDOWN_CYCLES[selectedRow]);
        drawMenu(mainMenu);
      
        qDebug() << "[MainWindow]: Countdown Cycle set to " << COUNTDOWN_CYCLES[selectedRow] << " mins";
      
    } else if (menu == MENUS[4]){
        if(selectedRow==0){
            shutdownTherapy();
            drawMenu(mainMenu);
        }else{
            saveTherapy = true;
        }
    } else if (menu == MENUS[5]){
        if(selectedRow==0){
            drawMenu(mainMenu);
        }else{
            therapies[therapies.size()-1] = therapies[selectedRow-1];
            drawMenu(mainMenu);
        }
    }
}
void MainWindow::shutdownTherapy(){
    ui->timerLabel->setText("");
    timer->stop();
    if(saveTherapy){
        saveTherapy = false;
        therapies.append(new Therapy());
    }
}


void MainWindow::forceBattery(double target){
    updateBattery(target-battery);
}

void MainWindow::updateBattery(float change){
    battery += change;
    if(battery<0)battery=0;
    if(battery>100)battery=100;

    ui->battery->setValue(battery);
    if(battery<=2){
        changePowerState();
    }else if(battery<=2.2){
        ui->batteryWarning->setText("Shutting down");
    }else if(battery<=5){
        ui->batteryWarning->setText("Low Battery");
    }else{
        ui->batteryWarning->setText("");
    }
}


void MainWindow::updateTimer(){
    if(therapies[therapies.size()-1]->getPower()>14){
        changePowerState();
        disableButtons(true);
        ui->powerButton->setDisabled(true);
    }

    ui->timerLabel->setText(QString::number(timeTraker/60)+":"+QStringLiteral("%1").arg(timeTraker%60,2,10,QLatin1Char('0')));
    if(skinContact) timeTraker -= 1;
    if(timeTraker<=0){shutdownTherapy();}

    updateBattery(-0.007);
}
