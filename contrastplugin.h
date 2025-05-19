#ifndef CONTRAST_PLUGIN_H
#define CONTRAST_PLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QString>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "PhotoEditorPluginInterface.h"

class ContrastPlugin: public QObject, public PhotoEditorPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PHOTO_EDITOR_PLUGIN_INTERFACE_IID)
    Q_INTERFACES(PhotoEditorPluginInterface)
public:
    QString name() override;
    void edit(const cv::Mat &input, cv::Mat &output, QWidget* parent) override;
};

#endif // CONTRAST_PLUGIN_H
