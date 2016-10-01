#include "DefaultActorHeaderViewer.h"

#include "UI/Widget.h"

namespace Editor
{
	DefaultActorHeaderViewer::DefaultActorHeaderViewer()
	{
		mDataView = mnew UIWidget();
	}

	DefaultActorHeaderViewer::~DefaultActorHeaderViewer()
	{
		delete mDataView;
	}

	void DefaultActorHeaderViewer::SetTargetActors(const Vector<Actor*>& actors)
	{

	}

	UIWidget* DefaultActorHeaderViewer::GetWidget() const
	{
		return mDataView;
	}
}
 
CLASS_META(Editor::DefaultActorHeaderViewer)
{
	BASE_CLASS(Editor::IActorHeaderViewer);

	PUBLIC_FIELD(mDataView);

	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
}
END_META;
 