#include "WriteEnable_Control.h"
#include <cstddef>

namespace WaitEnableWrite
{
    unsigned char* WriteEnable_Control::ptr_coreId_For_WritePraise_Index = NULL;
    int* WriteEnable_Control::ptr_count_CoreId_WriteActive[4] = { NULL, NULL, NULL, NULL };//NUMBER OF CORES
    int* WriteEnable_Control::ptr_count_CoreId_WriteIdle[4] = { NULL, NULL, NULL, NULL };//NUMBER OF CORES
    int* WriteEnable_Control::ptr_count_CoreId_WriteWait[4] = { NULL, NULL, NULL, NULL };//NUMBER OF CORES
    bool WriteEnable_Control::flag_WriteState[4][2] = { {NULL, NULL}, {NULL, NULL}, {NULL, NULL}, {NULL, NULL} };//NUMBER OF CORES
    unsigned char* WriteEnable_Control::ptr_new_coreId_For_WritePraise_Index = NULL;
    bool WriteEnable_Control::praisingWrite = NULL;
    unsigned char* WriteEnable_Control::ptr_que_CoreToWrite[4] = { NULL, NULL, NULL, NULL };//NUMBER OF CORES

    WriteEnable_Control::WriteEnable_Control(
        class WaitEnableWrite::Global_WriteEnable* ptr_Global,
        unsigned char* ptr_MyNumImplementedCores
    )
    {
        ptr_coreId_For_WritePraise_Index = new unsigned char(0);
        int* ptr_count_CoreId_WriteActive[4] = { new int(0), new int(0), new int(0), new int(0) };//NUMBER OF CORES
        int* ptr_count_CoreId_WriteIdle[4] = { new int(0), new int(0), new int(0), new int(0) };//NUMBER OF CORES
        int* ptr_count_CoreId_WriteWait[4] = { new int(0), new int(0), new int(0), new int(0) };//NUMBER OF CORES
        bool flag_WriteState[4][2] = { {bool(false), bool(false)}, {bool(false), bool(false)}, {bool(false), bool(false)}, {bool(false), bool(false)}};//NUMBER OF CORES
        flag_WriteState[0][0] = ptr_Global->GetConst_Write_IDLE(0);
        flag_WriteState[0][1] = ptr_Global->GetConst_Write_IDLE(1);
        flag_WriteState[1][0] = ptr_Global->GetConst_Write_IDLE(0);
        flag_WriteState[1][1] = ptr_Global->GetConst_Write_IDLE(1);
        flag_WriteState[2][0] = ptr_Global->GetConst_Write_IDLE(0);
        flag_WriteState[2][1] = ptr_Global->GetConst_Write_IDLE(1);
        flag_WriteState[3][0] = ptr_Global->GetConst_Write_IDLE(0);
        flag_WriteState[3][1] = ptr_Global->GetConst_Write_IDLE(1);
        unsigned char* ptr_new_coreId_For_WritePraise_Index = new unsigned char(1);
        bool praisingWrite = new bool(false);
        unsigned char* ptr_que_CoreToWrite[4] = { new unsigned char(0), new unsigned char(1), new unsigned char(2), new unsigned char(3) };//NUMBER OF CORES
        for (unsigned char index=0; index < *ptr_MyNumImplementedCores; index++)
        {
            ptr_que_CoreToWrite[index] = &index;
        }
    }

    WriteEnable_Control::~WriteEnable_Control()
    {
        delete ptr_coreId_For_WritePraise_Index;
        delete ptr_new_coreId_For_WritePraise_Index;
        for (int index = 0; index < 4; index++)//NUMBER OF CORES
        {
            delete ptr_count_CoreId_WriteActive[index];
            delete ptr_count_CoreId_WriteIdle[index];
            delete ptr_count_CoreId_WriteWait[index];

            delete ptr_que_CoreToWrite[index];
            
        }
        //delete flag_WriteState[4][2];
        //delete praisingWrite;
    }

