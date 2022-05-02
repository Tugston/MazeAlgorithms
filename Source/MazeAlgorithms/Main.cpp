#include <SFML/Graphics.hpp>
#include<string>
#include"Grid.h"

#define Cells 10
#define Speed 1000 * .01
#define WindowSize 500


int main()
{
    sf::RenderWindow window(sf::VideoMode(WindowSize, WindowSize), "Maze");

    grid map;
    map.SetupGrid(window, Cells, Cells);


    srand(time(NULL));

    bool Editing = true;

    std::cout << "**INSTRUCTIONS**\n";
    std::cout << "Left Click: Adds Wall at Mouse Position\n";
    std::cout << "Space: Adds End Point at Mouse Position\n";
    std::cout << "Enter: Start the Simulation\n";
    std::cout << "\n\n";
    std::cout << "NOTE: IF END POINT ISN'T DECLARED IT DEFAULTS (0, 0), SO PROGRAM WON'T RUN\n";
    std::cout << "setting the end point on a wall could lead to undefined behaviour.";




    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event) || Editing)
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
                if (event.key.code == sf::Mouse::Left)
                {
                    map.SelectCell(window);
                }

                if (event.key.code == sf::Keyboard::Space)
                {
                    map.SetEndPoint(window);
                }

                if (event.key.code == sf::Keyboard::Enter) {
                    Editing = false;
                }
            }
        }

        map.BFS(window);
        Sleep(Speed);
    }

    return 0;
}