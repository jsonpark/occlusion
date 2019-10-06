#include "occlusion/widget/widget.h"

#include <glad/glad.h>

namespace occlusion
{
Widget::Widget(Engine* engine)
  : engine_(engine)
{
}

Widget::Widget(Engine* engine, int x, int y, int width, int height)
  : engine_(engine), x_(x), y_(y), width_(width), height_(height)
{
}

Widget::~Widget()
{
}

void Widget::Resize(int x, int y, int width, int height)
{
  x_ = x;
  y_ = y;
  width_ = width;
  height_ = height;
}

void Widget::Draw()
{
}

void Widget::SetViewport()
{
  glViewport(x_, y_, width_, height_);
}
}
