#include "DefaultActorTransformViewer.h"

namespace Editor
{
	DefaultActorTransformViewer::DefaultActorTransformViewer()
	{

	}

	DefaultActorTransformViewer::~DefaultActorTransformViewer()
	{

	}

	void DefaultActorTransformViewer::SetTargetActors(const Vector<Actor*>& actors)
	{

	}

}
 
CLASS_META(Editor::DefaultActorTransformViewer)
{
	BASE_CLASS(Editor::IActorTransformViewer);


	PUBLIC_FUNCTION(void, SetTargetActors, const Vector<Actor*>&);
}
END_META;
 