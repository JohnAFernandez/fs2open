#pragma once

#include "globalincs/globals.h"
#include "globalincs/pstypes.h"
#include "globalincs/flagset.h"

class object;

class model_draw_list;

namespace local_nebula
{
FLAG_LIST(Nebula_Flags) {
    
}

class l_nebula{
    SCP_string _name;
    float _radius { 0.0f };

    int	_modelnum { -1 };
	int _objnum { -1 };                 // objnum of this jump node
	int _polymodel_instance_num {-1}; // polymodel instance number, to allow transformations 

    flagset< m_flags {0};
    color m_display_color;			// Color node will be shown in (Default:0/255/0/255)
	vec3d m_pos;

	CJumpNode(const CJumpNode&);
	CJumpNode& operator=(const CJumpNode&);
public:
    //Constructors
    CJumpNode();
    CJumpNode(const vec3d *position);
	CJumpNode(CJumpNode&& other) noexcept;

	CJumpNode& operator=(CJumpNode&&) noexcept;
    
    //Destructor
    ~CJumpNode();
	
	//Getting
    const char *GetName();
    int GetModelNumber();
    int GetSCPObjectNumber();
	int GetPolymodelInstanceNum(); 
    object *GetSCPObject();
    color GetColor();
    vec3d *GetPosition();

    //Setting
    void SetAlphaColor(int r, int g, int b, int alpha);
    void SetModel(const char *model_name, bool show_polys=false);
    void SetName(const char *new_name);
    void SetVisibility(bool enabled);
    
    //Query
    bool IsHidden();
    bool IsColored();
    bool IsSpecialModel();

    //Rendering
	void Render(vec3d *pos, vec3d *view_pos = NULL);
	void Render(model_draw_list *scene, vec3d *pos, vec3d *view_pos = NULL);
}
} // end of namespace