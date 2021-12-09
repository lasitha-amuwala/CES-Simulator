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
    QVector<Therapy*> therapies;
    QTimer* timer;
    int timeTraker;
    float battery;
    int power;

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
};
#endif // MAINWINDOW_H
