//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "SinbadState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
SinbadState::SinbadState()
{
	mInfo["Title"] = "Untitled";
	mInfo["Description"] = "";
	mInfo["Category"] = "Unsorted";
	mInfo["Thumbnail"] = "";
	mInfo["Help"] = "";
    m_bQuit             = false;
    m_pDetailsPanel	= 0;
	m_pCamera		= 0;
	m_pCameraMan	= 0;
	m_pChara		= 0;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void SinbadState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering SinbadState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");

    m_pCamera = m_pSceneMgr->createCamera("MainCamera");
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	m_pCamera->setAspectRatio((Ogre::Real)OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth() / (Ogre::Real)OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight());
	m_pCamera->setNearClipDistance(5);

	m_pCameraMan = new OgreBites::SdkCameraMan(m_pCamera); 

    buildGUI();
 
    createScene();
}

void SinbadState::createScene()
{
	// set background and some fog
	//OgreFramework::getSingletonPtr()->m_pViewport->setBackgroundColour(ColourValue(1.0f, 1.0f, 0.8f));
	//m_pSceneMgr->setFog(Ogre::FOG_LINEAR, ColourValue(1.0f, 1.0f, 0.8f), 0, 15, 100);
	
	//PTR TuanNA begin comment
	//[- 7/9/2016]
	Ogre::TexturePtr rttTexture =
		Ogre::TextureManager::getSingleton().createManual(
		"RttTex",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D,
		1024, 768,
		0,
		Ogre::PF_R8G8B8,
		Ogre::TU_RENDERTARGET);

	Ogre::RenderTexture* renderTexture = rttTexture->getBuffer()->getRenderTarget();
	renderTexture->addViewport(m_pCamera);
	renderTexture->getViewport(0)->setClearEveryFrame(true);
	renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
	renderTexture->getViewport(0)->setOverlaysEnabled(false);

	renderTexture->update();
	renderTexture->writeContentsToFile("start.png");

	mMiniscreen = new Ogre::Rectangle2D(true);
	mMiniscreen->setCorners(.5, 1.0, 1.0, .5);
	mMiniscreen->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);

	Ogre::SceneNode* miniscreenNode =
		m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	miniscreenNode->attachObject(mMiniscreen);

	Ogre::MaterialPtr renderMat =
		Ogre::MaterialManager::getSingleton().create(
		"RttMat",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	renderMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	renderMat->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex");

	mMiniscreen->setMaterial("RttMat");

	renderTexture->addListener(this);
	//PTR TuanNA end comment 

	// set shadow properties
	m_pSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
	m_pSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
	m_pSceneMgr->setShadowTextureSize(1024);
	m_pSceneMgr->setShadowTextureCount(1);

	// disable default camera control so the character can do its own
	m_pCameraMan->setStyle(OgreBites::CS_MANUAL);

	// use a small amount of ambient lighting
	m_pSceneMgr->setAmbientLight(ColourValue(1, 1, 1));

	//set sky
	m_pSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

	// add a bright light above the scene
	Light* light = m_pSceneMgr->createLight();
	light->setType(Light::LT_POINT);
	light->setPosition(-10, 40, 20);
	light->setSpecularColour(ColourValue::White);

	//PTR TuanNA begin comment
	//[createGrassMesh- 7/9/2016]
	createGrassMesh();

	Ogre::Plane plane;
	plane.normal = Ogre::Vector3::UNIT_Y;
	plane.d = 0;

	Ogre::MeshManager::getSingleton().createPlane(
		"floor",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		450.0, 450.0,
		10, 10, true, 1,
		50.0, 50.0,
		Ogre::Vector3::UNIT_Z);

	Ogre::Entity* planeEntity = m_pSceneMgr->createEntity("floor");
	planeEntity->setMaterialName("Examples/GrassFloor");
	planeEntity->setCastShadows(false);
	m_pSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(planeEntity);

	Ogre::Entity* grass = m_pSceneMgr->createEntity("GrassBladesMesh");
	Ogre::StaticGeometry* sg = m_pSceneMgr->createStaticGeometry("GrassArea");

	const int size = 375;
	const int amount = 20;

	sg->setRegionDimensions(Ogre::Vector3(size, size, size));
	sg->setOrigin(Ogre::Vector3(-size/2, 0, -size/2));

	for (int x = -size/2; x < size/2; x += (size / amount))
	{
		for (int z = -size/2; z < size/2; z += (size / amount))
		{
			Ogre::Real r = size / (float)amount / 2;
			Ogre::Vector3 pos(
				x + Ogre::Math::RangeRandom(-r, r),
				0,
				z + Ogre::Math::RangeRandom(-r, r));

			Ogre::Vector3 scale(1, Ogre::Math::RangeRandom(0.9, 1.1), 1);

			Ogre::Quaternion orientation;
			orientation.FromAngleAxis(
				Ogre::Degree(Ogre::Math::RangeRandom(0, 359)),
				Ogre::Vector3::UNIT_Y);

			sg->addEntity(grass, pos, orientation, scale);
		}
	}

	sg->build();
	//PTR TuanNA end comment
	
/*	
	//PTR TuanNA begin comment
	//[Remove Plan Rockwall- 7/9/2016]
	// create a floor mesh resource
	MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	Plane(Vector3::UNIT_Y, 0), 100, 100, 10, 10, true, 1, 10, 10, Vector3::UNIT_Z);

	// create a floor entity, give it a material, and place it at the origin
	Entity* floor = m_pSceneMgr->createEntity("Floor", "floor");
	floor->setMaterialName("Examples/Rockwall");
	floor->setCastShadows(false);
	m_pSceneMgr->getRootSceneNode()->attachObject(floor);
	//PTR TuanNA end comment
*/	

	//load scene
	DotSceneLoader* pDotSceneLoader = new DotSceneLoader();
    pDotSceneLoader->parseDotScene("CubeScene.xml", "General", m_pSceneMgr, m_pSceneMgr->getRootSceneNode());
	delete pDotSceneLoader;

	m_pCamera->setNearClipDistance(0.1);
	m_pCamera->setFarClipDistance(100000);
	// create our character controller
	m_pChara = new SinbadCharacterController(m_pCamera);

	OgreFramework::getSingletonPtr()->m_pTrayMgr->toggleAdvancedFrameStats();

	StringVector items;
	items.push_back("Help");
	OgreBites::ParamsPanel* help = OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "HelpMessage", 100, items);
	help->setParamValue("Help", "H / F1");
}

