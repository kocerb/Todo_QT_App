#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QString>

namespace Ui {
class Task;
}

class Task : public QWidget
{
    Q_OBJECT

public:
    explicit Task(const QString& name, QWidget *parent = 0);
    ~Task();

    void setName(const QString& name);
    QString name() const;
    bool isCompleted() const;

public slots:
    void rename();

private slots:
    void checked(bool checked);

signals:
    void removed(Task* task);
    /*
     * The remove task is straightforward to implement, but we'll study some new concepts along the way.
     * The Task has to notify its owner and parent (MainWindow) that the removeTaskButtonQPushButton has been clicked.
     */
    void statusChanged(Task* task);

private:
    Ui::Task *ui;
};

#endif //TASK_H
