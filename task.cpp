#include "Task.h"
#include "ui_Task.h"
#include <QInputDialog>

Task::Task(const QString& name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Task)
{
    ui->setupUi(this);
    setName(name);
    connect(ui->editButton, &QPushButton::clicked, this, &Task::rename);
    connect(ui->removeButton, &QPushButton::clicked, [this]{emit removed(this);});
    /*
     * This code excerpt shows a very interesting feature of C++11: lambdas. In our example, the lambda is the following part:
     * [this]{emit removed(this);}
     * What we did here is to connect the clicked signal to an anonymous inline function, a lambda.
     * Qt allows signal relaying by connecting a signal to another signal if their signatures match.
     * It's not the case here; the clicked signal has no parameter and the removed signal needs a Task*.
     * A lambda avoids the declaration of a verbose slot in Task.
     * Qt 5 accepts a lambda instead of a slot in a connect, and both syntaxes can be used.
     */
    connect(ui->checkBox, &QCheckBox::toggled, this, &Task::checked);
}

Task::~Task()
{
    delete ui;
}

void Task::setName(const QString& name)
{
    ui->checkBox->setText(name);
}

void Task::rename()
{
    bool ok;
    QString value = QInputDialog::getText(this, tr("Edit task"), tr("Task name"), QLineEdit::Normal, this->name(), &ok);

    if (ok && !value.isEmpty()) {
        setName(value);
    }
}

void Task::checked(bool checked)
{
    QFont font(ui->checkBox->font());
    font.setStrikeOut(checked);
    ui->checkBox->setFont(font);
    emit statusChanged(this);
}

QString Task::name() const
{
    return ui->checkBox->text();
}

bool Task::isCompleted() const
{
    return ui->checkBox->isChecked();
}
