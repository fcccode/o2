#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/DragAndDrop.h"

namespace o2
{
	class Actor;
	class Component;
	class Text;
	class UIButton;
	class UIWidget;
}

namespace Editor
{
	class UIActorsTree;
	class UIAssetsIconsScrollArea;

	// -----------------------------------
	// Editor actor component property box
	// -----------------------------------
	class ComponentProperty: public IPropertyField, public KeyboardEventsListener, public DragDropArea
	{
	public:
		// Default constructor
		ComponentProperty(UIWidget* widget = nullptr);

		// Destructor
		~ComponentProperty();

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Returns value
		Component* GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		// Specializes field type
		void SpecializeType(const Type* type);

		// Returns specialized type
		const Type* GetSpecializedType() const;

		// Sets value actor
		void SetValue(Component* value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		IOBJECT(ComponentProperty);

	protected:
		TargetsVec  mValuesProxies;           // Fields' pointers
		Component*  mCommonValue = nullptr;    // Common field value (if not different)
		bool        mValuesDifferent = true;   // Are values different
		const Type* mComponentType = nullptr;  // Component value type

		UIWidget*   mPropertyWidget = nullptr; // Property widget
		UIButton*   mRevertBtn = nullptr;      // Revert to source prototype button
		UIWidget*   mBox = nullptr;            // Edit box 
		Text*       mNameText = nullptr;       // Component name text

	protected:
		// Reverts target value to source
		void RevertoToPrototype(IAbstractValueProxy* target, IAbstractValueProxy* source, IObject* targetOwner);

		// Sets common value actor
		void SetCommonValue(Component* value);

		// Checks value for reverting to prototype
		void CheckRevertableState() override;

		// It is called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when key was pressed
		void OnKeyPressed(const Input::Key& key) override;

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group) override;

		// It is called when actors tree nodes was dragged and dropped to this
		void OnDroppedFromActorsTree(UIActorsTree* actorsTree);

		// It is called when actors tree nodes was dragged and entered to this
		void OnDragEnterFromActorsTree(UIActorsTree* actorsTree);

		// It is called when actors tree nodes was dragged and exited from this
		void OnDragExitFromActorsTree(UIActorsTree* actorsTree);

		// It is called when assets scroll icons was dragged and dropped to this
		void OnDroppedFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll);

		// It is called when assets scroll icons was dragged and entered to this
		void OnDragEnterFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll);

		// It is called when assets scroll icons was dragged and exited from this
		void OnDragExitFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetValueByUser(Component* value);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}

CLASS_BASES_META(Editor::ComponentProperty)
{
	BASE_CLASS(Editor::IPropertyField);
	BASE_CLASS(o2::KeyboardEventsListener);
	BASE_CLASS(o2::DragDropArea);
}
END_META;
CLASS_FIELDS_META(Editor::ComponentProperty)
{
	PROTECTED_FIELD(mValuesProxies);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mComponentType);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mBox);
	PROTECTED_FIELD(mNameText);
}
END_META;
CLASS_METHODS_META(Editor::ComponentProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(Component*, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PUBLIC_FUNCTION(const Type*, GetSpecializedType);
	PUBLIC_FUNCTION(void, SetValue, Component*);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, RevertoToPrototype, IAbstractValueProxy*, IAbstractValueProxy*, IObject*);
	PROTECTED_FUNCTION(void, SetCommonValue, Component*);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDroppedFromActorsTree, UIActorsTree*);
	PROTECTED_FUNCTION(void, OnDragEnterFromActorsTree, UIActorsTree*);
	PROTECTED_FUNCTION(void, OnDragExitFromActorsTree, UIActorsTree*);
	PROTECTED_FUNCTION(void, OnDroppedFromAssetsScroll, UIAssetsIconsScrollArea*);
	PROTECTED_FUNCTION(void, OnDragEnterFromAssetsScroll, UIAssetsIconsScrollArea*);
	PROTECTED_FUNCTION(void, OnDragExitFromAssetsScroll, UIAssetsIconsScrollArea*);
	PROTECTED_FUNCTION(void, SetValueByUser, Component*);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
