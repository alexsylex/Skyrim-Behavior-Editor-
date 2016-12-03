#include "hkblookatmodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/hkxfile.h"

/*
 * CLASS: hkbLookAtModifier
*/

uint hkbLookAtModifier::refCount = 0;

QString hkbLookAtModifier::classname = "hkbLookAtModifier";

hkbLookAtModifier::hkbLookAtModifier(BehaviorFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      newTargetGain(0),
      onGain(0),
      offGain(0),
      limitAngleDegrees(0),
      limitAngleLeft(0),
      limitAngleRight(0),
      limitAngleUp(0),
      limitAngleDown(0),
      headIndex(-1),
      neckIndex(-1),
      isOn(false),
      individualLimitsOn(false),
      isTargetInsideLimitCone(false)
{
    setType(HKB_LOOK_AT_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "hkbLookAtModifier"+QString::number(refCount);
}

QString hkbLookAtModifier::getClassname(){
    return classname;
}

QString hkbLookAtModifier::getName() const{
    return name;
}

bool hkbLookAtModifier::readData(const HkxXmlReader &reader, long index){
    bool ok;
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "variableBindingSet"){
            if (!variableBindingSet.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'variableBindingSet' reference!\nObject Reference: "+ref);
            }
        }else if (text == "userData"){
            userData = reader.getElementValueAt(index).toULong(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'userData' data field!\nObject Reference: "+ref);
            }
        }else if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }else if (text == "enable"){
            enable = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'enable' data field!\nObject Reference: "+ref);
            }
        }else if (text == "targetWS"){
            targetWS = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'targetWS' data field!\nObject Reference: "+ref);
            }
        }else if (text == "headForwardLS"){
            headForwardLS = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'headForwardLS' data field!\nObject Reference: "+ref);
            }
        }else if (text == "neckForwardLS"){
            neckForwardLS = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'neckForwardLS' data field!\nObject Reference: "+ref);
            }
        }else if (text == "neckRightLS"){
            neckRightLS = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'neckRightLS' data field!\nObject Reference: "+ref);
            }
        }else if (text == "eyePositionHS"){
            eyePositionHS = readVector4(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'eyePositionHS' data field!\nObject Reference: "+ref);
            }
        }else if (text == "newTargetGain"){
            newTargetGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'newTargetGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "onGain"){
            onGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'onGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "offGain"){
            offGain = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'offGain' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitAngleDegrees"){
            limitAngleDegrees = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'limitAngleDegrees' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitAngleLeft"){
            limitAngleLeft = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'limitAngleLeft' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitAngleRight"){
            limitAngleRight = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'limitAngleRight' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitAngleUp"){
            limitAngleUp = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'limitAngleUp' data field!\nObject Reference: "+ref);
            }
        }else if (text == "limitAngleDown"){
            limitAngleDown = reader.getElementValueAt(index).toDouble(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'limitAngleDown' data field!\nObject Reference: "+ref);
            }
        }else if (text == "headIndex"){
            headIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'headIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "neckIndex"){
            neckIndex = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'neckIndex' data field!\nObject Reference: "+ref);
            }
        }else if (text == "isOn"){
            isOn = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'isOn' data field!\nObject Reference: "+ref);
            }
        }else if (text == "individualLimitsOn"){
            individualLimitsOn = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'individualLimitsOn' data field!\nObject Reference: "+ref);
            }
        }else if (text == "isTargetInsideLimitCone"){
            isTargetInsideLimitCone = toBool(reader.getElementValueAt(index), &ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'isTargetInsideLimitCone' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkbLookAtModifier::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QString refString = "null";
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        if (variableBindingSet.data()){
            refString = variableBindingSet.data()->getReferenceString();
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("variableBindingSet"), refString);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("userData"), QString::number(userData));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("name"), name);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("enable"), getBoolAsString(enable));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("targetWS"), targetWS.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("headForwardLS"), headForwardLS.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("neckForwardLS"), neckForwardLS.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("neckRightLS"), neckRightLS.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("eyePositionHS"), eyePositionHS.getValueAsString());
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("newTargetGain"), QString::number(newTargetGain));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("onGain"), QString::number(onGain));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("offGain"), QString::number(offGain));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleDegrees"), QString::number(limitAngleDegrees));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleLeft"), QString::number(limitAngleLeft));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleRight"), QString::number(limitAngleRight));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleUp"), QString::number(limitAngleUp));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("limitAngleDown"), QString::number(limitAngleDown));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("headIndex"), QString::number(headIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("neckIndex"), QString::number(neckIndex));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("isOn"), getBoolAsString(isOn));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("individualLimitsOn"), getBoolAsString(individualLimitsOn));
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("isTargetInsideLimitCone"), getBoolAsString(isTargetInsideLimitCone));
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
    }
    return true;
}

bool hkbLookAtModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<DataIconManager *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    return true;
}

void hkbLookAtModifier::unlink(){
    HkDynamicObject::unlink();
}

bool hkbLookAtModifier::evaulateDataValidity(){
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

hkbLookAtModifier::~hkbLookAtModifier(){
    refCount--;
}