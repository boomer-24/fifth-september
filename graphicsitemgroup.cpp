#include "graphicsitemgroup.h"

GraphicsItemGroup::GraphicsItemGroup(QObject* parent) : QObject(parent)
{
    qDebug() << "ItemGroup Ctor";
    QObject::connect(this, SIGNAL(signalItemGroupIsMoving()), this, SLOT(slotChangeItemGroup()));   //  нахрена? сделать через обычный метод. клоун
    this->offsetScaling_.setX(0);
    this->offsetScaling_.setY(0);
    this->offsetScaling_.setX(0);
    this->offsetScaling_.setY(0);
}

void GraphicsItemGroup::mousePressEvent(QGraphicsSceneMouseEvent* _pressEvent)
{
    QGraphicsItemGroup::mousePressEvent(_pressEvent);
    
    this->departureMovingPoint_.setX(this->pos().rx());
    this->departureMovingPoint_.setY(this->pos().ry());
    qDebug() << "departure: " << this->pos();
}

void GraphicsItemGroup::mouseReleaseEvent(QGraphicsSceneMouseEvent* _releaseEvent)
{
    QGraphicsItemGroup::mouseReleaseEvent(_releaseEvent);
    this->arrivalMovingPoint_.setX(this->pos().rx());
    this->arrivalMovingPoint_.setY(this->pos().ry());
    this->tileOffsetX_ = 0; this->tileOffsetY_ = 0;
    QPoint offset; //разовое смещение
    offset.setX(-this->arrivalMovingPoint_.rx() + this->departureMovingPoint_.rx());    //ОТРИЦАТЕЛЬНЫЙ, КОГДА СМЕЩЕНИЕ _ВПРАВО_подгружать слева
    offset.setY(-this->arrivalMovingPoint_.ry() + this->departureMovingPoint_.ry());    //ОТРИЦАТЕЛЬНЫЙ, КОГДА СМЕЩЕНИЕ _ВВЕРХ_
    this->sumOffsetPix_ += offset;

    qDebug() << "arrival: " << this->pos();

    if (this->sumOffsetPix_.rx() >= this->TILESIDE_)
    {
        if (finishWidth_ < this->vvvPixmapItemPointer_.at(currentZoomLevel_).at(0).size())
        {
            this->tileOffsetX_ = this->sumOffsetPix_.rx() / this->TILESIDE_;
            this->sumOffsetPix_.setX(sumOffsetPix_.rx() % this->TILESIDE_);
            emit this->signalItemGroupIsMoving();
        }
    }else if (this->sumOffsetPix_.rx() <= -this->TILESIDE_)
    {
        if (startWidth_ > 0)
        {
            this->tileOffsetX_ = this->sumOffsetPix_.rx() / this->TILESIDE_;
            this->sumOffsetPix_.setX(sumOffsetPix_.rx() % this->TILESIDE_);
            emit this->signalItemGroupIsMoving();
        }
    }
    
    if (this->sumOffsetPix_.ry() >= this->TILESIDE_)
    {
        if (finishHeight_ < this->vvvPixmapItemPointer_.at(currentZoomLevel_).size())
        {
            this->tileOffsetY_ = this->sumOffsetPix_.ry() / this->TILESIDE_;
            this->sumOffsetPix_.setY(sumOffsetPix_.ry() % this->TILESIDE_);
            emit this->signalItemGroupIsMoving();
        }
    }else if (this->sumOffsetPix_.ry() <= -this->TILESIDE_)
    {
        if (startHeight_ > 0)
        {
            this->tileOffsetY_ = this->sumOffsetPix_.ry() / this->TILESIDE_;
            this->sumOffsetPix_.setY(sumOffsetPix_.ry() % this->TILESIDE_);
            emit this->signalItemGroupIsMoving();
        }
    }
    //qDebug() << "arrival: " << this->pos(); //qDebug() << "current offset: " << offset; //qDebug() << "sumOffset: " << this->sumOffsetPix_ << "\n"; //qDebug() << this->pos();
}

//void GraphicsItemGroup::wheelEvent(QGraphicsSceneWheelEvent* _wheelevent)
//{
//    qDebug() << "wheel";
//    qDebug () << "_wheelevent->delta(): " << _wheelevent->delta();
//    qDebug () << " _wheelevent->pos(): " << _wheelevent->pos();
//    qDebug () << "_wheelevent->scenePos(): " << _wheelevent->scenePos();
//}

