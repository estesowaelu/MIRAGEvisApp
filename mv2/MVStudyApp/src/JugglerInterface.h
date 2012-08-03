#ifndef DP3DJUGGLERH
#define DP3DJUGGLERH

#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Draw/OSG/OsgApp.h>


// OSG
#include <osg/Matrix>
#include <osg/Transform>
#include <osg/MatrixTransform>
#include "HistoryFrame.h"


class JugglerInterface : public vrj::OsgApp
{
  public:
    JugglerInterface(vrj::Kernel* kern, int & argc, char** argv);
    virtual ~JugglerInterface();

    virtual void init();
    virtual void initScene();
    virtual void preFrame();
    virtual void latePreFrame();
	virtual void draw();
    void bufferPreDraw();

    virtual osg::Group* getScene();

	void configSceneView(osgUtil::SceneView* newSceneViewer);
	void setStudyMode(StudyMode s);		//set to single, two, or three users
  private:
  
  //OSG stuff
	osg::ref_ptr<osg::Group> _rootNode;
	osg::ref_ptr<osg::FrameStamp> _frameStamp;
		
	//device interfaces for Juggler
	gadget::PositionInterface  mSharedHead;			//the head position for which our views are being drawn
	gadget::PositionInterface  mHeadPos[3];		//actual head positions
	gadget::PositionInterface  mGunPos[2];			//gun positions
	gadget::DigitalInterface   mTrigger[2];			//gun triggers
	gadget::DigitalInterface   mButton[2];			//interface for digital buttons (ie gamepad)

};

#endif
