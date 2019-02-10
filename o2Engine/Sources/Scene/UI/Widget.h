#pragma once

#include "Scene/Actor.h"
#include "Scene/Drawable.h"
#include "Utils/Math/Layout.h"

namespace o2
{
	class IRectDrawable;
	class WidgetLayer;
	class WidgetLayout;
	class WidgetState;

	// ------------------------------------------------------
	// Basic UI Widget. Its a simple and basic element of UI, 
	// everything other UI's are based on this
	// ------------------------------------------------------
	class Widget : public Actor, public SceneDrawable
	{
	public:
		typedef Vector<Widget*> WidgetsVec;
		typedef Vector<WidgetLayer*> LayersVec;
		typedef Vector<WidgetState*> StatesVec;

	public:
		PROPERTIES(Widget);

		PROPERTY(float, transparency, SetTransparency, GetTransparency); // Transparency property
		GETTER(float, resTransparency, GetResTransparency);              // Result transparency getter, depends on parent transparency

		PROPERTY(Widget*, parentWidget, SetParentWidget, GetParentWidget); // Parent widget property
		GETTER(WidgetsVec, childrenWidgets, GetChildrenNonConst);            // Widget children getter

		GETTER(LayersVec, layers, GetLayers); // Layers getter
		GETTER(StatesVec, states, GetStates); // States getter

		ACCESSOR(Widget*, childWidget, String, GetChildWidget, GetAllChilds); // Widget child accessor by path like "child/subchild/somechild"
		ACCESSOR(WidgetLayer*, layer, String, GetLayer, GetAllLayers);        // Widget layer accessor by path like "layer/sublayer/target"
		ACCESSOR(WidgetState*, state, String, GetStateObject, GetAllStates);  // Widget state accessor by name

	public:
		WidgetLayout* const layout;     // Widget layout

	public:
		Function<void()> onLayoutUpdated; // Layout change event
		Function<void()> onFocused;       // Widget focused event
		Function<void()> onUnfocused;     // Widget unfocused event
		Function<void()> onShow;          // Widget showing vent
		Function<void()> onHide;          // Widget hiding event

	public:
		// Default constructor
		Widget(ActorCreateMode mode = ActorCreateMode::Default);

		// Widget constructor from prototype
		Widget(const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::Default);

		// Widget constructor with components
		Widget(ComponentsVec components, ActorCreateMode mode = ActorCreateMode::Default);

		// Copy-constructor
		Widget(const Widget& other);

		// Virtual destructor
		virtual ~Widget();


		// Copy-operator
		Widget& operator=(const Widget& other);


		// Updates layers, states and widget
		void Update(float dt) override;

		// Updates childs
		void UpdateChildren(float dt) override;


		// Updates self transform, dependent parents and children transforms
		void UpdateTransform() override;

		// Updates children and internal children transforms
		void UpdateChildrenTransforms() override;


		// Draws widget and child widgets with not overridden depth
		void Draw() override;

		// Forcible drawing in area with transparency
		void ForceDraw(const RectF& area, float transparency);


		// Sets layout dirty, and update it in update loop
		void SetLayoutDirty();


		// Returns parent widget
		Widget* GetParentWidget() const;

		// Returns children rectangle layout
		RectF GetChildrenRect() const;

		// Returns child widget by path (like "root/some node/other node/target node")
		Widget* GetChildWidget(const String& path) const;

		// Adds child widget and returns them
		Widget* AddChildWidget(Widget* widget);

		// Adds child widget at position and returns them
		Widget* AddChildWidget(Widget* widget, int position);

		// Returns constant children widgets vector
		const WidgetsVec& GetChildWidgets() const;

		// Sets index position in parent or scene
		void SetIndexInSiblings(int index) override;


		// Adds layer
		WidgetLayer* AddLayer(WidgetLayer* layer);

		// Adds layer
		WidgetLayer* AddLayer(const String& name, IRectDrawable* drawable,
								const Layout& layout = Layout::BothStretch(), float depth = 0.0f);

		// Removes layer
		void RemoveLayer(WidgetLayer* layer, bool release = true);

		// Removes layer
		void RemoveLayer(const String& path);

