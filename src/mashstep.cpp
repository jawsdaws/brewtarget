/*
 * mashstep.cpp is part of Brewtarget, and is Copyright Philip G. Lee
 * (rocketman768@gmail.com), 2009.
 *
 * Brewtarget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Brewtarget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>
#include <vector>
#include "xmlnode.h"
#include "stringparsing.h"
#include "mashstep.h"
#include "brewtarget.h"
#include <QDomElement>
#include <QDomText>

bool operator<(MashStep &m1, MashStep &m2)
{
   return m1.name < m2.name;
}

bool operator==(MashStep &m1, MashStep &m2)
{
   return m1.name == m2.name;
}

/*
std::string MashStep::toXml()
{
   std::string ret = "<MASH_STEP>\n";
   
   ret += "<NAME>"+name+"</NAME>\n";
   ret += "<VERSION>"+intToString(version)+"</VERSION>\n";
   ret += "<TYPE>"+type+"</TYPE>\n";
   ret += "<INFUSE_AMOUNT>"+doubleToString(infuseAmount_l)+"</INFUSE_AMOUNT>\n";
   ret += "<STEP_TEMP>"+doubleToString(stepTemp_c)+"</STEP_TEMP>\n";
   ret += "<STEP_TIME>"+doubleToString(stepTime_min)+"</STEP_TIME>\n";
   ret += "<RAMP_TIME>"+doubleToString(rampTime_min)+"</RAMP_TIME>\n";
   ret += "<END_TEMP>"+doubleToString(endTemp_c)+"</END_TEMP>\n";
   ret += "<INFUSE_TEMP>"+doubleToString(infuseTemp_c)+"</INFUSE_TEMP>\n";
   ret += "<DECOCTION_AMOUNT>"+doubleToString(decoctionAmount_l)+"</DECOCTION_AMOUNT>\n";
   
   ret += "</MASH_STEP>\n";
   return ret;
}
*/

void MashStep::toXml(QDomDocument& doc, QDomNode& parent)
{
   QDomElement mashStepNode;
   QDomElement tmpNode;
   QDomText tmpText;
   
   mashStepNode = doc.createElement("MASH_STEP");
   
   tmpNode = doc.createElement("NAME");
   tmpText = doc.createTextNode(name.c_str());
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("VERSION");
   tmpText = doc.createTextNode(text(version));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("TYPE");
   tmpText = doc.createTextNode(type.c_str());
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("INFUSE_AMOUNT");
   tmpText = doc.createTextNode(text(infuseAmount_l));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("STEP_TEMP");
   tmpText = doc.createTextNode(text(stepTemp_c));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("STEP_TIME");
   tmpText = doc.createTextNode(text(stepTime_min));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("RAMP_TIME");
   tmpText = doc.createTextNode(text(rampTime_min));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("END_TEMP");
   tmpText = doc.createTextNode(text(endTemp_c));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("INFUSE_TEMP");
   tmpText = doc.createTextNode(text(infuseTemp_c));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   tmpNode = doc.createElement("DECOCTION_AMOUNT");
   tmpText = doc.createTextNode(text(decoctionAmount_l));
   tmpNode.appendChild(tmpText);
   mashStepNode.appendChild(tmpNode);
   
   parent.appendChild(mashStepNode);
}

//==============================CONSTRUCTORS====================================

void MashStep::setDefaults()
{
   name = "";
   type = "Infusion";
   infuseAmount_l = 0.0;
   infuseTemp_c = 0.0;
   stepTemp_c = 0.0;
   stepTime_min = 0.0;
   rampTime_min = 0.0;
   endTemp_c = 0.0;
   decoctionAmount_l = 0.0;
}

MashStep::MashStep()
{
   setDefaults();
}

