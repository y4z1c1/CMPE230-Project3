#include "Cell.h"
#include <QPixmap>

Cell::Cell() : revealed(false), mine(false), flag(false), hint(false), adjacentMines(0), imagePath("") {
    updateIcon();
}

// resets the cell as if it is the beginning of the game
void Cell::reset() {
    revealed = false;
    mine = false;
    flag = false;
    hint = false;
    adjacentMines = 0;
    updateIcon();
}


// updates the icon by changing the image path
void Cell::updateIcon() {
    if (revealed) {
        if (mine) {
            imagePath = ":/assets/mine.png";
        } else {
            imagePath = QString(":/assets/%1.png").arg(adjacentMines);
        }
    } else {
        if (flag) {
            imagePath = ":/assets/flag.png";
        } else if (hint) {
            imagePath = ":/assets/hint.png";
        } else {
            imagePath = ":/assets/empty.png";
        }
    }
    icon = QIcon(QPixmap(imagePath).scaled(30, 30, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
}
