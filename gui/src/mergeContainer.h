/* 
 * File:   executionContainer.h
 * Author: biyanin
 *
 * Created on August 21, 2015, 11:30 AM
 */

#ifndef MERGECONTAINER_H
#define	MERGECONTAINER_H

#include <QWidget>
#include <QProcess>
#include <QDir>
#include <QSignalMapper>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QDesktopServices>
#include <QStackedWidget>
#include <QComboBox>
#include <QToolButton>
#include <QSpacerItem>
#include <QScrollArea>
#include <QStatusBar>
#include <QProgressBar>
#include <QToolBar>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>

#include <confData.h>
#include <confManual.h>
#include <scriptProgress.h>
#include <viewContainer.h>
#include <resizeableStackedWidget.h>
#include <scriptModule.h>
#include <confInterface.h>
#include <confModel.h>
#include <LogViewer.h>
#include <controlBar.h>
#include <levelGroup.h>
#include <resultsModule.h>
#include <reprojectWindow.h>
#include <confEditor.h>

#include "blockContainer.h"

class mergeContainer : public QWidget
{
    Q_OBJECT

public:
    mergeContainer(confData* data, resultsData *results, QWidget *parent = NULL);

public slots:

    void setMerge2DMode();
    void setMerge3DMode();
    void setCustomMode();
    void setSPMode();
    
    void scriptChanged(scriptModule *module, QModelIndex index);
    void merge2DScriptChanged(QModelIndex index);
    void merge3DScriptChanged(QModelIndex index);
    void customScriptChanged(QModelIndex index);
    void singleParticleScriptChanged(QModelIndex index);

    void scriptCompleted(scriptModule *module, QModelIndex index);
    void merge2DScriptCompleted(QModelIndex index);
    void merge3DScriptCompleted(QModelIndex index);
    void customScriptCompleted(QModelIndex index);
    void singleParticleScriptCompleted(QModelIndex index);

    void subscriptActivated(QModelIndex item);
    
    //void maximizeWindow(int option);
    void maximizeLogWindow(bool maximize);
    void maximizeParameterWindow(bool maximize);

    void reload();

    void launchFileBrowser();
    void launchLogBrowser();

    void showSubTitle(bool show);
    
    void updateFontInfo();
    
    void execute(bool halt);
    void stopPlay();
    
    void updateScriptLabel(const QString& label);
    void increaseScriptProgress(int increament);
    void setScriptProgress(int progress);

signals:
    void scriptCompletedSignal();

private:
    
    blockContainer* setupLogWindow();
    blockContainer* setupParameterWindow();
    QToolBar* setupToolbar();
    
    void addToScriptsWidget(QWidget *widget);
    
    confData *mainData;

    scriptModule *merge2DScripts;
    scriptModule *merge3DScripts;
    scriptModule *customScripts;
    scriptModule *singleParticleScripts;

    QStackedWidget* scriptsWidget;
    QListView* subscriptWidget;

    QToolButton* showMerge2DScripts;
    QToolButton* showMerge3DScripts;
    QToolButton* showCustomScripts;
    QToolButton* showSPScripts;
    
    resultsData *results;

    confInterface *parameters;

    QSplitter* centralSplitter;
    QSplitter *centerRightSplitter;

    resizeableStackedWidget *localParameters;

    resultsModule *resultsView;

    LogViewer *logViewer;

    QComboBox* userLevelButtons;
    QComboBox* verbosityControl;

    QHash<uint, int> localIndex;
    
    QProgressBar* progressBar;
    
    QLabel* scriptLabel;
    QLabel* subTitleLabel;
    QPushButton* runButton;
    QPushButton* refreshButton;
    QPushButton* manualButton;

};


#endif	/* EXECUTIONCONTAINER_H */
