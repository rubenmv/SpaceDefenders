#pragma once

#include "GUIElement.h"
#include "GUIText.h"

#include <vector>

class GUITable : public GUIElement
{
public:
	static const unsigned int DEFAULT_ROW_SELECTED = 65535;
	static const unsigned int ROW_HEIGHT = 20;
	static const unsigned int COLUMN_DEFAULT_WIDTH = 50;

	struct Cell
	{
		int index;
		unsigned int rowIndex;
		unsigned int columnIndex;
		GUIText* text;
	};

	struct Row
	{
		int index;
	};

	struct Column
	{
		int index;
		int width;
		GUIText* caption;
	};

	GUITable( float left, float top, int id );
	virtual~GUITable();

	unsigned int getRowCount()
	{
		return rows.size();
	}

	unsigned int getColumnCount()
	{
		return columns.size();
	}

	//! Adds a column
	/** If columnIndex is outside the current range, do push new colum at the end */
	void addColumn(const wchar_t* caption);

	void setColumnWidth(unsigned int id, int width);

	// Agrega una fila
	void addRow();

	// Devuelve la fila seleccionada
	unsigned int getSelected()
	{
		return selectedRow;
	}

	// Calcula la fila seleccionada a partir de la posicion dada en pantalla
	void setSelected(float yPosition);

	// Elimina todo el contenido
	void clear();

	//! Set the text of a cell
	void setCellText(unsigned int rowIndex, unsigned int columnIndex, std::wstring text);

	// Posiciona la tabla junto a todas sus celdas
	void setPosition(float x, float y);

	// Dibuja todos los elementos GUIText de las celdas
	void draw(sf::RenderWindow* window);

private:
	std::vector< Column > columns;
	std::vector< Row > rows;
	std::vector< Cell > cells;
	std::vector< Cell > titles;

	unsigned int selectedRow;

	// Rectangulo que 
	sf::RectangleShape highlight;

	// Actualiza la anchura de la tabla
	void updateWidth();
};