void GraphicsItemGroup::AddTilesToPixmapItemGroup(const int& _hStart, const int& _hFinish, const int& _wStart, const int& _wFinish)
{
    QString tilePath;
    QPixmap pixmapForGroup;
    for (int h = _hStart; h < _hFinish; h++)
    {
        for (int w = _wStart; w < _wFinish; w++)
        {
            tilePath = this->vvvTilePaths_.operator [](currentZoomLevel_).operator[](h).operator[](w);
            pixmapForGroup.load(tilePath);
            this->CreateTile(tilePath, pixmapForGroup);
            this->vvvPixmapItemPointer_.operator [](currentZoomLevel_).operator[](h).operator[](w) = new QGraphicsPixmapItem;
            this->vvvPixmapItemPointer_.operator [](currentZoomLevel_).operator[](h).operator[](w)->setPixmap(pixmapForGroup);
            this->vvvPixmapItemPointer_.operator [](currentZoomLevel_).operator[](h).operator[](w)->setOffset(this->pos().rx() + w * TILESIDE_, this->pos().ry() + h * TILESIDE_);
            this->addToGroup(this->vvvPixmapItemPointer_.operator [](currentZoomLevel_).operator[](h).operator[](w));
        }
    }
}

void GraphicsItemGroup::RemoveTilesFromPixmapItemGroup(const int& _hStart, const int& _hFinish, const int& _wStart, const int& _wFinish)
{
    for (int h = _hStart; h < _hFinish; h++)
        for (int w = _wStart; w < _wFinish; w++)
        {
            this->removeFromGroup(this->vvvPixmapItemPointer_.operator [](currentZoomLevel_).operator[](h).operator[](w));
            delete this->vvvPixmapItemPointer_.operator [](currentZoomLevel_).operator[](h).operator[](w);
        }
}

void GraphicsItemGroup::ChangeWidthItemGroup(const int& _w)
{
    qDebug() << "ChangeWidthItemGroup( " << _w << " )";
    int loopAddWstart, loopAddWfinish;
    int loopRemoveWstart, loopRemoveWfinish;
    if (_w < 0)    //ОТРИЦАТЕЛЬНЫЙ, КОГДА СМЕЩЕНИЕ _ВПРАВО___подгружать слева
    {
        loopAddWstart = this->startWidth_ + _w;     loopAddWfinish = this->startWidth_;
        loopRemoveWstart = this->finishWidth_ + _w; loopRemoveWfinish = this->finishWidth_;

        (loopAddWstart < 0) ? (loopAddWstart = 0) : (loopAddWstart = loopAddWstart);
        (loopRemoveWstart < itemGroupSize_.width()/* / 3*/) ? (loopRemoveWstart = itemGroupSize_.width()/* / 3*/) : (loopRemoveWstart = loopRemoveWstart);

        this->AddTilesToPixmapItemGroup(this->startHeight_, this->finishHeight_, loopAddWstart, loopAddWfinish);
        this->RemoveTilesFromPixmapItemGroup(this->startHeight_, this->finishHeight_, loopRemoveWstart, loopRemoveWfinish);

        qDebug() << "this->startWidth_ before change: " << this->startWidth_ << ";  loopAddWstart: " << loopAddWstart;
        qDebug() << "this->finishWidth_ before change: " << this->finishWidth_ << ";    loopRemoveWstart: " << loopRemoveWstart;
        this->startWidth_ = loopAddWstart;
        this->finishWidth_ = loopRemoveWstart;
        qDebug() << "ASSIGN............    " << "ItemGroupSize: " << this->itemGroupSize_;
        qDebug() << "this->startWidth_ after change: " << this->startWidth_ << ";  loopAddWstart: " << loopAddWstart;
        qDebug() << "this->finishWidth_ after change: " << this->finishWidth_ << ";    loopRemoveWstart: " << loopRemoveWstart << "\n";
    } else if (_w > 0)
    {
        loopAddWstart = this->finishWidth_;     loopAddWfinish = this->finishWidth_ + _w;
        loopRemoveWstart = this->startWidth_;   loopRemoveWfinish = this->startWidth_ + _w;

        (loopAddWfinish > vvvPixmapItemPointer_.at(currentZoomLevel_).at(0).size()) ? (loopAddWfinish = vvvPixmapItemPointer_.at(currentZoomLevel_).at(0).size()) : (loopAddWfinish = loopAddWfinish);
        (loopRemoveWfinish > vvvPixmapItemPointer_.at(currentZoomLevel_).at(0).size() - itemGroupSize_.width()/* / 3*/) ? (loopRemoveWfinish = vvvPixmapItemPointer_.at(currentZoomLevel_).at(0).size() - itemGroupSize_.width()/* / 3*/) : (loopRemoveWfinish = loopRemoveWfinish);

        this->AddTilesToPixmapItemGroup(this->startHeight_, this->finishHeight_, loopAddWstart, loopAddWfinish);
        this->RemoveTilesFromPixmapItemGroup(this->startHeight_, this->finishHeight_, loopRemoveWstart, loopRemoveWfinish);

        qDebug() << "this->startWidth_ before change: " << this->startWidth_ << ";  loopRemoveWfinish: " << loopRemoveWfinish;
        qDebug() << "this->finishWidth_ before change: " << this->finishWidth_ << ";    loopaddWfinish: " << loopAddWfinish;
        this->startWidth_ = loopRemoveWfinish;
        this->finishWidth_ = loopAddWfinish;
        qDebug() << "ASSIGN............    " << "ItemGroupSize: " << this->itemGroupSize_;
        qDebug() << "this->startWidth_ after change: " << this->startWidth_ << ";  loopRemoveWfinish: " << loopRemoveWfinish;
        qDebug() << "this->finishWidth_ after change: " << this->finishWidth_ << ";    loopaddWfinish: " << loopAddWfinish << "\n";
    }
}