void SinbadState::createGrassMesh()
{
	const float width = 10;
	const float height = 12;
	Ogre::Vector3 vec(width / 2, 0, 0);
	Ogre::ManualObject obj("GrassObject");

	Ogre::Quaternion quat;
	quat.FromAngleAxis(Ogre::Degree(60), Ogre::Vector3::UNIT_Y);

	obj.begin("Examples/GrassBlades", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	for (int i = 0; i < 3; ++i)
	{
		obj.position(-vec.x, height, -vec.z);
		obj.textureCoord(0, 0);
		obj.position(vec.x, height, vec.z);
		obj.textureCoord(1, 0);
		obj.position(-vec.x, 0, -vec.z);
		obj.textureCoord(0, 1);
		obj.position(vec.x, 0, vec.z);
		obj.textureCoord(1, 1);

		int offset = 4 * i;
		obj.triangle(offset, offset + 3, offset + 1);
		obj.triangle(offset, offset + 2, offset + 3);

		vec = quat * vec;
	}

	obj.end();
	obj.convertToMesh("GrassBladesMesh");
}


bool SinbadState::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing SinbadState...");
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void SinbadState::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming SinbadState...");
 
    buildGUI();
 
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_bQuit = false;
}

void SinbadState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving SinbadState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
	if (m_pCameraMan) delete m_pCameraMan;
	
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
}


