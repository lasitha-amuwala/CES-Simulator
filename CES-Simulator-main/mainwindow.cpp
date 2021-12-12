#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "menu.h"
#include <QLCDNumber>

const QStringList MENUS = {"Main Menu", "Frequencies", "Wave Forms", "Countdown Cycles", "Start Therapy", "History"};
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

    frequency = FREQUENCIES[0];
    waveform = WAVEFORMS[0];
    countdown = COUNTDOWN_CYCLES[0];

    battery = 100;
    timer = new QTimer(this);
    current = 1;

    //device being idle timer
    this->idle_timer = new QTimer(this);
    connect(idle_timer, SIGNAL(timeout()), this, SLOT(updateIdleCountdown()));
    idle_timer->setSingleShot(true);

    //electrodes being idle timer
    this->electrodes_timer = new QTimer(this);
    connect(electrodes_timer, SIGNAL(timeout()), this, SLOT(updateElectrodesIdleCountdown()));
    electrodes_timer->setSingleShot(true);

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
    connect(ui->setCurrent, SIGNAL(valueChanged(int)), this, SLOT(forceCurrent(int)));
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);

    displayOptions();
    disableButtons(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//##########################################################################################################################################
//####################                                                                                                  ####################
//####################                                           UI Buttons                                             ####################
//####################                                                                                                  ####################
//##########################################################################################################################################


void MainWindow::navigateUp(){
    int currRow = ui->MainMenu->currentRow();
    int nextRow = (currRow == 0)? ui->MainMenu->count() - 1 : currRow - 1;
    currentMenu.setSelectedRow(nextRow);
    ui->MainMenu->setCurrentRow(nextRow);
    resetIdle = true;
}

void MainWindow::navigateDown(){
    int currRow = ui->MainMenu->currentRow();
    int nextRow = (currRow + 1 == ui->MainMenu->count())? 0 : currRow + 1;
    ui->MainMenu->setCurrentRow(nextRow);
    currentMenu.setSelectedRow(nextRow);
    resetIdle = true;
}

//plus button
void MainWindow::increaseCurrent(){
   if(current == 10) return;
    current += 1;
    therapies[therapies.size()-1]->setCurrent(current);
    displayOptions();
    resetIdle = true;
}

//minus button
void MainWindow::decreaseCurrent(){
    current = (current <= 2)? 1 : current - 2;
    therapies[therapies.size()-1]->setCurrent(current);
    displayOptions();
    resetIdle = true;
}

void MainWindow::disableButtons(bool x){
    ui->upButton->setDisabled(x);
    ui->downButton->setDisabled(x);
    ui->homeButton->setDisabled(x);
    ui->okButton->setDisabled(x);
    ui->minusButton->setDisabled(x);
    ui->plusButton->setDisabled(x);
}

void MainWindow::changePowerState(){
    powerState = !powerState;
    if(powerState){
        if(battery==0){
            powerState = false;
        }else{
            drawMenu(mainMenu);
            disableButtons(false);
            updateIdleCountdown();
        }
    }else{
        disableButtons(true);
        shutdownTherapy();
    }
    ui->displayWidget->setVisible(powerState);
}

void MainWindow::goHome(){
    shutdownTherapy();
    drawMenu(mainMenu);
    resetIdle = true;
}

void MainWindow::okButton(){
    QString menu = currentMenu.getName();
    int selectedRow = currentMenu.getSelectedRow();
    resetIdle = true;
    if(menu == MENUS[0]){
        if(      selectedRow == 0){ drawMenu(frequencyMenu); }
        else if (selectedRow == 1){ drawMenu(waveformMenu);}
        else if (selectedRow == 2){ drawMenu(countdownCycleMenu);}
        else if (selectedRow == 3){ drawMenu(therapyMenu);
            timeTracker = therapies[therapies.size()-1]->getTime()*60;
            timer->start(1000);
            updateTimer();
            therapies[therapies.size()-1]->setStart(QDateTime::currentDateTime());
        }  else if (selectedRow == 4){
            QStringList oldTherapy = {"Go Back"};
            foreach (Therapy* therapy,therapies) {
                oldTherapy << therapy->getStart().date().toString() +
                              "\n - Waveform: " + therapy->getWaveform() +
                              "\n - Countdown: " + QString::number(therapy->getTime()) + " mins" +
                              "\n - Frequency: " + QString::number(therapy->getFrequency()) + " Hz" +
                              "\n - Current: " + QString::number(therapy->getCurrent() * 50) + " uA ";
            }
            oldTherapy.takeLast();
            Menu history = Menu(MENUS[5], oldTherapy);
            drawMenu(history);
        }

    } else if (menu == MENUS[1]){
         frequency = FREQUENCIES[selectedRow];
         therapies[therapies.size()-1]->setFrequency(frequency);
         drawMenu(mainMenu);

        qDebug() << "[MainWindow]: Frequency set to " << FREQUENCIES[selectedRow] << "Hz";

    } else if (menu == MENUS[2]){
        waveform = WAVEFORMS[selectedRow];
        therapies[therapies.size()-1]->setWaveform(waveform);
        drawMenu(mainMenu);

        qDebug() << "[MainWindow]: Wave Form set to " << WAVEFORMS[selectedRow];

    } else if (menu == MENUS[3]){
        countdown = COUNTDOWN_CYCLES[selectedRow];
        therapies[therapies.size()-1]->setTime(countdown);
        drawMenu(mainMenu);

        qDebug() << "[MainWindow]: Countdown Cycle set to " << COUNTDOWN_CYCLES[selectedRow] << " mins";
      
    } else if (menu == MENUS[4]){
        if(selectedRow==0){
            shutdownTherapy();
            ui->homeButton->setDisabled(false);
            drawMenu(mainMenu);
        }else{
            saveTherapy = true;
        }
    } else if (menu == MENUS[5]){
        if(selectedRow==0){
            drawMenu(mainMenu);
        }else{
            therapies[therapies.size()-1] = therapies[selectedRow-1];
            current = therapies[therapies.size()-1]->getCurrent();
            drawMenu(mainMenu);
        }
    }
}

//##########################################################################################################################################
//####################                                                                                                  ####################
//####################                                        Admin Buttons                                             ####################
//####################                                                                                                  ####################
//##########################################################################################################################################

//vitually atach/detach the electrodes
void MainWindow::toggleElectrodes(){
    skinContact = !skinContact;
    ui->electrodes->setChecked(skinContact);
    ui->toggleElectrodes_2->setStyleSheet(skinContact? "background-color:#FFA626;" : "");
    if(!skinContact) {
        updateIdleCountdown();
        updateElectrodesIdleCountdown();
    }
    resetIdle = true;
}

//set the device battery level
void MainWindow::forceBattery(double target){
    battery = target;
    updateBattery(0);
}

//set the deivice current
void MainWindow::forceCurrent(int target){
    if(target <= 500){
        ui->powerLevelBar->setValue(target);
        current = target/50;

    }else if(target > 700 && powerState){
        changePowerState();
        ui->powerButton->setDisabled(true);
        qDebug() << "[MainWindow]: Device Disabled - CURRENT LIMIT EXCEEDED";
    }
}

//##########################################################################################################################################
//####################                                                                                                  ####################
//####################                                       Back end Functions                                         ####################
//####################                                                                                                  ####################
//##########################################################################################################################################

//Draws the selectable menu options
void MainWindow::drawMenu(Menu &menu){
    ui->MainMenu->clear();
    ui->MainMenu->addItems(menu.getMenuItems());
    ui->MainMenu->setCurrentRow(menu.getSelectedRow());
    ui->menuLabel->setText(menu.getName());
    currentMenu = menu;
    displayOptions();
    resetIdle = true;
}

//Shows general inforamtion on the device status
void MainWindow::displayOptions(){
    ui->battery->setValue(battery);
    ui->frequencyLabel->setText(QString::number(frequency) + " Hz");
    ui->waveformLabel->setText(waveform);
    ui->countdownLabel->setText(QString::number(countdown) + " mins");
    ui->powerLevelBar->setValue(current * 50);
    ui->setCurrent->setValue(current * 50);
}

//Cleans up after a therapy ends
void MainWindow::shutdownTherapy(){
    ui->timerLabel->setText("");
    timer->stop();
    timeTracker = 0;
    if(saveTherapy){
        saveTherapy = false;
        therapies.append(new Therapy(waveform, frequency, countdown, current));
    }
}

//##########################################################################################################################################
//####################                                                                                                  ####################
//####################                                       Update Functions                                           ####################
//####################                                                                                                  ####################
//##########################################################################################################################################

//Changes the stored battery value
void MainWindow::updateBattery(float change){
    if(skinContact) battery += change;
    if(battery<0)battery=0;
    if(battery>100)battery=100;

    ui->battery->setValue(battery);
	ui->setBattery->setValue(battery);

    if(battery <= 0){
        if(powerState) changePowerState();
        ui->blackScreen->setText("");
    }else if(battery <= 1){
        if(powerState){
            changePowerState();
            ui->blackScreen->setText("Shutting Down...");
        }
    }else if(battery <= 2){
        ui->blackScreen->setText("");
        ui->batteryWarning->setText("Shutting Down...");
    }else if(battery <= 5) {
        ui->blackScreen->setText("");
        ui->batteryWarning->setText("Low Battery");
         qDebug() << "[MainWindow]: Device Shutdown - Low Battery ";
    }
    else if(change>=0){
        ui->batteryWarning->setText("");
        ui->blackScreen->setText("");
    }
}

//Displays time left on an active therapy
void MainWindow::updateTimer(){
    if(therapies[therapies.size()-1]->getCurrent()>14){
        changePowerState();
        disableButtons(true);
        ui->powerButton->setDisabled(true);
    }

    ui->timerLabel->setText(QString::number(timeTracker/60)+":"+QStringLiteral("%1").arg(timeTracker%60,2,10,QLatin1Char('0')));
    if(skinContact)   timeTracker -= 1;
    if(timeTracker<=0){shutdownTherapy();}

    updateBattery(-0.007);
}

// Method to check Idle Device
void MainWindow::updateIdleCountdown()
{
    updateBattery(0);
    if(resetIdle){
        idle_timer->stop();
        deviceIdletmp=deviceIdle;
        resetIdle = false;
    }
    idle_timer->start(1000);
    ui->deviceIdle->setText(QString::number(deviceIdletmp/60)+":"+QStringLiteral("%1").arg(deviceIdletmp%60,2,10,QLatin1Char('0')));
    if(skinContact) {
        ui->batteryWarning->setText("");
        deviceIdletmp=deviceIdle;
        idle_timer->stop();
    } else {
        deviceIdletmp -= 1;
        if(deviceIdletmp<1){
            ui->batteryWarning->setText("");
            changePowerState();
            deviceIdletmp=deviceIdle;
            idle_timer->stop();
            qDebug() << "[MainWindow]: Device Shutdown - Idle Device ";
        }
        if(deviceIdletmp<=3){
            ui->batteryWarning->setText("Shutting down");
        }else if(deviceIdletmp<=5){
            ui->batteryWarning->setText("Device Idle");
        }
    }

}

// Method to check Idle Electrodes
void MainWindow::updateElectrodesIdleCountdown()
{
    updateBattery(0);
    if(resetElectrodes){
        electrodes_timer->stop();
        electrodesIdle=5;
        resetElectrodes = false;
    }
    electrodes_timer->start(1000);
    if(timeTracker>0 && timeTracker < therapies[therapies.size()-1]->getTime()*60){
        if(skinContact) {
            ui->batteryWarning->setText("");
            electrodesIdle=5;
            electrodes_timer->stop();
        } else {
            electrodesIdle -= 1;
            if(electrodesIdle<=0){
                ui->batteryWarning->setText("");
                shutdownTherapy();
                electrodesIdle=5;
                electrodes_timer->stop();
                drawMenu(mainMenu);
                qDebug() << "[MainWindow]: Therapy Shutdown - Idle Electrodes ";
            }else if(electrodesIdle<=3){
                ui->batteryWarning->setText("Shutting down - Therapy");
            }else if(electrodesIdle<=5){
                ui->batteryWarning->setText("Electrodes Idle");
            }
        }
        ui->electrodesIdle->setText(QString::number(electrodesIdle/60)+":"+QStringLiteral("%1").arg(electrodesIdle%60,2,10,QLatin1Char('0')));
    }
}
