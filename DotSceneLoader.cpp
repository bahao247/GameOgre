#include "DotSceneLoader.hpp"
#include <Ogre.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <Terrain/OgreTerrainMaterialGeneratorA.h>
 
#pragma warning(disable:4390)
#pragma warning(disable:4305)
 
 
DotSceneLoader::DotSceneLoader() : mSceneMgr(0), mTerrainGroup(0) 
{
    mTerrainGlobalOptions = OGRE_NEW Ogre::TerrainGlobalOptions();
}
 
 
DotSceneLoader::~DotSceneLoader()
{
    if(mTerrainGroup)
    {
        OGRE_DELETE mTerrainGroup;
    }
 
    OGRE_DELETE mTerrainGlobalOptions;
}
 
void DotSceneLoader::parseDotScene(const Ogre::String &SceneName, const Ogre::String &groupName, Ogre::SceneManager *yourSceneMgr, Ogre::SceneNode *pAttachNode, const Ogre::String &sPrependNode)
{
    //PTR TuanNA- set up shared object values... [21:28:09 4/9/2016 by TNA]
    m_sGroupName = groupName;
    mSceneMgr = yourSceneMgr;
    m_sPrependNode = sPrependNode;
    staticObjects.clear();
    dynamicObjects.clear();
 
    rapidxml::xml_document<> XMLDoc;    //PTR TuanNA- character type defaults to char... [21:28:21 4/9/2016 by TNA]
 
    rapidxml::xml_node<>* XMLRoot;
 
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(SceneName, groupName );
    char* scene = strdup(stream->getAsString().c_str());
    XMLDoc.parse<0>(scene);
 
    //PTR TuanNA- Grab the scene node... [21:28:31 4/9/2016 by TNA]
    XMLRoot = XMLDoc.first_node("scene");
 
    //PTR TuanNA- Validate the File... [21:32:00 4/9/2016 by TNA]
    if( getAttrib(XMLRoot, "formatVersion", "") == "")
    {
        Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Error: Invalid .scene File. Missing <scene>" );
        delete scene;
        return;
    }
 
    //PTR TuanNA- Figure out where to attach any nodes we create... [21:32:35 4/9/2016 by TNA]
    mAttachNode = pAttachNode;
    if(!mAttachNode)
        mAttachNode = mSceneMgr->getRootSceneNode();
 
    //PTR TuanNA- Process the scene... [21:32:50 4/9/2016 by TNA]
    processScene(XMLRoot);
 
    delete scene;
}
 
void DotSceneLoader::processScene(rapidxml::xml_node<>* XMLRoot)
{
    //PTR TuanNA- Process the scene parameters... [21:33:41 4/9/2016 by TNA]
    Ogre::String version = getAttrib(XMLRoot, "formatVersion", "unknown");
 
    Ogre::String message = "[DotSceneLoader] Parsing dotScene file with version " + version;
    if(XMLRoot->first_attribute("ID"))
        message += ", id " + Ogre::String(XMLRoot->first_attribute("ID")->value());
    if(XMLRoot->first_attribute("sceneManager"))
        message += ", scene manager " + Ogre::String(XMLRoot->first_attribute("sceneManager")->value());
    if(XMLRoot->first_attribute("minOgreVersion"))
        message += ", min. Ogre version " + Ogre::String(XMLRoot->first_attribute("minOgreVersion")->value());
    if(XMLRoot->first_attribute("author"))
        message += ", author " + Ogre::String(XMLRoot->first_attribute("author")->value());
 
    Ogre::LogManager::getSingleton().logMessage(message);
 
    rapidxml::xml_node<>* pElement;
 
    //PTR TuanNA- Process environment (?)... [21:35:33 4/9/2016 by TNA]
    pElement = XMLRoot->first_node("environment");
    if(pElement)
        processEnvironment(pElement);
 
    //PTR TuanNA- Process nodes (?)... [21:35:42 4/9/2016 by TNA]
    pElement = XMLRoot->first_node("nodes");
    if(pElement)
        processNodes(pElement);
 
    //PTR TuanNA- Process externals (?)... [21:35:53 4/9/2016 by TNA]
    pElement = XMLRoot->first_node("externals");
    if(pElement)
        processExternals(pElement);
 
    //PTR TuanNA- Process userDataReference (?)... [21:35:59 4/9/2016 by TNA]
    pElement = XMLRoot->first_node("userDataReference");
    if(pElement)
        processUserDataReference(pElement);
 
    //PTR TuanNA- Process octree (?)... [21:36:06 4/9/2016 by TNA]
    pElement = XMLRoot->first_node("octree");
    if(pElement)
        processOctree(pElement);
 
    //PTR TuanNA- Process light (?)... [21:36:13 4/9/2016 by TNA]
    //pElement = XMLRoot->first_node("light");
    //if(pElement)
    //    processLight(pElement);
 
    //PTR TuanNA- Process camera (?)... [21:36:24 4/9/2016 by TNA]
    pElement = XMLRoot->first_node("camera");
    if(pElement)
        processCamera(pElement);
 
    //PTR TuanNA- Process terrain (?)... [21:36:32 4/9/2016 by TNA]
    pElement = XMLRoot->first_node("terrain");
    if(pElement)
        processTerrain(pElement);
}
 
