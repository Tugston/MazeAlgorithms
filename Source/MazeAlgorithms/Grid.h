#ifndef GRID_H
#define GRID_H

#include"Cell.h"
#include<queue>
#include<set>
#include<Windows.h>
#include<iostream>

class grid {
public:
	grid() {
		//makes the program automatically terminate if no end point is provided
		EndPoint = std::make_pair(0, 0);
	};

	//sets up the starting position and the blank cells
	void SetupGrid(sf::RenderWindow& window, int amountX, int amountY) {
		float width = window.getSize().x;
		float height = window.getSize().y;

		spots.resize(amountX, std::vector<cell>(amountY));

		size = amountX;

		//sets the first cell to visited to indicate the starting position
		SetVisited(std::make_pair(0, 0));

		//default first value to 1
		spots[0][0].SetValue(1);

		//maps out grid
		for (int i = 0; i < amountX; i++)
		{
			for (int y = 0; y < amountY; y++) {

				spots[i][y].Update(window.getSize().x / amountX, window.getSize().y / amountY);


				if (y > 0) {
					spots[i][y].SetPosition(spots[i][y - 1].GetPosition().x + spots[i][y - 1].GetXLength(), 0);
				}

				if (i > 0) {
					spots[i][y].SetPosition(spots[i - 1][y].GetPosition().x, spots[i - 1][y].GetPosition().y + spots[i - 1][y].GetYLength());
				}

				window.draw(spots[i][y].GetShape());

			}
		}

		//spots[5][5].SetWall();

		window.display();
	}

	//performs a bfs search through the entirity of the cells
	void BFS(sf::RenderWindow& window) {
		AddChildren(std::make_pair(0, 0));

		while (visited.count(EndPoint) < 1)
		{
			if (ValidLocation(children.front())) {
				SetVisited(children.front());
				AddChildren(children.front());

				Draw(window);
				//std::cout << children.front().first << "  " << children.front().second << std::endl;

				FindParent();

				children.pop();
			}
			else {
				//invalid position
				children.pop();
			}

			Sleep(10);
		}


		ClearChildren();
		visited.clear();
		BackTrack(window);
	}

	//BackTracking Algorithm
	void BackTrack(sf::RenderWindow& window) {
		std::pair<int, int> CurrentLocation = EndPoint;
		std::pair<int, int> SmallestPosition = EndPoint;


		while (CurrentLocation != std::make_pair(0, 0))
		{
			AddChildren(CurrentLocation);

			//there is always only 4 children
			for (int i = 0; i < 4; i++)
			{
				if (ValidLocation(children.front()))
				{
					if (spots[children.front().first][children.front().second].GetValue() <=
						spots[SmallestPosition.first][SmallestPosition.second].GetValue() &&
						spots[children.front().first][children.front().second].GetValue() != 0) {
						SmallestPosition = children.front();
					}
					visited.emplace(children.front());
				}
				children.pop();
			}


			spots[CurrentLocation.first][CurrentLocation.second].SetEnd();
			CurrentLocation = SmallestPosition;
			Draw(window);

			//PrintSpots();
			//std::cout << "\n\n";
		}

		//the final one doesn't get set in the algorithm due to the loop condition
		spots[0][0].SetEnd();
	}

	//sets the location to visited
	void SetVisited(std::pair<int, int> location) {
		spots[location.first][location.second].SetVisited(true);
		spots[location.first][location.second].UpdateVisisted();
		visited.emplace(std::make_pair(location.first, location.second));
	}

	//draws the spots to the window and displays
	void Draw(sf::RenderWindow& window) {
		for (int i = 0; i < size; i++) {
			for (int y = 0; y < size; y++) {
				window.draw(spots[i][y].GetShape());
			}
		}

		window.display();
		window.clear();
	}

	//adds all the cells children
	void AddChildren(std::pair<int, int> ParentPosition) {
		children.push(std::make_pair(ParentPosition.first - 1, ParentPosition.second));
		children.push(std::make_pair(ParentPosition.first, ParentPosition.second - 1));
		children.push(std::make_pair(ParentPosition.first + 1, ParentPosition.second));
		children.push(std::make_pair(ParentPosition.first, ParentPosition.second + 1));
	}

