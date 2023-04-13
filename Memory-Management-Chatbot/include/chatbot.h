#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>
#include <iostream>
#include "chatlogic.h"

class GraphNode; // forward declaration

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

    // copy & move assignment operators
    ChatBot &operator=(ChatBot &source)
    {
        std::cout << "ChatBot Copy Assignment: " << &source << " to " << this << "\n";
        if (this == &source) 
            return *this;
        _image = new wxBitmap(*source._image);
        _currentNode = source._currentNode;
        _rootNode = source._rootNode;
        _chatLogic = source._chatLogic;
        _chatLogic->SetChatbotHandle(this);
        source._currentNode = nullptr;
        source._rootNode = nullptr;
        source._chatLogic = nullptr;
        return *this;
    }
    ChatBot &operator=(ChatBot &&source)
    {
        std::cout << "ChatBot Move Assignment: " << &source << " to " << this << "\n";
        if (this == &source)
            return *this;
        _image = source._image;
        _currentNode = source._currentNode;
        _rootNode = source._rootNode;
        _chatLogic = source._chatLogic;
        _chatLogic->SetChatbotHandle(this);
        source._image = NULL;
        source._currentNode = nullptr;
        source._rootNode = nullptr;
        source._chatLogic = nullptr;
        return *this;
    }

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
