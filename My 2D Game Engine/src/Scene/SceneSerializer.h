#pragma once
#include "Scene.h"
#include "JSON/json.hpp"
using namespace nlohmann;

class SceneSerializer
{
private:
	Scene* m_Scene;

private:
	void SerializeGameObject(json& out, std::unique_ptr<GameObject>& gameObject);
	void SerializeComponent(json& out, Transform* transform);
	void SerializeComponent(json& out, SpriteRenderer* spriteRenderer);
	void SerializeComponent(json& out, BoxCollider* collider);
	void SerializeComponent(json& out, CircleCollider* collider);
	void SerializeComponent(json& out, Rigidbody* rigidbody);
	void SerializeComponent(json& out, Camera* camera);

	void DeserializeGameObject(json& in, std::unique_ptr<GameObject>& gameObject);
	void DeserializeComponent(json& in, Transform* transform);
	void DeserializeComponent(json& in, SpriteRenderer* spriteRenderer);
	void DeserializeComponent(json& in, BoxCollider* collider);
	void DeserializeComponent(json& in, CircleCollider* collider);
	void DeserializeComponent(json& in, Rigidbody* rigidbody);
	void DeserializeComponent(json& in, Camera* camera);

public:
	SceneSerializer(Scene* scene);

	void Serialize();
	void Deserialize();
};

