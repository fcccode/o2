#include "stdafx.h"
#include "IActorComponentViewer.h"

#include "Core/EditorScope.h"
#include "Core/UI/SpoilerWithHead.h"
#include "Scene/UI/Widgets/Image.h"
#include "Scene/UI/UIManager.h"

namespace Editor
{
	IActorComponentViewer::IActorComponentViewer()
	{
		PushScopeEnterOnStack scope;

		mSpoiler = o2UI.CreateWidget<SpoilerWithHead>();

		mSpoiler->expandHeight = false;
		mSpoiler->expandWidth = true;
		mSpoiler->fitByChildren = true;
		mSpoiler->borderBottom = 5;
		mSpoiler->borderRight = 5;
		mSpoiler->SetCaption("Transform");
		mSpoiler->GetIcon()->SetImageName("ui/UI4_big_file_icon.png");

		mSpoiler->SetExpanded(true);
	}

	IActorComponentViewer::~IActorComponentViewer()
	{
		if (mSpoiler)
			delete mSpoiler;
	}

	Widget* IActorComponentViewer::GetWidget() const
	{
		return mSpoiler;
	}

	void IActorComponentViewer::Expand()
	{
		mSpoiler->Expand();
	}

	void IActorComponentViewer::Collapse()
	{
		mSpoiler->Collapse();
	}

	void IActorComponentViewer::Refresh()
	{}

	void IActorComponentViewer::Rebuild()
	{}

	bool IActorComponentViewer::IsBuiltWithEmpty() const
	{
		return false;
	}

}

DECLARE_CLASS(Editor::IActorComponentViewer);
