#pragma once

#include "core/descriptor_pool.h"
#include "core/descriptor_set.h"
#include "core/descriptor_set_layout.h"
#include "core/framebuffer.h"
#include "core/pipeline.h"
#include "rendering/pipeline_state.h"
#include "rendering/render_target.h"
#include "resource_record.h"

#include "common/helpers.h"

namespace std
{
template <>
struct hash<clan::ShaderSource>
{
	std::size_t operator()(const clan::ShaderSource &shader_source) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, shader_source.get_id());

		return result;
	}
};

template <>
struct hash<clan::ShaderVariant>
{
	std::size_t operator()(const clan::ShaderVariant &shader_variant) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, shader_variant.get_id());

		return result;
	}
};

template <>
struct hash<clan::ShaderModule>
{
	std::size_t operator()(const clan::ShaderModule &shader_module) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, shader_module.get_id());

		return result;
	}
};

template <>
struct hash<clan::DescriptorSetLayout>
{
	std::size_t operator()(const clan::DescriptorSetLayout &descriptor_set_layout) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, descriptor_set_layout.get_handle());

		return result;
	}
};

template <>
struct hash<clan::DescriptorPool>
{
	std::size_t operator()(const clan::DescriptorPool &descriptor_pool) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, descriptor_pool.get_descriptor_set_layout());

		return result;
	}
};

template <>
struct hash<clan::PipelineLayout>
{
	std::size_t operator()(const clan::PipelineLayout &pipeline_layout) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, pipeline_layout.get_handle());

		return result;
	}
};

template <>
struct hash<clan::RenderPass>
{
	std::size_t operator()(const clan::RenderPass &render_pass) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, render_pass.get_handle());

		return result;
	}
};

template <>
struct hash<clan::Attachment>
{
	std::size_t operator()(const clan::Attachment &attachment) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, static_cast<std::underlying_type<VkFormat>::type>(attachment.format));
		clan::hash_combine(result, static_cast<std::underlying_type<VkSampleCountFlagBits>::type>(attachment.samples));
		clan::hash_combine(result, attachment.usage);
		clan::hash_combine(result, static_cast<std::underlying_type<VkImageLayout>::type>(attachment.initial_layout));

		return result;
	}
};

template <>
struct hash<clan::LoadStoreInfo>
{
	std::size_t operator()(const clan::LoadStoreInfo &load_store_info) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, static_cast<std::underlying_type<VkAttachmentLoadOp>::type>(load_store_info.load_op));
		clan::hash_combine(result, static_cast<std::underlying_type<VkAttachmentStoreOp>::type>(load_store_info.store_op));

		return result;
	}
};

template <>
struct hash<clan::SubpassInfo>
{
	std::size_t operator()(const clan::SubpassInfo &subpass_info) const
	{
		std::size_t result = 0;

		for (uint32_t output_attachment : subpass_info.output_attachments)
		{
			clan::hash_combine(result, output_attachment);
		}

		for (uint32_t input_attachment : subpass_info.input_attachments)
		{
			clan::hash_combine(result, input_attachment);
		}

		for (uint32_t resolve_attachment : subpass_info.color_resolve_attachments)
		{
			clan::hash_combine(result, resolve_attachment);
		}

		clan::hash_combine(result, subpass_info.disable_depth_stencil_attachment);
		clan::hash_combine(result, subpass_info.depth_stencil_resolve_attachment);
		clan::hash_combine(result, subpass_info.depth_stencil_resolve_mode);

		return result;
	}
};

template <>
struct hash<clan::SpecializationConstantState>
{
	std::size_t operator()(const clan::SpecializationConstantState &specialization_constant_state) const
	{
		std::size_t result = 0;

		for (auto constants : specialization_constant_state.get_specialization_constant_state())
		{
			clan::hash_combine(result, constants.first);
			for (const auto data : constants.second)
			{
				clan::hash_combine(result, data);
			}
		}

		return result;
	}
};

