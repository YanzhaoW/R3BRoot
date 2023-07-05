/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019-2023 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

// -----------------------------------------------------------
// -----                 R3BFileSource                   -----
// -----    Created 12/05/21 by J.L. Rodriguez-Sanchez   -----
// -----------------------------------------------------------

#include "R3BFileSource.h"
#include "R3BLogger.h"

#include "FairEventHeader.h"
#include "FairFileHeader.h"
#include "FairLogger.h"
#include "FairMCEventHeader.h"
#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "R3BEventHeader.h"
#include "TChainElement.h"
#include "TFolder.h"
#include "TObjArray.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TString.h"
#include <TChain.h>
#include <TKey.h>
#include <TList.h>
#include <TObjString.h>
#include <algorithm>
#include <boost/format.hpp>
#include <cstdlib>
#include <fmt/core.h>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <typeinfo>

// TMCThreadLocal R3BFileSource* R3BFileSource::fSourceInstance = 0;

namespace
{
    auto const DEFAULT_TITLE = "InputRootFile";

    template <typename ContainerType, typename DataType>
    auto Vector2TContainer(std::vector<DataType>& vec) -> std::unique_ptr<ContainerType>
    {
        using RawType = std::remove_reference_t<std::remove_pointer_t<DataType>>;
        static_assert(std::is_base_of_v<TObject, RawType>);
        auto list = std::make_unique<ContainerType>();
        for (auto& iter : vec)
        {
            if constexpr (std::is_pointer_v<DataType>)
            {
                list->Add(iter);
            }
            else
            {
                list->Add(&iter);
            }
        }
        return list;
    }

    template <typename StringType = std::string>
    auto GetBranchList(TFile* rootFile, std::string_view listName) -> std::vector<StringType>
    {
        auto branchList = std::vector<StringType>{};
        if (auto* list = dynamic_cast<TList*>(rootFile->Get(listName.data())); list != nullptr)
        {
            for (const auto& str : TRangeDynCast<TObjString>(list))
            {
                branchList.emplace_back(str->GetString().Data());
            }
        }
        else
        {
            throw std::runtime_error(fmt::format(
                "R3BFileSource: No branch list named {0} in input file {1}", listName, rootFile->GetName()));
        }
        return branchList;
    }

    auto HasBranchList(TFile* rootFile, const std::vector<std::string>& branchList) -> bool
    {
        auto const newBranchList = GetBranchList(rootFile, "BranchList");
        auto view1 = std::vector<std::string_view>(branchList.begin(), branchList.end());
        auto view2 = std::vector<std::string_view>(newBranchList.begin(), newBranchList.end());

        std::sort(view1.begin(), view1.end());
        std::sort(view2.begin(), view2.end());
        return view1 == view2;
    }

    template <typename ContainerType>
    auto GetDataFromAnyFolder(TFile* rootFile, const ContainerType& folderNames) -> std::optional<TKey*>
    {
        for (auto const& name : folderNames)
        {
            R3BLOG(debug, "looking for " + name);
            auto* dataFolder = dynamic_cast<TKey*>(rootFile->FindKey(name.c_str()));
            if (dataFolder != nullptr)
            {
                R3BLOG(debug, name + " has been found!");
                return dataFolder;
            }
        }
        return {};
    }

    auto Get_TChain_FromFairRM() -> TChain*
    {
        auto const chainTitle = "/" + std::string{ FairRootManager::GetFolderName() };
        auto inChain = std::make_unique<TChain>(FairRootManager::GetTreeName(), chainTitle.c_str());
        R3BLOG(debug, "Chain created");
        LOG(info) << "chain name: " << FairRootManager::GetTreeName();
        FairRootManager::Instance()->SetInChain(inChain.release());
        return FairRootManager::Instance()->GetInChain();
    }
} // namespace

auto R3BInputRootFiles::AddFileName(std::string fileName) -> std::optional<std::string>
{
    auto const msg = fmt::format("Adding {} to file source\n", fileName);
    R3BLOG(info, msg);
    if (fileNames_.empty())
    {
        Intitialize(fileName);
    }
    else if (!ValidateFile(fileName))
    {
        return fileName;
    }
    fileNames_.emplace_back(std::move(fileName));
    return {};
}

