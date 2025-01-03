#pragma once
#include "WriteEnable_Control.h"
#include "Global_WriteEnable.h"
#include "Framework_WriteEnable.h"

namespace WaitEnableWrite
{
    class WriteEnable
    {
    public:
        WriteEnable();
        virtual ~WriteEnable();
        void Initialise_Control(
            class Global_WriteEnable* ptr_Global,
            unsigned char* ptr_MyNumImplementedCores
        );
        void Write_End(
            class WriteEnable_Control* ptr_WriteEnable_Control,
            unsigned char* ptr_coreId,
            unsigned char* ptr_MyNumImplementedCores,
            class Global_WriteEnable* ptr_Global
        );
        void Write_Start(
            class WriteEnable_Control* ptr_WriteEnable_Control,
            unsigned char* ptr_coreId,
            unsigned char* ptr_MyNumImplementedCores,
            class Global_WriteEnable* ptr_Global
        );

        class WriteEnable_Control* Get_WriteEnable_Control();
        class Global_WriteEnable* Get_GlobalForWriteControl();
    protected:

    private:
        static class Global_WriteEnable* ptr_Global;
        static class WriteEnable_Control* ptr_WriteEnable_Control;
    };
}