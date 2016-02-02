#include "Scene.h"

#include "Scene/Actor.h"
#include "Scene/DrawableComponent.h"

namespace o2
{
	DECLARE_SINGLETON(Scene);

	Scene::Scene()
	{
		mDefaultLayer = AddLayer("Default");
	}

	Scene::~Scene()
	{
		Clear();

		for (auto layer : mLayers)
			delete layer;
	}

	void Scene::Update(float dt)
	{
		for (auto actor : mActors)
			actor->Update(dt);

		for (auto actor : mActors)
			actor->UpdateChilds(dt);
	}

	void Scene::Draw()
	{
		for (auto layer : mLayers)
		{
			for (auto comp : layer->enabledDrawables)
				comp->Draw();
		}
	}

#if IS_EDITOR
	void Scene::OnActorChanged(Actor* actor)
	{
		if (!mChangedActors.Contains(actor))
			mChangedActors.Add(actor);
	}

	void Scene::CheckChangedActors()
	{
		if (mChangedActors.Count() > 0)
		{
			onChanged(mChangedActors);
			mChangedActors.Clear();
		}
	}
#endif

	Scene::Layer* Scene::GetLayer(const String& name) const
	{
		return mLayers.FindMatch([&](auto x) { return x->name == name; });
	}

	Scene::Layer* Scene::GetDefaultLayer() const
	{
		return mDefaultLayer;
	}

	Scene::Layer* Scene::AddLayer(const String& name)
	{
		if (GetLayer(name))
			return AddLayer(name + "_");

		Layer* newLayer = mnew Layer();
		newLayer->name = name;
		mLayers.Add(newLayer);
		return newLayer;
	}

	void Scene::RemoveLayer(Layer* layer, bool removeActors /*= true*/)
	{
		if (layer == mDefaultLayer)
			return;

		if (removeActors)
		{
			auto actors = layer->actors;
			for (auto actor : actors)
				delete actor;
		}

		mLayers.Remove(layer);

		delete layer;
	}

	void Scene::RemoveLayer(const String& name, bool removeActors /*= true*/)
	{
		RemoveLayer(GetLayer(name), removeActors);
	}

	Scene::LayersVec& Scene::GetLayers()
	{
		return mLayers;
	}

	const Scene::ActorsVec& Scene::GetAllActors() const
	{
		return mActors;
	}

	Scene::ActorsVec& Scene::GetAllActors()
	{
		return mActors;
	}

	Actor* Scene::FindActor(const String& path)
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

		for (auto actor : mActors)
		{
			if (actor->mName == pathPart)
			{
				if (delPos == -1)
					return actor;
				else
					return actor->GetChild(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	void Scene::Clear()
	{
		auto allActors = mActors;
		for (auto actor : allActors)
			delete actor;
	}

	void Scene::Load(const String& path, bool append /*= false*/)
	{
		if (!append)
			Clear();

		DataNode data;
		data.LoadFromFile(path);
		ActorsVec loadActors = *data["actors"];
	}

	void Scene::Save(const String& path)
	{
		DataNode data;
		data = mActors;
		data.SaveToFile(path);
	}

	void Scene::Layer::RegDrawableComponent(DrawableComponent* component)
	{
		drawables.Add(component);

		if (component->mResEnabled)
			ComponentEnabled(component);
	}

	void Scene::Layer::UnregDrawableComponent(DrawableComponent* component)
	{
		if (component->mResEnabled)
			ComponentDisabled(component);

		drawables.Remove(component);
	}

	void Scene::Layer::ComponentDepthChanged(DrawableComponent* component)
	{
		ComponentDisabled(component);
		ComponentEnabled(component);
	}

	void Scene::Layer::ComponentEnabled(DrawableComponent* component)
	{
		const int binSearchRangeSizeStop = 5;
		int rangeMin = 0, rangeMax = enabledDrawables.Count();
		float targetDepth = component->mDrawingDepth;
		int position = 0;
		bool skipLinearSearch = false;

		while (rangeMax - rangeMin > binSearchRangeSizeStop)
		{
			int center = (rangeMin + rangeMax) >> 1;

			float centerValue = enabledDrawables[center]->mDrawingDepth;

			if (targetDepth < centerValue)
				rangeMax = center;
			else if (targetDepth > centerValue)
				rangeMin = center;
			else
			{
				position = center;
				skipLinearSearch = true;
				break;
			}
		}

		if (!skipLinearSearch)
		{
			for (position = rangeMin; position < rangeMax; position++)
				if (enabledDrawables[position]->mDrawingDepth > targetDepth)
					break;
		}

		enabledDrawables.Insert(component, position);
	}

	void Scene::Layer::ComponentDisabled(DrawableComponent* component)
	{
		enabledDrawables.Remove(component);
	}

}