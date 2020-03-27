#pragma once
#include "o2/Scene/UI/Widgets/VerticalLayout.h"

using namespace o2;

namespace Editor
{
	class IObjectPropertiesViewer;

	// -----------------------------------------------------------------
	// Basic automatic object viewer. Selects suitable properties viewer
	// -----------------------------------------------------------------
	class ObjectViewer: public VerticalLayout
	{
	public:
		// Default constructor, creates view widget as vertical layout
		ObjectViewer();

		// Refreshing controls and properties by target objects with prototypes
		void Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets);

		// Refreshing controls and properties by target objects
		void Refresh(const Vector<IObject*>& targetObjets);

		// Returns viewing objects type
		const Type* GetViewingObjectType() const;

		SERIALIZABLE(ObjectViewer);

	protected:
		IObjectPropertiesViewer* mPropertiesViewer = nullptr; // Object properties viewer
	};
}

CLASS_BASES_META(Editor::ObjectViewer)
{
	BASE_CLASS(o2::VerticalLayout);
}
END_META;
CLASS_FIELDS_META(Editor::ObjectViewer)
{
	PROTECTED_FIELD(mPropertiesViewer);
}
END_META;
CLASS_METHODS_META(Editor::ObjectViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	PUBLIC_FUNCTION(void, Refresh, _tmp1);
	PUBLIC_FUNCTION(void, Refresh, const Vector<IObject*>&);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
}
END_META;