void GraphicsItemGroup::ChangeHeightItemGroup(const int& _h)
{
    qDebug() << "ChangeHeightItemGroup( " << _h << " )";
    int loopAddHstart, loopAddHfinish;
    int loopRemoveHstart, loopRemoveHfinish;
    if (_h < 0)    //ОТРИЦАТЕЛЬНЫЙ, КОГДА СМЕЩЕНИЕ _ВНИЗ___подгружать сверху
    {
        loopAddHstart = this->startHeight_ + _h;     loopAddHfinish = this->startHeight_;
        loopRemoveHstart = this->finishHeight_ + _h; loopRemoveHfinish = this->finishHeight_;

        (loopAddHstart < 0) ? (loopAddHstart = 0) : (loopAddHstart = loopAddHstart);
        (loopRemoveHstart < itemGroupSize_.height()/* / 3*/) ? (loopRemoveHstart = itemGroupSize_.height()/* / 3*/) : (loopRemoveHstart = loopRemoveHstart);

        this->AddTilesToPixmapItemGroup(loopAddHstart, loopAddHfinish, this->startWidth_, this->finishWidth_);
        this->RemoveTilesFromPixmapItemGroup(loopRemoveHstart, loopRemoveHfinish, this->startWidth_, this->finishWidth_);

        qDebug() << "this->startHeight_ before change: " << this->startHeight_ << ";  loopAddHstart: " << loopAddHstart;
        qDebug() << "this->finishHidth_ before change: " << this->finishHeight_ << ";    loopRemoveHstart: " << loopRemoveHstart;
        this->startHeight_ = loopAddHstart;
        this->finishHeight_ = loopRemoveHstart;
        qDebug() << "ASSIGN............    " << "ItemGroupSize: " << this->itemGroupSize_;
        qDebug() << "this->startHeight_ after change: " << this->startHeight_ << ";  loopAddHstart: " << loopAddHstart;
        qDebug() << "this->finishHeight_ after change: " << this->finishHeight_ << ";    loopRemoveHstart: " << loopRemoveHstart << "\n";


    } else if (_h > 0)
    {
        loopAddHstart = this->finishHeight_;     loopAddHfinish = this->finishHeight_ + _h;
        loopRemoveHstart = this->startHeight_;   loopRemoveHfinish = this->startHeight_ + _h;

        (loopAddHfinish > vvvPixmapItemPointer_.at(currentZoomLevel_).size()) ? (loopAddHfinish = vvvPixmapItemPointer_.at(currentZoomLevel_).size()) : (loopAddHfinish = loopAddHfinish);
        (loopRemoveHfinish > vvvPixmapItemPointer_.at(currentZoomLevel_).size() - itemGroupSize_.height()/* / 3*/) ? (loopRemoveHfinish = vvvPixmapItemPointer_.at(currentZoomLevel_).size() - itemGroupSize_.height()/* / 3*/) : (loopRemoveHfinish = loopRemoveHfinish);

        this->AddTilesToPixmapItemGroup(loopAddHstart, loopAddHfinish, this->startWidth_, this->finishWidth_);
        this->RemoveTilesFromPixmapItemGroup(loopRemoveHstart, loopRemoveHfinish, this->startWidth_, this->finishWidth_);

        qDebug() << "this->startHeight_ before change: " << this->startHeight_ << ";  loopRemoveHfinish: " << loopRemoveHfinish;
        qDebug() << "this->finishHeight_ before change: " << this->finishHeight_ << ";    loopaddHfinish: " << loopAddHfinish;
        this->startHeight_ = loopRemoveHfinish;
        this->finishHeight_ = loopAddHfinish;
        qDebug() << "ASSIGN............    " << "ItemGroupSize: " << this->itemGroupSize_;
        qDebug() << "this->startHeight_ after change: " << this->startHeight_ << ";  loopRemoveHfinish: " << loopRemoveHfinish;
        qDebug() << "this->finishHeight_ after change: " << this->finishHeight_ << ";    loopaddHfinish: " << loopAddHfinish << "\n";
    }
}

