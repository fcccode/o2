#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UIEditBox;
	class UIWidget;
}

namespace Editor
{
	// -------------------------------
	// Editor string property edit box
	// -------------------------------
	class StringProperty: public IPropertyField
	{
	public:
		// Default constructor
		StringProperty(UIWidget* widget = nullptr);

		// Destructor
		~StringProperty();

		// Sets fields
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		String GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(const String& value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(StringProperty);

	protected:
		Function<void(void*, const String&)> mAssignFunc; // Value assign function
		Function<String(void*)>              mGetFunc;    // Get value function

		TargetsVec mValuesPointers;  // Fields' pointers
		String     mCommonValue;     // Common field value (if not different)
		bool       mValuesDifferent; // Are values different

		UIWidget*  mPropertyWidget = nullptr; // Property root widget, contains editbox and revert button
		UIEditBox* mEditBox = nullptr;        // Edit box 
		UIButton*  mRevertBtn = nullptr;      // Revert to prototype button

	protected:
		// Sets common value
		void SetCommonValue(const String& value);

		// Edit box change event
		void OnEdited(const WString& data);
	};
}
 