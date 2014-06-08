#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>
#include <math.h>
#include <vector>

/**\class Matrix
 * La clase Matrix define una matriz de clase T
 * Se crean objetos usando Matriz<type>, por ejemplo:
 * Matrix<int>, Matrix<float>, Matrix<double>, Matriz<Rational>
 * Solo tiene constructor con parametros (n,m) filas y columnas
 * y crea una matriz llena de 0, se rellena acediendo a las posiciones
 * e insertando elementos, o con la funcion fill. Se puede acceder de
 * dos formas, la forma directa variablematriz.matrix[0]0] para la posicion (1,1)
 * o mediante el operador(), con sus indices reales variablematriz(1,1) para la posicion (1,1)*/
template<class T>
class Matrix {

	/**\brief Sobrecarga del operador salida.
	 * \param os - ostream de salida.
	 * \param mat - matriz para mostrar.
	 * \return Salida de la matriz.*/
	template<class U>
	friend std::ostream& operator<<(std::ostream &os, const Matrix<U> &mat);
public:
	//--------- FORMA CANÓNICA ------------//
	Matrix();
	/**\brief Constructor.
	 * \param f - Este numero determinara el numero de filas de la matriz
	 * \param c - Este numero determinara el numero de columnas de la matriz*/
	Matrix(int f, int c);

	/**\brief Destructor*/
	virtual ~Matrix();

	/**\brief Constructor de Copia
	 * \param mat - La matriz a copiar*/
	Matrix(const Matrix<T> &mat);

	/**\brief Devuelve el valor de c.
	 *\return devuelve el valor de c.*/
	int getC(void) const;

	/**\brief Devuelve el valor de f.
	 *\return devuelve el valor de f.*/
	int getF(void) const;


	/**\brief Asigna un dato a una posicion (x,y) de la matriz.
	 *\param n - La posicion de x (columna) de la matriz
	 *\param m - La posicion de y (fila) de la matriz.
	 *\param dato - El dato que vamos a añadir.*/
	void setNM(const int, const int, const T);

	/**\brief Operador de asigancion
	 * \param mat - La matriz a asignar
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> & operator=(const Matrix<T> &mat);

	/**\brief Operador de suma
	 * \param mat - La matriz a sumar
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> operator+(const Matrix<T> &mat) const;

	/**\brief Operador de multiplicación
	 * \param mat - La matriz a multiplicar
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> operator*(const Matrix<T> &mat) const;

	/**\brief Operador de multiplicación
	 * \param rat - El T a multiplicar
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> operator*(const T &rat) const;

	/**\brief Operador de division
	 * \param rat - El T a dividir
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> operator/(const T &rat) const;

	/**\brief Operador de corchetes
	 * \param n - fila a la que queremos acceder
	 * \param m - columna a la que queremos accder
	 * \return *this modificada*/
	T& operator()(const int n, const int m);

	/**\brief Operador de corchetes
	 * \param n - fila a la que queremos acceder
	 * \param m - columna a la que queremos accder
	 * \return tmp Una matriz temporal modificada*/
	T operator()(const int n, const int m) const;

	/**\brief Operador ==, devuelve true si son iguales
	 * \param mat - La matriz a comparara
	 * \return tre si son iguales*/
	bool operator==(const Matrix<T> &mat);

	/**\brief Operador !=, devuelve true si son diferentes
	 * \param mat - La matriz a comparara
	 * \return true si son diferentes*/
	bool operator!=(const Matrix<T> &mat);

	/**\brief Concatena dos matrices
	 * \param mat - La matriz a concatenar
	 * \return tmp una matriz que contiene la concatenacion this&mat */
	Matrix<T> conMatrix(const Matrix<T> &mat) const;

	/**\brief Devuelve una Matriz cambiando su tamaño por nxm
	 * \param n - numero de filas de la nueva matriz
	 * \param m -  numero de columnas de la nueva matriz
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> chaMatrix(int n, int m) const;

	/**\brief Devuelve una Sub-Matriz desde la posicion nixmi hasta nfxmf
	 * \param ni - fila donde empezara la nueva matriz
	 * \param mi - columna donde empezara la nueva matriz
	 * \param nf - fila donde acabara la nueva matriz
	 * \param mf - columna donde acabara la nueva matriz
	 * \return tmp Una matriz temporal modificada*/
	Matrix<T> subMatrix(int, int, int, int) const;

