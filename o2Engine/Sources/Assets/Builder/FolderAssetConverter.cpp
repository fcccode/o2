#include "FolderAssetConverter.h"

#include "Assets/Assets.h"
#include "Assets/BinaryAsset.h"
#include "Assets/FolderAsset.h"
#include "Utils/FileSystem/FileSystem.h"

namespace o2
{
	Vector<TypeId> FolderAssetConverter::GetProcessingAssetsTypes() const
	{
		Vector<TypeId> res;
		res.Add(TypeOf(FolderAsset).ID());
		return res;
	}

	void FolderAssetConverter::ConvertAsset(const AssetTree::AssetNode& node)
	{
		String sourceAssetPath = o2Assets.GetAssetsPath() + node.mPath;
		String buildedAssetPath = o2Assets.GetDataPath() + node.mPath;
		String sourceAssetMetaPath = sourceAssetPath + ".meta";
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FolderCreate(buildedAssetPath);
		o2FileSystem.FileCopy(sourceAssetMetaPath, buildedAssetMetaPath);
	}

	void FolderAssetConverter::RemoveAsset(const AssetTree::AssetNode& node)
	{
		String buildedAssetPath = o2Assets.GetDataPath() + node.mPath;
		String buildedAssetMetaPath = buildedAssetPath + ".meta";

		o2FileSystem.FolderRemove(buildedAssetPath);
		o2FileSystem.FileDelete(buildedAssetMetaPath);
	}

	void FolderAssetConverter::MoveAsset(const AssetTree::AssetNode& nodeFrom, const AssetTree::AssetNode& nodeTo)
	{
		String fullPathFrom = o2Assets.GetDataPath() + nodeFrom.mPath;
		String fullPathTo = o2Assets.GetDataPath() + nodeTo.mPath;
		String fullMetaPathFrom = fullPathFrom + ".meta";
		String fullMetaPathTo = fullPathTo + ".meta";

		o2FileSystem.FolderCreate(fullPathTo);
		o2FileSystem.FileMove(fullMetaPathFrom, fullMetaPathTo);

		mRemovedFolders.Add(fullPathFrom);
		mRemovedFolders.Remove(fullPathTo);
	}

	Vector<UID> FolderAssetConverter::AssetsPostProcess()
	{
		for (auto fold : mRemovedFolders)
			o2FileSystem.FolderRemove(fold);

		return Vector<UID>();
	}

	void FolderAssetConverter::Reset()
	{
		mRemovedFolders.Clear();
	}
}
 
CLASS_META(o2::FolderAssetConverter)
{
	BASE_CLASS(o2::IAssetConverter);

	PROTECTED_FIELD(mRemovedFolders);

	PUBLIC_FUNCTION(Vector<TypeId>, GetProcessingAssetsTypes);
	PUBLIC_FUNCTION(void, ConvertAsset, const AssetTree::AssetNode&);
	PUBLIC_FUNCTION(void, RemoveAsset, const AssetTree::AssetNode&);
	PUBLIC_FUNCTION(void, MoveAsset, const AssetTree::AssetNode&, const AssetTree::AssetNode&);
	PUBLIC_FUNCTION(Vector<UID>, AssetsPostProcess);
	PUBLIC_FUNCTION(void, Reset);
}
END_META;
 
