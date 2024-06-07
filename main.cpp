#include <QApplication>
#include "MainWindow.h"

// initializes the game
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MinesweeperWindow window;
    window.show();
    return app.exec();
}
