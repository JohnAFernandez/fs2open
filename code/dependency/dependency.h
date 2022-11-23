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
