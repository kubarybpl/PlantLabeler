
/**
 * @file rightpanel.h
 * @brief Definition of the rightPanel class which extends QWidget class for providing drawing tools.
 */

#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H


#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>

/**
 * @class rightPanel
 * @brief The rightPanel class provides drawing tools and next navigation buttons to manage image editing.
 *
 * This class contains interface elements such as sliders to adjust brush size, buttons to apply different colors,
 * and navigation buttons to move between images. It also offers visibility toggles for masks and backgrounds.
 */

class rightPanel : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a rightPanel widget.
     * @param parent Pointer to QWidget parent class, default is nullptr.
     */
    rightPanel(QWidget *parent);

    ~rightPanel();

private:
    QSlider *brushSizeSlider;                   ///< Slider to adjust the brush size.
    QPushButton *undo;                          ///< Button to undo the last drawing operation.
    QPushButton *redo;                          ///< Button to restore an undone drawing.
    QPushButton *nextButton;                    ///< Button to navigate to the next image.
    QPushButton *previousButton;                ///< Button to navigate to the previous image.
    QPushButton *dirtButton;                    ///< Button to apply transparent.
    QPushButton *plantButton;                   ///< Button to apply green color.
    QPushButton *weedButton;                    ///< Button to apply red color.
    QPushButton *maskVisibiltyButton;           ///< Button to toggle mask visibility.
    QPushButton *backgroundVisibilityButton;    ///< Button to toggle background visibility.

public slots:
    /**
     * @brief Toogles text on the visibility buttons.
     * @param msg Message with the information what have been changed.
     */
    changeButton(QString &msg);

private slots:
    /**
     * @brief Slot triggered on color buttons, emits colorSignal(const QString &colorName) to change brush color.
     */
    colorClicked();

    /**
     * @brief Slot triggered on nextButton, sends signal to load next image to edit.
     */
    nextClicked();

    /**
     * @brief Slot triggered on visibilityButton emits visibilitySignal(const QString &visibility).
     */
    toggleView();

signals:
    /**
     * @brief Signal to change brush color.
     * @param colorName contains string name of color.
     */
    void colorSignal(const QString &colorName);

    /**
     * @brief Signal to toggle visibility of mask/background.
     * @param visibility String of thing to be toggled.
     */
    void visibilitySignal(const QString &visibility);

    /**
     * @brief Signal to change size of brush.
     */
    void sliderChanged(int size);

    /**
     * @brief Signal to change image to edit.
     */
    nextButtonClicked();

    /**
     * @brief Signal to change image to edit.
     */
    previousButtonClicked();

    /**
     * @brief Signal to undo last drawing operation.
     */
    undoSignal();

    /**
     * @brief Signal to restore undone drawing operation.
     */
    redoSignal();


    /**
     * @brief Signal emitted for setting transparent brush.
     */
    void dirtSignal();

    /**
     * @brief Signal emitted for setting green brush.
     */
    void plantSignal();

    /**
     * @brief Signal emitted for setting red brush.
     */
    void weedSignal();
};

#endif // RIGHTPANEL_H