		// Removes all layers
		void RemoveAllLayers();

		// Returns layer by path. Returns null if layer isn't exist
		WidgetLayer* GetLayer(const String& path) const;

		// Returns layer by name in layers hierarchy. Returns null if layer isn't exist
		WidgetLayer* FindLayer(const String& name) const;

		// Searches layer with drawable with specified type
		template<typename _type>
		_type* GetLayerDrawableByType();

		// Returns layer by path. Returns null if layer isn't exist or layer drawable has different type
		template<typename _type>
		_type* GetLayerDrawable(const String& path) const;

		// Returns all layers
		const LayersVec& GetLayers() const;


		// Adds new state with name
		WidgetState* AddState(const String& name);

		// Adds new state with name and animation
		WidgetState* AddState(const String& name, const Animation& animation);

		// Adds state
		WidgetState* AddState(WidgetState* state);

		// Removes state by name
		bool RemoveState(const String& name);

		// Removes state
		bool RemoveState(WidgetState* state);

		// Removes all states
		void RemoveAllStates();

		// Sets state value
		void SetState(const String& name, bool state);

		// Sets state value immediately
		void SetStateForcible(const String& name, bool state);

		// Returns state value
		bool GetState(const String& name) const;

		// Returns state object by name
		WidgetState* GetStateObject(const String& name) const;

		// Returns all states
		const StatesVec& GetStates() const;


		// Sets depth overriding
		void SetDepthOverridden(bool overrideDepth);

		// Is sorting depth overridden
		bool IsDepthOverriden() const;


		// Sets widget's transparency
		void SetTransparency(float transparency);

		// Returns widget's transparency
		float GetTransparency() const;

		// Returns widget's result transparency (depends on parent's result transparency)
		float GetResTransparency() const;


		// Sets visibility
		void SetEnableForcible(bool visible);

		// Sets visibility to true
		void Show(bool forcible = false);

		// Sets visibility to false
		void Hide(bool forcible = false);


		// Focus this widget
		void Focus();

		// Unfocus this widget
		void Unfocus();

		// Returns is this widget focused
		bool IsFocused() const;

		// Returns is this widget can be focused
		virtual bool IsFocusable() const;

		// Sets widget can be focused
		void SetFocusable(bool focusable);


		// Returns true if point is under drawable
		bool IsUnderPoint(const Vec2F& point);


		// Sets parent,  doesn't adds to parent's children but adds to internal children
		void SetInternalParent(Widget* parent, bool worldPositionStays = false);

		// Adds widget to internal children
		void AddInternalWidget(Widget* widget, bool worldPositionStays = false);

		// Returns internal child widget by path (like "root/some node/other node/target node")
		Widget* GetInternalWidget(const String& path) const;

		// Returns internal child widget by path (like "root/some node/other node/target node")
		template<typename _type>
		_type* GetInternalWidgetByType(const String& path) const;

		// Searches widget with name in internal widgets hierarchy
		Widget* FindInternalWidget(const String& name) const;

		// Searches widget with type and name in internal widgets hierarchy
		template<typename _type>
		_type* FindInternalWidgetByType(const String& name) const;

		// Searches widget with type in internal widgets hierarchy
		template<typename _type>
		_type* FindInternalWidgetByType() const;

		SERIALIZABLE(Widget);

	protected:
		using Actor::mLayer;
		using Actor::mIsOnScene;

		LayersVec      mLayers;                 // Layers array @SERIALIZABLE
		StatesVec      mStates;                 // States array @SERIALIZABLE

		Widget*      mParentWidget = nullptr; // Parent widget. When parent is not widget, this field will be null  @EXCLUDE_POINTER_SEARCH
		WidgetsVec     mChildWidgets;           // Children widgets, a part of all children
		WidgetsVec     mInternalWidgets;        // Internal widgets, used same as children widgets, but not really children @SERIALIZABLE
		WidgetsVec     mDrawingChildren;        // Children widgets, which drawing depth isn't overridden

		RectF          mChildrenWorldRect;      // World rectangle for children arranging

