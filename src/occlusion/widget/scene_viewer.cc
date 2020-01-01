#include "occlusion/widget/scene_viewer.h"

#include "occlusion/engine.h"

namespace occlusion
{
SceneViewer::SceneViewer(Engine* engine)
  : Widget(engine)
{
}

SceneViewer::SceneViewer(Engine* engine, int x, int y, int width, int height)
  : Widget(engine, x, y, width, height)
{
}

SceneViewer::~SceneViewer()
{
}

void SceneViewer::Draw()
{
  SetViewport();
}
}
