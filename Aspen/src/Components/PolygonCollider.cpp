#include "PolygonCollider.h"

void PolygonCollider::SetShape()
{
	m_Shape.Set(m_Vertices.data(), m_Vertices.size());
	m_FixtureDef.shape = &m_Shape;
}

PolygonCollider::PolygonCollider(GameObject* gameObject, Transform* transform)
	: Collider(gameObject, transform)
	, Component(gameObject, transform)
{
	m_Vertices.push_back({ -0.5f, -0.5f });
	m_Vertices.push_back({ 0.5f, -0.5f });
	m_Vertices.push_back({ 0.0f,  0.5f });
}

void PolygonCollider::Start()
{
	SetShape();
	Collider::Start();
}

void PolygonCollider::AddVertex(const Vector2f& vertexPos)
{
	m_Vertices.push_back({ vertexPos.x, vertexPos.y });
	ResetShape();
}

void PolygonCollider::RemoveVertex(const Vector2f& vertexPos)
{
	for (auto it = m_Vertices.begin(); it != m_Vertices.end(); ++it)
	{
		if (*it == b2Vec2{ vertexPos.x, vertexPos.y })
		{
			it = m_Vertices.erase(it);
			break;
		}
	}

	ResetShape();
}

void PolygonCollider::RemoveVertex(const std::vector<b2Vec2>::const_iterator& vertex)
{
	m_Vertices.erase(vertex);
	ResetShape();
}

void PolygonCollider::SetVerices(const std::vector<b2Vec2>& verticies)
{
	m_Vertices = verticies;
	ResetShape();
}

void PolygonCollider::Serialize(json& out) const
{
	out["PolygonCollider"] =
	{
		{ "Vertices", m_Vertices.size() }
	};

	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		json& vertexOut = out["PolygonCollider"]["Vertex"];
		vertexOut[std::to_string(i)]["X"] = m_Vertices[i].x;
		vertexOut[std::to_string(i)]["Y"] = m_Vertices[i].y;
	}

	Collider::Serialize(out["PolygonCollider"]);
}

void PolygonCollider::Deserialize(json& in)
{
	Collider::Deserialize(in);

	m_Vertices.resize(in["Vertices"]);

	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		m_Vertices[i].x = in["Vertex"][std::to_string(i)]["X"];
		m_Vertices[i].y = in["Vertex"][std::to_string(i)]["Y"];
	}
}