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

#ifndef R3BFileSource_H
#define R3BFileSource_H 1

#include "FairSource.h"
#include "R3BShared.h"
#include "TChain.h"
#include "TF1.h"
#include "TFile.h"
#include "TFolder.h"
#include "TMCtls.h"
#include <TObjString.h>
#include <fstream>
#include <list>

class FairEventHeader;
class FairFileHeader;
class FairMCEventHeader;
class TString;
class FairLogger;
class FairRuntimeDb;
class R3BEventHeader;
class FairRootManager;

class R3BInputRootFiles
{
  public:
    using Strings = std::vector<std::string>;
    R3BInputRootFiles() = default;
    auto AddFileName(std::string name) -> std::optional<std::string>;
    void SetInputFileChain(TChain* chain);
    void RegisterTo(FairRootManager*);

    [[nodiscard]] auto is_friend() const -> bool { return is_friend_; }
    void Make_as_friend() { is_friend_ = true; }
    void SetFriend(R3BInputRootFiles& friendFiles);
    // Getters:
    [[nodiscard]] auto GetBranchListRef() const -> const auto& { return branchList_; }
    [[nodiscard]] auto GetBaseFileName() const -> const auto& { return fileNames_.front(); }
    [[nodiscard]] auto GetTreeName() const -> const auto& { return treeName_; }
    [[nodiscard]] auto GetFolderName() const -> const auto& { return folderName_; }
    [[nodiscard]] auto GetTitle() const -> const auto& { return title_; }
    [[nodiscard]] auto GetEntries() const -> int64_t;
    [[nodiscard]] auto GetChain() const -> TChain* { return rootChain_; }
    [[nodiscard]] auto GetInitialRunID() const { return initial_RunID_; }

    // Setters:
    void SetTreeName(std::string_view treeName) { treeName_ = treeName; }
    void SetTitle(std::string_view title) { title_ = title; }
    void SetFileHeaderName(std::string_view fileHeader) { fileHeader_ = fileHeader; }

    // rule of five:
    ~R3BInputRootFiles() = default;
    R3BInputRootFiles(const R3BInputRootFiles&) = delete;
    R3BInputRootFiles(R3BInputRootFiles&&) = default;
    R3BInputRootFiles& operator=(const R3BInputRootFiles&) = delete;
    R3BInputRootFiles& operator=(R3BInputRootFiles&&) = default;

  private:
    bool is_friend_ = false;
    uint initial_RunID_ = 0;
    std::string title_;
    std::string treeName_ = "evt";
    std::string folderName_;
    std::string fileHeader_;
    Strings fileNames_;
    Strings branchList_;
    std::vector<TObjString> timeBasedBranchList_;
    std::vector<R3B::unique_rootfile> validRootFiles_;
    std::vector<TFolder*> validTreeFolders_;
    TChain* rootChain_ = nullptr;

    void Intitialize(std::string_view filename);
    auto ValidateFile(const std::string& filename) -> bool;
    static auto ExtractMainFolder(TFile*) -> std::optional<TKey*>;
    auto ExtractRunId(TFile* rootFile) -> std::optional<uint>;
};

class R3BFileSource : public FairSource
{
  public:
    R3BFileSource();
    explicit R3BFileSource(std::string file, const std::string& title = "InputRootFile");
    R3BFileSource(std::vector<std::string> fileNames, const std::string& title);
    explicit R3BFileSource(std::vector<std::string> fileNames);

    void AddFile(std::string);
    void AddFriend(std::string_view);
    void SetFileHeaderName(std::string_view fileHeaderName) { inputDataFiles_.SetFileHeaderName(fileHeaderName); }

  private:
    R3BInputRootFiles inputDataFiles_;
    FairEventHeader* evtHeader_ = nullptr;
    std::vector<R3BInputRootFiles> inputFriendFiles_;
    std::vector<std::string> dataFileNames_;
    std::vector<std::string> friendFileNames_;

    Bool_t Init() override;
    Int_t ReadEvent(UInt_t eventID = 0) override;
    void Close() override;
    void Reset() override;
    Bool_t InitUnpackers() override { return kTRUE; }
    Bool_t ReInitUnpackers() override { return kTRUE; }
    Source_Type GetSourceType() override { return kFILE; }
    void SetParUnpackers() override {}
    Int_t CheckMaxEventNo(Int_t EvtEnd = 0) override;
    void ReadBranchEvent(const char* BrName) override;
    void ReadBranchEvent(const char* BrName, Int_t Entry) override;
    void FillEventHeader(FairEventHeader* evtHeader) override;
    Bool_t ActivateObject(TObject** obj, const char* BrName) override;
    Bool_t SpecifyRunId() override;

    //=========================================================

    // auto GetRootFiles() const -> const std::vector<R3B::unique_rootfile>& { return fRootFiles; }
    // [[deprecated("Use GetRootFiles instead!")]] const TFile* GetRootFile() { return fRootFile; }
    // /** Add a friend file (input) by name)*/
    // [[deprecated]] void AddFriend(const TString&);
    // /**Add ROOT file to input, the file will be chained to already added files*/
    // void AddFriendsToChain();
    // void PrintFriendList();
    // void CheckFriendChains();
    // void CreateNewFriendChain(TString inputFile, TString inputLevel);
    // TTree* GetInTree() { return fInChain->GetTree(); }
    // TChain* GetInChain() { return fInChain; }
    // TFile* GetInFile() { return fRootFile; }
    // [[deprecated("Root files are closed automatically")]] void CloseInFile()
    // {
    //     if (fRootFile != nullptr)
    //     {
    //         fRootFile->Close();
    //     }
    // }
    // /**Set the input tree when running on PROOF worker*/
    // void SetInTree(TTree* tempTree);
    // TObjArray* GetListOfFolders() { return fListFolder; }
    // TFolder* GetBranchDescriptionFolder() { return nullptr; }
    // UInt_t GetEntries() const { return fNoOfEntries; }

