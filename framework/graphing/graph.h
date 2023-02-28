#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include <json.hpp>

#include "graph_node.h"

namespace clan
{
    namespace graphing
    {
        struct Edge
        {
            size_t id;

            size_t from;

            size_t to;

            nlohmann::json options;

            Edge(size_t id_, size_t s, size_t t) :
                id(id_),
                from(s),
                to(t){};
        };

        /**
         * @brief Graph is an implementation of an adjacency list graph. The nodes are created from a variadic function and their implementation is defined by the given NodeType
         * 
         * @tparam NodeType either FrameworkNode or SceneNode
         */
        class Graph
        {
        public:
            static const size_t node_not_found = 0;

            Graph(const char *name);

            /**
             * @brief Create a new style
             * 
             * @param style_name the group name
             * @param color the hex color of the group
             */
            void new_style(std::string style_name, std::string color);

            /**
             * @brief Create a node object
             * 
             * @param title of node
             * @param style corresponds to the key used when using new_type(style, color)
             * @param data json data to be displayed with node
             * @return size_t id of node
             */
            size_t create_node(const char *title = "Node", const char *style = NULL, const nlohmann::json &data = {})
            {
                size_t id = new_id();
                nodes[id] = std::make_unique<Node>(id, title, style, data);
                return id;
            }

        private:
            std::string name;

            size_t next_id = 1;

            std::vector<Edge> adj;
            
            std::unordered_map<size_t, std::unique_ptr<Node>> nodes;

            std::unordered_map<std::string, size_t> refs;

            std::unordered_map<std::string, std::string> style_colors;
        };
    }
}