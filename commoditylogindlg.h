#ifndef COMMODITYLOGINDLG_H
#define COMMODITYLOGINDLG_H

#include <QDialog>
#include <QSqlDatabase>     // 用于连接、创建数据库
#include <QSqlQuery>        // 用于DML（数据操纵语言）、DDL（数据定义语言）
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>      //消息框

QT_BEGIN_NAMESPACE
namespace Ui { class CommodityLoginDlg; }
QT_END_NAMESPACE

class CommodityLoginDlg : public QDialog
{
    Q_OBJECT

public:
    CommodityLoginDlg(QWidget *parent = nullptr);
    ~CommodityLoginDlg();

private slots:
    void on_pushButton_Sort_clicked();      //数据排序

    void on_pushButton_Insert_clicked();    //数据插入

    void on_pushButton_Search_clicked();    //数据查询

    void on_pushButton_Update_clicked();    //数据更新

    void on_pushButton_Delete_clicked();    //数据删除

    void on_tableView_clicked(const QModelIndex &index);    //获取表格信息

private: //自定义函数
    void createDatabaseFunc();      //创建SQLite数据库
    void createTableFunc();         //创建SQLite数据表
    void queryTableFunc(QString strsql= "select *from commodityInformation");          //执行查询操作(用于显示表格)，默认显示整张表

    QSqlDatabase sqldb;             //创建数据库与Qt的链接
    QSqlQueryModel *sqlmodel;       //存储结果集  (QSqlQueryModel为只读模型)


private:
    Ui::CommodityLoginDlg *ui;
};
#endif // COMMODITYLOGINDLG_H
