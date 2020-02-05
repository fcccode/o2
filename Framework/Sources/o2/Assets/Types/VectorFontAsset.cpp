#include "o2/stdafx.h"
#include "VectorFontAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Render/BitmapFont.h"
#include "o2/Render/Font.h"
#include "o2/Render/Render.h"

namespace o2
{
	VectorFontAsset::Meta::~Meta()
	{
		for (auto eff : mEffects)
			delete eff;
	}

	bool VectorFontAsset::Meta::IsEqual(AssetMeta* other) const
	{
		if (!AssetMeta::IsEqual(other))
			return false;

		Meta* otherMeta = (Meta*)other;
		for (auto eff : mEffects)
		{
			bool found = false;
			for (auto otherEff : otherMeta->mEffects)
			{
				if (eff && eff->IsEqual(otherEff))
				{
					found = true;
					break;
				}
			}

			if (!found)
				return false;
		}

		return true;
	}

	VectorFontAsset::VectorFontAsset():
		FontAsset(mnew Meta())
	{}

	VectorFontAsset::VectorFontAsset(const VectorFontAsset& asset):
		FontAsset(asset), meta(this)
	{}

	VectorFontAsset& VectorFontAsset::operator=(const VectorFontAsset& asset)
	{
		FontAsset::operator=(asset);
		mFont = asset.mFont;
		return *this;
	}

	const Vector<VectorFont::Effect*>& VectorFontAsset::GetEffects() const
	{
		return GetMeta()->mEffects;
	}

	void VectorFontAsset::AddEffect(VectorFont::Effect* effect)
	{
		GetMeta()->mEffects.Add(effect);
		((VectorFont*)mFont.mFont)->AddEffect(effect);
	}

	void VectorFontAsset::RemoveEffect(VectorFont::Effect* effect)
	{
		GetMeta()->mEffects.Remove(effect);
		((VectorFont*)mFont.mFont)->RemoveEffect(effect);
	}

	void VectorFontAsset::RemoveAllEffects()
	{
		for (auto eff : GetMeta()->mEffects)
			delete eff;

		GetMeta()->mEffects.Clear();

		((VectorFont*)mFont.mFont)->RemoveAllEffects();
	}

	const char* VectorFontAsset::GetFileExtensions() const
	{
		return "ttf";
	}

	VectorFontAsset::Meta* VectorFontAsset::GetMeta() const
	{
		return (Meta*)mInfo.meta;
	}

	void VectorFontAsset::LoadData(const String& path)
	{
		mFont = o2Render.mFonts.FindMatch([&](auto fnt) { return fnt->GetFileName() == path; });

		if (!mFont)
		{
			mFont = mnew VectorFont(path);

			VectorFont* vectorFont = (VectorFont*)mFont.mFont;
			for (auto eff : GetMeta()->mEffects)
			{
				if (eff)
					vectorFont->AddEffect(eff->CloneAs<VectorFont::Effect>());
			}
		}
	}
}

DECLARE_CLASS_MANUAL(o2::DefaultAssetMeta<o2::VectorFontAsset>);

DECLARE_CLASS(o2::VectorFontAsset);

DECLARE_CLASS(o2::VectorFontAsset::Meta);