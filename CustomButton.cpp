#include "CustomButton.h"
#include <QMessageBox>

CustomButton::CustomButton(QWidget *parent) : QPushButton(parent) {
}


// overrides mousePressEvent to sense both the right click and the left click
void CustomButton::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit leftClicked();
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked();
    }
    QPushButton::mousePressEvent(event);
}
