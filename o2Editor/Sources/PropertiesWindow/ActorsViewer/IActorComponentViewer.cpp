#include "IActorComponentViewer.h"

#include "UI/Widget.h"
#include "UI/VerticalLayout.h"

namespace Editor
{
	IActorComponentViewer::IActorComponentViewer()
	{
		auto layout = mnew UIVerticalLayout();
		mDataView = layout;

		layout->spacing = 5.0f;
		layout->border = RectF();
		layout->expandHeight = false;
		layout->expandWidth = true;
		layout->fitByChildren = true;
		layout->baseCorner = BaseCorner::Top;
		layout->layout = UIWidgetLayout::BothStretch();
	}

	IActorComponentViewer::~IActorComponentViewer()
	{
		delete mDataView;
	}

	UIWidget* IActorComponentViewer::GetWidget() const
	{
		return mDataView;
	}

	void IActorComponentViewer::Expand()
	{

	}

	void IActorComponentViewer::Collapse()
	{

	}

}
 
CLASS_META(Editor::IActorComponentViewer)
{
	BASE_CLASS(o2::IObject);

	PROTECTED_FIELD(mDataView);

	PUBLIC_FUNCTION(void, SetTargetComponents, const Vector<Component*>&);
	PUBLIC_FUNCTION(const Type*, GetComponentType);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
}
END_META;
 