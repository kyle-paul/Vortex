#pragma once

namespace Vortex
{
    class TimeStep 
    {
    public:
        TimeStep(float time = 0.0f) 
            : m_Time(time)
        {

        }
        operator float() const { return m_Time; }
        
        float GetSeconds() const { return m_Time; }
        float GetMilliSeconds() const { return m_Time * 1000; }
    
    private:
        float m_Time;
    };
}