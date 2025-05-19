#include "contrastplugin.h"

QString ContrastPlugin::name() {
    return "Contrast";
}

static cv::Mat adjustContrast(const cv::Mat &input, double alpha, int beta = 0) {
    cv::Mat output;
    input.convertTo(output, -1, alpha, beta);
    return output;
}

static QPixmap CvMatToQPixmap(cv::Mat matImage) {
    QImage image_edited;

    if (matImage.channels() == 4) {
        cv::Mat converted;
        cv::cvtColor(matImage, converted, cv::COLOR_BGRA2RGBA);
        image_edited = QImage(converted.data, converted.cols, converted.rows, converted.step, QImage::Format_RGBA8888).copy();
    } else if (matImage.channels() == 3) {
        cv::Mat converted;
        cv::cvtColor(matImage, converted, cv::COLOR_BGR2RGB);
        image_edited = QImage(converted.data, converted.cols, converted.rows, converted.step, QImage::Format_RGB888).copy();
    } else if (matImage.channels() == 1) {
        image_edited = QImage(matImage.data, matImage.cols, matImage.rows, matImage.step, QImage::Format_Grayscale8).copy();
    } else {
        image_edited = QImage();
    }

    return QPixmap::fromImage(image_edited);
}


void ContrastPlugin::edit(const cv::Mat &input, cv::Mat &output, QWidget* parent) {
    cv::Mat original = input.clone();
    output = original.clone();  // Khởi tạo output

    QDialog dialog(parent);
    dialog.setWindowTitle("Adjust Contrast");
    dialog.setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QLabel *label = new QLabel("Contrast: 1.0x", &dialog);
    QSlider *slider = new QSlider(Qt::Horizontal, &dialog);
    QPushButton *okButton = new QPushButton("Apply", &dialog);

    slider->setRange(10, 500); // 100 = 1.0x
    slider->setValue(100);

    // Preview image using QGraphicsView
    QGraphicsView *previewView = new QGraphicsView(&dialog);
    QGraphicsScene *previewScene = new QGraphicsScene(previewView);
    previewView->setScene(previewScene);
    previewView->setFixedHeight(180);  // preview size
    previewView->setAlignment(Qt::AlignCenter);

    layout->addWidget(previewView);
    layout->addWidget(label);
    layout->addWidget(slider);
    layout->addWidget(okButton);

    // Initial preview image
    QPixmap pixmap = CvMatToQPixmap(original);
    QGraphicsPixmapItem *pixmapItem = previewScene->addPixmap(pixmap);
    previewScene->setSceneRect(pixmap.rect());
    QTimer::singleShot(0, &dialog, [&]() {
        previewView->fitInView(pixmapItem, Qt::KeepAspectRatio);
    });

    // Update preview on slider move
    QObject::connect(slider, &QSlider::valueChanged, &dialog, [=]() mutable {
        double alpha = slider->value() / 100.0;
        label->setText(QString("Contrast: %1x").arg(alpha, 0, 'f', 2));
        cv::Mat adjusted = adjustContrast(original, alpha);
        QPixmap newPixmap = CvMatToQPixmap(adjusted);
        pixmapItem->setPixmap(newPixmap);
        previewScene->setSceneRect(newPixmap.rect());
        previewView->fitInView(pixmapItem, Qt::KeepAspectRatio);
    });

    // On apply: save output and close
    QObject::connect(okButton, &QPushButton::clicked, &dialog, [slider, &output, original, &dialog]() {
        double alpha = slider->value() / 100.0;
        output = adjustContrast(original, alpha);
        dialog.accept();
    });

    dialog.exec();
}
