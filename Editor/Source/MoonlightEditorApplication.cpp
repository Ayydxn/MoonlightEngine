#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.h"

class CMoonlightEditorLayer : public CLayer
{
public:
    CMoonlightEditorLayer()
        : CLayer("Moonlight Editor Layer") {}

    void OnAttach() override
    {
        APPLICATION_LOG_INFO_TAG("Core", "Hello from the Moonlight Editor Layer!");
    }
};

class CMoonlightEditorApplication : public CApplication
{
public:
    CMoonlightEditorApplication(const CApplicationSpecification& Specification)
        : CApplication(Specification)
    {
        PushLayer(new CMoonlightEditorLayer());
    }
};

CApplication* CreateApplication(char** Arguments, int32 ArgumentCount)
{
    CApplicationSpecification MoonlightEditorApplicationSpecification;
    MoonlightEditorApplicationSpecification.Name = "Moonlight Editor";

    return new CMoonlightEditorApplication(MoonlightEditorApplicationSpecification);
}