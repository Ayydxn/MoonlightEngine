#include "MoonlightPCH.h"
#include "VulkanShader.h"
#include "VulkanContext.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shaders/ShaderUtils.h"
#include "RHICore/ShaderCompiler.h"
#include "Utils/FileUtils.h"

#include <ranges>

namespace
{
    vk::ShaderStageFlagBits ConvertMoonlightShaderStageToVulkan(const EShaderStage ShaderStage)
    {
        switch (ShaderStage)
        {
            case EShaderStage::Unknown: return static_cast<vk::ShaderStageFlagBits>(-1);
            case EShaderStage::Vertex: return vk::ShaderStageFlagBits::eVertex;
            case EShaderStage::Fragment: return vk::ShaderStageFlagBits::eFragment;
        }

        verifyEnginef(false, "Failed to convert Moonlight shader stage to Shaderc!")
        return static_cast<vk::ShaderStageFlagBits>(-1);
    }
}

CVulkanShader::CVulkanShader(const std::string& Name, const std::string& Source, EShaderStage ShaderStage)
    : m_LogicalDevice(CVulkanContext::GetInstance()->GetLogicalDevice()->GetHandle()), m_Name(Name)
{
    IShaderCompiler& ShaderCompiler = IShaderCompiler::GetInstance();
    std::vector<uint32> ShaderBytecode;
    
    ShaderCompiler.CompileShader(Name, Source, ShaderStage, ShaderBytecode);

    m_ShaderStageToSPIRV[ShaderStage] = ShaderBytecode;

    CreateShaderModules();
}

CVulkanShader::CVulkanShader(const std::filesystem::path& Filepath)
    : m_LogicalDevice(CVulkanContext::GetInstance()->GetLogicalDevice()->GetHandle()), m_Name(Filepath.stem().string())
{
    std::vector<std::filesystem::path> ShadersToCompile = {};
    IShaderCompiler& ShaderCompiler = IShaderCompiler::GetInstance();

    // In the directory that the shader is presumably located in, we find all of its associated shaders (vertex shaders, fragment shaders, etc.)
    // and add them to a list of shaders that are to be compiled.
    //
    // What is considered to be an "associated shader", is simply the file having the same name as the shader. For example:
    // Passing "Resources/Shaders/MyShader" means that "MyShader.vert.glsl" is associated with it.
    // Passing "Resources/Shaders/MyShader" means that "MyOtherShader.vert.glsl" is not associated with it.
    for (const auto& FileEntry : std::filesystem::directory_iterator(CFileUtils::GetFileParentDirectory(Filepath)))
    {
        const auto FileEntryPath = std::filesystem::path(FileEntry);
        const auto FileEntryName = Filepath.stem().string().substr(0, Filepath.stem().string().find_last_of('.'));

        if (FileEntryName == Filepath.stem().string())
            ShadersToCompile.push_back(FileEntryPath);
    }

    for (const auto& ShaderToCompile : ShadersToCompile)
    {
        EShaderStage ShaderStage = CShaderUtils::GetShaderStageFromFileExtension(ShaderToCompile);
        
        std::vector<uint32> ShaderBytecode;
        ShaderCompiler.CompileShaderFromFile(ShaderToCompile, ShaderStage, ShaderBytecode);
        
        m_ShaderStageToSPIRV[ShaderStage] = ShaderBytecode;
    }

    CreateShaderModules();
}

CVulkanShader::~CVulkanShader()
{
    for (const vk::ShaderModule& ShaderModule : m_ShaderModules)
        m_LogicalDevice.destroyShaderModule(ShaderModule);
}

void CVulkanShader::CreateShaderModules()
{
    for (const auto& [ShaderStage, ShaderBytecode] : m_ShaderStageToSPIRV)
    {
        vk::ShaderModuleCreateInfo ShaderModuleCreateInfo;
        ShaderModuleCreateInfo.sType = vk::StructureType::eShaderModuleCreateInfo;
        ShaderModuleCreateInfo.codeSize = static_cast<uint32>(ShaderBytecode.size()) * sizeof(uint32);
        ShaderModuleCreateInfo.pCode = ShaderBytecode.data();

        vk::ShaderModule ShaderModule;

        try
        {
            ShaderModule = m_LogicalDevice.createShaderModule(ShaderModuleCreateInfo);
        }
        catch (const vk::SystemError& VulkanSystemError)
        {
            verifyEnginef(false, "Failed to create Vulkan shader module! ({})", VulkanSystemError.what())
        }

        m_ShaderModules.push_back(ShaderModule);

        vk::PipelineShaderStageCreateInfo& PipelineShaderStageCreateInfo = m_PipelineShaderStageCreateInfos.emplace_back();
        PipelineShaderStageCreateInfo.sType = vk::StructureType::ePipelineShaderStageCreateInfo;
        PipelineShaderStageCreateInfo.stage = ConvertMoonlightShaderStageToVulkan(ShaderStage);
        PipelineShaderStageCreateInfo.module = ShaderModule;
        PipelineShaderStageCreateInfo.pName = "main";
    }
}