    void WriteEnable_Control::WriteEnable_Activate(
        unsigned char* ptr_coreId,
        class WaitEnableWrite::Global_WriteEnable* ptr_Global,
        unsigned char* ptr_MyNumImplementedCores
    )
    {
        for (unsigned char index = 0; index < *ptr_MyNumImplementedCores; index++)
        {
            SetFlag_writeState(ptr_coreId, index, ptr_Global->GetConst_Write_WRITE(index));
        }
    }

    void WriteEnable_Control::WriteEnable_SortQue(
        unsigned char* ptr_MyNumImplementedCores,
        class WaitEnableWrite::Global_WriteEnable* ptr_Global
    )
    {
        for (unsigned char index_A = 0; index_A < *ptr_MyNumImplementedCores - 1; index_A++)
        {
            for (unsigned char index_B = index_A + 1; index_B < *ptr_MyNumImplementedCores; index_B++)
            {
                if (GetFlag_writeState(Get_que_CoreToWrite(&index_A), 0) == ptr_Global->GetConst_Write_WRITE(0)
                    && GetFlag_writeState(Get_que_CoreToWrite(&index_A), 1) == ptr_Global->GetConst_Write_WRITE(1))
                {
                    if ((GetFlag_writeState(Get_que_CoreToWrite(&index_B), 0) == ptr_Global->GetConst_Write_WAIT(0)
                        && GetFlag_writeState(Get_que_CoreToWrite(&index_B), 1) == ptr_Global->GetConst_Write_WAIT(1))
                        || (GetFlag_writeState(Get_que_CoreToWrite(&index_B), 0) == ptr_Global->GetConst_Write_IDLE(0)
                            && GetFlag_writeState(Get_que_CoreToWrite(&index_B), 1) == ptr_Global->GetConst_Write_IDLE(1)))
                    {
                        WriteEnable_ShiftQueValues(&index_A, &index_B);
                    }
                    else if (GetFlag_writeState(Get_que_CoreToWrite(&index_B), 0) == ptr_Global->GetConst_Write_WRITE(0)
                        && GetFlag_writeState(Get_que_CoreToWrite(&index_B), 0) == ptr_Global->GetConst_Write_WRITE(1))
                    {
                        if (*Get_count_WriteActive(&index_A) > *Get_count_WriteActive(&index_B))
                        {
                            WriteEnable_ShiftQueValues(&index_A, &index_B);
                        }
                    }
                }
                else if (GetFlag_writeState(Get_que_CoreToWrite(&index_A), 0) == ptr_Global->GetConst_Write_IDLE(0)
                    && GetFlag_writeState(Get_que_CoreToWrite(&index_A), 1) == ptr_Global->GetConst_Write_IDLE(1))
                {
                    if (GetFlag_writeState(Get_que_CoreToWrite(&index_B), 0) == ptr_Global->GetConst_Write_WAIT(0)
                        && GetFlag_writeState(Get_que_CoreToWrite(&index_B), 1) == ptr_Global->GetConst_Write_WAIT(1))
                    {
                        WriteEnable_ShiftQueValues(&index_A, &index_B);
                    }
                    else if (GetFlag_writeState(Get_que_CoreToWrite(&index_B), 0) == ptr_Global->GetConst_Write_IDLE(0)
                        && GetFlag_writeState(Get_que_CoreToWrite(&index_B), 1) == ptr_Global->GetConst_Write_IDLE(1))
                    {
                        if (*Get_count_WriteIdle(&index_A) < *Get_count_WriteIdle(&index_B))
                        {
                            WriteEnable_ShiftQueValues(&index_A, &index_B);
                        }
                    }
                }
                else if (GetFlag_writeState(Get_que_CoreToWrite(&index_A), 0) == ptr_Global->GetConst_Write_WAIT(0)
                    && GetFlag_writeState(Get_que_CoreToWrite(&index_A), 1) == ptr_Global->GetConst_Write_WAIT(1))
                {
                    if (GetFlag_writeState(Get_que_CoreToWrite(&index_B), 0) == ptr_Global->GetConst_Write_WAIT(0)
                        && GetFlag_writeState(Get_que_CoreToWrite(&index_B), 1) == ptr_Global->GetConst_Write_WAIT(1))
                    {
                        if (*Get_count_WriteWait(&index_A) > *Get_count_WriteWait(&index_B))
                        {
                            WriteEnable_ShiftQueValues(&index_A, &index_B);
                        }
                    }
                }
            }
        }
    }

