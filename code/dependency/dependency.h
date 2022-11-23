#pragma once

#include "globalincs/pstypes"

// Alternatively, enumerate every single mod that is possibly mentioned on the dependency list
// MPSUPER BEM -> SUPER BEM -> BEM -> MEDIAVPS -> RETAIL
//					V FRED'S COCKPITS -> RANDOM TOSH -> MEDIAVPS -> RETAIL
// So, go through each dependency possibility and search for the one that's furthest down
// Then you have to record what mod was looking for that.

// Make a list of the versions of retail to check, then go to the mods searching for it.
// MEDIAVPS was looking for retail, look at what versions of retail it accepts.
// MEDIAVPS accepts 2.0 (the only version)
// Look at other mods that are searching for retail.
// No other mods searching for retail.
// RETAIL 2.0 (the lowest level of dependency) 
// is acceptable in all possible dependency resolutions.
// Add Retail 2.0 as the end of all the Mediavp dependency lists.
// Look at the next level up, MEDIAVPS is the next level up.
// Go to the mods that are looking for it.
// Bem will only accept 3.4.  All the other versions get eliminated on its trees.
// MediaVps looks at Random Tosh next
// Random Tosh (the only version) wants 3.5,  
// But it has been marked as eliminated by (gasp) all versions of Bem
// (So at each version of each node, we have to list what mods and versions eliminate it)
// (Because we're going from the bottom up, building the dependency tree)
// (we basically are able to )
// Random Tosh is listed as in conflict with BEM
// WE STILL GO UP THE TREE.
// Random Tosh was wanted by Fred's Cockpits
// Fred's cockpits only require random tosh for version 1.0, 1.1 is fine. 
// So Fred's cockpit's trees for 1.0 inherit Tosh's conflicts.  
// IT CANNOT COEXIST WITH BEM and the reason is listed. 
// All possible versions of MediaVPs compatible with Fred were eliminated by BEM
// But Fred 1.1 works because it does not require Tosh.
// So Fred's 1.1 dependency tree is empty and does not contain conflicts.


// The one problem that's coming up is that it's hard to figure out when to stop evaluating 
// if something conflicts.  We have to keep evaluating until their last junction.


// Ok, go through each thing listed in the mod's dependency, and the recursively
// look through the things it would depend on and recursively what they would depend on.
// As you go down the list, when you get to a new node, list all the things that
// are looking for that node.

//MPSUPER BEM -> SUPER BEM -> BEM -> MEDIAVPS -> RETAIL
//				  V FRED'S COCKPITS -> RANDOM TOSH -> MEDIAVPS -> RETAIL

// The last place to determine whether BEM and TOSH are compatible is at SUPER BEM?
// Well, there's the other thing.  If all versions of TOSH conflict with all version of BEM
// Then we look at the level above them to see if something can be eliminated.
// But it's a waste because we should just be looking at the junction.

// That's just the thing.... We can carry the list of conflicts up the tree and add to them.

// Circular dependencies will need checking.

enum Dependency_Result {
    SUCCESS,
    CIRCULAR_REFERENCE,
    CONFLICT
}

struct mod_version {
    SCP_string name;
    SCP_string version;
};

// lol we're going to write this later.
SCP_vector<mod_version> get_direct_dependencies(mod_version mod_in);

// returns true in success, false in failure.
std::pair<Dependency_Result, SCP_vector<mod_version>>  sol_gate_resolve_dependencies(mod_version mod_in)
{
    // get our first new dependencies
    auto mod_list = get_direct_dependencies(mod_in);

    // set up our return
    std::pair<Dependency_Result, SCP_vector<mod_version>> return_val;

    // if there are no dependencies, just return.
    if (mod_list.empty()){
        return_val.first = Dependency_Result::SUCCESS;
        return_val.push_back(mod_in);
        return return_val;
    }

    // Our naive dependency tree
    // std::pair will allow us to detect circular references
    SCP_vector<SCP_vector<std::pair<mod_version, uint>>> dep_tree;

    // I don't think this will hold, but I can dream.
    dep_tree.emplace_back();

    // quickly add the first dependencies to the tree.
    for (auto item : mod_in){
        dep_tree.front().emplace_back(item, 0);
    }

    SCP_unordered_set<mod_version> found_mods;
    SCP_unordered_set<std::pair<uint, uint>> to_check;

    // now start adding dependencies further down
    while(!mod_list.empty()){
        SCP_vector<mod_version> new_mod_list;
        // again, we'll see.
        dep_tree.emplace_back();
        uint id = 0;

        // get the next set of dependencies for the dependencies that were just received.
        for (auto& new_dep : mod_list){
            auto portion = get_direct_dependencies(new_dep);

            for (auto& item : portion){
                dep_tree.back().emplace_back(item, id);
                if (found_mods.insert(item).first){
                    to_search.insert(std::pair<uint, uint>(dep_tree.size() - 1, id));
                }              
            }

            new_mod_list.push_back(std::move(portion));
            
            ++id;
        }

        // TODO finish circular dependency check.
        // might not be the right check
        if (!to_check.empty()){
            for (auto& test : new_mod_list) {
                for (auto& vec : dep_tree){
                    for (auto& item : vec){
                        if (item == test){

                        }
                    }
                }
            }
        }


        mod_list = std::move(new_mod_list);
    }

    


}





SCP_unordered_set<SCP_string> found_mods;






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
