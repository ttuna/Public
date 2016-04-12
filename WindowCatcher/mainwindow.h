#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWindow>
#include <QWidget>
#include <QProcess>
#include <QAbstractNativeEventFilter>
#include <QDebug>
#include <Windows.h>

namespace Ui {
class MainWindow;
}

#define WM_HANDLE_FOUND (WM_USER + 0)

class MouseEventDispatcher;

// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionLaunch_triggered();
    void on_actionCatchWindow_triggered();
    void on_actionWrite_triggered();
    void on_actionTarget_triggered();
    void onProcessData(const quint64 &in_proc_id, const WId& in_id);

private:
    WId getMainWindowByProcId(const qint64 &in_proc_id);
    WId getWindowOnMouseClick();

    Ui::MainWindow *ui;
    bool m_launched = false;
    QString m_title;
    WId m_win_id = 0;
    quint64 m_proc_id;
    QProcess* m_process;
    QWindow* m_window = nullptr;
    QWidget* m_win_container = nullptr;
    MouseEventDispatcher* m_mouse_event_disp = nullptr;
};

// ----------------------------------------------------------------------------
// Window handle by mouse click ... native event dispatcher.
// !!!ATTENTION!!! Seems to work for application messages ONLY ... (same results as Win API call 'GetMessage')
// ----------------------------------------------------------------------------
class MouseEventDispatcher : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    MouseEventDispatcher(QObject* parent = 0) : QObject(parent), QAbstractNativeEventFilter()
    {}

    bool QAbstractNativeEventFilter::nativeEventFilter(const QByteArray & eventType, void * message, long * result)
    {
        Q_UNUSED(result)
        //qDebug() << "nativeEventFilter";

        MSG* msg = (MSG*)message;
        if (msg == nullptr) return false;

        if (msg->message == WM_HANDLE_FOUND)
        {
            qDebug() << eventType << msg->hwnd << msg->pt.x << msg->pt.y << hex << showbase << msg->wParam << msg->lParam;
            emit signalProcessData(msg->wParam, msg->lParam);
        }

        return false;
    }

signals:
    void signalProcessData(const quint64& proc_id, const WId& hWnd);
};

#endif // MAINWINDOW_H
