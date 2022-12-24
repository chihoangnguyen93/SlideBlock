//
//  LevelGame.cpp
//  SlideBlock
//
//  Created by Harry Nguyen on 12/8/16.
//
//

#include "LevelGame.h"

LevelGame::LevelGame() {}

LevelGame::~LevelGame() {}

void LevelGame::createDataWithFile(const char* fileName){
  if(!arrayData.empty()) {
    arrayData.clear();
  }
  XMLDocument* doc = new XMLDocument();
  std::string data = FileUtils::getInstance()->getStringFromFile(fileName);
  XMLError errorID = doc->Parse(data.c_str());
  if (errorID != 0) {
    return;
  }
  XMLElement* root = doc->RootElement();
  for (XMLElement* elemetdata = root->FirstChildElement(); elemetdata; elemetdata = elemetdata->NextSiblingElement()) {
    XMLElement* child = elemetdata->FirstChildElement("x1");
    if (child) {
      int x1 = atoi(child->GetText());
      XMLElement* child2 = child->NextSiblingElement();
      if(child2) {
        int y1 = atoi(child2->GetText());
        XMLElement* child3 = child2->NextSiblingElement();
        if (child3) {
          int x2 = atoi(child3->GetText());
          XMLElement* child4 = child3->NextSiblingElement();
          if(child4) {
            int y2 = atoi(child4->GetText());
            XMLElement* child5 = child4->NextSiblingElement();
            if(child5) {
              int typeObject = atoi(child5->GetText());
              DataGame *data = new DataGame(x1, y1, x2, y2, typeObject);
              arrayData.push_back(data);
            }
          }
        }
      }
    }
  }
  delete doc;
}

std::vector<DataGame*> LevelGame::getDataForLevel() {
  return arrayData;
}
