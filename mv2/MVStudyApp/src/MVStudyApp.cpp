/*
 *  MVStudyApp.cpp
 *  Hybrid OSG Template
 *
 *  Created by Kenneth Kopecky on 8/15/09.
 *  Copyright 2009 Orange Element Studios. All rights reserved.
 *
 *	Modified by Timothy Honeywell, June/July 2012.
 *
 */

#include "MVStudyApp.h"
#include <osg/Geometry>
#include <osg/Geode>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>

using namespace osg;

void MVStudyApp::init()
{
	getMillis();
	mTotalTime = 0;
	mRoot = new Group;
	mNavigation = new MatrixTransform;
	mRoot->addChild(mNavigation.get());
	mRoot->getOrCreateStateSet()->setMode(GL_NORMALIZE, true);

	mModelGroup = new Group;
	mNavigation->addChild(mModelGroup.get());

	//set up lighting
	LightSource* ls = new LightSource;
	mNavigation->addChild(ls);
	Light* light = ls->getLight();
	light->setDiffuse(Vec4(0.7f, 0.7f, 0.7f, 1.0f));
	light->setSpecular(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	light->setAmbient(Vec4(0.05f, 0.05f, 0.05f, 1.0f));
	light->setPosition(Vec4(100.0f, 100.0f, 100.0f, 0.0f));
	
	//add our scene node to the scenegraph
	mSceneNode = new Group;
	mModelGroup->addChild(mSceneNode);
	
	//set the current scene to a default
	mCurrentScene = new Group;
	mCurrentFrame.sceneName = "default";
	mScenes[mCurrentFrame.sceneName] = mCurrentScene;
	mSceneNode->addChild(mCurrentScene);
	addObjectToScene("data/grass.obj", Vec3(0, 0, -25), 100, 0, -90, 0);

	sceneindex = 0;
	eventindex = 0;
	cycleindex = 0;
	
	for (int i=0; i<18; i++) {
		sceneselect[i] = i;
	}
	std::random_shuffle(&sceneselect[0], &sceneselect[17]);
	
	std::stringstream ss;
	int index_id;
	if(mIsMaster)
	{
		std::fstream fstr;

		fstr.open("logs/ctn.dex", std::fstream::in);
		fstr >> index_id;						// read in old index
		index_id++;							// increment index
		fstr.close();

		fstr.open("logs/ctn.dex", std::fstream::out);
		ss << index_id;						// read in index
		fstr << ss.str();					// write out new index
		fstr.close();
	
		outputTrialData = "logs/trial" + ss.str() + ".dex"; // create new filename
	}
}

void MVStudyApp::writeLogLine(std::string data)
{
	std::fstream dataOut;
	dataOut.open(outputTrialData.c_str(), std::fstream::out | std::fstream::app); // link to trial datafile
	dataOut << data;
	dataOut.close();	
}

void MVStudyApp::loadSceneFile(std::string name)
{
	//verify that the file exists
	std::string foundName = osgDB::findDataFile(name);
	if(foundName == "")
	{
		printf("***Error!  Couldn't find the file %s\n", name.c_str());
		return;
	}
	
	//create a scene node for this if we haven't already
	if(mScenes[name] == NULL)
	{
		mScenes[name] = new Group;
		printf("Creating new scene for %s\n", name.c_str());

	}
	mCurrentScene = mScenes[name];
	mSceneNode->removeChildren(0, mSceneNode->getNumChildren());		//remove all children from the sceneholder node and add the current scene to it
	mCurrentScene->removeChildren(0, mCurrentScene->getNumChildren());	//remove all children from the scene's
	
}

void MVStudyApp::buttonInput(unsigned int button, bool pressed)
{
	printf("button input:  %i, %i\n", button, pressed);
	if(!pressed) return;
	switch(button)
	{
		case 0: advanceScene(); break; // go forward through scene sequence
	}
}

long MVStudyApp::getMillis()
{
	//return mTotalTime * 1000;
	static unsigned long tzero = 0;	//time at which we started the app
	if(tzero == 0)		//this'll be the first time go through this function
	{
		//not threadsafe the first time!
		timeval t;
		gettimeofday(&t, NULL);
		tzero = t.tv_sec;
	}
	timeval time;
	gettimeofday(&time, NULL);
	long millis = ((time.tv_sec-tzero) * 1000) + (time.tv_usec / 1000);
	return millis;
}

std::string MVStudyApp::printMatrix(float* mat)
{
	for(int i = 0;i < 4; i++)
		printf("%.4f		%.4f		%.4f		%.4f\n", mat[i], mat[i+4], mat[i+8], mat[i+12]);
	printf("\n");
	
	std::stringstream ss;
	ss << " " << mat[0] << " " << mat[1] << " " << mat[2] << " " << mat[3] << " "
		<< mat[4]  << " " << mat[5] << " " << mat[6] << " " << mat[7] << " "
		<< mat[8] << " "  << mat[9] << " " << mat[10] << " " << mat[11] << " "
		<< mat[12] << " "  << mat[13] << " " << mat[14] << " " << mat[15];
	return ss.str();
}

void MVStudyApp::triggerInput(unsigned int trigger, bool pulled)
{
	if (!pulled) return;

	eventindex++;
	
	printf("Trigger %i pulled: %i\n", trigger, pulled);
	
	std::stringstream ss;
	ss << eventindex << " tp " << sceneindex << " " << trigger << " " << getMillis()
		<< printMatrix(mCurrentFrame.headPos[trigger].ptr()) << " " << printMatrix(mCurrentFrame.gunPos[trigger].ptr()) << "\n";
	
	writeLogLine(ss.str());
}

void MVStudyApp::advanceScene()
{
	printf("advance scene:  cycleindex:  %i    sceneIndex %i (scene %i)\n", cycleindex, sceneindex, sceneselect[sceneindex]);
	// change target display coordinates if in trial sequence
	if (cycleindex > 0) {
		if(sceneindex < 18)
		{		
			switch (sceneselect[sceneindex]) {
				case 0: tx = -5; ty = 3; tz = -20; break;
				case 1: tx = 0; ty = 3; tz = -20; break;
				case 2: tx = 5; ty = 3; tz = -20; break;
				case 3: tx = -5; ty = 7; tz = -20; break;
				case 4: tx = 0; ty = 7; tz = -20; break;
				case 5: tx = 5; ty = 7; tz = -20; break;
				case 6: tx = -5; ty = 11; tz = -20; break;
				case 7: tx = 0; ty = 11; tz = -20; break;
				case 8: tx = 5; ty = 11; tz = -20; break;
				case 9: tx = -5; ty = 3; tz = -30; break;
				case 10: tx = 0; ty = 3; tz = -30; break;
				case 11: tx = 5; ty = 3; tz = -30; break;
				case 12: tx = -5; ty = 7; tz = -30; break;
				case 13: tx = 0; ty = 7; tz = -30; break;
				case 14: tx = 5; ty = 7; tz = -30; break;
				case 15: tx = -5; ty = 11; tz = -30; break;
				case 16: tx = 0; ty = 11; tz = -30; break;
				case 17: tx = 5; ty = 11; tz = -30; break;
				default: tx = 0; ty = -20; tz = -25; break; // plain scene
			}
		}		
		else
		{
			tx = 0; ty = -20; tz = -25;
		}
		printf("Target placed at %i, %i, %i\n", tx, ty, tz);
		sceneindex++;
		// if all targets in a cycle have been completed, reset the target counter and move to the next cycle
		if (sceneindex > 18) {
			printf("CYCLE COMPLETE\n");
			sceneindex = 0;
			cycleindex++;
			if (cycleindex > 5) exit(0);
			// reset sceneselect array
			std::random_shuffle(&sceneselect[0], &sceneselect[17]);
		}
	}
	// change target display coordinates if in test sequence
	else {
		printf("test sequence cycleindex: %i\n", cycleindex);
		switch (sceneindex) {
			case 0: tx = 0; ty = 7; tz = -25; break; // target at dead center
			case 1: tx = 0; ty = -20; tz = -25; break; // plain scene
			default: tx = 0; ty = -20; tz = -25; break; // plain scene
		}
		sceneindex++;
		printf("sceneindex incremented\n");
		if (sceneindex > 1) {
			printf("CYCLE COMPLETE\n");
			sceneindex = 0;
			printf("sceneindex reset\n");
			cycleindex++;
			printf("cycleindex incremented\n");
		}
	}
	
	eventindex++;
	std::stringstream ss;
	ss << eventindex << " tc " << sceneindex << " " << getMillis() << " " << tx << " " << ty << " " << tz << "\n";
	writeLogLine(ss.str());
	pushTarget();
}

void MVStudyApp::update(float dt)
{
	//printf("millis:  %i (%3f)\n", getMillis(), dt);
	mTotalTime += dt;
	//process navigation, animation, etc
}

//transform the head and wand matrix into navigation space (optional, depends on how you're using them)
void MVStudyApp::setSharedHeadMatrix(Matrixf mat)
{
	mHeadMatrix = mat*mNavigation->getInverseMatrix();
	mCurrentFrame.sharedHead = mat;
//	mCurrentFrame.sharedHead.ptr()[1-16];
}

osg::Node* MVStudyApp::addObjectToScene(std::string name, Vec3 pos, float scale, float rotY, float rotX, float rotZ)
{
	//make a scene node if we don't have one yet
//	printf("adding object %s...\n", name.c_str());
	if(!mCurrentScene)
	{
		printf("***Error!  Adding an object to no scene! ):\n");
//		return;
	}
	
	//use OSGDB to find a path for the file
	std::string foundName = osgDB::findDataFile(name);
	if(foundName == "")
	{
		printf("***Error!  Couldn't find the file %s\n", name.c_str());
//		return;
	}
	
	Node* loaded = osgDB::readNodeFile(foundName);
	if(!loaded)
	{
		printf("***Error!  There was a problem reading the file %s\n", foundName.c_str());
//		return;
	}
//	osgDB::writeNodeFile(*loaded, "Loaded.ive");
	MatrixTransform* mt = new MatrixTransform;
	mt->addChild(loaded);
	
	//build the transform for the object based on the numbers we put in
	Matrix mat;
	Matrix trans = Matrix::translate(pos);
	Matrix scaleMat = Matrix::scale(scale, scale, scale);
	Matrix yRot = Matrix::rotate(osg::DegreesToRadians(rotY), Vec3(0, 1, 0));
	Matrix xRot = Matrix::rotate(osg::DegreesToRadians(rotX), Vec3(1, 0, 0));
	Matrix zRot = Matrix::rotate(osg::DegreesToRadians(rotZ), Vec3(0, 0, 1));	
	
	mat = scaleMat * zRot * xRot * yRot * trans;
	mt->setMatrix(mat);
	mCurrentScene->addChild(mt);
	return mt;
}

void MVStudyApp::pushTarget()
{
	mCurrentScene->removeChild(currTrgtMT);
	newTrgtMT = addObjectToScene("data/target.obj", Vec3(tx, ty, tz), 0.25, 0, -90, 90);
	currTrgtMT = newTrgtMT;
	mCurrentScene->removeChild(currStndMT);
	newStndMT = addObjectToScene("data/treetrunk.obj", Vec3(tx, ty-15, tz-1), 2, 0, -90, 0);
	currStndMT = newStndMT;
}