void R3BInputRootFiles::SetInputFileChain(TChain* chain)
{
    if (rootChain_ != nullptr)
    {
        throw std::runtime_error("R3BInputRootFiles: TChain has already been created!");
    }
    rootChain_ = chain;
    for (auto const& filename : fileNames_)
    {
        rootChain_->AddFile(filename.c_str(), TTree::kMaxEntries, treeName_.c_str());
    }
}
void R3BInputRootFiles::RegisterTo(FairRootManager* rootMan)
{
    if (fileNames_.empty())
    {
        throw std::runtime_error("R3BFileSource: file name list is empty!");
    }

    if (!is_friend_)
    {
        rootMan->SetListOfFolders(Vector2TContainer<TObjArray>(validTreeFolders_).release());
        SetInputFileChain(Get_TChain_FromFairRM());
    }
}

auto R3BInputRootFiles::ExtractMainFolder(TFile* rootFile) -> std::optional<TKey*>
{
    auto const folderNames =
        std::array<std::string, 4>{ FairRootManager::GetFolderName(), "r3broot", "cbmout", "cbmroot" };

    return GetDataFromAnyFolder(rootFile, folderNames);
}

auto R3BInputRootFiles::ValidateFile(const std::string& filename) -> bool
{
    auto rootFile = R3B::make_rootfile(filename.c_str());
    auto folderKey = ExtractMainFolder(rootFile.get());
    auto res = folderKey.has_value() && HasBranchList(rootFile.get(), branchList_);
    if (res)
    {
        if (!folderName_.empty() && (folderKey.value()->GetName() != folderName_))
        {
            R3BLOG(warn, "Different folder name!");
        }
        if (!is_friend_)
        {
            validRootFiles_.push_back(std::move(rootFile));
            validTreeFolders_.push_back((folderKey.value())->ReadObject<TFolder>());
        }
    }
    return res;
}

auto R3BInputRootFiles::ExtractRunId(TFile* rootFile) -> std::optional<uint>
{
    //
    auto* header = rootFile->Get<FairFileHeader>(fileHeader_.c_str());
    if (header == nullptr)
    {
        return {};
    }
    auto runID = header->GetRunId();
    return runID;
}

void R3BInputRootFiles::Intitialize(std::string_view filename)
{
    auto file = R3B::make_rootfile(filename.data());

    if (const auto runID = ExtractRunId(file.get()); runID.has_value() && runID.value() != 0)
    {
        auto const msg = fmt::format(R"(Successfully extract RunID "{}" from root file "{}")", runID.value(), filename);
        R3BLOG(debug, msg);
        initial_RunID_ = runID.value();
    }
    else
    {
        auto const msg = fmt::format("Failed to extract RunID from root file \"{}\"", filename);
        R3BLOG(error, msg);
    }

    if (auto folderKey = ExtractMainFolder(file.get()); folderKey.has_value())
    {
        folderName_ = folderKey.value()->GetName();
    }
    else
    {
        throw std::logic_error(fmt::format("R3BFileSource: Cannot find main folder from the root file {}!", filename));
    }

    branchList_ = GetBranchList(file.get(), "BranchList");
    for (auto const& branchName : branchList_)
    {
        FairRootManager::Instance()->AddBranchToList(branchName.c_str());
    }

    if (timeBasedBranchList_ = GetBranchList<TObjString>(file.get(), "TimeBasedBranchList");
        timeBasedBranchList_.empty())
    {
        LOG(warn) << "No time based branch list in input file";
    }
    FairRootManager::Instance()->SetTimeBasedBranchNameList(Vector2TContainer<TList>(timeBasedBranchList_).release());

    // TODO:
    //  fInputLevel.push_back(fInputTitle);
    //  fCheckInputBranches[fInputTitle] = std::move(branchList);
}

