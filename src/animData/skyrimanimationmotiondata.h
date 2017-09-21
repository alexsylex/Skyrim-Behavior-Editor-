#ifndef SKYRIMANIMATIONMOTIONDATA_H
#define SKYRIMANIMATIONMOTIONDATA_H

#include <QTextStream>

#include "skyrimanimationtranslation.h"
#include "skyrimanimationrotation.h"

class ProjectAnimData;
class QFile;

class SkyrimAnimationMotionData
{
    friend class SkyrimAnimationDataUI;
    friend class ProjectAnimData;
public:
    SkyrimAnimationMotionData();
    SkyrimAnimationMotionData(ProjectAnimData *par, uint ind = 0, qreal dur = 0, const QVector <SkyrimAnimationTranslation> & trans = QVector <SkyrimAnimationTranslation>(), const QVector <SkyrimAnimationRotation> & rots = QVector <SkyrimAnimationRotation>());
    SkyrimAnimationMotionData(const SkyrimAnimationMotionData & other);
    bool read(QFile * file, uint &lineCount);
    bool write(QFile * file, QTextStream & out) const;
    uint lineCount() const;
    void addTranslation(const SkyrimAnimationTranslation & trans = SkyrimAnimationTranslation());
    bool removeTranslation(int index);
    void addRotation(const SkyrimAnimationRotation & rot = SkyrimAnimationRotation());
    bool removeRotation(int index);
private:
    ProjectAnimData *parent;
    uint animationIndex;
    qreal duration;
    QVector <SkyrimAnimationTranslation> translations;
    QVector <SkyrimAnimationRotation> rotations;
    bool chopLine(QFile *file, QByteArray &line, uint &linecount);
};

#endif // SKYRIMANIMATIONMOTIONDATA_H