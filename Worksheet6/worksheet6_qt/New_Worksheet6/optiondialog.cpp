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

void OptionDialog::updateFromPart(ModelPart* part) {
    // 1. Set the Name and Visibility checkbox
    ui->nameLineEdit->setText(part->data(0).toString()); // [cite: 367, 368]
    ui->isVisibleCheckBox->setChecked(part->visible()); // [cite: 369]

    // 2. Set sliders to the part's current RGB values using your names
    ui->rSlider->setValue(part->getColourR());
    ui->gSlider->setValue(part->getColourG());
    ui->bSlider->setValue(part->getColourB());
}

void OptionDialog::updatePart(ModelPart* part) {
    // 1. Save the Name and Visibility back to the part
    part->set(0, ui->nameLineEdit->text()); // [cite: 373]
    part->setVisible(ui->isVisibleCheckBox->isChecked()); // [cite: 369]

    // 2. Save your slider values back to the part's internal storage
    part->setColour(ui->rSlider->value(),
                    ui->gSlider->value(),
                    ui->bSlider->value());
}
