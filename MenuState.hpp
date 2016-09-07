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
| Filename   : MenuState.hpp                                                           
---------------------------------------------------------------------------------------
| Written in : C++                                                     
---------------------------------------------------------------------------------------
| Compiler   : Microsoft Visual C++ 2010 Service Pack 1.                               
---------------------------------------------------------------------------------------
| Engine     : Ogre 1.8;...                                                                     
---------------------------------------------------------------------------------------
| Written by : Ba Hao Nguyen – Sports editors, IT.                                
---------------------------------------------------------------------------------------
| Note       : Menu State...                                
---------------------------------------------------------------------------------------
| Create File: 15:00:00 4/7/2016 by TNA                                                             
---------------------------------------------------------------------------------------
| Last Update: 15:04:39 4/9/2016 by TNA                                                             
---------------------------------------------------------------------------------------
*/ 
#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "AppState.hpp"
//PTR TuanNA- Include... [15:05:42 4/9/2016 by TNA] #include "GuiMouseOver.h"
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class MenuState : public AppState
{
public:
    MenuState();
	
	//PTR TuanNA- This line calls the macro defined in AppState.hpp, making this class a valid application state... [15:07:58 4/9/2016 by TNA]
	DECLARE_APPSTATE_CLASS(MenuState)
	
	/*
	//PTR TuanNA begin comment
	//Always called when starting this state and does the following steps:
		1. Create SceneManager
		2. Create a Camera
		3. Set input callbacks
		4. Tell the GUI which SceneManager to use (removed)
		5. Set the mouse cursor image (removed)
		6. Tell the GUI System which Window to render (removed)
		7. Get the buttons from the GUI and set the respective click handlers
		8. Call createScene() to fill the scene with content (although no scene to fill)...
		-  [15:13:50 4/9/2016 by TNA]
	//PTR TuanNA end comment
	*/
	void enter();

	//PTR TuanNA- Create Scene... [15:14:44 4/9/2016 by TNA]
	void createScene();

	//PTR TuanNA- Exits the Menu State. Called when starting a new Game State and therefore a loading screen will be shown.... [15:20:15 4/9/2016 by TNA]
	void exit();

	/*
	//PTR TuanNA begin comment
	// Didn't bother to set anything here since MenuState will never be paused. ...-  [15:23:43 4/9/2016 by TNA]
	bool pause();
	
	//Didn't bother to set anything here since MenuState will never be resumed.
	void resume();

	//The game state will always be in the background waiting. Pops the Menu state. 
	void resumeToGameState();

	//Creates a new Game State.
	void createNewGameState();

	//Pops the Game state in the states list.
	void disconnect();

	//Pops all states and quits the application.
	void quit();
	//PTR TuanNA end comment
	*/
	//PTR TuanNA- Quits on [Escape] and forwards the unhandled input to the GameFramework class... [15:25:14 4/9/2016 by TNA]
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	
	//PTR TuanNA- Forwards the unhandled input to the GameFramework class... [15:26:11 4/9/2016 by TNA]
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
	
	//PTR TuanNA- Injects the mouse movements to the GUI.... [15:26:44 4/9/2016 by TNA]
	bool mouseMoved(const OIS::MouseEvent &evt);
	
	//PTR TuanNA- Injects the mouse clicks to the GUI... [15:27:18 4/9/2016 by TNA]
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
 
	void buttonHit(OgreBites::Button* button);
 
	void update(double timeSinceLastFrame);
 
private:
	bool m_bQuit;
	//PTR TuanNA- GuiMouseOver... [15:29:21 4/9/2016 by TNA]GuiMouseOver<MenuState>*			mvGuiMouseOver;
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||