		bool           mOverrideDepth = false;  // Is sorting order depth overridden. If not, sorting order depends on hierarchy @SERIALIZABLE

		float          mTransparency = 1.0f;	// Widget transparency @SERIALIZABLE
		float          mResTransparency = 1.0f; // Widget result transparency, depends on parent's result transparency

		LayersVec      mDrawingLayers;          // Layers ordered by depth, which drawing before children (depth < 1000)
		LayersVec      mTopDrawingLayers;       // Layers ordered by depth, which drawing after children (depth > 1000)

		WidgetState* mFocusedState = nullptr; // Focused widget state
		bool           mIsFocused = false;      // Is widget focused
		bool           mIsFocusable = false;    // Is widget can be focused @SERIALIZABLE

		WidgetState* mVisibleState = nullptr; // Widget visibility state

		bool           mIsClipped = false;      // Is widget fully clipped by some scissors

		RectF          mBounds;                 // Widget bounds by drawing layers
		RectF          mBoundsWithChilds;       // Widget with childs bounds

	protected:
		// Updates result read enable flag
		void UpdateResEnabled() override;

		// Updates enabling
		void UpdateResEnabledInHierarchy() override;

		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when transformation was changed and updated
		void OnTransformUpdated() override;

		// It is called when parent changed
		void OnParentChanged(Actor* oldParent) override;

		// It is called when child actor was added
		void OnChildAdded(Actor* child) override;

		// It is called when child actor was removed
		void OnChildRemoved(Actor* child) override;

		// It is called when layer was changed
		void OnLayerChanged(SceneLayer* oldLayer) override;

		// It is called when actor excluding from scene, removes this from layer drawables
		void OnExcludeFromScene() override;

		// It is called when actor including from scene, including this to layer drawables
		void OnIncludeToScene() override;

		// Moves widget's to delta and checks for clipping
		virtual void MoveAndCheckClipping(const Vec2F& delta, const RectF& clipArea);

		// It is called when child widget was added
		virtual void OnChildAdded(Widget* child);

		// It is called when child widget was removed
		virtual void OnChildRemoved(Widget* child);

		// It is called when widget was selected
		virtual void OnFocused();

		// It is called when widget was deselected
		virtual void OnUnfocused();

		// Returns layout width with children
		virtual float GetMinWidthWithChildren() const;

		// Returns layout height with childer
		virtual float GetMinHeightWithChildren() const;

		// Returns layout width weight with children
		virtual float GetWidthWeightWithChildren() const;

		// Returns layout height weight with children
		virtual float GetHeightWeightWithChildren() const;

		// Updates bounds by drawing layers
		virtual void UpdateBounds();

		// Updates bound with children
		virtual void UpdateBoundsWithChilds();

		// Checks widget clipping by area
		virtual void CheckClipping(const RectF& clipArea);

		// Updates transparency for this and children widgets
		virtual void UpdateTransparency();

		// Updates result visibility
		virtual void UpdateVisibility(bool updateLayout = true);

		// Updates layers layouts, calls after updating widget layout
		virtual void UpdateLayersLayouts();

		// It is called when child widget was selected
		virtual void OnChildFocused(Widget* child);

		// It is called when layer added and updates drawing sequence
		virtual void OnLayerAdded(WidgetLayer* layer);

		// It is called when widget state was added
		virtual void OnStateAdded(WidgetState* state);

		// Draws debug frame by mAbsoluteRect
		void DrawDebugFrame();

		// Updates drawing children widgets list
		void UpdateDrawingChildren();

		// Updates layers drawing sequence
		void UpdateLayersDrawingSequence();

		// Sets new target for all states animations
		void RetargetStatesAnimations();

		// Sets parent widget, used for property
		void SetParentWidget(Widget* widget);

		// Returns children widgets (for property)
		WidgetsVec GetChildrenNonConst();

		// Returns layers (for property)
		LayersVec GetLayersNonConst();

		// Returns states (for property)
		StatesVec GetStatesNonConst();

		// Returns dictionary of all layers by names
		Dictionary<String, WidgetLayer*> GetAllLayers();

		// Returns dictionary of all children by names
		Dictionary<String, Widget*> GetAllChilds();

