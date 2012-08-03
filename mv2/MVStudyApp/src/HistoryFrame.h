//
//  HistoryFrame.h
//  MVStudyApp
//
//  Created by Ken Kopecky on 6/18/12.
//

#ifndef MVStudyApp_HistoryFrame_h
#define MVStudyApp_HistoryFrame_h

#include <osg/Matrixf>
#include <vector>

//a history frame records all the useful study data for one instant.  Feel free to add to it


class TiXMLElement;		//if we're using TinyXML for XML input/output, this will give us a predeclaration of the data type we  need

enum ButtonState {OFF, ON, TOGGLE_OFF, TOGGLE_ON};			//the four possible states of your standard digital button
enum StudyMode {SINGLE_USER, TWO_USERS, THREE_USERS};		//what are we currently testing?
//5 should be plenty, right?
#define MAX_PARTICIPANTS 5

class HistoryFrame
{
public:
	HistoryFrame()
	{
		for(int i = 0; i < MAX_PARTICIPANTS; i++)	buttonState[i] = 0;
		frameNum = 1;
		frameTime = 0;
		studyMode = SINGLE_USER;
	}
	
	TiXMLElement* toXML()		{return NULL;}				//TODO:  fill this in.
	//all our data
	int frameNum;
	float frameTime;
	osg::Matrixf gunPos[MAX_PARTICIPANTS];
	osg::Matrixf headPos[MAX_PARTICIPANTS];
	osg::Matrixf sharedHead;
	bool buttonState[MAX_PARTICIPANTS];
	std::string sceneName;
	StudyMode studyMode;

};

#endif
