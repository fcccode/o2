#include "AssetProperty.h"
 
CLASS_META(Editor::IAssetProperty)
{
	BASE_CLASS(Editor::IPropertyField);
	BASE_CLASS(o2::DrawableCursorEventsListener);
	BASE_CLASS(o2::KeyboardEventsListener);


	PUBLIC_FUNCTION(void, SetAssetId, AssetId);
}
END_META;
 