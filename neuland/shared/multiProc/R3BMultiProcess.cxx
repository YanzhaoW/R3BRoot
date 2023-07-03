#include "R3BMultiProcess.h"
#include <R3BNeulandCommon.h>
#include <bitset>
#include <poll.h>
#include <sys/wait.h>

namespace R3B::yml
{
    using Neuland::StringsConcat;
    constexpr int POLL_TIMEOUT = 1000;

    MultiIndexManager* GetGlobalIndexManager()
    {
        static MultiIndexManager indexMan = MultiIndexManager{};
        return &indexMan;
    }
    Pack* GetGlobalIndices()
    {
        static Pack indices_pack = Pack{};
        return &indices_pack;
    }

    void SetGlobalIndices(const Pack& pack)
    {
        auto* indices = GetGlobalIndices();
        *indices = pack;
    }

    // The reason behind this two non-const global variables is because sigaction cannot take a member function. There
    // could be some better ways without using non-const global variables.
    std::set<int> child_PIDs; // NOLINT
    std::mutex PRINT_MUTEX{}; // NOLINT

    MultiProc_options const DEFAULT_OPTION = { .maximal_print = 50,
                                               .child_exit_waiting_time = 50,
                                               .listening_waiting_time = 20,
                                               .next_process_waiting_time = 1000 };
    template <typename Iter>
    void MultiIndexManager::AppendNewEntryFromOld(const Packs& old_packs, const Pack& entry, Iter last_pack)
    {
        if (old_packs.empty())
        {
            *last_pack++ = entry;
        }
        else
        {
            for (auto pack : old_packs)
            {
                auto entry_tmp = entry;
                // merge destroys the input parameter
                pack.merge(entry_tmp);
                *last_pack++ = std::move(pack);
            }
        }
    }

    auto MultiIndexManager::GetSuffix(const Pack& pack) const -> std::string
    {
        auto output = std::string{};
        using namespace std::string_literals;
        for (const auto& [name, key] : pack)
        {
            auto name_temp = (suffix_len_ > 0) ? name.back().substr(0, suffix_len_) : name.back();
            output += ("_"s + std::string{ name_temp } + "_"s + std::to_string(key));
        }
        return output;
    }

    void MultiIndexManager::Create_packs()
    {
        indexPacks_ = std::vector<Pack>{};
        for (const auto& [label_chain, max_size] : entry_max_sizes_)
        {
            auto new_packs = std::vector<Pack>{};
            for (size_t index{ 0 }; index < max_size; ++index)
            {
                AppendNewEntryFromOld(indexPacks_, { { label_chain, index } }, std::back_inserter(new_packs));
            }
            indexPacks_ = std::move(new_packs);
        }
    }

    void MultiIndexManager::AddEntry(std::vector<std::string_view> label_chain, size_t size)
    {
        if (size != 0)
        {
            entry_max_sizes_.insert_or_assign(std::move(label_chain), size);
        }
    }

    auto MultiIndexManager::PopPack() -> Pack
    {
        auto indexPack = std::move(indexPacks_.back());
        indexPacks_.pop_back();
        return indexPack;
    }

    void MultiIndexManager::PrintPack(const Pack& pack)
    {
        std::cout << "****************"
                  << "\n";
        for (const auto& [key, index] : pack)
        {
            std::cout << StringsConcat(key) << "\t" << index << "\n";
        }
    }
    using boost::format;
    namespace
    {
        // constexpr int MAXIMAL_PRINT = 2;
        // constexpr size_t WAITING_TIME = 50;
        // constexpr size_t THREAD_WAITING_TIME = 10;
        // constexpr size_t LISTEN_WAIT_TIME = 100;

        // constexpr int maximal_print_ = 2;
        // constexpr size_t child_exit_waiting_time_ = 50;
        // constexpr size_t listening_waiting_time_ = 100;

        template <typename String>
        void PrintMSG(const String& msg)
        {
            auto lock = std::scoped_lock{ PRINT_MUTEX };
            std::cout << msg << "\n";
        }

        void handle_sigINT(int /*sig*/)
        {
            for (auto const it : child_PIDs)
            {
                if (it > 0)
                {
                    PrintMSG(format("[Process] Try to kill child process %1%.") % it);
                    ::kill(it, SIGINT);
                }
            }
            using namespace std::string_literals;
            auto const msg = "Mother process "s + std::to_string(getpid()) + " stopped because of SIGINIT!"s;
            throw std::runtime_error(msg);
        }
        void handle_sigINT_child(int /*sig*/)
        {
            using namespace std::string_literals;
            auto const msg = "Child Process "s + std::to_string(getpid()) + " stopped because of SIGINIT!"s;
            throw std::runtime_error(msg);
        }

        // read a line from a file descriptor
        auto Read_line(int file_desc) -> std::string
        {
            auto str = std::string{};
            auto buffer = char{};
            do
            {
                auto res = read(file_desc, &buffer, 1);
                if (res == -1)
                {
                    throw std::runtime_error("Error occured while reading a line from the pipe!");
                }

                if (res == 0)
                {
                    return {};
                }
                str.push_back(buffer);

            } while (buffer != '\n');
            str.pop_back();
            return str;
        }
    } // namespace

