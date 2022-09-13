#include "Windows.h"
#include "../../Aspen/src/Engine/Engine.h"
#include "../../Aspen/src/GameObject/GameObject.h"
#include "../../Aspen/src/Components/AllComponents.h"
#include "../../Aspen/src/Scene/Scene.h"
#include "../../Aspen/src/Input/Input.h"
#include "../../Aspen/src/Core/Time.h"

class Enemy : public Script
{
	GenerateBody()                      
public:               
void InitProperties() 
{                     
   m_Properties.push_back({ &m_MovingSpeed, VariableTypes::FLOAT, "m_MovingSpeed"});
}                     

private:
	[[SerializedField]] float m_MovingSpeed = 1.0f;

	int m_MovingDirection = 0;

	GameObject* m_Player;
	Transform* m_PlayerTransform;
	Animator* m_Animator;
	SpriteRenderer* m_SpriteRenderer;
public:
	void Start() override
	{
		m_Animator = GetComponent<Animator>();
		m_SpriteRenderer = GetComponent<SpriteRenderer>();
		auto objects = gameObject->GetScene()->GetObjectsWithTag(Tag("Player"));
		if (objects.size() != 0)
		{
			m_Player = objects[0];
			if (m_Player != nullptr)
				m_PlayerTransform = m_Player->transform;
		}
	}

	void Update() override
	{
		if (m_Player != nullptr)
		{
			Vector2f to = transform->position - m_PlayerTransform->position;
			if (transform->position.x < m_PlayerTransform->position.x)
				m_MovingDirection = -1;
			else
				m_MovingDirection = 1;

			m_SpriteRenderer->flipX = m_MovingDirection == -1;
			if (to.Magnitude() > 1.0f)
			{
				m_Animator->PlayAnimation("Walk");
				transform->position.x -= m_MovingDirection * m_MovingSpeed * Time::DeltaTime();
			}
			else
			{
				m_Animator->PlayAnimation("Idle");
			}
		}
	}
};                                                          
extern "C"                                              
{                                                         
   __declspec(dllexport) Script* CreateEnemy()
   {                                                      
       Enemy* script = new Enemy();
        script->InitProperties();                         
        return script;                                    
   }                                                      
}                                                         
