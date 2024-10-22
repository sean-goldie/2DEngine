/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#pragma once

#include <typeindex>
#include <map>
#include <vector>
#include "Event/Event.h"

// Possible TODO: Make this less dependent on dynamic types and templates. 
// Can we just keep function pointers and void* handlers in maps/vectors instead? 
// This feels very dependent on templated types and bug-prone. 
// Plus for some reason we're passing Event& around to all the callbacks and
// I'm not sure that is necessary.

// Abstract base class for EventCallback
class IEventCallback
{
public:
    IEventCallback() = default;
    virtual ~IEventCallback() = 0;

    void Execute(Event EventToExecute)
    {
        Call(EventToExecute);
    }

protected:
    virtual void Call(Event EventToCall) = 0;
};

// Template extension of IEventCallback
template <typename THandler, typename TEvent>
class EventCallback : public IEventCallback
{
private:
    typedef void (THandler::* CallbackFunction)(TEvent&);

public:
    /** 
     * CallbackFunction function pointer must take argument 0 of type TEvent& and then optional args
     * to be forwarded to that Event's constructor.
     */
    EventCallback(THandler* Handler = nullptr, CallbackFunction Callback = nullptr)
        : Handler(Handler), Callback(Callback) {}

    ~EventCallback() override = default;

private:
    CallbackFunction Callback;

    THandler* Handler;

protected:
    void Call(Event EventToCall) override
    {
        std::invoke(Callback, Handler, static_cast<TEvent&>(EventToCall));
    }
};

class EventBus
{
public:
    ~EventBus()
    {
        for (const auto& [typeID, handlerList] : HandlerMap)
        {
            for (IEventCallback* ptr : handlerList)
            {
                delete ptr;
            }
        }
    }

    template <typename THandler, typename TEvent>
    void RegisterHandler(const THandler* Handler, void (THandler::* CallbackFunction)(TEvent&))
    {
        const auto typeID = std::type_index(typeid(TEvent));
        const auto newCallback = new EventCallback<THandler, TEvent>(Handler, CallbackFunction);

        if (HandlerMap.count(typeID))
        {
            HandlerMap[typeID].push_back(newCallback);
        }
        else
        {
            HandlerMap[typeID] = std::list<IEventCallback*>{ newCallback };
        }
    }

    template <typename TEvent, typename ...TArgs>
    void EmitEvent(TArgs&& ...Args)
    {
        const auto typeID = std::type_index(typeid(TEvent));

        if (HandlerMap.count(typeID))
        {
            for (IEventCallback* callback : HandlerMap[typeID])
            {
                callback->Execute(TEvent(std::forward<TArgs>(Args)...));
            }
        }
    }

private:
    std::map<std::type_index, std::list<IEventCallback*>> HandlerMap;
};