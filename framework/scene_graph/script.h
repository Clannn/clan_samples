#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include "platform/input_events.h"
#include "scene_graph/component.h"

namespace clan
{
    namespace sg
    {
/**
 * @brief Generic structure to receive platform events.
 *        Used for adding game logic to scene graph objects.
 */
class Script : public Component
{
public:
	Script(const std::string &name = "");

	virtual ~Script() = default;

	virtual std::type_index get_type() override;

	/**
	 * @brief Main loop script events
	 */
	virtual void update(float delta_time) = 0;

	virtual void input_event(const InputEvent &input_event);

	virtual void resize(uint32_t width, uint32_t height);
};

class NodeScript : public Script
{
  public:
	NodeScript(Node &node, const std::string &name = "");

	virtual ~NodeScript() = default;

	Node &get_node();

  private:
	Node &node;
};
    }
}