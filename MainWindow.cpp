#include "MainWindow.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <QDebug>

// constructor for the MinesweeperWindow
MinesweeperWindow::MinesweeperWindow(QWidget *parent) : QMainWindow(parent), totalMines(30), rows(16), cols(16), score(0), gameOver(false) {
        auto *widget = new QWidget(this);
        setCentralWidget(widget);

        auto *layout = new QVBoxLayout(widget);
        scoreLabel = new QLabel("Score: 0", this);
        auto *restartButton = new QPushButton("Restart", this);
        auto *hintButton = new QPushButton("Hint", this);

        // adds score label, restart and hint buttons to the layout
        auto *buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(scoreLabel);
        buttonLayout->addWidget(restartButton);
        buttonLayout->addWidget(hintButton);

        layout->addLayout(buttonLayout);

        // sets up the grid layout for the game board
        auto *gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(cols, rows, cols, rows);
        layout->addLayout(gridLayout);
        layout->setAlignment(gridLayout, Qt::AlignCenter);

        widget->setLayout(layout);

        // initializes cells and buttons
        cells.resize(rows * cols);
        buttons.resize(rows * cols);

        // creates and configures each button and cell
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                int index = row * cols + col;
                cells[index] = new Cell();
                buttons[index] = new CustomButton();
                buttons[index]->setFixedSize(30, 30);
                buttons[index]->setIcon(cells[index]->icon);
                buttons[index]->setIconSize(QSize(30, 30));
                buttons[index]->setFlat(true);
                buttons[index]->setStyleSheet("border: none;");
                gridLayout->addWidget(buttons[index], row, col);

                // connects button signals to reveal and flag functions
                connect(buttons[index], &CustomButton::leftClicked, this, [this, index]() {
                    revealCell(index);
                });

                connect(buttons[index], &CustomButton::rightClicked, this, [this, index]() {
                    attachFlag(index);
                });
            }
        }

        connect(restartButton, &QPushButton::clicked, this, &MinesweeperWindow::resetGame);

        connect(hintButton, &QPushButton::clicked, this, &MinesweeperWindow::giveHint);


        distributeMines(); // randomly distributes mines
        calculateAdjacentMines(); // calculates adjacent mines for each cell
        setWindowTitle("Minesweeper");
        resize(400, 450);
}


// provides hints to the player
void MinesweeperWindow::giveHint() {

    if(gameOver){
        return;
    }

    // finds cells that can be safely revealed or marked as mines
    findSafeCells();


    // if all mines are known, assume all remaining cells are safe
    if (knownMines.size() == totalMines) {
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                int index = row * cols + col;
                Cell* currentCell = cells[index];
                if (!currentCell->revealed && !knownMines.contains(index)) {
                    if (!safeCells.contains(index)) {
                        safeCells.append(index);  // assume all remaining cells are safe if mines match known mines
                    }
                }
            }
        }
    }

    // reveals a hint from the safeCells list
    if (!safeCells.isEmpty()) {
        int hintIndex = safeCells.at(0);
        if (cells[hintIndex]->hint) {
            revealCell(hintIndex);  // directly reveals if already hinted (second click)
            return;
        }

        cells[hintIndex]->hint = true;
        cells[hintIndex]->flag = false;
        cells[hintIndex]->updateIcon();
        buttons[hintIndex]->setIcon(cells[hintIndex]->icon);
    } else {
        return;


    }
}


// finds safe cells that can be revealed without risk
void MinesweeperWindow::findSafeCells(){

    bool change = false;

    // identifies cells that can be safely marked as mines
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int index = row * cols + col;
            Cell* currentCell = cells[index];
            if (currentCell->revealed && currentCell->adjacentMines > 0) {
                int unrevealedCount = 0;
                QVector<int> adjacentIndices;

                // checks all adjacent cells
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        int neighborRow = row + i;
                        int neighborCol = col + j;
                        if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < cols) {
                            int neighborIndex = neighborRow * cols + neighborCol;
                            Cell* neighbor = cells[neighborIndex];
                            if (!neighbor->revealed && !safeCells.contains(neighborIndex)) {
                                unrevealedCount++;
                                adjacentIndices.append(neighborIndex);
                            }

                        }
                    }
                }

                // if all unrevealed cells around a revealed cell are mines, mark them as known mines
                if (unrevealedCount  == currentCell->adjacentMines) {
                    for (int adjIndex : adjacentIndices) {
                        if(!knownMines.contains(adjIndex)){
                            knownMines.append(adjIndex);
                            change = true;
                        }
                    }
                }
            }
        }
    }

    // identifies safe cells that can be revealed
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int index = row * cols + col;
            Cell* currentCell = cells[index];
            if (currentCell->revealed && currentCell->adjacentMines > 0) {
                int neighborMines = 0;
                QVector<int> adjacentIndices;

                // checks all adjacent cells
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        int neighborRow = row + i;
                        int neighborCol = col + j;
                        if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < cols) {
                            int neighborIndex = neighborRow * cols + neighborCol;
                            Cell* neighbor = cells[neighborIndex];
                            if (!neighbor->revealed) {
                                if (knownMines.contains(neighborIndex)){
                                    neighborMines++;
                                }
                                else{
                                    adjacentIndices.append(neighborIndex);
                                }
                            }

                        }
                    }
                }

                // if the number of known mines around a revealed cell matches its adjacent mines count, mark rest of the celss as safe
                if (neighborMines == currentCell->adjacentMines) {
                    for (int adjIndex : adjacentIndices) {
                        if(!safeCells.contains(adjIndex)){
                            safeCells.append(adjIndex);
                            change = true;
                        }
                    }
                }
            }
        }
    }


    // find new safe cells recursively
    if(change){
        findSafeCells();
    }


}


