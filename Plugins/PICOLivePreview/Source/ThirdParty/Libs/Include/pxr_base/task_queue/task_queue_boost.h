#pragma once
#include <memory>
#include <thread>
#include <list>

#include "pxr_base/task_queue/task_queue.h"
#include "asio2/asio/asio.hpp"

namespace pxr_base
{
    using namespace asio;
    
    class TaskQueueBoost final:public TaskQueue
    {
    public:
        TaskQueueBoost(size_t thread_num);
        ~TaskQueueBoost();

        typedef asio::io_context ContextType;
        virtual void PostTask(std::unique_ptr<Task> task, bool in_order = false) override;
        virtual void PostDelayedTask(std::unique_ptr<Task> task, uint32_t delay_mills) override;
        virtual bool IsQueueThread() override;
        virtual size_t TaskInQueue() const override;
        ContextType& GetContext();
    private:
        void ThreadProcess();
    private:
        io_context context_;
        executor_work_guard<io_context::executor_type> work_;
        io_context::strand strand_;
        std::list<std::unique_ptr<std::thread>> threads_;
        std::atomic_size_t task_in_queue_;
    };
}