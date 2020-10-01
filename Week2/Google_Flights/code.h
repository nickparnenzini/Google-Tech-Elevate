#ifndef ELEVATE_CODE_H_
#define ELEVATE_CODE_H_

#include <map>
#include <string>

namespace elevate {
class Edge;

class Node {
 public:
  std::string code;
  std::map<std::string, Edge> connections;

  Node(std::string code_) { code = code_; }
  Node() {}
};

class Edge {
 public:
  Node node;
  int price;

  Edge(int price_, Node node_) {
    price = price_;
    node = node_;
  }
};
}  // namespace elevate

#endif