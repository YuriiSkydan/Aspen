#include "BoxCollider.h"

BoxCollider::BoxCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{ }

void BoxCollider::Serialize(json& out) const
{
	Component::Serialize(out["BoxCollider"]);

	out["BoxCollider"] =
	{
		{ "Size",
		{ { "X", size.x },
		  { "Y", size.y } }},
		{ "Offset",
		{ { "X", offset.x },
		  { "Y", offset.y } }}
	};
}

void BoxCollider::Deserialize(json& in)
{
	Component::Deserialize(in);

	size.x = in["Size"]["X"];
	size.y = in["Size"]["Y"];

	offset.x = in["Offset"]["X"];
	offset.y = in["Offset"]["Y"];
}