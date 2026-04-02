#pragma once

#include "CoreDefines.h"

#include <memory>

class MOONLIGHT_API IIndexBuffer
{
public:
    virtual ~IIndexBuffer() = default;
    
    virtual uint32 GetCount() const = 0; 
};