template <>
struct hash<clan::ShaderResource>
{
	std::size_t operator()(const clan::ShaderResource &shader_resource) const
	{
		std::size_t result = 0;

		if (shader_resource.type == clan::ShaderResourceType::Input ||
		    shader_resource.type == clan::ShaderResourceType::Output ||
		    shader_resource.type == clan::ShaderResourceType::PushConstant ||
		    shader_resource.type == clan::ShaderResourceType::SpecializationConstant)
		{
			return result;
		}

		clan::hash_combine(result, shader_resource.set);
		clan::hash_combine(result, shader_resource.binding);
		clan::hash_combine(result, static_cast<std::underlying_type<clan::ShaderResourceType>::type>(shader_resource.type));
		clan::hash_combine(result, shader_resource.mode);

		return result;
	}
};

template <>
struct hash<VkDescriptorBufferInfo>
{
	std::size_t operator()(const VkDescriptorBufferInfo &descriptor_buffer_info) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, descriptor_buffer_info.buffer);
		clan::hash_combine(result, descriptor_buffer_info.range);
		clan::hash_combine(result, descriptor_buffer_info.offset);

		return result;
	}
};

template <>
struct hash<VkDescriptorImageInfo>
{
	std::size_t operator()(const VkDescriptorImageInfo &descriptor_image_info) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, descriptor_image_info.imageView);
		clan::hash_combine(result, static_cast<std::underlying_type<VkImageLayout>::type>(descriptor_image_info.imageLayout));
		clan::hash_combine(result, descriptor_image_info.sampler);

		return result;
	}
};

template <>
struct hash<VkWriteDescriptorSet>
{
	std::size_t operator()(const VkWriteDescriptorSet &write_descriptor_set) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, write_descriptor_set.dstSet);
		clan::hash_combine(result, write_descriptor_set.dstBinding);
		clan::hash_combine(result, write_descriptor_set.dstArrayElement);
		clan::hash_combine(result, write_descriptor_set.descriptorCount);
		clan::hash_combine(result, write_descriptor_set.descriptorType);

		switch (write_descriptor_set.descriptorType)
		{
			case VK_DESCRIPTOR_TYPE_SAMPLER:
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
			case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
				for (uint32_t i = 0; i < write_descriptor_set.descriptorCount; i++)
				{
					clan::hash_combine(result, write_descriptor_set.pImageInfo[i]);
				}
				break;

			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
			case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
				for (uint32_t i = 0; i < write_descriptor_set.descriptorCount; i++)
				{
					clan::hash_combine(result, write_descriptor_set.pTexelBufferView[i]);
				}
				break;

			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
				for (uint32_t i = 0; i < write_descriptor_set.descriptorCount; i++)
				{
					clan::hash_combine(result, write_descriptor_set.pBufferInfo[i]);
				}
				break;

			default:
				// Not implemented
				break;
		};

		return result;
	}
};

template <>
struct hash<VkVertexInputAttributeDescription>
{
	std::size_t operator()(const VkVertexInputAttributeDescription &vertex_attrib) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, vertex_attrib.binding);
		clan::hash_combine(result, static_cast<std::underlying_type<VkFormat>::type>(vertex_attrib.format));
		clan::hash_combine(result, vertex_attrib.location);
		clan::hash_combine(result, vertex_attrib.offset);

		return result;
	}
};

template <>
struct hash<VkVertexInputBindingDescription>
{
	std::size_t operator()(const VkVertexInputBindingDescription &vertex_binding) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, vertex_binding.binding);
		clan::hash_combine(result, static_cast<std::underlying_type<VkVertexInputRate>::type>(vertex_binding.inputRate));
		clan::hash_combine(result, vertex_binding.stride);

		return result;
	}
};

template <>
struct hash<clan::StencilOpState>
{
	std::size_t operator()(const clan::StencilOpState &stencil) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, static_cast<std::underlying_type<VkCompareOp>::type>(stencil.compare_op));
		clan::hash_combine(result, static_cast<std::underlying_type<VkStencilOp>::type>(stencil.depth_fail_op));
		clan::hash_combine(result, static_cast<std::underlying_type<VkStencilOp>::type>(stencil.fail_op));
		clan::hash_combine(result, static_cast<std::underlying_type<VkStencilOp>::type>(stencil.pass_op));

		return result;
	}
};

template <>
struct hash<VkExtent2D>
{
	size_t operator()(const VkExtent2D &extent) const
	{
		size_t result = 0;

		clan::hash_combine(result, extent.width);
		clan::hash_combine(result, extent.height);

		return result;
	}
};

