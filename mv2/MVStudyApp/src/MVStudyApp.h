/*
 *  MVStudyApp.h
 *  Hybrid OSG Template
 *
 *  Created by Kenneth Kopecky on 8/15/09.
 *  Copyright 2009 Orange Element Studios. All rights reserved.
 *
 *	Modified by Timothy Honeywell, June/July 2012.
 *
 */

#ifndef MVStudyAppCUZKENLOVESYOU
#define MVStudyAppCUZKENLOVESYOU
#include <osg/Group>
#include <osg/Geode>
#include <osg/LightSource>
#include <osg/MatrixTransform>
#include <map>
#include <HistoryFrame.h>
#include <ctime>
#include <sys/time.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class MVStudyApp
{
public:

	void init();

	void update(float dt);										//timestep update.  For animations and stuff

	osg::Group* getRoot() {return mRoot.get();}					//get our root scene node
	osg::Group* getModelGroup()	{return mModelGroup.get();}

	void setHeadMatrix(int which, osg::Matrixf m){mCurrentFrame.headPos[which] = m;}				//set the actual head position of a user
	void setSharedHeadMatrix(osg::Matrixf m);					//set the head matrix Juggler is using to draw its views
	void setGunMatrix(int which, osg::Matrixf m) {mCurrentFrame.gunPos[which] = m;}				//set the position of one of the user's guns
	
	void buttonInput(unsigned int button, bool pressed);
	void triggerInput(unsigned int trigger, bool pulled);
	void advanceScene();											//go to the next scene in our list
	void pushTarget();
	long getMillis();
	std::string printMatrix(float* mat);

	static MVStudyApp& instance()	{static MVStudyApp a;  return a;}				//singleton instance
	
	//add a 3d model to the current scene.  Rotation angles are in degrees.  pos is in feet
	osg::Node* addObjectToScene(std::string name, osg::Vec3 pos, float scale = 1.0, float rotY = 0, float rotX = 0, float rotZ = 0);
	
	void loadSceneFile(std::string name);						//load a scene file.  if this file was already loaded, it will clear its objects and load them again
	void setStudyMode(StudyMode m);								//set how many heads we're messing with

	void writeLogLine(std::string data);
	void setIsMaster(bool b)	{mIsMaster = b;}

	std::string outputTrialData;				// placeholder for new trial file filename

protected:

	MVStudyApp()	{mIsMaster = true;}
	bool mIsMaster;				//is this the master node on the cluster?

	int tx;
	int ty;
	int tz;
	
	int eventindex;
	int sceneindex;
	int cycleindex;
	int sceneselect [18];
	
	osg::ref_ptr<osg::Group> mRoot;								//root node of our scenegraph
	osg::ref_ptr<osg::MatrixTransform> mNavigation;				//navigation matrix.  for moving the scene around
	osg::ref_ptr<osg::Group> mModelGroup;						//this is where we put things into the app!

	osg::Group* mSceneNode;										//holds the background loaded scene
	osg::Group* mCurrentScene;									//the scene we're using at the moment
	
	osg::Node* currTrgtMT;
	osg::Node* newTrgtMT;
	osg::Node* currStndMT;
	osg::Node* newStndMT;
	
	osg::Matrixf mHeadMatrix;			//this matrix represents the position for which Juggler is drawing its view
	std::map<std::string, osg::ref_ptr<osg::Group> >		mScenes;		//our collection of scenes
	
	float mTotalTime;
	
	//recorded data
	HistoryFrame mCurrentFrame;
	std::vector<HistoryFrame> mRecordedFrames;

};

#endif

