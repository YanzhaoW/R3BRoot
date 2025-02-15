#pragma once

#include <R3BNeulandCalData2.h>

namespace R3B::Neuland::Calibration
{
    struct MilleCalData
    {
      public:
        MilleCalData() = default;
        explicit MilleCalData(const BarCalData& bar_cal_data)
            : module_num{ bar_cal_data.module_num }
            , left{ bar_cal_data.left.front() }
            , right{ bar_cal_data.right.front() }
        {
        }
        unsigned int module_num = 0; // 1 based bar num
        CalDataSignal left;
        CalDataSignal right;
    };

    class MilleDataProcessor
    {
      public:
        struct FitPar
        {
            double offset = 0.;
            double slope = 0.;
        };
        struct FitResult
        {
            FitPar x_z;
            FitPar y_z;
        };

        explicit MilleDataProcessor(int num_of_modules);
        auto operator()(const std::vector<BarCalData>& signals) -> const auto&;
        [[nodiscard]] auto get_data() const -> const auto& { return data_regsiters_; }
        [[nodiscard]] auto get_fit() const -> const auto& { return fit_result_; }
        void reset();

      private:
        std::unordered_map<int, std::vector<MilleCalData>> data_regsiters_;
        FitResult fit_result_;

        void init_data_registers(int num_of_modules);
        void remove_isolated_bar_signal();
        void fit_planes();
    };
} // namespace R3B::Neuland::Calibration
