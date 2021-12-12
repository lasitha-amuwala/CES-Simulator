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
    Ui::MainWindow *ui;  //Main UI Window

    Menu currentMenu;  // Current Selected Menu
    Menu mainMenu;     // Main Menu
    Menu frequencyMenu;   // Frequency Menu
    Menu waveformMenu;    // Wave Form Menu
    Menu countdownCycleMenu; // Count Down Menu
    Menu therapyMenu;  // Therapy Menu

    bool saveTherapy;  //Indicator for saving a therapy
    bool skinContact;   //Indicator used to track skin Contact
    bool powerState;    //Indicator to track Power ON/OFF of the device
    bool resetIdle=false;  //Indicator to track if device is in idle state
    bool resetElectrodes=false; //Indicator to track if electrodes is in idle state
    bool autoshutdown=false; //Indicator to track if auto shutdown is pressed
    QVector<Therapy*> therapies; // List of therapies
    QTimer* timer;   // Main Therapy Timer
    QTimer* idle_timer;  // Checking for device being Idle
    QTimer* electrodes_timer;  // Checking for electrodes being Idle
    QTimer* autoshutdown_timer;  // Testing AutoShutdown for device

    float frequency;
    QString waveform;
    int countdown;

    int timeTraker;     //Time tracker counter
    float battery;       //Battery level
    int current;             // Power of the therapies
    int deviceIdle=1800;  // Checking for device being Idle - 30 minutes
    int electrodesIdle=5;  // Checking for electrodes being Idle - 5 seconds
    int deviceIdletmp;  // Temporary - Checking for device being Idle - 30 minutes
    int electrodesIdletmp;  // Temporary - Checking for electrodes being Idle - 5 seconds
    int autoshutdowntimer=30;  // Testing auto shutdown timer - 30 sec
    int autoshutdowntimertmp;  // Temporary Testing auto shutdown timer

    void drawMenu(Menu&);     //Method to draw the main layout of the Device
    void disableButtons(bool); //Method to disable and enable all the buttons on the device - use mainly when Power ON/OFF
    void displayOptions();    //Method to display all the options
    void updateTimer();       //Method to update the timer when a therapy starts
    void updateBattery(float);  //Method to update battery level when  a therapy starts
    void shutdownTherapy();    //Method to shutdown the Therapy when skin contact is lost

private slots:
    void changePowerState();   //Method to change the Power ON/OFF
    void navigateDown();       //Method to navigate down the menu
    void navigateUp();          //Method to navigate up the menu
    void okButton();            //Method to select a menu option
    void goHome();                 //Method to reset and display the main menu
    void increaseCurrent();    //Method to increase the current
    void decreaseCurrent();    //Method to decrease the current
    void toggleElectrodes();   //Method to toggle ON/OFF the Electrodes
    void forceBattery(double);  //Method to track the Battery Level base on the power of the therapy
    void forceCurrent(int);   //Method to track the Current Level
    void updateIdleCountdown();   //Method to track the countdown of device being idle
    void updateElectrodesIdleCountdown();  //Method to track the countdown of electrodes being idle
    void autoShutdownCountdown();  //Method to track the countdown for auto Shutdown testing
    void autoShutdown_test();  //Method to test auto Shutdown

};
#endif // MAINWINDOW_H
