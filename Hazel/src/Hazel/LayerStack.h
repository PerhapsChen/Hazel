#pragma once

#include "Hazel/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel {

	class HAZEL_API LayerStack
	{
	public:
		LayerStack(); // 初始化m_LayerInsert为m_Layers.begin(),作为分界线, 此时m_Layers为空
		~LayerStack(); // 循环 delete layers.

		//? 将一个stack划分为2个部分，左侧存常规layers， 右侧存overlay layers.

		void PushLayer(Layer* layer);		// 在分界线左侧插入layer
		void PushOverlay(Layer* layer);     // 在分界线右侧插入overlay layers
		void PopLayer(Layer* layer);		// 找到layer删除，因为在分界线左边，erase导致分界线右移，所以减一抵消
		void PopOverlay(Layer* layer);		// 找到overlay layer删除，因为在右边，不影响分界线

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};

}



