#pragma once
#include "hzpch.h"
#include "Hazel/Core.h"

namespace Hazel {

// Events in Hazel are currently blocking, meaning when an event occurs it
// immediately gets dispatched and must be dealt with right then an there.
// For the future, a better strategy might be to buffer events in an event
// bus and process them during the "event" part of the update stage.

enum class EventType
{
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
    None = 0,
    EventCategoryApplication    = BIT(0),
    EventCategoryInput          = BIT(1),
    EventCategoryKeyboard       = BIT(2),
    EventCategoryMouse          = BIT(3),
    EventCategoryMouseButton    = BIT(4)
};

// 根据给定的事件类型，生成对应的函数，包括获取事件类型、获取事件名称、获取事件分类
// static 修饰的函数，可以通过类名直接调用，不需要实例化对象
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

// 根据给定的事件分类，生成对应的函数，包括获取事件分类
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class HAZEL_API Event
{
    //friend class EventDispatcher; // 事件分发器
public:

    bool Handled = false;

    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    inline bool IsInCategory(EventCategory category)
    {
        return GetCategoryFlags() & category;
    }

};

class EventDispatcher
{
    template<typename T>
    using EventFn = std::function<bool(T&)>;
public:
    // 使用事件构造事件分发器
    EventDispatcher(Event& event)
        : m_Event(event)
    {
    }

    template<typename T>
    bool Dispatch(EventFn<T> func)
    {
        // 判断事件类型是否为 T 类型，如果是，则调用 func 函数
        if (m_Event.GetEventType() == T::GetStaticType())
        {
            m_Event.Handled = func(*(T*)&m_Event);
            return true;
        }
        return false;
    }
private:
    Event& m_Event;
};

// 重载输出流操作符，用于spdlog日志系统
inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    return os << e.ToString();
}

} // end of Hazel::
