#include "Framework_WriteEnable.h"
#include <cstddef>

namespace WaitEnableWrite
{
	class WriteEnable::WriteEnable* Framework_WriteEnable::ptr_WriteEnable = NULL;

	Framework_WriteEnable::Framework_WriteEnable()
	{
		ptr_WriteEnable = new class WriteEnable::WriteEnable();
		while (ptr_WriteEnable == NULL) { /* wait untill created */ }
		ptr_WriteEnable->Initialise_Control(
			ptr_WriteEnable->Get_GlobalForWriteControl(),
			ptr_WriteEnable->Get_GlobalForWriteControl()->Get_NumCores()
		);
	}

	Framework_WriteEnable::~Framework_WriteEnable()
	{

	}

	WriteEnable* Framework_WriteEnable::Get_WriteEnable()
	{
		return ptr_WriteEnable;
	}

}