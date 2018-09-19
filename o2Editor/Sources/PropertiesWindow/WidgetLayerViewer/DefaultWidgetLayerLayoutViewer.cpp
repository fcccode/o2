#include "stdafx.h"
#include "DefaultWidgetLayerLayoutViewer.h"

#include "Core/Actions/PropertyChange.h"
#include "Core/EditorApplication.h"
#include "Core/Properties/Widgets/Vector2FloatProperty.h"
#include "Core/UI/SpoilerWithHead.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Image.h"
#include "UI/Label.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "Utils/Math/Layout.h"

namespace Editor
{

	DefaultWidgetLayerLayoutViewer::DefaultWidgetLayerLayoutViewer()
	{
		auto rightTopAnchorPropertyContainer = mnew UIWidget();
		rightTopAnchorPropertyContainer->name = "right top anchor";
		rightTopAnchorPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(rightTopAnchorPropertyContainer);

		auto anchorIcon = o2UI.CreateImage("ui/UI3_icon_anchor.png");
		*anchorIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		rightTopAnchorPropertyContainer->AddChild(anchorIcon);

		mAnchorRightTopProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mAnchorRightTopProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mAnchorRightTopProperty->GetChildByType<UILabel>("layout/properties/x label")->text = "R";
		mAnchorRightTopProperty->GetChildByType<UILabel>("layout/properties/y label")->text = "T";
		mAnchorRightTopProperty->SetValuePath("layout/anchorMax");
		mAnchorRightTopProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		rightTopAnchorPropertyContainer->AddChild(mAnchorRightTopProperty);

		// Left bottom
		auto leftBottomAnchorPropertyContainer = mnew UIWidget();
		leftBottomAnchorPropertyContainer->name = "left bottom anchor";
		leftBottomAnchorPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(leftBottomAnchorPropertyContainer);

		mAnchorLeftBottomProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mAnchorLeftBottomProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mAnchorLeftBottomProperty->GetChildByType<UILabel>("layout/properties/x label")->text = "L";
		mAnchorLeftBottomProperty->GetChildByType<UILabel>("layout/properties/y label")->text = "B";
		mAnchorLeftBottomProperty->SetValuePath("layout/anchorMin");
		mAnchorLeftBottomProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		leftBottomAnchorPropertyContainer->AddChild(mAnchorLeftBottomProperty);

		// Offsets
		// Right top
		auto rightTopOffsetPropertyContainer = mnew UIWidget();
		rightTopOffsetPropertyContainer->name = "right top offset";
		rightTopOffsetPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(rightTopOffsetPropertyContainer);

		auto offsetIcon = o2UI.CreateImage("ui/UI3_icon_offsets.png");
		*offsetIcon->layout = UIWidgetLayout::Based(BaseCorner::LeftTop, Vec2F(20, 20), Vec2F(0, 0));
		rightTopOffsetPropertyContainer->AddChild(offsetIcon);

		mOffsetRightTopProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mOffsetRightTopProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mOffsetRightTopProperty->GetChildByType<UILabel>("layout/properties/x label")->text = "R";
		mOffsetRightTopProperty->GetChildByType<UILabel>("layout/properties/y label")->text = "T";
		mOffsetRightTopProperty->SetValuePath("layout/offsetMax");
		mOffsetRightTopProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		rightTopOffsetPropertyContainer->AddChild(mOffsetRightTopProperty);

		// Left bottom
		auto leftBottomOffsetPropertyContainer = mnew UIWidget();
		leftBottomOffsetPropertyContainer->name = "left bottom offset";
		leftBottomOffsetPropertyContainer->layout->minHeight = 20;
		mSpoiler->AddChild(leftBottomOffsetPropertyContainer);

		mOffsetLeftBottomProperty = o2UI.CreateWidget<Vec2FProperty>("colored");
		*mOffsetLeftBottomProperty->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 20, 0, 20, 0);
		mOffsetLeftBottomProperty->GetChildByType<UILabel>("layout/properties/x label")->text = "L";
		mOffsetLeftBottomProperty->GetChildByType<UILabel>("layout/properties/y label")->text = "B";
		mOffsetLeftBottomProperty->SetValuePath("layout/offsetMin");
		mOffsetLeftBottomProperty->onChangeCompleted = THIS_FUNC(OnPropertyChanged);
		leftBottomOffsetPropertyContainer->AddChild(mOffsetLeftBottomProperty);
	}

	DefaultWidgetLayerLayoutViewer::~DefaultWidgetLayerLayoutViewer()
	{
		delete mSpoiler;
	}

	void DefaultWidgetLayerLayoutViewer::SetTargetLayers(const Vector<UIWidgetLayer*>& layers)
	{
		mLayers = layers;

		auto prototypes = layers.Select<UIWidgetLayer*>([](UIWidgetLayer* x) { return nullptr; });

		mAnchorRightTopProperty->SelectValueAndPrototypePointers<Vec2F, UIWidgetLayer>(
			layers, prototypes, [](UIWidgetLayer* x) { return &x->layout.anchorMax; });

		mAnchorLeftBottomProperty->SelectValueAndPrototypePointers<Vec2F, UIWidgetLayer>(
			layers, prototypes, [](UIWidgetLayer* x) { return &x->layout.anchorMin; });

		mOffsetRightTopProperty->SelectValueAndPrototypePointers<Vec2F, UIWidgetLayer>(
			layers, prototypes, [](UIWidgetLayer* x) { return &x->layout.offsetMax; });

		mOffsetLeftBottomProperty->SelectValueAndPrototypePointers<Vec2F, UIWidgetLayer>(
			layers, prototypes, [](UIWidgetLayer* x) { return &x->layout.offsetMin; });
	}

	void DefaultWidgetLayerLayoutViewer::Refresh()
	{
		mAnchorRightTopProperty->Refresh();
		mAnchorLeftBottomProperty->Refresh();
		mOffsetRightTopProperty->Refresh();
		mOffsetLeftBottomProperty->Refresh();
	}

	void DefaultWidgetLayerLayoutViewer::OnPropertyChanged(const String& path, const Vector<DataNode>& prevValue, 
														   const Vector<DataNode>& newValue)
	{
		PropertyChangeAction* action = mnew PropertyChangeAction(
			o2EditorSceneScreen.GetSelectedObjects(), path, prevValue, newValue);

		o2EditorApplication.DoneAction(action);
	}

}

DECLARE_CLASS(Editor::DefaultWidgetLayerLayoutViewer);