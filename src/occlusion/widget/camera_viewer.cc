#include "occlusion/widget/camera_viewer.h"

#include "occlusion/engine.h"

namespace occlusion
{
CameraViewer::CameraViewer(Engine* engine)
  : Widget(engine)
{
}

CameraViewer::CameraViewer(Engine* engine, int x, int y, int width, int height)
  : Widget(engine, x, y, width, height)
{
}

CameraViewer::~CameraViewer()
{
}

void CameraViewer::Draw()
{
  SetViewport();
}
}
