#pragma once

#include "Hurikan/Renderer/Framebuffer.h"

namespace Hurikan {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		void Invalidate();
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual const uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; };

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureFormat> m_ColorAttachmentFormat;
		FramebufferTextureFormat m_DepthAttachmentFornat = FramebufferTextureFormat::None;
	};
}
