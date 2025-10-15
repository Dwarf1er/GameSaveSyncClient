#include "pathEditableRowWidget.h"
#include <QHBoxLayout>

PathEditableRowWidget::PathEditableRowWidget(int pathId, QWidget* parent)
    : QWidget(parent) {
    this->pathId = pathId;
    this->setAutoFillBackground(true);

    auto layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    lineEdit = new QLineEdit();
    lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    folderDialog = new QPushButton("browse...");
    folderDialog->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    this->layout()->addWidget(lineEdit);
    this->layout()->addWidget(folderDialog);
}
