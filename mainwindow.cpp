#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    this->scene_ = new QGraphicsScene;
    this->ui_->graphicsView->setScene(this->scene_);
    this->itemGroup_ = new GraphicsItemGroup;
    this->scene_->addItem(this->itemGroup_);
    this->itemGroup_->setFlag(QGraphicsItem::ItemIsMovable);

    QObject::connect(this, SIGNAL(signalCenterOn()), this, SLOT(slotCenterOn()));
}

MainWindow::~MainWindow()
{
    delete ui_;
    //    delete this->scene_;
    delete this->itemGroup_;
}

void MainWindow::resizeEvent(QResizeEvent* event)   //  ИЗМЕНЯТЬ И ItemGroupSize!!!
{
    this->ui_->graphicsView->setFixedSize(this->size() - QSize(this->ui_->graphicsView->x() + 5, this->ui_->graphicsView->y() + 35));
    this->itemGroup_->setItemGroupSize((this->ui_->graphicsView->size() / this->itemGroup_->getTileSide() + QSize(1,1)) * 3);    //давай без хардкода, а И КАКОГО ХРЕНА QSize(1,0)??
    Q_UNUSED(event);
}

void MainWindow::on_pbOpenXml_clicked()
{
    QString filePath = (QFileDialog::getOpenFileName(this,tr("Open xml file"), "D:/___TILES/w_tif", tr("*.*"))); //  make only XML
    if (!filePath.isEmpty())
    {
        QString pathToStorage("");
        QStringList pathStrList = filePath.split("/");
        for (int i = 0; i < pathStrList.size() - 1; i++) // -1 чтобы без ".xml"
        {
            pathToStorage.append(pathStrList.at(i)).append("/");
        }
        pathToStorage.append("1_");
        this->itemGroup_->setStoragePath(pathToStorage);
        this->imgDesc_.Initialize(filePath);
        this->itemGroup_->InitializeVVV(imgDesc_.getLayerDescriptions());
        this->zoomLevel_ = this->itemGroup_->getCurrentZoomLevel();
    }else qDebug() << "xml ne load :-/";
}

void MainWindow::on_pushButton_clicked()
{
    this->itemGroup_->FirstAdditionToItemGroup();
    //        emit signalCenterOn();  //НЕ РАБОТАЕТ СКОТИНА
    //    this->ui_->graphicsView->centerOn(this->itemGroup_);

}

void MainWindow::on_pb_setPos_clicked()  //invoke signalCenterOn()
{
    //    this->ui_->graphicsView->centerOn(this->itemGroup_);
    emit signalCenterOn();
}

void MainWindow::on_pb_plusZL_clicked()
{
    if (this->zoomLevel_ < this->itemGroup_->getCountOfLevels() - 1)
    {
        this->zoomLevel_++;
        this->ui_->labelLevel->setNum(this->zoomLevel_);
        this->itemGroup_->increaseMagnification(this->zoomLevel_);
        this->ui_->graphicsView->centerOn(this->itemGroup_);
        QPoint p(itemGroup_->getScalingOffset());
        this->itemGroup_->setPos(itemGroup_->pos().rx() + p.rx(), itemGroup_->pos().ry() + p.ry());
    }
}

void MainWindow::on_pb_minusZL_clicked()
{
    if (this->zoomLevel_ > 0)
    {
        this->zoomLevel_--;
        this->ui_->labelLevel->setNum(this->zoomLevel_);
        this->itemGroup_->decreaseMagnification(this->zoomLevel_);
    }
}

void MainWindow::slotCenterOn()
{    
    this->ui_->graphicsView->centerOn(this->itemGroup_);
    qDebug() << "itemGroup_->pos() after centerOn: " << itemGroup_->pos();
    //    QPoint p(itemGroup_->getScalingOffset());
    //    this->itemGroup_->setPos(itemGroup_->pos().rx() - p.rx(), itemGroup_->pos().ry() - p.ry());
    qDebug() << "slotCenterOn()...";

//    QPoint p(itemGroup_->getScalingOffset());
    //    this->itemGroup_->setPos(itemGroup_->pos().rx() - p.rx(), itemGroup_->pos().ry() + p.ry());
}

void MainWindow::on_pb_showOffset_clicked() //qDebug() << itemgroup.pos()
{
    qDebug() << "itemGroup.pos(): " << this->itemGroup_->pos();
}


void MainWindow::on_pb_precisionOffset_clicked()
{
    QPoint p(itemGroup_->getScalingOffset());
    this->itemGroup_->setPos(itemGroup_->pos().rx() + p.rx(), itemGroup_->pos().ry() + p.ry());
}
