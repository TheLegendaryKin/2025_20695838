#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include "ModelPartList.h"

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();
    void setFromIndex(const QModelIndex &index, ModelPartList *model);
    void applyToIndex(QModelIndex index, ModelPartList* model);
    QString getName() const;
    int getR() const;
    int getG() const;
    int getB() const;
    bool getIsVisible() const;



private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
