#pragma once

#include "Render/RectDrawable.h"
#include "Utils/Basic/ITree.h"
#include "Utils/Editor/Attributes/ExpandedByDefaultAttribute.h"
#include "Utils/Math/Layout.h"
#include "Utils/Serialization/Serializable.h"
#include "Utils/Types/String.h"

namespace o2
{
	class UIWidget;

	class UIWidgetLayer;
	typedef Vector<UIWidgetLayer*>  LayersVec;

#if IS_EDITOR
	typedef SceneEditableObject UIWidgetLayerBase;
#else
	typedef ISerializable UIWidgetLayerBase;
#endif

	// ---------------------
	// Widget drawable layer
	// ---------------------
	class UIWidgetLayer: public UIWidgetLayerBase
	{
	public:
		typedef Vector<UIWidgetLayer*> ChildrenVec;

	public:
		PROPERTIES(UIWidgetLayer);
		PROPERTY(bool, enabled, SetEnabled, IsEnabled);                       // Enable property
		PROPERTY(float, depth, SetDepth, GetDepth);                           // Drawing depth (higher depths will draw later)
		PROPERTY(float, transparency, SetTransparency, GetTransparency);      // Drawable transparency property

		ACCESSOR(UIWidgetLayer*, child, String, GetChild, GetAllChildLayers); // Child layer accessor

#if IS_EDITOR
		PROPERTY(bool, locked, SetLocked, IsLocked);                          // Is locked property
#endif 

	public:
		String          name;               // Name of layer @SERIALIZABLE
					    
		Layout          layout;             // Drawable layout @SERIALIZABLE
		Layout          interactableLayout; // Interactable area layout @SERIALIZABLE
					    
		IRectDrawable*  drawable;           // Drawable @SERIALIZABLE @EXPANDED_BY_DEFAULT

	public:
		// Default constructor
		UIWidgetLayer();

		// Copy-constructor
		UIWidgetLayer(const UIWidgetLayer& other);

		// Destructor
		~UIWidgetLayer();

		// Copy-operator
		UIWidgetLayer& operator=(const UIWidgetLayer& other);


		// Returns pointer to owner widget
		UIWidget* GetOwnerWidget() const;


		// Draws drawable
		void Draw();


		// Returns is layer enabled
		bool IsEnabled() const override;

		// Returns is layer enabled and all parent are enabled too
		bool IsEnabledInHierarchy() const override;

		// Sets enabling of layer
		void SetEnabled(bool enabled) override;


		// Updates drawable and layout
		void Update(float dt);


		// Sets parent layer
		void SetParent(UIWidgetLayer* parent);

		// Returns parent layer
		UIWidgetLayer* GetParent() const;


		// Adds new child layer and returns him
		UIWidgetLayer* AddChild(UIWidgetLayer* node);

		// Remove child layer and releases him if needs
		bool RemoveChild(UIWidgetLayer* node, bool release = true);

		// Removes and releases all children nodes
		void RemoveAllChilds();

		// Return child layers
		ChildrenVec& GetChilds();

		// Returns constant child layers
		const ChildrenVec& GetChilds() const;

		// Adds child layer
		UIWidgetLayer* AddChildLayer(const String& name, IRectDrawable* drawable, const Layout& layout = Layout::BothStretch(),
									 float depth = 0.0f);

		// Returns child layer by path
		UIWidgetLayer* GetChild(const String& path);

		// Returns child layer by name
		UIWidgetLayer* FindChild(const String& name);

		// Returns child layer with type
		template<typename _type>
		_type* FindChild() const;

		// Returns all child layers
		LayersVec GetAllChilds() const;


		// Sets depth
		void SetDepth(float depth);

		// Returns depth
		float GetDepth() const;


		// Sets transparency and updates this and children result transparencies
		void SetTransparency(float transparency);

		// Returns transparency
		float GetTransparency();

		// Returns result transparency
		float GetResTransparency() const;


		// Returns true if layer is under point
		bool IsUnderPoint(const Vec2F& point);

		// Returns layout rectangle
		const RectF& GetRect() const;


		SERIALIZABLE(UIWidgetLayer);

#if IS_EDITOR
		// Returns true when object is on scene
		bool IsOnScene() const override;

		// Returns unique id
		SceneUID GetID() const override;

		// Generates new random id 
		void GenerateNewID(bool childs = true) override;


		// Returns name of object
		String GetName() const override;

		// Sets name of object
		void SetName(const String& name) override;


		// Returns list of object's children
		Vector<SceneEditableObject*> GetEditablesChildren() const override;