template <>
struct hash<VkOffset2D>
{
	size_t operator()(const VkOffset2D &offset) const
	{
		size_t result = 0;

		clan::hash_combine(result, offset.x);
		clan::hash_combine(result, offset.y);

		return result;
	}
};

template <>
struct hash<VkRect2D>
{
	size_t operator()(const VkRect2D &rect) const
	{
		size_t result = 0;

		clan::hash_combine(result, rect.extent);
		clan::hash_combine(result, rect.offset);

		return result;
	}
};

template <>
struct hash<VkViewport>
{
	size_t operator()(const VkViewport &viewport) const
	{
		size_t result = 0;

		clan::hash_combine(result, viewport.width);
		clan::hash_combine(result, viewport.height);
		clan::hash_combine(result, viewport.maxDepth);
		clan::hash_combine(result, viewport.minDepth);
		clan::hash_combine(result, viewport.x);
		clan::hash_combine(result, viewport.y);

		return result;
	}
};

template <>
struct hash<clan::ColorBlendAttachmentState>
{
	std::size_t operator()(const clan::ColorBlendAttachmentState &color_blend_attachment) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, static_cast<std::underlying_type<VkBlendOp>::type>(color_blend_attachment.alpha_blend_op));
		clan::hash_combine(result, color_blend_attachment.blend_enable);
		clan::hash_combine(result, static_cast<std::underlying_type<VkBlendOp>::type>(color_blend_attachment.color_blend_op));
		clan::hash_combine(result, color_blend_attachment.color_write_mask);
		clan::hash_combine(result, static_cast<std::underlying_type<VkBlendFactor>::type>(color_blend_attachment.dst_alpha_blend_factor));
		clan::hash_combine(result, static_cast<std::underlying_type<VkBlendFactor>::type>(color_blend_attachment.dst_color_blend_factor));
		clan::hash_combine(result, static_cast<std::underlying_type<VkBlendFactor>::type>(color_blend_attachment.src_alpha_blend_factor));
		clan::hash_combine(result, static_cast<std::underlying_type<VkBlendFactor>::type>(color_blend_attachment.src_color_blend_factor));

		return result;
	}
};

template <>
struct hash<clan::RenderTarget>
{
	std::size_t operator()(const clan::RenderTarget &render_target) const
	{
		std::size_t result = 0;

		for (auto &view : render_target.get_views())
		{
			clan::hash_combine(result, view.get_handle());
			clan::hash_combine(result, view.get_image().get_handle());
		}

		return result;
	}
};