void DotSceneLoader::processNodes(rapidxml::xml_node<>* XMLNode)
{
    rapidxml::xml_node<>* pElement;
 
    //PTR TuanNA- Process node (*)... [21:36:41 4/9/2016 by TNA]
    pElement = XMLNode->first_node("node");
    while(pElement)
    {
        processNode(pElement);
        pElement = pElement->next_sibling("node");
    }
 
    //PTR TuanNA- Process position (?)... [21:36:46 4/9/2016 by TNA]
    pElement = XMLNode->first_node("position");
    if(pElement)
    {
        mAttachNode->setPosition(parseVector3(pElement));
        mAttachNode->setInitialState();
    }
 
    //PTR TuanNA- Process rotation (?)... [21:37:02 4/9/2016 by TNA]
    pElement = XMLNode->first_node("rotation");
    if(pElement)
    {
        mAttachNode->setOrientation(parseQuaternion(pElement));
        mAttachNode->setInitialState();
    }
 
    //PTR TuanNA- Process scale (?)... [21:37:07 4/9/2016 by TNA]
    pElement = XMLNode->first_node("scale");
    if(pElement)
    {
        mAttachNode->setScale(parseVector3(pElement));
        mAttachNode->setInitialState();
    }
}
 
void DotSceneLoader::processExternals(rapidxml::xml_node<>* XMLNode)
{
    //! @todo Implement this
}
 
void DotSceneLoader::processEnvironment(rapidxml::xml_node<>* XMLNode)
{
    rapidxml::xml_node<>* pElement;
 
    //PTR TuanNA- Process camera (?)... [21:37:27 4/9/2016 by TNA]
    pElement = XMLNode->first_node("camera");
    if(pElement)
        processCamera(pElement);
 
    //PTR TuanNA- Process fog (?)... [21:37:31 4/9/2016 by TNA]
    pElement = XMLNode->first_node("fog");
    if(pElement)
        processFog(pElement);
 
    //PTR TuanNA- Process skyBox (?)... [21:37:36 4/9/2016 by TNA]
    pElement = XMLNode->first_node("skyBox");
    if(pElement)
        processSkyBox(pElement);
 
    //PTR TuanNA- Process skyDome (?)... [21:37:45 4/9/2016 by TNA]
    pElement = XMLNode->first_node("skyDome");
    if(pElement)
        processSkyDome(pElement);
 
    //PTR TuanNA- Process skyPlane (?)... [21:37:50 4/9/2016 by TNA]
    pElement = XMLNode->first_node("skyPlane");
    if(pElement)
        processSkyPlane(pElement);
 
    //PTR TuanNA- Process clipping (?)... [21:37:55 4/9/2016 by TNA]
    pElement = XMLNode->first_node("clipping");
    if(pElement)
        processClipping(pElement);
 
    //PTR TuanNA- Process colourAmbient (?)... [21:38:02 4/9/2016 by TNA]
    pElement = XMLNode->first_node("colourAmbient");
    if(pElement)
        mSceneMgr->setAmbientLight(parseColour(pElement));
 
    //PTR TuanNA- Process colourBackground (?)... [21:38:07 4/9/2016 by TNA]
    //! @todo Set the background colour of all viewports (RenderWindow has to be provided then)
    pElement = XMLNode->first_node("colourBackground");
    if(pElement)
        ;//mSceneMgr->set(parseColour(pElement));
 
    //PTR TuanNA- Process userDataReference (?)... [21:38:26 4/9/2016 by TNA]
    pElement = XMLNode->first_node("userDataReference");
    if(pElement)
        processUserDataReference(pElement);
}
 
void DotSceneLoader::processTerrain(rapidxml::xml_node<>* XMLNode)
{
    Ogre::Real worldSize = getAttribReal(XMLNode, "worldSize");
    int mapSize = Ogre::StringConverter::parseInt(XMLNode->first_attribute("mapSize")->value());
    bool colourmapEnabled = getAttribBool(XMLNode, "colourmapEnabled");
    int colourMapTextureSize = Ogre::StringConverter::parseInt(XMLNode->first_attribute("colourMapTextureSize")->value());
    int compositeMapDistance = Ogre::StringConverter::parseInt(XMLNode->first_attribute("tuningCompositeMapDistance")->value());
    int maxPixelError = Ogre::StringConverter::parseInt(XMLNode->first_attribute("tuningMaxPixelError")->value());
 
    Ogre::Vector3 lightdir(0, -0.3, 0.75);
    lightdir.normalise();
    Ogre::Light* l = mSceneMgr->createLight("tstLight");
    l->setType(Ogre::Light::LT_DIRECTIONAL);
    l->setDirection(lightdir);
    l->setDiffuseColour(Ogre::ColourValue(1.0, 1.0, 1.0));
    l->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));
 
    mTerrainGlobalOptions->setMaxPixelError((Ogre::Real)maxPixelError);
    mTerrainGlobalOptions->setCompositeMapDistance((Ogre::Real)compositeMapDistance);
    mTerrainGlobalOptions->setLightMapDirection(lightdir);
    mTerrainGlobalOptions->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
    mTerrainGlobalOptions->setCompositeMapDiffuse(l->getDiffuseColour());
 
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, mapSize, worldSize);
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
 
    mTerrainGroup->setResourceGroup("General");
 
    rapidxml::xml_node<>* pElement;
    rapidxml::xml_node<>* pPageElement;
 
    //PTR TuanNA- Process terrain pages (*)... [21:38:49 4/9/2016 by TNA]
    pElement = XMLNode->first_node("terrainPages");
    if(pElement)
    {
        pPageElement = pElement->first_node("terrainPage");
        while(pPageElement)
        {
            processTerrainPage(pPageElement);
            pPageElement = pPageElement->next_sibling("terrainPage");
        }
    }
    mTerrainGroup->loadAllTerrains(true);
 
    mTerrainGroup->freeTemporaryResources();
    //PTR TuanNA- Remove setPosition()... [21:38:55 4/9/2016 by TNA]mTerrain->setPosition(mTerrainPosition);
}
 
