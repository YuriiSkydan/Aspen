#pragma once
#include "Component.h"
 
class ASPEN AudioListener : public Component
{
public:
	AudioListener(GameObject* gameObject, Transform* transform);

	void Update() override;
};