	//check if the current cell child is valid
	bool ValidLocation(std::pair<int, int> location) {
		//check visited
		if (CheckVisisted(location) == 0) {
			//check bounds
			if (location.first >= 0 && location.first < size && location.second >= 0 && location.second < size) {
				if (spots[location.first][location.second].IsWall() == false) {
					return true;
				}
			}
		}

		return false;
	}

	//returns true if the location has been visited
	bool CheckVisisted(std::pair<int, int> location) {
		return visited.count(location) == 1;
	}

	//sets the walls
	void SetWalls(sf::RenderWindow& window, std::pair<int, int> location) {
		spots[location.first][location.second].SetWall();
		Draw(window);
	}

	//sets the end
	void SetEndPointValue(sf::RenderWindow& window, std::pair<int, int> location) {
		spots[location.first][location.second].SetEnd();
		Draw(window);
	}


	//sets the walls when clicking
	void SelectCell(sf::RenderWindow& window) {
		for (int i = 0; i < size; i++)
		{
			for (int y = 0; y < size; y++) {
				if (spots[i][y].GetShape().getGlobalBounds().contains(window.mapPixelToCoords
				(sf::Mouse::getPosition(window))))
				{
					SetWalls(window, std::make_pair(i, y));
				}
			}
		}
	}

	//sets the end point to find when space is pressed
	void SetEndPoint(sf::RenderWindow& window) {
		for (int i = 0; i < size; i++)
		{
			for (int y = 0; y < size; y++) {
				if (spots[i][y].GetShape().getGlobalBounds().contains(window.mapPixelToCoords
				(sf::Mouse::getPosition(window))))
				{
					EndPoint = std::make_pair(i, y);
					SetEndPointValue(window, std::make_pair(i, y));
				}
			}
		}
	}

	//finds the parent for backtracking
	void FindParent() {
		if (children.front().first - 1 >= 0 && spots[children.front().first - 1]
			[children.front().second].GetValue() != -1 && spots[children.front().first - 1]
			[children.front().second].GetValue() != 0) {
			spots[children.front().first][children.front().second].SetValue(
				spots[children.front().first - 1][children.front().second].GetValue() + 1);
		}
		else if (children.front().second - 1 >= 0 && spots[children.front().first]
			[children.front().second - 1].GetValue() != -1 && spots[children.front().first]
			[children.front().second - 1].GetValue() != 0) {
			spots[children.front().first][children.front().second].SetValue(
				spots[children.front().first][children.front().second - 1].GetValue() + 1);
		}
		else if (children.front().first + 1 <= size && spots[children.front().first + 1]
			[children.front().second].GetValue() != -1 && spots[children.front().first + 1]
			[children.front().second].GetValue() != 0) {
			spots[children.front().first][children.front().second].SetValue(
				spots[children.front().first + 1][children.front().second].GetValue() + 1);
		}
		else if (children.front().second + 1 <= size && spots[children.front().first]
			[children.front().second + 1].GetValue() != -1 && spots[children.front().first]
			[children.front().second + 1].GetValue() != 0) {
			spots[children.front().first][children.front().second].SetValue(
				spots[children.front().first][children.front().second + 1].GetValue() + 1);
		}
	}

	//clears all the children
	void ClearChildren() {
		int size = children.size();
		for (int i = 0; i < size; i++)
		{
			children.pop();
		}
	}

	//testing functions
	void PrintSpots() {
		for (int i = 0; i < size; i++)
		{
			for (int y = 0; y < size; y++) {
				std::cout << spots[i][y].GetValue();
			}

			std::cout << "\n";
		}
	}

private:
	std::vector<std::vector<cell>> spots;
	std::set<std::pair<int, int>> visited;
	std::queue<std::pair<int, int>> children;
	std::pair<int, int> EndPoint;
	int size;
};



#endif //GRID_H
