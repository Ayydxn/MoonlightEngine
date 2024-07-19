#pragma once

enum class EShaderStage
{
    Unknown,

    Vertex,
    Fragment
};

class IShader
{
public:
    virtual ~IShader() = default;
};
