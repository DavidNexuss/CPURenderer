#ifndef TEXTURENODE_HPP
#define TEXTURENODE_HPP

#include "node.hpp"
namespace vc {
class TextureNode : public Node{

public:
  TextureNode(const std::string& path);
  inline virtual void icomputeOutput() override { }
};
}

#endif // TEXTURENODE_HPP
