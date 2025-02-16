#include "R3BNeulandMilleCalDataProcessor.h"
#include <Fit/BinData.h>
#include <Math/WrappedMultiTF1.h>
#include <R3BNeulandCommon.h>
#include <algorithm>
#include <range/v3/algorithm.hpp>

namespace R3B::Neuland::Calibration
{
    MilleDataProcessor::MilleDataProcessor(int num_of_modules)
    {
        init_data_registers(num_of_modules);
        fitter.SetFunction(
            ROOT::Math::WrappedMultiTF1{ fit_function, static_cast<unsigned int>(fit_function.GetNdim()) }, false);
    }

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
        x_z_vals.clear();
        y_z_vals.clear();
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
        fit_planes();

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

    void MilleDataProcessor::fit_planes()
    {
        fill_fit_data();

        fit_plane_data();
    }

    void MilleDataProcessor::fill_fit_data()
    {
        for (auto& [plane_id, bar_data] : data_regsiters_)
        {
            const auto is_plane_horizontal = IsPlaneIDHorizontal(plane_id);
            auto& fit_data = is_plane_horizontal ? y_z_vals : x_z_vals;
            const auto z_val = PlaneID2ZPos(plane_id);

            const auto displacement =
                std::accumulate(bar_data.begin(),
                                bar_data.end(),
                                0.,
                                [](double sum, const MilleCalData& signal)
                                { return sum + GetBarVerticalDisplacement(static_cast<int>(signal.module_num)); }) /
                static_cast<double>(bar_data.size());
            fit_data.z_vals.push_back(z_val);
            fit_data.z_errs.push_back(BarSize_Z / 2.);
            fit_data.errs.push_back(0.);
            fit_data.vals.push_back(displacement);
        }
    }

    void MilleDataProcessor::fit_plane_data()
    {
        const auto x_z_data = ROOT::Fit::BinData{ static_cast<unsigned int>(x_z_vals.size()),
                                                  x_z_vals.z_vals.data(),
                                                  x_z_vals.vals.data(),
                                                  x_z_vals.z_errs.data(),
                                                  x_z_vals.errs.data() };
        const auto y_z_data = ROOT::Fit::BinData{ static_cast<unsigned int>(y_z_vals.size()),
                                                  y_z_vals.z_vals.data(),
                                                  y_z_vals.vals.data(),
                                                  y_z_vals.z_errs.data(),
                                                  y_z_vals.errs.data() };
        fitter.Fit(x_z_data);
        fit_result_.x_z.slope = fitter.Result().Parameter(0);
        fit_result_.x_z.offset = fitter.Result().Parameter(1);

        fitter.Fit(y_z_data);
        fit_result_.y_z.slope = fitter.Result().Parameter(0);
        fit_result_.y_z.offset = fitter.Result().Parameter(1);
    }

    auto MilleDataProcessor::calculate_residual(double z_val, double val, int module_num) const -> double
    {
        const auto is_plane_horizontal = IsPlaneIDHorizontal(ModuleID2PlaneID(module_num - 1));
        const auto& fit_result = is_plane_horizontal ? fit_result_.x_z : fit_result_.y_z;
        const auto diff = val - (fit_result.slope * z_val) - fit_result.offset;
        return diff * diff;
    }
} // namespace R3B::Neuland::Calibration
