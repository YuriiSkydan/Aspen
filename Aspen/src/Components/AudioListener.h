#pragma once
#include "Component.h"
 
class AudioListener : public Component
{
public:
	AudioListener(GameObject* gameObject, Transform* transform);

	void Update() override;
};