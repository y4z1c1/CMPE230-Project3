#ifndef CELL_H
#define CELL_H

#include <QString>
#include <QIcon>

class Cell {
public:
    Cell();

    void reset();    // resets the cell
    void updateIcon();   // updates the image path of the icon

    bool revealed; // if the cell is revealed or not
    bool mine;  // if the cell contains a mine or not
    bool flag;  // if the cell is flagged or not
    bool hint;  // if the hint button turned the cell into the green or not
    int adjacentMines;   // number of mines that are adjacent to the cell
    QString imagePath;   // path of the image
    QIcon icon;    // icon type
};

#endif // CELL_H
