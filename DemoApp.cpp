//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "DemoApp.hpp"
 
#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "SinbadState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
DemoApp::DemoApp()
{
	m_pAppStateManager = 0;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
DemoApp::~DemoApp()
{
	delete m_pAppStateManager;
        delete OgreFramework::getSingletonPtr();
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void DemoApp::startDemo()
{
	//PTR TuanNA- Create App: Title, log and select state [21:16:40 4/9/2016 by TNA]
	new OgreFramework();
	if(!OgreFramework::getSingletonPtr()->initOgre("Game Ogre3D", 0, 0))
		return;
 
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");
 
	m_pAppStateManager = new AppStateManager();
	
	MenuState::create(m_pAppStateManager, "MenuState");
	GameState::create(m_pAppStateManager, "GameState");
	PauseState::create(m_pAppStateManager, "PauseState");
	SinbadState::create(m_pAppStateManager, "SinbadState");
 
	m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||