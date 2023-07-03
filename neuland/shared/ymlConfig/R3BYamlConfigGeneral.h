#pragma once

#include "R3BYamlConfig.h"
#include <string>

namespace R3B::yml
{

    struct OutputFile
    {
        DNode<std::string> para = { "para" };
        DNode<std::string> ana = { "ana" };
        DNode<std::string> simu = { "simu" };
        DNode<std::string> online = { "simu" };
    };
    struct InputFile
    {
        DNode<std::string> para = { "para" };
        DNode<std::string> ana = { "ana" };
        DNode<std::string> online = { "simu" };
    };

    struct Simulation
    {
        DNode<std::string> material_file = { "material-file" };
        DNode<std::string> simu_engine = { "simu-engine" };
        DNode<bool> store_trajectory = { "store-trajectory" };
        struct Event
        {
            DNode<int> number = { "number" };
            DNode<int> print = { "print" };
        };
        DNode<Event> event = { "event" };

        struct Particle_gun
        {
            DNode<std::string> particle_name = { "particle-name" };
            DNode<MPValue<int>> multiplicity = { "multiplicity" };
            DNode<std::vector<double>> position = { "position" };
            DNode<std::vector<double>> theta = { "theta" };
            DNode<std::vector<double>> phi = { "phi" };
            DNode<std::vector<double>> energy = { "energy" };
        };
        DNode<Particle_gun> particle_gun = { "particle-gun" };
    };

    struct Analysis
    {
        DNode<std::vector<int>> event = { "event" };
    };

    struct Multi_Process
    {
        DNode<bool> status = { "status" };
        DNode<int> max = { "max" };
        DNode<int> suffix_len = { "suffix-len" };
    };

    struct General
    {
        DNode<int> threads = { "threads" };
        DNode<Multi_Process> multi_process = { "multi-process" };
        DNode<std::string> source_directory = { "source-directory" };
        DNode<std::string> log_level = { "log-level" };
        DNode<InputFile> inputfile = { "inputfile" };
        DNode<OutputFile> outputfile = { "outputfile" };
        DNode<Simulation> simulation = { "simulation" };
        DNode<Analysis> analysis = { "analysis" };
    };
} // namespace R3B::yml
