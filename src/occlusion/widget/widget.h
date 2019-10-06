#ifndef OCCLUSION_WIDGET_WIDGET_H_
#define OCCLUSION_WIDGET_WIDGET_H_

namespace occlusion
{
class Engine;

class Widget
{
public:
  Widget(Engine* engine);
  Widget(Engine* engine, int x, int y, int width, int height);
  virtual ~Widget();

  bool IsShown()
  {
    return show_;
  }

  void Hide()
  {
    show_ = false;
  }

  void Show()
  {
    show_ = true;
  }

  void Resize(int x, int y, int width, int height);

  virtual void Draw();

protected:
  auto GetEngine()
  {
    return engine_;
  }

  void SetViewport();

private:
  Engine* engine_;

  bool show_ = true;

  // Window
  int x_ = 0;
  int y_ = 0;
  int width_ = 200;
  int height_ = 200;
};
}

#endif // OCCLUSION_WIDGET_WIDGET_H_
