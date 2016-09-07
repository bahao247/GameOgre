//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef PAUSE_STATE_HPP
#define PAUSE_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "AppState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class PauseState : public AppState
{
public:
    PauseState();
	//PTR TuanNA- This line calls the macro defined in AppState.hpp, making this class a valid application state... [15:07:58 4/9/2016 by TNA]
    DECLARE_APPSTATE_CLASS(PauseState)
	
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

	//PTR TuanNA- createScene... [15:32:46 4/9/2016 by TNA]
    void createScene();

	//PTR TuanNA- Exits the Menu State. Called when starting a new Game State and therefore a loading screen will be shown.... [15:33:01 4/9/2016 by TNA]
    void exit();
	
	//PTR TuanNA- Quits on [Escape] and forwards the unhandled input to the GameFramework class... [15:33:23 4/9/2016 by TNA]
    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    
	//PTR TuanNA- Forwards the unhandled input to the GameFramework class... [15:33:37 4/9/2016 by TNA]
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
	
	//PTR TuanNA- Injects the mouse clicks to the GUI... [15:33:50 4/9/2016 by TNA]
    bool mouseMoved(const OIS::MouseEvent &evt);
    
	//PTR TuanNA- Injects the mouse clicks to the GUI... [15:34:06 4/9/2016 by TNA]
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
 
	//PTR TuanNA- Button hit... [15:34:55 4/9/2016 by TNA]
    void buttonHit(OgreBites::Button* button);
    
	//PTR TuanNA- Question Dialog Closed... [15:34:44 4/9/2016 by TNA]
	void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);
	
	//PTR TuanNA- Update... [15:35:33 4/9/2016 by TNA]
    void update(double timeSinceLastFrame);
 
private:
    bool m_bQuit;
    bool m_bQuestionActive;
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||