void DotSceneLoader::processTerrainPage(rapidxml::xml_node<>* XMLNode)
{
    Ogre::String name = getAttrib(XMLNode, "name");
    int pageX = Ogre::StringConverter::parseInt(XMLNode->first_attribute("pageX")->value());
    int pageY = Ogre::StringConverter::parseInt(XMLNode->first_attribute("pageY")->value());
 
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), name))
    {
        mTerrainGroup->defineTerrain(pageX, pageY, name);
    }
}
 
void DotSceneLoader::processUserDataReference(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
    //! @todo Implement this
}
 
void DotSceneLoader::processOctree(rapidxml::xml_node<>* XMLNode)
{
    //! @todo Implement this
}
 
void DotSceneLoader::processLight(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
    //PTR TuanNA- Process attributes... [21:39:34 4/9/2016 by TNA]
    Ogre::String name = getAttrib(XMLNode, "name");
    Ogre::String id = getAttrib(XMLNode, "id");
 
    //PTR TuanNA- Create the light... [21:39:52 4/9/2016 by TNA]
    Ogre::Light *pLight = mSceneMgr->createLight(name);
    if(pParent)
        pParent->attachObject(pLight);
 
    Ogre::String sValue = getAttrib(XMLNode, "type");
    if(sValue == "point")
        pLight->setType(Ogre::Light::LT_POINT);
    else if(sValue == "directional")
        pLight->setType(Ogre::Light::LT_DIRECTIONAL);
    else if(sValue == "spot")
        pLight->setType(Ogre::Light::LT_SPOTLIGHT);
    else if(sValue == "radPoint")
        pLight->setType(Ogre::Light::LT_POINT);
 
    pLight->setVisible(getAttribBool(XMLNode, "visible", true));
    pLight->setCastShadows(getAttribBool(XMLNode, "castShadows", true));
 
    rapidxml::xml_node<>* pElement;
 
    //PTR TuanNA- Process position (?)... [21:41:32 4/9/2016 by TNA]
    pElement = XMLNode->first_node("position");
    if(pElement)
        pLight->setPosition(parseVector3(pElement));
 
    //PTR TuanNA- Process normal (?)... [21:41:38 4/9/2016 by TNA]
    pElement = XMLNode->first_node("normal");
    if(pElement)
        pLight->setDirection(parseVector3(pElement));
 
    pElement = XMLNode->first_node("directionVector");
    if(pElement)
    {
        pLight->setDirection(parseVector3(pElement));
        mLightDirection = parseVector3(pElement);
    }
 
    //PTR TuanNA- Process colourDiffuse (?)... [21:41:49 4/9/2016 by TNA]
    pElement = XMLNode->first_node("colourDiffuse");
    if(pElement)
        pLight->setDiffuseColour(parseColour(pElement));
 
    //PTR TuanNA- Process colourSpecular (?)... [21:41:56 4/9/2016 by TNA]
    pElement = XMLNode->first_node("colourSpecular");
    if(pElement)
        pLight->setSpecularColour(parseColour(pElement));
 
    if(sValue != "directional")
    {
        //PTR TuanNA- Process lightRange (?)... [21:42:16 4/9/2016 by TNA]
        pElement = XMLNode->first_node("lightRange");
        if(pElement)
            processLightRange(pElement, pLight);
 
        // Process lightAttenuation (?)
        pElement = XMLNode->first_node("lightAttenuation");
        if(pElement)
            processLightAttenuation(pElement, pLight);
    }
    // Process userDataReference (?)
    pElement = XMLNode->first_node("userDataReference");
    if(pElement)
        ;//processUserDataReference(pElement, pLight);
}
 
