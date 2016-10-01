#include "UIAssetIcon.h"

#include "Render/Text.h"
#include "Utils/FileSystem/FileSystem.h"

namespace Editor
{
	UIAssetIcon::UIAssetIcon():
		UIWidget(), mNameText(nullptr)
	{
		RetargetStatesAnimations();
	}

	UIAssetIcon::UIAssetIcon(const UIAssetIcon& other):
		UIWidget(other)
	{
		mNameText = GetLayerDrawable<Text>("assetName");

		RetargetStatesAnimations();
		UpdateLayout();
	}

	UIAssetIcon::~UIAssetIcon()
	{}

	UIAssetIcon& UIAssetIcon::operator=(const UIAssetIcon& other)
	{
		UIWidget::operator=(other);
		mNameText = GetLayerDrawable<Text>("assetName");

		return *this;
	}

	void UIAssetIcon::SetAssetInfo(const AssetInfo& info)
	{
		mAssetInfo = info;

		if (mNameText)
			mNameText->text = o2FileSystem.GetPathWithoutDirectories(info.mPath);
	}

	const AssetInfo& UIAssetIcon::GetAssetInfo() const
	{
		return mAssetInfo;
	}

}
 
CLASS_META(Editor::UIAssetIcon)
{
	BASE_CLASS(o2::UIWidget);

	PROTECTED_FIELD(mNameText);
	PROTECTED_FIELD(mAssetInfo);

	PUBLIC_FUNCTION(void, SetAssetInfo, const AssetInfo&);
	PUBLIC_FUNCTION(const AssetInfo&, GetAssetInfo);
}
END_META;
 