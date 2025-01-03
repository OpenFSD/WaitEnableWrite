#include "WriteEnable.h"
#include <cstddef>

namespace WaitEnableWrite
{
    class Global_WriteEnable* WriteEnable::ptr_Global = NULL;
    class WriteEnable_Control* WriteEnable::ptr_WriteEnable_Control = NULL;

    WriteEnable::WriteEnable()

    {
        ptr_Global = new WaitEnableWrite::Global_WriteEnable();
        ptr_WriteEnable_Control = NULL;
    }

    WriteEnable::~WriteEnable()
    {
        delete ptr_WriteEnable_Control;
    }

    void WriteEnable::Initialise_Control(
        class WaitEnableWrite::Global_WriteEnable* ptr_Global,
        unsigned char* ptr_MyNumImplementedCores
    )
    {
        ptr_WriteEnable_Control = new class WaitEnableWrite::WriteEnable_Control(ptr_Global, ptr_MyNumImplementedCores);
        while (ptr_WriteEnable_Control == NULL) { /* wait untill created */ }
    }

    void WriteEnable::Write_End(
        class WaitEnableWrite::WriteEnable_Control* ptr_WriteEnable_Control,
        unsigned char* ptr_coreId,
        unsigned char* ptr_MyNumImplementedCores,
        class WaitEnableWrite::Global_WriteEnable* ptr_Global
    )
    {
        for (unsigned char index = 0; index < 2; index++)
        {
            ptr_WriteEnable_Control->SetFlag_writeState(ptr_coreId, index, ptr_Global->GetConst_Write_IDLE(index));
        }
        ptr_WriteEnable_Control->Set_new_coreIdForWritePraiseIndex(*ptr_WriteEnable_Control->Get_coreIdForWritePraiseIndex() + 1);
        if (int(*ptr_WriteEnable_Control->Get_new_coreIdForWritePraiseIndex()) == 3)
        {
            ptr_WriteEnable_Control->Set_new_coreIdForWritePraiseIndex(0);
        }
        ptr_WriteEnable_Control->WriteQue_Update(
            ptr_MyNumImplementedCores
        );
        ptr_WriteEnable_Control->WriteEnable_SortQue(
            ptr_MyNumImplementedCores,
            ptr_Global
        );
        ptr_WriteEnable_Control->SetFlag_readWrite_Open(false);
    }
    void WriteEnable::Write_Start(
        class WaitEnableWrite::WriteEnable_Control* ptr_WriteEnable_Control,
        unsigned char* ptr_coreId,
        unsigned char* ptr_MyNumImplementedCores,
        class WaitEnableWrite::Global_WriteEnable* ptr_Global
    )
    {
        ptr_WriteEnable_Control->WriteEnable_Request(
            ptr_coreId,
            ptr_MyNumImplementedCores,
            ptr_Global
        );
        ptr_WriteEnable_Control->WriteQue_Update(
            ptr_MyNumImplementedCores
        );
        ptr_WriteEnable_Control->WriteEnable_SortQue(
            ptr_MyNumImplementedCores,
            ptr_Global
        );
        ptr_WriteEnable_Control->WriteEnable_Activate(
            ptr_coreId,
            ptr_Global,
            ptr_MyNumImplementedCores
        );
    }

    WriteEnable_Control* WriteEnable::Get_WriteEnable_Control()
    {
        return ptr_WriteEnable_Control;
    }

    WaitEnableWrite::Global_WriteEnable* WriteEnable::Get_GlobalForWriteControl()
    {
        return ptr_Global;
    }
}