void DotSceneLoader::processCamera(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
    // Process attributes
    Ogre::String name = getAttrib(XMLNode, "name");
    Ogre::String id = getAttrib(XMLNode, "id");
    Ogre::Real fov = getAttribReal(XMLNode, "fov", 45);
    Ogre::Real aspectRatio = getAttribReal(XMLNode, "aspectRatio", 1.3333);
    Ogre::String projectionType = getAttrib(XMLNode, "projectionType", "perspective");
 
    // Create the camera
    Ogre::Camera *pCamera = mSceneMgr->createCamera(name);
 
    //TODO: make a flag or attribute indicating whether or not the camera should be attached to any parent node.
    //if(pParent)
    //    pParent->attachObject(pCamera);
 
    // Set the field-of-view
    //! @todo Is this always in degrees?
    //pCamera->setFOVy(Ogre::Degree(fov));
 
    // Set the aspect ratio
    //pCamera->setAspectRatio(aspectRatio);
 
    //PTR TuanNA- Set the projection type... [21:48:38 4/9/2016 by TNA]
    if(projectionType == "perspective")
        pCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
    else if(projectionType == "orthographic")
        pCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
 
    rapidxml::xml_node<>* pElement;
 
    //PTR TuanNA- Process clipping (?)... [21:49:38 4/9/2016 by TNA]
    pElement = XMLNode->first_node("clipping");
    if(pElement)
    {
        Ogre::Real nearDist = getAttribReal(pElement, "near");
        pCamera->setNearClipDistance(nearDist);
 
        Ogre::Real farDist =  getAttribReal(pElement, "far");
        pCamera->setFarClipDistance(farDist);
    }
 
    //PTR TuanNA- Process position (?)... [21:50:24 4/9/2016 by TNA]
    pElement = XMLNode->first_node("position");
    if(pElement)
        pCamera->setPosition(parseVector3(pElement));
 
    //PTR TuanNA- Process rotation (?)... [21:50:30 4/9/2016 by TNA]
    pElement = XMLNode->first_node("rotation");
    if(pElement)
        pCamera->setOrientation(parseQuaternion(pElement));
 
    //PTR TuanNA- Process normal (?)... [21:50:36 4/9/2016 by TNA]
    pElement = XMLNode->first_node("normal");
    if(pElement)
        ;//!< @todo What to do with this element?
 
    // Process lookTarget (?)
    pElement = XMLNode->first_node("lookTarget");
    if(pElement)
        ;//!< @todo Implement the camera look target
 
    // Process trackTarget (?)
    pElement = XMLNode->first_node("trackTarget");
    if(pElement)
        ;//!< @todo Implement the camera track target
 
    // Process userDataReference (?)
    pElement = XMLNode->first_node("userDataReference");
    if(pElement)
        ;//!< @todo Implement the camera user data reference
 
    //PTR TuanNA- Construct a scenenode is no parent... [21:51:39 4/9/2016 by TNA]
    if(!pParent)
    {
        Ogre::SceneNode* pNode = mAttachNode->createChildSceneNode(name);
        pNode->setPosition(pCamera->getPosition());
        pNode->setOrientation(pCamera->getOrientation());
        pNode->scale(1,1,1);
    }
}
 
