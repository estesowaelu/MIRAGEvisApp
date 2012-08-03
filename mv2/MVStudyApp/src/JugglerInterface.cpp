#include "JugglerInterface.h"
#include "MVStudyApp.h"


JugglerInterface::JugglerInterface(vrj::Kernel* kern, int& argc, char** argv) : vrj::OsgApp(kern)
{
}

JugglerInterface::~JugglerInterface()
{
}

void JugglerInterface::init()
{
	//link our position proxies to the names they're connected to in Juggler
	//each of the supplied names must match something in one of our config files
	mSharedHead.init("HelmetABQ");		//this one is our virtual head position	
	
	//these three are the actual tracked heads
	mHeadPos[0].init("HelmetA");
	mHeadPos[1].init("HelmetB");
	mHeadPos[2].init("HelmetC");
	
	//and actual tracked guns
	mGunPos[0].init("WeaponA");
	mGunPos[1].init("WeaponB");
	
	//hook up our gun triggers now
	mTrigger[0].init("Trigger0");
	mTrigger[1].init("Trigger1");
	
	mButton[0].init("VJButton0");
	mButton[1].init("VJButton1");
	
	vrj::OsgApp::init();
}

void JugglerInterface::initScene()
{
	printf("______initing scene_____\n");
	 MVStudyApp::instance().init();
	 _rootNode = MVStudyApp::instance().getRoot();
}

void printMatrix(const float* mat)
{
	for(int i = 0;i < 4; i++)
		printf("%.4f		%.4f		%.4f		%.4f\n", mat[i], mat[i+4], mat[i+8], mat[i+12]);
	printf("\n");
	
}



void JugglerInterface::preFrame()
{	
	//trigger handling
	
	//if(mButton1->getData() == gadget::Digital::TOGGLE_ON)
	//ShaderManager::instance().reloadShaders();
	
	//printf("preframe end\n");
}

void JugglerInterface::latePreFrame()
{
	//printf("preframe begin\n");
	static double tLast = mSharedHead->getTimeStamp().secd();
	double tNow = mSharedHead->getTimeStamp().secd();
	double dt = tNow - tLast;
	tLast = tNow;
	
	//pass changes in button state on to the app
	for(int i = 0; i < 2; i++)
	{
		if(mButton[i]->getData() == gadget::Digital::TOGGLE_ON)
			MVStudyApp::instance().buttonInput(i, true);
		else if(mButton[i]->getData() == gadget::Digital::TOGGLE_OFF)
			MVStudyApp::instance().buttonInput(i, false);

		//printf("button state %i:  %i\n", i, mButton[i]->getData());
	}
	
	//pass changes in trigger state on to the app
	for(int i = 0; i < 2; i++)
	{
		if(mTrigger[i]->getData() == gadget::Digital::TOGGLE_ON)
			MVStudyApp::instance().triggerInput(i, true);
		else if(mTrigger[i]->getData() == gadget::Digital::TOGGLE_OFF)
			MVStudyApp::instance().triggerInput(i, false);
	//	if(mTrigger[i]->getData())
	//		printf("Gun %i fire!\n", i);
		//printf("trigger state %i: %i\n", i, mTrigger[i]->getData());
	}

	MVStudyApp::instance().update(dt);
	//attach the wand's matrix to the paint sprayer

	osg::Matrixf headMatrix	(mSharedHead->getData().mData);
	MVStudyApp::instance().setSharedHeadMatrix(headMatrix);
	for(int i = 0; i < 2; i++)
	{
		MVStudyApp::instance().setHeadMatrix(i, osg::Matrixf(mHeadPos[i]->getData().mData));
		MVStudyApp::instance().setGunMatrix(i, osg::Matrixf(mGunPos[i]->getData().mData));
	}
	
//	printf("HelmetA\n");
//	printMatrix(mHeadPos[0]->getData().mData);
}

void JugglerInterface::bufferPreDraw()
{
	glClearColor(0.5, 0.5, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	const osg::FrameStamp* frameStamp = (*sceneViewer)->getFrameStamp();
}

osg::Group* JugglerInterface::getScene()
{
	return _rootNode.get();
}

// Override OsgApp::configSceneView to add FrameStamp to SceneView.
// This helps animations and shaders work properly in OSG.
void JugglerInterface::configSceneView(osgUtil::SceneView* newSceneViewer)
{
	vrj::OsgApp::configSceneView(newSceneViewer);
	newSceneViewer->setClearColor(osg::Vec4(0.7f, 0.7f, 0.8f, 0.0f));
	return;
	newSceneViewer->setDefaults();
	 
	// Needed for stereo to work.
//	 newSceneViewer->setDrawBufferValue(GL_NONE);

	 _frameStamp = new osg::FrameStamp;
	 newSceneViewer->setFrameStamp(_frameStamp.get());
}

void JugglerInterface::draw()
{
	//printf("Begin draw\n");
	vrj::OsgApp::draw();
	//printf("end draw\n");
}
