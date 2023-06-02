#include "R3BNeulandSimulator.h"
#include <FairBoxGenerator.h>
#include <FairConstField.h>
#include <FairParRootFileIo.h>
#include <FairPrimaryGenerator.h>
#include <FairRootFileSink.h>
#include <FairRunSim.h>
#include <FairRuntimeDb.h>
#include <G4RunManager.hh>
#include <G4UserEventAction.hh>
#include <TG4EventAction.h>

namespace R3B::Neuland
{

    std::unordered_map<std::string, int> const PDGID_map = { { "neutron", 2112 },
                                                             { "proton", 2212 },
                                                             { "electron", 11 },
                                                             { "gamma", 22 },
                                                             { "alpha", 1000020040 } };

    Simulator::Simulator()
        : run_{ std::make_unique<FairRunSim>() }
    {
    }

    void Simulator::Run()
    {
        const auto& general = GetGeneralConfig();
        InitRun();
        run_->Run(general().simulation().event().number());
    }

    void Simulator::InitRun()
    {
        SetupRun(run_.get());

        auto primGen = std::make_unique<FairPrimaryGenerator>();
        SetupGenerator(primGen.get());
        run_->SetGenerator(primGen.release());
        SetupModules(run_.get());
        run_->Init();
        SetupEventPrint();
        SetupParaOutput();
    }

    void Simulator::SetupParaOutput()
    {
        const auto& general = GetGeneralConfig();
        const auto& config = general().outputfile();
        auto parFileIO = std::make_unique<FairParRootFileIo>(true);
        parFileIO->open(config.para().c_str());
        auto* rtdb = run_->GetRuntimeDb();
        rtdb->setOutput(parFileIO.release());
        rtdb->saveOutput();
    }

    void Simulator::SetupModules(FairRunSim* run)
    {

        for (auto& module : detectors_)
        {
            if (module == nullptr)
            {
                throw std::runtime_error("empty detector being added!");
            }
            LOG(info) << "Adding detector " << module->GetName() << "\n";
            run->AddModule(module.release());
        }
    }
    void Simulator::SetupEventPrint()
    {
        const auto& general = GetGeneralConfig();
        const auto& config = general().simulation().event();
        auto* grun = G4RunManager::GetRunManager();
        grun->SetPrintProgress(config.print());
        auto* event =
            dynamic_cast<TG4EventAction*>(const_cast<G4UserEventAction*>(grun->GetUserEventAction())); // NOLINT
        event->VerboseLevel(0);
    }

    void Simulator::SetupRun(FairRunSim* run)
    {
        const auto& general = GetGeneralConfig();
        const auto& ymlConfig = general();
        const auto& simuConfig = ymlConfig.simulation();

        run->SetName(simuConfig.simu_engine().c_str());
        run->SetStoreTraj(simuConfig.store_trajectory());
        run->SetMaterials(simuConfig.material_file().c_str());

        run->SetSink(std::make_unique<FairRootFileSink>(ymlConfig.inputfile().simu().c_str()));
        auto fairField = std::make_unique<FairConstField>();
        run->SetField(fairField.release());
    }


    void Simulator::SetupGenerator(FairPrimaryGenerator* primGen)
    {
        const auto& general = GetGeneralConfig();
        const auto& particleConfig = general().simulation().particle_gun();
        const auto& pName = particleConfig.particle_name();
        auto boxGen = std::make_unique<FairBoxGenerator>(Get_PDG_ID(pName), particleConfig.multiplicity());

        const auto& pos = particleConfig.position();
        boxGen->SetXYZ(pos[0], pos[1], pos[2]);
        boxGen->SetThetaRange(particleConfig.theta()[0], particleConfig.theta()[1]);
        boxGen->SetPhiRange(particleConfig.phi()[0], particleConfig.phi()[1]);

        const auto& energy = particleConfig.energy();
        boxGen->SetEkinRange(energy[0], energy[1]);
        primGen->AddGenerator(boxGen.release());
    }

    auto Simulator::Get_PDG_ID(const std::string& name) -> int
    {
        auto pid = PDGID_map.find(name);
        if (pid == PDGID_map.end())
        {
            auto msg = std::string{ "unrecognized particle name: " } + name;
            throw std::runtime_error(msg);
        }

        return pid->second;
    }

} // namespace R3B::Neuland