void DotSceneLoader::processNode(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
    //PTR TuanNA- Construct the node's name... [21:52:08 4/9/2016 by TNA]
    Ogre::String name = m_sPrependNode + getAttrib(XMLNode, "name");
 
    //PTR TuanNA- Create the scene node... [21:52:12 4/9/2016 by TNA]
    Ogre::SceneNode *pNode;
    if(name.empty())
    {
        //PTR TuanNA- Let Ogre choose the name... [21:52:19 4/9/2016 by TNA]
        if(pParent)
            pNode = pParent->createChildSceneNode();
        else
            pNode = mAttachNode->createChildSceneNode();
    }
    else
    {
        //PTR TuanNA- Provide the name... [21:52:28 4/9/2016 by TNA]
        if(pParent)
            pNode = pParent->createChildSceneNode(name);
        else
            pNode = mAttachNode->createChildSceneNode(name);
    }
 
    //PTR TuanNA- Process other attributes... [21:52:44 4/9/2016 by TNA]
    Ogre::String id = getAttrib(XMLNode, "id");
    bool isTarget = getAttribBool(XMLNode, "isTarget");
 
    rapidxml::xml_node<>* pElement;
 
    //PTR TuanNA- Process position (?)... [21:52:50 4/9/2016 by TNA]
    pElement = XMLNode->first_node("position");
    if(pElement)
    {
        pNode->setPosition(parseVector3(pElement));
        pNode->setInitialState();
    }
 
    //PTR TuanNA- Process rotation (?)... [21:52:55 4/9/2016 by TNA]
    pElement = XMLNode->first_node("rotation");
    if(pElement)
    {
        pNode->setOrientation(parseQuaternion(pElement));
        pNode->setInitialState();
    }
 
    //PTR TuanNA- Process scale (?)... [21:53:00 4/9/2016 by TNA]
    pElement = XMLNode->first_node("scale");
    if(pElement)
    {
        pNode->setScale(parseVector3(pElement));
        pNode->setInitialState();
    }
 
    //PTR TuanNA- Process lookTarget (?)... [21:53:07 4/9/2016 by TNA]
    pElement = XMLNode->first_node("lookTarget");
    if(pElement)
        processLookTarget(pElement, pNode);
 
    //PTR TuanNA- Process trackTarget (?)... [21:53:13 4/9/2016 by TNA]
    pElement = XMLNode->first_node("trackTarget");
    if(pElement)
        processTrackTarget(pElement, pNode);
 
    //PTR TuanNA- Process node (*)... [21:53:17 4/9/2016 by TNA]
    pElement = XMLNode->first_node("node");
    while(pElement)
    {
        processNode(pElement, pNode);
        pElement = pElement->next_sibling("node");
    }
 
    //PTR TuanNA- Process entity (*)... [21:53:25 4/9/2016 by TNA]
    pElement = XMLNode->first_node("entity");
    while(pElement)
    {
        processEntity(pElement, pNode);
        pElement = pElement->next_sibling("entity");
    }
 
    // Process light (*)
    //pElement = XMLNode->first_node("light");
    //while(pElement)
    //{
    //    processLight(pElement, pNode);
    //    pElement = pElement->next_sibling("light");
    //}
 
    //PTR TuanNA- Process camera (*)... [21:53:32 4/9/2016 by TNA]
    pElement = XMLNode->first_node("camera");
    while(pElement)
    {
        processCamera(pElement, pNode);
        pElement = pElement->next_sibling("camera");
    }
 
    //PTR TuanNA- Process particleSystem (*)... [21:53:38 4/9/2016 by TNA]
    pElement = XMLNode->first_node("particleSystem");
    while(pElement)
    {
        processParticleSystem(pElement, pNode);
        pElement = pElement->next_sibling("particleSystem");
    }
 
    //PTR TuanNA- Process billboardSet (*)... [21:53:45 4/9/2016 by TNA]
    pElement = XMLNode->first_node("billboardSet");
    while(pElement)
    {
        processBillboardSet(pElement, pNode);
        pElement = pElement->next_sibling("billboardSet");
    }
 
    //PTR TuanNA- Process plane (*)... [21:53:50 4/9/2016 by TNA]
    pElement = XMLNode->first_node("plane");
    while(pElement)
    {
        processPlane(pElement, pNode);
        pElement = pElement->next_sibling("plane");
    }
 
    //PTR TuanNA- Process userDataReference (?)... [21:53:56 4/9/2016 by TNA]
    pElement = XMLNode->first_node("userDataReference");
    if(pElement)
        processUserDataReference(pElement, pNode);
}
 
void DotSceneLoader::processLookTarget(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
    //! @todo Is this correct? Cause I don't have a clue actually
 
    // Process attributes
    Ogre::String nodeName = getAttrib(XMLNode, "nodeName");
 
    Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT;
    Ogre::String sValue = getAttrib(XMLNode, "relativeTo");
    if(sValue == "local")
        relativeTo = Ogre::Node::TS_LOCAL;
    else if(sValue == "parent")
        relativeTo = Ogre::Node::TS_PARENT;
    else if(sValue == "world")
        relativeTo = Ogre::Node::TS_WORLD;
 
    rapidxml::xml_node<>* pElement;
 
    //PTR TuanNA- Process position (?)... [21:54:04 4/9/2016 by TNA]
    Ogre::Vector3 position;
    pElement = XMLNode->first_node("position");
    if(pElement)
        position = parseVector3(pElement);
 
    //PTR TuanNA- Process localDirection (?)... [21:54:08 4/9/2016 by TNA]
    Ogre::Vector3 localDirection = Ogre::Vector3::NEGATIVE_UNIT_Z;
    pElement = XMLNode->first_node("localDirection");
    if(pElement)
        localDirection = parseVector3(pElement);
 
    //PTR TuanNA- Setup the look target... [21:54:12 4/9/2016 by TNA]
    try
    {
        if(!nodeName.empty())
        {
            Ogre::SceneNode *pLookNode = mSceneMgr->getSceneNode(nodeName);
            position = pLookNode->_getDerivedPosition();
        }
 
        pParent->lookAt(position, relativeTo, localDirection);
    }
    catch(Ogre::Exception &/*e*/)
    {
        Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error processing a look target!");
    }
}
 
void DotSceneLoader::processTrackTarget(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
    //PTR TuanNA- Process attributes... [21:54:18 4/9/2016 by TNA]
    Ogre::String nodeName = getAttrib(XMLNode, "nodeName");
 
    rapidxml::xml_node<>* pElement;
 
    //PTR TuanNA- Process localDirection (?)... [21:54:26 4/9/2016 by TNA]
    Ogre::Vector3 localDirection = Ogre::Vector3::NEGATIVE_UNIT_Z;
    pElement = XMLNode->first_node("localDirection");
    if(pElement)
        localDirection = parseVector3(pElement);
 
    //PTR TuanNA- Process offset (?)... [21:54:31 4/9/2016 by TNA]
    Ogre::Vector3 offset = Ogre::Vector3::ZERO;
    pElement = XMLNode->first_node("offset");
    if(pElement)
        offset = parseVector3(pElement);
 
    //PTR TuanNA- Setup the track target... [21:54:36 4/9/2016 by TNA]
    try
    {
        Ogre::SceneNode *pTrackNode = mSceneMgr->getSceneNode(nodeName);
        pParent->setAutoTracking(true, pTrackNode, localDirection, offset);
    }
    catch(Ogre::Exception &/*e*/)
    {
        Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error processing a track target!");
    }
}
 
