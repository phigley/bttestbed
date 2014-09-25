//
//  main.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/28/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "sdlapplication.h"
#include "world.h"
#include "ai/npc.h"
#include "ai/state.h"

#include "rapidxml/rapidxml.hpp"

#include <iostream>
#include <memory>

class XmlDocument
{
public:

    XmlDocument(const char* fileName)
    {
        FILE* file = fopen(fileName, "r");
        assert(file);
        if (file)
        {
            fseek(file, 0, SEEK_END);
            const std::size_t fileSize = ftell(file);
            
            xmlData = new char[fileSize + 1];
            rewind(file);
            
            fread(xmlData, 1, fileSize, file);
            fclose(file);

            document.parse<0>(xmlData);
        }
    }

    ~XmlDocument()
    {
        delete[] xmlData;
    }
    
    rapidxml::xml_document<>& getDocument() { return document; }
    
private:

    char* xmlData = nullptr;
    rapidxml::xml_document<> document;
};

int main(int argc, const char * argv[])
{
	SdlApplication app{800, 800};

    XmlDocument xmlDocument("bts/test.xml");

    auto rootNode = xmlDocument.getDocument().first_node();
    assert(rootNode != nullptr);
    if( rootNode )
    {
        assert(0 == strcmp(rootNode->name(), "root"));
    
        app.getWorld().addEntity<AI::NPC>(*rootNode);
    }
    
    while( app.beginFrame() )
    {
        app.render();
    }

    return 0;
}

