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
| Filename   : GameState.hpp                                                           
---------------------------------------------------------------------------------------
| Written in : C++                                                     
---------------------------------------------------------------------------------------
| Compiler   : Microsoft Visual C++ 2010 Service Pack 1.                               
---------------------------------------------------------------------------------------
| Engine     : Ogre 1.8;...                                                                     
---------------------------------------------------------------------------------------
| Written by : Ba Hao Nguyen – Sports editors, IT.                                
---------------------------------------------------------------------------------------
| Note       : Game State...                                
---------------------------------------------------------------------------------------
| Create File: 14:00:00 4/7/2016 by TNA                                                             
---------------------------------------------------------------------------------------
| Last Update: 14:25:05 4/9/2016 by TNA                                                             
---------------------------------------------------------------------------------------
*/ 
#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "AppState.hpp"
#include "SinbadCharacterController.hpp" 
#include "DotSceneLoader.hpp"
 
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>
/*
//PTR TuanNA begin comment
//Includes- [14:33:40 4/9/2016 by TNA]
#include "CameraHandler.h"
#include "Player.h"
#include "PowerUp.h"
#include "WorldManager.h"
#include "PhysicsManager.h"
#include "SceneLoader.h"
#include "NPCAimer.h"
#include "NPCHandler.h"
#include <stdlib.h>
#include <time.h>

#include "GameCommon.h"...
//PTR TuanNA end comment
*/ 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
enum QueryFlags
{
    OGRE_HEAD_MASK	= 1<<0,
    CUBE_MASK		= 1<<1
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class GameState : public AppState
{
public:
	GameState();
 
	DECLARE_APPSTATE_CLASS(GameState)
 
	/*
	//PTR TuanNA begin comment
	//Every time we enter a GameState we do the following:
		1. Create SceneManager
		2. Create a Camera
		3. Set input callbacks
		4. Load Sounds
		5. Create a scene....
		-  [14:34:21 4/9/2016 by TNA]
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
		-  [14:35:31 4/9/2016 by TNA]
	//PTR TuanNA end comment
	*/
	void createScene();

	//PTR TuanNA- Delete Physics, Chunk system, Unload GameState sound and finally destroys Camera and Scene Manager.... [14:36:16 4/9/2016 by TNA]
	void exit();

	//PTR TuanNA- Pause the Game State (Physics, AI etc) and hides the Game GUI.... [14:36:32 4/9/2016 by TNA]
	bool pause();
	
	//PTR TuanNA- Resumes the earlier paused Game State. Shows the Game GUI.... [14:36:48 4/9/2016 by TNA]
	void resume();
 
	void moveCamera();
	
	//PTR TuanNA- In every frame we check for input from the keyboard here.... [14:37:22 4/9/2016 by TNA]
	void getInput();
    
	//PTR TuanNA- Build GUI... [14:54:51 4/9/2016 by TNA]
	void buildGUI();
	
	//PTR TuanNA- Every time a key is pressed, this function is called. Good for activating new PowerUps.... [14:37:59 4/9/2016 by TNA]
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	
	//PTR TuanNA- Every time a key is released, this function is called. Good for resetting move direction.... [14:38:35 4/9/2016 by TNA]
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
	
	/*
	//PTR TuanNA begin comment
	//This function is called every time the mouse moves. 
	//All camera modes has their own way of defining a rotation. 
	The CameraHandler class takes care of this....-  [14:39:00 4/9/2016 by TNA]
	//PTR TuanNA end comment
	*/
	bool mouseMoved(const OIS::MouseEvent &arg);
	
	//PTR TuanNA- When the mouse is pressed, this function is called. Good for start shooting or creating joints in the MoveBox mode.... [14:40:39 4/9/2016 by TNA]
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	
	//PTR TuanNA- See mousePressed. Just de-activates what mousePressed activates. ... [14:44:28 4/9/2016 by TNA]
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	
	//PTR TuanNA- Select item [14:50:23 4/9/2016 by TNA]
	void onLeftPressed(const OIS::MouseEvent &evt);
    void itemSelected(OgreBites::SelectMenu* menu);
	
	//PTR TuanNA- This updates the whole game every frame. Called from the Game Loop.... [14:50:02 4/9/2016 by TNA]
	void update(double timeSinceLastFrame);
 
private:

	/*
	//PTR TuanNA begin comment
	//Create Motion Blur effects. Taken from the OGRE samples. -  [14:53:11 4/9/2016 by TNA]
		void createMotionBlurEffects();
	
		bool                                    mvQuit,mvRightButton;
		PhysicsManager*							mvpPhysicsMgr;
		Player*									mvpPlayer;
		PowerUp*								mvpPowerUps;
		WorldManager*							mvpWorld;
	
	//PTR TuanNA end comment
	*/
	Ogre::SceneNode*		m_pOgreHeadNode;
	Ogre::Entity*			m_pOgreHeadEntity;
	Ogre::MaterialPtr		m_pOgreHeadMat;
	Ogre::MaterialPtr		m_pOgreHeadMatHigh;
 
    OgreBites::ParamsPanel*		m_pDetailsPanel;
	bool				m_bQuit;
 
	Ogre::Vector3			m_TranslateVector;
	Ogre::Real			m_MoveSpeed;
	Ogre::Degree			m_RotateSpeed;
	float				m_MoveScale;
	Ogre::Degree			m_RotScale;
 
	Ogre::RaySceneQuery*		m_pRSQ;
	Ogre::SceneNode*		m_pCurrentObject;
	Ogre::Entity*			m_pCurrentEntity;
	bool				m_bLMouseDown, m_bRMouseDown;
	bool				m_bSettingsMode;
	SinbadCharacterController*	m_pChara;
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||