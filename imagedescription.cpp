#include "imagedescription.h"

ImageDescription::ImageDescription()
{
    qDebug() << "imageDescription Ctor";
}

void ImageDescription::Initialize(const QString& _pathToXml)
{
    if (!vZoomLevelsDescription_.isEmpty())
        this->vZoomLevelsDescription_.clear();

    QDomDocument domDoc;
    QFile file(_pathToXml);
    if (file.open(QIODevice::ReadOnly))
    {
        if (domDoc.setContent(&file))
        {
            QDomElement domElement = domDoc.documentElement();
            QDomNode domNode = domElement.firstChild();
            while(!domNode.isNull())
            {
                if(domNode.isElement())
                {
                    QDomElement domElement = domNode.toElement();
                    if(!domElement.isNull())
                    {
                        ZoomLevelDescription ZLDescription;
                        if(domElement.tagName() == "scale")
                        {
                            ZLDescription.setValue((domElement.attribute("value")).toInt());
                            ZLDescription.setWidth((domElement.attribute("width")).toInt());
                            ZLDescription.setHeight((domElement.attribute("height")).toInt());
                            ZLDescription.setTileAmountW((domElement.attribute("tile_amount_w")).toInt());
                            ZLDescription.setTileAmountH((domElement.attribute("tile_amount_h")).toInt());                            
                            this->vZoomLevelsDescription_.push_front(ZLDescription);                            
                        }
                    }
                    domNode = domNode.nextSibling();
                }
            }
        } else qDebug() << "It`s no XML!";
    } else qDebug() << "File not open =(";
}

const QVector<ZoomLevelDescription>& ImageDescription::getLayerDescriptions() const
{
    return this->vZoomLevelsDescription_;
}

const ZoomLevelDescription& ImageDescription::getOneLayerDescription(const int& number)
{
    return this->vZoomLevelsDescription_.at(number);
}

