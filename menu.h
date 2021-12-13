#ifndef MENU_H
#define MENU_H

#include "QString"
#include "QStringList"

// Menu is a QStringlist of menu items

// name: the name of the menu
// menuItems: a list of the items in the menu
// selectedRow: the currently selected item index of the menu

// getMenuItems(); returns a QStringList of the menu's items
// getName(); returns the name of the menu
// getSelectedRow(); returns the currently selected item index for the menu
// setSelectedRow(int); takes an int as input and sets the selectedRow

class Menu
{

public:
    Menu(QString="", QStringList={}, int=0);
    ~Menu();

    QStringList getMenuItems(); //
    QString getName();
    int getSelectedRow();
    void setSelectedRow(int);

private:
    QString name;
    QStringList menuItems;
    int selectedRow;
};

#endif // MENU_H
