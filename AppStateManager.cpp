//|||||||||||||||||||||||||||||||||||||||||||||||
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
 | Filename   : AppStateManager.cpp                                                           
 ---------------------------------------------------------------------------------------
 | Written in : C++                                                     
 ---------------------------------------------------------------------------------------
 | Compiler   : Microsoft Visual C++ 2010 Service Pack 1.                               
 ---------------------------------------------------------------------------------------
 | Engine     : Ogre 1.8;...                                                                     
 ---------------------------------------------------------------------------------------
 | Written by : Ba Hao Nguyen – Sports editors, IT.                                
 ---------------------------------------------------------------------------------------
 | Note       : The class AppStateManager inherits from the class AppStateListener 
 shown above and mainly implements its abstract methods. Furthermore it contains 
 - a std::vector for all existing states (mtStates).							  
 - a std::vector for the active states, so the stack of those states currently.
 is use (mtActiveStateStack)...                                
 ---------------------------------------------------------------------------------------
 | Create File: 16:00:00 4/7/2016 by TNA                                                             
 ---------------------------------------------------------------------------------------
 | Last Update: 16:54:13 4/9/2016 by TNA                                                             
 ---------------------------------------------------------------------------------------
 */
#include "AppStateManager.hpp"
 
#include <OgreWindowEventUtilities.h>
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
AppStateManager::AppStateManager()
{
	// PTR TuanNA [Setting the shutdown indicator- 11/7/2016]
	m_bShutdown = false;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
AppStateManager::~AppStateManager()
{
	state_info si;
	// PTR TuanNA [Exiting all active application states and emptying the std::vectors- 11/7/2016]
        while(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}
 
	while(!m_States.empty())
	{
		si = m_States.back();
                si.state->destroy();
                m_States.pop_back();
	}
}

void AppStateManager::manageAppState(Ogre::String stateName, AppState* state)
{
	// PTR TuanNA [Called from within the state creation macro and sets some information of the new state, 
	//as well as pushing it on the active states stack- 11/7/2016]
	try
	{
		state_info new_state_info;
		new_state_info.name = stateName;
		new_state_info.state = state;
		m_States.push_back(new_state_info);		
	}
	catch(std::exception& e)
	{
		delete state;
		throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR, "Error while trying to manage a new AppState\n" + Ogre::String(e.what()), "AppStateManager.cpp (77)");
	}
}
AppState* AppStateManager::findByName(Ogre::String stateName)
{
	// PTR TuanNA [Returns a pointer to the state with the respective name- 11/7/2016]
	std::vector<state_info>::iterator itr;
 
	for(itr=m_States.begin();itr!=m_States.end();itr++)
	{
		if(itr->name==stateName)
			return itr->state;
	}
 
	return 0;
}

void AppStateManager::start(AppState* state)
{
	/*
	//PTR TuanNA begin comment
	//Main loop of the application that does the following steps:
		1. Change to the state specified...
	//[21:00:51 4/9/2016 by TNA]
	//PTR TuanNA end comment
	*/
	changeAppState(state);
 
	int timeSinceLastFrame = 1;
	long startTime = 0;
	
	//PTR TuanNA- 2. Start loop... [21:01:14 4/9/2016 by TNA]
	while(!m_bShutdown)
	{
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;
 
		Ogre::WindowEventUtilities::messagePump();
 
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{
			startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();
			//PTR TuanNA- 3. Capture keyboard and mouse input... [21:01:32 4/9/2016 by TNA]
			OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
			OgreFramework::getSingletonPtr()->m_pMouse->capture();
			
			//PTR TuanNA- 4. Update the current state (the top most of the stack)... [21:01:51 4/9/2016 by TNA]
			m_ActiveStateStack.back()->update(timeSinceLastFrame);
			
			OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
			
			//PTR TuanNA- 5. Call the OgreFramework class to update and render... [21:01:59 4/9/2016 by TNA]
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
 
			timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
		}
		else
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
		}
	}
 
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
}

void AppStateManager::changeAppState(AppState* state)
{
	//PTR TuanNA- Exits the current state (if there is any) and starts the new specified one.... [21:02:41 4/9/2016 by TNA]
	if(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}
 
	m_ActiveStateStack.push_back(state);
	init(state);
	m_ActiveStateStack.back()->enter();
}

bool AppStateManager::pushAppState(AppState* state)
{
	//PTR TuanNA- Puts an new state on the top of the stack and starts it... [21:03:16 4/9/2016 by TNA]
	if(!m_ActiveStateStack.empty())
	{
		if(!m_ActiveStateStack.back()->pause())
			return false;
	}
 
	m_ActiveStateStack.push_back(state);
	init(state);
	m_ActiveStateStack.back()->enter();
 
	return true;
}

void AppStateManager::popAppState()
{
	//PTR TuanNA- Removes the top most state and resumes the one below if there is one, otherwise shutdown the application... [21:05:30 4/9/2016 by TNA]
	if(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}
 
	if(!m_ActiveStateStack.empty())
	{
		init(m_ActiveStateStack.back());
		m_ActiveStateStack.back()->resume();
	}
    else
		shutdown();
}

void AppStateManager::popAllAndPushAppState(AppState* state)
{
	//PTR TuanNA- Removes all states and activates shutdown.... [21:06:29 4/9/2016 by TNA]
    while(!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->exit();
        m_ActiveStateStack.pop_back();
    }
 
    pushAppState(state);
}

void AppStateManager::pauseAppState()
{
	if(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->pause();
	}
 
	if(m_ActiveStateStack.size() > 2)
	{
		init(m_ActiveStateStack.at(m_ActiveStateStack.size() - 2));
		m_ActiveStateStack.at(m_ActiveStateStack.size() - 2)->resume();
	}
}

void AppStateManager::shutdown()
{
	//PTR TuanNA- Exits the application... [21:06:47 4/9/2016 by TNA]
	m_bShutdown = true;
}

void AppStateManager::init(AppState* state)
{
	//PTR TuanNA- Initializes a new state and links the input callback on it, as well as resetting the Ogre statistics 
	//	(FPS, triangle count, batch count,..)- [21:07:05 4/9/2016 by TNA]
    OgreFramework::getSingletonPtr()->m_pKeyboard->setEventCallback(state);
	OgreFramework::getSingletonPtr()->m_pMouse->setEventCallback(state);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(state);
 
	OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();
}