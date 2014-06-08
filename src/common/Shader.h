#pragma once
#include <string>



class Shader
{
public:
	Shader(const char* vertexFileName,const char* fragmentFileName);
	~Shader(void);

	std::string getVertexFileName()
	{
		return vertex;
	}

	std::string getFragmentFileName()
	{
		return fragment;
	}

	void setVertexFileName(const char* vertexFileName)
	{
		this->vertex = readFile(vertexFileName);	
	}

	void setFragmentFileName(const char* fragmentFileName)
	{
		this->fragment = readFile(fragmentFileName);
	}

private:

	std::string vertex;
	std::string fragment;
	//lee un archivo y lo devuelve como string
	std::string readFile(const char* fileName);
};

