#ifndef CELL_H
#define CELL_H

#include<SFML/Graphics.hpp>

class cell {
public:
	cell() {
		visited = false;
		wall = false;
		value = 0;
	}

	//update the cell
	void Update(float sizeX, float sizeY) {
		CellShape.setSize(sf::Vector2f(sizeX, sizeY));

		if (visited == false) {
			CellShape.setFillColor(sf::Color::Black);
		}
		else {
			UpdateVisisted();
		}

		CellShape.setOutlineThickness(1.0f);
		CellShape.setOutlineColor(sf::Color::White);

	}

	//sets wall
	//changes fill color and sets wall value to true
	void SetWall() {
		CellShape.setFillColor(sf::Color::Red);
		wall = true;
		value = -1;
	}

	//sets the fill color to the end color
	void SetEnd() {
		CellShape.setFillColor(sf::Color::Blue);
	}

	//check if this cell is a wall
	bool IsWall() {
		return wall;
	}

	//updates visited color
	void UpdateVisisted() {
		CellShape.setFillColor(sf::Color::Green);
	}

	//updates the cells position
	void SetPosition(float positionX, float positionY) {
		CellShape.setPosition(sf::Vector2f(positionX, positionY));
	}

	//sets the visited bool to true for the update function
	void SetVisited(bool NewVisisted) { visited = NewVisisted; };

	//sets the distance value
	void SetValue(int value) {
		this->value = value;
	}

	//returns the distane value
	int GetValue() { return value; };


	sf::Vector2f GetPosition() { return CellShape.getPosition(); };
	float GetXLength() { return CellShape.getSize().x; };
	float GetYLength() { return CellShape.getSize().y; };
	bool GetVisited() { return visited; };

	//returns the shapes object
	sf::RectangleShape GetShape() { return CellShape; };

private:
	sf::RectangleShape CellShape;

	//these two are for displaying only, no algorithm use
	bool visited;
	bool wall;

	//the value of the actual cell has to be here since they are vectors of cells instead of vectors of ints
	int value;
};





#endif //CELL_H
