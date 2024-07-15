#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.h"

class FMoonlightEditorApplication : public CApplication
{
public:
    FMoonlightEditorApplication(const CApplicationSpecification& Specification)
        : CApplication(Specification) {}
};

CApplication* CreateApplication(char** Arguments, int32 ArgumentCount)
{
    CApplicationSpecification MoonlightEditorApplicationSpecification;
    MoonlightEditorApplicationSpecification.Name = "Moonlight Editor";

    return new FMoonlightEditorApplication(MoonlightEditorApplicationSpecification);
}