//
// Created by Lun0xxx on 13/08/2023.
//

#ifndef WEBBROWSER_WEBBROWSER_HPP
#define WEBBROWSER_WEBBROWSER_HPP

#include <QMainWindow>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QLineEdit>
#include <QTabWidget>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QVBoxLayout>
#include <QProgressBar>

class WebBrowser : public QMainWindow {
    Q_OBJECT
    public:
        WebBrowser();
        ~WebBrowser();
        QWebEngineView *actualPage();
        QWidget *createTabPageWeb(QString url);

    public slots:
        void addTab();
        void closeTab();
        void nextPage();
        void previousPage();
        void pageStop();
        void reload();
        void goToHome();
        void goToUrl();
        void changeWindowTitle(const QString &title);
        void changeTitle(int index);
        void changeUrl(QUrl url);
        void startLoading();
        void loading(int progression);
        void endLoading(bool b);
        void showAbout();
        void showAboutQt();

    private:
        QMenu *m_fichierMenu;
        QMenu *m_navigationMenu;
        QMenu *m_helpMenu;

        QAction *m_openTabAction;
        QAction *m_quitTabAction;
        QAction *m_previousPageAction;
        QAction *m_nextPageAction;
        QAction *m_stopPageLoadingAction;
        QAction *m_reloadPageAction;
        QAction *m_toHomePageAction;
        QAction *m_goToLinkAction;
        QAction *m_aboutAction;
        QAction *m_aboutQtAction;

        QToolBar *m_toolBar;

        QLineEdit *m_searchBar;

        QTabWidget *m_onglets;

        QProgressBar *m_loadBar;
};

#endif //WEBBROWSER_WEBBROWSER_HPP