// reveals all mines when the game is over
void MinesweeperWindow::revealAllMines() {
    for (int i = 0; i < mines.size(); ++i) {
        mines[i]->revealed = true;
        mines[i]->updateIcon();
        buttons[minesIndexes.at(i)]->setIcon(mines[i]->icon);
    }

}

// resets the game to its initial state
void MinesweeperWindow::resetGame() {
    gameOver = false;

    for (Cell* cell : cells) {
        cell->reset();
    }
    safeCells.clear();
    knownMines.clear();
    minesIndexes.clear();
    mines.clear();

    distributeMines();
    calculateAdjacentMines();
    score = 0;
    updateScore();
    for (int i = 0; i < cells.size(); ++i) {
        buttons[i]->setIcon(cells[i]->icon);
    }
}

// randomly distributes mines to the board
void MinesweeperWindow::distributeMines() {
    int mineCount = totalMines;
    while (mineCount > 0) {
        int index = QRandomGenerator::global()->bounded(rows * cols);
        if (!cells[index]->mine) {
            cells[index]->mine = true;
            mines.append(cells[index]);
            minesIndexes.append(index);
            mineCount--;
        }
    }
}

// calculates the number of adjacent mines for each cell
void MinesweeperWindow::calculateAdjacentMines() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int index = row * cols + col;
            if (!cells[index]->mine) {
                int mineCount = 0;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        int neighborRow = row + i;
                        int neighborCol = col + j;
                        if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < cols) {
                            int neighborIndex = neighborRow * cols + neighborCol;
                            if (cells[neighborIndex]->mine) {
                                mineCount++;
                            }
                        }
                    }
                }
                cells[index]->adjacentMines = mineCount;
                cells[index]->updateIcon();
            }
        }
    }
}

// reveals a cell and updates the game state accordingly
void MinesweeperWindow::revealCell(int index) {
    if(!safeCells.isEmpty()){
        if(safeCells.contains(index)){
            safeCells.removeOne(index);
        }
    }

    if (gameOver || cells[index]->revealed) {
        return;
    }

    if (!cells[index]->revealed) {
        cells[index]->revealed = true;
        cells[index]->updateIcon();
        buttons[index]->setIcon(cells[index]->icon);

        if (cells[index]->mine) {
            revealAllMines();
            QMessageBox::information(this, "Game Over", "You lose!");
            gameOver = true;
            return;
        } else {
            score++;
            updateScore();
            checkWinCondition();


            // recursively reveals adjacent cells if no adjacent mines
            if (cells[index]->adjacentMines == 0) {
                int row = index / cols;
                int col = index % cols;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        int neighborRow = row + i;
                        int neighborCol = col + j;
                        if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < cols) {
                            int neighborIndex = neighborRow * cols + neighborCol;
                            if (!cells[neighborIndex]->revealed) {
                                revealCell(neighborIndex);
                            }
                        }
                    }
                }
            }
        }
    }
}


// attaches or detaches a flag on a cell
void MinesweeperWindow::attachFlag(int index) {


    if (gameOver) {
        return;
    }

    if (!cells[index]->flag) {
        cells[index]->flag = true;
        cells[index]->updateIcon();
        buttons[index]->setIcon(cells[index]->icon);

    }
    else{
        cells[index]->flag = false;
        cells[index]->updateIcon();
        buttons[index]->setIcon(cells[index]->icon);
    }


}

// checks if the player has won the game
void MinesweeperWindow::checkWinCondition() {
    if(!(rows*cols-totalMines == score)){
        return;
    }
    QMessageBox::information(this, "Congratulations", "You win!");
    gameOver = true;
}

// updates the score display
void MinesweeperWindow::updateScore() {
    scoreLabel->setText(QString("Score: %1").arg(score));
}
