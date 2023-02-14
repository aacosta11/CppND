#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>
#include <iostream>

class GraphNode; // forward declaration
class ChatLogic; // forward declaration

/*
    ChatBot instances are copied with EXCLUSIVE OWNERSHIP semantics. 
    you have been warned.
*/
class ChatBot
{
private:
    // data handles (owned)
    wxBitmap *_image; // avatar image

    // data handles (not owned)
    GraphNode *_currentNode;
    GraphNode *_rootNode;
    ChatLogic *_chatLogic;

    // proprietary functions
    int ComputeLevenshteinDistance(std::string s1, std::string s2);

public:
    // constructors / destructors
    ChatBot();
    ChatBot(std::string filename);
    ~ChatBot();

    // copy & move constructors
    ChatBot(ChatBot &source);
    ChatBot(ChatBot &&source);

    ChatBot &operator=(ChatBot &source) // copy assignment operator
    {
        std::cout << "ChatBot Copy Assignment: " << &source << " to " << this << "\n";
        if (this == &source) 
            return *this;
        // copy source handles
        _image = source._image;
        _currentNode = source._currentNode;
        _rootNode = source._rootNode;
        _chatLogic = source._chatLogic;
        // invalidate source handles
        source._image = NULL;
        source._currentNode = nullptr;
        source._rootNode = nullptr;
        source._chatLogic = nullptr;
        // return instance
        return *this;
    } // EOF copy assignemtn operator

    ChatBot &operator=(ChatBot &&source) // move assignment operator
    {
        std::cout << "ChatBot Move Assignment: " << &source << " to " << this << "\n";
        if (this == &source)
            return *this;
        // copy source handles
        _image = source._image;
        _currentNode = source._currentNode;
        _rootNode = source._rootNode;
        _chatLogic = source._chatLogic;
        // invalidate source handles
        source._image = NULL;
        source._currentNode = nullptr;
        source._rootNode = nullptr;
        source._chatLogic = nullptr;
        // return instance
        return *this;
    } // EOF move assingment operator

    // getters / setters
    void SetCurrentNode(GraphNode *node);
    void SetRootNode(GraphNode *rootNode) { _rootNode = rootNode; }
    void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }
    ChatLogic* GetChatLogicHandle() { return _chatLogic; }
    wxBitmap *GetImageHandle() { return _image; }

    // communication
    void ReceiveMessageFromUser(std::string message);
};

#endif /* CHATBOT_H_ */
