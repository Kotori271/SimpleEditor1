#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QCloseEvent>
#include <QTextCursor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    //文件菜单
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();

    //查找替换
    void on_actionFind_triggered();
    void on_actionReplace_triggered();

    //编辑菜单
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionSelectAll_triggered();

    //格式菜单
    void on_actionWordWrap_triggered(bool checked);
    void on_actionFont_triggered();
    void on_actionFontColor_triggered();
    void on_actionBgColorEdit_triggered();

    //查看菜单
    void on_actionShowToolbar_triggered(bool checked);

private:
    Ui::MainWindow *ui;

    QString m_currentFilePath;
    bool m_isModified;

    void updateWindowTitle();
    bool maybeSave();
    void updateStatusBar();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
