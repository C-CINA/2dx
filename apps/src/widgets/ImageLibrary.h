#ifndef IMAGELIBRARY_H
#define IMAGELIBRARY_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QTimer>
#include <QToolButton>

#include "ImageThumbnails.h"
#include "FileWatcher.h"
#include "BlockContainer.h"

class ImageLibrary : public QWidget { 
    Q_OBJECT
    
public:
    ImageLibrary(QWidget* parent=0);
    ImageThumbnails* thumbnailContainer() {
        return thumbnails_;
    }
    
public slots:
    void updateData(const QString& imagePath="");
    void updateChecks();
    void timedLoad();
    void setHeaderTitle(const QString& title);
    
signals:
    void shouldLoadImage(const QString&);

private:
    QGridLayout* fillFormLayout(const QStringList& labels);
    void updateFormData(const QString& imagePath, const QStringList& params);
    void setSelectionCount(int count);
    QWidget* setupHeader();
    
    QWidget* expandedWidget;
    
    ImageThumbnails* thumbnails_;
    QGridLayout* dataLayout;
    QGroupBox* dataBox;
    QList<QLabel*> valueLabels;
    
    FileWatcher watcher;
    QTimer timer;
    
    QLabel* headerTitle;
    QLabel* selectionLabel;
    QToolButton* infoButton;
    
};

#endif /* IMAGELIBRARY_H */

