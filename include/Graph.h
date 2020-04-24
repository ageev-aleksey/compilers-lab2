//
// Created by nrx on 20.03.2020.
//

#ifndef STATE_MACHINE_FINITESTATEMACHINE_H
#define STATE_MACHINE_FINITESTATEMACHINE_H

#include <iostream>
#include<list>
#include <set>
#include "RegexExceptions.h"

struct Empty {

};


template <typename NodeData, typename LinkData>
class Graph {

public:
    class iterator;
    struct Node;
    struct Link {
       /* Link() {
        }*/
        Link(const LinkData &d, const iterator &n) : node(n), data(d) {

        }
        Link(const Link &link) : node(link.node), data(link.data){
            //std::cout << "Link copy constructor" << std::endl;
        }
        Link& operator=(const Link &link) {
            data = link.data;
            node = link.node;
        }
        LinkData data;
        iterator node;
    };
    struct Node {
        Node() {
        }
        Node(Node &&n) {
           // std::cout << "Node move constructor" << std::endl;
            data = n.data;
            links = n.links;
            index = n.index;
        }
        Node(const Node &n) {
           // std::cout << "Node copy constructor" << std::endl;
            data = n.data;
            links = n.links;
            index = n.index;
        }
        Node& operator=(const Node &n) {
           // std::cout << "Node operator =" << std::endl;
            data = n.data;
            links = n.links;
            index = n.index;
        }

        NodeData data;
        std::list<Link> links;
        size_t index;
    };
    class iterator {
    public:
        explicit iterator(typename std::list<Node>::iterator itr /*,Graph *const ptr*/) : currentNode(itr) /*ownerPtr(ptr)*/ {
        }

        iterator(const iterator &other) : //ownerPtr(other.ownerPtr),
         currentNode(other.currentNode) {
        }
        iterator& operator=(const iterator &other) {
            this->currentNode = other.currentNode;
            return *this;
        }

        bool operator==(const iterator &other) const {
            return this->currentNode == other.currentNode;
        }
        bool operator!=(const iterator &other) const {
            return this->currentNode != other.currentNode;
        }
        Node getNode() {
            return *currentNode;
        }

         std::list<Link>& getLinks()  {
            return currentNode->links;
        }

        const std::list<Link>& getLinks() const {
            return const_cast<iterator*>(this)->getLinks();
        }

        NodeData& getData() {
            return currentNode->data;
        }
        const NodeData& getData() const{
            return currentNode->data;
        }
        size_t getIndex() const {
            return currentNode->index;
        }
        bool isOwner(const Graph &graph) const{
            //return &graph == ownerPtr;
        }

        iterator& operator++() {
            currentNode++;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }


    private:

        typename std::list<Node>::iterator currentNode;
        // Graph *const ownerPtr;
        friend class Graph;
    };

    Graph(){
    }

    Graph(const Graph& g) {
//        nodes = g.nodes;
//        for(auto &el : nodes) {
//            for(typename std::list<Link>::iterator itr = el.links.begin(); itr != el.links.end(); ++itr) {
//                size_t index_linked_node = itr->node.getIndex();
//                for(typename std::list<Node>::iterator nodes_itr = nodes.begin();
//                    nodes_itr != nodes.end(); ++nodes_itr) {
//                    if(nodes_itr->index == index_linked_node) {
//                        itr->node = iterator(nodes_itr);
//                        break;
//                    }
//                }
//            }
//        }
      *this = g;

    }
    Graph& operator=(const Graph& g) {
        nodes.clear();
        nodes.resize(g.nodes.size());
        size_t i = 0;
        for(auto &node : nodes){
            node.index = i;
            i++;
        }

        for(auto oldNode = g.nodes.begin(), node = nodes.begin(); oldNode != g.nodes.end();
            ++oldNode, ++node) {
            node->data = oldNode->data;
            for(auto link = oldNode->links.begin(); link != oldNode->links.end();
                ++link) {
                Link l(link->data, getNodeByIndex(link->node.getIndex()));
                node->links.push_back(l);
            }

        }
        return *this;
    }

    void addLink(iterator vertexBegin, iterator vertexEnd, const LinkData data) {
       /* if(!vertexBegin.isOwner(*this) || !vertexEnd.isOwner(*this)) {
            throw InvalidVertexException("The transmitted vertex does not belong to this graph");
        }*/

        vertexBegin.getLinks().push_back({data, vertexEnd});
    }

