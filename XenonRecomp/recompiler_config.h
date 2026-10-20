#pragma once

struct RecompilerSwitchTable
{
    uint32_t r;
    std::vector<uint32_t> labels;
};

struct RecompilerMidAsmHook
{
    std::string name;
    std::vector<std::string> registers;

    bool ret = false;
    bool returnOnTrue = false;
    bool returnOnFalse = false;

    uint32_t jumpAddress = 0;
    uint32_t jumpAddressOnTrue = 0;
    uint32_t jumpAddressOnFalse = 0;

    bool afterInstruction = false;
};

struct RecompilerConfig
{
    std::string directoryPath;
    std::string filePath;
    std::string patchFilePath;
    std::string patchedFilePath;
    std::string outDirectoryPath;
    std::string switchTableFilePath;
    std::unordered_map<uint32_t, RecompilerSwitchTable> switchTables;
    bool skipLr = false;
    bool ctrAsLocalVariable = false;
    bool xerAsLocalVariable = false;
    bool reservedRegisterAsLocalVariable = false;
    bool skipMsr = false;
    bool crRegistersAsLocalVariables = false;
    bool nonArgumentRegistersAsLocalVariables = false;
    bool nonVolatileRegistersAsLocalVariables = false;
    // group_stores: runs of stores off one base register in straight-line code (no label, branch or call between them,
    // the register unchanged, the run spanning less than 4 KB) get one PPC_CHECK_STORES before their first store, and
    // their stores are PPC_STORE_COVERED_*: a runtime that checks each store's pages (to see what the guest writes) can
    // check such a run once. With the defaults (no check; covered stores are plain ones) the code does the same.
    bool groupStores = false;
    uint32_t restGpr14Address = 0;
    uint32_t saveGpr14Address = 0;
    uint32_t restFpr14Address = 0;
    uint32_t saveFpr14Address = 0;
    uint32_t restVmx14Address = 0;
    uint32_t saveVmx14Address = 0;
    uint32_t restVmx64Address = 0;
    uint32_t saveVmx64Address = 0;
    uint32_t longJmpAddress = 0;
    uint32_t setJmpAddress = 0;
    std::unordered_map<uint32_t, uint32_t> functions;
    // Functions that keep every register in the PPC context despite the *_as_local options, because they exchange
    // values with other functions outside the calling convention (SEH funclets and their parents, functions split
    // around jump tables, the register save/restore helpers): keep_registers_in_context, or the same array in the file
    // keep_registers_in_context_file_path names.
    std::unordered_set<uint32_t> keepRegistersInContext;
    std::unordered_map<uint32_t, uint32_t> invalidInstructions;
    std::unordered_map<uint32_t, RecompilerMidAsmHook> midAsmHooks;

    void Load(const std::string_view& configFilePath);
};