void GraphicsItemGroup::increaseMagnification(const int& _newLayerNumber)   //  +
{
    if (0 <= _newLayerNumber && _newLayerNumber < this->vvvPixmapItemPointer_.size())
    {               // ЭТО ЕСЛИ В ПЛЮС
        double coefficientX = double(this->vvvPixmapItemPointer_.at(_newLayerNumber).at(0).size()) / double(this->vvvPixmapItemPointer_.at(currentZoomLevel_).at(0).size());//для увеличения
        double coefficientY = double(this->vvvPixmapItemPointer_.at(_newLayerNumber).size()) / double(this->vvvPixmapItemPointer_.at(currentZoomLevel_).size());//для увеличения

        double tileX = double(double(startWidth_) + double((finishWidth_ - startWidth_) / 2)) * coefficientX;
        double tileY = double(double(startHeight_) + double((finishHeight_ - startHeight_) / 2)) * coefficientY;//  может тут еще учитывать this->pos....

        double offsetX = tileX - int(tileX);
        double offsetY = tileY - int(tileY);

        offsetX *= this->TILESIDE_;
        offsetY *= this->TILESIDE_;
        qDebug() << "offsetX: " << offsetX << "     offsetY: " << offsetY;

        int offsetScX = this->offsetScaling_.rx();
        int offsetScY = this->offsetScaling_.ry();

        qDebug() << "prevOffsetX: " << offsetScX << "      prevOffsetY: " << offsetScY;

        if (offsetScX + offsetX >= this->TILESIDE_)
        {
            tileX++;
            this->offsetScaling_.setX(offsetScX + offsetX - this->TILESIDE_);
        } else this->offsetScaling_.setX(offsetScX + offsetX);

        if (offsetScY + offsetY >= this->TILESIDE_)
        {
            tileY++;
            this->offsetScaling_.setY(offsetScY + offsetY - this->TILESIDE_);
        } else this->offsetScaling_.setY(offsetScY + offsetY);

        int leftT = int(tileX) - this->itemGroupSize_.rwidth() / 2 + 1;    //И ТУТ ЧТО-ТО SMELL A RAT
        int rightT = int(tileX) + this->itemGroupSize_.rwidth() / 2 + 1;
        int upT = int(tileY) - this->itemGroupSize_.height() / 2 + 1;
        int downT = int(tileY) + this->itemGroupSize_.height() / 2 + 1;

        this->RemoveTilesFromPixmapItemGroup(startHeight_, finishHeight_, startWidth_, finishWidth_);

        this->currentZoomLevel_ = _newLayerNumber;

        this->AddTilesToPixmapItemGroup(upT, downT, leftT, rightT);

        this->startWidth_ = leftT;
        this->finishWidth_ = rightT;
        this->startHeight_ = upT;
        this->finishHeight_ = downT;
    }
}

