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
| Filename   : SinbadState.hpp                                                           
---------------------------------------------------------------------------------------
| Written in : C++                                                     
---------------------------------------------------------------------------------------
| Compiler   : Microsoft Visual C++ 2010 Service Pack 1.                               
---------------------------------------------------------------------------------------
| Engine     : Ogre 1.8;...                                                                     
---------------------------------------------------------------------------------------
| Written by : Ba Hao Nguyen – Sports editors, IT.                                
---------------------------------------------------------------------------------------
| Note       : Sinbad State...                                
---------------------------------------------------------------------------------------
| Create File: 15:00:00 4/7/2016 by TNA                                                             
---------------------------------------------------------------------------------------
| Last Update: 15:44:23 4/9/2016 by TNA                                                             
---------------------------------------------------------------------------------------
*/ 
#ifndef SINBAD_STATE_HPP
#define SINBAD_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "AppState.hpp"
#include "SinbadCharacterController.hpp" 
#include "DotSceneLoader.hpp"
#include "SdkCameraMan.h" 
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>
#include <OgreRenderTexture.h>
#include <OgreRenderTargetListener.h>
#include <OgreHardwarePixelBuffer.h>
//|||||||||||||||||||||||||||||||||||||||||||||||
  
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class SinbadState : public AppState,
	public Ogre::FrameListener,
	public Ogre::RenderTargetListener
{
public:
	SinbadState();
 
	DECLARE_APPSTATE_CLASS(SinbadState)
	
	/*
	//PTR TuanNA begin comment
	//Every time we enter a GameState we do the following:
		1. Create SceneManager
		2. Create a Camera
		3. Set input callbacks
		4. Load Sounds
		5. Create a scene....
		-  [16:01:01 4/9/2016 by TNA]
	
	//PTR TuanNA end comment
	*/
	void enter();

	/*
	//PTR TuanNA begin comment
	//In createScene we do the following:
		1. Initiate Physics.
		2. Set SkyBox, Shadows and Directional Light.
		3. Create Motion Blur Effects.
		4. Create the grid for the Chunks class.
		5. Create the terrain.
		6. Create the Player.
		7. Create the Camera.
		8. Create PowerUps
		9. Add NPCs
		10. Add concent to the scene(Dynamic and static objects)...
		-  [16:01:27 4/9/2016 by TNA]
	//PTR TuanNA end comment
	*/
	void createScene();

	//PTR TuanNA- Delete Physics, Chunk system, Unload GameState sound and finally destroys Camera and Scene Manager.... [16:03:17 4/9/2016 by TNA]
	void exit();

	//PTR TuanNA- Pause the Game State (Physics, AI etc) and hides the Game GUI.... [16:03:28 4/9/2016 by TNA]
	bool pause();

	//PTR TuanNA- Resumes the earlier paused Game State. Shows the Game GUI.... [16:03:37 4/9/2016 by TNA]
	void resume();
	
	//PTR TuanNA- Build GUI.... [16:03:53 4/9/2016 by TNA]
	void buildGUI();
	
	//PTR TuanNA- Every time a key is pressed, this function is called. Good for activating new PowerUps.... [16:04:21 4/9/2016 by TNA]
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	
	//PTR TuanNA- Every time a key is released, this function is called. Good for resetting move direction.... [16:04:36 4/9/2016 by TNA]
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
	
	/*
	//PTR TuanNA begin comment
	//This function is called every time the mouse moves. All camera modes has their own way
	of defining a rotation. The CameraHandler class takes care of this....
	//[16:05:00 4/9/2016 by TNA]
	//PTR TuanNA end comment
	*/
	bool mouseMoved(const OIS::MouseEvent &arg);
	
	//PTR TuanNA- When the mouse is pressed, this function is called. Good for start shooting or creating joints in the MoveBox mode.... [16:07:23 4/9/2016 by TNA]
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	
	//PTR TuanNA- See mousePressed. Just de-activates what mousePressed activates.... [16:07:44 4/9/2016 by TNA]
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	
	//PTR TuanNA- Item select... [16:08:36 4/9/2016 by TNA]
	void itemSelected(OgreBites::SelectMenu* menu);
	
	//PTR TuanNA- This updates the whole game every frame. Called from the Game Loop.... [16:08:16 4/9/2016 by TNA]
	void update(double timeSinceLastFrame);
 
private:
	//PTR TuanNA- sample details panel... [16:09:16 4/9/2016 by TNA]
	OgreBites::ParamsPanel*		m_pDetailsPanel;   		
	bool						m_bQuit;
	bool						m_pCursorWasVisible;		//PTR TuanNA- Was cursor visible before dialog appeared... [16:09:32 4/9/2016 by TNA]
	bool						m_pDragLook;              //PTR TuanNA- Click and drag to free-look... [16:09:42 4/9/2016 by TNA] 
	OgreBites::SdkCameraMan*	m_pCameraMan;
	SinbadCharacterController*	m_pChara;
	Ogre::NameValuePairList		mInfo;    //PTR TuanNA- Custom sample info... [16:09:49 4/9/2016 by TNA]
	//PTR TuanNA [mMiniscreen- 7/9/2016]
	virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& rte);
	virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& rte);

	void createGrassMesh(); //PTR TuanNA [Add func createGrassMesh()- 7/9/2016]
	Ogre::Rectangle2D* mMiniscreen;//PTR TuanNA [mMiniscreen- 7/9/2016]
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||