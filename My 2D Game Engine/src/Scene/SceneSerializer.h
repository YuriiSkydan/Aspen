#pragma once
#include "Scene.h"
#include "../src/GameObject/GameObject.h"
#include "JSON/json.hpp"
using namespace nlohmann;

class SceneSerializer
{
private:
	Scene* m_Scene;

private:
	template<typename... Components>
	void SerializeComponents(json& out, const std::unique_ptr<GameObject>& gameObject) const
	{
		([&]()
			{
				if (gameObject->HasComponent<Components>())
				{
					Components* component = gameObject->GetComponent<Components>();
					SerializeComponent(out, component);
				}
			}(), ...);
	}
	void SerializeGameObject(json& out, const std::unique_ptr<GameObject>& gameObject) const;
	void SerializeComponent(json& out, Transform* transform) const;
	void SerializeComponent(json& out, SpriteRenderer* spriteRenderer) const;
	void SerializeComponent(json& out, BoxCollider* collider) const;
	void SerializeComponent(json& out, CircleCollider* collider) const;
	void SerializeComponent(json& out, Rigidbody* rigidbody) const;
	void SerializeComponent(json& out, Camera* camera) const;

	void DeserializeGameObject(json& in, std::unique_ptr<GameObject>& gameObject);
	void DeserializeComponent(json& in, Transform* transform);
	void DeserializeComponent(json& in, SpriteRenderer* spriteRenderer);
	void DeserializeComponent(json& in, BoxCollider* collider);
	void DeserializeComponent(json& in, CircleCollider* collider);
	void DeserializeComponent(json& in, Rigidbody* rigidbody);
	void DeserializeComponent(json& in, Camera* camera);

public:
	SceneSerializer(Scene* scene);

	void Serialize() const;
	void Deserialize();
};