	/**\brief esCuadrada
	 * \return bool - devuelve true si la matriz que lo llama es cuadrada*/
	bool isSquare(void) const;

	/**\brief esIdentidad
	 * \return bool - devuelve true si la matriz que lo llama es la identidad*/
	bool isIdentity(void);
	
	/**\brief Convierte cualquier matriz cuadrada en la Identidad
	 * return this, transformada si era cuadrada.*/
	void setIdentity(void);

	/**\brief Calcula el determinate de una matriz hasta tamaño 3x3
	 * \param mat - La matriz a la que calculamos al inversa
	 * \return det es el determinate de la matriz*/
	T determinant(void) const;

	/**\brief Operador de fila con T, sirve para multiplicar/dividir a una fila un T - n1/rat o n1*rat
	 * \param n1 numero de la fila a la que se le multiplica/divide
	 * \param op operador a realizar * (para multiplicar) o / (para dividir)
	 * \param rat T que multiplica/divide a n1
	 * \return *this devuelve la matriz modificada*/
	Matrix<T>& operatorFR(const int, const char, const T);

	/**\brief Operador de filas y T, es una convinacion de las anteriores
	 * sirve para multiplicar/dividir un T a una fila y despues restar/sumar a otra fila - n1-(n2*rat) o n1+(n2/rat)
	 * \param n1 numero de la fila a la que se le restar/sumar el resultado de n2*rat
	 * \param op operador a realizar * (para multiplicar) o / (para dividir)
	 * \param n2 numero de la fila que suma/resta a n1
	 * \param rat T que multiplica/divide a n1
	 * \return *this devuelve la matriz modificada*/
	Matrix<T>& operatorFFR(const int, const char, const int, const T);

	/**\brief Devuelve true si la columna que le pasamos como argumento es toda de 0's
	 * \param col - numero de columna a comprobar (de 0 a f-1)
	 * \return resultado de si es o no toda de 0*/
	bool colZeros(const int);

	/**\brief Devuelve la fila del donde se encuentra un elemento distinto de 0, que no sea (fil,col)
	 * \param fil - numero de la fila a comproar (de 0 a f-1)
	 * \param col - numero de la columna a comproar (de 0 a c-1)
	 * \return piv, contiene el numero de la fila distinta de 0*/
	int filPivot(const int, const int);

	/**\brief Calcula la inversa de una matriz por Gauss-Jordan
	 * \param mat - La matriz a la que calculamos al inversa*/
	Matrix<T> inverse(void) const;

private:
	/**\brief Es una matriz de elementos Ts*/
	T **matrix;
	/**\brief Es un entero que representa el numero de filas*/
	int f;
	/**\brief Es un entero que representa el numero de columnas*/
	int c;
	/**\brief Es T de error*/
	T error;
};

//Definicion de las funciones.
template<class T>
Matrix<T>::Matrix() {
	int nf = 4;
	int nc = 4;
	if (nf > 0 && nc > 0) {

		matrix = new T*[nf];
		for (int i = 0; i < nf; i++)
			matrix[i] = new T[nc];
		c = nc;
		f = nf;
	} else {
		matrix = NULL;
		c = 0;
		f = 0;
	}

}
template<class T>
Matrix<T>::Matrix(int nf, int nc) {
	if (nf > 0 && nc > 0) {

		matrix = new T*[nf];
		for (int i = 0; i < nf; i++)
			matrix[i] = new T[nc];
		c = nc;
		f = nf;
	} else {
		matrix = NULL;
		c = 0;
		f = 0;
	}
}

template<class T>
Matrix<T>::~Matrix() {
	if(matrix!=NULL)
	{
		for (int y = 0; y < f; y++) {
			delete[] matrix[y];
		}
		delete[] matrix;
		matrix=NULL;
	}
}

template<class T>
Matrix<T>::Matrix(const Matrix & mat) {
	int af = mat.getF();
	int ac = mat.getC();

	matrix = new T*[af];
	for (int i = 0; i < af; i++) {
		matrix[i] = new T[ac];
	}
	c = ac;
	f = af;

	for (int n = 0; n < af; n++)
		for (int m = 0; m < ac; m++) {
			matrix[n][m] = mat.matrix[n][m];
		}
}

template<class T>
int Matrix<T>::getC(void) const {
	return c;
}

