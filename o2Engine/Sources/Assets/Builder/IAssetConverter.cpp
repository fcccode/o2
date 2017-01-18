#include "IAssetConverter.h"

#include "Assets/Builder/AssetsBuilder.h"

namespace o2
{
	Vector<TypeId> IAssetConverter::GetProcessingAssetsTypes() const
	{
		return Vector<TypeId>();
	}

	void IAssetConverter::ConvertAsset(const AssetTree::AssetNode& node)
	{}

	void IAssetConverter::RemoveAsset(const AssetTree::AssetNode& node)
	{}

	void IAssetConverter::MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo)
	{}

	Vector<UID> IAssetConverter::AssetsPostProcess()
	{
		return Vector<UID>();
	}

	void IAssetConverter::Reset()
	{}

	void IAssetConverter::SetAssetsBuilder(AssetsBuilder* builder)
	{
		mAssetsBuilder = builder;
	}

}
 
CLASS_META(o2::IAssetConverter)
{
	BASE_CLASS(o2::IObject);

	PROTECTED_FIELD(mAssetsBuilder);

	PUBLIC_FUNCTION(Vector<TypeId>, GetProcessingAssetsTypes);
	PUBLIC_FUNCTION(void, ConvertAsset, const AssetTree::AssetNode&);
	PUBLIC_FUNCTION(void, RemoveAsset, const AssetTree::AssetNode&);
	PUBLIC_FUNCTION(void, MoveAsset, const AssetTree::AssetNode&, const AssetTree::AssetNode&);
	PUBLIC_FUNCTION(Vector<UID>, AssetsPostProcess);
	PUBLIC_FUNCTION(void, Reset);
	PUBLIC_FUNCTION(void, SetAssetsBuilder, AssetsBuilder*);
}
END_META;
 