    iterator addNodeInFront(const NodeData &data) {
        //return addNode(nodes.begin(), data);
        Node n;
        n.data = data;
        nodes.push_front(n);
        size_t  i = 0;
        for(auto &el : nodes) {
            el.index = i;
            i++;
        }
        return iterator(nodes.begin());
    }

    iterator addNodeInBack(const NodeData &data) {
       // return addNode(nodes.end(), data);
       Node n;
       n.data = data;
       if(nodes.empty()) {
           n.index = 0;
           nodes.push_back(n);
           return  iterator(nodes.begin());
       } else {
           n.index = nodes.back().index + 1;
           nodes.push_back(n);
           return iterator(--nodes.end());
       }
    }

    iterator getNodeByIndex(size_t index) {
        auto itr = nodes.begin();
        for(size_t i = 0; i < index; i++) {
            itr++;
        }
        return iterator(itr);
    }

    iterator end() {
        return iterator(nodes.end());
    }

    iterator begin() {
        return iterator(nodes.begin());
    }

    iterator convertIterator(const typename std::list<Node>::iterator &lnitr) {
        return iterator(lnitr);
    }

//    iterator addNode(iterator pos, const NodeData &data) {
//        return addNode(pos.currentNode, data);
//    }
//
//    iterator addNode(typename std::list<Node>::iterator pos,  const NodeData &data) {
//        Node n;
//        n.data = data;
//        n.index = 0;
//        auto newNode = nodes.insert(pos, n);
//        auto itr = newNode;
//        size_t index = 0;
//        if(itr == --nodes.end()) {
//            index == (--itr)->index+1;
//            (++itr)->index = index;
//            return newNode;
//        } else if(itr != nodes.begin()) {
//            index  = (--itr)->index+1;
//            (++itr)->index = index;
//            ++itr;
//            ++index;
//        }
//        for(; itr != nodes.end(); itr++) {
//            itr->index = index;
//            index++;
//        }
//
//        return iterator(newNode);
//    }

     std::list<Node>& getNodes() {
        return nodes;
    }
    typename std::list<Node>::iterator getNodesBeginIterator() {
        return nodes.begin();
    }
    typename std::list<Node>::iterator getNodesEndIterator() {
        return nodes.end();
    }
    iterator firstNode() {
        if(nodes.empty()) {
            throw GraphException("Grpah is empty");
        }
        return iterator(nodes.begin());
    }
    const iterator firstNode() const {
        return const_cast<Graph*>(this)->firstNode();
    }

    iterator lastNode() {
        if(nodes.empty()) {
            throw GraphException("Grpah is empty");
        }
        return iterator(--(nodes.end()));
    }

    const iterator lastNode() const {
        return const_cast<Graph*>(this)->lastNode();
    }

    /*void splice(iterator pos, Graph &&g) {
        nodes.splice(nodes.end(), g.nodes);
    }*/

    /*static Graph concatenate(Graph &first, iterator nodeFirstGraph,Graph &second, iterator nodeSecondGraph, LinkData data) {
       Graph res;
       res.nodes.splice(res.nodes.end(), first.nodes);
       res.nodes.splice(res.nodes.end(), second.nodes);
       //res.addLink(nodeFirstGraph, nodeSecondGraph);
       nodeFirstGraph.currentNode->links.push_back({data, nodeSecondGraph});
        int i = 0;
        for(auto &el : res.nodes) {
            el.index = i;
            i++;
        }
       return res;
    }*/

    void concatenate(Graph &other) {
        auto itr = --nodes.end();
        nodes.splice(nodes.end(), other.nodes);
        int i = 0;
        for(auto &el : nodes) {
            el.index = i;
            i++;
        }
    }

    size_t numNodes() {
        return nodes.size();
    }

    std::set<LinkData> getUniqueLinkData() {
        std::set<LinkData> result;
        for(auto &el : nodes) {
            for(auto link : el.links) {
                result.insert(link.data);
            }
        }
        return result;
    }

private:
    std::list<Node> nodes;
};




#endif //STATE_MACHINE_FINITESTATEMACHINE_H
