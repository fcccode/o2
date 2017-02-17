#pragma once

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UIToggle;
	class UIWidget;
}

namespace Editor
{
	// -----------------------
	// Editor boolean property
	// -----------------------
	class BooleanProperty: public IPropertyField
	{
	public:
		// Default constructor
		BooleanProperty(UIWidget* widget = nullptr);

		// Destructor
		~BooleanProperty();

		// Sets fields
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		bool GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Sets value
		void SetValue(bool value);

		// Sets value as unknown
		void SetUnknownValue(bool defaultValue = false);

		// Returns editing by this field type
		const Type* GetFieldType() const;

		IOBJECT(BooleanProperty);

	protected:
		Function<void(void*, const bool&)> mAssignFunc; // Value assign function
		Function<bool(void*)>              mGetFunc;    // Get value function

		TargetsVec mValuesPointers;           // Fields' pointers
		bool       mCommonValue;              // Common field value (if not different)
		bool       mValuesDifferent;          // Are values different

		UIWidget*  mPropertyWidget = nullptr; // Property root widget
		UIButton*  mRevertBtn = nullptr;      // Property revert button to prototype source
		UIToggle*  mToggle = nullptr;         // Toggle 

	protected:
		// Sets value
		void SetCommonValue(bool value);

		// Edit box change event
		void OnEdited(bool value);
	};
}
 