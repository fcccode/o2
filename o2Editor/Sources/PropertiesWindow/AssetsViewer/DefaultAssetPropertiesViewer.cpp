#include "stdafx.h"
#include "DefaultAssetPropertiesViewer.h"

#include "Core/EditorScope.h"
#include "Core/Properties/Properties.h"
#include "Scene/UI/Widgets/VerticalLayout.h"
#include "Scene/UI/WidgetLayout.h"

namespace Editor
{
	DefaultAssetPropertiesViewer::DefaultAssetPropertiesViewer()
	{
		PushScopeEnterOnStack scope;

		mPropertiesLayout = mnew UIVerticalLayout();
		mPropertiesLayout->name = "properties";
		mPropertiesLayout->spacing = 5.0f;
		mPropertiesLayout->border = BorderF(5, 5, 5, 5);
		mPropertiesLayout->expandHeight = false;
		mPropertiesLayout->expandWidth = true;
		mPropertiesLayout->fitByChildren = true;
		mPropertiesLayout->baseCorner = BaseCorner::Top;
		*mPropertiesLayout->layout = UIWidgetLayout::HorStretch(VerAlign::Top, 0, 0, 10);
	}

	DefaultAssetPropertiesViewer::~DefaultAssetPropertiesViewer()
	{
		delete mPropertiesLayout;
	}

	void DefaultAssetPropertiesViewer::SetTargetAssets(const Vector<AssetRef*>& assets)
	{
		mTargetAssets = assets;
		mFieldProperties.Set(assets.Select<IObject*>([](auto x) { return (IObject*)(*x)->GetMeta(); }));
	}

	const Type* DefaultAssetPropertiesViewer::GetAssetType() const
	{
		return mAssetType;
	}

	UIWidget* DefaultAssetPropertiesViewer::GetWidget() const
	{
		return mPropertiesLayout;
	}

	void DefaultAssetPropertiesViewer::SpecializeAssetType(const Type* type)
	{
		PushScopeEnterOnStack scope;

		mAssetType = type;
		o2EditorProperties.BuildObjectProperties((UIVerticalLayout*)mPropertiesLayout, type, mFieldProperties, "");

		mPropertiesLayout->name = "asset properties " + type->GetName();
	}

}

DECLARE_CLASS(Editor::DefaultAssetPropertiesViewer);
