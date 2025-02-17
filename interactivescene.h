
/**
 * @file interactivescene.h
 * @brief Header file for the interactiveScene class, which provides a custom interactive scene for image editing.
 */

#ifndef INTERACTIVESCENE_H
#define INTERACTIVESCENE_H


#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPathItem>
#include <QStack>
#include <QGraphicsSceneWheelEvent>
#include <QPainter>
#include <QKeyEvent>


#undef slots // Prevent "slots" names from qt and libtorch to conflict
#include <torch/script.h>
#include<torch/torch.h>
#define slots Q_SLOTS

#define MODEL_PATH "E:/One Drive/OneDrive - Politechnika Warszawska/Magisterka/model/"

/**
 * @class interactiveScene
 * @brief The interactiveScene class extends QGraphicsScene to provide functionalities such as drawing, undo/redo, and image masking.
 */

class interactiveScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /**
     * @brief interactiveScene::interactiveScene.
     * @param parent The parent QGraphicsView, default nullptr.
     */
    interactiveScene(QGraphicsView *parent);

    /**
     * Sets the image for editing within the scene, also checks and loads a mask if available.
     * @brief set image on screen.
     * @param imagePath Path to image file to set.
     */
    void setImageItem(const QString &imagePath);

    /**
     * Predicts mask for actual image.
     * @brief Predicts mask.
     * @param imagePath Path to image file to set.
     * @throws c10::Error
     */
    void inference();

    /**
     * @brief sets the color of the pen used for drawing.
     * @param color The color name to set.
     * "Ziemia" - ground, "Roślina" - plant, "Chwast" - weed.
     */
    void setColor(QString color);

    /**
     * @brief setModel Loads chosen model
     * @param modelFileName
     * @throws c10::Error
     */
    void setModel(QString modelFileName = "unet_init.pt");

    /**
     * @brief Saves the mask.
     */
    void saveMask();

    /**
     * @brief Continue to the next image in file tree, saving the current state and clearing the undo/redo stacks.
     * @param path The QString path to the next image to load.
     */
    void nextImage(QString path);

    /**
     * @brief Returns to the previous image in file tree, saving the current state and clearing the undo/redo stacks.
     * @param path The QString path to the previous image to load.
     */
    void previousImage(QString path);

    /**
     * @brief Returns a pointer to the current pen used in the scene.
     * @return A pointer to QPen.
     */
    QPen* getPen();

     /**
     * @brief Returns path of image being edited.
     * @return QString of current image path.
     */
    QString getPath();

    /**
     * @brief Undo last editing operation.
     */
    void undo();

    /**
     * @brief Redo editing operation.
     */
    void redo();

    /**
     * @brief Change size of the brush.
     * @param size The new width of brush.
     */
    void setBrushSize(int size);

    /**
     * @brief Change size of the brush.
     * @param value Opacity in scale 0-10, later convertet to percents.
     */
    void changeMaskOpacity(int value);

public slots:
    /**
     * Show or hide background/mask, if success emit signal to change text on button.
     * @brief sets visibility of mask/background.
     * @param visibility A command string, indicates change.
     */
    void setVisibility(QString visibility);

    /**
     * @brief Emit signal to change cursor.
     */
    void setCursor();

protected:

    /**
     *  @brief Overriding left mouse button pressing event.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     *  @brief Overriding left mouse button moving event.
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     *  @brief Overriding left mouse button relase event.
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     * @brief Overriding wheelEvent for hiding/showing mask/background
     * @param event
     */
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;


private:
    bool isDrawing;                     ///< Flag to check if currently drawing.
    QString currentPath;                ///< Path of the currently loaded image.
    QGraphicsPixmapItem *image;         ///< QGraphicsItem for displaying the main image.
    QGraphicsPixmapItem *frontItem;     ///< QGraphicsItem for displaying the editable layer.
    QPixmap front;                      ///< Pixmap used for the editable front layer.
    QStack<QPixmap> undoStack;          ///< Stack for undo operations.
    QStack<QPixmap> redoStack;          ///< Stack for redo operations.
    QPen pen;                           ///< Pen used for drawing on the image.
    float opacity;                      ///< Opacity of the mask.
    bool isZpressed;                    ///< Flag to check if 'z' is pressed
    bool isXpressed;                    ///< Flag to check if 'x' is pressed
    QPointF lastPoint;                  ///< Last point registered in drawing.
    bool modified;                      ///< Flag to check if file is modified.
    torch::jit::script::Module model;   ///< Neural network model



signals:
    /**
     * @brief Signal to change the button text in the interface.
     * @param msg The new button text.
     */
    void changeButton(QString &msg);

    /**
     * @brief Signal to change the cursor appearance.
     */
    void changeCursor();

};

#endif // INTERACTIVESCENE_H
