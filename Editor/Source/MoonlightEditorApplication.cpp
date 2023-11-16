#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.h"
#include "Input/Input.h"

class FMoonlightEditorLayer : public FLayer
{
public:
    FMoonlightEditorLayer()
        : FLayer("Moonlight Editor Layer") {}
};

class FMoonlightEditorApplication : public FApplication
{
public:
    FMoonlightEditorApplication(const FApplicationSpecification& Specification)
        : FApplication(Specification)
    {
        PushLayer(new FMoonlightEditorLayer());
    }
};

FApplication* CreateApplication(char** Arguments, int32 ArgumentCount)
{
    FApplicationSpecification MoonlightEditorApplicationSpecification;
    MoonlightEditorApplicationSpecification.Name = "Moonlight Editor";
    MoonlightEditorApplicationSpecification.CommandLineArguments = FCommandLineArguments(Arguments, ArgumentCount);

    return new FMoonlightEditorApplication(MoonlightEditorApplicationSpecification);
}