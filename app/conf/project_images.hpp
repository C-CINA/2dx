/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   project_images.hpp
 * Author: biyanin
 *
 * Created on June 9, 2016, 9:10 PM
 */

#ifndef PROJECT_IMAGES_HPP
#define PROJECT_IMAGES_HPP

#include <iostream>

#include <QSettings>
#include <QString>
#include <QStringList>

#include "repositories/project_repo.hpp"

namespace tdx {
    
    namespace app {
        
        namespace conf {
            
            class ProjectImages : public QSettings {

            public:
                ProjectImages()
                : QSettings(repo::ProjectRepo::Instance().imagesConfFilePath(), QSettings::Format::IniFormat) {
                }
                
                int getLastImageNumber() {
                    if(images().isEmpty()) return 0;
                    else return images().last().toInt();
                }
                
                QString imagePath(const QString& imageUid) {
                    beginGroup("images");
                    beginGroup(imageUid);
                    QString val = value("path").toString();
                    endGroup();
                    endGroup();
                    return val;
                }
                
                QString imageGroup(const QString& imageUid) {
                    beginGroup("images");
                    beginGroup(imageUid);
                    QString val = value("group").toString();
                    endGroup();
                    endGroup();
                    return val;
                }
                
                
                void setImageProperties(const QString& imageUid, const QString& path, const QString& group) {
                    beginGroup("images");
                    beginGroup(imageUid);
                    setValue("path", path);
                    setValue("group", group);
                    endGroup();
                    endGroup();
                    addImageGroup(group);
                }
                
                QStringList images() {
                    beginGroup("images");
                    QStringList ims = childGroups();
                    endGroup();
                    return ims;
                }
                
                void addImageGroup(const QString& group) {
                    QStringList paths = imageGroups();
                    if(paths.contains(group)) return; 
                    paths.append(group);
                    clear();
                    beginWriteArray("groups");
                    for (int i = 0; i < paths.size(); ++i) {
                        setArrayIndex(i);
                        setValue("group", paths.at(i));
                    }
                    endArray();
                }

                QStringList imageGroups() {
                    QStringList paths;
                    int size = beginReadArray("groups");
                    for (int i = 0; i < size; ++i) {
                        setArrayIndex(i);
                        if(value("group").toString() != "") paths << value("group").toString();
                    }
                    endArray();
                    return paths;
                }
                
            };
        }
    }
}

#endif /* PROJECT_IMAGES_HPP */

