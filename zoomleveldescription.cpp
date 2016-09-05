#include "zoomleveldescription.h"

ZoomLevelDescription::ZoomLevelDescription()
{
//    qDebug() << "ZoomLevel Ctor";
}

ZoomLevelDescription::~ZoomLevelDescription()
{
//    qDebug() << "~ZoomLevel Dtor";
}

void ZoomLevelDescription::setValue(const int &_v)
{
    this->value_ = _v - 1;
}
const int& ZoomLevelDescription::getValue() const
{
    return this->value_;
}

void ZoomLevelDescription::setWidth(const int& _w)
{
    this->width_ = _w - 1;
}
const int& ZoomLevelDescription::getWidth() const
{
    return this->width_;
}

void ZoomLevelDescription::setHeight(const int &_h)
{
    this->height_ = _h - 1;
}
const int& ZoomLevelDescription::getHeight() const
{
    return this->height_;
}

void ZoomLevelDescription::setTileAmountW(const int &_taw)
{
    this->tile_amount_w_ = _taw - 1;
}
const int& ZoomLevelDescription::getTileAmountW() const
{
    return this->tile_amount_w_;
}

void ZoomLevelDescription::setTileAmountH(const int &_tah)
{
    this->tile_amount_h_ = _tah - 1;
}
const int& ZoomLevelDescription::getTileAmountH() const
{
    return this->tile_amount_h_;
}
