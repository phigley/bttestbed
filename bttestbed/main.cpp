//
//  main.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/28/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "application.h"
#include "world.h"
#include "ai/npc.h"
#include "ai/state.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include <iostream>
#include <memory>

class XmlDocument
{
public:

    XmlDocument(const char* fileName)
        : file(fileName)
    {
        document.parse<0>(file.data());

    }

    rapidxml::xml_document<>& getDocument() { return document; }
    
private:

    rapidxml::file<>         file;
    rapidxml::xml_document<> document;
};


void addNPC(Application& app, const char* behaviorFile)
{
    XmlDocument xmlDocument(behaviorFile);

    auto rootNode = xmlDocument.getDocument().first_node();
    assert(rootNode != nullptr);
    if( rootNode )
    {
        assert(0 == strcmp(rootNode->name(), "root"));
    
        app.getWorld().addEntity<AI::NPC>(*rootNode);
    }
}

int main(int argc, const char * argv[])
{
	Application app{800, 800};

    addNPC(app, "bts/test.xml");
    addNPC(app, "bts/test2.xml");
    
    while( app.beginFrame() )
    {
        app.render();
    }

    return 0;
}

