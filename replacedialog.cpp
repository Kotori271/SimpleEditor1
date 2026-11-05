#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QTextCursor>
#include <QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

//查找下一个
void ReplaceDialog::on_pushButton_clicked()
{
    QString target = ui->leFind->text();
    if(target.isEmpty()) return;
    QTextCursor cur = m_editor->textCursor();
    QTextDocument *doc = m_editor->document();
    QTextDocument::FindFlags flag;
    if(ui->cbCaseSensitive->isChecked())
        flag |= QTextDocument::FindCaseSensitively;
    if(ui->radioButton->isChecked())
        flag |= QTextDocument::FindBackward;

    cur = doc->find(target, cur, flag);
    if(!cur.isNull()){
        m_editor->setTextCursor(cur);
    }else{
        QMessageBox::information(this,"提示","未找到匹配文本");
    }
}

//替换
void ReplaceDialog::on_pushButton_2_clicked()
{
    QString target = ui->leFind->text();
    QString repText = ui->leReplace->text();
    if(target.isEmpty()) return;
    QTextCursor cur = m_editor->textCursor();
    if(cur.selectedText() == target){
        cur.insertText(repText);
        m_editor->setTextCursor(cur);
    }
    on_pushButton_clicked();
}

//全部替换
void ReplaceDialog::on_pushButton_3_clicked()
{
    QString target = ui->leFind->text();
    QString repText = ui->leReplace->text();
    if(target.isEmpty()) return;
    int count = 0;
    m_editor->moveCursor(QTextCursor::Start);
    QTextDocument::FindFlags flag;
    if(ui->cbCaseSensitive->isChecked())
        flag |= QTextDocument::FindCaseSensitively;
    QTextCursor cur = m_editor->document()->find(target,cur,flag);
    while(!cur.isNull()){
        cur.insertText(repText);
        count++;
        cur = m_editor->document()->find(target,cur,flag);
    }
    QMessageBox::information(this,"提示",QString("共替换 %1 处").arg(count));
}
