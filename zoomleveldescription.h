#ifndef ZOOMLEVELDESCRIPTION_H
#define ZOOMLEVELDESCRIPTION_H

#include <QDebug>

class ZoomLevelDescription
{
public:
    ZoomLevelDescription();
    ~ZoomLevelDescription();

public:
    void setValue(const int& _v);
    const int& getValue()const;

    void setWidth(const int& _w);
    const int& getWidth()const;

    void setHeight(const int& _h);
    const int& getHeight()const;

    void setTileAmountW(const int& _taw);
    const int& getTileAmountW()const;

    void setTileAmountH(const int& _tah);
    const int& getTileAmountH()const;

private:
    int value_, width_, height_, tile_amount_w_, tile_amount_h_;
};

#endif // ZOOMLEVELDESCRIPTION_H