void R3BInputRootFiles::SetFriend(R3BInputRootFiles& friendFiles)
{
    if (is_friend_)
    {
        throw std::runtime_error("can not set friendFiles with another friendFile!");
    }
    auto chain = std::make_unique<TChain>(friendFiles.GetTitle().c_str(), friendFiles.GetFolderName().c_str());
    friendFiles.SetInputFileChain(chain.get());
    rootChain_->AddFriend(chain.release());
}

[[nodiscard]] auto R3BInputRootFiles::GetEntries() const -> int64_t
{
    if (rootChain_ == nullptr)
    {
        throw std::runtime_error("Can't get entries before being initialized!");
    }
    return rootChain_->GetEntries();
}

R3BFileSource::R3BFileSource(std::vector<std::string> fileNames, const std::string& title)
{
    inputDataFiles_.SetTitle(title);
    inputDataFiles_.SetFileHeaderName("FileHeader");
    for (auto& name : fileNames)
    {
        if (name.empty())
        {
            continue;
        }
        AddFile(std::move(name));
    }
}

R3BFileSource::R3BFileSource(std::string file, const std::string& title)
    : R3BFileSource(std::vector<std::string>{ std::move(file) }, title)
{
}

R3BFileSource::R3BFileSource(std::vector<std::string> fileNames)
    : R3BFileSource(std::move(fileNames), DEFAULT_TITLE)
{
}

R3BFileSource::R3BFileSource()
    : R3BFileSource(std::string{})
{
}

void R3BFileSource::AddFile(std::string fileName)
{

    if (auto const res = inputDataFiles_.AddFileName(std::move(fileName)); res.has_value())
    {
        R3BLOG(error,
               fmt::format(
                   "Root file {0} is incompatible with the first root file {1}", res.value(), dataFileNames_.front()));
    }
    dataFileNames_.emplace_back(fileName);
}

void R3BFileSource::AddFriend(std::string_view fileName)
{
    //
    auto rootfile = R3B::make_rootfile(fileName.data());
    auto friendGroup = std::find_if(inputFriendFiles_.begin(),
                                    inputFriendFiles_.end(),
                                    [&rootfile](const auto& friends)
                                    { return HasBranchList(rootfile.get(), friends.GetBranchListRef()); });
    if (friendGroup == inputFriendFiles_.end())
    {
        auto newFriendGroup = R3BInputRootFiles{};
        newFriendGroup.Make_as_friend();
        inputFriendFiles_.push_back(std::move(newFriendGroup));
        friendGroup = --inputFriendFiles_.end();
        friendGroup->SetTitle(fmt::format("FriendTree_{}", inputFriendFiles_.size()));
    }
    auto res = friendGroup->AddFileName(std::string{ fileName });
    if (res.has_value())
    {
        R3BLOG(error,
               fmt::format("Friend file {0} is incompatible with the first friend file {1}",
                           res.value(),
                           friendGroup->GetBaseFileName()));
    }
    else
    {
        // TODO: really need it?
        friendFileNames_.emplace_back(fileName);
    }
}

Bool_t R3BFileSource::Init()
{
    inputDataFiles_.RegisterTo(FairRootManager::Instance());

    for (auto& friendGroup : inputFriendFiles_)
    {
        inputDataFiles_.SetFriend(friendGroup);
    }

    return true;
}

void R3BFileSource::FillEventHeader(FairEventHeader* evtHeader)
{
    if (evtHeader == nullptr)
    {
        throw std::logic_error("Filled event header is empty!");
    }
    evtHeader_ = evtHeader;

    auto const init_runID = inputDataFiles_.GetInitialRunID();

    if (init_runID == 0)
    {
        throw std::logic_error("RunId is not being set!");
    }

    if (init_runID != GetRunId())
    {
        R3BLOG(error, "runID being set is different from the runID in the data file!");
    }
    evtHeader->SetRunId(init_runID);

    // TODO: what is this?
    // feh->SetRunId(fRunId);
    // feh->SetInputFileId(0);
}

Int_t R3BFileSource::CheckMaxEventNo(Int_t EvtEnd)
{
    return (EvtEnd == 0) ? inputDataFiles_.GetEntries() : EvtEnd; // NOLINT
}

