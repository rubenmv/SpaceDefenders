#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <vector>

/*
	Crea y elimina procesos haciendo uso del CreateProccess de Windows
*/
class ProcessManager
{
public:
	ProcessManager(void);
	~ProcessManager(void);

	// Lanza un nuevo proceso
	unsigned long createProcess(LPSTR path);

	// Elimina un proceso
	void killProcess(unsigned long id); // unsigned long = DWORD

private:
	PROCESS_INFORMATION pi; //Pointer to PROCESS_INFORMATION structure

	std::vector<PROCESS_INFORMATION> piList; // Lista de punteros a struct PROCESS__INFORMATION
};