void GraphicsItemGroup::decreaseMagnification(const int& _newLayerNumber)
{
    if (0 < _newLayerNumber && _newLayerNumber <= this->vvvPixmapItemPointer_.size())
    {               // ЭТО ЕСЛИ В МИНУС
        double coefficientX = double(this->vvvPixmapItemPointer_.at(_newLayerNumber).at(0).size()) / double(this->vvvPixmapItemPointer_.at(currentZoomLevel_).at(0).size());//для уменьшения
        double coefficientY = double(this->vvvPixmapItemPointer_.at(_newLayerNumber).size()) / double(this->vvvPixmapItemPointer_.at(currentZoomLevel_).size());//для уменьшения

        double tileX = double(double(startWidth_) + double((finishWidth_ - startWidth_) / 2)) * coefficientX;
        double tileY = double(double(startHeight_) + double((finishHeight_ - startHeight_) / 2)) * coefficientY;

        double offsetX = tileX - int(tileX);
        double offsetY = tileY - int(tileY);

        offsetX *= -this->TILESIDE_;
        offsetY *= -this->TILESIDE_;
        qDebug() << "offsetX: " << offsetX << "     offsetY: " << offsetY;

        int offsetScX = this->offsetScaling_.rx();
        int offsetScY = this->offsetScaling_.ry();

        qDebug() << "prevOffsetX: " << offsetScX << "      prevOffsetY: " << offsetScY;

        if (offsetScX + offsetX <= -this->TILESIDE_)
        {
            tileX--;
            this->offsetScaling_.setX(offsetScX + offsetX + this->TILESIDE_);
        } else this->offsetScaling_.setX(offsetScX + offsetX);

        if (offsetScY + offsetY <= -this->TILESIDE_)
        {
            tileY--;
            this->offsetScaling_.setY(offsetScY + offsetY + this->TILESIDE_);
        } else this->offsetScaling_.setY(offsetScY + offsetY);

        int leftT = int(tileX) - this->itemGroupSize_.rwidth() / 2 + 1;    //И ТУТ ЧТО-ТО SMELL A RAT
        int rightT = int(tileX) + this->itemGroupSize_.rwidth() / 2 + 1;
        int upT = int(tileY) - this->itemGroupSize_.height() / 2 + 1;
        int downT = int(tileY) + this->itemGroupSize_.height() / 2 + 1;

        this->RemoveTilesFromPixmapItemGroup(startHeight_, finishHeight_, startWidth_, finishWidth_);

        this->currentZoomLevel_ = _newLayerNumber;

        this->AddTilesToPixmapItemGroup(upT, downT, leftT, rightT);

        this->startWidth_ = leftT;
        this->finishWidth_ = rightT;
        this->startHeight_ = upT;
        this->finishHeight_ = downT;
    }

}

const QPoint& GraphicsItemGroup::getScalingOffset() const
{
    return this->offsetScaling_;
}

const QPoint& GraphicsItemGroup::getPreviousPos() const
{
    return this->previousPos_;
}

const int& GraphicsItemGroup::getCountOfLevels() const
{
    return this->vvvPixmapItemPointer_.size();
}

void GraphicsItemGroup::slotChangeItemGroup()
{    
    if (this->tileOffsetY_ != 0)
        this->ChangeHeightItemGroup(this->tileOffsetY_);      //ОТРИЦАТЕЛЬНЫЙ, КОГДА СМЕЩЕНИЕ _ВНИЗ___подгружать сверху
    if (this->tileOffsetX_ != 0)
        this->ChangeWidthItemGroup(this->tileOffsetX_);    //ОТРИЦАТЕЛЬНЫЙ, КОГДА СМЕЩЕНИЕ _ВПРАВО___подгружать слева
}

void GraphicsItemGroup::InitializeVVV(const QVector<ZoomLevelDescription>& _ZoomLevelDescription)
{
    this->currentZoomLevel_ = 0;
    const QString xCHARACTER("x=");
    const QString yCHARACTER("y=");
    const QString pngCHARACTER(".png");
    QString str("");
    
    if (!this->vvvPixmapItemPointer_.isEmpty())
        this->vvvPixmapItemPointer_.clear();
    if (!this->vvvTilePaths_.isEmpty())
        this->vvvTilePaths_.clear();
    
    for (int level = 0; level < _ZoomLevelDescription.size(); level++)
    {
        const ZoomLevelDescription& ZL = _ZoomLevelDescription.at(level);
        QVector<QVector<QString>> temp_2_PathVector;
        QVector<QVector<QGraphicsPixmapItem*>> temp_2_ItemVector;
        for (int h = 0; h < ZL.getTileAmountH(); h++)
        {
            QVector<QString> temp_1_PathVector;
            QVector<QGraphicsPixmapItem*> temp_1_ItemVector;
            for (int w = 0; w < ZL.getTileAmountW(); w++)
            {
                str.clear();
                str.append(this->storagePath_).append(QString::number(_ZoomLevelDescription.size() - level)).append("/");
                str.append(yCHARACTER).append(QString::number(h)).append(xCHARACTER).append(QString::number(w)).append(pngCHARACTER);
                temp_1_PathVector.push_back(str);
                QGraphicsPixmapItem* pixItem;
                temp_1_ItemVector.push_back(pixItem);
            }
            temp_2_PathVector.push_back(temp_1_PathVector);
            temp_1_PathVector.clear();
            temp_2_ItemVector.push_back(temp_1_ItemVector);
            temp_1_ItemVector.clear();
        }
        this->vvvTilePaths_.push_back(temp_2_PathVector);
        this->vvvPixmapItemPointer_.push_back(temp_2_ItemVector);
    }
    //    this->currentZoomLevel_ = vvvPixmapItemPointer_.size() - 1;
    this->upperLeftVisibleTileNumber_.setX(vvvPixmapItemPointer_.at(currentZoomLevel_).at(0).size() / 2 - this->itemGroupSize_.width() / 6);
    this->upperLeftVisibleTileNumber_.setY(vvvPixmapItemPointer_.at(currentZoomLevel_).size() / 2 - this->itemGroupSize_.height() / 6);
}