//----------------------------------------------------------------
void R3BFileSource::ReadBranchEvent(const char* BrName)
{
    auto const currentEventID = evtHeader_->GetMCEntryNumber();
    ReadBranchEvent(BrName, currentEventID);
}

void R3BFileSource::ReadBranchEvent(const char* BrName, Int_t entryID)
{
    auto const read_bytes = inputDataFiles_.GetChain()->FindBranch(BrName)->GetEntry(entryID);
    if (read_bytes == 0)
    {
        LOG(warn) << fmt::format("Failed to read the data of the event {0} from the branch {1}", entryID, BrName);
    }
}

void R3BFileSource::Close()
{ /*CloseInFile();*/
}

void R3BFileSource::Reset() {}

Int_t R3BFileSource::ReadEvent(UInt_t eventID)
{
    // fCurrentEntryNo = eventID;
    // fEventTime = GetEventTime();

    // TODO: make colors as variables
    // TODO: add disable option
    auto* chain = inputDataFiles_.GetChain();
    auto const total_entries_num = chain->GetEntries();
    if (eventID > 0)
    {
        fmt::print("Processed: \033[32m {1} \033[0m of \033[34m {2} \033[0m (\033[33m {3:8.2f} \033[0m of "
                   "100), current RunId \033[31m {4:3d} \033[0m \r",
                   (eventID + 1),
                   total_entries_num,
                   100. * (eventID / static_cast<double>(total_entries_num)),
                   fRunId);
        std::cout << std::flush;
    }

    // TODO: clean this mess

    auto read_bytes = chain->GetEntry(eventID);
    if (read_bytes == 0)
    {
        LOG(warn) << fmt::format("Failed to read the data of the event {0} from the source", eventID);
        return 1;
    }
    return 0;
}

Bool_t R3BFileSource::ActivateObject(TObject** obj, const char* BrName)
{
    auto* chain = inputDataFiles_.GetChain();
    chain->SetBranchStatus(BrName, true);
    chain->SetBranchAddress(BrName, obj);
    return kTRUE;
}

// WTF is this?
Bool_t R3BFileSource::SpecifyRunId() { return ReadEvent(0) == 0; }

// ==============================================================================
// void R3BFileSource::SetInTree(TTree* tempTree)
// {
//     fInTree = NULL;
//     fInTree = tempTree;
//     fRootFile = static_cast<TFile*>(tempTree->GetCurrentFile());
//     fInChain->Reset();
//     IsInitialized = kFALSE;
//     Init();
// }

// // ----  Method GetRunid -----------------------------------------------------
// Int_t R3BFileSource::GetRunid(uint64_t st)
// {
//     UInt_t fArraysize = fTimestamp.size();
//     if (fArraysize != fRunid.size())
//     {
//         R3BLOG(error, "\033[5m\033[31m Different number of RunIds and timestamps \033[0m");
//         prevts = 0;
//         nextts = 0;
//         return 1;
//     }

//     for (Int_t j = 0; j < fArraysize - 1; j++)
//         if (st >= fTimestamp[j] && st < fTimestamp[j + 1])
//         {
//             prevts = fTimestamp[j];
//             nextts = fTimestamp[j + 1];
//             R3BLOG(debug, "New timestamp " << nextts << " for RunId " << fRunid[j]);
//             return fRunid[j];
//         }

//     if (st >= fTimestamp[fArraysize - 1])
//     {
//         prevts = fTimestamp[fArraysize - 1];
//         nextts = fTimestamp[fArraysize - 1];
//         R3BLOG(debug, "Prev/next timestamp " << prevts << "/" << nextts << " for runid " << fRunid[fArraysize - 1]);
//         return fRunid[fArraysize - 1];
//     }

//     if (nextts > 0)
//     {
//         R3BLOG(warn, "\033[5m\033[33m RunId was not found, it will be 1 \033[0m");
//     }
//     prevts = 0;
//     nextts = 0;

//     return 1;
// }

// // ----  Method ReadEvent ----------------------------------------------------

