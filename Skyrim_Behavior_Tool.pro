#-------------------------------------------------
#
# Project created by QtCreator 2016-02-09T01:56:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Skyrim_Behavior_Tool
TEMPLATE = app

# remove possible other optimization flags
#QMAKE_CXXFLAGS_DEBUG -= -O
#QMAKE_CXXFLAGS_DEBUG -= -O1
#QMAKE_CXXFLAGS_DEBUG -= -O2

# add the desired -O3 if not present
#QMAKE_CXXFLAGS_DEBUG *= -O3

SOURCES += \
    src/main.cpp \
    src/filetypes/hkxfile.cpp \
    src/ui/fileselectwindow.cpp \
    src/ui/mainwindow.cpp \
    src/xml/hkxxmlreader.cpp \
    src/hkxclasses/hkbbehaviorgraphstringdata.cpp \
    src/hkxclasses/hkxobject.cpp \
    src/hkxclasses/hkbvariablevalueset.cpp \
    src/hkxclasses/hkbbehaviorgraphdata.cpp \
    src/hkxclasses/generators/hkbbehaviorgraph.cpp \
    src/hkxclasses/generators/hkbgenerator.cpp \
    src/ui/behaviorgraphui/behaviorgraphicons.cpp \
    src/ui/behaviorgraphui/behaviorgraphui.cpp \
    src/ui/objectdataui/hkobjectui.cpp \
    src/hkxclasses/hkrootlevelcontainer.cpp \
    src/hkxclasses/generators/hkbbehaviorreferencegenerator.cpp \
    src/hkxclasses/generators/hkbstatemachine.cpp \
    src/hkxclasses/generators/hkbstatemachinestateinfo.cpp \
    src/hkxclasses/generators/hkbclipgenerator.cpp \
    src/hkxclasses/generators/bscyclicblendtransitiongenerator.cpp \
    src/hkxclasses/generators/hkbblendergenerator.cpp \
    src/hkxclasses/generators/hkbblendergeneratorchild.cpp \
    src/hkxclasses/generators/hkbmanualselectorgenerator.cpp \
    src/hkxclasses/generators/hkbmodifiergenerator.cpp \
    src/hkxclasses/generators/bsboneswitchgenerator.cpp \
    src/hkxclasses/generators/bsboneswitchgeneratorbonedata.cpp \
    src/hkxclasses/generators/bsistatetagginggenerator.cpp \
    src/hkxclasses/generators/bssynchronizedclipgenerator.cpp \
    src/hkxclasses/generators/hkbposematchinggenerator.cpp \
    src/hkxclasses/generators/bsoffsetanimationgenerator.cpp \
    src/hkxclasses/hkbstringeventpayload.cpp \
    src/hkxclasses/hkbboneweightarray.cpp \
    src/hkxclasses/hkbstatemachineeventpropertyarray.cpp \
    src/hkxclasses/hkbstatemachinetransitioninfoarray.cpp

HEADERS  += \
    src/filetypes/hkxfile.h \
    src/hkxclasses/modifiers.h \
    src/ui/fileselectwindow.h \
    src/ui/mainwindow.h \
    src/xml/hkxxmlreader.h \
    src/hkxclasses/hkbbehaviorgraphstringdata.h \
    src/hkxclasses/hkxobject.h \
    src/hkxclasses/hkbvariablevalueset.h \
    src/hkxclasses/hkbbehaviorgraphdata.h \
    src/hkxclasses/generators/hkbbehaviorgraph.h \
    src/hkxclasses/generators/hkbgenerator.h \
    src/ui/behaviorgraphui/behaviorgraphicons.h \
    src/ui/behaviorgraphui/behaviorgraphui.h \
    src/ui/objectdataui/hkobjectui.h \
    src/hkxclasses/hkrootlevelcontainer.h \
    src/hkxclasses/generators/hkbbehaviorreferencegenerator.h \
    src/hkxclasses/generators/hkbstatemachine.h \
    src/hkxclasses/generators/hkbstatemachinestateinfo.h \
    src/hkxclasses/generators/hkbclipgenerator.h \
    src/hkxclasses/generators/bscyclicblendtransitiongenerator.h \
    src/hkxclasses/generators/hkbblendergenerator.h \
    src/hkxclasses/generators/hkbblendergeneratorchild.h \
    src/hkxclasses/generators/hkbmanualselectorgenerator.h \
    src/hkxclasses/generators/hkbmodifiergenerator.h \
    src/hkxclasses/generators/bsboneswitchgenerator.h \
    src/hkxclasses/generators/bsboneswitchgeneratorbonedata.h \
    src/hkxclasses/generators/bsistatetagginggenerator.h \
    src/hkxclasses/generators/bssynchronizedclipgenerator.h \
    src/hkxclasses/generators/hkbposematchinggenerator.h \
    src/hkxclasses/generators/bsoffsetanimationgenerator.h \
    src/hkxclasses/hkbstringeventpayload.h \
    src/hkxclasses/hkbboneweightarray.h \
    src/hkxclasses/hkbstatemachineeventpropertyarray.h \
    src/hkxclasses/hkbstatemachinetransitioninfoarray.h \
    src/ui/genericdatawidgets.h