		// Returns dictionary of all states by names
		Dictionary<String, WidgetState*> GetAllStates();

		// Beginning serialization callback
		void OnSerialize(DataNode& node) const override;

		// It is called when deserialized
		void OnDeserialized(const DataNode& node) override;

		friend class Scene;
		friend class UIContextMenu;
		friend class CustomDropDown;
		friend class CustomList;
		friend class DropDown;
		friend class EditBox;
		friend class GridLayout;
		friend class HorizontalLayout;
		friend class HorizontalProgress;
		friend class HorizontalScrollBar;
		friend class Label;
		friend class List;
		friend class LongList;
		friend class UIManager;
		friend class MenuPanel;
		friend class ScrollArea;
		friend class Spoiler;
		friend class Tree;
		friend class UITreeNode;
		friend class VerticalLayout;
		friend class VerticalProgress;
		friend class VerticalScrollBar;
		friend class WidgetLayer;
		friend class WidgetLayout;
		friend class Window;

#if IS_EDITOR
		class LayersEditable : public SceneEditableObject
		{
		public:
			// Default constructor
			LayersEditable();

			// Constructor with widget
			LayersEditable(Widget* widget);

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


			// Returns transform, override when it's supports
			Basis GetTransform() const override;

			SERIALIZABLE(LayersEditable);

		private:
			Widget* mWidget = nullptr;
			SceneUID  mUID = Math::Random();

			friend class Widget;
		};

		class InternalChildrenEditableEditable : public SceneEditableObject
		{
		public:
			// Default constructor
			InternalChildrenEditableEditable();

			// Constructor with widget
			InternalChildrenEditableEditable(Widget* widget);

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


			// Returns transform, override when it's supports
			Basis GetTransform() const override;

			SERIALIZABLE(InternalChildrenEditableEditable);

		private:
			Widget* mWidget = nullptr;
			SceneUID  mUID = Math::Random();

			friend class Widget;
		};

		static bool isEditorLayersVisible;           // Is widgets layers visible in hierarchy
		static bool isEditorInternalChildrenVisible; // Is widgets internal children visible in hierarchy

		LayersEditable layersEditable = LayersEditable(this);
		InternalChildrenEditableEditable internalChildrenEditable = InternalChildrenEditableEditable(this);

	public:
		// Returns object's parent object. Return nullptr when it is a root scene object
		SceneEditableObject* GetEditableParent() const override;


		// Returns list of object's children
		Vector<SceneEditableObject*> GetEditablesChildren() const override;

		// Adds child. idx is place in parent children. idx == -1 means last
		void AddEditableChild(SceneEditableObject* object, int idx = -1) override;


		// Returns is that type of object can be transformed
		bool IsSupportsTransforming() const override;

		// Returns transform, override when it's supports
		Basis GetTransform() const override;

		// Sets transform of object, override when it's supports
		void SetTransform(const Basis& transform) override;


		// Returns is that type of object can be transformed with layout
		bool IsSupportsLayout() const override;

		// Returns layout, override when it's supports
		Layout GetLayout() const override;

		// Sets layout of object, override when it's supports
		void SetLayout(const Layout& layout) override;


		// Returns pointer to owner editable object
		SceneEditableObject* GetEditableOwner() override;


		friend class LayersEditable;
		friend class InternalChildrenEditableEditable;
#endif // IS_EDITOR
	};
}

#include "Scene/UI/WidgetLayer.h"

namespace o2
{
	template<typename _type>
	_type* Widget::GetLayerDrawableByType()
	{
		for (auto layer : mLayers)
		{
			if (layer->GetDrawable() && layer->GetDrawable()->GetType() == TypeOf(_type))
				return (_type*)layer->GetDrawable();
		}

		for (auto layer : mLayers)
		{
			auto res = layer->FindChild<_type>();
			if (res)
				return res;
		}

		return nullptr;
	}

	template<typename _type>
	_type* Widget::GetLayerDrawable(const String& path) const
	{
		auto layer = GetLayer(path);
		if (layer && layer->GetDrawable() && layer->GetDrawable()->GetType() == TypeOf(_type))
			return (_type*)layer->GetDrawable();

		return nullptr;
	}

