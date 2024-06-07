#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QVector>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Cell.h"
#include "CustomButton.h"

class MinesweeperWindow : public QMainWindow {
    Q_OBJECT

public:
    MinesweeperWindow(QWidget *parent = nullptr);    // constructor

private slots:
    void giveHint();   // gives hint
    void findSafeCells();   // finds the cells with a simple algorithm that are hintable
    void revealAllMines();  // reveals all the mines at the end of the game
    void resetGame();   // restarts the game
    void distributeMines();   // distribute mines randomly to the grid
    void calculateAdjacentMines();   // updates the number of the adjacent mines for each cell
    void revealCell(int index);   // reveals the cell in the given position
    void attachFlag(int index);   // attachs flag to the cell in the given position
    void checkWinCondition();    // checks if the game is won
    void updateScore();    // updates score by the number of revealed safe cells

private:
    int totalMines;   // number of total mines
    QVector<int> minesIndexes;   // list of the indexes of the mines
    QVector<Cell*> cells;    // list containing all the cells
    QVector<CustomButton*> buttons;   // list containing buttons
    QVector<Cell*> mines;   // list containing all the mines
    int rows, cols;   // row and column numbers of the grid
    int score;   // score of the game
    QLabel *scoreLabel;   // score label
    bool gameOver;   // if the game is over or not
    QVector<int> knownMines;   // list of mine indexes that are surely known to be a mine
    QVector<int> safeCells;    // list of cell indexes that are surely known to be safe
};

#endif // MAINWINDOW_H
