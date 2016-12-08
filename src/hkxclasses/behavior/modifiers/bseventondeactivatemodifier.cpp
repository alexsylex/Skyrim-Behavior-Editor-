#include "bseventondeactivatemodifier.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/behaviorfile.h"

/*
 * CLASS: BSEventOnDeactivateModifier
*/

uint BSEventOnDeactivateModifier::refCount = 0;

QString BSEventOnDeactivateModifier::classname = "BSEventOnDeactivateModifier";

BSEventOnDeactivateModifier::BSEventOnDeactivateModifier(HkxFile *parent, long ref)
    : hkbModifier(parent, ref),
      userData(0),
      enable(true),
      id(-1)
{
    setType(HKB_EVENT_DRIVEN_MODIFIER, TYPE_MODIFIER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
    name = "BSEventOnDeactivateModifier"+QString::number(refCount);
}

QString BSEventOnDeactivateModifier::getClassname(){
    return classname;
}

QString BSEventOnDeactivateModifier::getName() const{
    return name;
}

bool BSEventOnDeactivateModifier::readData(const HkxXmlReader &reader, long index){
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
        }else if (text == "id"){
            id = reader.getElementValueAt(index).toInt(&ok);
            if (!ok){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'id' data field!\nObject Reference: "+ref);
            }
        }else if (text == "payload"){
            if (!payload.readReference(index, reader)){
                writeToLog(getClassname()+": readData()!\nFailed to properly read 'payload' reference!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool BSEventOnDeactivateModifier::write(HkxXMLWriter *writer){
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
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("event"), "");
        writer->writeLine(writer->object, true);
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("id"), QString::number(id));
        if (payload.data()){
            refString = payload.data()->getReferenceString();
        }else{
            refString = "null";
        }
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("payload"), refString);
        writer->writeLine(writer->object, false);
        writer->writeLine(writer->parameter, false);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        if (variableBindingSet.data() && !variableBindingSet.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'variableBindingSet'!!!", true);
        }
        if (payload.data() && !payload.data()->write(writer)){
            getParentFile()->writeToLog(getClassname()+": write()!\nUnable to write 'payload'!!!", true);
        }
    }
    return true;
}

bool BSEventOnDeactivateModifier::link(){
    if (!getParentFile()){
        return false;
    }
    if (!static_cast<DataIconManager *>(this)->linkVar()){
        writeToLog(getClassname()+": link()!\nFailed to properly link 'variableBindingSet' data field!\nObject Name: "+name);
    }
    HkxObjectExpSharedPtr *ptr = static_cast<BehaviorFile *>(getParentFile())->findHkxObject(payload.getReference());
    if (ptr){
        if ((*ptr)->getSignature() != HKB_STRING_EVENT_PAYLOAD){
            writeToLog(getClassname()+": linkVar()!\nThe linked object 'payload' is not a HKB_STRING_EVENT_PAYLOAD!");
            setDataValidity(false);
        }
        payload = *ptr;
    }
    return true;
}

void BSEventOnDeactivateModifier::unlink(){
    HkDynamicObject::unlink();
    payload = HkxObjectExpSharedPtr();
}

bool BSEventOnDeactivateModifier::evaulateDataValidity(){    //Check if event id is valid???
    if (!HkDynamicObject::evaulateDataValidity()){
        return false;
    }else if (name == ""){
    }else if (payload.data() && payload.data()->getSignature() != HKB_STRING_EVENT_PAYLOAD){
    }else{
        setDataValidity(true);
        return true;
    }
    setDataValidity(false);
    return false;
}

BSEventOnDeactivateModifier::~BSEventOnDeactivateModifier(){
    refCount--;
}