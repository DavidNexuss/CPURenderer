#include "node.hpp"


namespace vc {

  void Node::setOutput(DataValue* value, int outputSlot) {
    outputValues.resize(std::max(int(outputValues.size()), outputSlot+ 1));
    outputValues[outputValues] = std::make_unique<DataValue>(value);
  }

  void Node::computeOutput(int frame) {
    if(currentFrame != frame) icomputeOutput();
    currentFrame = frame;
  }

  void Node::addInputNode(Node* input, int inputSlot) {
    inputNodes.resize(std::max(int(inputNodes.size()), inputSlot + 1));
    inputNodes[inputSlot] = std::make_shared<Node>(input);
  }

  int Node::getInputCount() {
    return inputNodes.size();
  }

  int Node::getOutputCount() {
    return outputValues.size();
  }
}
