#pragma once

#include "o2/Application/Input.h"
#include "o2/Events/CursorAreaEventsListenersLayer.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/Containers/Vector.h"

// Events system accessor macros
#define o2Events o2::EventSystem::Instance()

namespace o2
{
	class ApplicationEventsListener;
	class CursorAreaEventsListener;
	class CursorEventsListener;
	class DragableObject;
	class KeyboardEventsListener;
	class ShortcutKeysListenersManager;

	// -----------------------
	// Event processing system
	// -----------------------
	class EventSystem: public Singleton<EventSystem>
	{
	public:
		static bool eventsListenersEnabledByDefault; // Then it is true, new events listeners will be enabled on initialization

	public:
		// Returns first cursor event listener under cursor with specified type
		template<typename _type = CursorAreaEventsListener>
		_type* GetCursorListenerUnderCursor(CursorId cursorId) const;

		// Returns all cursor listeners under cursor arranged by depth
		Vector<CursorAreaEventsListener*> GetAllCursorListenersUnderCursor(CursorId cursorId) const;

		// Breaks cursor event. All pressed listeners will be unpressed with specific event OnPressBreak
		void BreakCursorEvent();

		// Updates and processes events
		void Update(float dt);

		// Post update events
		void PostUpdate();

	protected:
		// Default constructor
		EventSystem();

		// Destructor
		~EventSystem();

		// Returns time between clicks for double click reaction
		float GetDoubleClickTime() const;

		// It is called when application was started
		void OnApplicationStarted();

		// It is called when application closing
		void OnApplicationClosing();

		// It is called when application activated
		void OnApplicationActivated();

		// It is called when application deactivated
		void OnApplicationDeactivated();

		// It is called when application frame was sized
		void OnApplicationSized();

		// processes cursor tracing for cursor
		void ProcessCursorTracing(const Input::Cursor& cursor);

		// Processes cursor enter event
		void ProcessCursorEnter();

		// Processes cursor exit event
		void ProcessCursorExit();

		// Processes cursor pressed event
		void ProcessCursorPressed(const Input::Cursor& cursor);

		// Processes cursor down event
		void ProcessCursorDown(const Input::Cursor& cursor);

		// Processes cursor released event
		void ProcessCursorReleased(const Input::Cursor& cursor);

		// Processes mouse right button pressed event
		void ProcessRBPressed();

		// Processes mouse right button down event
		void ProcessRBDown();

		// Processes mouse right button released event
		void ProcessRBReleased();

		// Processes mouse middle button pressed event
		void ProcessMBPressed();

		// Processes mouse middle button down event
		void ProcessMBDown();

		// Processes mouse middle button released event
		void ProcessMBReleased();

		// Processes scrolling event
		void ProcessScrolling();

		// Processes key pressed event
		void ProcessKeyPressed(const Input::Key& key);

		// Processes key down event
		void ProcessKeyDown(const Input::Key& key);

		// Processes key released event
		void ProcessKeyReleased(const Input::Key& key);

	protected:
		float mDblClickTime = 0.3f; // Time between clicks for double click reaction

		Vector<CursorEventsListener*> mCursorListeners;     // All cursor non area listeners

		CursorAreaEventListenersLayer  mCursorAreaListenersBasicLayer; // Basic cursor area events listeners layer, for main screen
		CursorAreaEventListenersLayer* mCurrentCursorAreaEventsLayer;  // Current list of area listeners

		Map<CursorId, Vector<CursorAreaEventsListener*>> mPressedListeners;             // Pressed listeners for all pressed cursors
		Vector<CursorAreaEventsListener*>                mRightButtonPressedListeners;  // Right mouse button pressed listener
		Vector<CursorAreaEventsListener*>                mMiddleButtonPressedListeners; // Middle mouse button pressed listener

		Map<CursorId, Vector<CursorAreaEventsListener*>> mUnderCursorListeners;     // Under cursor listeners for each cursor
		Map<CursorId, Vector<CursorAreaEventsListener*>> mLastUnderCursorListeners; // Under cursor listeners for each cursor on last frame

		Vector<DragableObject*> mDragListeners; // Drag events listeners

		Vector<KeyboardEventsListener*>     mKeyboardListeners;    // Keyboard events listeners
		Vector<ApplicationEventsListener*>  mApplicationListeners; // Application events listeners

		ShortcutKeysListenersManager* mShortcutEventsManager; // Shortcut events manager

	protected:
		// Sets current cursor area events listeners layer
		static void SetCursorAreaEventsListenersLayer(CursorAreaEventListenersLayer* layer);

		// Registering cursor area events listener
		static void DrawnCursorAreaListener(CursorAreaEventsListener* listener);

		// Unregistering cursor area events listener
		static void UnregCursorAreaListener(CursorAreaEventsListener* listener);

		// Registering cursor events listener
		static void RegCursorListener(CursorEventsListener* listener);

		// Unregistering cursor events listener
		static void UnregCursorListener(CursorEventsListener* listener);

		// Registering drag events listener
		static void RegDragListener(DragableObject* listener);

		// Unregistering drag events listener
		static void UnregDragListener(DragableObject* listener);

		// Registering keyboard events listener
		static void RegKeyboardListener(KeyboardEventsListener* listener);

		// Unregistering keyboard events listener
		static void UnregKeyboardListener(KeyboardEventsListener* listener);

		// Registering application events listener
		static void RegApplicationListener(ApplicationEventsListener* listener);

		// Unregistering application events listener
		static void UnregApplicationListener(ApplicationEventsListener* listener);

		friend class Application;
		friend class ApplicationEventsListener;
		friend class CursorAreaEventListenersLayer;
		friend class CursorAreaEventsListener;
		friend class CursorEventsListener;
		friend class DragableObject;
		friend class KeyboardEventsListener;
		friend class WndProcFunc;
	};

	template<typename _type /*= CursorAreaEventsListener*/>
	_type* EventSystem::GetCursorListenerUnderCursor(CursorId cursorId) const
	{
		if (mUnderCursorListeners.ContainsKey(cursorId))
		{
			for (auto listener : mUnderCursorListeners.Get(cursorId))
			{
				if (auto tListener = dynamic_cast<_type*>(listener))
					return tListener;

				if (!listener->IsInputTransparent())
					break;
			}
		}

		return nullptr;
	}
}