MashStep::MashStep( const XmlNode *node)
{
   std::vector<XmlNode *> children;
   std::vector<XmlNode *> tmpVec;
   std::string tag;
   std::string leafText;
   XmlNode* leaf;
   unsigned int i, childrenSize;
   bool hasName=false, hasVersion=false, hasType=false, hasInfuseAmount=false,
        hasStepTemp=false, hasStepTime=false;
   
   setDefaults();
   
   if( node->getTag() != "MASH_STEP" )
      throw MashStepException("initializer not passed a MASH_STEP node.");
   
   node->getChildren( children );
   childrenSize = children.size();
   
   for( i = 0; i < childrenSize; ++i )
   {
      tag = children[i]->getTag();
      children[i]->getChildren( tmpVec );
      
      // All valid children of YEAST only have zero or one child.
      if( tmpVec.size() > 1 )
         throw MashStepException("Tag \""+tag+"\" has more than one child.");
      
      // Have to deal with the fact that this node might not have
      // and children at all.
      if( tmpVec.size() == 1 )
         leaf = tmpVec[0];
      else
         leaf = &XmlNode();

      // It must be a leaf if it is a valid BeerXML entry.
      if( ! leaf->isLeaf() )
         throw MashStepException("Should have been a leaf but is not.");
      
      leafText = leaf->getLeafText();
      
      if( tag == "NAME" )
      {
         setName(leafText);
         hasName = true;
      }
      else if( tag == "VERSION" )
      {
         if( parseInt(leafText) != version )
            std::cerr << "Warning: XML MASH_STEP is not version " << version << std::endl;
         hasVersion=true;
      }
      else if( tag == "TYPE" )
      {
         setType(leafText);
         hasType=true;
      }
      else if( tag == "INFUSE_AMOUNT" )
      {
         setInfuseAmount_l(parseDouble(leafText));
         hasInfuseAmount=true;
      }
      else if( tag == "STEP_TEMP" )
      {
         setStepTemp_c(parseDouble(leafText));
         hasStepTemp=true;
      }
      else if( tag == "STEP_TIME" )
      {
         setStepTime_min(parseDouble(leafText));
         hasStepTime=true;
      }
      else if( tag == "RAMP_TIME" )
         setRampTime_min(parseDouble(leafText));
      else if( tag == "END_TEMP" )
         setEndTemp_c(parseDouble(leafText));
      // My extensions
      else if( tag == "INFUSE_TEMP" )
         setInfuseTemp_c(parseDouble(leafText));
      else if( tag == "DECOCTION_AMOUNT" )
         setDecoctionAmount_l(parseDouble(leafText));
      // ===
      else
         std::cerr << "Warning: MASH_STEP does not support the tag: " << tag << std::endl;
   } // end for()
   
   if( !hasName || !hasVersion || !hasType || !hasStepTemp || !hasStepTime ||
       (type=="Infusion" && !hasInfuseAmount) )
      throw MashStepException("missing required field.");
} // end MashStep()

MashStep::MashStep(const QDomNode& mashStepNode)
{
   QDomNode node, child;
   QDomText textNode;
   QString property, value;

   setDefaults();

   for( node = mashStepNode.firstChild(); ! node.isNull(); node = node.nextSibling() )
   {
      if( ! node.isElement() )
      {
         Brewtarget::log(Brewtarget::WARNING, QString("Node at line %1 is not an element.").arg(textNode.lineNumber()) );
         continue;
      }

      child = node.firstChild();
      if( child.isNull() || ! child.isText() )
         continue;

      property = node.nodeName();
      textNode = child.toText();
      value = textNode.nodeValue();

      if( property == "NAME" )
      {
         name = value.toStdString();
      }
      else if( property == "VERSION" )
      {
         if( version != getInt(textNode) )
            Brewtarget::log(Brewtarget::ERROR, QString("YEAST says it is not version %1. Line %2").arg(version).arg(textNode.lineNumber()) );
      }
      else if( property == "TYPE" )
      {
         if( isValidType(value.toStdString()) )
            type = value.toStdString();
         else
            Brewtarget::log(Brewtarget::ERROR, QString("%1 is not a valid type for MASHSTEP. Line %2").arg(value).arg(textNode.lineNumber()) );
      }
      else if( property == "INFUSE_AMOUNT" )
      {
         setInfuseAmount_l(getDouble(textNode));
      }
      else if( property == "STEP_TEMP" )
      {
         setStepTemp_c(getDouble(textNode));
      }
      else if( property == "STEP_TIME" )
      {
         setStepTime_min(getDouble(textNode));
      }
      else if( property == "RAMP_TIME" )
      {
         setRampTime_min(getDouble(textNode));
      }
      else if( property == "END_TEMP" )
      {
         setEndTemp_c(getDouble(textNode));
      }
      else if( property == "INFUSE_TEMP" )
      {
         setInfuseTemp_c(getDouble(textNode));
      }
      else if( property == "DECOCTION_AMOUNT" )
      {
         setDecoctionAmount_l(getDouble(textNode));
      }
      else
         Brewtarget::log(Brewtarget::WARNING, QString("Unsupported MASHSTEP property: %1. Line %2").arg(property).arg(node.lineNumber()) );
   }
}

