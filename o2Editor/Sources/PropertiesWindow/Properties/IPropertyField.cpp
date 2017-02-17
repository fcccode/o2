#include "IPropertyField.h"

#include "UI/Widget.h"

namespace Editor
{
	void Editor::IPropertyField::SetValuePtr(const Vector<void*>& targets, bool isProperty)
	{
		SetValueAndPrototypePtr(targets.Select<Pair<void*, void*>>([](void* x) { return Pair<void*, void*>(x, nullptr); }), isProperty);
	}
}

CLASS_META(Editor::IPropertyField)
{
	BASE_CLASS(o2::IObject);

	PUBLIC_FIELD(onChanged);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, SetValuePtr, const Vector<void*>&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
}
END_META;
 