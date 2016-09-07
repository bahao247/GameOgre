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
| Filename   : AdvancedOgreFramework.hpp                                                           
---------------------------------------------------------------------------------------
| Written in : C++                                                     
---------------------------------------------------------------------------------------
| Compiler   : Microsoft Visual C++ 2010 Service Pack 1.                               
---------------------------------------------------------------------------------------
| Engine     : Ogre 1.8;...                                                                     
---------------------------------------------------------------------------------------
| Written by : Ba Hao Nguyen – Sports editors, IT.                                
---------------------------------------------------------------------------------------
| Note       :	It contains most of the Ogre related variables:					   
 - Root																		   	
 - RenderWindow																   		
 - Viewport																       				
 - Log																		   
 - Timer																		   
 - InputManager / Keyboard / Mouse											   																			   		
 It also offers functions to handle input, but that is almost not used here, as 
 each application state needs a different input behavior and therefore this is   
 directly modeled in the application states. However, everything common for the  
 whole application can be put here.											   
---------------------------------------------------------------------------------------
| Create File: 17:00:00 2/7/2016 by TNA                                                             
---------------------------------------------------------------------------------------
| Last Update: 17:06:22 2/9/2016 by TNA                                                             
---------------------------------------------------------------------------------------
*/
//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_FRAMEWORK_HPP
#define OGRE_FRAMEWORK_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
/*
//PTR TuanNA begin comment
//Include library...-  [11:47:07 3/9/2016 by TNA]
#include "SoundManager.h"
#include "GuiHandler.h"
#include "Settings.h"
//PTR TuanNA end comment
*/

//|||||||||||||||||||||||||||||||||||||||||||||||

class OgreFramework : public Ogre::Singleton<OgreFramework>, OIS::KeyListener, OIS::MouseListener
{
public:
	OgreFramework();//PTR TuanNA- Constructor... [11:26:52 3/9/2016 by TNA]
	~OgreFramework();//PTR TuanNA- Destructor, clearing up... [11:27:10 3/9/2016 by TNA]

	/*
	//PTR TuanNA begin comment
	//Powers up Ogre with the following steps:
	1. Create the log manager
	2. Create the Root
	3. Create the RenderWindow and the Viewport
	4. Power up OIS	
	5. If there was no MouseListener or KeyboardListener passed as a parameter, use the ones 
	from this class, otherwise the passed ones (however you still can use both, by calling 
	the OgreFramework class input functions when you handle input elsewhere)
	6. Load resources
	7. Start Timer
	8. Set up the GUI Renderer and the GUI System and load all needed layouts (removed)
	9. create and show the debug overlay (removed)

	Note: After running this function you still won't see anything on the screen as there is 
	no Camera and no SceneManager. Those are members of the individual application states!-  [11:29:00 3/9/2016 by TNA]
	//PTR TuanNA end comment
	*/
	bool initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MouseListener *pMouseListener = 0);
	
	/*
	//PTR TuanNA begin comment
	//Loads the resources-  [11:41:45 3/9/2016 by TNA]
	// Loads the resources used for creating the Menu.
	void loadMenuResources();

	// Loads the resource used in the Game.
	void loadGameResources();

#ifdef DEBUG
	// Loads resources used for debugging.
	void loadDebugResources();
#endif
	//PTR TuanNA end comment
	*/
	
	void updateOgre(double timeSinceLastFrame);//PTR TuanNA- Is called once per frame by the AppStateManager to update everything directly related to Ogre, in our case only the statistics... [11:40:19 3/9/2016 by TNA]

	//PTR TuanNA begin comment
	//Handles to buffered input, common for the whole application-  [11:38:42 3/9/2016 by TNA]
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	//PTR TuanNA end comment

	Ogre::Root*					m_pRoot;
	Ogre::RenderWindow*			m_pRenderWnd;
	Ogre::Viewport*				m_pViewport;
	Ogre::Log*					m_pLog;
	Ogre::Timer*				m_pTimer;

	OIS::InputManager*			m_pInputMgr;
	OIS::Keyboard*				m_pKeyboard;
	OIS::Mouse*					m_pMouse;

    OgreBites::SdkTrayManager*	m_pTrayMgr;

private:
	OgreFramework(const OgreFramework&);
	OgreFramework& operator= (const OgreFramework&);
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||