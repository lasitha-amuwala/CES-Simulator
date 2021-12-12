#include "menu.h"

Menu::Menu(QString n, QStringList mi, int r) : name(n), menuItems(mi), selectedRow(r)
{}

Menu::~Menu(){}

QStringList Menu::getMenuItems(){return menuItems; }
QString Menu::getName(){return name; }
int Menu::getSelectedRow(){return selectedRow ;}
void Menu::setSelectedRow(int row){ selectedRow = row; }
