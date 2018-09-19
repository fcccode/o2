#pragma once

#include "Core/Properties/FieldPropertiesInfo.h"
#include "Core/Properties/IPropertyField.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class UIButton;
	class UIContextMenu;
	class UILabel;
	class UISpoiler;
	class UIWidget;
}

namespace Editor
{
	// ------------------------------
	// Editor object pointer property
	// ------------------------------
	class ObjectPtrProperty: public IPropertyField
	{
	public:
		// Default constructor
		ObjectPtrProperty();

		// Copy constructor
		ObjectPtrProperty(const ObjectPtrProperty& other);

		// Copy operator
		ObjectPtrProperty& operator=(const ObjectPtrProperty& other);

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		// Specializes field type. Just storing type, but not creating fields
		void SpecializeType(const Type* type) override;

		// Returns specialized type
		const Type* GetSpecializedType() const override;

		// Sets property caption
		void SetCaption(const WString& text) override;

		// Returns property caption
		WString GetCaption() const override;

		// Expands property fields
		void Expand();

		// Collapses property field
		void Collapse();

		// Sets properties expanding
		void SetExpanded(bool expanded);

		// Returns is properties expanded
		bool IsExpanded() const;

		// Returns fields properties info
		const FieldPropertiesInfo& GetPropertiesInfo() const;

		IOBJECT(ObjectPtrProperty);

	protected:
		const Type* mObjectType = nullptr;    // Type of target objects
		const Type* mObjectPtrType = nullptr; // Type of target object pointer

		TargetsVec          mTargetObjects;   // Target objects
		FieldPropertiesInfo mFieldProperties; // Field properties information

		UIWidget*      mTypeContainer = nullptr;      // Type caption and create/delete button container widget, placed on spoiler head
		UISpoiler*     mSpoiler = nullptr;            // Properties spoiler
		UILabel*       mTypeCaption = nullptr;        // Caption that shows type of object or nullptr
		UIButton*      mCreateDeleteButton = nullptr; // CReate and delete button. Create - when value is nullptr, delete - when not
		UIContextMenu* mCreateMenu = nullptr;         // Create object context menu. Initializes with types derived from mObjectType 
													  // when this type changing and create button were pressed

		bool mPropertiesInitialized = false; // True when properties were built and initialized. 
		                                     // Properties building when spoiler is expanding or when changing type and spoiler is still expanding

		bool mContextInitialized = false;    // True when context menu initialized with available types of objects. 
		                                     // Context menu initializes when type changed and create button pressed

		bool mImmediateCreateObject = false; // True when no reason to show context menu, because there is only one available type

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// It is called when create button pressed and shows create object menu
		void OnCreateOrDeletePressed();

		// Creates object by type
		void CreateObject(const Type* type);
	};
}

CLASS_BASES_META(Editor::ObjectPtrProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::ObjectPtrProperty)
{
	PROTECTED_FIELD(mObjectType);
	PROTECTED_FIELD(mObjectPtrType);
	PROTECTED_FIELD(mTargetObjects);
	PROTECTED_FIELD(mFieldProperties);
	PROTECTED_FIELD(mTypeContainer);
	PROTECTED_FIELD(mSpoiler);
	PROTECTED_FIELD(mTypeCaption);
	PROTECTED_FIELD(mCreateDeleteButton);
	PROTECTED_FIELD(mCreateMenu);
	PROTECTED_FIELD(mPropertiesInitialized);
	PROTECTED_FIELD(mContextInitialized);
	PROTECTED_FIELD(mImmediateCreateObject);
}
END_META;
CLASS_METHODS_META(Editor::ObjectPtrProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PUBLIC_FUNCTION(const Type*, GetSpecializedType);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, SetExpanded, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PUBLIC_FUNCTION(const FieldPropertiesInfo&, GetPropertiesInfo);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, OnCreateOrDeletePressed);
	PROTECTED_FUNCTION(void, CreateObject, const Type*);
}
END_META;