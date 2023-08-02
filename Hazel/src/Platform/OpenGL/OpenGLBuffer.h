#pragma once

#include "Hazel/Renderer/Buffer.h"
#include "PlatForm/OpenGL/OpenGLTexture.h"

namespace Hazel {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(uint32_t width, uint32_t height);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual Ref<Texture2D> GetDepthAttachmentTexture() const { return m_DepthAttachmentTexture; }
		//Ref<Texture2D> GetColorAttachmentTexture() const { return m_ColorAttachmentTexture; }
		//Ref<Texture2D> GetDepthStencilAttachmentTexture() const { return m_DepthStencilAttachmentTexture; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_DepthAttachment;
		//uint32_t m_ColorAttachment;
		//uint32_t m_DepthStencilAttachment;
		Ref<Texture2D> m_DepthAttachmentTexture;
		//Ref<Texture2D> m_ColorAttachmentTexture;
		//Ref<Texture2D> m_DepthStencilAttachmentTexture;
	};

}