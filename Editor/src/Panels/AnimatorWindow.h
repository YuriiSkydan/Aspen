#pragma once
#include "Layer.h"
#include "ImGuizmo/ImSequencer.h"

class AnimatorSequencer : public ImSequencer::SequenceInterface
{
private:


public:
	int GetFrameMin() const override;
	int GetFrameMax() const override;
	int GetItemCount() const override;

	 void BeginEdit(int /*index*/) override {}
	 void EndEdit() override {}
	 int GetItemTypeCount() const override { return 0; }
	 const char* GetItemTypeName(int /*typeIndex*/) const  override { return ""; }
	 const char* GetItemLabel(int /*index*/) const override { return ""; }
	 const char* GetCollapseFmt() const override { return "%d Frames / %d entries"; }

	void Get(int index, int** start, int** end, int* type, unsigned int* color) override {};
	void Add(int /*type*/) override {}
	void Del(int /*index*/) override {}
	void Duplicate(int /*index*/) override {}

	void Copy() override {}
	void Paste() override {}

	size_t GetCustomHeight(int /*index*/) override { return 0; }
	void DoubleClick(int /*index*/) override {}
	void CustomDraw(int /*index*/, ImDrawList* /*draw_list*/, const ImRect& /*rc*/, const ImRect& /*legendRect*/, const ImRect& /*clippingRect*/, const ImRect& /*legendClippingRect*/) override {}
	void CustomDrawCompact(int /*index*/, ImDrawList* /*draw_list*/, const ImRect& /*rc*/, const ImRect& /*clippingRect*/) override {}

};

class AnimatorWindow : public Layer
{
private:
	AnimatorSequencer m_Sequencer;

public:
	void ImGuiRender() override;
};

