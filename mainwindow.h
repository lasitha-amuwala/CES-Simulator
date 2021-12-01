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
    bool powerState;
    void drawMenu(Menu&);
    Menu currentMenu;

private slots:
    void changePowerState();
    void navigateDown();
    void navigateUp();
    void okButton();
};
#endif // MAINWINDOW_H
