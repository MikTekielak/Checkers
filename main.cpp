#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Checkers");
    int squareSize = 100;
    int pawnSize = 80;

    sf::RectangleShape squares[8][8];
    sf::CircleShape pawns[24];
    int board[8][8];
    int pawnOwners[24];
    int pawnRows[24];
    int pawnCols[24];
    bool pawnCaptured[24];

    int pawnIndex = 0;
    int selectedPawnIndex = -1;
    int turn = 1; // 1 for black's turn, 2 for white's turn

    // Initialize the board
    for (int row = 0; row < 8; ++row)
        for (int col = 0; col < 8; ++col)
            board[row][col] = -1;

    // Create the board and pawns
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            squares[row][col].setSize(sf::Vector2f(squareSize, squareSize));
            squares[row][col].setPosition(col * squareSize, row * squareSize);
            if ((row + col) % 2 == 0)
                squares[row][col].setFillColor(sf::Color::White);
            else
                squares[row][col].setFillColor(sf::Color::Red);

            // Place black pawns
            if (row < 3 && (row + col) % 2 != 0) {
                pawnOwners[pawnIndex] = 1;
                pawnRows[pawnIndex] = row;
                pawnCols[pawnIndex] = col;
                pawnCaptured[pawnIndex] = false;

                pawns[pawnIndex].setRadius(pawnSize / 2);
                pawns[pawnIndex].setFillColor(sf::Color::Black);
                pawns[pawnIndex].setPosition(col * squareSize + squareSize / 2 - pawnSize / 2,
                                             row * squareSize + squareSize / 2 - pawnSize / 2);

                board[row][col] = pawnIndex;
                pawnIndex++;
            }
            // Place white pawns
            else if (row > 4 && (row + col) % 2 != 0) {
                pawnOwners[pawnIndex] = 2;
                pawnRows[pawnIndex] = row;
                pawnCols[pawnIndex] = col;
                pawnCaptured[pawnIndex] = false;

                pawns[pawnIndex].setRadius(pawnSize / 2);
                pawns[pawnIndex].setFillColor(sf::Color::White);
                pawns[pawnIndex].setPosition(col * squareSize + squareSize / 2 - pawnSize / 2,
                                             row * squareSize + squareSize / 2 - pawnSize / 2);

                board[row][col] = pawnIndex;
                pawnIndex++;
            }
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle mouse clicks
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;
                int col = mouseX / squareSize;
                int row = mouseY / squareSize;

                if (col >= 0 && col < 8 && row >= 0 && row < 8) {
                    if (selectedPawnIndex == -1) {
                        // Select a pawn
                        if (board[row][col] != -1) {
                            int pawnIdx = board[row][col];
                            if (pawnOwners[pawnIdx] == turn && !pawnCaptured[pawnIdx]) {
                                selectedPawnIndex = pawnIdx;
                            }
                        }
                    } else {
                        // Move the selected pawn
                        int currRow = pawnRows[selectedPawnIndex];
                        int currCol = pawnCols[selectedPawnIndex];
                        int owner = pawnOwners[selectedPawnIndex];

                        // Check if destination is empty
                        if (board[row][col] == -1) {
                            int rowDiff = row - currRow;
                            int colDiff = col - currCol;

                            int dir = (owner == 1) ? 1 : -1; // Direction based on owner

                            // Normal move
                            if (rowDiff == dir && abs(colDiff) == 1 ) {
                                board[currRow][currCol] = -1;
                                board[row][col] = selectedPawnIndex;
                                pawnRows[selectedPawnIndex] = row;
                                pawnCols[selectedPawnIndex] = col;

                                pawns[selectedPawnIndex].setPosition(col * squareSize + squareSize / 2 - pawnSize / 2,
                                                                     row * squareSize + squareSize / 2 - pawnSize / 2);

                                selectedPawnIndex = -1;
                                turn = (turn == 1) ? 2 : 1; // Switch turns
                            }
                            // Capture move
                            else if (rowDiff == 2 * dir && abs(colDiff) == 2) {
                                int midRow = currRow + dir;
                                int midCol = currCol + colDiff / 2;

                                if (board[midRow][midCol] != -1 && pawnOwners[board[midRow][midCol]] != owner) {
                                    // Capture opponent's pawn
                                    int capturedPawnIndex = board[midRow][midCol];
                                    board[midRow][midCol] = -1;
                                    pawnCaptured[capturedPawnIndex] = true;
                                    pawns[capturedPawnIndex].setPosition(-100, -100); // Move off-screen

                                    board[currRow][currCol] = -1;
                                    board[row][col] = selectedPawnIndex;
                                    pawnRows[selectedPawnIndex] = row;
                                    pawnCols[selectedPawnIndex] = col;

                                    pawns[selectedPawnIndex].setPosition(col * squareSize + squareSize / 2 - pawnSize / 2,
                                                                         row * squareSize + squareSize / 2 - pawnSize / 2);

                                    selectedPawnIndex = -1;
                                    turn = (turn == 1) ? 2 : 1; // Switch turns
                                } else {
                                    selectedPawnIndex = -1; // Invalid move
                                }
                            } else {
                                selectedPawnIndex = -1; // Invalid move
                            }
                        } else {
                            selectedPawnIndex = -1; // Cannot move to occupied square
                        }
                    }
                }
            }
        }

        window.clear();

        // Draw the board
        for (int row = 0; row < 8; ++row)
            for (int col = 0; col < 8; ++col)
                window.draw(squares[row][col]);

        // Draw the pawns
        for (int i = 0; i < 24; ++i) {
            if (!pawnCaptured[i]) {
                if (i == selectedPawnIndex) {
                    // Highlight selected pawn
                    sf::CircleShape highlight(pawnSize / 2 + 5);
                    highlight.setFillColor(sf::Color::Transparent);
                    highlight.setOutlineThickness(3);
                    highlight.setOutlineColor(sf::Color::Yellow);
                    highlight.setPosition(pawns[i].getPosition().x - 5, pawns[i].getPosition().y - 5);
                    window.draw(highlight);
                }
                window.draw(pawns[i]);
            }
        }

        window.display();
    }

    return 0;
}
