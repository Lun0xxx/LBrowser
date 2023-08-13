//
// Created by Lun0xxx on 13/08/2023.
//

#include "../headers/WebBrowser.hpp"
#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QMainWindow>
#include <QStatusBar>

/**
 * FR : Fonction constructeur permettant de générer le navigateur
 * EN : Constructor function for generating the web browser
 */
WebBrowser::WebBrowser() {
    setMinimumWidth(600);
    setMinimumHeight(500);

    m_fichierMenu = menuBar()->addMenu("&Fichier");
    m_navigationMenu = menuBar()->addMenu("&Navigation");
    m_helpMenu = menuBar()->addMenu("?");

    m_openTabAction = new QAction("Ouvrir un onglet", m_fichierMenu);
    m_quitTabAction = new QAction("Fermer l'onglet", m_fichierMenu);
    m_previousPageAction = new QAction("Page précédente", m_navigationMenu);
    m_nextPageAction = new QAction("Page suivante", m_navigationMenu);
    m_stopPageLoadingAction = new QAction("Arrêter le chargement", m_navigationMenu);
    m_reloadPageAction = new QAction("Recharger la page", m_navigationMenu);
    m_toHomePageAction = new QAction("Revenir au menu", m_navigationMenu);
    m_goToLinkAction = new QAction("Aller à la page", m_navigationMenu);
    m_aboutAction = new QAction("A propos", m_helpMenu);
    m_aboutQtAction = new QAction("A propos de Qt", m_helpMenu);

    m_searchBar = new QLineEdit;
    m_searchBar->setPlaceholderText("https://www.github.com/Lun0xxx");

    m_previousPageAction->setIcon(QIcon("leftArrowIcon.png"));
    m_nextPageAction->setIcon(QIcon("rightArrowIcon.png"));
    m_stopPageLoadingAction->setIcon(QIcon("crossIcon.png"));
    m_reloadPageAction->setIcon(QIcon("reloadIcon.png"));
    m_toHomePageAction->setIcon(QIcon("homeIcon.png"));
    m_goToLinkAction->setIcon(QIcon("goToIcon.png"));
    m_aboutAction->setIcon(QIcon("questionMarkIcon.png"));

    m_previousPageAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Left));
    m_nextPageAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Right));
    m_stopPageLoadingAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    m_reloadPageAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    m_toHomePageAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    m_goToLinkAction->setShortcut(QKeySequence(Qt::Key_Return));

    m_fichierMenu->addAction(m_openTabAction);
    m_fichierMenu->addAction(m_quitTabAction);

    m_navigationMenu->addAction(m_previousPageAction);
    m_navigationMenu->addAction(m_nextPageAction);
    m_navigationMenu->addAction(m_stopPageLoadingAction);
    m_navigationMenu->addAction(m_reloadPageAction);
    m_navigationMenu->addAction(m_toHomePageAction);
    m_navigationMenu->addAction(m_goToLinkAction);

    m_helpMenu->addAction(m_aboutAction);
    m_helpMenu->addAction(m_aboutQtAction);

    m_toolBar = addToolBar("Browser Tool Bar");
    m_toolBar->addAction(m_previousPageAction);
    m_toolBar->addAction(m_nextPageAction);
    m_toolBar->addAction(m_stopPageLoadingAction);
    m_toolBar->addAction(m_reloadPageAction);
    m_toolBar->addAction(m_toHomePageAction);
    m_toolBar->addWidget(m_searchBar);
    m_toolBar->addAction(m_goToLinkAction);

    m_onglets = new QTabWidget(this);
    m_onglets->addTab(createTabPageWeb("http://www.google.com"), "Nouvel onglet");

    statusBar();
    m_loadBar = new QProgressBar();
    m_loadBar->setMinimum(0);
    m_loadBar->setMaximum(100);
    m_loadBar->setVisible(false);
    statusBar()->addWidget(m_loadBar);

    setCentralWidget(m_onglets);

    connect(m_openTabAction, SIGNAL(triggered(bool)), this, SLOT(addTab()));
    connect(m_quitTabAction, SIGNAL(triggered(bool)), this, SLOT(closeTab()));
    connect(m_previousPageAction, SIGNAL(triggered(bool)), this, SLOT(previousPage()));
    connect(m_nextPageAction, SIGNAL(triggered(bool)), this, SLOT(nextPage()));
    connect(m_stopPageLoadingAction, SIGNAL(triggered(bool)), this, SLOT(pageStop()));
    connect(m_reloadPageAction, SIGNAL(triggered(bool)), this, SLOT(reload()));
    connect(m_toHomePageAction, SIGNAL(triggered(bool)), this, SLOT(goToHome()));
    connect(m_goToLinkAction, SIGNAL(triggered(bool)), this, SLOT(goToUrl()));
    connect(m_onglets, SIGNAL(currentChanged(int)), this, SLOT(changeTitle(int)));
    connect(m_aboutAction, SIGNAL(triggered(bool)), this, SLOT(showAbout()));
    connect(m_aboutQtAction, SIGNAL(triggered(bool)), this, SLOT(showAboutQt()));
}

