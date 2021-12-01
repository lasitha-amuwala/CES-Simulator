#include "menu.h"

Menu::Menu(QString n, QStringList mi, int r) : name(n), menuItems(mi), selectedRow(r)
{
}

Menu::~Menu(){}

QStringList Menu::getMenuItems(){return menuItems; }
int Menu::getSelectedRow(){return selectedRow ;}
QString Menu::getName(){return name; }
void Menu::setSelectedRow(int row){ selectedRow = row; }
