#include "Modules/ModuleManager.h"

class FTestDemoModule : public IModuleInterface
{
public:
    virtual void StartupModule() override 
    {
    }
    virtual void ShutdownModule() override 
    {
    }
};

IMPLEMENT_MODULE(FTestDemoModule, TestDemo);