    // [[deprecated("Use AddFile instead")]] void SetInputFile(const TString& name);

    // /** Set the repetition time of the beam when it can interact (beamTime) and when no interaction happen (gapTime).
    //  * The total repetition time is beamTime + gapTime */
    // void SetBeamTime(Double_t beamTime, Double_t gapTime);
    // /** Set the min and max limit for event time in ns */
    // void SetEventTimeInterval(Double_t min, Double_t max);
    // /** Set the mean time for the event in ns */
    // void SetEventMeanTime(Double_t mean);
    // void SetEventTime();
    // Double_t GetDeltaEventTime();
    // void SetFileHeader(FairFileHeader* f) { fFileHeader = f; }
    // Double_t GetEventTime();

    // /**Set the status of the EvtHeader
    //  *@param Status:  True: The header was creatged in this session and has to be filled
    //           FALSE: We use an existing header from previous data level
    //  */
    // void SetEvtHeaderNew(Bool_t Status) { fEvtHeaderIsNew = Status; }
    // Bool_t IsEvtHeaderNew() { return fEvtHeaderIsNew; }

    // /** Allow to disable the testing the file layout when adding files to a chain.
    //  */
    // void SetCheckFileLayout(Bool_t enable) { fCheckFileLayout = enable; }

    // void SetInputFileName(TString tstr) { fInputFileName = tstr; }

    // static pointer to this class
    // static TMCThreadLocal R3BFileSource* fSourceInstance;
    /** Title of input source, could be input, background or signal*/
    // std::string fInputTitle;
    // /**ROOT file*/
    // TFile* fRootFile = nullptr;
    // std::string fRootFileName;
    // std::vector<R3B::unique_rootfile> fRootFiles;
    // /** Current Entry number */
    // Int_t fCurrentEntryNr = 0; //!
    // /** List of all files added with AddFriend */
    // std::map<TString, TChain*> fFriendTypeList;                       //!
    // std::map<TString, std::vector<std::string>> fCheckInputBranches;  //!
    // std::vector<std::string> fInputLevel;                             //!
    // std::map<TString, std::multimap<TString, TArrayI>> fRunIdInfoAll; //!
    /**Input Chain */
    // TChain* fInChain = nullptr;
    // /**Input Tree */
    // TTree* fInTree = nullptr;
    // /** list of folders from all input (and friends) files*/
    // TObjArray* fListFolder = nullptr; //!
    // /** RuntimeDb*/
    // FairRuntimeDb* fRtdb = nullptr;
    // /**folder structure of output*/
    // TFolder* fFolderOut = nullptr;
    // /***/
    // unsigned int fSourceIdentifier = 0;
    // /**No of Entries in this source*/
    // unsigned int fNoOfEntries = 0;
    // /**Initialization flag, true if initialized*/
    // Bool_t IsInitialized = false;

    // /** MC Event header */
    // FairMCEventHeader* fMCHeader = nullptr; //!

    // /**Event Header*/
    // R3BEventHeader* fEvtHeader = nullptr; //!

    // /**File Header*/
    // FairFileHeader* fFileHeader = nullptr; //!

    // /** This is true if the event time used, came from simulation*/
    // Bool_t fEventTimeInMCHeader = true; //!
    // /**This flag is true if the event header was created in this session
    //  * otherwise it is false which means the header was created in a previous data
    //  * level and used here (e.g. in the digi)
    //  */
    // Bool_t fEvtHeaderIsNew = false; //!

    // /** for internal use, to return the same event time for the same entry*/
    // UInt_t fCurrentEntryNo = 0; //!
    // /** for internal use, to return the same event time for the same entry*/
    // UInt_t fTimeforEntryNo = -1; //!

    // /** min time for one event (ns) */
    // Double_t fEventTimeMin = 0; //!
    // /** max time for one Event (ns) */
    // Double_t fEventTimeMax = 0; //!
    // /** Time of event since th start (ns) */
    // Double_t fEventTime = 0; //!
    // /** Time of particles in beam (ns) */
    // Double_t fBeamTime = 0; //!
    // /** Time without particles in beam (gap) (ns) */
    // Double_t fGapTime = -1; //!
    // /** EventMean time used (P(t)=1/fEventMeanTime*Exp(-t/fEventMeanTime) */
    // Double_t fEventMeanTime = 0; //!
    // /** used to generate random numbers for event time; */
    // TF1* fTimeProb = nullptr; //!
    // /** True if the file layout should be checked when adding files to a chain.
    //  *  Default value is true.
    //  */
    // Bool_t fCheckFileLayout = true; //!

    // /** GetRunid method to obtain the runid as function of timestamps */
    // Int_t GetRunid(uint64_t ts);

    // /** input file with runids */
    // std::ifstream fInputTxTFile;
    // TString fInputFileName;

    // UInt_t fExpid = 0;
    // std::vector<UInt_t> fRunid;
    // std::vector<uint64_t> fTimestamp;
    // uint64_t prevts = 0;
    // uint64_t nextts = 0;

    // R3BFileSource(const R3BFileSource&);
    // R3BFileSource operator=(const R3BFileSource&);
    // void InChainInit();
    // void TextFileInit();
    // Bool_t CompareBranchList(TFile* fileHandle, TString inputLevel);

    ClassDefOverride(R3BFileSource, 0) // NOLINT
};

#endif /* R3BFileSource_H */
