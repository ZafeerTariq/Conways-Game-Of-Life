#include <iostream>
#include <SFML/Graphics.hpp>

class Grid {
private:
	class Cell;
	int rows;
	int cols;
	int cellLength;
	int ticks;
	Cell** cells;

public:
	Grid( int r, int c );
	void draw( sf::RenderWindow& window );
	void update();
	void turnCellOn(int x, int y);
	void resetTicks();
	void clearGrid();
	~Grid();
};

class Grid::Cell {
	private:
		int x;
		int y;
		int width;
		int height;

	public:
		sf::RectangleShape cell;
		bool isOn;
		Cell();
		Cell( int x, int y, int w, int h );
};

Grid::Grid( int r, int c ) {
	this->rows = r;
	this->cols = c;
	this->ticks = 0;
	this->cellLength = 25;

	cells = new Cell* [rows];
	for ( size_t i = 0; i < rows; i++ ) {
		cells[i] = new Cell[cols];
	}

	// start with Gosper glider gun
	cells[10][3].isOn = true;
	cells[11][3].isOn = true;
	cells[10][4].isOn = true;
	cells[11][4].isOn = true;
	cells[10][13].isOn = true;
	cells[11][13].isOn = true;
	cells[12][13].isOn = true;
	cells[9][14].isOn = true;
	cells[13][14].isOn = true;
	cells[8][15].isOn = true;
	cells[8][16].isOn = true;
	cells[14][15].isOn = true;
	cells[14][16].isOn = true;
	cells[11][17].isOn = true;
	cells[9][18].isOn = true;
	cells[13][18].isOn = true;
	cells[10][19].isOn = true;
	cells[11][19].isOn = true;
	cells[12][19].isOn = true;
	cells[11][20].isOn = true;
	cells[8][23].isOn = true;
	cells[9][23].isOn = true;
	cells[10][23].isOn = true;
	cells[8][24].isOn = true;
	cells[9][24].isOn = true;
	cells[10][24].isOn = true;
	cells[7][25].isOn = true;
	cells[11][25].isOn = true;
	cells[6][27].isOn = true;
	cells[7][27].isOn = true;
	cells[11][27].isOn = true;
	cells[12][27].isOn = true;
	cells[8][37].isOn = true;
	cells[9][37].isOn = true;
	cells[8][38].isOn = true;
	cells[9][38].isOn = true;

	for ( size_t i = 0; i < rows; i++ ) {
		for ( size_t j = 0; j < cols; j++ ) {
			cells[i][j].cell.setSize( sf::Vector2f( cellLength, cellLength ) );
			cells[i][j].cell.setPosition( sf::Vector2f( 10 + ( ( cellLength + 5 ) * j ), 10 + ( ( cellLength + 5 ) * i ) ) );

			if ( cells[i][j].isOn )
				cells[i][j].cell.setFillColor( sf::Color( 0, 255, 0, 255 ) );
		}
	}
}

void Grid::draw( sf::RenderWindow& window ) {
	for ( size_t i = 0; i < rows; i++ ) {
		for ( size_t j = 0; j < cols; j++ ) {
			window.draw( cells[i][j].cell );
		}
	}
}

void Grid::update() {
	std::cout << "updating tick number: " << ticks << "..." << std::endl;

	std::vector<sf::Vector2<int>> nextOn;
	std::vector<sf::Vector2<int>> nextOff;

	for ( int i = 0; i < rows; i++ ) {
		for ( int j = 0; j < cols; j++ ) {
			int neighbors = 0;

			for ( int k = i - 1; k < i + 2; k++ ) {
				for ( int l = j - 1; l < j + 2; l++ ) {
					if ( k < 0 || l < 0 || k >= rows || l >= cols || ( k == i && l == j )) {
						continue;
					}
					neighbors += cells[k][l].isOn ? 1 : 0;
				}
			}

			if ( neighbors < 2 )
				nextOff.push_back( sf::Vector2<int>( i, j ) );
			else if ( cells[i][j].isOn && ( neighbors == 2 || neighbors == 3 ) )
				nextOn.push_back( sf::Vector2<int>( i, j ) );
			else if ( cells[i][j].isOn && neighbors > 3 )
				nextOff.push_back( sf::Vector2<int>( i, j ) );
			else if ( !cells[i][j].isOn && neighbors == 3 )
				nextOn.push_back( sf::Vector2<int>( i, j ) );
		}
	}

	for ( size_t i = 0; i < nextOn.size(); i++ ) {
		cells[nextOn[i].x][nextOn[i].y].isOn = true;
	}
	for ( size_t i = 0; i < nextOff.size(); i++ ) {
		cells[nextOff[i].x][nextOff[i].y].isOn = false;
	}

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			if ( cells[i][j].isOn )
				cells[i][j].cell.setFillColor( sf::Color( 0, 255, 0, 255 ) );
			else
				cells[i][j].cell.setFillColor( sf::Color( 255, 255, 255, 255 ) );
		}
	}

	std::cout << "tick number: " << ticks << " updated\n";
	ticks++;
}

void Grid::turnCellOn(int x, int y) {
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			sf::Vector2f position =  cells[i][j].cell.getPosition();
			if ( position.x + cellLength >= x && position.x <= x )
				if ( position.y + cellLength >= y && position.y <= y ) {
					cells[i][j].isOn = !cells[i][j].isOn;
					if ( cells[i][j].isOn )
						cells[i][j].cell.setFillColor( sf::Color( 0, 255, 0, 255 ) );
					else
						cells[i][j].cell.setFillColor( sf::Color( 255, 255, 255, 255 ) );
					break;
				}
		}
	}
}

void Grid::resetTicks() {
	ticks = 0;
}

void Grid::clearGrid() {
	for ( size_t i = 0; i < rows; i++ ) {
		for ( size_t j = 0; j < cols; j++ ) {
			cells[i][j].isOn = false;
			cells[i][j].cell.setFillColor( sf::Color( 255, 255, 255, 255 ) );
		}
	}
}

Grid::~Grid() {
	for ( size_t i = 0; i < rows; i++ ) {
		delete[] cells[i];
	}
	delete[] cells;
}

Grid::Cell::Cell() {
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

Grid::Cell::Cell( int x, int y, int w, int h ) {
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
	this->isOn = false;

	cell.setSize( sf::Vector2f( width, height ) );
	cell.setPosition( sf::Vector2f( x, y ) );
}