	template<typename _type>
	_type* Widget::GetInternalWidgetByType(const String& path) const
	{
		return dynamic_cast<_type*>(GetInternalWidget(path));
	}

	template<typename _type>
	_type* Widget::FindInternalWidgetByType(const String& name) const
	{
		for (auto widget : mInternalWidgets)
		{
			if (widget->GetName() == name)
			{
				if (_type* res = dynamic_cast<_type*>(widget))
					return res;
			}

			if (_type* res = widget->FindChildByTypeAndName<_type>((String)name))
				return res;

			if (auto internalWidget = widget->FindInternalWidget(name))
				if (_type* res = dynamic_cast<_type*>(internalWidget))
					return res;
		}

		return nullptr;
	}

	template<typename _type>
	_type* Widget::FindInternalWidgetByType() const
	{
		for (auto widget : mInternalWidgets)
		{
			if (_type* res = dynamic_cast<_type*>(widget))
				return res;

			if (_type* res = widget->FindChildByType<_type>())
				return res;

			if (_type* res = widget->FindInternalWidgetByType<_type>())
				return res;
		}

		return nullptr;
	}

}

CLASS_BASES_META(o2::Widget)
{
	BASE_CLASS(o2::Actor);
	BASE_CLASS(o2::SceneDrawable);
}
END_META;
CLASS_FIELDS_META(o2::Widget)
{
	PUBLIC_FIELD(transparency);
	PUBLIC_FIELD(resTransparency);
	PUBLIC_FIELD(parentWidget);
	PUBLIC_FIELD(childrenWidgets);
	PUBLIC_FIELD(layers);
	PUBLIC_FIELD(states);
	PUBLIC_FIELD(childWidget);
	PUBLIC_FIELD(layer);
	PUBLIC_FIELD(state);
	PUBLIC_FIELD(layout);
	PUBLIC_FIELD(onLayoutUpdated);
	PUBLIC_FIELD(onFocused);
	PUBLIC_FIELD(onUnfocused);
	PUBLIC_FIELD(onShow);
	PUBLIC_FIELD(onHide);
	PROTECTED_FIELD(mLayers).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mStates).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mParentWidget).EXCLUDE_POINTER_SEARCH_ATTRIBUTE();
	PROTECTED_FIELD(mChildWidgets);
	PROTECTED_FIELD(mInternalWidgets).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mDrawingChildren);
	PROTECTED_FIELD(mChildrenWorldRect);
	PROTECTED_FIELD(mOverrideDepth).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTransparency).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mResTransparency);
	PROTECTED_FIELD(mDrawingLayers);
	PROTECTED_FIELD(mFocusedState);
	PROTECTED_FIELD(mIsFocused);
	PROTECTED_FIELD(mIsFocusable).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mVisibleState);
	PROTECTED_FIELD(mIsClipped);
	PROTECTED_FIELD(mBounds);
	PROTECTED_FIELD(mBoundsWithChilds);
	PROTECTED_FIELD(layersEditable);
	PROTECTED_FIELD(internalChildrenEditable);
}
END_META;
CLASS_METHODS_META(o2::Widget)
{

	typedef Dictionary<String, WidgetLayer*> _tmp1;
	typedef Dictionary<String, Widget*> _tmp2;
	typedef Dictionary<String, WidgetState*> _tmp3;

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, UpdateChildren, float);
	PUBLIC_FUNCTION(void, UpdateTransform);
	PUBLIC_FUNCTION(void, UpdateChildrenTransforms);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, ForceDraw, const RectF&, float);
	PUBLIC_FUNCTION(void, SetLayoutDirty);
	PUBLIC_FUNCTION(Widget*, GetParentWidget);
	PUBLIC_FUNCTION(RectF, GetChildrenRect);
	PUBLIC_FUNCTION(Widget*, GetChildWidget, const String&);
	PUBLIC_FUNCTION(Widget*, AddChildWidget, Widget*);
	PUBLIC_FUNCTION(Widget*, AddChildWidget, Widget*, int);
	PUBLIC_FUNCTION(const WidgetsVec&, GetChildWidgets);
	PUBLIC_FUNCTION(void, SetIndexInSiblings, int);
	PUBLIC_FUNCTION(WidgetLayer*, AddLayer, WidgetLayer*);
	PUBLIC_FUNCTION(WidgetLayer*, AddLayer, const String&, IRectDrawable*, const Layout&, float);
	PUBLIC_FUNCTION(void, RemoveLayer, WidgetLayer*, bool);
	PUBLIC_FUNCTION(void, RemoveLayer, const String&);
	PUBLIC_FUNCTION(void, RemoveAllLayers);
	PUBLIC_FUNCTION(WidgetLayer*, GetLayer, const String&);
	PUBLIC_FUNCTION(WidgetLayer*, FindLayer, const String&);
	PUBLIC_FUNCTION(const LayersVec&, GetLayers);
	PUBLIC_FUNCTION(WidgetState*, AddState, const String&);
	PUBLIC_FUNCTION(WidgetState*, AddState, const String&, const Animation&);
	PUBLIC_FUNCTION(WidgetState*, AddState, WidgetState*);
	PUBLIC_FUNCTION(bool, RemoveState, const String&);
	PUBLIC_FUNCTION(bool, RemoveState, WidgetState*);
	PUBLIC_FUNCTION(void, RemoveAllStates);
	PUBLIC_FUNCTION(void, SetState, const String&, bool);
	PUBLIC_FUNCTION(void, SetStateForcible, const String&, bool);
	PUBLIC_FUNCTION(bool, GetState, const String&);
	PUBLIC_FUNCTION(WidgetState*, GetStateObject, const String&);
	PUBLIC_FUNCTION(const StatesVec&, GetStates);
	PUBLIC_FUNCTION(void, SetDepthOverridden, bool);
	PUBLIC_FUNCTION(bool, IsDepthOverriden);
	PUBLIC_FUNCTION(void, SetTransparency, float);
	PUBLIC_FUNCTION(float, GetTransparency);
	PUBLIC_FUNCTION(float, GetResTransparency);
	PUBLIC_FUNCTION(void, SetEnableForcible, bool);
	PUBLIC_FUNCTION(void, Show, bool);
	PUBLIC_FUNCTION(void, Hide, bool);
	PUBLIC_FUNCTION(void, Focus);
	PUBLIC_FUNCTION(void, Unfocus);
	PUBLIC_FUNCTION(bool, IsFocused);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(void, SetFocusable, bool);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, SetInternalParent, Widget*, bool);
	PUBLIC_FUNCTION(void, AddInternalWidget, Widget*, bool);
	PUBLIC_FUNCTION(Widget*, GetInternalWidget, const String&);
	PUBLIC_FUNCTION(Widget*, FindInternalWidget, const String&);
	PROTECTED_FUNCTION(void, UpdateResEnabled);
	PROTECTED_FUNCTION(void, UpdateResEnabledInHierarchy);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, OnParentChanged, Actor*);
	PROTECTED_FUNCTION(void, OnChildAdded, Actor*);
	PROTECTED_FUNCTION(void, OnChildRemoved, Actor*);
	PROTECTED_FUNCTION(void, OnLayerChanged, SceneLayer*);
	PROTECTED_FUNCTION(void, OnExcludeFromScene);
	PROTECTED_FUNCTION(void, OnIncludeToScene);
	PROTECTED_FUNCTION(void, MoveAndCheckClipping, const Vec2F&, const RectF&);
	PROTECTED_FUNCTION(void, OnChildAdded, Widget*);
	PROTECTED_FUNCTION(void, OnChildRemoved, Widget*);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, OnUnfocused);
	PROTECTED_FUNCTION(float, GetMinWidthWithChildren);
	PROTECTED_FUNCTION(float, GetMinHeightWithChildren);
	PROTECTED_FUNCTION(float, GetWidthWeightWithChildren);
	PROTECTED_FUNCTION(float, GetHeightWeightWithChildren);
	PROTECTED_FUNCTION(void, UpdateBounds);
	PROTECTED_FUNCTION(void, UpdateBoundsWithChilds);
	PROTECTED_FUNCTION(void, CheckClipping, const RectF&);
	PROTECTED_FUNCTION(void, UpdateTransparency);
	PROTECTED_FUNCTION(void, UpdateVisibility, bool);
	PROTECTED_FUNCTION(void, UpdateLayersLayouts);
	PROTECTED_FUNCTION(void, OnChildFocused, Widget*);
	PROTECTED_FUNCTION(void, OnLayerAdded, WidgetLayer*);
	PROTECTED_FUNCTION(void, OnStateAdded, WidgetState*);
	PROTECTED_FUNCTION(void, DrawDebugFrame);
	PROTECTED_FUNCTION(void, UpdateDrawingChildren);
	PROTECTED_FUNCTION(void, UpdateLayersDrawingSequence);
	PROTECTED_FUNCTION(void, RetargetStatesAnimations);
	PROTECTED_FUNCTION(void, SetParentWidget, Widget*);
	PROTECTED_FUNCTION(WidgetsVec, GetChildrenNonConst);
	PROTECTED_FUNCTION(LayersVec, GetLayersNonConst);
	PROTECTED_FUNCTION(StatesVec, GetStatesNonConst);
	PROTECTED_FUNCTION(_tmp1, GetAllLayers);
	PROTECTED_FUNCTION(_tmp2, GetAllChilds);
	PROTECTED_FUNCTION(_tmp3, GetAllStates);
	PROTECTED_FUNCTION(void, OnSerialize, DataNode&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableParent);
	PUBLIC_FUNCTION(Vector<SceneEditableObject*>, GetEditablesChildren);
	PUBLIC_FUNCTION(void, AddEditableChild, SceneEditableObject*, int);
	PUBLIC_FUNCTION(bool, IsSupportsTransforming);
	PUBLIC_FUNCTION(Basis, GetTransform);
	PUBLIC_FUNCTION(void, SetTransform, const Basis&);
	PUBLIC_FUNCTION(bool, IsSupportsLayout);
	PUBLIC_FUNCTION(Layout, GetLayout);
	PUBLIC_FUNCTION(void, SetLayout, const Layout&);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableOwner);
}
END_META;