		// Returns object's parent object. Return nullptr when it is a root scene object
		SceneEditableObject* GetEditableParent() const override;

		// Sets parent object. nullptr means make this object as root. idx is place in parent children. idx == -1 means last
		void SetEditableParent(SceneEditableObject* object) override;

		// Adds child. idx is place in parent children. idx == -1 means last
		void AddEditableChild(SceneEditableObject* object, int idx = -1) override;

		// Sets index in siblings - children of parent
		void SetIndexInSiblings(int idx) override;

		// Checks that this object can be added as child to another object
		bool CanBeParentedTo(const Type& parentType) override;


		// Returns is that type of object can be enabled and disabled
		bool IsSupportsDisabling() const override;


		// Returns is that type of object can be locked
		bool IsSupportsLocking() const override;

		// Returns is object locked, override when it's supports
		bool IsLocked() const override;

		// Returns is object locked and all parent are locked too
		bool IsLockedInHierarchy() const override;

		// Sets locking of object, override when it's supports
		void SetLocked(bool locked) override;


		// Returns is that type of object can be transformed
		bool IsSupportsTransforming() const override;

		// Returns transform, override when it's supports
		Basis GetTransform() const override;

		// Sets transform of object, override when it's supports
		void SetTransform(const Basis& transform) override;

		// Updates transform immediately
		void UpdateSelfTransform() override;


		// Returns is object supports pivot 
		bool IsSupportsPivot() const override;

		// Sets transform pivot point
		void SetPivot(const Vec2F& pivot) override;

		// Returns transform pivot
		Vec2F GetPivot() const override;


		// Returns is that type of object can be transformed with layout
		bool IsSupportsLayout() const override;

		// Returns layout, override when it's supports
		Layout GetLayout() const override;

		// Sets layout of object, override when it's supports
		void SetLayout(const Layout& layout) override;


		// It is called when something changed in this object
		void OnChanged() override;

#endif // IS_EDITOR

	protected:
		bool           mEnabled = true;         // Is layer enabled
		float          mTransparency = 1.0f;    // Layer transparency @SERIALIZABLE
		float          mResTransparency = 1.0f; // Result drawable transparency, depends on parent transparency
		float          mDepth = 0.0f;           // Depth of drawable @SERIALIZABLE
		RectF          mAbsolutePosition;       // Result absolute drawable position
		RectF          mInteractableArea;       // Interactable area, depends on interactableLayout
		UIWidget*      mOwnerWidget = nullptr;  // Owner widget pointer @EXCLUDE_POINTER_SEARCH
		UIWidgetLayer* mParent = nullptr;       // Pointer to parent layer @EXCLUDE_POINTER_SEARCH
		ChildrenVec    mChildren;               // Children layers @SERIALIZABLE

#if IS_EDITOR
		bool           mIsLocked = false;       // Is locked
		SceneUID       mUID = Math::Random();   // Scene editor uid
#endif // IS_EDITOR

	protected:
		// Completion deserialization callback
		void OnDeserialized(const DataNode& node) override;

		// Sets owner widget for this and children
		void SetOwnerWidget(UIWidget* owner);

		// It is called when added new child layer, sets his owner same as this owner and calls UpdateLayersDrawingSequence in owner
		void OnChildAdded(UIWidgetLayer* child);

		// Updates drawable rect, calling when widget's layout was changed
		void UpdateLayout();

		// It is called when transparency was changed and updates children transparencies
		void UpdateResTransparency();

		// It is called when owner widget including in scene. Registers editable object and it's children
		void OnIncludeInScene();

		// It is called when owner widget excluding in scene. Unregisters editable object and it's children
		void OnExcludeFromScene();

		// Returns dictionary with all child layers
		Dictionary<String, UIWidgetLayer*> GetAllChildLayers();

		friend class UIWidget;
	};

	template<typename _type>
	_type* UIWidgetLayer::FindChild() const
	{
		for (auto child : mChildren)
			if (child->drawable && child->drawable->GetType() == TypeOf(_type))
				return (_type*)(child->drawable);

		for (auto child : mChildren)
		{
			auto res = child->FindChild<_type>();
			if (res)
				return res;
		}

		return nullptr;
	}
}