template<class T>
int Matrix<T>::getF(void) const {
	return f;
}


template<class T>
void Matrix<T>::setNM(const int n, const int m, const T dato) {
	matrix[n][m] = dato;
}


template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T> & mat) {
	if (this != &mat) {
		int af = mat.getF();
		int ac = mat.getC();

		this->~Matrix();

		matrix = new T*[af];
		for (int i = 0; i < af; i++) {
			matrix[i] = new T[ac];
		}
		c = ac;
		f = af;

		for (int n = 0; n < af; n++)
			for (int m = 0; m < ac; m++) {
				//this->setXY(x, y, mat.getXY(x, y));
				matrix[n][m] = mat.matrix[n][m];
			}
	}
	return *this;
}

template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &mat) const {
	Matrix<T> tmp(f, c);
	if (c == mat.c && f == mat.f) {
		for (int n = 0; n < f; n++)
			for (int m = 0; m < c; m++) {
				tmp.matrix[n][m] = (matrix[n][m] + mat.matrix[n][m]);
			}
	}
	return tmp;
}

template<class T>
Matrix<T> Matrix<T>::operator *(const Matrix<T>& mat) const {
	Matrix<T> tmp(f, mat.c);
	if (c == mat.f) {
		for (int n = 0; n < f; n++)
			for (int m = 0; m < mat.c; m++) {
				tmp.matrix[n][m] = 0;
				for (int z = 0; z < c; z++)
					tmp.matrix[n][m] = tmp.matrix[n][m]
							+ (matrix[n][z] * mat.matrix[z][m]);
			}
	}
	return tmp;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const T& rat) const {
	Matrix<T> tmp(f, c);

	for (int n = 0; n < f; n++)
		for (int m = 0; m < c; m++)
			tmp.matrix[n][m] = (matrix[n][m] * rat);

	return tmp;
}

template<class T>
Matrix<T> Matrix<T>::operator/(const T& rat) const {

	return operator *(rat.inversa());

}

template<class T>
T& Matrix<T>::operator()(const int n, const int m) {
	if (n > 0 && m > 0) {
		if (n <= f && m <= c) {
			return matrix[n - 1][m - 1];
		} else {
			int nf = n, nc = m;
			if (f > n)
				nf = f;
			else
				nf = n;
			if (c > m)
				nc = c;
			else
				nc = m;
			*this = chaMatrix(nf, nc);
			return matrix[n - 1][m - 1];
		}
	} else {
		return error;
	}
}

template<class T>
T Matrix<T>::operator()(const int n, const int m) const {
	if ((n > 0 && n <= f) && (m > 0 && m <= c))
		return matrix[n - 1][m - 1];
	else
		return T();
}

template<class T>
bool Matrix<T>::operator==(const Matrix<T> &mat) {
	bool iguales = false;
	if (f == mat.f && c == mat.c) {
		iguales = true;
		for (int n = 0; n < f && iguales; n++)
			for (int m = 0; m < c && iguales; m++)
				if (matrix[n][m] != mat.matrix[n][m])
					iguales = false;

	}
	return iguales;
}

template<class T>
bool Matrix<T>::operator!=(const Matrix<T> &mat) {
	return (!operator==(mat));
}

template<class U>
std::ostream& operator<<(std::ostream &os, const Matrix<U> &mat) {

	for (int n = 0; n < mat.f; n++) {
		for (int m = 0; m < mat.c; m++) {
			os << mat.matrix[n][m] << "\t";
		}
		os << endl;
	}
	return os;
}

template<class T>
Matrix<T> Matrix<T>::chaMatrix(int n, int m) const {
	Matrix<T> tmp(n, m);

	if (n > f)
		n = f;
	if (m > c)
		m = c;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			tmp.matrix[i][j] = matrix[i][j];

	return tmp;
}

template<class T>
Matrix<T> Matrix<T>::subMatrix(int ni, int mi, int nf, int mf) const {
	if (ni > 0 && mi > 0) {
		if (nf > f)
			nf = f;
		if (mf > c)
			mf = c;
		int n = nf - ni + 1;
		int m = mf - mi + 1;
		Matrix<T> tmp(n, m);

		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++) {
				tmp.matrix[i][j] = matrix[i + ni - 1][j + mi - 1];
			}

		return tmp;
	} else {
		return *this;
	}
}