void DotSceneLoader::processEntity(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
    //PTR TuanNA- Process attributes... [21:54:41 4/9/2016 by TNA]
    Ogre::String name = getAttrib(XMLNode, "name");
    Ogre::String id = getAttrib(XMLNode, "id");
    Ogre::String meshFile = getAttrib(XMLNode, "meshFile");
    Ogre::String materialFile = getAttrib(XMLNode, "materialFile");
    bool isStatic = getAttribBool(XMLNode, "static", false);;
    bool castShadows = getAttribBool(XMLNode, "castShadows", true);
 
    //PTR TuanNA- TEMP: Maintain a list of static and dynamic objects... [21:54:46 4/9/2016 by TNA]
    if(isStatic)
        staticObjects.push_back(name);
    else
        dynamicObjects.push_back(name);
 
    rapidxml::xml_node<>* pElement;
 
    //PTR TuanNA- Process vertexBuffer (?)... [21:54:51 4/9/2016 by TNA]
    pElement = XMLNode->first_node("vertexBuffer");
    if(pElement)
        ;//processVertexBuffer(pElement);
 
    //PTR TuanNA- Process indexBuffer (?)... [21:55:00 4/9/2016 by TNA]
    pElement = XMLNode->first_node("indexBuffer");
    if(pElement)
        ;//processIndexBuffer(pElement);
 
    //PTR TuanNA- Create the entity... [21:55:08 4/9/2016 by TNA]
    Ogre::Entity *pEntity = 0;
    try
    {
        Ogre::MeshManager::getSingleton().load(meshFile, m_sGroupName);
        pEntity = mSceneMgr->createEntity(name, meshFile);
        pEntity->setCastShadows(castShadows);
        pParent->attachObject(pEntity);
 
        if(!materialFile.empty())
            pEntity->setMaterialName(materialFile);
    }
    catch(Ogre::Exception &/*e*/)
    {
        Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error loading an entity!");
    }
 
    //PTR TuanNA- Process userDataReference (?)... [21:55:14 4/9/2016 by TNA]
    pElement = XMLNode->first_node("userDataReference");
    if(pElement)
        processUserDataReference(pElement, pEntity);
 
 
}
 
void DotSceneLoader::processParticleSystem(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
    //PTR TuanNA- Process attributes... [21:55:23 4/9/2016 by TNA]
    Ogre::String name = getAttrib(XMLNode, "name");
    Ogre::String id = getAttrib(XMLNode, "id");
    Ogre::String file = getAttrib(XMLNode, "file");
 
    //PTR TuanNA- Create the particle system... [21:55:28 4/9/2016 by TNA]
    try
    {
        Ogre::ParticleSystem *pParticles = mSceneMgr->createParticleSystem(name, file);
        pParent->attachObject(pParticles);
    }
    catch(Ogre::Exception &/*e*/)
    {
        Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error creating a particle system!");
    }
}
 
void DotSceneLoader::processBillboardSet(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
    //! @todo Implement this
}
 
void DotSceneLoader::processPlane(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
    Ogre::String name = getAttrib(XMLNode, "name");
    Ogre::Real distance = getAttribReal(XMLNode, "distance");
    Ogre::Real width = getAttribReal(XMLNode, "width");
    Ogre::Real height = getAttribReal(XMLNode, "height");
    int xSegments = Ogre::StringConverter::parseInt(getAttrib(XMLNode, "xSegments"));
    int ySegments = Ogre::StringConverter::parseInt(getAttrib(XMLNode, "ySegments"));
    int numTexCoordSets = Ogre::StringConverter::parseInt(getAttrib(XMLNode, "numTexCoordSets"));
    Ogre::Real uTile = getAttribReal(XMLNode, "uTile");
    Ogre::Real vTile = getAttribReal(XMLNode, "vTile");
    Ogre::String material = getAttrib(XMLNode, "material");
    bool hasNormals = getAttribBool(XMLNode, "hasNormals");
    Ogre::Vector3 normal = parseVector3(XMLNode->first_node("normal"));
    Ogre::Vector3 up = parseVector3(XMLNode->first_node("upVector"));
 
    Ogre::Plane plane(normal, distance);
    Ogre::MeshPtr res = Ogre::MeshManager::getSingletonPtr()->createPlane(
                        name + "mesh", "General", plane, width, height, xSegments, ySegments, hasNormals,
    numTexCoordSets, uTile, vTile, up);
    Ogre::Entity* ent = mSceneMgr->createEntity(name, name + "mesh");
 
    ent->setMaterialName(material);
 
    pParent->attachObject(ent);
}
 
