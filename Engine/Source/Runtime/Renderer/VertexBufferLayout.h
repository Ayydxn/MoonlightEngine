#pragma once

enum class EShaderDataType
{
    Unknown,

    Float, Float2, Float3, Float4,
    Matrix3, Matrix4,
    Int, Int2, Int3, Int4,
    Boolean
};

struct FVertexBufferElement
{
    std::string ElementName;
    EShaderDataType ElementDataType;
    uint32 Size;
    uint32 Offset;
    bool bNormalized;

    FVertexBufferElement() = default;

    FVertexBufferElement(const EShaderDataType DataType, std::string Name, const bool bIsNormalized = false)
        : ElementName(std::move(Name)), ElementDataType(DataType), Size(GetShaderDataTypeSize(DataType)), Offset(0), bNormalized(bIsNormalized) {}

    uint32_t GetComponentCount() const
    {
        switch (ElementDataType)
        {
            case EShaderDataType::Unknown:    return 0;
            case EShaderDataType::Float:      return 1;
            case EShaderDataType::Float2:     return 2;
            case EShaderDataType::Float3:     return 3;
            case EShaderDataType::Float4:     return 4;
            case EShaderDataType::Matrix3:    return 3 * 3;
            case EShaderDataType::Matrix4:    return 4 * 4;
            case EShaderDataType::Int:        return 1;
            case EShaderDataType::Int2:       return 2;
            case EShaderDataType::Int3:       return 3;
            case EShaderDataType::Int4:       return 4;
            case EShaderDataType::Boolean:    return 1;
            default:
            {
                assertEnginef(false, "Failed to get the component count of vertex buffer element!")
                return 0;
            }
        }
    }
private:
    uint32 GetShaderDataTypeSize(const EShaderDataType ShaderDataType)
    {
        switch (ShaderDataType)
        {
            case EShaderDataType::Unknown: return 0;
            case EShaderDataType::Float:   return 4;
            case EShaderDataType::Float2:  return 4 * 2;
            case EShaderDataType::Float3:  return 4 * 3;
            case EShaderDataType::Float4:  return 4 * 4;
            case EShaderDataType::Matrix3: return 4 * 3 * 3;
            case EShaderDataType::Matrix4: return 4 * 4 * 4;
            case EShaderDataType::Int:     return 4;
            case EShaderDataType::Int2:    return 4 * 2;
            case EShaderDataType::Int3:    return 4 * 3;
            case EShaderDataType::Int4:    return 4 * 4;
            case EShaderDataType::Boolean: return 1;
            default:
            {
                assertEnginef(false, "Failed to get the size of the shader data type!")
                return 0;
            }
        }
    }
};

class FVertexBufferLayout
{
public:
    FVertexBufferLayout() = default;

    FVertexBufferLayout(const std::initializer_list<FVertexBufferElement>& VertexBufferElements)
        : m_VertexBufferElements(VertexBufferElements)
    {
        CalculateOffsetsAndStride();
    }

    const std::vector<FVertexBufferElement>& GetElements() const { return m_VertexBufferElements; }
    uint32 GetStride() const { return m_Stride; }
    uint32 GetElementCount() const { return static_cast<uint32>(m_VertexBufferElements.size()); }

    [[nodiscard]] std::vector<FVertexBufferElement>::iterator begin() { return m_VertexBufferElements.begin(); }
    [[nodiscard]] std::vector<FVertexBufferElement>::iterator end() { return m_VertexBufferElements.end(); }
    [[nodiscard]] std::vector<FVertexBufferElement>::const_iterator begin() const { return m_VertexBufferElements.begin(); }
    [[nodiscard]] std::vector<FVertexBufferElement>::const_iterator end() const { return m_VertexBufferElements.end(); }
private:
    void CalculateOffsetsAndStride();
private:
    std::vector<FVertexBufferElement> m_VertexBufferElements;
    uint32 m_Stride = 0;
};