/**
 * FR : Fonction destructrice permettant de libérer la mémoire
 * EN : Destructor function for freeing memory
 */
WebBrowser::~WebBrowser() {
    delete m_fichierMenu;
    delete m_navigationMenu;
    delete m_helpMenu;
    delete m_openTabAction;
    delete m_quitTabAction;
    delete m_previousPageAction;
    delete m_nextPageAction;
    delete m_stopPageLoadingAction;
    delete m_reloadPageAction;
    delete m_toHomePageAction;
    delete m_goToLinkAction;
    delete m_aboutAction;
    delete m_aboutQtAction;
    delete m_toolBar;
    delete m_searchBar;
    delete m_onglets;
    delete m_loadBar;
};

/**
 * FR : Fonction permettant de récupérer la page actuelle
 * EN : Function for retrieving the actual page
 * @return
 */
QWebEngineView *WebBrowser::actualPage() {
    return m_onglets->currentWidget()->findChild<QWebEngineView *>();
}

/**
 * FR : Fonction permettant de créer un onglet
 * EN : Function for creating a tab
 * @param url
 * @return
 */
QWidget *WebBrowser::createTabPageWeb(QString url) {
    QWidget *tabPage = new QWidget;
    QWebEngineView *webPage = new QWebEngineView(tabPage);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(webPage);
    tabPage->setLayout(layout);

    if(url.isEmpty()) {
        webPage->load(QUrl("http://www.google.com"));
    } else {
        if(url.left(7) != "http://" && url.left(8) != "https://") {
            url = "http://" + url;
        }
        webPage->load(QUrl(url));
    }

    connect(webPage, SIGNAL(titleChanged(const QString &)), this, SLOT(changeWindowTitle(const QString &)));
    connect(webPage, SIGNAL(urlChanged(const QUrl &)), this, SLOT(changeUrl(QUrl)));
    connect(webPage, SIGNAL(loadStarted()), this, SLOT(startLoading()));
    connect(webPage, SIGNAL(loadProgress(int)), this, SLOT(loading(int)));
    connect(webPage, SIGNAL(loadFinished(bool)), this, SLOT(endLoading(bool)));

    return tabPage;
}

/**
 * FR : Fonction permettant d'ajouter un onglet
 * EN : Function for adding a tab
 */
void WebBrowser::addTab() {
    int indexNewTab = m_onglets->addTab(createTabPageWeb("http://www.google.com"), "Nouvel onglet");
    m_onglets->setCurrentIndex(indexNewTab);
}

/**
 * FR : Fonction permettant de fermer un onglet
 * EN : Function for closing a tab
 */
void WebBrowser::closeTab() {
    if(m_onglets->count() > 1) {
        m_onglets->removeTab(m_onglets->currentIndex());
    } else {
        QMessageBox::critical(this, "Erreur", "Vous devez laisser au minimum un onglet ouvert.");
    }
}

/**
 * FR : Fonction permettant d'aller à la page suivante grâce au bouton
 * EN : Function for going to the next page thanks to the button
 */
void WebBrowser::nextPage() {
    actualPage()->forward();
}

/**
 * FR : Fonction permettant d'aller à la page précédente grâce au bouton
 * EN : Function for going to the previous page thanks to the button
 */
void WebBrowser::previousPage() {
    actualPage()->back();
}