    void WriteEnable_Control::WriteEnable_Request(
        unsigned char* ptr_coreId,
        unsigned char* ptr_MyNumImplementedCores,
        class WaitEnableWrite::Global_WriteEnable* ptr_Global
    )
    {
        while (GetFlag_readWrite_Open() == true)
        {
            DynamicStagger(ptr_coreId);
        }
        SetFlag_readWrite_Open(true);
        Set_coreIdForWritePraiseIndex(*Get_new_coreIdForWritePraiseIndex());
        if (*Get_coreIdForWritePraiseIndex() == *ptr_coreId)
        {
            for (unsigned char index = 0; index < 2; index++)
            {
                SetFlag_writeState(ptr_coreId, index, ptr_Global->GetConst_Write_WAIT(index));
            }
            // Function exit.
        }
        else
        {
            Set_new_coreIdForWritePraiseIndex(*Get_coreIdForWritePraiseIndex() + 1);
            if (*Get_new_coreIdForWritePraiseIndex() == *ptr_MyNumImplementedCores)
            {
                Set_new_coreIdForWritePraiseIndex(0);
            }
            SetFlag_readWrite_Open(false);
            WriteEnable_Request(
                ptr_coreId,
                ptr_MyNumImplementedCores,
                ptr_Global

            );
        }
    }

    void WriteEnable_Control::WriteQue_Update(
        unsigned char* ptr_MyNumImplementedCores
    )
    {
        for (unsigned char index = 0; index < *ptr_MyNumImplementedCores; index++)
        {
            if (GetFlag_writeState(&index, 0) == false)
            {
                if (GetFlag_writeState(&index, 1) == false)
                {
                    Set_count_WriteActive(&index, 0);
                    Set_count_WriteIdle(&index, *Get_count_WriteIdle(&index) + 1);
                    Set_count_WriteWait(&index, 0);
                }
                else if (GetFlag_writeState(&index, 1) == true)
                {
                    Set_count_WriteActive(&index, 0);
                    Set_count_WriteIdle(&index, 0);
                    Set_count_WriteWait(&index, *Get_count_WriteWait(&index) + 1);
                }
            }
            else if (GetFlag_writeState(&index, 0) == true)
            {
                if (GetFlag_writeState(&index, 1) == false)
                {
                    Set_count_WriteActive(&index, *Get_count_WriteActive(&index) + 1);
                    Set_count_WriteIdle(&index, 0);
                    Set_count_WriteWait(&index, 0);
                }
            }
        }
    }

    unsigned char* WriteEnable_Control::Get_coreIdForWritePraiseIndex()
    {
        return ptr_coreId_For_WritePraise_Index;
    }
    int* WriteEnable_Control::Get_count_WriteActive(unsigned char* ptr_coreId)
    {
        return ptr_count_CoreId_WriteActive[*ptr_coreId];
    }
    int* WriteEnable_Control::Get_count_WriteIdle(unsigned char* ptr_coreId)
    {
        return ptr_count_CoreId_WriteIdle[*ptr_coreId];
    }
    int* WriteEnable_Control::Get_count_WriteWait(unsigned char* ptr_coreId)
    {
        return ptr_count_CoreId_WriteWait[*ptr_coreId];
    }
    unsigned char* WriteEnable_Control::GetFlag_CoreId_WriteEnable()
    {
        return ptr_que_CoreToWrite[0];
    }
    unsigned char* WriteEnable_Control::Get_new_coreIdForWritePraiseIndex()
    {
        return ptr_new_coreId_For_WritePraise_Index;
    }
    unsigned char* WriteEnable_Control::Get_que_CoreToWrite(unsigned char* index)
    {
        return ptr_que_CoreToWrite[*index];
    }

