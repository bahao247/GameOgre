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
| Filename   : AdvancedOgreFramework.cpp                                                           
---------------------------------------------------------------------------------------
| Written in : C++                                                 
---------------------------------------------------------------------------------------
| Compiler   : Microsoft Visual C++ 2010 Service Pack 1.                               
---------------------------------------------------------------------------------------
| Engine     : Ogre3D                                                                     
---------------------------------------------------------------------------------------
| Written by : Ba Hao Nguyen – Sports editors, IT.                                
---------------------------------------------------------------------------------------
| Note       : loadGameResources                                
---------------------------------------------------------------------------------------
| Create File: 23:54:51 10/7/2016 by bahao                                                             
---------------------------------------------------------------------------------------
| Last Update: 23:54:51 10/7/2016 by bahao                                                             
---------------------------------------------------------------------------------------
*/
/*---------------------------------------------------------------------------------*/
/*																				   */
/* Description:	It contains most of the Ogre related variables:					   */
/* - Root																		   */	
/* - RenderWindow																   */		
/* - Viewport																       */				
/* - Log																		   */
/* - Timer																		   */
/* - InputManager / Keyboard / Mouse											   */
/*																				   */		
/* It also offers functions to handle input, but that is almost not used here, as  */
/* each application state needs a different input behavior and therefore this is   */
/* directly modeled in the application states. However, everything common for the  */
/* whole application can be put here.											   */
/*---------------------------------------------------------------------------------*/

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AdvancedOgreFramework.hpp"
#include "res/Resource.h" // PTR TuanNA [Add include- 10/7/2016]

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||
//Singleton is initialized
template<> OgreFramework* Ogre::Singleton<OgreFramework>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::OgreFramework()
{
    m_pRoot				= 0;
    m_pRenderWnd		= 0;
    m_pViewport			= 0;
    m_pLog				= 0;
    m_pTimer			= 0;

    m_pInputMgr			= 0;
    m_pKeyboard			= 0;
    m_pMouse			= 0;
    m_pTrayMgr          = 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::~OgreFramework()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
    if(m_pTrayMgr)      delete m_pTrayMgr;
    if(m_pInputMgr)		OIS::InputManager::destroyInputSystem(m_pInputMgr);
    if(m_pRoot)			delete m_pRoot;

	/*
	//PTR TuanNA begin comment
	//
	if(mpSound)
		mpSound->deleteSound();
	
	delete mpGui;
	delete mpSound;	
	delete mpRoot;...
	
	//[16:33:44 4/9/2016 by TNA]
	//PTR TuanNA end comment
	*/
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
	// PTR TuanNA [Create and initiate the Log- 10/7/2016]
    Ogre::LogManager* logMgr = new Ogre::LogManager();

    m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    m_pLog->setDebugOutputEnabled(true);

	// PTR TuanNA [Create an Ogre Root- 10/7/2016]
    m_pRoot = new Ogre::Root();
//PTR TuanNA begin comment
//[Add icon to app- 10/7/2016]
    if(m_pRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		m_pRenderWnd = m_pRoot->initialise(true, "OgreFramework Render Window");

		// Let's add a nice window icon
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		HWND hwnd;
		m_pRenderWnd->getCustomAttribute("WINDOW", (void*)&hwnd);
		LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON) );
		SetClassLong( hwnd, GCL_HICON, iconID );
#endif
//PTR TuanNA end comment
		// PTR TuanNA [Remove CM m_pRenderWnd- 10/7/2016] m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

		// PTR TuanNA [Add a viewport the to Render window- 11/7/2016]
		m_pViewport = m_pRenderWnd->addViewport(0);
		m_pViewport->setBackgroundColour(ColourValue(0.5f, 0.5f, 0.5f, 1.0f));

		m_pViewport->setCamera(0);

		//PTR TuanNA- Init Ogre Render Window... [16:37:20 4/9/2016 by TNA]
		size_t hWnd = 0;
		OIS::ParamList paramList;
		m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

		paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

		// PTR TuanNA [Create Input System- 11/7/2016]
		m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

		m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
		m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));

		m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
		m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();

		if(pKeyListener == 0)
			m_pKeyboard->setEventCallback(this);
		else
			m_pKeyboard->setEventCallback(pKeyListener);

		if(pMouseListener == 0)
			m_pMouse->setEventCallback(this);
		else
			m_pMouse->setEventCallback(pMouseListener);

		Ogre::String secName, typeName, archName;
		Ogre::ConfigFile cf;
#ifdef _DEBUG
		cf.load("resources_d.cfg");
#else
		cf.load("resources.cfg");
#endif

		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			}
		}

		// PTR TuanNA [Configure the amount of MipMaps per texture(to avoid aliasing).- 11/7/2016]
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		/*
		//PTR TuanNA begin comment
		////Load cursor resources
			Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Cursor");
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Media/Cursor.zip","Zip","Cursor");
			Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Cursor");
		
			//Create the GUI handler
			mpGui = new GuiHandler(mpRenderWnd);
		
		#ifdef DEBUG
			loadDebugResources();
			mpGui->initDebugGui();
		#endif
			
			//Link the log to the Settings singleton
			Settings::getSingletonPtr()->setLog(mpLog);	
		
			//Initiate the sound.
			mpSound = new SoundManager();
		//[16:43:54 4/9/2016 by TNA]
		//PTR TuanNA end comment
		*/

		m_pTrayMgr = new OgreBites::SdkTrayManager("AOFTrayMgr", m_pRenderWnd, m_pMouse, 0);
		
		// PTR TuanNA [Initiate the Timer- 11/7/2016]
		m_pTimer = new Ogre::Timer();
		m_pTimer->reset();

		m_pRenderWnd->setActive(true);
		return true;
	}
	else
	{
		return false;
	}
}  

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        m_pRenderWnd->writeContentsToTimestampedFile("AOF_Screenshot_", ".jpg");
        return true;
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_O))
    {
        if(m_pTrayMgr->isLogoVisible())
        {
            m_pTrayMgr->hideFrameStats();
            m_pTrayMgr->hideLogo();
        }
        else
        {
            m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
            m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
        }
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::updateOgre(double timeSinceLastFrame)
{
}

//|||||||||||||||||||||||||||||||||||||||||||||||
/*
//PTR TuanNA begin comment
//---------------------------------------------------------------------------------
void GameFramework::loadMenuResources()
{
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Menu");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Media/Menu.zip","Zip","Menu");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Menu");
}
//---------------------------------------------------------------------------------
void GameFramework::loadGameResources()
{
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Media/Gui.zip","Zip","Game");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Media/Material.zip","Zip","Game");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Media/Meshes.zip","Zip","Game");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Media/ParticleSystems.zip","Zip","Game");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Media/Terrain.zip","Zip","Game");	
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Media/Textures.zip","Zip","Game");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Media/ActionBar.zip","Zip","Game");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Media/Generated Terrain","FileSystem","Game");

	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Game");
}
//---------------------------------------------------------------------------------
#ifdef DEBUG
void GameFramework::loadDebugResources()
{
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Debug");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Media/Debug.zip","Zip","Debug");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Debug");
}
#endif...

//[16:47:18 4/9/2016 by TNA]
//PTR TuanNA end comment
*/