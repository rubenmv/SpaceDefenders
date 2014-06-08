#include "Shader.h"
#include <string>
#include <fstream>
#include <iostream>

Shader::Shader(const char* vertexFileName,const char* fragmentFileName)
{
	this->vertex = readFile(vertexFileName);	
	this->fragment = readFile(fragmentFileName);
}


Shader::~Shader(void)
{
}


std::string Shader::readFile(const char* fileName )
{
	std::ifstream file;
	file.open(fileName);
	std::string string;

		if (!file) {
			std::cout << "ERROR:No pudo leerse el archivo\n";
			return string;
		}else{

			std::string line;
			while(getline(file, line))
			{
				string += line + "\n";
			}				
			
			return string;
		}

	return string;
}
