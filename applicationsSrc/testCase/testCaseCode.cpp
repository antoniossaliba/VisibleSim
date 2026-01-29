#include "testCaseCode.hpp"

TestCaseCode::TestCaseCode(SlidingCubesBlock *host):SlidingCubesBlockCode(host),module(host) {
    // @warning Do not remove block below, as a blockcode with a NULL host might be created
    //  for command line parsing
    if (not host) return;

    // Registers a callback (mySendDistanceFunc) to the message of type E
    addMessageEventFunc2(SENDDISTANCE_MSG_ID,
                       std::bind(&TestCaseCode::mySendDistanceFunc,this,
                       std::placeholders::_1, std::placeholders::_2));

    // Registers a callback (myAnswerDistanceFunc) to the message of type S
    addMessageEventFunc2(ANSWERDISTANCE_MSG_ID,
                       std::bind(&TestCaseCode::myAnswerDistanceFunc,this,
                       std::placeholders::_1, std::placeholders::_2));

    // Registers a callback (myAskDistanceFunc) to the message of type D
    addMessageEventFunc2(ASKDISTANCE_MSG_ID,
                       std::bind(&TestCaseCode::myAskDistanceFunc,this,
                       std::placeholders::_1, std::placeholders::_2));

    // Registers a callback (myOnPathFunc) to the message of type T
    addMessageEventFunc2(ONPATH_MSG_ID,
                       std::bind(&TestCaseCode::myOnPathFunc,this,
                       std::placeholders::_1, std::placeholders::_2));

}

void TestCaseCode::startup() {
    console << "start " << module->blockId << "\n";
    if (module->blockId == 1) { // Master id is 1
        module->setColor(RED);
        distance = 0;
        sendMessageToAllNeighbors("Send Distance",new MessageOf<int>(SENDDISTANCE_MSG_ID,distance),100,200,0);
    }
    else if (isTarget == true)
    {
        module->setColor(GREEN);
    }
    else if(isWall == true)
    {
        module->setColor(BLACK);
    }
    else
    {
        distance = -1;
        module->setColor(LIGHTGREY);
    }
}
void TestCaseCode::mySendDistanceFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {

    MessageOf<int>* msg = static_cast<MessageOf<int>*>(_msg.get());
    int d = *msg->getData();
    if(distance == -1 && isWall == false)
    {
        distance = d+1;
        sendMessageToAllNeighbors("Distance",new MessageOf<int>(SENDDISTANCE_MSG_ID,distance),100,200,0);
    }
    if(isTarget == true)
    {
        distance = d+1;
        sendMessageToAllNeighbors("Ask for Distance",new MessageOf<int>(ASKDISTANCE_MSG_ID,distance),100,200,0);
    }

};

void TestCaseCode::myAnswerDistanceFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {

    MessageOf<int>* msg = static_cast<MessageOf<int>*>(_msg.get());
    int d = *msg->getData();
    if(d == distance-1 && targetSent == false && isWall == false)
    {
        targetSent = true;
        sendMessage("On path",new Message(ONPATH_MSG_ID),sender,0,0);
    }

};

void TestCaseCode::myAskDistanceFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    sendMessage("Answer Distance",new MessageOf<int>(ANSWERDISTANCE_MSG_ID,distance),sender,0,0);

};

void TestCaseCode::myOnPathFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    if(module->blockId != 1 && isWall == false)
    {
        module->setColor(BLUE);
    }
    sendMessageToAllNeighbors("Ask for Distance",new MessageOf<int>(ASKDISTANCE_MSG_ID,distance),100,200,0);

};

void TestCaseCode::parseUserBlockElements(TiXmlElement *blockElt)
{
    blockElt->QueryBoolAttribute("isTarget", &isTarget);
    blockElt->QueryBoolAttribute("isWall", &isWall);
    // (pour parsing cf.  simulatorCore/src/base/simulator.cpp)
}

void TestCaseCode::processLocalEvent(std::shared_ptr<Event> pev) {
    std::shared_ptr<Message> message;
    stringstream info;

    // Do not remove line below
    SlidingCubesBlockCode::processLocalEvent(pev);

    switch (pev->eventType) {
        case EVENT_ADD_NEIGHBOR: {
            // Do something when a neighbor is added to an interface of the module
            break;
        }

        case EVENT_REMOVE_NEIGHBOR: {
            // Do something when a neighbor is removed from an interface of the module
            break;
        }

        case EVENT_INTERRUPTION: {
            // Do something when the module receives an event
            break;
        }
    break;
    }
};
