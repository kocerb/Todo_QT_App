#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mTasks()
{
    ui->setupUi(this);
    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addTask);
    /*
    Let's analyze how a connection is done:

    sender: This is the object that will send the signal. In our example, it is the QPushButton named addTaskButton added from the UI designer.

    &Sender::signalName: This is the pointer to the member signal function. Here, we want do something when the clicked signal is triggered.

    receiver: This is the object that will receive and handle the signal. In our case, it is the QApplication object created in main.cpp.

    &Receiver::slotName: This is a pointer to one of the receiver's member slot functions. In this example, we use the built-in quit() slot from Qapplication, which will exit the application.

    You can now compile and run this short example. You will terminate the application if you click on the addTaskButton of your MainWindow.
    */
    updateStatus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTask()
{
    bool ok;
    QString name = QInputDialog::getText(this,
                                         tr("Add task"),
                                         tr("Task name"),
                                         QLineEdit::Normal,
                                         tr("Untitled task"),
                                         &ok);
        if (ok && !name.isEmpty()) {
        qDebug() << "Adding new task";
        Task* task = new Task("Untitled task");
        connect(task, &Task::removed, this, &MainWindow::removeTask);
        connect(task, &Task::statusChanged, this, &MainWindow::taskStatusChanged);
        mTasks.append(task);
        ui->tasksLayout->addWidget(task);
    /*
     * In our case, the ui->tasksLayout->addWidget(task) call has an interesting side-effect; the ownership of the task is transferred to tasksLayout.
     * The QObject* parent defined in Task constructor is now tasksLayout, and the Task destructor will be called when tasksLayout releases its own memory
     * by recursively iterating through its children and calling their destructor.
     */
        updateStatus();
    }
}

void MainWindow::removeTask(Task *task)
{
    mTasks.removeOne(task);
    ui->tasksLayout->removeWidget(task);
    task->setParent(0);
    delete task;
    /*
     * If we commented these lines, here is how removeTask() will look:

        void MainWindow::removeTask(Task* task)
        {
            mTasks.removeOne(task);
            ui->tasksLayout->removeWidget(task);
            // task->setParent(0);
            // delete task;
        }

     * If you add a log message in Task destructor and execute the program, this log message will be displayed.
     * Nonetheless, the Qt documentation tells us in Qlayout::removeWidget part: The ownership of a widget remains the same as when it was added.
     * Instead, what really happens is that the task class's parent becomes centralWidget, the tasksLayout class's parent.
     * We want Qt to forget everything about task, that's why we call task->setParent(0).
     * We can then safely delete it and call it a day.
     */
    updateStatus();
}

void MainWindow::taskStatusChanged(Task* /*task*/)
{
    updateStatus();
}

void MainWindow::updateStatus()
{
    qDebug() << "Status updated";
    int completedCount = 0;
    for (auto t : mTasks) {
        if (t->isCompleted()) {
            completedCount++;
        }
    }
    /* The auto keyword is another great new semantic.
     * The compiler deduces the variable type automatically based on the initializer.
     */
    int todoCount = mTasks.size() - completedCount;

    ui->statusLabel->setText(
                QString("Status: %1 todo / %2 completed").arg(todoCount).arg(completedCount));
}
