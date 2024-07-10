
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
    QComboBox *colorComboBox;
    QPushButton *undo;
    QPushButton *redo;
    QPushButton *nextButton;
signals:
    undoSignal();
    redoSignal();

};

#endif // RIGHTPANEL_H
