#include "Modules/ModuleManager.h"

class FHunterWorldModule : public IModuleInterface
{
public:
    virtual void StartupModule() override 
    {
    }
    virtual void ShutdownModule() override 
    {
    }
};

IMPLEMENT_MODULE(FHunterWorldModule, HunterWorld);