#include "AnimatorWindow.h"
#include "imgui/imgui.h"

int AnimatorSequencer::GetFrameMin() const
{
	return 0;
}

int AnimatorSequencer::GetFrameMax() const
{
	return 0;
}

int AnimatorSequencer::GetItemCount() const
{
	return 0;
}

void AnimatorWindow::ImGuiRender()
{
	ImGui::Begin("Animator");

	int currentFrame = 0;
	bool expanded = false;
	int selectedEntry = 0;
	int firstFrame = 0;
	//ImSequencer::Sequencer(&m_Sequencer, &currentFrame, &expanded, &selectedEntry, &firstFrame, 0);

	ImGui::End();
}
