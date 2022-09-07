#pragma once
#include "HierarchyPanel.h"

#ifndef IMGUI_API
#undef IMGUI_API
#endif

#define IMGUI_API __declspec(dllimport)

#include "imgui/imgui.h"

class AnimationClip;
class Material;

enum ComponentPropertyFlags : int8_t
{
	None      = 0,
	NoDisable = 1 << 0,
	NoRemove  = 1 << 2,
	NoReset   = 1 << 3
};

ComponentPropertyFlags operator|(ComponentPropertyFlags a, ComponentPropertyFlags b);
ComponentPropertyFlags operator&(ComponentPropertyFlags a, ComponentPropertyFlags b);

ComponentPropertyFlags operator|=(ComponentPropertyFlags& a, ComponentPropertyFlags b);
ComponentPropertyFlags operator&=(ComponentPropertyFlags& a, ComponentPropertyFlags b);

class Inspector
{
private:
	Ptr<GameObject>& m_SelectedGameObject;

	AnimationClip* m_ChoosenClip = nullptr;

	unsigned int m_FirstCollumnWidth = 110;
	unsigned int m_SecondCollumnWidth;
	unsigned int m_ItemWidth;

	bool m_TagsAndLayersManager = false;
private:
	template<typename ComponentType>
	void DragFloat(std::string_view id, ComponentType* component, float previewValue, void(ComponentType::* function)(float), float speed = 1.0f, float min = FLT_MIN, float max = FLT_MAX)
	{
		if (ImGui::DragFloat(id.data(), &previewValue, speed, min, max))
			(component->*function)(previewValue);
	}

	template<typename ComponentType>
	void DragFloat2(std::string_view id, ComponentType* component, Vector2f previewValues, void(ComponentType::* function)(const Vector2f&), float speed = 1.0f, float min = FLT_MIN, float max = FLT_MAX)
	{
		if (ImGui::DragFloat2(id.data(), (float*)&previewValues, speed, min, max))
			(component->*function)(previewValues);
	}

	template<typename ComponentType>
	void Checkbox(std::string_view id, ComponentType* component, bool previewValue, void(ComponentType::* function)(bool value))
	{
		if (ImGui::Checkbox(id.data(), &previewValue))
			(component->*function)(previewValue);
	}

	void RenderMaterial(unsigned int collumnIndex, Material* material = nullptr);

	void RenderGameObject();
	void RenderGameObjectProperties();
	void RenderComponents();

	template<typename... Components>
	void RenderComponents(Component* component)
	{
		bool hasDrown = false;

		([&]()
			{
				if (!hasDrown)
				{
					Components* castComponent = dynamic_cast<Components*>(component);
					if (castComponent != nullptr)
					{
						RenderComponent(castComponent);
						ImGui::Separator();
						hasDrown = true;
					}
				}
			}(), ...);
	}

	bool RenderComponentHeader(const std::string& componentName, Component* component, ComponentPropertyFlags flags = None);
	void RenderComponent(Transform* transform);
	void RenderComponent(SpriteRenderer* spriteRenderer);
	void RenderComponent(Camera* camera);
	void RenderComponent(Rigidbody* rigidbody);
	void RenderComponent(Collider* collider);
	void RenderComponent(BoxCollider* boxCollider);
	void RenderComponent(CircleCollider* circleCollider);
	void RenderComponent(PolygonCollider* polygonCollider);
	void RenderComponent(Animator* animator);
	void RenderComponent(AudioSource* audioSource);
	void RenderComponent(AudioListener* audioListener);
	void RenderComponent(Script* script);

	void RenderAddComponentButton();
	void RenderTagsAndLayersManager();
public:
	Inspector(Ptr<GameObject>& gameObjectRef);
	void ImGuiRender();
};
