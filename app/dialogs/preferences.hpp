/* 
 * Author: Nikhil Biyani - nikhil(dot)biyani(at)gmail(dot)com
 *
 * This file is a part of 2dx.
 * 
 * 2dx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any 
 * later version.
 * 
 * 2dx is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public 
 * License for more details <http://www.gnu.org/licenses/>.
 */

#ifndef PREFERENCES_HPP
#define PREFERENCES_HPP

#include <QObject>
#include <QDialog>

class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QSettings;
class QFont;

namespace tdx {

    namespace app {

        namespace dialog {

            class PreferencesDialog : public QDialog {
                Q_OBJECT

            public:
                PreferencesDialog();

            public slots:
                void changePage(QListWidgetItem* current, QListWidgetItem* previous);

            private:
                void createIcons();
                
                QWidget* getApperancePage();
                QWidget* getViewersPage();
                QWidget* getAppsPage();

                QListWidget* contentsWidget_;
                QStackedWidget* pagesWidget_;
            };


        }
    }
}

#endif 

