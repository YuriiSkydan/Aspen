#pragma once
#include "box2d/b2_fixture.h"
#include "../Math/Vector2.h"
#include "Collision.h"
#include "Trigger.h"
#include "Material.h"

class ASPEN Collider : public Collision, public Trigger
{
protected:
	b2Body* m_Body = nullptr;
	b2FixtureDef m_FixtureDef;
	b2Fixture* m_Fixture;
	Vector2f m_Offset{ 0.0f, 0.0f };

public:
	Material material;

protected:
	Collider(GameObject* gameObject, Transform* transform);

	void Start() override;

	void Reset() override;
	void ResetShape();
	void ResetFixture();
	void SetFixtureDef();
	virtual void SetShape();

	b2Body* GetBody() const { return m_Body; }

	void Serialize(json& out) const override;
	void Deserialize(json& in) override;

public:
	void SetOffset(const Vector2f& offset);
	void SetIsTrigger(bool isTrigger);
	bool IsTrigger() const { return m_IsTrigger; }
	const Vector2f& GetOffset() const { return m_Offset; }
};