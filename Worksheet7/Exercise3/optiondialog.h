#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include "ModelPartList.h"
// Add to optiondialog.h
#include "ModelPart.h"



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
    void updateFromPart(ModelPart* part);
    void updatePart(ModelPart* part);


private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
