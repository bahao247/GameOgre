//|||||||||||||||||||||||||||||||||||||||||||||||
 
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
 
	void enter();
	void createScene();
	void exit();
	bool pause();
	void resume();
 
	void buildGUI();
 
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
 
	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
 
	void itemSelected(OgreBites::SelectMenu* menu);
 
	void update(double timeSinceLastFrame);
 
private:
	OgreBites::ParamsPanel*		m_pDetailsPanel;   		// sample details panel
	bool						m_bQuit;
	bool						m_pCursorWasVisible;		// was cursor visible before dialog appeared
	bool						m_pDragLook;              // click and drag to free-look
	OgreBites::SdkCameraMan*	m_pCameraMan;
	SinbadCharacterController*	m_pChara;
	Ogre::NameValuePairList		mInfo;    // custom sample info

	//PTR TuanNA [mMiniscreen- 7/9/2016]
	virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& rte);
	virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& rte);

	void createGrassMesh(); //PTR TuanNA [Add func createGrassMesh()- 7/9/2016]
	Ogre::Rectangle2D* mMiniscreen;//PTR TuanNA [mMiniscreen- 7/9/2016]
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||