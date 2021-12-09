#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include "menu.h"
#include "therapy.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Menu currentMenu;
    Menu mainMenu;
    Menu frequencyMenu;
    Menu waveformMenu;
    Menu countdownCycleMenu;
    Menu therapyMenu;

    bool saveTherapy;
    bool skinContact;
    bool powerState;
    bool resetIdeal=false;
    bool resetElectrodes=false;
    QVector<Therapy*> therapies;
    QTimer* timer;
    QTimer* ideal_timer;  // Checking for device being ideal
    QTimer* electrodes_timer;  // Checking for electrodes being ideal

    int timeTraker;
    float battery;
    int power;
    int deviceIdeal=1800;  // Checking for device being ideal - 30 minutes
    int electrodesIdeal=5;  // Checking for electrodes being ideal - 5 seconds
    int deviceIdealtmp;  // Temporary - Checking for device being ideal - 30 minutes
    int electrodesIdealtmp;  // Temporary - Checking for electrodes being ideal - 5 seconds

    void drawMenu(Menu&);
    void disableButtons(bool);
    void displayOptions();
    void updateTimer();
    void updateBattery(float);
    void shutdownTherapy();

private slots:
    void changePowerState();
    void navigateDown();
    void navigateUp();
    void okButton();
    void goHome();
    void increaseCurrent();
    void decreaseCurrent();
    void toggleElectrodes();
    void forceBattery(double);
    void updateIdealCountdown();
    void updateElectrodesIdealCountdown();
};
#endif // MAINWINDOW_H
