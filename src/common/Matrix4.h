#pragma once

#include <iostream>
#include <vector>
#include "Math.h"

// Clase matriz de 4 dimensiones
// La matriz se guarda en un array unidimensional para evitar conversiones a OpenGL
template<class T>
class Matrix4
{
public:
	// 4x4 por defecto
	Matrix4()
	{
		width = 4; // Columnas
		height = 4; // Filas
		size = width * height;

		matrix = new T[size];
		setZeros();
	}

	virtual ~Matrix4()
	{
		delete[] matrix;
		matrix = NULL;
	}

	// Obtiene un valor de la matriz por indice
	inline T operator()(int index)
	{
		return matrix[index];
	}

	// Obtiene un valor de la matriz por fila y columna
	inline T operator()(int row, int col)
	{
		int index = (row * width) + col;
		return matrix[index];
	}

	// Sobrecarga de operadores
	// =
	inline void operator=(Matrix4& right)
	{
		if (this->getSize() == right.getSize())
		{
			for (int i = 0; i < size; i++)
			{
				this->matrix[i] = right(i);
			}
		}
	}

	// *
	inline Matrix4<T>& operator*(Matrix4& right)
	{
		Matrix4<T> temp; // Todo a 0

		if (this->getSize() == right.getSize())
		{
			int index, index2, index3;
			T* mat = right.getArray();

			for (int i = 0; i < height; i++) // Filas (row)
			{
				for (int j = 0; j < width; j++) // Columnas (col)
				{
					for (int z = 0; z < width; z++)
					{
						index = (width * i) + j;
						index2 = (width * i) + z;
						index3 = (width * z) + j;
						temp.setValue(index, temp(index) + (this->matrix[index2] * mat[index3]) );
					}
				}
			}

			*this = temp; // = sobrecargado
		}
		return *this;
	}

	inline void setMatrix(Matrix4<T> matrix)
	{
		T* array = matrix.getArray();
		for (int i = 0; i < matrix.getSize(); i++)
		{
			this->matrix[i] = array[i];
		}
	}	

	// Establece un valor en una posicion row,col
	inline void setValue(int row, int col, T value)
	{
		int index = (width * row) + col;
		matrix[index] = value;
	}

	// Establece un valor en indice
	inline void setValue(int index, T value)
	{
		matrix[index] = value;
	}

	inline T* getArray()
	{
		// Convierte el array a column-order
		/*
		for (int i = 0; i < 4; i+=4)
		{
			matrix[i] = matrix[i*4];
		}
		*/
		int num_celda = 0;
		int index = 0;
		Matrix4<T> temp;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				index = i + j * 4;

				temp.setValue( num_celda, matrix[index] );

				num_celda++;
			}
		}
		*this = temp;
		return matrix;
	}

	inline int getSize()
	{
		return this->size;
	}

	inline int getRows()
	{
		return this->height;
	}

	inline int getCols()
	{
		return this->width;
	}

	// Establece un valor en una posicion row,col
	inline T getValue(int row, int col)
	{
		int index = (width * row) + col;
		return matrix[index];
	}

	// Inicializa a la matriz  a ceros
	inline void setZeros()
	{
		for (int i = 0; i < size; i++)
		{
			matrix[i] = 0;
		}
	}

	// Inicializa a la matriz identidad
	inline void setIdentity()
	{
		int row = 0;
		int col = 0;

		for (int i = 0; i < size; i++)
		{
			col = (int)(i / width); // Columna
			row = i % width; // Fila

			if (col == row)
			{
				matrix[i] = 1;
			}
			else
			{
				matrix[i] = 0;
			}
		}
	}
	
	// Inversa de la matriz
	// Utiliza el mismo metodo que la implementacion MESA de la libreria glu
	// Existe una implementacion de Intel mas compleja pero, supuestamente, mas rapida: ftp://download.intel.com/design/PentiumIII/sml/24504301.pdf
	inline Matrix4<T>& inverse()
	{
		T inv[16], det;

		T* m = matrix;

		inv[ 0] =  m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inv[ 4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inv[ 8] =  m[4] * m[ 9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[ 9];
		inv[12] = -m[4] * m[ 9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[ 9];
		inv[ 1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv[ 5] =  m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv[ 9] = -m[0] * m[ 9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[ 9];
		inv[13] =  m[0] * m[ 9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[ 9];
		inv[ 2] =  m[1] * m[ 6] * m[15] - m[1] * m[ 7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[ 7] - m[13] * m[3] * m[ 6];
		inv[ 6] = -m[0] * m[ 6] * m[15] + m[0] * m[ 7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[ 7] + m[12] * m[3] * m[ 6];
		inv[10] =  m[0] * m[ 5] * m[15] - m[0] * m[ 7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[ 7] - m[12] * m[3] * m[ 5];
		inv[14] = -m[0] * m[ 5] * m[14] + m[0] * m[ 6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[ 6] + m[12] * m[2] * m[ 5];
		inv[ 3] = -m[1] * m[ 6] * m[11] + m[1] * m[ 7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[ 9] * m[2] * m[ 7] + m[ 9] * m[3] * m[ 6];
		inv[ 7] =  m[0] * m[ 6] * m[11] - m[0] * m[ 7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[ 8] * m[2] * m[ 7] - m[ 8] * m[3] * m[ 6];
		inv[11] = -m[0] * m[ 5] * m[11] + m[0] * m[ 7] * m[ 9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[ 9] - m[ 8] * m[1] * m[ 7] + m[ 8] * m[3] * m[ 5];
		inv[15] =  m[0] * m[ 5] * m[10] - m[0] * m[ 6] * m[ 9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[ 9] + m[ 8] * m[1] * m[ 6] - m[ 8] * m[2] * m[ 5];

		det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		// Solo si el determinante no es cero
		if(det != 0)
		{
			det = 1.f / det;

			// Resultado final, matriz por determinante
			for(int i = 0; i < 16; i++)
			{
				matrix[i] = inv[i] * det;
			}
		}

		return *this;		
	}

	// Imprime la matriz por la salida estandar
	inline void print()
	{
		for (int i = 0; i < size; i++)
		{
			if (i % width == 0)
			{
				std::cout << std::endl;
			}
			std::cout << matrix[i] << " ";
		}
		std::cout << std::endl;
	}
private:
	// Todo se guarda en un vector unidimensional
	T* matrix;

	int height; // Filas
	int width; // Columnas
	int size;
};