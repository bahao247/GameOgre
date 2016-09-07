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
| Filename   : AppState.hpp                                                           
---------------------------------------------------------------------------------------
| Written in : C++                                                     
---------------------------------------------------------------------------------------
| Compiler   : Microsoft Visual C++ 2010 Service Pack 1.                               
---------------------------------------------------------------------------------------
| Engine     : Ogre 1.8;...                                                                     
---------------------------------------------------------------------------------------
| Written by : Ba Hao Nguyen – Sports editors, IT.                                
---------------------------------------------------------------------------------------
| Note       : Description:	In this file, two classes are defined: AppStateListener and AppState. More info later down....                                
---------------------------------------------------------------------------------------
| Create File: 11:00:00 4/7/2016 by TNA                                                             
---------------------------------------------------------------------------------------
| Last Update: 11:15:26 4/9/2016 by TNA                                                             
---------------------------------------------------------------------------------------
*/
//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef APP_STATE_HPP
#define APP_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AdvancedOgreFramework.hpp"

class AppState;

//|||||||||||||||||||||||||||||||||||||||||||||||
//PTR TuanNA- The first class will later be inherited by the application state manager, but has to be defined here due to design reasons.... [12:10:33 4/9/2016 by TNA]
class AppStateListener
{
public:
	AppStateListener(){};
	virtual ~AppStateListener(){};

	//PTR TuanNA- Function to later add a new state to the manager... [12:11:40 4/9/2016 by TNA]
	virtual void manageAppState(Ogre::String stateName, AppState* state) = 0;

	//PTR TuanNA- Returns a pointer to the state with the respective name... [12:12:30 4/9/2016 by TNA]
	virtual AppState*	findByName(Ogre::String stateName) = 0;

	//PTR TuanNA- Exits the current app state and starts the one specified as the parameter... [12:13:40 4/9/2016 by TNA]
	virtual void		changeAppState(AppState *state) = 0;
	
	//PTR TuanNA- Puts a new app state on the active state stack that will then be excecuted.... [12:14:39 4/9/2016 by TNA]
	virtual bool		pushAppState(AppState* state) = 0;
	
	//PTR TuanNA- Removes the top active state from the stack, which results in returning to the one below.... [12:15:24 4/9/2016 by TNA]
	virtual void		popAppState() = 0;
	
	//PTR TuanNA- Pause state... [12:16:12 4/9/2016 by TNA]
	virtual void		pauseAppState() = 0;
	
	//PTR TuanNA- Finish state... [12:16:27 4/9/2016 by TNA]
	virtual void		shutdown() = 0;
    
	//PTR TuanNA- Not in the Advanced Framework originally. Removes all states and shut down the application.... [12:17:17 4/9/2016 by TNA]
	virtual void        popAllAndPushAppState(AppState* state) = 0;
};

//|||||||||||||||||||||||||||||||||||||||||||||||
//PTR TuanNA- The second class is the app state blueprint from which each actual application state will inherit:... [12:17:58 4/9/2016 by TNA]
class AppState : public OIS::KeyListener, public OIS::MouseListener, public OgreBites::SdkTrayListener
{
public:
	static void create(AppStateListener* parent, const Ogre::String name){};

	void destroy(){delete this;}

	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual bool pause(){return true;}
	virtual void resume(){};
	virtual void update(double timeSinceLastFrame) = 0;

protected:
	AppState(){};
	//PTR TuanNA- Inherit AppStateListener... [12:20:31 4/9/2016 by TNA]
	AppState*	findByName(Ogre::String stateName){return m_pParent->findByName(stateName);}
	void		changeAppState(AppState* state){m_pParent->changeAppState(state);}
	bool		pushAppState(AppState* state){return m_pParent->pushAppState(state);}
	void		popAppState(){m_pParent->popAppState();}
	void		shutdown(){m_pParent->shutdown();}
    void        popAllAndPushAppState(AppState* state){m_pParent->popAllAndPushAppState(state);}

	//PTR TuanNA- m_pParent is pointer of AppStateListener... [12:24:15 4/9/2016 by TNA]
	AppStateListener*			m_pParent;

	Ogre::Camera*				m_pCamera;
	Ogre::SceneManager*			m_pSceneMgr;
    Ogre::FrameEvent            m_FrameEvent;
};

//|||||||||||||||||||||||||||||||||||||||||||||||
//PTR TuanNA begin comment
//The last part of this file is a big #define statement. It defines the makro DECLARE_APPSTATE_CLASS via 
//that you can later create the game states. Note: The backslashes there are very important as they tell the 
//compiler to take those lines as one big one. Otherwise you would get compiler errors......- [12:26:34 4/9/2016 by TNA]

#define DECLARE_APPSTATE_CLASS(T)										\
static void create(AppStateListener* parent, const Ogre::String name)	\
{																		\
	T* myAppState = new T();											\
	myAppState->m_pParent = parent;										\
	parent->manageAppState(name, myAppState);							\
}

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif
//PTR TuanNA end comment
//|||||||||||||||||||||||||||||||||||||||||||||||