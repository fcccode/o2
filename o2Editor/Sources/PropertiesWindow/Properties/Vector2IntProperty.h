#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/CursorEventsArea.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UIEditBox;
	class UIHorizontalLayout;
	class UIWidget;
}

namespace Editor
{
	// ------------------------------
	// Editor integer vector property
	// ------------------------------
	class Vec2IProperty: public IPropertyField
	{
	public:
		// Default constructor
		Vec2IProperty(UIWidget* widget = nullptr);

		// Destructor
		~Vec2IProperty();

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Sets value
		void SetValue(const Vec2I& value);

		// Sets value X
		void SetValueX(int value);

		// Sets value Y
		void SetValueY(int value);

		// Sets value as unknown
		void SetUnknownValue(const Vec2I& defaultValue = Vec2I());

		// Sets value X as unknown
		void SetXUnknownValue(int defaultValue = 0);

		// Sets value Y as unknown
		void SetYUnknownValue(int defaultValue = 0);

		// Returns value
		Vec2I GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		IOBJECT(Vec2IProperty);

	protected:
		TargetsVec mValuesProxies;            // Fields' pointers
		Vec2I      mCommonValue;              // Common field value (if not different)
		bool       mXValuesDifferent = true;  // Are left values different
		bool       mYValuesDifferent = true;  // Are bottom values different\
				 								
		UIWidget*  mPropertyWidget = nullptr; // Property root widget, contains editboxes and revert button
		UIButton*  mRevertBtn = nullptr;      // Revert to prototype button
		UIEditBox* mXEditBox = nullptr;       // X Edit box 
		UIEditBox* mYEditBox = nullptr;       // Y Edit box 

		CursorEventsArea mXDragHangle; // X Value changing drag handle
		CursorEventsArea mYDragHangle; // Y Value changing drag handle

	protected:
		// Sets common value
		void SetCommonValue(const Vec2I& value);

		// Sets common value X
		void SetCommonValueX(int value);

		// Sets common value Y
		void SetCommonValueY(int value);

		// Checks value for reverting to prototype
		void CheckRevertableState();

		// X Edit box change event
		void OnXEdited(const WString& data);

		// Y Edit box change event
		void OnYEdited(const WString& data);

		// It is called when key was released
		void OnKeyReleased(const Input::Key& key);

		// It is called when drag handle was moved and changes the property value
		void OnXDragHandleMoved(const Input::Cursor& cursor);

		// It is called when drag handle was moved and changes the property value
		void OnYDragHandleMoved(const Input::Cursor& cursor);

		// It is called when change value move handle pressed, sets cursor infinite mode and stores value to data
		void OnXMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change value move handle pressed, sets cursor infinite mode and stores value to data
		void OnYMoveHandlePressed(const Input::Cursor& cursor);

		// It is called when change value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnXMoveHandleReleased(const Input::Cursor& cursor);

		// It is called when change value move handle  released, turns off cursor infinite mode, 
		// checks value was changed then calls value change completed event
		void OnYMoveHandleReleased(const Input::Cursor& cursor);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetXValueByUser(int value);

		// Sets value, checks value changed, calls onChangeCompleted
		void SetYValueByUser(int value);

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();
	};
}

CLASS_BASES_META(Editor::Vec2IProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::Vec2IProperty)
{
	PROTECTED_FIELD(mValuesProxies);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mXValuesDifferent);
	PROTECTED_FIELD(mYValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mXEditBox);
	PROTECTED_FIELD(mYEditBox);
	PROTECTED_FIELD(mXDragHangle);
	PROTECTED_FIELD(mYDragHangle);
}
END_META;
CLASS_METHODS_META(Editor::Vec2IProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(void, SetValue, const Vec2I&);
	PUBLIC_FUNCTION(void, SetValueX, int);
	PUBLIC_FUNCTION(void, SetValueY, int);
	PUBLIC_FUNCTION(void, SetUnknownValue, const Vec2I&);
	PUBLIC_FUNCTION(void, SetXUnknownValue, int);
	PUBLIC_FUNCTION(void, SetYUnknownValue, int);
	PUBLIC_FUNCTION(Vec2I, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const Vec2I&);
	PROTECTED_FUNCTION(void, SetCommonValueX, int);
	PROTECTED_FUNCTION(void, SetCommonValueY, int);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnXEdited, const WString&);
	PROTECTED_FUNCTION(void, OnYEdited, const WString&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
	PROTECTED_FUNCTION(void, OnXDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnYDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnXMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnYMoveHandlePressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnXMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnYMoveHandleReleased, const Input::Cursor&);
	PROTECTED_FUNCTION(void, SetXValueByUser, int);
	PROTECTED_FUNCTION(void, SetYValueByUser, int);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
}
END_META;
