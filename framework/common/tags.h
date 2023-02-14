#pragma once

#include <algorithm>
#include <vector>

/**
 * @brief Used to represent a tag
*/
typedef void (*TagID)();

/**
 * @brief Tags acts as a unique identifier to categorizes objects
 * 
 * Tags are uniquely defined using different type names. The easiest way of creating a new tag is to use an empty struct
 * struct TagName{};
 * struct DifferentTag{};
 * Tag<TagName>::ID == Tag<TagName>::member != Tag<DifferentTag>:ID
 * 
 * @tparam TAGS - A set of tags
*/
template <typename... TAGS>
class Tag
{
public:
    Tag()
    {
        m_tags = {Tag<TAGS>::ID...};
    }

    static void member() {};

     /**
      * @brief Unique TagID for a given Tag<TagName>
     */
    constexpr static TagID ID = member;

    static bool has_tag(TagID id)
    {
        return std::find(m_tags.begin(), m_tags.end(), id) != m_tags.end();
    }

    template <typename C>
    static bool has_tag()
    {
        return has_tag(Tag<C>::ID);
    }

    template <typename... C>
    static bool has_tags()
    {
        std::vector<TagID> query = {Tag<C>::ID...};
        bool res = true;
        for(auto id : query){
            res &= has_tag(id);
        }
        return res;
    }

private:
    static std::vector<TagID> m_tags;
};
