
#include <iostream>
#include <memory>
#include "Sources/System/Application.h"
#include "Sources/System/Log.h"

int main()
{
    // Set Debug Flags
    int	nDbgFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    nDbgFlags |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(nDbgFlags);

    std::cout << "Hello World!\n";
    LogInsert("Hello World!\n");
    
    auto myApplication=std::make_unique<Application>();

    myApplication->Init();
    myApplication->Run();

    std::cout << "\n\n";
    system("pause");
}
