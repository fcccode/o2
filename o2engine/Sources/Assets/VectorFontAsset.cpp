#include "VectorFontAsset.h"

#include "Assets/Assets.h"
#include "Render/BitmapFont.h"
#include "Render/Font.h"
#include "Render/Render.h"

namespace o2
{
	VectorFontAsset::MetaInfo::~MetaInfo()
	{
		for (auto eff : mEffects)
			delete eff;
	}

	Type::Id VectorFontAsset::MetaInfo::GetAssetType() const
	{
		return VectorFontAsset::type.ID();
	}

	bool VectorFontAsset::MetaInfo::IsEqual(IMetaInfo* other) const
	{
		if (!IMetaInfo::IsEqual(other))
			return false;

		MetaInfo* otherMeta = (MetaInfo*)other;
		for (auto eff : mEffects)
		{
			bool found = false;
			for (auto otherEff : otherMeta->mEffects)
			{
				if (eff->IsEqual(otherEff))
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
		Asset()
	{
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	VectorFontAsset::VectorFontAsset(const String& path):
		Asset()
	{
		mPath = path;
		mMeta = mnew MetaInfo();
		IdRef() = o2Assets.GetAssetId(path);

		InitializeProperties();

		Load();
	}

	VectorFontAsset::VectorFontAsset(AssetId id)
	{
		mMeta = mnew MetaInfo();
		IdRef() = id;
		mPath = o2Assets.GetAssetPath(id);
		InitializeProperties();

		Load();
	}

	VectorFontAsset::VectorFontAsset(const VectorFontAsset& asset):
		Asset(asset), mFont(asset.mFont)
	{
		mMeta = mnew MetaInfo();
		InitializeProperties();
	}

	VectorFontAsset::~VectorFontAsset()
	{}

	FontRef VectorFontAsset::GetFont() const
	{
		return mFont;
	}

	const VectorFontAsset::EffectsVec& VectorFontAsset::GetEffects() const
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

	VectorFontAsset::MetaInfo* VectorFontAsset::GetMeta() const
	{
		return (MetaInfo*)mMeta;
	}

	void VectorFontAsset::LoadData(const String& path)
	{
		mFont = o2Render.mFonts.FindMatch([&](auto fnt) { return fnt->GetFileName() == path; });

		if (!mFont)
		{
			mFont = mnew VectorFont(path);

			VectorFont* vectorFont = (VectorFont*)mFont.mFont;
			for (auto eff : GetMeta()->mEffects)
				vectorFont->AddEffect(eff->Clone());
		}
	}

	void VectorFontAsset::InitializeProperties()
	{
		INITIALIZE_GETTER(VectorFontAsset, meta, GetMeta);
		INITIALIZE_GETTER(VectorFontAsset, font, GetFont);
	}

	VectorFontAsset& VectorFontAsset::operator=(const VectorFontAsset& asset)
	{
		mFont = asset.mFont;
		return *this;
	}
}