// void R3BFileSource::AddFriend(const TString& fName) { friendFileNames_.emplace_back(fName.Data()); }

// void R3BFileSource::AddFriendsToChain()
// {
//     // Loop over all Friend files and extract the type. The type is defined by
//     // the tree which is stored in the file. If there is already a chain of with
//     // this type of tree then the file will be added to this chain.
//     // If there is no such chain it will be created.
//     //
//     // Check if the order of runids and the event numbers per runid for all
//     // friend chains is the same as the one defined by the input chain.
//     // TODO: Should the order be corrected or should the execution be stopped.
//     // The order in the input tree defined by the order in which the files have
//     // been added. A file is defined by the runid.

//     // In the old way it was needed sometimes to add a freind file more
//     // than once. This is not needed any longer, so we remove deuplicates
//     // from the list and display a warning.
//     std::list<TString> friendList;
//     for (auto fileName : friendFileNames_)
//     {
//         if (find(friendList.begin(), friendList.end(), fileName) == friendList.end())
//         {
//             friendList.push_back(fileName);
//         }
//     }
//     // TODO: print a warning if it was neccessary to remove a filname from the
//     // list. This can be chacked by comparing the size of both list

//     TFile* temp = gFile;

//     Int_t friendType = 1;
//     // Loop over all files which have been added as friends
//     for (auto fileName : friendList)
//     {
//         TString inputLevel;
//         // Loop over all already defined input levels to check if this type
//         // of friend tree is already added.
//         // If this type of friend tree already exist add the file to the
//         // then already existing friend chain. If this type of friend tree
//         // does not exist already create a new friend chain and add the file.
//         Bool_t inputLevelFound = kFALSE;
//         TFile* inputFile;
//         for (auto level : fInputLevel)
//         {
//             inputLevel = level;

//             LOG(debug2) << "open root file " << fileName;
//             inputFile = TFile::Open(fileName);
//             if (inputFile->IsZombie())
//             {
//                 LOG(fatal) << "Error opening the file " << level
//                            << " which should be added to the input chain or as friend chain";
//             }

//             // Check if the branchlist is already stored in the map. If it is
//             // already stored add the file to the chain.
//             Bool_t isOk = CompareBranchList(inputFile, inputLevel);
//             if (isOk)
//             {
//                 inputLevelFound = kTRUE;
//                 inputFile->Close();
//                 continue;
//             }
//             LOG(debug2) << "close root file " << inputFile->GetName();
//             inputFile->Close();
//         }
//         if (!inputLevelFound)
//         {
//             inputLevel = Form("FriendTree_%i", friendType);
//             CreateNewFriendChain(fileName, inputLevel);
//             friendType++;
//         }

//         TChain* chain = static_cast<TChain*>(fFriendTypeList[inputLevel]);
//         chain->AddFile(fileName, 1234567890, FairRootManager::GetTreeName());
//     }
//     gFile = temp;

//     // Check if all friend chains have the same runids and the same
//     // number of event numbers as the corresponding input chain
//     // CheckFriendChains();

//     // Add all the friend chains which have been created to the main input chain.
//     for (const auto& mi : fFriendTypeList)
//     {
//         TChain* chain = static_cast<TChain*>(mi.second);
//         fInChain->AddFriend(chain);
//     }

//     // Print some output about the input structure
//     PrintFriendList();
// }

// void R3BFileSource::PrintFriendList()
// {
//     // Print information about the input structure
//     // List files from the input chain together with all files of
//     // all friend chains

//     LOG(info) << "The input consists out of the following trees and files: ";
//     LOG(info) << " - " << fInChain->GetName();
//     TObjArray* fileElements = fInChain->GetListOfFiles();
//     TIter next(fileElements);
//     TChainElement* chEl = 0;
//     while ((chEl = static_cast<TChainElement*>(next())))
//     {
//         LOG(info) << "    - " << chEl->GetTitle();
//     }

