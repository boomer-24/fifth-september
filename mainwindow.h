#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsitemgroup.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void resizeEvent(QResizeEvent* event);
    void on_pbOpenXml_clicked();
    void on_pushButton_clicked();

    void on_pb_plusZL_clicked();
    void on_pb_minusZL_clicked();

    void on_pb_setPos_clicked();
    void on_pb_precisionOffset_clicked();

    void on_pb_showOffset_clicked();

    void slotCenterOn();

signals:
    void signalCenterOn();

private:
    Ui::MainWindow* ui_;
    QGraphicsScene* scene_;
    ImageDescription imgDesc_;
    GraphicsItemGroup* itemGroup_;

    int zoomLevel_ = 0;
};

#endif // MAINWINDOW_H
