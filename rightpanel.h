
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
    QPushButton *dirtButton;
    QPushButton *plantButton;
    QPushButton *weedButton;
private slots:
    colorClicked();
signals:
    void colorSignal(const QString &colorName);
    undoSignal();
    redoSignal();
    dirtSignal();
    plantSignal();
    weedSignal();
};

#endif // RIGHTPANEL_H
