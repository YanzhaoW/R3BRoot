#pragma once

#include <Fit/Fitter.h>
#include <R3BNeulandCalData2.h>
#include <TF1.h>

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
        struct FitData
        {
            std::vector<double> z_vals;
            std::vector<double> z_errs;
            std::vector<double> vals;
            std::vector<double> errs;

            void clear()
            {
                z_vals.clear();
                z_errs.clear();
                vals.clear();
                errs.clear();
            }
            [[nodiscard]] auto size() const { return z_vals.size(); }
        };

        explicit MilleDataProcessor(int num_of_modules);
        auto operator()(const std::vector<BarCalData>& signals) -> const auto&;
        [[nodiscard]] auto get_data() const -> const auto& { return data_regsiters_; }
        [[nodiscard]] auto get_fit() const -> const auto& { return fit_result_; }
        [[nodiscard]] auto calculate_residual(double z_val, double val, int module_num) const -> double;
        void reset();

      private:
        std::unordered_map<int, std::vector<MilleCalData>> data_regsiters_;
        FitResult fit_result_;
        FitData x_z_vals;
        FitData y_z_vals;

        ROOT::Fit::Fitter fitter;
        TF1 fit_function{ "mille_fitting", "[0] * x + [1]" };

        void init_data_registers(int num_of_modules);
        void remove_isolated_bar_signal();
        void fit_planes();
        void fill_fit_data();
        void fit_plane_data();
    };
} // namespace R3B::Neuland::Calibration
