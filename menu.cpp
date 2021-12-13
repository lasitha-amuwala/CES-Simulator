#include "menu.h"

Menu::Menu(QString n, QStringList mi, int r) : name(n), menuItems(mi), selectedRow(r)
{}

Menu::~Menu(){}

QStringList Menu::getMenuItems(){return menuItems; }  // returns menuItmes
QString Menu::getName(){return name; } // returns name of menu
int Menu::getSelectedRow(){return selectedRow ;} // returns currently selected item index of menu
void Menu::setSelectedRow(int row){ selectedRow = row; } // sets selected item index of menu
