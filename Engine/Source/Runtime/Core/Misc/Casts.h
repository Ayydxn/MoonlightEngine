#pragma once

#include <memory>

template<typename To, typename From>
std::shared_ptr<To> Cast(const std::shared_ptr<From>& Source)
{
    return std::dynamic_pointer_cast<To>(Source);
}