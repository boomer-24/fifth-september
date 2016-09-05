#ifndef IMAGEDESCRIPTION_H
#define IMAGEDESCRIPTION_H

#include "zoomleveldescription.h"
#include <QtXml>

class ImageDescription
{
public:
    ImageDescription();

    void Initialize(const QString& _pathToXml);
    const QVector<ZoomLevelDescription>& getLayerDescriptions()const;
    const ZoomLevelDescription& getOneLayerDescription(const int& number);

private:
    QVector<ZoomLevelDescription> vZoomLevelsDescription_;
};

#endif // IMAGEDESCRIPTION_H
