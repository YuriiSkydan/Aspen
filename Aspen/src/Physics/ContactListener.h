#pragma once
#include "box2d/b2_contact.h"
#include "box2d/b2_world_callbacks.h"

class Scene;
class GameObject;

//Make a seperate file for it
class ContactListener : public b2ContactListener
{
private:
	Scene* m_ScenePtr;

private:
	void OnTriggerEnter(GameObject* gameObject);
	void OnTriggerStay(GameObject* gameObject);
	void OnTriggerExit(GameObject* gameObject);

	void OnCollisionEnter(GameObject* gameObject, GameObject* entered);
	void OnCollisionStay(GameObject* gameObject);
	void OnCollisionExit(GameObject* gameObject);

public:
	ContactListener(Scene* scene);
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
};