template <>
struct hash<clan::PipelineState>
{
	std::size_t operator()(const clan::PipelineState &pipeline_state) const
	{
		std::size_t result = 0;

		clan::hash_combine(result, pipeline_state.get_pipeline_layout().get_handle());

		// For graphics only
		if (auto render_pass = pipeline_state.get_render_pass())
		{
			clan::hash_combine(result, render_pass->get_handle());
		}

		clan::hash_combine(result, pipeline_state.get_specialization_constant_state());

		clan::hash_combine(result, pipeline_state.get_subpass_index());

		for (auto shader_module : pipeline_state.get_pipeline_layout().get_shader_modules())
		{
			clan::hash_combine(result, shader_module->get_id());
		}

		// VkPipelineVertexInputStateCreateInfo
		for (auto &attribute : pipeline_state.get_vertex_input_state().attributes)
		{
			clan::hash_combine(result, attribute);
		}

		for (auto &binding : pipeline_state.get_vertex_input_state().bindings)
		{
			clan::hash_combine(result, binding);
		}

		// VkPipelineInputAssemblyStateCreateInfo
		clan::hash_combine(result, pipeline_state.get_input_assembly_state().primitive_restart_enable);
		clan::hash_combine(result, static_cast<std::underlying_type<VkPrimitiveTopology>::type>(pipeline_state.get_input_assembly_state().topology));

		//VkPipelineViewportStateCreateInfo
		clan::hash_combine(result, pipeline_state.get_viewport_state().viewport_count);
		clan::hash_combine(result, pipeline_state.get_viewport_state().scissor_count);

		// VkPipelineRasterizationStateCreateInfo
		clan::hash_combine(result, pipeline_state.get_rasterization_state().cull_mode);
		clan::hash_combine(result, pipeline_state.get_rasterization_state().depth_bias_enable);
		clan::hash_combine(result, pipeline_state.get_rasterization_state().depth_clamp_enable);
		clan::hash_combine(result, static_cast<std::underlying_type<VkFrontFace>::type>(pipeline_state.get_rasterization_state().front_face));
		clan::hash_combine(result, static_cast<std::underlying_type<VkPolygonMode>::type>(pipeline_state.get_rasterization_state().polygon_mode));
		clan::hash_combine(result, pipeline_state.get_rasterization_state().rasterizer_discard_enable);

		// VkPipelineMultisampleStateCreateInfo
		clan::hash_combine(result, pipeline_state.get_multisample_state().alpha_to_coverage_enable);
		clan::hash_combine(result, pipeline_state.get_multisample_state().alpha_to_one_enable);
		clan::hash_combine(result, pipeline_state.get_multisample_state().min_sample_shading);
		clan::hash_combine(result, static_cast<std::underlying_type<VkSampleCountFlagBits>::type>(pipeline_state.get_multisample_state().rasterization_samples));
		clan::hash_combine(result, pipeline_state.get_multisample_state().sample_shading_enable);
		clan::hash_combine(result, pipeline_state.get_multisample_state().sample_mask);

		// VkPipelineDepthStencilStateCreateInfo
		clan::hash_combine(result, pipeline_state.get_depth_stencil_state().back);
		clan::hash_combine(result, pipeline_state.get_depth_stencil_state().depth_bounds_test_enable);
		clan::hash_combine(result, static_cast<std::underlying_type<VkCompareOp>::type>(pipeline_state.get_depth_stencil_state().depth_compare_op));
		clan::hash_combine(result, pipeline_state.get_depth_stencil_state().depth_test_enable);
		clan::hash_combine(result, pipeline_state.get_depth_stencil_state().depth_write_enable);
		clan::hash_combine(result, pipeline_state.get_depth_stencil_state().front);
		clan::hash_combine(result, pipeline_state.get_depth_stencil_state().stencil_test_enable);

		// VkPipelineColorBlendStateCreateInfo
		clan::hash_combine(result, static_cast<std::underlying_type<VkLogicOp>::type>(pipeline_state.get_color_blend_state().logic_op));
		clan::hash_combine(result, pipeline_state.get_color_blend_state().logic_op_enable);

		for (auto &attachment : pipeline_state.get_color_blend_state().attachments)
		{
			clan::hash_combine(result, attachment);
		}

		return result;
	}
};
}        // namespace std

namespace clan
{
namespace
{
template <typename T>
inline void hash_param(size_t &seed, const T &value)
{
	hash_combine(seed, value);
}

template <>
inline void hash_param(size_t & /*seed*/, const VkPipelineCache & /*value*/)
{
}

template <>
inline void hash_param<std::vector<uint8_t>>(
    size_t &                    seed,
    const std::vector<uint8_t> &value)
{
	hash_combine(seed, std::string{value.begin(), value.end()});
}

template <>
inline void hash_param<std::vector<Attachment>>(
    size_t &                       seed,
    const std::vector<Attachment> &value)
{
	for (auto &attachment : value)
	{
		hash_combine(seed, attachment);
	}
}

template <>
inline void hash_param<std::vector<LoadStoreInfo>>(
    size_t &                          seed,
    const std::vector<LoadStoreInfo> &value)
{
	for (auto &load_store_info : value)
	{
		hash_combine(seed, load_store_info);
	}
}

template <>
inline void hash_param<std::vector<SubpassInfo>>(
    size_t &                        seed,
    const std::vector<SubpassInfo> &value)
{
	for (auto &subpass_info : value)
	{
		hash_combine(seed, subpass_info);
	}
}

template <>
inline void hash_param<std::vector<ShaderModule *>>(
    size_t &                           seed,
    const std::vector<ShaderModule *> &value)
{
	for (auto &shader_module : value)
	{
		hash_combine(seed, shader_module->get_id());
	}
}

template <>
inline void hash_param<std::vector<ShaderResource>>(
    size_t &                           seed,
    const std::vector<ShaderResource> &value)
{
	for (auto &resource : value)
	{
		hash_combine(seed, resource);
	}
}

template <>
inline void hash_param<std::map<uint32_t, std::map<uint32_t, VkDescriptorBufferInfo>>>(
    size_t &                                                              seed,
    const std::map<uint32_t, std::map<uint32_t, VkDescriptorBufferInfo>> &value)
{
	for (auto &binding_set : value)
	{
		hash_combine(seed, binding_set.first);

		for (auto &binding_element : binding_set.second)
		{
			hash_combine(seed, binding_element.first);
			hash_combine(seed, binding_element.second);
		}
	}
}

template <>
inline void hash_param<std::map<uint32_t, std::map<uint32_t, VkDescriptorImageInfo>>>(
    size_t &                                                             seed,
    const std::map<uint32_t, std::map<uint32_t, VkDescriptorImageInfo>> &value)
{
	for (auto &binding_set : value)
	{
		hash_combine(seed, binding_set.first);

		for (auto &binding_element : binding_set.second)
		{
			hash_combine(seed, binding_element.first);
			hash_combine(seed, binding_element.second);
		}
	}
}

template <typename T, typename... Args>
inline void hash_param(size_t &seed, const T &first_arg, const Args &... args)
{
	hash_param(seed, first_arg);

	hash_param(seed, args...);
}

template <class T, class... A>
struct RecordHelper
{
	size_t record(ResourceRecord & /*recorder*/, A &... /*args*/)
	{
		return 0;
	}