/**
 * FR : Fonction permettant de rafraîchir la page grâce au bouton
 * EN : Function for refreshing the page thanks to the button
 */
void WebBrowser::reload() {
    actualPage()->reload();
}

/**
 * FR : Fonction permettant de stopper le chargement d'une page grâce au bouton
 * EN : Function for stopping the loading of the page thanks to the button
 */
void WebBrowser::pageStop() {
    actualPage()->stop();
}

/**
 * FR : Fonction permettant de retourner à la page d'accueil grâce au bouton
 * EN : Function for going back to the home page thanks to the button
 */
void WebBrowser::goToHome() {
    actualPage()->setUrl(QUrl("http://www.google.com"));
}

/**
 * FR : Fonction permettant d'aller sur le lien ou de chercher le mot clé sur le navigateur
 * EN : Function for going to the URL or the keyword on the browser
 */
void WebBrowser::goToUrl() {
    if(m_searchBar->text().isEmpty()) {
        return;
    } else {
        if(m_searchBar->text().left(7) == "http://" || m_searchBar->text().left(8) == "https://") {
            actualPage()->setUrl(QUrl(m_searchBar->text()));
        } else {
            actualPage()->load("http://www.google.com/search?q=" + m_searchBar->text());
        }
        m_searchBar->setText(actualPage()->url().toString());
    }
}

/**
 * FR : Fonction permettant de changer le titre de l'application et de l'onglet
 * EN : Function for changing the title of the application and the tab
 * @param title
 */
void WebBrowser::changeWindowTitle(const QString &title) {
    if(title.size() > 40) {
        setWindowTitle(title.left(40) + "... - LBrowser");
        m_onglets->setTabText(m_onglets->currentIndex(), title.left(30) + "...");
    } else {
        setWindowTitle(title + " - LBrowser");
        m_onglets->setTabText(m_onglets->currentIndex(), title);
    }
}

/**
 * FR : Fonction permettant de changer l'URL affiché dans la barre de recherche
 * EN : Function for changing the displayed URL on the search bar
 * @param url
 */
void WebBrowser::changeUrl(QUrl url) {
    m_searchBar->setText(url.toString());
}

/**
 * FR : Fonction permettant de changer le titre de l'application et l'URL affiché sur la barre de recherche
 * EN : Function for changing the title of the application and the URL displayed on the search bar
 * @param index
 */
void WebBrowser::changeTitle(int index) {
    changeWindowTitle(actualPage()->title());
    changeUrl(actualPage()->url());
}

/**
 * FR : Fonction permettant d'afficher une barre de progression lorsque la page commence à charger
 * EN : Function for displaying a progression bar when the web page starts loading
 */
void WebBrowser::startLoading() {
    m_loadBar->setVisible(true);
}

/**
 * FR : Fonction permettant de modifier la valeur de la barre de progression en fonction du chargement de la page
 * EN : Function for editing the progression bar value based on the loading of the page
 * @param progression
 */
void WebBrowser::loading(int progression) {
    m_loadBar->setValue(progression);
}

/**
 * FR : Fonction permettant de faire disparaître la barre de progression lorsque la page est finie de charger et d'afficher un message "Prêt"
 * EN : Function for making disappear the progression bar when the page is end loading and to display a message "Ready"
 * @param b
 */
void WebBrowser::endLoading(bool b) {
    m_loadBar->setVisible(false);
    statusBar()->showMessage("Prêt", 2000);
}

/**
 * FR : Fonction permettant d'afficher des informations sur l'application
 * EN : Function for displaying informations about the application
*/
void WebBrowser::showAbout() {
    QMessageBox::information(this, "Informations sur LBrowser", "Développeur : Lunoxx\n\nDéveloppé le : Dimanche 13 Août\n\nCeci est un petit navigateur Web ayant quelques petites fonctionnalités basiques.\n\nMerci.");
}

/**
 * FR : Fonction permettant d'afficher des informations sur la librarie Qt
 * EN : Function for displaying informations about the Qt Library
*/
void WebBrowser::showAboutQt() {
    QMessageBox::information(this, "Informations sur Qt", "Qt 6.5.2\n\nQt est une librarie permettant de développer des applications sur différents systèmes d'exploitation et s'adapte à ceux-ci.\n\nCette librarie est open source, sous licence GPL.");
}