    void ChildProc::Setup()
    {
        // skip if in mother process
        Close_read();
        dup2(GetWriteFD(), STDOUT_FILENO);
        dup2(GetWriteFD(), STDERR_FILENO);

        struct sigaction sig_action
        {
        };
        sig_action.sa_handler = &handle_sigINT_child; // NOLINT
        sigaction(SIGINT, &sig_action, nullptr);
        std::cout << "Child process starts..." << std::endl;
    }

    void ChildProc::Finish()
    {
        Close_write();
        // PrintMSG(format("Child finished: %1%") % getpid());
    }

    MultiProc::MultiProc(MultiProc_options option)
        : mother_PID_(getpid())
        , option_{ option }
    {
        struct sigaction sig_action
        {
        };
        sig_action.sa_handler = &handle_sigINT; // NOLINT
        sigaction(SIGINT, &sig_action, nullptr);
    }

    auto CheckPipeRead(int read_fd) -> bool
    {
        struct pollfd poll_fd
        {
            .fd = read_fd, .events = POLLIN
        };
        auto poll_res = poll(&poll_fd, 1, POLL_TIMEOUT);
        bool res = (poll_res == 1) && (((poll_fd.revents & POLLIN) != 0)); // NOLINT
        // PrintMSG(format("<0> revents res: %1%") % poll_fd.revents);
        // PrintMSG(format("<0> poll res: %1%") % poll_res);
        return res;
    }

    void MultiProc::Listen(const ChildProc& child) const
    {
        for (size_t count{}; count < option_.maximal_print; ++count)
        {
            if (CheckPipeRead(child.GetReadFD()))
            {
                const auto msg = boost::format("<%1%>: %2%") % child.GetPID() % Read_line(child.GetReadFD());
                PrintMSG(msg);
            }
        }
    }

    auto CheckPipeEOF(int read_fd) -> bool
    {
        struct pollfd poll_fd
        {
            .fd = read_fd, .events = POLLIN
        };
        auto poll_res = poll(&poll_fd, 1, 0);
        bool res = (((poll_fd.revents & POLLIN) != 0) || ((poll_fd.revents & POLLHUP) == 0)); // NOLINT
        if (poll_res == 0 || poll_fd.revents == 16)                                           // NOLINT
        {
            return false;
        }
        return res; // NOLINT
    }

    auto MultiProc::CheckPipesEOF() const -> bool
    {
        const auto& children = GetChildren();
        auto not_empty = false;
        for (const auto& child : children)
        {
            not_empty = (not_empty || CheckPipeEOF(child.GetReadFD()));
        }
        return not_empty;
    }

    void MultiProc::Listen_all() const
    {
        while (CheckPipesEOF() || !is_call_finished)
        {
            {
                auto lock = std::scoped_lock{ child_vector_mutex_ };
                const auto& children = GetChildren();
                for (const auto& child : children)
                {
                    Listen(child);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(option_.listening_waiting_time));
        }
    }

    void MultiProc::RunProcesses()
    {
        // The processes_ vector is always locked after calling this function
        auto const lock = std::scoped_lock{ processes_mutex_ };
        while (!processes_.empty())
        {

            const auto avaiable_np = nProc - current_NP_;
            for (size_t np{}; np < avaiable_np; ++np)
            {
                if (processes_.empty())
                {
                    break;
                }
                // Spawun child processes with fork().
                // ATTENTION: All the child processes after this function call should be directly led to the return of
                // main. The most easy way (also the wrong way) to do this is calling abort() function, which doesn't
                // perform any cleanup. This could cause unclosed files. Throwing an exception, on the other hand, has a
                // negative impact on the speed. The best way should be using multiple returns, leading all the ways to
                // the main.
                processes_.back()();

                // child process must goes directly to end
                if (getpid() != mother_PID_)
                {
                    return;
                }
                ++current_NP_;
                processes_.pop_back();
            }
            // sleep one second
            auto const waiting_time = std::chrono::milliseconds(option_.next_process_waiting_time);
            std::this_thread::sleep_for(waiting_time);
        }
        SetFinished();
    }

    void MultiProc::Run()
    {
        auto process_thread = std::thread([this]() { this->RunProcesses(); });
        // child process must goes directly to end
        if (getpid() != mother_PID_)
        {
            return;
        }

        auto listen_thread = std::thread([this]() { this->Listen_all(); });
        Wait();
        process_thread.join();
        listen_thread.join();
    }

    void MultiProc::Wait()
    {
        while (true)
        {
            auto wstatus = 0;
            auto child_id = waitpid(-1, &wstatus, WNOHANG);
            // PrintMSG(format("<0> child id:%1%") % child_id);
            // PrintMSG(format("<0> child exit:%1%") % wstatus);
            if (child_id < 0 && is_call_finished)
            {
                // PrintMSG(format("<0> child id:%1%") % child_id);
                break;
            }
            if (WIFEXITED(wstatus) && child_id > 0) // NOLINT
            {
                // PrintMSG(format("<0> finished child id: %1%") % child_id);
                if (current_NP_ != 0)
                {
                    --current_NP_;
                }
                if (WEXITSTATUS(wstatus) == 0) // NOLINT
                {
                    auto lock = std::scoped_lock{ child_vector_mutex_ };
                    child_PIDs.erase(child_id);
                    PrintMSG(format("<0> Child process on ID %1% has been executed successfully.") % child_id);
                }
                else
                {
                    PrintMSG(format("<0> Child process on ID %1% has failed.") % child_id);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(option_.child_exit_waiting_time));
        }
    }
} // namespace R3B::yml