//================================"SET" METHODS=================================
void MashStep::setName( const std::string &var )
{
   name = std::string(var);
   hasChanged();
}

void MashStep::setInfuseTemp_c(double var)
{
   infuseTemp_c = var;
   hasChanged();
}

void MashStep::setType( const std::string &var )
{
   if( ! isValidType(var) )
      throw MashStepException("invalid type: " + var );
   else
   {
      type = std::string(var);
      hasChanged();
   }
}

void MashStep::setInfuseAmount_l( double var )
{
   if( var < 0.0 )
      throw MashStepException("number cannot be negative: "+ doubleToString(var) );
   else
   {
      infuseAmount_l = var;
      hasChanged();
   }
}

void MashStep::setStepTemp_c( double var )
{
   if( var < -273.15 )
      throw MashStepException("Temperature below absolute zero: " + doubleToString(var));
   else
   {
      stepTemp_c = var;
      hasChanged();
   }
}

void MashStep::setStepTime_min( double var )
{
   if( var < 0.0 )
      throw MashStepException("number cannot be negative: "+ doubleToString(var) );
   else
   {
      stepTime_min = var;
      hasChanged();
   }
}

void MashStep::setRampTime_min( double var )
{
   if( var < 0.0 )
      throw MashStepException("number cannot be negative: "+ doubleToString(var) );
   else
   {
      rampTime_min = var;
      hasChanged();
   }
}

void MashStep::setEndTemp_c( double var )
{
   if( var < -273.15 )
      throw MashStepException("Temperature below absolute zero: " + doubleToString(var));
   else
   {
      endTemp_c = var;
      hasChanged();
   }
}

void MashStep::setDecoctionAmount_l(double var)
{
   decoctionAmount_l = var;
   hasChanged();
}

//============================="GET" METHODS====================================
std::string MashStep::getName() const
{
   return name;
}

double MashStep::getInfuseTemp_c() const
{
   return infuseTemp_c;
}

std::string MashStep::getType() const
{
   return type;
}

double MashStep::getInfuseAmount_l() const
{
   return infuseAmount_l;
}

double MashStep::getStepTemp_c() const
{
   return stepTemp_c;
}

double MashStep::getStepTime_min() const
{
   return stepTime_min;
}

double MashStep::getRampTime_min() const
{
   return rampTime_min;
}

double MashStep::getEndTemp_c() const
{
   return endTemp_c;
}

double MashStep::getDecoctionAmount_l() const
{
   return decoctionAmount_l;
}

bool MashStep::isValidType( const std::string &str ) const
{
   static const std::string types[] = {"Infusion", "Temperature", "Decoction"};
   static const unsigned int size = 3;
   unsigned int i;
   
   for( i = 0; i < size; ++i )
      if( str == types[i] )
         return true;
   
   return false;
}