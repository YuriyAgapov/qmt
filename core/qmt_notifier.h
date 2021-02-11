#pragma once

#include <functional>
#include <memory>
#include <vector>

namespace Qmt
{
    template <typename ListenerType>
    class Notifier
    {
    public:
        using WeakListenerTypeSP = std::weak_ptr<ListenerType>;

        template <typename TargetListenerType>
        void for_each(std::function<void (TargetListenerType&)> func)
        {
            for (auto iter = _listeners.begin(); iter != _listeners.end(); )
            {
                auto listener = iter->lock();
                if (listener)
                {
                    auto targetListenerType = std::dynamic_pointer_cast<TargetListenerType>(listener);
                    if (targetListenerType)
                        func(*targetListenerType);
                    ++iter;
                }
                else
                {
                    iter = _listeners.erase(iter);
                }
            }
        }

        void addListener(WeakListenerTypeSP listener)
        {
            _listeners.push_back(listener);
        }

        void removeListener(WeakListenerTypeSP listener)
        {
            _listeners.erase(std::remove_if(_listeners.begin(),
                                            _listeners.end(), [listener](WeakListenerTypeSP l)
            {
                return listener.lock() == l.lock();
            }));
        }

    private:
        std::vector<WeakListenerTypeSP> _listeners;
    };
}//namespace Qmt
