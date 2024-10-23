#pragma once
#include "Vortex.h"
#include "ParticleSystem.h"
#include <glm/glm.hpp>

class Player
{
public:
    Player();
    ~Player();

    void LoadAssets();
    void OnRender();
    void OnUpdate(Vortex::TimeStep ts);

    float GetRotation() { return m_Velocity.y * 4.0f - 90.0f; }
	inline const glm::vec2& GetPosition() const { return m_Position; }

    void OnImGuiRender();
	void Reset();
    
    uint32_t GetScore() const { return (uint32_t)(m_Position.x + 10.0f) / 10.0f; }
    
private:
    glm::vec2 m_Position = { -10.0f, 0.0f };
	glm::vec2 m_Velocity = { 5.0f, 0.0f };

	float m_EnginePower = 0.5f;
	float m_Gravity = 0.4f;

	float m_Time = 0.0f;
	float m_SmokeEmitInterval = 0.4f;
	float m_SmokeNextEmitTime = m_SmokeEmitInterval;

    ParticleProps m_SmokeParticle, m_EngineParticle;
	ParticleSystem m_ParticleSystem;

    Vortex::Ref<Vortex::Texture2D> m_ShipTexture;
};