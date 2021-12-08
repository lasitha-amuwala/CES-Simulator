#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "menu.h"

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

    QString waveform;
    float frequency;
    int countdownCycle;
    bool powerState;
    int powerLevel;

    void drawMenu(Menu&);
    void disableButtons(bool);
    void displayOptions();

private slots:
    void changePowerState();
    void navigateDown();
    void navigateUp();
    void okButton();
    void goHome();
};
#endif // MAINWINDOW_H
