#include "ContactListener.h"
#include "../Scene/SceneManager.h"

//Contact Listener
void ContactListener::OnTriggerEnter(GameObject* gameObject)
{
	auto& scripts = gameObject->GetScripts();
	Trigger* trigger = gameObject->GetComponent<Trigger>();

	if (trigger != nullptr)
	{
		for (auto& script : scripts)
			script->OnTriggerEnter(trigger);
	}
}

void ContactListener::OnTriggerExit(GameObject* gameObject)
{
	auto& scripts = gameObject->GetScripts();
	Trigger* trigger = gameObject->GetComponent<Trigger>();

	if (trigger != nullptr)
	{
		for (auto& script : scripts)
			script->OnTriggerExit(trigger);
	}
}

void ContactListener::OnCollisionEnter(GameObject* gameObject, GameObject* entered)
{
	auto& scripts = gameObject->GetScripts();
	Collision* collision = entered->GetComponent<Collision>();

	if (collision != nullptr)
	{
		for (auto& script : scripts)
			script->OnCollisionEnter(collision);
	}
}

void ContactListener::OnCollisionExit(GameObject* gameObject)
{
	auto& scripts = gameObject->GetScripts();
	Collision* collision = gameObject->GetComponent<Collision>();

	if (collision != nullptr)
	{
		for (auto& script : scripts)
			script->OnCollisionExit(collision);
	}
}

void ContactListener::BeginContact(b2Contact* contact)
{
	int objectAID = contact->GetFixtureA()->GetUserData().pointer;
	int objectBID = contact->GetFixtureB()->GetUserData().pointer;
	GameObject* objectA = SceneManager::GetActiveScene()->GetObjectWithID(objectAID);
	GameObject* objectB = SceneManager::GetActiveScene()->GetObjectWithID(objectBID);

	if (!contact->GetFixtureA()->IsSensor() || !contact->GetFixtureB()->IsSensor())
	{
		if (contact->GetFixtureA()->IsSensor())
		{
			OnTriggerEnter(objectA);
		}
		else if (contact->GetFixtureB()->IsSensor())
		{
			OnTriggerEnter(objectB);
		}
		else
		{
			OnCollisionEnter(objectA, objectB);
			OnCollisionEnter(objectB, objectA);
		}
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	int objectAID = contact->GetFixtureA()->GetUserData().pointer;
	int objectBID = contact->GetFixtureB()->GetUserData().pointer;
	GameObject* objectA = SceneManager::GetActiveScene()->GetObjectWithID(objectAID);
	GameObject* objectB = SceneManager::GetActiveScene()->GetObjectWithID(objectBID);

	if (!contact->GetFixtureA()->IsSensor() || !contact->GetFixtureB()->IsSensor())
	{
		if (contact->GetFixtureA()->IsSensor())
		{
			OnTriggerExit(objectA);
		}
		else if (contact->GetFixtureB()->IsSensor())
		{
			OnTriggerExit(objectB);
		}
		else
		{
			OnCollisionExit(objectA);
			OnCollisionExit(objectB);
		}
	}
}