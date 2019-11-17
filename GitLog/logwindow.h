#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QMainWindow>

namespace Ui {
class LogWindow;
}

class GitLogView;

class LogWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = 0);
    ~LogWindow();

private:
    Ui::LogWindow *ui;

    GitLogView *logView;
};

#endif // LOGWINDOW_H
