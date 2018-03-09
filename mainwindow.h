#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "Task.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateStatus();

public slots:
    void addTask();
    void removeTask(Task* task);
    /*
    Qt uses a specific slot keyword to identify slots.
    Since a slot is a function, you can always adjust the visibility (public, protected or private) depending on your needs.”
    */
    void taskStatusChanged(Task* task);

private:
    Ui::MainWindow *ui;
    QVector<Task*> mTasks;
};

#endif // MAINWINDOW_H
