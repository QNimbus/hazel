#pragma once

#include "hazel/renderer/FrameBuffer.h"

namespace Hazel {

	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; };

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }

	private:
		FrameBufferSpecification m_Specification;
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
	};

}