bool SinbadState::keyPressed(const OIS::KeyEvent &evt)
{
	if (!OgreFramework::getSingletonPtr()->m_pTrayMgr->isDialogVisible()) m_pChara->injectKeyDown(evt);
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        pushAppState(findByName("PauseState"));
        return true;
    }

	if (evt.key == OIS::KC_H || evt.key == OIS::KC_F1)   // toggle visibility of help dialog
			{
				if (!OgreFramework::getSingletonPtr()->m_pTrayMgr->isDialogVisible() && mInfo["Help"] != "") OgreFramework::getSingletonPtr()->m_pTrayMgr->showOkDialog("Help", mInfo["Help"]);
				else OgreFramework::getSingletonPtr()->m_pTrayMgr->closeDialog();
			}

			if (OgreFramework::getSingletonPtr()->m_pTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

			if (evt.key == OIS::KC_F)   // toggle visibility of advanced frame stats
			{
				OgreFramework::getSingletonPtr()->m_pTrayMgr->toggleAdvancedFrameStats();
			}
			else if (evt.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
			{
				if (m_pDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
				{
					OgreFramework::getSingletonPtr()->m_pTrayMgr->moveWidgetToTray(m_pDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
					m_pDetailsPanel->show();
				}
				else
				{
					OgreFramework::getSingletonPtr()->m_pTrayMgr->removeWidgetFromTray(m_pDetailsPanel);
					m_pDetailsPanel->hide();
				}
			}
			else if (evt.key == OIS::KC_T)   // cycle polygon rendering mode
			{
				Ogre::String newVal;
				Ogre::TextureFilterOptions tfo;
				unsigned int aniso;

				switch (m_pDetailsPanel->getParamValue(9).asUTF8()[0])
				{
				case 'B':
					newVal = "Trilinear";
					tfo = Ogre::TFO_TRILINEAR;
					aniso = 1;
					break;
				case 'T':
					newVal = "Anisotropic";
					tfo = Ogre::TFO_ANISOTROPIC;
					aniso = 8;
					break;
				case 'A':
					newVal = "None";
					tfo = Ogre::TFO_NONE;
					aniso = 1;
					break;
				default:
					newVal = "Bilinear";
					tfo = Ogre::TFO_BILINEAR;
					aniso = 1;
				}

				Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
				Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
				m_pDetailsPanel->setParamValue(9, newVal);
			}
			else if (evt.key == OIS::KC_R)   // cycle polygon rendering mode
			{
				Ogre::String newVal;
				Ogre::PolygonMode pm;

				switch (m_pCamera->getPolygonMode())
				{
				case Ogre::PM_SOLID:
					newVal = "Wireframe";
					pm = Ogre::PM_WIREFRAME;
					break;
				case Ogre::PM_WIREFRAME:
					newVal = "Points";
					pm = Ogre::PM_POINTS;
					break;
				default:
					newVal = "Solid";
					pm = Ogre::PM_SOLID;
				}

				m_pCamera->setPolygonMode(pm);
				m_pDetailsPanel->setParamValue(10, newVal);
			}
			else if(evt.key == OIS::KC_F5)   // refresh all textures
			{
				Ogre::TextureManager::getSingleton().reloadAll();
			}
			else if (evt.key == OIS::KC_SYSRQ)   // take a screenshot
			{
				OgreFramework::getSingletonPtr()->m_pRenderWnd->writeContentsToTimestampedFile("screenshot", ".png");
			}

			m_pCameraMan->injectKeyDown(evt);
			return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool SinbadState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	m_pChara->injectKeyUp(keyEventRef);
	m_pCameraMan->injectKeyUp(keyEventRef);
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

bool SinbadState::mouseMoved(const OIS::MouseEvent &evt)
{
	if (!OgreFramework::getSingletonPtr()->m_pTrayMgr->isDialogVisible()) m_pChara->injectMouseMove(evt);
	if (OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
	m_pCameraMan->injectMouseMove(evt);
  
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool SinbadState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	// relay input events to character controller
	if (!OgreFramework::getSingletonPtr()->m_pTrayMgr->isDialogVisible()) m_pChara->injectMouseDown(evt, id);
	if (OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
            
	if (m_pDragLook && id == OIS::MB_Left)
	{
		m_pCameraMan->setStyle(OgreBites::CS_MANUAL);
		OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
	}

	m_pCameraMan->injectMouseDown(evt, id);
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool SinbadState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if (OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
            
	if (m_pDragLook && id == OIS::MB_Left)
	{
		m_pCameraMan->setStyle(OgreBites::CS_MANUAL);
		OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
	}

	m_pCameraMan->injectMouseUp(evt, id);

	return true;
}


void SinbadState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;

	m_pChara->addTime(timeSinceLastFrame/1000);

    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
 
	if (!OgreFramework::getSingletonPtr()->m_pTrayMgr->isDialogVisible())
	{
		m_pCameraMan->frameRenderingQueued(m_FrameEvent);   // if dialog isn't up, then update the camera

		if (m_pDetailsPanel->isVisible())   // if details panel is visible, then update its contents
		{
			m_pDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().x));
			m_pDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().y));
			m_pDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().z));
			m_pDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().w));
			m_pDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().x));
			m_pDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().y));
			m_pDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().z));
		}	
	}

 
    //getInput();
    //moveCamera();
}

void SinbadState::buildGUI()
{
	// show stats and logo and hide the cursor
	OgreFramework::getSingletonPtr()->m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "GameLbl", "Game mode", 250);
	OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();


    // create a params panel for displaying sample details
	Ogre::StringVector items;
	items.push_back("cam.pX");
	items.push_back("cam.pY");
	items.push_back("cam.pZ");
	items.push_back("");
	items.push_back("cam.oW");
	items.push_back("cam.oX");
	items.push_back("cam.oY");
	items.push_back("cam.oZ");
	items.push_back("");
	items.push_back("Filtering");
	items.push_back("Poly Mode");
 
	m_pDetailsPanel = OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
	m_pDetailsPanel->hide();

	m_pDetailsPanel->setParamValue(9, "Bilinear");
	m_pDetailsPanel->setParamValue(10, "Solid");

}

void SinbadState::itemSelected(OgreBites::SelectMenu* menu)
{
    switch(menu->getSelectionIndex())
    {
    case 0:
        m_pCamera->setPolygonMode(Ogre::PM_SOLID);break;
    case 1:
        m_pCamera->setPolygonMode(Ogre::PM_WIREFRAME);break;
    case 2:
        m_pCamera->setPolygonMode(Ogre::PM_POINTS);break;
    }
}
//////////////////////////////////////////////////////////////////////////
void SinbadState::preRenderTargetUpdate(const Ogre::RenderTargetEvent& rte)
{
	mMiniscreen->setVisible(false);
}
//////////////////////////////////////////////////////////////////////////
void SinbadState::postRenderTargetUpdate(const Ogre::RenderTargetEvent& rte)
{
	mMiniscreen->setVisible(true);
}