CLASS_BASES_META(o2::UIWidgetLayer)
{
	BASE_CLASS(o2::SceneEditableObject);
}
END_META;
CLASS_FIELDS_META(o2::UIWidgetLayer)
{
	PUBLIC_FIELD(enabled);
	PUBLIC_FIELD(depth);
	PUBLIC_FIELD(transparency);
	PUBLIC_FIELD(child);
	PUBLIC_FIELD(locked);
	PUBLIC_FIELD(name).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(layout).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(interactableLayout).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(drawable).EXPANDED_BY_DEFAULT_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mEnabled);
	PROTECTED_FIELD(mTransparency).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mResTransparency);
	PROTECTED_FIELD(mDepth).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAbsolutePosition);
	PROTECTED_FIELD(mInteractableArea);
	PROTECTED_FIELD(mOwnerWidget).EXCLUDE_POINTER_SEARCH_ATTRIBUTE();
	PROTECTED_FIELD(mParent).EXCLUDE_POINTER_SEARCH_ATTRIBUTE();
	PROTECTED_FIELD(mChildren).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mIsLocked);
	PROTECTED_FIELD(mUID);
}
END_META;
CLASS_METHODS_META(o2::UIWidgetLayer)
{

	typedef Dictionary<String, UIWidgetLayer*> _tmp1;

	PUBLIC_FUNCTION(UIWidget*, GetOwnerWidget);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(bool, IsEnabledInHierarchy);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, SetParent, UIWidgetLayer*);
	PUBLIC_FUNCTION(UIWidgetLayer*, GetParent);
	PUBLIC_FUNCTION(UIWidgetLayer*, AddChild, UIWidgetLayer*);
	PUBLIC_FUNCTION(bool, RemoveChild, UIWidgetLayer*, bool);
	PUBLIC_FUNCTION(void, RemoveAllChilds);
	PUBLIC_FUNCTION(ChildrenVec&, GetChilds);
	PUBLIC_FUNCTION(const ChildrenVec&, GetChilds);
	PUBLIC_FUNCTION(UIWidgetLayer*, AddChildLayer, const String&, IRectDrawable*, const Layout&, float);
	PUBLIC_FUNCTION(UIWidgetLayer*, GetChild, const String&);
	PUBLIC_FUNCTION(UIWidgetLayer*, FindChild, const String&);
	PUBLIC_FUNCTION(LayersVec, GetAllChilds);
	PUBLIC_FUNCTION(void, SetDepth, float);
	PUBLIC_FUNCTION(float, GetDepth);
	PUBLIC_FUNCTION(void, SetTransparency, float);
	PUBLIC_FUNCTION(float, GetTransparency);
	PUBLIC_FUNCTION(float, GetResTransparency);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(const RectF&, GetRect);
	PUBLIC_FUNCTION(bool, IsOnScene);
	PUBLIC_FUNCTION(SceneUID, GetID);
	PUBLIC_FUNCTION(void, GenerateNewID, bool);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(Vector<SceneEditableObject*>, GetEditablesChildren);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableParent);
	PUBLIC_FUNCTION(void, SetEditableParent, SceneEditableObject*);
	PUBLIC_FUNCTION(void, AddEditableChild, SceneEditableObject*, int);
	PUBLIC_FUNCTION(void, SetIndexInSiblings, int);
	PUBLIC_FUNCTION(bool, CanBeParentedTo, const Type&);
	PUBLIC_FUNCTION(bool, IsSupportsDisabling);
	PUBLIC_FUNCTION(bool, IsSupportsLocking);
	PUBLIC_FUNCTION(bool, IsLocked);
	PUBLIC_FUNCTION(bool, IsLockedInHierarchy);
	PUBLIC_FUNCTION(void, SetLocked, bool);
	PUBLIC_FUNCTION(bool, IsSupportsTransforming);
	PUBLIC_FUNCTION(Basis, GetTransform);
	PUBLIC_FUNCTION(void, SetTransform, const Basis&);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PUBLIC_FUNCTION(bool, IsSupportsPivot);
	PUBLIC_FUNCTION(void, SetPivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPivot);
	PUBLIC_FUNCTION(bool, IsSupportsLayout);
	PUBLIC_FUNCTION(Layout, GetLayout);
	PUBLIC_FUNCTION(void, SetLayout, const Layout&);
	PUBLIC_FUNCTION(void, OnChanged);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, SetOwnerWidget, UIWidget*);
	PROTECTED_FUNCTION(void, OnChildAdded, UIWidgetLayer*);
	PROTECTED_FUNCTION(void, UpdateLayout);
	PROTECTED_FUNCTION(void, UpdateResTransparency);
	PROTECTED_FUNCTION(void, OnIncludeInScene);
	PROTECTED_FUNCTION(void, OnExcludeFromScene);
	PROTECTED_FUNCTION(_tmp1, GetAllChildLayers);
}
END_META;
