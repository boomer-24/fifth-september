#ifndef GRAPHICSITEMGROUP_H
#define GRAPHICSITEMGROUP_H

#include "imagedescription.h"

//#include <cassert>
#include <QObject>
#include <QGraphicsItemGroup>
#include <QPixmap>
#include <QGraphicsSceneWheelEvent>

class GraphicsItemGroup : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit GraphicsItemGroup(QObject *parent = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent* _pressEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* _releaseEvent);
//    void wheelEvent(QGraphicsSceneWheelEvent* _wheelevent);

    void InitializeVVV(const QVector<ZoomLevelDescription>& _ZoomLevelDescription);    
    void setStoragePath(const QString& _storagePath);
    void setItemGroupSize(const QSize& _size);
    const QSize& getItemGroupSize()  const;
    const int& getTileSide() const;   //в пикселях
    const int& getCurrentZoomLevel() const;
    void CreateTile(const QString& _tilePath, QPixmap& _pixmap);
    void FirstAdditionToItemGroup();

    void AddTilesToPixmapItemGroup(const int& _hStart, const int& _hFinish, const int& _wStart, const int& _wFinish);
    void RemoveTilesFromPixmapItemGroup(const int& _hStart, const int& _hFinish, const int& _wStart, const int& _wFinish);

    void ChangeWidthItemGroup(const int& _w);
    void ChangeHeightItemGroup(const int& _h);    

    void increaseMagnification(const int& _newLayerNumber);
    void decreaseMagnification(const int& _newLayerNumber);
    const QPoint& getScalingOffset()const;

    const QPoint& getPreviousPos() const;    //for scaling
    const int& getCountOfLevels() const;

signals:
    void signalItemGroupIsMoving();

public slots:
    void slotChangeItemGroup();

private:
    const int TILESIDE_ = 256;
    QVector<QVector<QVector<QGraphicsPixmapItem*>>> vvvPixmapItemPointer_;
    QVector<QVector<QVector<QString>>> vvvTilePaths_;
    QString storagePath_;
    QSize itemGroupSize_;   //в тайлах
    QPoint upperLeftVisibleTileNumber_;
    int currentZoomLevel_ = 0;

    int startWidth_;  int finishWidth_;
    int startHeight_; int finishHeight_;

    QPoint departureMovingPoint_, arrivalMovingPoint_;
    QPoint sumOffsetPix_;
    int tileOffsetX_, tileOffsetY_;

    QPoint previousPos_;    //for scaling
    QPoint offsetScaling_;


};

#endif // GRAPHICSITEMGROUP_H
