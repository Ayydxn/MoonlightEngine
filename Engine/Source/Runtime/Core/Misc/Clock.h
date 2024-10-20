#pragma once

class CClock
{
public:
    CClock() = default;
    ~CClock() = default;

    void Start();
    void Update();
    void Stop();

    float GetElapsedTime() const { return m_Elapsed; }
private:
    float m_StartTime = 0.0f;
    float m_Elapsed = 0.0f;
};
