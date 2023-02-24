#ifndef NODE_HPP
#define NODE_HPP
#include <vector>
#include <memory>
#include <string>
#include "data.hpp"

namespace vc {

class NodeClass {
  std::vector<std::string> inputLabels;
  std::vector<std::string> outputLabels;
};

class Node {
  std::vector<std::shared_ptr<Node>> inputNodes;
  std::vector<std::unique_ptr<DataValue>> outputValues;
  int currentFrame = -1;
  NodeClass* nodeClass;

protected:
  void setOutput(DataValue* value, int outputSlot);
  void computeOutput(int frame);

  virtual void icomputeOutput() = 0;
public:

  Node();
  void addInputNode(Node* input, int inputSlot);
  DataValue& getOutput(int outputSlot, int frame);

  int getInputCount();
  int getOutputCount();

  inline const std::vector<std::shared_ptr<Node>>& getNodes() const { return inputNodes; }
  inline const NodeClass* getClass() const { return nodeClass; }
};
}
#endif // NODE_HPP
