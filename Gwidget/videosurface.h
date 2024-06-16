#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QAbstractVideoSurface>

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit VideoSurface(QObject *parent = nullptr);

signals:
    void showImage(QImage img);
public slots:

    // QAbstractVideoSurface interface
public:
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const;
    bool present(const QVideoFrame &frame);
};

#endif // VIDEOSURFACE_H
