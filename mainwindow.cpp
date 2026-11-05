#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "replacedialog.h"
#include <QFileInfo>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_currentFilePath.clear();
    m_isModified = false;
    updateWindowTitle();

    //文本变更标记修改
    connect(ui->plainTextEdit,&QPlainTextEdit::textChanged,this,[=](){
        m_isModified = true;
        updateWindowTitle();
    });

    //控制复制剪切按钮状态
    connect(ui->plainTextEdit,&QPlainTextEdit::copyAvailable,this,[=](bool ok){
        ui->actionCut->setEnabled(ok);
        ui->actionCopy->setEnabled(ok);
    });

    //光标移动刷新状态栏
    connect(ui->plainTextEdit,&QPlainTextEdit::cursorPositionChanged,this,&MainWindow::updateStatusBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateWindowTitle()
{
    QString title;
    if(m_currentFilePath.isEmpty()){
        title = "新建文本文件 - 编辑器";
    }else{
        QFileInfo info(m_currentFilePath);
        title = info.fileName() + " - 编辑器";
    }
    if(m_isModified){
        title.prepend("*");
    }
    setWindowTitle(title);
}

bool MainWindow::maybeSave()
{
    if(!m_isModified)
        return true;

    auto ret = QMessageBox::question(this,"文件尚未保存",
                                     "文档已经修改，是否保存更改？",
                                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if(ret == QMessageBox::Save){
        on_actionSave_triggered();
        return true;
    }else if(ret == QMessageBox::Discard){
        return true;
    }else{
        return false;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave()){
        event->accept();
    }else{
        event->ignore();
    }
}

void MainWindow::updateStatusBar()
{
    QTextCursor cur = ui->plainTextEdit->textCursor();
    int line = cur.blockNumber() + 1;
    int col = cur.positionInBlock() + 1;
    int totalLines = ui->plainTextEdit->blockCount();
    int len = ui->plainTextEdit->toPlainText().size();

    ui->statusBar->showMessage(QString("length:%1 lines:%2 Ln:%3 Col:%4")
                                   .arg(len).arg(totalLines).arg(line).arg(col));
}

//==== 文件菜单 ====
void MainWindow::on_actionNew_triggered()
{
    if(!maybeSave()) return;
    ui->plainTextEdit->clear();
    m_currentFilePath.clear();
    m_isModified = false;
    updateWindowTitle();
}

void MainWindow::on_actionOpen_triggered()
{
    if(!maybeSave()) return;
    QString path = QFileDialog::getOpenFileName(this,"打开文件","","文本文件(*.txt)");
    if(path.isEmpty()) return;
    QFile f(path);
    if(f.open(QIODevice::ReadOnly)){
        ui->plainTextEdit->setPlainText(f.readAll());
        f.close();
        m_currentFilePath = path;
        m_isModified = false;
        updateWindowTitle();
    }else{
        QMessageBox::warning(this,"错误","打开失败");
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(m_currentFilePath.isEmpty()){
        on_actionSaveAs_triggered();
        return;
    }
    QFile f(m_currentFilePath);
    if(f.open(QIODevice::WriteOnly)){
        f.write(ui->plainTextEdit->toPlainText().toUtf8());
        f.close();
        m_isModified = false;
        updateWindowTitle();
    }else{
        QMessageBox::warning(this,"错误","保存失败");
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString path = QFileDialog::getSaveFileName(this,"另存为","","文本文件(*.txt)");
    if(path.isEmpty()) return;
    QFile f(path);
    if(f.open(QIODevice::WriteOnly)){
        f.write(ui->plainTextEdit->toPlainText().toUtf8());
        f.close();
        m_currentFilePath = path;
        m_isModified = false;
        updateWindowTitle();
    }else{
        QMessageBox::warning(this,"错误","保存失败");
    }
}

void MainWindow::on_actionExit_triggered()
{
    if(maybeSave()){
        close();
    }
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg(this);
    dlg.exec();
}

//==== 查找替换 ====
void MainWindow::on_actionFind_triggered()
{
    ReplaceDialog dlg(this);
    dlg.setEditor(ui->plainTextEdit);
    dlg.exec();
}

void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg(this);
    dlg.setEditor(ui->plainTextEdit);
    dlg.exec();
}

//==== 编辑菜单 ====
void MainWindow::on_actionUndo_triggered()
{
    ui->plainTextEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->plainTextEdit->redo();
}

void MainWindow::on_actionCut_triggered()
{
    ui->plainTextEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->plainTextEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->plainTextEdit->paste();
}

void MainWindow::on_actionSelectAll_triggered()
{
    ui->plainTextEdit->selectAll();
}

// ==========格式菜单============
//自动换行
void MainWindow::on_actionWordWrap_triggered(bool checked)
{
    if(checked){
        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    }else{
        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    }
}

//选择字体
void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont f = QFontDialog::getFont(&ok, ui->plainTextEdit->font(), this, "选择字体");
    if(ok){
        ui->plainTextEdit->setFont(f);
    }
}

//字体颜色
void MainWindow::on_actionFontColor_triggered()
{
    QColor c = QColorDialog::getColor(Qt::black,this,"字体颜色");
    if(!c.isValid()) return;

    QTextCharFormat fmt;
    fmt.setForeground(c);
    QTextCursor cursor = ui->plainTextEdit->textCursor();
    if(cursor.hasSelection()){
        cursor.mergeCharFormat(fmt);
    }else{
        QPalette pal = ui->plainTextEdit->palette();
        pal.setColor(QPalette::Text,c);
        ui->plainTextEdit->setPalette(pal);
    }
}

//编辑器背景色
void MainWindow::on_actionBgColorEdit_triggered()
{
    QColor c = QColorDialog::getColor(Qt::white,this,"编辑器背景色");
    if(!c.isValid()) return;
    QPalette pal = ui->plainTextEdit->palette();
    pal.setColor(QPalette::Base,c);
    ui->plainTextEdit->setPalette(pal);
}

//========查看菜单：显示/隐藏工具栏========
void MainWindow::on_actionShowToolbar_triggered(bool checked)
{
    ui->mainToolBar->setVisible(checked);
}
