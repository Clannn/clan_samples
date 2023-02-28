#include "graphing/graph_node.h"

namespace clan
{
    namespace graphing
    {
        Node::Node(size_t id, const char* title, const char* style, const nlohmann::json& data)
        {
            attributes["id"]    = id;
            attributes["label"] = title;
            attributes["data"]  = data;
            attributes["style"] = style;
        }
    }
}