//     for (const auto& mi : fFriendTypeList)
//     {
//         TChain* chain = static_cast<TChain*>(mi.second);
//         LOG(info) << " - " << chain->GetName();
//         fileElements = chain->GetListOfFiles();
//         TIter next1(fileElements);
//         chEl = 0;
//         while ((chEl = static_cast<TChainElement*>(next1())))
//         {
//             LOG(info) << "    - " << chEl->GetTitle();
//         }
//     }
// }

// void R3BFileSource::CheckFriendChains()
// {
//     std::multimap<TString, std::multimap<TString, TArrayI>>::iterator it1;
//     std::multimap<TString, TArrayI> map1;

//     // Get the structure from the input chain
//     it1 = fRunIdInfoAll.find("InputChain");
//     map1 = it1->second;
//     std::vector<Int_t> runid;
//     std::vector<Int_t> events;
//     for (auto& mmi : map1)
//     {
//         TArrayI bla = mmi.second;
//         runid.push_back(bla[0]);
//         events.push_back(bla[1]);
//     }

//     // Now loop over all chains except the input chain and comapare the
//     // runids and event numbers.
//     // If there is a mismatch stop the execution.
//     Int_t errorFlag = 0;
//     TString inputLevel;
//     for (auto level : fInputLevel)
//     {
//         inputLevel = level;
//         if (!inputLevel.Contains("InputChain"))
//         {
//             it1 = fRunIdInfoAll.find(inputLevel);
//             map1 = it1->second;
//             if (runid.size() != map1.size())
//             {
//                 errorFlag = 1;
//                 break;
//             }
//             Int_t counter = 0;
//             for (auto& mmi : map1)
//             {
//                 TArrayI bla = mmi.second;
//                 if ((bla[0] != runid[counter]) || (bla[1] != events[counter]))
//                 {
//                     errorFlag = 2;
//                     break;
//                 }
//                 counter++;
//             }
//             if (errorFlag > 0)
//             {
//                 break;
//             }
//         }
//     }

//     if (errorFlag > 0)
//     {
//         LOG(error) << "The input chain and the friend chain " << inputLevel.Data() << " have a different structure:";
//         if (errorFlag == 1)
//         {
//             LOG(error) << "The input chain has the following runids and event numbers:";
//             for (UInt_t i = 0; i < runid.size(); i++)
//             {
//                 LOG(error) << " - Runid " << runid[i] << " with " << events[i] << " events";
//             }
//             LOG(error) << "The " << inputLevel.Data() << " chain has the following runids and event numbers:";
//             for (auto& mmi : map1)
//             {
//                 TArrayI bla = mmi.second;
//                 LOG(error) << " - Runid " << bla[0] << " with " << bla[1] << " events";
//             }
//         }
//         if (errorFlag == 2)
//         {
//             Int_t counter = 0;
//             for (auto& mmi : map1)
//             {
//                 TArrayI bla = mmi.second;
//                 LOG(error) << "Runid Input Chain, " << inputLevel.Data() << " chain: " << bla[0] << ", "
//                            << runid[counter];
//                 LOG(error) << "Event number Input Chain, " << inputLevel.Data() << " chain: " << bla[1] << ", "
//                            << events[counter];
//                 counter++;
//             }
//         }
//         LOG(fatal) << "Event structure mismatch";
//     }
// }

// void R3BFileSource::CreateNewFriendChain(TString inputFile, TString inputLevel)
// {

//     TFile* temp = gFile;
//     LOG(debug2) << "open root file " << inputFile;
//     TFile* f = TFile::Open(inputFile);

//     TString folderName1 = FairRootManager::GetFolderName();
//     TString folderName = Form("/%s", folderName1.Data());
//     auto* added = dynamic_cast<TFolder*>(f->Get(folderName1));
//     if (!added)
//     {
//         folderName = "/r3broot";
//         folderName1 = "r3broot";
//         added = dynamic_cast<TFolder*>(f->Get("r3broot"));
//         if (!added)
//         {
//             folderName = "/cbmout";
//             folderName1 = "cbmout";
//             added = dynamic_cast<TFolder*>(f->Get("cbmout"));
//             if (!added)
//             {
//                 folderName = "/cbmroot";
//                 folderName1 = "cbmroot";
//                 added = dynamic_cast<TFolder*>(f->Get("cbmroot"));
//                 if (!added)
//                 {
//                     LOG(fatal) << "Could not find folder r3broot, cbmout nor cbmroot.";
//                     exit(-1);
//                 }
//             }
//         }
//     }
//     folderName1 = folderName1 + "_" + inputLevel;
//     added->SetName(folderName1);
//     fListFolder->Add(added);

