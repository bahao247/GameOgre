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
| Filename   : SinbadCharacterController.hpp                                                           
---------------------------------------------------------------------------------------
| Written in : C++                                                     
---------------------------------------------------------------------------------------
| Compiler   : Microsoft Visual C++ 2010 Service Pack 1.                               
---------------------------------------------------------------------------------------
| Engine     : Ogre 1.8;...                                                                     
---------------------------------------------------------------------------------------
| Written by : Ba Hao Nguyen – Sports editors, IT.                                
---------------------------------------------------------------------------------------
| Note       : Sinbad Character Controller...                                
---------------------------------------------------------------------------------------
| Create File: 16:00:00 4/7/2016 by TNA                                                             
---------------------------------------------------------------------------------------
| Last Update: 16:10:40 4/9/2016 by TNA                                                             
---------------------------------------------------------------------------------------
*/
#ifndef __Sinbad_H__
#define __Sinbad_H__

#include "Ogre.h"
#include "OIS.h"

using namespace Ogre;

#define NUM_ANIMS 13           //PTR TuanNA- Number of animations the character has... [16:19:43 4/9/2016 by TNA]
#define CHAR_HEIGHT 5          //PTR TuanNA- Height of character's center of mass above ground... [16:19:51 4/9/2016 by TNA]
#define CAM_HEIGHT 2           //PTR TuanNA- Height of camera above character's center of mass... [16:19:59 4/9/2016 by TNA]
#define RUN_SPEED 17           //PTR TuanNA- Character running speed in units per second... [16:20:08 4/9/2016 by TNA]
#define TURN_SPEED 500.0f      //PTR TuanNA- Character turning in degrees per second... [16:20:16 4/9/2016 by TNA]
#define ANIM_FADE_SPEED 7.5f   //PTR TuanNA- Animation crossfade speed in % of full weight per second... [16:20:24 4/9/2016 by TNA]
#define JUMP_ACCEL 30.0f       //PTR TuanNA- Character jump acceleration in upward units per squared second... [16:20:31 4/9/2016 by TNA]
#define GRAVITY 90.0f          //PTR TuanNA- Gravity in downward units per squared second... [16:20:38 4/9/2016 by TNA]

class SinbadCharacterController
{
private:

	/*
	//PTR TuanNA begin comment
	// All the animations our character has, and a null ID...
	//...some of these affect separate body parts and will be blended together.
	//[16:21:52 4/9/2016 by TNA]
	//PTR TuanNA end comment
	*/
	enum AnimID
	{
		ANIM_IDLE_BASE,
		ANIM_IDLE_TOP,
		ANIM_RUN_BASE,
		ANIM_RUN_TOP,
		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,
		ANIM_DRAW_SWORDS,
		ANIM_SLICE_VERTICAL,
		ANIM_SLICE_HORIZONTAL,
		ANIM_DANCE,
		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
		ANIM_NONE
	};

public:
	//PTR TuanNA-  Character will move with camera [16:23:37 4/9/2016 by TNA]
	SinbadCharacterController(Camera* cam);
	
	//PTR TuanNA- Add time... [16:24:25 4/9/2016 by TNA]
	void addTime(Real deltaTime);

	//PTR TuanNA- Key Event... [16:24:55 4/9/2016 by TNA]
	void injectKeyDown(const OIS::KeyEvent& evt);

	void injectKeyUp(const OIS::KeyEvent& evt);
	
	void injectMouseMove(const OIS::MouseEvent& evt);
	
	void injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id);


private:

	void setupBody(SceneManager* sceneMgr);
	
	void setupAnimations();
	
	void setupCamera(Camera* cam);

	void updateBody(Real deltaTime);

	void updateAnimations(Real deltaTime);

	void fadeAnimations(Real deltaTime);

	void updateCamera(Real deltaTime);

	void updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom);

	void setBaseAnimation(AnimID id, bool reset = false);

	void setTopAnimation(AnimID id, bool reset = false);

	Camera* mCamera;
	SceneNode* mBodyNode;
	SceneNode* mCameraPivot;
	SceneNode* mCameraGoal;
	SceneNode* mCameraNode;
	Real mPivotPitch;
	Entity* mBodyEnt;
	Entity* mSword1;
	Entity* mSword2;
	RibbonTrail* mSwordTrail;
	AnimationState* mAnims[NUM_ANIMS];    //PTR TuanNA- Master animation list... [16:25:36 4/9/2016 by TNA]
	AnimID mBaseAnimID;                   //PTR TuanNA- Current base (full- or lower-body) animation... [16:25:44 4/9/2016 by TNA]
	AnimID mTopAnimID;                    //PTR TuanNA- Current top (upper-body) animation... [16:25:54 4/9/2016 by TNA]
	bool mFadingIn[NUM_ANIMS];            //PTR TuanNA- Which animations are fading in... [16:26:02 4/9/2016 by TNA]
	bool mFadingOut[NUM_ANIMS];           //PTR TuanNA- Which animations are fading out... [16:26:10 4/9/2016 by TNA]
	bool mSwordsDrawn;
	Vector3 mKeyDirection;      //PTR TuanNA- Player's local intended direction based on WASD keys... [16:27:38 4/9/2016 by TNA]
	Vector3 mGoalDirection;     //PTR TuanNA- Actual intended direction in world-space... [16:28:21 4/9/2016 by TNA]
	Real mVerticalVelocity;     //PTR TuanNA- For jumping... [16:28:29 4/9/2016 by TNA]
	Real mTimer;                //PTR TuanNA- General timer to see how long animations have been playing... [16:28:57 4/9/2016 by TNA]
};

#endif
