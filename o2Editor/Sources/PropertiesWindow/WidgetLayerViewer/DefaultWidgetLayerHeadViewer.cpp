#include "stdafx.h"
#include "DefaultWidgetLayerHeadViewer.h"

#include "Core/Actions/PropertyChange.h"
#include "Core/EditorApplication.h"
#include "Core/Properties/Widgets/BooleanProperty.h"
#include "Core/Properties/Widgets/StringProperty.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	DefaultWidgetLayerHeaderViewer::DefaultWidgetLayerHeaderViewer()
	{
		mDataView = mnew UIWidget();
		mDataView->name = "actor head";
		mDataView->layout->minHeight = 21;

		mEnableProperty = o2UI.CreateWidget<BooleanProperty>("actor head enable");
		*mEnableProperty->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(1, 0));
		mEnableProperty->SetValuePath("enabled");
		mEnableProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mEnableProperty);

		mNameProperty = o2UI.CreateWidget<StringProperty>("actor head name");
		*mNameProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 21, 15, 17, 2);
		mNameProperty->SetValuePath("name");
		mNameProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mNameProperty);

		mLockProperty = o2UI.CreateWidget<BooleanProperty>("actor head lock");
		*mLockProperty->layout = UIWidgetLayout::Based(BaseCorner::RightTop, Vec2F(20, 20), Vec2F(2, -1));
		mLockProperty->SetValuePath("locked");
		mLockProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		mDataView->AddChild(mLockProperty);
	}

	DefaultWidgetLayerHeaderViewer::~DefaultWidgetLayerHeaderViewer()
	{
		if (mEnableProperty)
			delete mEnableProperty;

		if (mNameProperty)
			delete mNameProperty;

		if (mLockProperty)
			delete mLockProperty;

		if (mDataView)
			delete mDataView;
	}

	void DefaultWidgetLayerHeaderViewer::SetTargetLayers(const Vector<UIWidgetLayer*>& layers)
	{
		mLayers = layers;

		Vector<UIWidgetLayer*> prototypes = layers.Select<UIWidgetLayer*>([](UIWidgetLayer* x) { return nullptr; });
// 		auto prototypes = layers.Select<UIWidgetLayer*>([](UIWidgetLayer* x) { 
// 			return x->GetOwnerWidget()->GetPrototypeLink().Get(); });

		mEnableProperty->SelectValueAndPrototypeProperties<UIWidgetLayer, decltype(UIWidgetLayer::enabled)>(
			layers, prototypes, [](UIWidgetLayer* x) { return &x->enabled; });

		mNameProperty->SelectValueAndPrototypePointers<String, UIWidgetLayer>(
			layers, prototypes, [](UIWidgetLayer* x) { return &x->name; });

		mLockProperty->SelectValueAndPrototypeProperties<UIWidgetLayer, decltype(UIWidgetLayer::locked)>(
			layers, prototypes, [](UIWidgetLayer* x) { return &x->locked; });
	}

	UIWidget* DefaultWidgetLayerHeaderViewer::GetWidget() const
	{
		return mDataView;
	}

	void DefaultWidgetLayerHeaderViewer::Refresh()
	{
		mEnableProperty->Refresh();
		mNameProperty->Refresh();
		mLockProperty->Refresh();
	}

	void DefaultWidgetLayerHeaderViewer::OnPropertyChanged(const String& path, const Vector<DataNode>& prevValue, 
														   const Vector<DataNode>& newValue)
	{
		PropertyChangeAction* action = mnew PropertyChangeAction(
			o2EditorSceneScreen.GetSelectedObjects(), path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

}

DECLARE_CLASS(Editor::DefaultWidgetLayerHeaderViewer);