//     /**Get The list of branches from the friend file and add it to the actual list*/
//     auto* list = dynamic_cast<TList*>(f->Get("BranchList"));
//     std::string chainName = inputLevel.Data();
//     fInputLevel.push_back(chainName);
//     fCheckInputBranches[chainName] = std::vector<std::string>{};
//     if (list)
//     {
//         TObjString* Obj = 0;
//         for (Int_t i = 0; i < list->GetEntries(); i++)
//         {
//             Obj = dynamic_cast<TObjString*>(list->At(i));
//             fCheckInputBranches[chainName].emplace_back(Obj->GetString().Data());
//             std::cout << Obj->GetString().Data() << std::endl;
//             FairRootManager::Instance()->AddBranchToList(Obj->GetString().Data());
//         }
//     }

//     TChain* chain = new TChain(inputLevel, folderName);
//     fFriendTypeList[inputLevel] = chain;

//     f->Close();
//     gFile = temp;
// }

// Bool_t R3BFileSource::CompareBranchList(TFile* fileHandle, TString inputLevel)
// {
//     // fill a set with the original branch structure
//     // This allows to use functions find and erase
//     std::set<TString> branches;
//     for (auto li : (fCheckInputBranches[inputLevel]))
//     {
//         branches.insert(li);
//     }

//     // To do so we have to loop over the branches in the file and to compare
//     // the branches in the file with the information stored in
//     // fCheckInputBranches["InputChain"]. If both lists are equal everything
//     // is okay

//     // Get The list of branches from the input file one by one and compare
//     // it to the reference list of branches which is defined for this tree.
//     // If a branch with the same name is found, this branch is removed from
//     // the list. If in the end no branch is left in the list everything is
//     // fine.
//     std::set<TString>::iterator iter1;
//     auto* list = dynamic_cast<TList*>(fileHandle->Get("BranchList"));
//     if (list)
//     {
//         TObjString* Obj = 0;
//         for (Int_t i = 0; i < list->GetEntries(); i++)
//         {
//             Obj = dynamic_cast<TObjString*>(list->At(i));
//             iter1 = branches.find(Obj->GetString().Data());
//             if (iter1 != branches.end())
//             {
//                 branches.erase(iter1);
//             }
//             else
//             {
//                 // Not found is an error because branch structure is
//                 // different. It is impossible to add to tree with a
//                 // different branch structure
//                 return kFALSE;
//             }
//         }
//     }
//     // If the size of branches is !=0 after removing all branches also in the
//     // reference list, this is also a sign that both branch list are not the
//     // same
//     if (branches.size() != 0)
//     {
//         LOG(info) << "Compare Branch List will return kFALSE. The list has " << branches.size() << " branches:";
//         for (auto branchName : branches)
//             LOG(info) << "  -> " << branchName;
//         return kFALSE;
//     }

//     return kTRUE;
// }

// void R3BFileSource::SetInputFile(const TString& name)
// {
//     LOG(debug2) << "open root file " << name;
//     fRootFile = TFile::Open(name.Data());
//     if (fRootFile->IsZombie())
//     {
//         LOG(fatal) << "Error opening the Input file";
//     }
//     LOG(info) << "R3BFileSource set------------";
// }

// void R3BFileSource::SetEventMeanTime(Double_t mean)
// {
//     fEventMeanTime = mean;
//     fTimeProb = new TF1("TimeProb", "(1/[0])*exp(-x/[0])", 0., mean * 10);
//     fTimeProb->SetParameter(0, mean);
//     fTimeProb->GetRandom();
//     fEventTimeInMCHeader = kFALSE;
// }

