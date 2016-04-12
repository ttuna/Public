
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>


#pragma comment(lib, "User32.lib")

namespace {
    struct CallbackData
    {
        DWORD proc_id;
        HWND win_id;
    };

    HHOOK hHook = NULL;

    // ------------------------------------------------------------------------
    // Window handle by Proc ID ...
    // ------------------------------------------------------------------------
    BOOL CALLBACK EnumWindowCallback(HWND hWnd, LPARAM lParam)
    {
        CallbackData& data = *(CallbackData*)lParam;
        DWORD proc_id = 0;
        GetWindowThreadProcessId(hWnd, &proc_id);
        qDebug() << "EnumWindowCallback - search process:" << data.proc_id << proc_id;
        if (data.proc_id == proc_id)
        {
            data.win_id = hWnd;
            qDebug() << "EnumWindowCallback - found process:" << data.proc_id << data.win_id;
            // found ... stop enumeration
            SetLastError(ERROR_SUCCESS);
            return FALSE;
        }

        // not found ... continue enumeration
        return TRUE;
    }

    // ------------------------------------------------------------------------
    // Window handle by mouse click ... hooked version
    // ------------------------------------------------------------------------
    LRESULT CALLBACK MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
    {
        switch( wParam )
        {
          case WM_LBUTTONDOWN:
            HWND hWnd = NULL;
            DWORD proc_id = 0;

            MOUSEHOOKSTRUCT& mouse_info = *(MOUSEHOOKSTRUCT*)lParam;
            qDebug() << "Left click: " << mouse_info.pt.x << mouse_info.pt.y << mouse_info.hwnd;
            UnhookWindowsHookEx(hHook);
            qDebug() << "mouse hook uninstalled ...";

            hWnd = WindowFromPoint(mouse_info.pt);
            qDebug() << "WindowFromPoint ==" << hWnd;

            GetWindowThreadProcessId(hWnd, &proc_id);
            qDebug() << "WindowFromPoint ==" << proc_id;

            PostMessage(0, WM_HANDLE_FOUND, (WPARAM)proc_id, (LPARAM)hWnd);
            return 1;
        }
        return CallNextHookEx(hHook, nCode, wParam, lParam);
    }
}

// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void MainWindow::on_actionLaunch_triggered()
{
    // TODO: kill running proc ...
    m_launched = false;

    QString file_name;
    file_name = QFileDialog::getOpenFileName(this, tr("Launch Application"), "C:", tr("Executable Files (*.exe)"));
    qDebug() << "onLaunch: file name ==" << file_name;

    m_process = new QProcess;
    qDebug() << "onLaunch: new process ==" << m_process;
    if (m_process == nullptr) return;

    m_process->start(file_name);
    if (!m_process->waitForStarted(30000)) return;

    m_proc_id = m_process->processId();
    m_launched = true;

    // allow the process to start up ...
    Sleep(1000);

    m_win_id = (WId)getMainWindowByProcId(m_proc_id);
    qDebug() << "onLaunch: win_id ==" << m_win_id;

    // TODO: catch process status changes ...
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void MainWindow::on_actionTarget_triggered()
{
#if 1
    // windows mouse hook ...
    qDebug() << "onTarget: install mouse hook ...";
    hHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, 0, 0);
    qDebug() << "onTarget: mouse hook ==" << hHook;

    // native event filter ...
    if (m_mouse_event_disp == nullptr)
    {
        qDebug() << "onTarget: install event filter ...";
        m_mouse_event_disp = new MouseEventDispatcher;
        if (m_mouse_event_disp == nullptr) return;

        connect(m_mouse_event_disp, &MouseEventDispatcher::signalProcessData, this, &MainWindow::onProcessData);

        qApp->installNativeEventFilter(m_mouse_event_disp);
    }
#else
    WId win_id = getWindowOnMouseClick();
    qDebug() << "onTarget: win_id==" << win_id;
#endif
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void MainWindow::on_actionCatchWindow_triggered()
{
    qDebug() << "onCatch";
    if (m_launched == false) return;
    if (m_win_id == 0) return;

    m_window = QWindow::fromWinId(m_win_id);
    qDebug() << "onCatch: window ==" << m_window;
    if (m_window == nullptr) return;

    QWidget* m_win_container = QWidget::createWindowContainer(m_window);
    qDebug() << "onCatch: win_container ==" << m_win_container;
    if (m_win_container == nullptr) return;

    setCentralWidget(m_win_container);
    m_window->requestActivate();
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void MainWindow::onProcessData(const quint64& in_proc_id, const WId &in_id)
{
    qDebug() << "onProcessData:" << hex << showbase << in_proc_id << in_id;
    if (in_id == 0) return;
    if (in_proc_id == 0) return;

    m_proc_id = in_proc_id;
    m_win_id = in_id;
    m_launched = true;
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void MainWindow::on_actionWrite_triggered()
{
    if (m_process == nullptr) return;
    if (m_launched == false) return;

    qDebug() << "onWrite: trying to write some stuff to proc ...";

    // TEST ...
    m_process->write("blub ...\0");
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
WId MainWindow::getMainWindowByProcId(const qint64& in_proc_id)
{
    if (in_proc_id <= 0) return 0;

    CallbackData data;
    data.proc_id = in_proc_id;

    if (!EnumWindows(EnumWindowCallback, (LPARAM)&data) && (GetLastError() == ERROR_SUCCESS))
        return (WId)data.win_id;

    return 0;
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
WId MainWindow::getWindowOnMouseClick()
{
    return 0;
}
