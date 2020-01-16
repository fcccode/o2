#pragma once

#include "o2/Utils/Serialization/DataNode.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2Editor/Core/Actions/IAction.h"

using namespace o2;

namespace o2
{
	class SceneEditableObject;
}

namespace Editor
{
	class DeleteAction: public IAction
	{
	public:
		class ObjectInfo: public ISerializable
		{
		public:
			DataNode objectData;   // @SERIALIZABLE
			SceneUID parentId;	   // @SERIALIZABLE
			SceneUID prevObjectId; // @SERIALIZABLE
			int      idx;          // @SERIALIZABLE

			bool operator==(const ObjectInfo& other) const;

			SERIALIZABLE(ObjectInfo);
		};

	public:
		Vector<ObjectInfo> objectsInfos;

	public:
		// Default constructor
		DeleteAction();

		// Constructor with objects, that will be deleted
		DeleteAction(const Vector<SceneEditableObject*>& objects);

		// Returns name of action
		String GetName() const;

		// Deletes objects again
		void Redo();

		// Reverting deleted objects
		void Undo();

		SERIALIZABLE(DeleteAction);
	};
}

CLASS_BASES_META(Editor::DeleteAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::DeleteAction)
{
	PUBLIC_FIELD(objectsInfos);
}
END_META;
CLASS_METHODS_META(Editor::DeleteAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

CLASS_BASES_META(Editor::DeleteAction::ObjectInfo)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::DeleteAction::ObjectInfo)
{
	PUBLIC_FIELD(objectData).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(parentId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(prevObjectId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(idx).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(Editor::DeleteAction::ObjectInfo)
{
}
END_META;