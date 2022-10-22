#pragma once

#include "globalincs/pstypes"




class dependency_manager {

    SCP_vector<std::pair<SCP_string, SCP_vector<SEMVER>>> mod; // populated by mod names and their versions

    // so the question is, do I want to enforce the requirements of lower levels....
    // I think so.  Because MediaVPs should not be forced to use something it doesn't want

    // Ok. So now... go through each level down, and look for a level that becomes completely unable to continue.
    // That is the most useful info, because you realize, "oh, I need to make sure my high level dependencies"
    // "allow for this low level dependency, and if I can't, I need to adjust my dependency tree."


    // Here's the secret sauce. Map, lookup by mod name and SEMVER version, then a vector which holds what 
    // eliminated that specific version. If this mod was the failure point, we'll be able to dump exactly what happned.
    SCP_undordered_map<std::pair<SCP_string, SEMVER>, SCP_vector<std::pair<SCP_string, SEMVER>>> eligibility;

    // We're going to go with "use the oldest available that works"
    void add_direct_dependencies(const std::pair<SCP_string, SEMVER>& mod_out)
    {
        // use whatever API here to get the list of direct dependencies.
        auto mod_list_in = hocus_pocus(mod_out);

        for (const auto& new_mod  : mod_list_in){
            auto semver_list_in = get_versions();

        }

        mod.push_
    }


}