void DotSceneLoader::processFog(rapidxml::xml_node<>* XMLNode)
{
    //PTR TuanNA- Process attributes... [21:55:35 4/9/2016 by TNA]
    Ogre::Real expDensity = getAttribReal(XMLNode, "density", 0.001);
    Ogre::Real linearStart = getAttribReal(XMLNode, "start", 0.0);
    Ogre::Real linearEnd = getAttribReal(XMLNode, "end", 1.0);
 
    Ogre::FogMode mode = Ogre::FOG_NONE;
    Ogre::String sMode = getAttrib(XMLNode, "mode");
    if(sMode == "none")
        mode = Ogre::FOG_NONE;
    else if(sMode == "exp")
        mode = Ogre::FOG_EXP;
    else if(sMode == "exp2")
        mode = Ogre::FOG_EXP2;
    else if(sMode == "linear")
        mode = Ogre::FOG_LINEAR;
    else
        mode = (Ogre::FogMode)Ogre::StringConverter::parseInt(sMode);
 
    rapidxml::xml_node<>* pElement;
 
    //PTR TuanNA- Process colourDiffuse (?)... [21:55:40 4/9/2016 by TNA]
    Ogre::ColourValue colourDiffuse = Ogre::ColourValue::White;
    pElement = XMLNode->first_node("colour");
    if(pElement)
        colourDiffuse = parseColour(pElement);
 
    //PTR TuanNA- Setup the fog... [21:55:45 4/9/2016 by TNA]
    mSceneMgr->setFog(mode, colourDiffuse, expDensity, linearStart, linearEnd);
}
 
void DotSceneLoader::processSkyBox(rapidxml::xml_node<>* XMLNode)
{
    //PTR TuanNA- Process attributes... [21:55:50 4/9/2016 by TNA]
    Ogre::String material = getAttrib(XMLNode, "material", "BaseWhite");
    Ogre::Real distance = getAttribReal(XMLNode, "distance", 5000);
    bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);
    bool active = getAttribBool(XMLNode, "active", false);
    if(!active)
        return;
 
    rapidxml::xml_node<>* pElement;
 
    //PTR TuanNA- Process rotation (?)... [21:55:54 4/9/2016 by TNA]
    Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
    pElement = XMLNode->first_node("rotation");
    if(pElement)
        rotation = parseQuaternion(pElement);
 
    //PTR TuanNA- Setup the sky box... [21:55:59 4/9/2016 by TNA]
    mSceneMgr->setSkyBox(true, material, distance, drawFirst, rotation, m_sGroupName);
}
 
void DotSceneLoader::processSkyDome(rapidxml::xml_node<>* XMLNode)
{
    //PTR TuanNA- Process attributes... [21:56:03 4/9/2016 by TNA]
    Ogre::String material = XMLNode->first_attribute("material")->value();
    Ogre::Real curvature = getAttribReal(XMLNode, "curvature", 10);
    Ogre::Real tiling = getAttribReal(XMLNode, "tiling", 8);
    Ogre::Real distance = getAttribReal(XMLNode, "distance", 4000);
    bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);
    bool active = getAttribBool(XMLNode, "active", false);
    if(!active)
        return;
 
    rapidxml::xml_node<>* pElement;
 
    //PTR TuanNA- Process rotation (?)... [21:56:10 4/9/2016 by TNA]
    Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
    pElement = XMLNode->first_node("rotation");
    if(pElement)
        rotation = parseQuaternion(pElement);
 
    //PTR TuanNA- Setup the sky dome... [21:56:14 4/9/2016 by TNA]
    mSceneMgr->setSkyDome(true, material, curvature, tiling, distance, drawFirst, rotation, 16, 16, -1, m_sGroupName);
}
 
void DotSceneLoader::processSkyPlane(rapidxml::xml_node<>* XMLNode)
{
    //PTR TuanNA- Process attributes... [21:56:20 4/9/2016 by TNA]
    Ogre::String material = getAttrib(XMLNode, "material");
    Ogre::Real planeX = getAttribReal(XMLNode, "planeX", 0);
    Ogre::Real planeY = getAttribReal(XMLNode, "planeY", -1);
    Ogre::Real planeZ = getAttribReal(XMLNode, "planeX", 0);
    Ogre::Real planeD = getAttribReal(XMLNode, "planeD", 5000);
    Ogre::Real scale = getAttribReal(XMLNode, "scale", 1000);
    Ogre::Real bow = getAttribReal(XMLNode, "bow", 0);
    Ogre::Real tiling = getAttribReal(XMLNode, "tiling", 10);
    bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);
 
    //PTR TuanNA- Setup the sky plane... [21:56:24 4/9/2016 by TNA]
    Ogre::Plane plane;
    plane.normal = Ogre::Vector3(planeX, planeY, planeZ);
    plane.d = planeD;
    mSceneMgr->setSkyPlane(true, plane, material, scale, tiling, drawFirst, bow, 1, 1, m_sGroupName);
}
 