CLASS_BASES_META(o2::Widget::LayersEditable)
{
	BASE_CLASS(o2::SceneEditableObject);
}
END_META;
CLASS_FIELDS_META(o2::Widget::LayersEditable)
{
	PRIVATE_FIELD(mWidget);
	PRIVATE_FIELD(mUID);
}
END_META;
CLASS_METHODS_META(o2::Widget::LayersEditable)
{

	PUBLIC_FUNCTION(SceneUID, GetID);
	PUBLIC_FUNCTION(void, GenerateNewID, bool);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(Vector<SceneEditableObject*>, GetEditablesChildren);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableParent);
	PUBLIC_FUNCTION(void, SetEditableParent, SceneEditableObject*);
	PUBLIC_FUNCTION(void, AddEditableChild, SceneEditableObject*, int);
	PUBLIC_FUNCTION(void, SetIndexInSiblings, int);
	PUBLIC_FUNCTION(Basis, GetTransform);
}
END_META;

CLASS_BASES_META(o2::Widget::InternalChildrenEditableEditable)
{
	BASE_CLASS(o2::SceneEditableObject);
}
END_META;
CLASS_FIELDS_META(o2::Widget::InternalChildrenEditableEditable)
{
	PRIVATE_FIELD(mWidget);
	PRIVATE_FIELD(mUID);
}
END_META;
CLASS_METHODS_META(o2::Widget::InternalChildrenEditableEditable)
{

	PUBLIC_FUNCTION(SceneUID, GetID);
	PUBLIC_FUNCTION(void, GenerateNewID, bool);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(Vector<SceneEditableObject*>, GetEditablesChildren);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableParent);
	PUBLIC_FUNCTION(void, SetEditableParent, SceneEditableObject*);
	PUBLIC_FUNCTION(void, AddEditableChild, SceneEditableObject*, int);
	PUBLIC_FUNCTION(void, SetIndexInSiblings, int);
	PUBLIC_FUNCTION(Basis, GetTransform);
}
END_META;