void GraphicsItemGroup::CreateTile(const QString& _tilePath, QPixmap& _pixmap)
{
    if (QFile::exists(_tilePath))
    {
        _pixmap.load(_tilePath);
        if (_pixmap.size() != QSize(this->TILESIDE_, this->TILESIDE_))
        {
            _pixmap = _pixmap.scaled(this->TILESIDE_, this->TILESIDE_);
            qDebug() << "_pixmap.size() != QSize(this->TILESIDE_, this->TILESIDE_)____СКЕЙЛЮ";
        }
    }
    else
    {
        qDebug() << "_pixmap not exist!";
        if (_pixmap.size() != QSize(this->TILESIDE_, this->TILESIDE_))
        {
            _pixmap.load("D:/____DEADBEEF/lenna.jpg");
            _pixmap = _pixmap.scaled(this->TILESIDE_, this->TILESIDE_);
        }
        _pixmap.fill(Qt::black);
    }
}

void GraphicsItemGroup::FirstAdditionToItemGroup()
{
    if (!this->vvvPixmapItemPointer_.isEmpty())
    {
        //проверять и очищать если не пустая группа
        int leftWidth, rightWidth, upHeight, downHeight;
        startHeight_ = this->upperLeftVisibleTileNumber_.y() - this->itemGroupSize_.height() / 3;   //внимательнее с выездом за границу
        startWidth_ = this->upperLeftVisibleTileNumber_.x() - this->itemGroupSize_.width() / 3;
        (startHeight_ < 0) ? (startHeight_ = 0) : (startHeight_ = startHeight_);
        (startWidth_ < 0) ? (startWidth_ = 0) : (startWidth_ = startWidth_);
        finishHeight_ = startHeight_ + this->itemGroupSize_.height();
        finishWidth_ = startWidth_ + this->itemGroupSize_.width();
        (finishHeight_ > this->vvvPixmapItemPointer_.at(currentZoomLevel_).size()) ? (finishHeight_ = this->vvvPixmapItemPointer_.at(currentZoomLevel_).size()) : (finishHeight_ = finishHeight_);
        (finishWidth_ > this->vvvPixmapItemPointer_.at(currentZoomLevel_).at(0).size()) ? (finishWidth_ = this->vvvPixmapItemPointer_.at(currentZoomLevel_).at(0).size()) : (finishWidth_ = finishWidth_);

//        this->AddTilesToPixmapItemGroup(upHeight, downHeight, leftWidth, rightWidth);
        this->AddTilesToPixmapItemGroup(startHeight_, finishHeight_, startWidth_, finishWidth_);
    }
    //    qDebug() << this->pos();
    //    this->setX(-this->itemGroupSize_.rwidth() / 3 * this->TILESIDE_);
    //    this->setY(-this->itemGroupSize_.rheight() / 3 * this->TILESIDE_);

    //    this->setPos(-this->itemGroupSize_.rwidth() / 3 * this->TILESIDE_, -this->itemGroupSize_.rheight() / 3 * this->TILESIDE_);
    qDebug() << this->pos();
}

void GraphicsItemGroup::setStoragePath(const QString& _storagePath)
{
    this->storagePath_ = _storagePath;
}

void GraphicsItemGroup::setItemGroupSize(const QSize& _size)
{
    this->itemGroupSize_ = _size;
}

const QSize& GraphicsItemGroup::getItemGroupSize() const
{
    return this->itemGroupSize_;
}

const int& GraphicsItemGroup::getTileSide() const
{
    return this->TILESIDE_;
}

const int& GraphicsItemGroup::getCurrentZoomLevel() const
{
    return this->currentZoomLevel_;
}