// void R3BFileSource::SetEventTimeInterval(Double_t min, Double_t max)
// {
//     fEventTimeMin = min;
//     fEventTimeMax = max;
//     fEventMeanTime = (fEventTimeMin + fEventTimeMax) / 2.0;
//     fEventTimeInMCHeader = kFALSE;
// }

// void R3BFileSource::SetBeamTime(Double_t beamTime, Double_t gapTime)
// {
//     fBeamTime = beamTime;
//     fGapTime = gapTime;
// }

// void R3BFileSource::SetEventTime()
// {
//     // Check if the time for the current entry is already set
//     if (fTimeforEntryNo == fCurrentEntryNo)
//         return;
//     LOG(debug) << "Set event time for Entry = " << fTimeforEntryNo << " , where the current entry is "
//                << fCurrentEntryNo << " and eventTime is " << fEventTime;
//     if (fBeamTime < 0)
//     {
//         fEventTime += GetDeltaEventTime();
//     }
//     else
//     {
//         do
//         {
//             fEventTime += GetDeltaEventTime();
//         } while (fmod(fEventTime, fBeamTime + fGapTime) > fBeamTime);
//     }
//     LOG(debug) << "New time = " << fEventTime;
//     fTimeforEntryNo = fCurrentEntryNo;
// }

// Double_t R3BFileSource::GetDeltaEventTime()
// {
//     Double_t deltaTime = 0;
//     if (fTimeProb != 0)
//     {
//         deltaTime = fTimeProb->GetRandom();
//         LOG(debug) << "Time set via sampling method : " << deltaTime;
//     }
//     else
//     {
//         deltaTime = gRandom->Uniform(fEventTimeMin, fEventTimeMax);
//         LOG(debug) << "Time set via Uniform Random : " << deltaTime;
//     }
//     return deltaTime;
// }

// Double_t R3BFileSource::GetEventTime()
// {
//     LOG(debug) << "-- Get Event Time --";
//     if (!fEvtHeaderIsNew && fEvtHeader != 0)
//     {
//         Double_t EvtTime = fEvtHeader->GetEventTime();
//         if (!(EvtTime < 0))
//         {
//             return EvtTime;
//         }
//     }

//     if (fEventTimeInMCHeader && !fMCHeader)
//     {
//         LOG(debug) << "No MCEventHeader, time is set to 0";
//         return 0;
//     }
//     else if (fEventTimeInMCHeader && fMCHeader)
//     {
//         fEventTime = fMCHeader->GetT();
//         LOG(debug) << "Get event time from MCEventHeader : " << fEventTime << " ns";
//         return fEventTime;
//     }
//     else
//     {

//         if (fTimeforEntryNo != fCurrentEntryNo)
//         {
//             SetEventTime();
//         }
//         LOG(debug) << "Calculate event time from user input : " << fEventTime << " ns";
//         return fEventTime;
//     }
// }

// void R3BFileSource::TextFileInit()
// {
//     // Open configuration file with runid values if needed in this step
//     fInputTxTFile.open(fInputFileName.Data(), std::fstream::in);
//     if (!fInputTxTFile.is_open())
//     {
//         R3BLOG(warn, "Input file for RunIds was not found, it is Ok!");
//     }
//     else
//     {
//         R3BLOG(info, "Input file for RunIds " << fInputFileName.Data() << " was found");
//         fInputTxTFile.clear();
//         fInputTxTFile.seekg(0, std::ios::beg);
//     }

//     if (fInputTxTFile.is_open())
//     {
//         R3BLOG(info, "Reading RunId file");
//         Int_t rid = 0;
//         Int_t expRun = 0;
//         int64_t timeStamp = 0;
//         while (fInputTxTFile >> std::hex >> rid >> expRun >> timeStamp)
//         {
//             fRunid.push_back(rid);
//             fTimestamp.push_back(timeStamp);
//             // Ignore the other stuff that might still be on that line
//             fInputTxTFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//         }

//         R3BLOG(info, "End of reading RunId file");
//         fInputTxTFile.close();
//     }
//     else
//     {
//         nextts = 0;
//     }
// }

ClassImp(R3BFileSource);
