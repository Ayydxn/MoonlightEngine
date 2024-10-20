#include "MoonlightPCH.h"
#include "Clock.h"

void CClock::Start()
{
    const auto Now = std::chrono::high_resolution_clock::now();

    m_StartTime = std::chrono::time_point_cast<std::chrono::duration<float>>(Now).time_since_epoch().count();
    m_Elapsed = 0.0f;
}

void CClock::Update()
{
    if (m_StartTime != 0.0f)
    {
        const auto Now = std::chrono::high_resolution_clock::now();

        m_Elapsed = std::chrono::time_point_cast<std::chrono::duration<float>>(Now).time_since_epoch().count() - m_StartTime;
    }
}

void CClock::Stop()
{
    m_StartTime = 0.0f;
}
