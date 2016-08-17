#ifndef SETBASISDIALOG_H
#define SETBASISDIALOG_H

#include <QDialog>

#include "molconv_window.h"

namespace Ui
{
    class setBasisDialog;
}

class setBasisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit setBasisDialog(QWidget *parent = 0);
    ~setBasisDialog();

    void prepare(molconv::moleculePtr &molecule);

private slots:
    void on_originAllCheckBox_stateChanged();
    void on_basisAllCheckBox_stateChanged();
    void on_originAtomPos_toggled(bool checked);
    void on_originAtomLine_toggled(bool checked);
    void on_basisAtoms_toggled(bool checked);
    void on_originALineScaleSlider_valueChanged(int value);
    void on_originALineScaleBox_valueChanged(double value);

private:
    Ui::setBasisDialog *ui;
    MolconvWindow *m_window;
};

#endif // SETBASISDIALOG_H