    void WriteEnable_Control::Set_count_WriteActive(unsigned char* ptr_coreId, int value)
    {
        ptr_count_CoreId_WriteActive[*ptr_coreId] = &value;
    }
    void WriteEnable_Control::Set_count_WriteIdle(unsigned char* ptr_coreId, int value)
    {
        ptr_count_CoreId_WriteIdle[*ptr_coreId] = &value;
    }
    void WriteEnable_Control::Set_count_WriteWait(unsigned char* ptr_coreId, int value)
    {
        ptr_count_CoreId_WriteWait[*ptr_coreId] = &value;
    }
    void WriteEnable_Control::SetFlag_readWrite_Open(bool value)
    {
        praisingWrite = value;
    }
    void WriteEnable_Control::SetFlag_writeState(unsigned char* ptr_coreId, unsigned char index, bool value)
    {
        flag_WriteState[*ptr_coreId][index] = &value;
    }
    void WriteEnable_Control::Set_new_coreIdForWritePraiseIndex(unsigned char value)
    {
        ptr_new_coreId_For_WritePraise_Index = &value;
    }
    void WriteEnable_Control::Set_que_CoreToWrite(unsigned char* index, unsigned char value)
    {
        ptr_que_CoreToWrite[*index] = &value;
    }

    void WriteEnable_Control::DynamicStagger(
        unsigned char* ptr_coreId
    )
    {
        if (*Get_coreIdForWritePraiseIndex() == *ptr_coreId)
        {
            //exit early
        }
        else
        {
            int* ptr_count = new int(0);
            while (*ptr_count < 20)//TODO record till flag change
            {
                ptr_count = ptr_count + 1;
            }
            delete ptr_count;
        }
    }

    void WriteEnable_Control::WriteEnable_ShiftQueValues(
        unsigned char* coreId_A,
        unsigned char* coreId_B
    )
    {
        int* ptr_temp_A = new int(0);
        ptr_temp_A = Get_count_WriteActive(coreId_A);
        Set_count_WriteActive(coreId_A, *Get_count_WriteActive(coreId_B));
        Set_count_WriteActive(coreId_B, *ptr_temp_A);

        ptr_temp_A = Get_count_WriteIdle(coreId_A);
        Set_count_WriteIdle(coreId_A, *Get_count_WriteIdle(coreId_B));
        Set_count_WriteIdle(coreId_B, *ptr_temp_A);

        ptr_temp_A = Get_count_WriteWait(coreId_A);
        Set_count_WriteWait(coreId_A, *Get_count_WriteWait(coreId_B));
        Set_count_WriteWait(coreId_B, *ptr_temp_A);
        delete ptr_temp_A;

        unsigned char* ptr_temp_B = new unsigned char(0);
        ptr_temp_B = Get_que_CoreToWrite(coreId_A);
        Set_que_CoreToWrite(coreId_A, *Get_que_CoreToWrite(coreId_B));
        Set_que_CoreToWrite(coreId_B, *ptr_temp_B);
        delete ptr_temp_B;
    }

    bool WriteEnable_Control::GetFlag_readWrite_Open()
    {
        return praisingWrite;
    }

    bool WriteEnable_Control::GetFlag_writeState(unsigned char* ptr_coreId, unsigned char index)
    {
        return flag_WriteState[*ptr_coreId][index];
    }

    void WriteEnable_Control::Set_coreIdForWritePraiseIndex(unsigned char value)
    {
        ptr_coreId_For_WritePraise_Index = &value;
    }
}