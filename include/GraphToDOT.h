//
// Created by nrx on 23.04.2020.
//

#ifndef LABA2_GRAPHTODOT_H
#define LABA2_GRAPHTODOT_H
#include "Graph.h"
#include <sstream>
#include <queue>
#include <set>
//
//
//template<typename NodeType, typename EdgeType>
//std::ostream& printLink(std::ostream &stream, const typename Graph<NodeType, EdgeType>::Node& n,
//        const typename Graph<NodeType, EdgeType>::Link &l) {
//    stream << '"' <<n.data << '"' << " -> " << l.node.getData() << " [label=" << l.data << "]\n";
//    return stream;
//}
//
//template<typename EdgeType>
//std::ostream& printLink(std::ostream &stream, const typename Graph<Empty, EdgeType>::Node& n,
//                        const typename Graph<Empty, EdgeType>::Link &l) {
//    stream << '"' <<n.index << '"' << " -> " << l.node.getData() << " [label=" << l.data << "]\n";
//    return stream;
//}
//
//
//template<typename NodeType, typename EdgeType>
//std::string graphToDOT(Graph<NodeType, EdgeType> graph) {
//    using Graph_t = Graph<NodeType, EdgeType>;
//    std::stringstream stream;
//    stream << "digraph {\n";
//    std::set<std::reference_wrapper<typename Graph_t::Node>> visitedNode;
//    for(auto start_node : graph.getNodes()) {
//        if(visitedNode.find(std::ref(start_node)) == visitedNode.end()) {
//            std::queue<std::reference_wrapper<typename Graph_t::Node>> neighbors;
//            neighbors.push(std::ref(start_node));
//            while (!neighbors.empty()) {
//                auto n = neighbors.front();
//                visitedNode.insert(n);
//                neighbors.pop();
//                for(typename Graph_t::Link &l : n.get().links) {
//                    printLink<EdgeType>(static_cast<std::ostream&>(stream), n.get(), l);
//                    typename Graph_t::Node findNode = l.node.getNode();
//                    if(visitedNode.find(std::ref(findNode))  == visitedNode.end()) {
//                        neighbors.push(std::ref(l.node));
//                    }
//
//                }
//            }
//
//        }
//    }
//
//    stream << "}\n";
//    return stream.str();
//}

template<typename NodeType, typename EdgeType>
std::ostream& printLink(std::ostream &stream, const typename Graph<NodeType, EdgeType>::Node& n,
        const typename Graph<NodeType, EdgeType>::Link &l) {
    stream << '"' <<n.data << '"' << " -> " << l.node.getData() << " [label=\"" << l.data << "\"]\n";
    return stream;
}

template<typename EdgeType>
std::ostream& printLink(std::ostream &stream, const typename Graph<Empty, EdgeType>::Node& n,
                        const typename Graph<Empty, EdgeType>::Link &l) {
    stream << '"' << n.index << '"' << " -> \"" << l.node.getIndex() << "\" [label=\"" << l.data << "\"]\n";
    return stream;
}

template<typename NodeType, typename EdgeType>
std::ostream& printNode(std::ostream &stream, const typename Graph<NodeType, EdgeType>::Node& n) {
    stream << '"' << n.data << '"' << "\n";
    return stream;
}

template<typename EdgeType>
std::ostream& printNode(std::ostream &stream, const typename Graph<Empty, EdgeType>::Node& n) {
    stream << '"' << n.index << '"' << "\n";
    return stream;
}


template<typename NodeType, typename EdgeType>
std::string graphToDOT(Graph<NodeType, EdgeType> graph) {
    std::stringstream stream;
    stream << "digraph {\n";
    for(typename Graph<NodeType, EdgeType>::Node &node : graph.getNodes()) {
        for(typename Graph<NodeType, EdgeType>::Link &link : node.links) {
            printLink<EdgeType>(static_cast<std::ostream&>(stream), node, link);
        }
        if(node.links.empty()) {
            printNode<EdgeType>(static_cast<std::ostream&>(stream), node);
        }
    }
    stream << "}\n";
    return stream.str();
}




#endif //LABA2_GRAPHTODOT_H
