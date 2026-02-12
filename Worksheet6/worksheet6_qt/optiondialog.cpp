#include "optiondialog.h"
#include "ui_optiondialog.h"
#include "ui_optiondialog.h"


OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::setFromIndex(const QModelIndex &index, ModelPartList *model)
{
    if (!index.isValid() || !model) return;

    // Column 0: name
    ui->nameLineEdit->setText(model->data(index, Qt::DisplayRole).toString());

    // Column 1: visible
    QModelIndex visibleIndex = model->index(index.row(), 1, index.parent());
    QString visibleText = model->data(visibleIndex, Qt::DisplayRole).toString();
    bool visible = (visibleText.toLower() == "true");
    ui->isVisibleCheckBox->setChecked(visible);

    // Colour not stored in model yet, set defaults
    ui->rSlider->setValue(255);
    ui->gSlider->setValue(255);
    ui->bSlider->setValue(255);
}

void OptionDialog::applyToIndex(QModelIndex index, ModelPartList* model)
{
    if (!index.isValid() || !model) return;

    model->setData(index, ui->nameLineEdit->text(), Qt::EditRole);

    QModelIndex visibleIndex = model->index(index.row(), 1, index.parent());

    QString visible = ui->isVisibleCheckBox->isChecked() ? "true" : "false";
    model->setData(visibleIndex, visible, Qt::EditRole);
}

QString OptionDialog::getName() const { return ui->nameLineEdit->text(); }
int OptionDialog::getR() const { return ui->rSlider->value(); }
int OptionDialog::getG() const { return ui->gSlider->value(); }
int OptionDialog::getB() const { return ui->bSlider->value(); }
bool OptionDialog::getIsVisible() const { return ui->isVisibleCheckBox->isChecked(); }
