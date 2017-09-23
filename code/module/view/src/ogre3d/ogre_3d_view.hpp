#pragma once

#include "module/view/include/view_intf.hpp"
#include "module/view/src/ogre3d/BaseApplication.hpp"

class Ogre_3D_View : 
    public View_Intf,
    public BaseApplication 
{
    public:
        Ogre_3D_View();
        
        virtual void add_cube(const position3 position, const std::string& label) override;
        virtual void add_arrow(const position3 from, const position3 to) override;
        virtual void run() override;
    
    private:
        virtual void createScene() override;
        virtual void createCamera() override;
        virtual void createViewports() override;
        
        Ogre::MaterialPtr mat_cube_1;
        Ogre::MaterialPtr mat_line_1;
};
