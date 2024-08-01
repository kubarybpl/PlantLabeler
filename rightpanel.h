
#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H


#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>

class rightPanel : public QWidget
{
    Q_OBJECT
public:
    rightPanel(QWidget *parent);
    ~rightPanel();

private:
    QSlider *brushSizeSlider;
    QPushButton *undo;
    QPushButton *redo;
    QPushButton *nextButton;
    QPushButton *previousButton;
    QPushButton *dirtButton;
    QPushButton *plantButton;
    QPushButton *weedButton;
    QPushButton *maskVisibiltyButton;
    QPushButton *backgroundVisibilityButton;

public slots:
    changeButton(QString &msg);

private slots:
    colorClicked();
    nextClicked();
    toggleView();

signals:
    void colorSignal(const QString &colorName);
    void visibilitySignal(const QString &visibility);
    nextButtonClicked();
    previousButtonClicked();
    undoSignal();
    redoSignal();
    dirtSignal();
    plantSignal();
    weedSignal();
};

#endif // RIGHTPANEL_H
