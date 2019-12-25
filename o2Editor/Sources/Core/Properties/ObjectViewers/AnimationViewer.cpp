#include "stdafx.h"
#include "AnimationViewer.h"

#include "AnimationWindow/AnimationWindow.h"
#include "Core/EditorScope.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/Widgets/Button.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	AnimationViewer::AnimationViewer() :
		DefaultObjectViewer()
	{
		PushEditorScopeOnStack scope;

		mEditButton = o2UI.CreateButton("Edit", THIS_FUNC(OnEditPressed));
	}

	void AnimationViewer::Refresh(const TargetsVec& targetObjets)
	{
		DefaultObjectViewer::Refresh(targetObjets);

		mLayout->AddChild(mEditButton);
		mEditButton->SetIndexInSiblings(mLayout->GetChildren().Count() - 1);

		mCurrentAnimation = !targetObjets.IsEmpty() ? dynamic_cast<Animation*>(targetObjets[0].first) : nullptr;
	}

	const Type* AnimationViewer::GetViewingObjectType() const
	{
		return GetViewingObjectTypeStatic();
	}

	const Type* AnimationViewer::GetViewingObjectTypeStatic()
	{
		return &TypeOf(Animation);
	}

	void AnimationViewer::OnEditPressed()
	{
		o2EditorAnimationWindow.SetAnimation(mCurrentAnimation);
		o2EditorAnimationWindow.SetTarget(dynamic_cast<Actor*>(o2EditorSceneScreen.GetSelectedObjects().Last()));
	}

}

DECLARE_CLASS(Editor::AnimationViewer);
