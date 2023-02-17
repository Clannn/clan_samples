#include "debug_info.h"

namespace clan
{
std::vector<std::unique_ptr<field::Base>> &DebugInfo::get_fields()
{
    return fields;
}

float DebugInfo::get_longest_label() const
{
	float column_width = 0.0f;
	for (auto &field : fields)
	{
		const std::string &label = field->label;

		if (label.size() > column_width)
		{
			column_width = static_cast<float>(label.size());
		}
	}
	return column_width;
}
}        // namespace clan