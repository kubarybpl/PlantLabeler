
/**
 * @file mainwindow.h
 * @brief Definition of the MainWindow class, which is the main interface for the application.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "interactivescene.h"
#include <QWheelEvent>
#include <leftpanel.h>
#include <rightpanel.h>

/**
 *  @class MainWindow
 *  @brief This class extends QMainWindow, integreates other interactiveScene, leftPanel, and rightPanel class togethers
 */

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    /**
     * @brief Constructs MainWindow.
     * @param parent QWidget class, defaul nullptr.
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief Paints custom cursor, so it represents size and color of the brush, contains zoom in formula
     */
    void setCursor();

protected:
    /**
     * @brief Reimplements wheelEvent, so image zoom can be performed.
     */
    void wheelEvent(QWheelEvent *event) override;

private:
    Ui::MainWindow *ui;             ///< Pointer to the UI setup class.
    rightPanel *toolBox;            ///< Pointer to the right panel.
    leftPanel *leftWidget;          ///< Pointer to the left panel.
    QGraphicsView *graphicsView;    ///< View that displays the interactiveScene.
    interactiveScene *scene;        ///< Scene which handles image loading and interaction.
    double zoom;                    ///< Current zoom.

    /**
     * @brief Gets next file path from leftWidget and passes it to interactiveScene, selects this file in tree view.
     */
    void nextImage();

    /**
     * @brief Gets previous file path from leftWidget and passes it to interactiveScene, selects this file in tree view.
     */
    void previousImage();

};

#endif // MAINWINDOW_H
