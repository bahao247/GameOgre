/*
---------------------------------------------------------------------------------------
   ___                   _															   
  /___\__ _ _ __ ___    /_\  _ __  _ __
 //  // _` | '__/ _ \  //_\\| '_ \| '_ \											   
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \
\___/ \__, |_|  \___| \_/ \_/ .__/| .___/
      |___/                 |_|   |_|                                 
      Ogre 1.8.x Application for VC10
      https://plus.google.com/+bahaonguyen/
---------------------------------------------------------------------------------------
| Filename   : DotSceneLoader.hpp                                                           
---------------------------------------------------------------------------------------
| Written in : C++                                                     
---------------------------------------------------------------------------------------
| Compiler   : Microsoft Visual C++ 2010 Service Pack 1.                               
---------------------------------------------------------------------------------------
| Engine     : Ogre 1.8;...                                                                     
---------------------------------------------------------------------------------------
| Written by : Ba Hao Nguyen – Sports editors, IT.                                
---------------------------------------------------------------------------------------
| Note       : Write to LogManager...                                
---------------------------------------------------------------------------------------
| Create File: 14:00:00 4/7/2016 by TNA                                                             
---------------------------------------------------------------------------------------
| Last Update: 14:07:50 4/9/2016 by TNA                                                             
---------------------------------------------------------------------------------------
*/
#ifndef DOT_SCENELOADER_H
#define DOT_SCENELOADER_H
 
//PTR TuanNA- Includes... [14:08:06 4/9/2016 by TNA]
#include <OgreString.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreResourceGroupManager.h>
#include <vector>
 
#include "rapidxml.hpp"

/*
//PTR TuanNA begin comment
//Include & SceneLoader...[14:11:00 4/9/2016 by TNA]
#include <tinyxml.h>
#include "PhysicsManager.h"
#include "Ogre.h"
#include "OgrePrerequisites.h"
#include "OgreTerrain.h"...

class SceneLoader
{
private:
TiXmlDocument			*mvpXMLDoc;
Ogre::SceneManager		*mvpSceneMgr;
Ogre::SceneNode			*mvpAttachNode;
PhysicsManager			*mvpPhysicsMgr;
Ogre::String			sFilename;
Ogre::Terrain*			mvpTerrain;

bool parseNode(TiXmlElement *XMLNode);
bool parseScene(TiXmlElement *XMLRoot);
void stripStr(const Ogre::String& string2trip, Ogre::String &base,Ogre::String &path);

public:
SceneLoader() : mvpSceneMgr(0) {};
~SceneLoader();

bool parseLevel(const Ogre::String& filename,const Ogre::String &group, Ogre::SceneManager*,Ogre::SceneNode *attachNode, PhysicsManager *physicsMgr,Ogre::Terrain* terrain);

};
//PTR TuanNA end comment
*/
 
    //PTR TuanNA- Forward declarations... [14:08:11 4/9/2016 by TNA]
    namespace Ogre
    {
        class SceneManager;
        class SceneNode;
        class TerrainGroup;
        class TerrainGlobalOptions;
    }
 
    class nodeProperty
    {
    public:
        Ogre::String nodeName;
        Ogre::String propertyNm;
        Ogre::String valueName;
        Ogre::String typeName;
 
        nodeProperty(const Ogre::String &node, const Ogre::String &propertyName, const Ogre::String &value, const Ogre::String &type)
            : nodeName(node), propertyNm(propertyName), valueName(value), typeName(type) {}
    };
 
    class DotSceneLoader
    {
    public:
        Ogre::TerrainGlobalOptions *mTerrainGlobalOptions;
 
        DotSceneLoader();
        virtual ~DotSceneLoader();
 
        void parseDotScene(const Ogre::String &SceneName, const Ogre::String &groupName, Ogre::SceneManager *yourSceneMgr, Ogre::SceneNode *pAttachNode = NULL, const Ogre::String &sPrependNode = "");
        Ogre::String getProperty(const Ogre::String &ndNm, const Ogre::String &prop);
 
        Ogre::TerrainGroup* getTerrainGroup() { return mTerrainGroup; }
 
        std::vector<nodeProperty> nodeProperties;
        std::vector<Ogre::String> staticObjects;
        std::vector<Ogre::String> dynamicObjects;
 
    protected:
        void processScene(rapidxml::xml_node<>* XMLRoot);
 
        void processNodes(rapidxml::xml_node<>* XMLNode);
        void processExternals(rapidxml::xml_node<>* XMLNode);
        void processEnvironment(rapidxml::xml_node<>* XMLNode);
        void processTerrain(rapidxml::xml_node<>* XMLNode);
        void processTerrainPage(rapidxml::xml_node<>* XMLNode);
        void processBlendmaps(rapidxml::xml_node<>* XMLNode);
        void processUserDataReference(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
        void processUserDataReference(rapidxml::xml_node<>* XMLNode, Ogre::Entity *pEntity);
        void processOctree(rapidxml::xml_node<>* XMLNode);
        void processLight(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
        void processCamera(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
 
        void processNode(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
        void processLookTarget(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
        void processTrackTarget(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
        void processEntity(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
        void processParticleSystem(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
        void processBillboardSet(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
        void processPlane(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
 
        void processFog(rapidxml::xml_node<>* XMLNode);
        void processSkyBox(rapidxml::xml_node<>* XMLNode);
        void processSkyDome(rapidxml::xml_node<>* XMLNode);
        void processSkyPlane(rapidxml::xml_node<>* XMLNode);
        void processClipping(rapidxml::xml_node<>* XMLNode);
 
        void processLightRange(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight);
        void processLightAttenuation(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight);
 
        Ogre::String getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, const Ogre::String &defaultValue = "");
        Ogre::Real getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, Ogre::Real defaultValue = 0);
        bool getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, bool defaultValue = false);
 
        Ogre::Vector3 parseVector3(rapidxml::xml_node<>* XMLNode);
        Ogre::Quaternion parseQuaternion(rapidxml::xml_node<>* XMLNode);
        Ogre::ColourValue parseColour(rapidxml::xml_node<>* XMLNode);
 
 
        Ogre::SceneManager *mSceneMgr;
        Ogre::SceneNode *mAttachNode;
        Ogre::String m_sGroupName;
        Ogre::String m_sPrependNode;
        Ogre::TerrainGroup* mTerrainGroup;
        Ogre::Vector3 mTerrainPosition;
        Ogre::Vector3 mLightDirection;
    };
 
#endif // DOT_SCENELOADER_H