#pragma once

#include <initializer_list>
#include <memory>

enum class EShaderDataType
{
    Float,
    Float2,
    Float3,
    Float4,

    Matrix3x3,
    Matrix4x4,

    Int,
    Int2,
    Int3,
    Int4,

    Boolean
};

enum class EVertexBufferUsage
{
    Static,
    Dynamic
};

struct CVertexBufferElement
{
    std::string Name;
    EShaderDataType DataType;
    uint32 Size;
    uint32 Offset;
    bool bIsNormalized;

    CVertexBufferElement() = default;
    CVertexBufferElement(std::string Name, const EShaderDataType DataType, bool bIsNormalized = false)
        : Name(std::move(Name)), DataType(DataType), Size(GetShaderDataTypeSize(DataType)), Offset(0), bIsNormalized(bIsNormalized) {}

    uint32 GetComponentCount() const
    {
        switch (DataType)
        {
            case EShaderDataType::Float:     return 1;
            case EShaderDataType::Float2:    return 2;
            case EShaderDataType::Float3:    return 3;
            case EShaderDataType::Float4:    return 4;
            case EShaderDataType::Matrix3x3: return 9;
            case EShaderDataType::Matrix4x4: return 16;
            case EShaderDataType::Int:       return 1;
            case EShaderDataType::Int2:      return 2;
            case EShaderDataType::Int3:      return 3;
            case EShaderDataType::Int4:      return 4;
            case EShaderDataType::Boolean:   return 1;
        }

        verifyEnginef(false, "Failed to get component count of vertex buffer element!");
        return 0;
    }
private:
    static uint32 GetShaderDataTypeSize(EShaderDataType ShaderDataType)
    {
        switch (ShaderDataType)
        {
            case EShaderDataType::Float:     return 4;
            case EShaderDataType::Float2:    return 8;
            case EShaderDataType::Float3:    return 12;
            case EShaderDataType::Float4:    return 16;
            case EShaderDataType::Matrix3x3: return 4 * 3 * 3;
            case EShaderDataType::Matrix4x4: return 4 * 4 * 4;
            case EShaderDataType::Int:       return 4;
            case EShaderDataType::Int2:      return 8;
            case EShaderDataType::Int3:      return 12;
            case EShaderDataType::Int4:      return 16;
            case EShaderDataType::Boolean:   return 1;
        }

        verifyEnginef(false, "Failed to get size of shader data type!")
        return 0;
    }
};

class CVertexBufferLayout
{
public:
    CVertexBufferLayout() = default;
    CVertexBufferLayout(const std::initializer_list<CVertexBufferElement>& VertexBufferElements)
        : m_VertexBufferElements(VertexBufferElements)
    {
        CalculateOffsetsAndStride();
    }

    const std::vector<CVertexBufferElement>& GetElements() const { return m_VertexBufferElements; }
    uint32 GetElementCount() const { return static_cast<uint32>(m_VertexBufferElements.size()); }
    uint32 GetStride() const { return m_Stride; }
    
    [[nodiscard]] std::vector<CVertexBufferElement>::iterator begin() { return m_VertexBufferElements.begin(); }
    [[nodiscard]] std::vector<CVertexBufferElement>::iterator end() { return m_VertexBufferElements.end(); }
    [[nodiscard]] std::vector<CVertexBufferElement>::const_iterator begin() const { return m_VertexBufferElements.begin(); }
    [[nodiscard]] std::vector<CVertexBufferElement>::const_iterator end() const { return m_VertexBufferElements.end(); }
private:
    void CalculateOffsetsAndStride()
    {
        uint32 Offset = 0;
        m_Stride = 0;

        for (auto& VertexBufferElement : m_VertexBufferElements)
        {
            VertexBufferElement.Offset = Offset;
            Offset += VertexBufferElement.Size;
            m_Stride += VertexBufferElement.Size;
        }
    }
private:
    std::vector<CVertexBufferElement> m_VertexBufferElements;

    uint32 m_Stride = 0;
};

class IVertexBuffer
{
public:
    virtual ~IVertexBuffer() = default;

    virtual uint64 GetSize() const = 0;
    
    static std::shared_ptr<IVertexBuffer> Create(const void* Data, uint64 Size, EVertexBufferUsage Usage = EVertexBufferUsage::Static);
    static std::shared_ptr<IVertexBuffer> Create(uint64 Size, EVertexBufferUsage Usage = EVertexBufferUsage::Dynamic);
};
