#pragma once
#include "pxr_base/task_queue/task_queue.h"
#include <mutex>
namespace pxr_base
{
    template<class T>
    class TaskQueueCaller
    {
    private:
        typedef std::shared_ptr<T> Ptr_t;
    public:
        template<class Func>
        inline bool CallInTaskQueue(Func f, size_t timeout_ms = 0)
        {
            Ptr_t ptr = static_cast<T*>(this)->shared_from_this();
            if (task_queue_->IsQueueThread())
            {
                return f(ptr);
            }

            bool res = false;
            if (timeout_ms)
            {
                std::mutex mtx;
                std::unique_lock<std::mutex> lck(mtx);
                std::condition_variable cond;
                task_queue_->PostTask(
                    [f, &res, &cond, ptr]()
                    {
                        res = f(ptr);
                        cond.notify_all();
                    });
                auto tmout = std::chrono::milliseconds(timeout_ms);
                if (cond.wait_for(lck, tmout) != std::cv_status::no_timeout)
                {
                    return false;
                }
            }
            else
            {
                task_queue_->PostTask(
                    [f, ptr]()
                    {
                        f(ptr);
                    });
                res = true;
            }
            return res;
        }
    protected:
        TaskQueueCaller(std::shared_ptr<TaskQueue> task_queue)
            :task_queue_(task_queue) {}
        std::shared_ptr<TaskQueue> task_queue_;
    };
}