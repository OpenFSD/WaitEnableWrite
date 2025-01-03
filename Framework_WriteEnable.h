#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include "WriteEnable.h"

namespace WaitEnableWrite
{
    class Framework_WriteEnable
    {
    public:
        Framework_WriteEnable();
        virtual ~Framework_WriteEnable();
        static class WriteEnable* Get_WriteEnable();

    protected:

    private:
        static class WriteEnable* ptr_WriteEnable;
    };
}