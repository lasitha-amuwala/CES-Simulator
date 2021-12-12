#ifndef MENU_H
#define MENU_H

#include "QString"
#include "QStringList"
class Menu
{
public:
    Menu(QString="", QStringList={}, int=0);
    ~Menu();

    QStringList getMenuItems();
    QString getName();
    int getSelectedRow();

    void setSelectedRow(int);

private:
    QString name;
    QStringList menuItems;
    int selectedRow;
};

#endif // MENU_H
