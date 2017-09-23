#include "ogre_3d_view.hpp"

#include <OgreMeshManager.h>
#include <OgreTechnique.h>
#include <OgreManualObject.h>

#include "movable_text.h"

static Ogre::Entity* create_node(
    Ogre::SceneManager* mSceneMgr,
    Ogre::MaterialPtr mat,
    const position3& position,
    const size3& size,
    const std::string& name
) {
    Ogre::Entity* cube = mSceneMgr->createEntity("cube.mesh");
    cube->setMaterial(mat);
    const double mesh_size = cube->getBoundingRadius();

    Ogre::SceneNode* cube_node = mSceneMgr->getRootSceneNode()
        ->createChildSceneNode(
            Ogre::Vector3(
                position.x,
                position.y,
                position.z
            )
        );

    // TODO check osy
    cube_node->setScale(size.x / mesh_size, size.y / mesh_size, size.z / mesh_size);

    cube_node->attachObject( cube );

    Ogre::SceneNode* text_node = mSceneMgr->getRootSceneNode()
        ->createChildSceneNode(
            Ogre::Vector3(
                position.x,
                position.y,
                position.z + size.z / 2.0
            )
        );
    text_node->setScale(0.2, 0.2, 0.2);
    Ogre::MovableText* msg = new Ogre::MovableText( "TXT_001", name, "BlueHighway-16" );
    msg->setCharacterHeight(16);
    msg->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_CENTER); // Center horizontally and display above the node

    text_node->attachObject(msg);

    return cube;
}

static void create_arrow(
    Ogre::SceneManager* mSceneMgr,
    const std::string& name,
    const std::string& mat_name,
    const position3& begin,
    const position3& end
) {
    Ogre::ManualObject* myManualObject =  mSceneMgr->createManualObject(name);
    Ogre::SceneNode* myManualObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(name + "_node");

    // NOTE: The second parameter to the create method is the resource group the material will be added to.
    // If the group you name does not exist (in your resources.cfg file) the library will assert() and your program will crash

    myManualObject->begin(mat_name, Ogre::RenderOperation::OT_LINE_LIST);
    myManualObject->position(begin.x, begin.y, begin.z);
    myManualObject->position(end.x, end.y, end.z);
    myManualObject->end();

    myManualObjectNode->attachObject(myManualObject);
}

void Ogre_3D_View::createScene() {

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mCamera->setPosition(50, 50, 0);
}

void Ogre_3D_View::createCamera() {
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(Ogre::Vector3(1, 300, 1));
    mCamera->lookAt(Ogre::Vector3(0.1, 0.1, 0.1));
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(10000);
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void Ogre_3D_View::createViewports() {
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    /*
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight())
    );
    */
}

Ogre_3D_View::Ogre_3D_View() {
    BaseApplication::setup();
    Ogre_3D_View::createCamera();
    Ogre_3D_View::createViewports();
    Ogre_3D_View::createScene();

    mat_cube_1 =
        static_cast<Ogre::MaterialPtr>(
            Ogre::MaterialManager::getSingleton().getByName("transparent_with_rim_mat")
        )->clone("mat_cube_1");

    mat_cube_1->getTechnique(0)->setLightingEnabled(true);
    mat_cube_1->getTechnique(0)->getPass(0)->setDiffuse(0, 0, 0, 0.5);
    mat_cube_1->getTechnique(0)->getPass(0)->setAmbient(0, 0, 0);
    mat_cube_1->getTechnique(0)->getPass(0)->setSelfIllumination(1, 0, 0);

    mat_line_1 = Ogre::MaterialManager::getSingleton().create("mat_line_1","General");

    mat_line_1->setReceiveShadows(false);
    mat_line_1->getTechnique(0)->setLightingEnabled(true);
    mat_line_1->getTechnique(0)->getPass(0)->setDiffuse(1,1,1,1);
    mat_line_1->getTechnique(0)->getPass(0)->setAmbient(1,1,1);
    mat_line_1->getTechnique(0)->getPass(0)->setSelfIllumination(1,1,1);
}

void Ogre_3D_View::add_cube(const position3 position, const std::string& label) {
    create_node(
        mSceneMgr,
        mat_cube_1,
        position * 20, // TODO scaling issue
        size3(1, 1, 1),
        label
    );
}

void Ogre_3D_View::add_arrow(const position3 from, const position3 to) {
    create_arrow(
        mSceneMgr,
        "x" + std::to_string(from.x) + "-" + std::to_string(to.x), // TODO hack
        "mat_line_1",
        from * 20, // TODO scaling issue
        to * 20 // TODO scaling issue
    );
}

void Ogre_3D_View::run() {
    go();
}