template<class T>
Matrix<T> Matrix<T>::conMatrix(const Matrix<T> &mat) const {
	if (f == mat.f) {
		Matrix<T> tmp(f, c + mat.c);

		for (int i = 0; i < f; i++) {
			for (int j = 0; j < c; j++)
				tmp.matrix[i][j] = matrix[i][j];
			for (int j2 = 0; j2 < mat.c; j2++)
				tmp.matrix[i][j2 + c] = mat.matrix[i][j2];
		}

		return tmp;
	} else
		return *this;
}

template<class T>
bool Matrix<T>::isSquare(void) const {
	return (c == f);
}

template<class T>
bool Matrix<T>::isIdentity(void) {
	bool ident = false;
	if (f == c) {
		Matrix<T> aux(f, c);
		aux.setIdentity();
		if (*this == aux)
			ident = true;
	}
	return ident;
}



template<class T>
void Matrix<T>::setIdentity(void) {
	if (isSquare()) {
		for (int n = 0; n < f; n++) {
			matrix[n][n] = 1;
			for (int m = 0; m < n; m++) {
				matrix[m][n] = 0;
				matrix[n][m] = 0;
			}
		}
	}
}

template<class T>
Matrix<T>& Matrix<T>::operatorFFR(const int n1, const char op, const int n2,
		const T rat) {
	if (op == '-') {
		for (int m = 0; m < c; m++) //Restamos la fila i y la fila n para conseguir el cero;
				{
			matrix[n1][m] = matrix[n1][m] - (matrix[n2][m] * rat);
		}
	} else if (op == '+') {
		for (int m = 0; m < c; m++) //Restamos la fila i y la fila n para conseguir el cero;
				{
			matrix[n1][m] = matrix[n1][m] + (matrix[n2][m] / rat);
		}
	}
	return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operatorFR(const int n, const char op, const T rat) {
	if (op == '/') {
		for (int m = 0; m < c; m++) //consigue el 1 en (i,i) y aplica a toda slas columas
				{
			matrix[n][m] = matrix[n][m] / rat;
		}
	} else if (op == '*') {
		for (int m = 0; m < c; m++) //consigue el 1 en (i,i) y aplica a toda slas columas
				{
			matrix[n][m] = matrix[n][m] * rat;
		}
	}
	return *this;
}

template<class T>
bool Matrix<T>::colZeros(const int col) {
	bool zeros = true;
	T zero(0);

	if (col < c)
		for (int n = 0; n < f; n++)
			if (matrix[n][col] != zero)
				zeros = false;

	return zeros;
}

template<class T>
int Matrix<T>::filPivot(const int fil, const int col) {
	int piv = 0;
	T zero(0);
	if (fil < f && col < c) {
		for (int n = 0; n < f; n++) {
			if (n != fil)
				if (matrix[n][col] != zero)
					piv = n;
		}
	}
	return piv;
}

template<class T>
Matrix<T> Matrix<T>::inverse(void) const {
	if (isSquare()) {
		Matrix<T> tmp(f, c), ident(f, c), aux(f, c * 2);
		ident.setIdentity();
		tmp = *this;
		tmp = tmp.conMatrix(ident);
		T den(0);
		int a;

		for (int i = 0; i < f; i++) //repeticion pora cada 1
				{
			if (!tmp.colZeros(i)) {
				den = tmp.matrix[i][i];
				if (den != 0)
					tmp.operatorFR(i, '/', den); //consigue el 1 en (i,i) y aplica a toda slas columas
				else {
					a = tmp.filPivot(i, i); //busco cualquier elemento distitno de 0 en la columna i que no sea el elemento (i,i)
					den = tmp.matrix[a][i]; //uso ese elemento (a,1) para poder generar el 1 en el elemento (i,i)

					tmp.operatorFFR(i, '+', a, den);
				}
				for (int n = 0; n < f; n++) //conseigue el 0 en las filas de esa columna
						{
					if (i != n) //Si no soy la fila del 1
							{
						den = tmp.matrix[n][i];
						if (den != 0) //si ya es 0 no hace falta hacer nada
								{
							tmp.operatorFFR(n, '-', i, den);
						}
					}
				}
			}
		}
		if (tmp.subMatrix(1, 1, f, c).isIdentity())
			return tmp.subMatrix(1, c + 1, f, c * 2);
	}
	return *this;
}

#endif /* MATRIX_H_ */
