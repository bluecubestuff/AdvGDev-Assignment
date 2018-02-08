#ifndef SCENE_H
#define SCENE_H

class Scene
{
protected:
	Scene* prevScene;

public:
	Scene() : prevScene(nullptr) {}
	virtual ~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	inline Scene* GetPrevScene() { return prevScene; }
	inline void SetPrevScene(Scene* _scene) { prevScene = _scene; }
};

#endif // SCENE_H