#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <cmath>

const int windowWidth = 800;
const int windowHeight = 800;
const int rows = 8;
const int cols = 8;
const int squareSize = windowWidth / cols;
const int pawnSize = squareSize / 2.5f;

struct Pawn {
    sf::CircleShape shape;
    bool isActive = false;
};

void handleWindowResize(sf::RenderWindow& window, sf::Event& event) {
        unsigned int newWidth = event.size.width;
        unsigned int newHeight = event.size.height;
        if (newWidth < windowWidth)
            newWidth = windowWidth;
        if (newHeight < windowHeight)
            newHeight = windowHeight;
        if (newWidth > newHeight)
            newHeight = newWidth;
        if (newWidth < newHeight)
            newHeight = newWidth;
        window.setSize(sf::Vector2u(newWidth, newHeight));
}
void drawBoard(sf::RenderWindow& window, sf::Vector2f boardPos[rows][cols]) {

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
            sf::Vector2f position(col * squareSize, row * squareSize);

            boardPos[row][col] = position;

            square.setPosition(position);

            if ((row + col) % 2 == 0)
                square.setFillColor(sf::Color::White);
            else
                square.setFillColor(sf::Color::Black);
            window.draw(square);
        }
    }
}
void initializePawns(std::vector<Pawn>& pawns,sf::Vector2f boardPos[rows][cols]) {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if ((row < 3) && (row + col) % 2 == 0) {
                Pawn pawn;
                pawn.shape.setRadius(pawnSize);
                pawn.shape.setFillColor(sf::Color::Black);
                sf::Vector2f pos = boardPos[row][col];
                pawn.shape.setPosition(pos.x + squareSize / 2 - pawnSize, pos.y + squareSize / 2 - pawnSize);
                pawns.push_back(pawn);
            } else if ((row >= 5) && (row + col) % 2 != 0) {
                Pawn pawn;
                pawn.shape.setRadius(pawnSize);
                pawn.shape.setFillColor(sf::Color::White);
                sf::Vector2f pos = boardPos[row][col];
                pawn.shape.setPosition(pos.x + squareSize / 2 - pawnSize, pos.y + squareSize / 2 - pawnSize);
                pawns.push_back(pawn);
            }
        }
    }
}

void movePawnToPosition(Pawn* pawn, const sf::Vector2f& targetPos) {
    pawn->shape.setPosition(targetPos.x - pawnSize, targetPos.y - pawnSize);  // Center the pawn in the square
}



int main()
{
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML window");
    sf::Vector2f boardPos[rows][cols];
    std::vector<Pawn> pawns;


    drawBoard(window, boardPos);
    initializePawns(pawns,boardPos);




    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //Resizing window below set size, or into not a square
        if (event.type == sf::Event::Resized)
            handleWindowResize(window, event);
        window.clear(sf::Color::White);
        //Drawing the grid and the pawns
        drawBoard(window,boardPos);
        for (const auto& pawn : pawns) {
            window.draw(pawn.shape);
        }

        window.display();
    }

    return EXIT_SUCCESS;
}