#include "ProcessManager.h"
#include <iostream>

ProcessManager::ProcessManager(void)
{
	ZeroMemory( &pi, sizeof(pi) );
}

ProcessManager::~ProcessManager(void)
{
	// Mata los procesos arrancados por la aplicacion
	while (!piList.empty())
	{
		TerminateProcess( piList.back().hProcess , NULL );
		piList.pop_back();
	}
}

// Crea un nuevo proceso a partir del path y devuelve su ID
unsigned long ProcessManager::createProcess(LPSTR path)
{
	STARTUPINFO si;			// Pointer to STARTUPINFO structure

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	
	// Start the child process. 
	if( !CreateProcess(
						NULL,	// No module name (use command line)
						path,	// Command line
						NULL,	// Process handle not inheritable
						NULL,	// Thread handle not inheritable
						FALSE,	// Set handle inheritance to FALSE
						0,		// No creation flags
						NULL,	// Use parent's environment block
						NULL,	// Use parent's starting directory 
						&si,	// Pointer to STARTUPINFO structure
						&pi )	// Pointer to PROCESS_INFORMATION structure
		)
	{
		std::cout << "Error al crear el proceso: " << GetLastError() << std::endl;
	}

	piList.push_back(pi);

	return pi.dwProcessId;
}

// Elimina un proceso por ID
void ProcessManager::killProcess(unsigned long id)
{
	size_t i = 0;

	if (!piList.empty())
	{
		while ( i < piList.size() )
		{
			if ( piList[i].dwProcessId == id )
			{
				break;
			}
			else
			{
				i++;
			}
		}

		TerminateProcess( piList[i].hProcess, NULL );
	}
}