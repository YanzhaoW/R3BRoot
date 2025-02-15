#include "R3BNeulandMilleCalDataProcessor.h"
#include <R3BNeulandCommon.h>
#include <algorithm>
#include <range/v3/algorithm.hpp>

namespace R3B::Neuland::Calibration
{
    MilleDataProcessor::MilleDataProcessor(int num_of_modules) { init_data_registers(num_of_modules); }

    void MilleDataProcessor::init_data_registers(int num_of_modules)
    {
        const auto num_of_planes = num_of_modules / BarsPerPlane;
        for (int plane_id{}; plane_id < num_of_planes; ++plane_id)
        {
            auto data_iter = data_regsiters_.insert_or_assign(plane_id, std::vector<MilleCalData>{}).first;
            data_iter->second.reserve(BarsPerPlane);
        }
    }

    void MilleDataProcessor::reset()
    {
        for (auto& [plane_id, bar_data] : data_regsiters_)
        {
            bar_data.clear();
        }
        fit_result_.x_z = FitPar{};
        fit_result_.y_z = FitPar{};
    }

    auto MilleDataProcessor::operator()(const std::vector<BarCalData>& signals) -> const auto&
    {
        // fill only the bar_cal_data with only one pmt signal on both sides
        for (const auto& signal : signals)
        {
            if (signal.left.size() == 1 && signal.right.size() == 1)
            {
                const auto bar_num = static_cast<int>(signal.module_num);
                const auto plane_id = ModuleID2PlaneID(bar_num - 1);
                data_regsiters_[plane_id].emplace_back(signal);
            }
        }

        remove_isolated_bar_signal();

        return *this;
    }

    void MilleDataProcessor::remove_isolated_bar_signal()
    {
        namespace rng = ranges;
        for (auto& [plane_id, bar_data] : data_regsiters_)
        {
            if (bar_data.size() < 2)
            {
                continue;
            }

            const auto& bar_signals = bar_data;
            auto check_if_isolated = [&bar_signals](const auto& signal) -> bool
            {
                const auto module_num = static_cast<int>(signal.module_num);
                return (rng::find_if(bar_signals,
                                     [module_num](const auto& bar_signal)
                                     { return bar_signal.module_num == module_num - 1; }) == bar_signals.end()) and
                       (rng::find_if(bar_signals,
                                     [module_num](const auto& bar_signal)
                                     { return bar_signal.module_num == module_num + 1; }) == bar_signals.end());
            };

            bar_data.erase(std::remove_if(bar_data.begin(), bar_data.end(), check_if_isolated), bar_data.end());
        }
    }

    void MilleDataProcessor::fit_planes() {}
} // namespace R3B::Neuland::Calibration