void DotSceneLoader::processClipping(rapidxml::xml_node<>* XMLNode)
{
    //! @todo Implement this
 
    //PTR TuanNA- Process attributes... [21:56:31 4/9/2016 by TNA]
    Ogre::Real fNear = getAttribReal(XMLNode, "near", 0);
    Ogre::Real fFar = getAttribReal(XMLNode, "far", 1);
}
 
void DotSceneLoader::processLightRange(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight)
{
    //PTR TuanNA- Process attributes... [21:56:40 4/9/2016 by TNA]
    Ogre::Real inner = getAttribReal(XMLNode, "inner");
    Ogre::Real outer = getAttribReal(XMLNode, "outer");
    Ogre::Real falloff = getAttribReal(XMLNode, "falloff", 1.0);
 
    //PTR TuanNA- Setup the light range... [21:56:44 4/9/2016 by TNA]
    pLight->setSpotlightRange(Ogre::Angle(inner), Ogre::Angle(outer), falloff);
}
 
void DotSceneLoader::processLightAttenuation(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight)
{
    //PTR TuanNA- Process attributes... [21:56:49 4/9/2016 by TNA]
    Ogre::Real range = getAttribReal(XMLNode, "range");
    Ogre::Real constant = getAttribReal(XMLNode, "constant");
    Ogre::Real linear = getAttribReal(XMLNode, "linear");
    Ogre::Real quadratic = getAttribReal(XMLNode, "quadratic");
 
    //PTR TuanNA- Setup the light attenuation... [21:56:53 4/9/2016 by TNA]
    pLight->setAttenuation(range, constant, linear, quadratic);
}
 
 
Ogre::String DotSceneLoader::getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, const Ogre::String &defaultValue)
{
    if(XMLNode->first_attribute(attrib.c_str()))
        return XMLNode->first_attribute(attrib.c_str())->value();
    else
        return defaultValue;
}
 
Ogre::Real DotSceneLoader::getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, Ogre::Real defaultValue)
{
    if(XMLNode->first_attribute(attrib.c_str()))
        return Ogre::StringConverter::parseReal(XMLNode->first_attribute(attrib.c_str())->value());
    else
        return defaultValue;
}
 
bool DotSceneLoader::getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, bool defaultValue)
{
    if(!XMLNode->first_attribute(attrib.c_str()))
        return defaultValue;
 
    if(Ogre::String(XMLNode->first_attribute(attrib.c_str())->value()) == "true")
        return true;
 
    return false;
}
 
Ogre::Vector3 DotSceneLoader::parseVector3(rapidxml::xml_node<>* XMLNode)
{
    return Ogre::Vector3(
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value()),
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value()),
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value())
    );
}
 
Ogre::Quaternion DotSceneLoader::parseQuaternion(rapidxml::xml_node<>* XMLNode)
{
    //! @todo Fix this crap!
 
    Ogre::Quaternion orientation;
 
    if(XMLNode->first_attribute("qx"))
    {
        orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qx")->value());
        orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qy")->value());
        orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qz")->value());
        orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qw")->value());
    }
    if(XMLNode->first_attribute("qw"))
    {
        orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qw")->value());
        orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qx")->value());
        orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qy")->value());
        orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qz")->value());
    }
    else if(XMLNode->first_attribute("axisX"))
    {
        Ogre::Vector3 axis;
        axis.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisX")->value());
        axis.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisY")->value());
        axis.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisZ")->value());
        Ogre::Real angle = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angle")->value());;
        orientation.FromAngleAxis(Ogre::Angle(angle), axis);
    }
    else if(XMLNode->first_attribute("angleX"))
    {
        Ogre::Vector3 axis;
        axis.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angleX")->value());
        axis.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angleY")->value());
        axis.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angleZ")->value());
        //orientation.FromAxes(&axis);
        //orientation.F
    }
    else if(XMLNode->first_attribute("x"))
    {
        orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value());
        orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value());
        orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value());
        orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("w")->value());
    }
    else if(XMLNode->first_attribute("w"))
    {
        orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("w")->value());
        orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value());
        orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value());
        orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value());
    }
 
    return orientation;
}
 
Ogre::ColourValue DotSceneLoader::parseColour(rapidxml::xml_node<>* XMLNode)
{
    return Ogre::ColourValue(
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("r")->value()),
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("g")->value()),
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("b")->value()),
        XMLNode->first_attribute("a") != NULL ? Ogre::StringConverter::parseReal(XMLNode->first_attribute("a")->value()) : 1
    );
}
 
Ogre::String DotSceneLoader::getProperty(const Ogre::String &ndNm, const Ogre::String &prop)
{
    for ( unsigned int i = 0 ; i < nodeProperties.size(); i++ )
    {
        if ( nodeProperties[i].nodeName == ndNm && nodeProperties[i].propertyNm == prop )
        {
            return nodeProperties[i].valueName;
        }
    }
 
    return "";
}
 
void DotSceneLoader::processUserDataReference(rapidxml::xml_node<>* XMLNode, Ogre::Entity *pEntity)
{
    Ogre::String str = XMLNode->first_attribute("id")->value();
    pEntity->setUserAny(Ogre::Any(str));
}
