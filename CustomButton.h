#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class CustomButton : public QPushButton {
    Q_OBJECT

public:
    explicit CustomButton(QWidget *parent = nullptr);   // constructor

signals:
    void leftClicked();   // emits signal when left click is sensed
    void rightClicked();  // emits signal when right click sensed

protected:
    void mousePressEvent(QMouseEvent *event) override;   // method to be overriden to differentiate right and left click
};

#endif // CUSTOMBUTTON_H
