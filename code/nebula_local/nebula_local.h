#pragma once

#include "globalincs/globals.h"
#include "globalincs/pstypes.h"
#include "globalincs/flagset.h"
#include "graphics/color.h"

class object;

class model_draw_list;

namespace Local_nebula
{
FLAG_LIST(Local_Nebula_Flags) {

    FULL_NEBULA,
    NUMBER_VALUES
}

class L_nebula{
    SCP_string _name;
    float _radius { 0.0f };

	int _objnum { -1 };                 // objnum of this jump node
    int	_modelnum { -1 };
	int _polymodel_instance_num {-1};   // polymodel instance number, to allow transformations 

    flagset<Nebula_Flags> _flags;
    hdr_color _backgound_fog_color;			

public:
    //Constructors
    L_nebula();
    L_nebula(vec3d const &position);

	//Getting
    SCP_string GetName();
    int GetModelNumber();
    int GetSCPObjectNumber();
	int GetPolymodelInstanceNum(); 
    object *GetSCPObject();
    hdr_color GetColor();
 
    //Setting
    void SetBackgroundColor(int const r, int const g, int const b, int const alpha);
    void SetModel(const char *model_name);
    void SetName(SCP_string const new_name);
    
    //Rendering
	void Render(vec3d *pos, vec3d *view_pos = nullptr);
	void Render(model_draw_list *scene, vec3d *pos, vec3d *view_pos = nullptr);
}

} // Local_nebula namespace
