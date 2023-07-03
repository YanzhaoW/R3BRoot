#pragma once
#include <boost/format.hpp>
#include <chrono>
#include <future>
#include <iostream>
#include <map>
#include <mutex>
#include <set>
#include <string_view>
#include <thread>
#include <unistd.h>
#include <vector>

namespace R3B::yml
{

    using EntryKey = std::vector<std::string_view>;
    using Pack = std::map<EntryKey, size_t>;
    class MultiIndexManager
    {
      public:
        using Packs = std::vector<Pack>;

        static void PrintPack(const Pack& pack);
        void Create_packs();
        [[nodiscard]] auto Get_packs() const -> const auto& { return indexPacks_; }
        void AddEntry(std::vector<std::string_view> label_chain, size_t size);
        auto PopPack() -> Pack;
        [[nodiscard]] auto GetSuffix(const Pack& pack) const -> std::string;
        void Set_suffix_len(size_t len) { suffix_len_ = len; }

      private:
        MultiIndexManager() = default;
        size_t suffix_len_ = 0;
        Packs indexPacks_;
        Pack entry_max_sizes_;
        template <typename Iter>
        static void AppendNewEntryFromOld(const Packs& old_packs, const Pack& entry, Iter last_pack);
        friend auto GetGlobalIndexManager() -> MultiIndexManager*;
    };

    extern std::set<int> child_PIDs; // NOLINT
    extern std::mutex PRINT_MUTEX;   // NOLINT

    void SetGlobalIndices(const Pack& pack);
    MultiIndexManager* GetGlobalIndexManager();
    Pack* GetGlobalIndices();

    struct MultiProc_options
    {
        int maximal_print;
        size_t child_exit_waiting_time;   // milli seconds
        size_t listening_waiting_time;    // milli seconds
        size_t next_process_waiting_time; // milli seconds
    };

    extern MultiProc_options const DEFAULT_OPTION;

    class ChildProc
    {
      public:
        explicit ChildProc(int pid)
            : PID_{ pid }
        {
        }

        void SetPipe(const std::array<int, 2>& pipe_fd) { pipe_fd_ = pipe_fd; }
        void Close_read() { close(pipe_fd_[0]); }
        void Close_write() { close(pipe_fd_[1]); }

        void Setup();
        void Finish();

        [[nodiscard]] auto GetPID() const -> int { return PID_; }
        [[nodiscard]] auto GetReadFD() const -> int { return pipe_fd_[0]; }
        [[nodiscard]] auto GetWriteFD() const -> int { return pipe_fd_[1]; }

      private:
        std::array<int, 2> pipe_fd_{};
        const int PID_{};
    };

    class MultiProc
    {
      public:
        explicit MultiProc(MultiProc_options option = DEFAULT_OPTION);
        void SetMaxNTread(size_t numP) { nProc = numP; }

        template <typename Proc>
        void AddProcess(Proc&& process)
        {
            auto const lock = std::scoped_lock{ processes_mutex_ };
            processes_.emplace_back([this, process]() { this->RunProcess(process); });
        }

        void Run();
        [[nodiscard]] auto GetMotherPID() const -> int { return mother_PID_; }

        // rule of 5:
        ~MultiProc() = default;
        MultiProc(const MultiProc&) = delete;
        MultiProc(MultiProc&&) = delete;
        auto operator=(const MultiProc&) -> MultiProc& = delete;
        auto operator=(MultiProc&&) -> MultiProc& = delete;

      private:
        void Listen(const ChildProc& child) const;
        void Listen_all() const;
        void Wait();
        void SetFinished() { is_call_finished = true; }

        template <typename Proc>
        void SpawnChild(Proc&& process)
        {
            auto pipe_fd = std::array<int, 2>{};

            if (pipe(pipe_fd.data()) == -1)
            {
                throw std::runtime_error("pipe open failed");
            }

            auto pid = fork();
            if (pid < 0)
            {
                throw std::runtime_error("Failed to create a subprocess!");
            }

            auto child = ChildProc{ pid };
            child.SetPipe(pipe_fd);

            if (pid > 0)
            {
                child.Close_write();
                auto lock = std::scoped_lock{ child_vector_mutex_ };
                child_PIDs.insert(pid);
                children_.push_back(child);
            }

            if (pid == 0)
            {
                // write to pipe
                child.Setup();
                process();
                child.Finish();
            }
        }
        void RunProcesses();
        [[nodiscard]] auto CheckPipesEOF() const -> bool;
        [[nodiscard]] auto GetChildren() const -> const std::vector<ChildProc>& { return children_; }
        template <typename Proc>
        void RunProcess(Proc&& process)
        {
            SpawnChild(std::forward<Proc>(process));
        }

        std::atomic<bool> is_call_finished = false;
        size_t nProc = 4;
        std::atomic<uint8_t> current_NP_ = 0;
        int const mother_PID_;
        std::vector<ChildProc> children_;
        std::vector<std::function<void()>> processes_;
        mutable std::mutex processes_mutex_;
        mutable std::mutex child_vector_mutex_;
        mutable std::mutex pid_mutex_;
        const MultiProc_options option_;
    };
} // namespace R3B::yml
