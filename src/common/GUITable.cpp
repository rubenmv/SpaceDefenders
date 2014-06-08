#include "GUITable.h"

#include "Clock.h"

GUITable::GUITable( float left, float top, int id )
{
	type = Type::TABLE;
	this->id = id;

	this->visible = true;
	this->enabled = true;

	// Valor por defecto: valor maximo de unsigned int
	selectedRow = DEFAULT_ROW_SELECTED;
	// Rectangulo de resaltado en seleccion
	highlight.setFillColor(sf::Color(200, 200, 200, 200));

	this->position = Vector2d(left, top);
	this->size = Vector2d(0, 0);
}

GUITable::~GUITable()
{
	clear();
}

void GUITable::addColumn( const wchar_t* caption )
{
	Column column;
	column.caption = new GUIText();
	column.caption->setString( caption );
	column.index = columns.size();
	column.width = COLUMN_DEFAULT_WIDTH; // Por defecto

	columns.push_back(column);

	// Actualizamos la anchura de la tabla
	updateWidth();
}

void GUITable::setColumnWidth( unsigned int id, int width )
{
	if (id < columns.size())
	{
		columns[id].width = width;
	}

	// Actualizamos la anchura de la tabla
	updateWidth();
}

void GUITable::addRow()
{
	if (!columns.empty())
	{
		Row row;
		row.index = rows.size();
		rows.push_back(row);

		// Agregamos las nuevas celdas
		for (size_t i = 0; i < columns.size(); i++)
		{
			Cell cell;
			cell.rowIndex = row.index;
			cell.columnIndex = i;
			cell.index = cells.size();
			cell.text = new GUIText();

			cells.push_back(cell);
		}
	}
	else
	{
		std::cout << "GUITable. ERROR: Debes agregar alguna columna antes de agregar filas" << std::endl;
	}

	// Actualizamos la altura de la tabla
	this->size.y += ROW_HEIGHT;

	setPosition(this->position.x, this->position.y);
}

// Limpia la tabla por completo, incluidas las columnas
void GUITable::clear()
{
	this->size = Vector2d(0, 0);
	selectedRow = DEFAULT_ROW_SELECTED;
	columns.clear();
	rows.clear();
	// Para las celdas hay que eliminar cada GUIText*
	for (size_t i = 0; i < cells.size(); i++)
	{
		delete cells[i].text;
		cells[i].text = NULL;
	}
	cells.clear();
}

void GUITable::setCellText( unsigned int rowIndex, unsigned int columnIndex, std::wstring text )
{
	// Recorremos las celdas para dibujar su elemento GUIText
	for ( size_t i = 0; i < cells.size(); i++ )
	{
		if ( cells[i].rowIndex == rowIndex && cells[i].columnIndex == columnIndex )
		{
			cells[i].text->setString( text.c_str() );
		}
	}
}

// Cambia la posicion de la tabla y la de todos sus elementos de texto
void GUITable::setPosition( float x, float y )
{
	Clock clock;
	clock.start();

	this->position = Vector2d(x, y);
	Vector2d textPosition; // Texto de cada celda

	// El primer elemento se situa a la izquierda de la tabla
	// los siguientes de la fila van sumando el width de su anterior
	textPosition.x = this->position.x; 

	int columnIndex;

	// Calcular la posicion de cada elemento de texto para cada celda
	// Hemos de suponer que las celdas estan ordenadas por filas de izquierda a derecha
	for ( size_t i = 0; i < cells.size(); i++ )
	{
		columnIndex = cells[i].columnIndex;

		textPosition.y = this->position.y + (cells[i].rowIndex * ROW_HEIGHT);

		cells[i].text->setPosition(textPosition.x, textPosition.y);

		// Si estamos en la ultima celda de la fila, reseteamos la x
		if ( columnIndex == columns.size() - 1 )
		{
			textPosition.x = this->position.x;
		}// En caso contrario sumamos el width de esta celda para la siguiente
		else
		{
			textPosition.x += columns[columnIndex].width;
		}
	}

	// Titulos de las columnas encima de la tabla
	textPosition.x = this->position.x;
	textPosition.y = this->position.y - ROW_HEIGHT;
	for (size_t i = 0; i < columns.size(); i++)
	{
		columns[i].caption->setPosition(textPosition.x, textPosition.y);
		textPosition.x += columns[i].width;
	}
}

// Actualiza la anchura de la tabla
// la altura se hace directamente al agregar filas
void GUITable::updateWidth()
{
	int width = 0;
	for (size_t i = 0; i < columns.size(); i++)
	{
		width += columns[i].width;
	}

	this->size.x = width;

	// Actualiza la posicion de todos los elementos de la tabla
	setPosition(this->position.x, this->position.y);
}

// Calcula la fila seleccionada a partir de la posicion dada en pantalla
void GUITable::setSelected( float yPosition )
{
	if (yPosition > this->position.y && yPosition < this->position.y + this->size.y)
	{
		// Para referencia desde posicion 0
		yPosition -= this->position.y;
		selectedRow = Math::floor(yPosition / ROW_HEIGHT); // Resta 1 porque empezamos en 0
	}
}

void GUITable::draw( sf::RenderWindow* window )
{
	// Primero dibujar el rectangulo de fila seleccionada
	if (selectedRow != DEFAULT_ROW_SELECTED)
	{
		highlight.setPosition(this->position.x, this->position.y + (selectedRow * ROW_HEIGHT));
		// Largo de la tabla y alto de altura de filas
		sf::Vector2f hSize(this->size.x, ROW_HEIGHT);
		highlight.setSize(hSize); // SFML pide este tipo de vector para el size pero no para el position O_o
		window->draw(highlight);
	}

	// Titulo de cada columna
	for (size_t i = 0; i < columns.size(); i++)
	{
		columns[i].caption->draw(window);
	}

	// Recorremos las celdas para dibujar su elemento GUIText
	for ( size_t i = 0; i < cells.size(); i++ )
	{
		cells[i].text->draw(window);
	}
}
