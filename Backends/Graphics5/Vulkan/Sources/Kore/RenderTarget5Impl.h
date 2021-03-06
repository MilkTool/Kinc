#pragma once

#include <vulkan/vulkan.h>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifdef RegisterClass
#undef RegisterClass
#endif

typedef struct {
	VkImage destImage;
	VkDeviceMemory destMemory;
	VkImageView destView;

	VkImage sourceImage;
	VkDeviceMemory sourceMemory;
	VkImageView sourceView;

	VkImage depthImage;
	VkDeviceMemory depthMemory;
	VkImageView depthView;
	int depthBufferBits;

	VkFramebuffer framebuffer;
	VkSampler sampler;
	VkRenderPass renderPass;

	VkFormat format;

	int stage;
	int stage_depth;
} RenderTarget5Impl;
