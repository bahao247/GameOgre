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
 | Filename   : AppStateManager.hpp                                                           
 ---------------------------------------------------------------------------------------
 | Written in : C++                                                     
 ---------------------------------------------------------------------------------------
 | Compiler   : Microsoft Visual C++ 2010 Service Pack 1.                               
 ---------------------------------------------------------------------------------------
 | Engine     : Ogre 1.8;...                                                                     
 ---------------------------------------------------------------------------------------
 | Written by : Ba Hao Nguyen – Sports editors, IT.                                
 ---------------------------------------------------------------------------------------
 | Note       : The class AppStateManager inherits from the class 
 AppStateListener shown above and mainly implements its abstract methods. 
 Furthermore it contains:
 - a std::vector for all existing states (mtStates).							   
 - a std::vector for the active states, so the stack of those states 
 currently is use (mtActiveStateStack)                                
 ---------------------------------------------------------------------------------------
 | Create File: 13:00:00 4/7/2016 by TNA                                                             
 ---------------------------------------------------------------------------------------
 | Last Update: 13:37:50 4/9/2016 by TNA                                                             
 ---------------------------------------------------------------------------------------
 */
#ifndef APP_STATE_MANAGER_HPP
#define APP_STATE_MANAGER_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "AppState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class AppStateManager : public AppStateListener
{
public:
	typedef struct
	{
		Ogre::String name;
		AppState* state;
	} state_info;
	//PTR TuanNA- Constructor, just setting the shutdown indicator. - [13:43:22 4/9/2016 by TNA]
	AppStateManager();
	//PTR TuanNA- Destructor, exiting all active application states and emptying the std::vectors.- [13:45:02 4/9/2016 by TNA]
	~AppStateManager();
	
	//PTR TuanNA- Called from within the state creation macro and sets some information of the new state, 
	//as well as pushing it on the active states stack... [13:48:44 4/9/2016 by TNA]
	void manageAppState(Ogre::String stateName, AppState* state);
	
	//PTR TuanNA- Returns a pointer to the state with the respective name.... [13:49:35 4/9/2016 by TNA]
	AppState* findByName(Ogre::String stateName);
	
	/*
	//PTR TuanNA begin comment
	//Main loop of the application that does the following steps:
		1. Change to the state specified
		2. Start loop
		3. Capture keyboard and mouse input
		4. Update the current state (the top most of the stack)
		5. Call the OgreFramework class to update and render...-  [13:51:05 4/9/2016 by TNA]
	//PTR TuanNA end comment
	*/
	void start(AppState* state);

	//PTR TuanNA- Exits the current state (if there is any) and starts the new specified one.... [13:52:26 4/9/2016 by TNA]
	void changeAppState(AppState* state);
	
	//PTR TuanNA- Puts an new state on the top of the stack and starts it.... [13:52:53 4/9/2016 by TNA]
	bool pushAppState(AppState* state);
	
	//PTR TuanNA- Removes the top most state and resumes the one below if there is one, otherwise shutdown the application.... [13:53:35 4/9/2016 by TNA]
	void popAppState();
	
	//PTR TuanNA- Exits the application... [13:54:21 4/9/2016 by TNA]
	void pauseAppState();

	/*
	//PTR TuanNA begin comment
	// Not in the Advanced Framework originally. Removes the Game state in the states stack. ...-  [13:57:05 4/9/2016 by TNA]
	void popGameState();
	
	//Not in the Advanced Framework originally. Removes all states and activates shutdown.
	void popAllAppStates();
	//PTR TuanNA end comment
	*/

	//PTR TuanNA- Pause the application... [13:54:21 4/9/2016 by TNA]
	void shutdown();
    
	//PTR TuanNA- Not in the Advanced Framework originally. Removes all states and activates shutdown.... [13:55:22 4/9/2016 by TNA]
	void popAllAndPushAppState(AppState* state);
 
protected:
	//PTR TuanNA- Initializes a new state and links the input callback on it, as well as resetting the Ogre statistics 
	//	(FPS, triangle count, batch count, ...... [13:59:54 4/9/2016 by TNA]
	void init(AppState *state);
 
	std::vector<AppState*>		m_ActiveStateStack;
	std::vector<state_info>		m_States;
	bool				m_bShutdown;
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||