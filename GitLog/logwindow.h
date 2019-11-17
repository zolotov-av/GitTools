#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QMainWindow>
#include <QSettings>

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

public slots:

    void openRepository();

protected:

    void resizeEvent(QResizeEvent *event) override;


private:
    Ui::LogWindow *ui;

    QSettings *cache;
    GitLogView *logView;

};

#endif // LOGWINDOW_H
