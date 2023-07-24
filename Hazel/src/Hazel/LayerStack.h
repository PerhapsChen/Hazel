#pragma once

#include "Hazel/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel {

	class HAZEL_API LayerStack
	{
	public:
		LayerStack(); // ��ʼ��m_LayerInsertΪm_Layers.begin(),��Ϊ�ֽ���, ��ʱm_LayersΪ��
		~LayerStack(); // ѭ�� delete layers.

		//? ��һ��stack����Ϊ2�����֣����泣��layers�� �Ҳ��overlay layers.

		void PushLayer(Layer* layer);		// �ڷֽ���������layer
		void PushOverlay(Layer* layer);     // �ڷֽ����Ҳ����overlay layers
		void PopLayer(Layer* layer);		// �ҵ�layerɾ������Ϊ�ڷֽ�����ߣ�erase���·ֽ������ƣ����Լ�һ����
		void PopOverlay(Layer* layer);		// �ҵ�overlay layerɾ������Ϊ���ұߣ���Ӱ��ֽ���

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};

}



