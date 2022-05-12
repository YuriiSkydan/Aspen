//#include "Physics.h"
//#include "src/GameObject/GameObject.h"
//#include "src/Math/Math.h"
//
//void PhysicsWorld::Update(float dt)
//{
//	{
//		for (Rigidbody* objA : m_Objects)
//		{
//			if (objA->m_MassData.mass != 0)
//			{
//				objA->velocity += (objA->m_Force * objA->m_MassData.invMass + m_Gravity) * dt;
//				//objA->angularVelocity += objA->torque * objA->m_MassData.invInertia * dt;
//				std::cout << "Angular Velocity: " << objA->angularVelocity << std::endl;
//				objA->transform->angle -= (objA->angularVelocity * 180 / std::numbers::pi) * dt;
//				objA->transform->position += objA->velocity * dt;
//			}
//
//			for (Rigidbody* objB : m_Objects)
//			{
//				if (objA != objB)
//				{
//					Manifold manifold(objA->m_Collider.get(), objB->m_Collider.get());
//					manifold.Solve();
//
//					if (manifold.contactCount != 0)
//					{
//						std::cout << "Contact points:\n";
//						for (size_t i = 0; i < manifold.contactCount; i++)
//							std::cout << manifold.contacts[0].x << "\t" << manifold.contacts[0].y << std::endl;
//
//						objA->gameObject->GetComponent<SpriteRenderer>()->SetColor(Color(0.0f, 1.0f, 0.0f, 1.0f));
//						manifold.Initialize();
//						manifold.ApplyImpulse();
//
//						manifold.PositionCorrection();
//						/*				Vector2 rv = objB->velocity - objA->velocity;
//
//										Vector2 normal = objB->transform->position;
//										normal -= objA->transform->position;
//										normal.Normalize();
//
//										float velAlongNormal = Dot(rv, normal);
//
//										if (velAlongNormal > 0)
//											break;
//
//										INFO("RV X: {0}", rv.x);
//										INFO("RV Y: {0}", rv.y);
//										INFO("Normal X: {0}", normal.x);
//										INFO("Normal Y: {0}", normal.y);
//										INFO("Velocity along normal: {0}", velAlongNormal);
//
//										float e = 1.0f;
//
//										float j = -(1 + e) * velAlongNormal;
//
//
//										j /= objA->m_MassData.invMass + objB->m_MassData.invMass;
//
//										Vector2 impulse = j * normal;
//										objA->velocity -= objA->m_MassData.invMass * impulse;
//										objB->velocity += objB->m_MassData.invMass * impulse;
//									*/
//					}
//					else
//						objA->gameObject->GetComponent<SpriteRenderer>()->SetColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
//				}	
//			}
//		}
//	}
//	{
//		//for (Rigidbody* objA : m_Objects)
//		//{
//		//	if (objA->m_Mass != 0)
//		//	{
//		//		objA->transform->m_Position += objA->velocity * dt;
//		//	}
//
//		//	for (Rigidbody* objB : m_Objects)
//		//	{
//		//		if (objA->m_Mass != 0 && objA != objB && objA->m_Collider->HasCollided(objB->m_Collider.get()))
//		//		{
//		//			if (objB->m_Mass != 0)
//		//			{
//		//				Vector2 rv = objB->velocity - objA->velocity;
//
//		//				Vector2 normal = objB->transform->m_Position;
//		//				normal -= objA->transform->m_Position;
//		//				normal.Normalize();
//
//		//				if (Vector2<float>::Dot(rv, normal) > 0)
//		//					return;
//
//		//				float mass = objA->m_Mass + objB->m_Mass;
//
//		//				Vector2 newAVelocity = (-normal * objB->velocity.Magnitude() * objB->m_Mass) / objA->m_Mass;
//		//				Vector2 newBVelocity = (normal * objA->velocity.Magnitude() * objA->m_Mass) / objB->m_Mass;
//
//		//				/*	std::cout << "A velocity: " << objA->velocity.x << std::endl;
//		//					std::cout << "B velocity: " << objB->velocity.x << std::endl;
//		//					std::cout << "Rv: " << rv.x << std::endl;
//		//					std::cout << "Normal: " << normal.x << std::endl;
//		//					std::cout << "New Velocity A: " << newAVelocity.x << std::endl;
//		//					std::cout << "New Velocity B: " << newBVelocity.x << std::endl;
//		//					system("pause");*/
//
//		//				objA->velocity = newAVelocity;
//		//				objB->velocity = newBVelocity;
//		//			}
//		//			else
//		//			{
//		//				Vector2 rv = objB->velocity - objA->velocity;
//
//		//				Vector2 normal = objB->transform->m_Position;
//		//				normal -= objA->transform->m_Position;
//		//				normal.Normalize();
//
//		//				if (Vector2<float>::Dot(rv, normal) > 0)
//		//					return;
//
//		//				objA->velocity = -objA->velocity;
//		//			}
//		//		}
//		//	}
//		//}
//	}
//}
//
////axis needs to be normalized
//bool OverlapOnAxis(BoxCollider* box1, BoxCollider* box2, Vector2f axis)
//{
//	Vector2f interval1 = GetInterval(box1, axis);
//	Vector2f interval2 = GetInterval(box2, axis);
//
//	return ((interval2.x <= interval1.y) && (interval1.x <= interval2.y));
//}
//
//Vector2f GetInterval(BoxCollider* box, Vector2f axis)
//{
//	Vector2f min /*= box->m_Min*/;
//	Vector2f max /*= box->m_Max*/;
//
//	Vector2f verticies[4] =
//	{ min, min + box->transform->Up() * box->transform->scale.y,
//		min + box->transform->Right() * box->transform->scale.x, max };
//
//	//Vector2<float> verticies[4] =
//	//{ Vector2(min.x, min.y), Vector2(min.x, max.y),
//	//  Vector2(max.x, min.y), Vector2(max.x, max.y) };
//
//	Vector2f result;
//	result = Dot(axis, verticies[0]);
//
//	for (size_t i = 1; i < 4; i++)
//	{
//		float projection = Dot(axis, verticies[i]);
//		if (projection < result.x)
//		{
//			result.x = projection;
//		}
//		if (projection > result.y)
//		{
//			result.y = projection;
//		}
//	}
//
//	return result;
//}
//
//Vector2f GetInterval(BoxCollider* box1, BoxCollider* box2, Vector2f axis)
//{
//	Vector2f min /*= box1->m_Min*/;
//	Vector2f max /*= box1->m_Max*/;
//
//	Vector2f verticies[4] =
//	{ min, min + box1->transform->Up() * box1->transform->scale.y,
//		min + box1->transform->Right() * box1->transform->scale.x, max };
//
//	//Vector2<float> verticies[4] =
//	//{ Vector2(min.x, min.y), Vector2(min.x, max.y),
//	//  Vector2(max.x, min.y), Vector2(max.x, max.y) };
//
//	Vector2f result;
//	result = Dot(axis, verticies[0]);
//
//	for (size_t i = 1; i < 4; i++)
//	{
//		float projection = Dot(axis, verticies[i]);
//		if (projection < result.x)
//		{
//			result.x = projection;
//		}
//		if (projection > result.y)
//		{
//			result.y = projection;
//		}
//	}
//
//	return result;
//}
//
//void PositionalCorrection(Rigidbody* objA, Rigidbody* bojB)
//{
//	const float percent = 0.2f;
//}
//
//float FindAxisLeastPenetration(int& faceIndex, BoxCollider* A, BoxCollider* B)
//{
//	float bestDistance = -FLT_MAX;
//	for (size_t i = 0; i < 4; i++)
//	{
//		Vector2 n = A->m_Normals[i];
//		Vector2 nw = A->transform->GetTransform() * n;
//		nw.Normalize();
//
//		Matrix2x2f bT = B->transform->GetTransform().Transpose();
//		n = bT * nw;
//
//		Vector2f s = B->GetSupport(-n);
//
//		Vector2f v = A->m_Verticies[i];
//		v = A->transform->GetTransform() * v + A->transform->position;
//		v -= B->transform->position;
//		v = bT * v;
//
//		float d = Dot(n, s - v);
//
//		if (d > bestDistance)
//		{
//			bestDistance = d;
//			faceIndex = i;
//		}
//	}
//
//	return bestDistance;
//}
//
//void FindIncidentFace(Vector2f* v, BoxCollider* RefBox, BoxCollider* IncBox, int referenceIndex)
//{
//	Vector2f referenceNormal = RefBox->m_Normals[referenceIndex];
//
//	referenceNormal = RefBox->transform->GetTransform() * referenceNormal;
//	referenceNormal = IncBox->transform->GetTransform().Transpose() * referenceNormal;
//
//	int incidentFace = 0;
//	float minDot = FLT_MAX;
//	for (size_t i = 0; i < 4; i++)
//	{
//		float dot = Dot(referenceNormal, IncBox->m_Normals[i]);
//		if (dot < minDot)
//		{
//			minDot = dot;
//			incidentFace = i;
//		}
//	}
//
//	v[0] = IncBox->transform->GetTransform() * IncBox->m_Verticies[incidentFace] + IncBox->transform->position;
//	incidentFace = incidentFace + 1 >= (4) ? 0 : incidentFace + 1;
//	v[1] = IncBox->transform->GetTransform() * IncBox->m_Verticies[incidentFace] + IncBox->transform->position;
//}
//
//int Clip(Vector2f n, float c, Vector2f* face)
//{
//	int sp = 0;
//	Vector2f out[2] = { face[0], face[1] };
//
//	float d1 = Dot(n, face[0]) - c;
//	float d2 = Dot(n, face[1]) - c;
//
//	if (d1 <= 0.0f) out[sp++] = face[0];
//	if (d2 <= 0.0f) out[sp++] = face[1];
//
//	if (d1 * d2 < 0.0f)
//	{
//		float alpha = d1 / (d1 - d2);
//		out[sp] = face[0] + alpha * (face[1] - face[0]);
//		++sp;
//	}
//
//	face[0] = out[0];
//	face[1] = out[1];
//
//	return sp;
//}
//
//Manifold::Manifold(BoxCollider* a, BoxCollider* b)
//	:A(a), B(b)
//{
//	rigidbodyA = a->transform->gameObject->GetComponent<Rigidbody>();
//	rigidbodyB = b->transform->gameObject->GetComponent<Rigidbody>();
//}
//
//void Manifold::Solve()
//{
//	int faceA;
//	float penetrationA = FindAxisLeastPenetration(faceA, A, B);
//	if (penetrationA >= 0.0f)
//		return;
//
//	int faceB;
//	float penetrationB = FindAxisLeastPenetration(faceB, B, A);
//	if (penetrationB >= 0.0f)
//		return;
//
//	int referenceIndex;
//	bool flip;
//
//	BoxCollider* RefBox;
//	BoxCollider* IncBox;
//
//	if (BiasGreaterThan(penetrationA, penetrationB))
//	{
//		std::cout << "\nName: " << A->transform->gameObject->GetName();
//		std::cout << "\tPenetration: " << penetrationA << std::endl;
//
//		std::cout << "Name: " << B->transform->gameObject->GetName();
//		std::cout << "\tPenetration: " << penetrationB << std::endl;
//
//
//		RefBox = A;
//		IncBox = B;
//		referenceIndex = faceA;
//		flip = false;
//	}
//	else
//	{
//		RefBox = B;
//		IncBox = A;
//		referenceIndex = faceB;
//		flip = true;
//	}
//
//	Vector2f incidentFace[2];
//	FindIncidentFace(incidentFace, RefBox, IncBox, referenceIndex);
//
//	Vector2f v1 = RefBox->m_Verticies[referenceIndex];
//	referenceIndex = referenceIndex + 1 == 4 ? 0 : referenceIndex + 1;
//	Vector2f v2 = RefBox->m_Verticies[referenceIndex];
//
//	v1 = RefBox->transform->GetTransform() * v1 + RefBox->transform->position;
//	v2 = RefBox->transform->GetTransform() * v2 + RefBox->transform->position;
//
//	Vector2f sidePlaneNormal = (v2 - v1);
//	sidePlaneNormal.Normalize();
//
//	Vector2f refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);
//
//	float refC = Dot(refFaceNormal, v1);
//	float negSide = -Dot(sidePlaneNormal, v1);
//	float posSide = Dot(sidePlaneNormal, v2);
//
//	if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
//		return;
//
//	if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
//		return;
//
//	normal = flip ? -refFaceNormal : refFaceNormal;
//
//	int cp = 0;
//	float seperation = Dot(refFaceNormal, incidentFace[0]) - refC;
//	if (seperation <= 0.0f)
//	{
//		contacts[cp] = incidentFace[0];
//		penetration = -seperation;
//		++cp;
//	}
//	else
//	{
//		penetration = 0;
//	}
//
//	seperation = Dot(refFaceNormal, incidentFace[1]) - refC;
//	if (seperation <= 0.0f)
//	{
//		contacts[cp] = incidentFace[1];
//
//		penetration += -seperation;
//		++cp;
//
//		penetration /= float(cp);
//	}
//
//	contactCount = cp;
//}
//
//void Manifold::Initialize()
//{
//	restitution = std::min(A->material.restitution, B->material.restitution);
//	
//	for (size_t i = 0; i < contactCount; i++)
//	{
//		Vector2f ra = contacts[i] - A->transform->position;
//		Vector2f rb = contacts[i] - B->transform->position;
//
//		Vector2f rv = rigidbodyB->velocity + Cross(rigidbodyB->angularVelocity, rb) -
//			rigidbodyA->velocity - Cross(rigidbodyA->angularVelocity, ra);
//	}
//}
//
//void Manifold::ApplyImpulse()
//{
//	if (rigidbodyA->GetMass() + rigidbodyB->GetMass() == 0)
//	{
//		rigidbodyA->velocity = Vector2f(0, 0);
//		rigidbodyB->velocity = Vector2f(0, 0);
//		return;
//	}
//
//	for (size_t i = 0; i < contactCount; ++i)
//	{
//		// Calculate radii from COM to contact
//		Vector2f ra = contacts[i] - A->transform->position;
//		Vector2f rb = contacts[i] - B->transform->position;
//
//		// Relative velocity
//		Vector2f rv = rigidbodyB->velocity + Cross(rigidbodyB->angularVelocity, rb) -
//			rigidbodyA->velocity - Cross(rigidbodyA->angularVelocity, ra);
//
//		// Relative velocity along the normal
//		float contactVel = Dot(rv, normal);
//
//		// Do not resolve if velocities are separating
//		if (contactVel > 0)
//			return;
//
//		float raCrossN = Cross(ra, normal);
//		float rbCrossN = Cross(rb, normal);
//		float invMassSum = rigidbodyA->m_MassData.invMass + rigidbodyB->m_MassData.invMass + 
//			std::pow(raCrossN, 2) * rigidbodyA->m_MassData.invInertia + std::pow(rbCrossN, 2) * rigidbodyB->m_MassData.invInertia;
//
//		// Calculate impulse scalar
//		float j = -(1.0f + restitution) * contactVel;
//		j /= invMassSum;
//		j /= (float)contactCount;
//
//		// Apply impulse
//		Vector2f impulse = normal * j;
//		rigidbodyA->ApplyImpulse(-impulse, ra);
//		rigidbodyB->ApplyImpulse(impulse, rb);
//
//		// Friction impulse
//		rv = rigidbodyB->velocity + Cross(rigidbodyB->angularVelocity, rb) -
//			rigidbodyA->velocity - Cross(rigidbodyA->angularVelocity, ra);
//
//		Vector2f t = rv - (normal * Dot(rv, normal));
//		t.Normalize();
//
//		// j tangent magnitude
//		float jt = -Dot(rv, t);
//		jt /= invMassSum;
//		jt /= (float)contactCount;
//
//		// Don't apply tiny friction impulses
//		if (jt == 0.0f)
//			return;
//
//		// Coulumb's law
//		Vector2f tangentImpulse;
//		if (std::abs(jt) < j * staticFriction)
//			tangentImpulse = t * jt;
//		else
//			tangentImpulse = t * -j * dynamicFriction;
//
//		// Apply friction impulse
//		rigidbodyA->ApplyImpulse(-tangentImpulse, ra);
//		rigidbodyB->ApplyImpulse(tangentImpulse, rb);
//	}
//}
//
//void Manifold::PositionCorrection()
//{
//	const float k_slop = 0.05f; // Penetration allowance
//	const float percent = 0.4f; // Penetration percentage to correct
//	Vector2f correction = (std::max(penetration - k_slop, 0.0f) / (rigidbodyA->m_MassData.invMass + rigidbodyB->m_MassData.invMass)) * normal * percent;
//	A->transform->position -= correction * rigidbodyA->m_MassData.invMass;
//	B->transform->position += correction * rigidbodyB->m_MassData.invMass;
//}

