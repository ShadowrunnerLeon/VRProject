#pragma once
#include <memory>
#include "pxr_base/task_queue/task.h"
#include "pxr_base/pxr_exports.h"
namespace pxr_base
{
    class Task;
    class PXR_API TaskQueue
    {
    public:
        static std::shared_ptr<TaskQueue> Create(size_t thread_num);
        virtual void PostTask(std::unique_ptr<Task> task, bool in_order = false) = 0;

        template <typename Closure>
        inline void PostTask(Closure&& closure)
        {
            PostTask(ToQueuedTask(closure));
        }
        virtual bool IsQueueThread() = 0;
        virtual size_t TaskInQueue() const = 0;
        virtual void PostDelayedTask(std::unique_ptr<Task> task, uint32_t delay_mills) = 0;
        template <typename Closure>
        inline void PostDelayedTask(Closure&& closure, uint32_t delay_mills)
        {
            PostDelayedTask(ToQueuedTask(closure), delay_mills);
        }
        virtual ~TaskQueue() {}
    };
}