#include "occlusion/engine.h"

#include <iostream>

int main()
{
  occlusion::Engine engine;

  try
  {
    engine.Run();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
