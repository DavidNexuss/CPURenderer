#include "texturenode.hpp"
#include <stb/stb_image.h>
#include "../gl.hpp"

namespace vc {
TextureNode::TextureNode(const std::string& path)
{
  int width;
  int height;
  int channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 3);

  int channelSize = 1;
  GLuint glObject = -1;
  glObject = gl::uploadTexture(width, height, channels,channelSize, data,glObject);

  TextureValue* value = new TextureValue;
  value->channel = 0;
  value->glObject = glObject;
  value->sourceFile = path;
  value->format.channels = channels;
  value->format.width = width;
  value->format.height = height;
  value->format.channelsSize = channelSize;

  setOutput(value, 0);
}
}