	void index(ResourceRecord & /*recorder*/, size_t /*index*/, T & /*resource*/)
	{
	}
};

template <class... A>
struct RecordHelper<ShaderModule, A...>
{
	size_t record(ResourceRecord &recorder, A &... args)
	{
		return recorder.register_shader_module(args...);
	}

	void index(ResourceRecord &recorder, size_t index, ShaderModule &shader_module)
	{
		recorder.set_shader_module(index, shader_module);
	}
};

template <class... A>
struct RecordHelper<PipelineLayout, A...>
{
	size_t record(ResourceRecord &recorder, A &... args)
	{
		return recorder.register_pipeline_layout(args...);
	}

	void index(ResourceRecord &recorder, size_t index, PipelineLayout &pipeline_layout)
	{
		recorder.set_pipeline_layout(index, pipeline_layout);
	}
};

template <class... A>
struct RecordHelper<RenderPass, A...>
{
	size_t record(ResourceRecord &recorder, A &... args)
	{
		return recorder.register_render_pass(args...);
	}

	void index(ResourceRecord &recorder, size_t index, RenderPass &render_pass)
	{
		recorder.set_render_pass(index, render_pass);
	}
};

template <class... A>
struct RecordHelper<GraphicsPipeline, A...>
{
	size_t record(ResourceRecord &recorder, A &... args)
	{
		return recorder.register_graphics_pipeline(args...);
	}

	void index(ResourceRecord &recorder, size_t index, GraphicsPipeline &graphics_pipeline)
	{
		recorder.set_graphics_pipeline(index, graphics_pipeline);
	}
};
}        // namespace

template <class T, class... A>
T &request_resource(Device &device, ResourceRecord *recorder, std::unordered_map<std::size_t, T> &resources, A &... args)
{
	RecordHelper<T, A...> record_helper;

	std::size_t hash{0U};
	hash_param(hash, args...);

	auto res_it = resources.find(hash);

	if (res_it != resources.end())
	{
		return res_it->second;
	}

	// If we do not have it already, create and cache it
	const char *res_type = typeid(T).name();
	size_t      res_id   = resources.size();

	LOGD("Building #{} cache object ({})", res_id, res_type);

// Only error handle in release
#ifndef DEBUG
	try
	{
#endif
		T resource(device, args...);

		auto res_ins_it = resources.emplace(hash, std::move(resource));

		if (!res_ins_it.second)
		{
			throw std::runtime_error{std::string{"Insertion error for #"} + std::to_string(res_id) + "cache object (" + res_type + ")"};
		}

		res_it = res_ins_it.first;

		if (recorder)
		{
			size_t index = record_helper.record(*recorder, args...);
			record_helper.index(*recorder, index, res_it->second);
		}
#ifndef DEBUG
	}
	catch (const std::exception &e)
	{
		LOGE("Creation error for #{} cache object ({})", res_id, res_type);
		throw e;
	}
#endif

	return res_it->second;
}
}
