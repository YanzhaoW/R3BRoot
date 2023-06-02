#pragma once
#include "R3BYamlConfig.h"
#include <vector>

namespace R3B::yml::Neuland
{

    struct Simulation
    {
        DNode<bool> status = { "status" };
        DNode<std::vector<double>> position = { "position" };
        DNode<int> nDP = { "nDP" };
    };

    struct Digitizer
    {
        DNode<std::string> paddle = { "paddle" };
        DNode<std::string> channel = { "channel" };
        DNode<std::string> hit_par = { "hit-par" };
    };

    struct Analysis
    {
        DNode<Digitizer> digitizer = { "digitizer" };
    };

    struct YamlConfig
    {
        DNode<Simulation> simulation = { "simulation" };
        DNode<Analysis> analysis = { "analysis" };
    };

} // namespace R3B::yml::Neuland
