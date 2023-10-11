#include "commoditylogindlg.h"
#include "ui_commoditylogindlg.h"

CommodityLoginDlg::CommodityLoginDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CommodityLoginDlg)
{
    ui->setupUi(this);

    //查看Qt包含的驱动
//    QStringList drivers = QSqlDatabase::drivers();
//    foreach (QString str, drivers) {
//        qDebug()<<str;
//    }

    //调用函数创建数据库
    createDatabaseFunc();

    //调用函数创建数据表
    //createTableFunc();

    //调用函数显示数据表
    queryTableFunc();
    //数据库关闭
//    sqldb.close();



}

CommodityLoginDlg::~CommodityLoginDlg()
{
    delete ui;
}



//---------------------------------------------------------------
//数据排序
//属性    索引      规则      索引
//编号    0        升序        0
//名称    1        降序        1
//价格    2
//---------------------------------------------------------------
void CommodityLoginDlg::on_pushButton_Sort_clicked()
{
    QString strsql;
    //升序
    if(ui->comboBox_Order->currentIndex() == 0)
    {
        //按编号排序
        if(ui->comboBox_Key->currentIndex() == 0)
        {
            strsql = QString("select * from commodityInformation order by id asc");
        }
        //按名称排序
        if(ui->comboBox_Key->currentIndex() == 1)
        {
            strsql = QString("select * from commodityInformation order by name asc");
        }
        //按价格排序
        if(ui->comboBox_Key->currentIndex() == 2)
        {
            strsql = QString("select * from commodityInformation order by price asc");
        }
    }
    //降序
    if(ui->comboBox_Order->currentIndex() == 1)
    {
        //按编号排序
        if(ui->comboBox_Key->currentIndex() == 0)
        {
            strsql = QString("select * from commodityInformation order by id desc");
        }
        //按名称排序
        if(ui->comboBox_Key->currentIndex() == 1)
        {
            strsql = QString("select * from commodityInformation order by name desc");
        }
        //按价格排序
        if(ui->comboBox_Key->currentIndex() == 2)
        {
            strsql = QString("select * from commodityInformation order by price desc");
        }
    }


    //debug
    //qDebug()<<strsql;


    QSqlQuery query;
    if(!query.exec(strsql)){
        QMessageBox::critical(this, "错误", "排序错误，请检查故障", QMessageBox::Ok);
        return;
    }
    //刷新显示
    queryTableFunc(strsql);
}



//---------------------------------------------------------------
//数据插入
//---------------------------------------------------------------
void CommodityLoginDlg::on_pushButton_Insert_clicked()
{
    QSqlQuery sqlquery;
    int id = ui->lineEdit_Id->text().toInt();
    //id>=0
    if(id < 0){
        QMessageBox::critical(this, "失败", "提示：输入错误？编号不能小于0", QMessageBox::Ok);
        return;
    }

    QString name = ui->lineEdit_Name->text();
    //name非空
    if(name == ""){
        QMessageBox::critical(this, "失败", "提示：输入错误？名称不能为空");
        return;
    }

    double price = ui->lineEdit_Price->text().toDouble();
    //price>0
    if(price <= 0){
        QMessageBox::critical(this, "失败", "提示：输入错误？价格不能小于0", QMessageBox::Ok);
        return;
    }

    //编辑sql语句
    QString strsql = QString("insert into commodityInformation "
                             "values(%1,'%2',%3)").arg(id).arg(name).arg(price);
    //验证插入操作是否成功
    if(sqlquery.exec(strsql) == false){
        QMessageBox::critical(0, "失败", "数据插入失败，请检查重试", QMessageBox::Ok);
    }
    else{
        QMessageBox::information(0, "成功", "数据插入成功", QMessageBox::Ok);
    }
    //编辑栏清空
    ui->lineEdit_Id->setText("");
    ui->lineEdit_Name->setText("");
    ui->lineEdit_Price->setText("");
    //显示表格
    queryTableFunc();
}



//---------------------------------------------------------------
//数据查询
//---------------------------------------------------------------
void CommodityLoginDlg::on_pushButton_Search_clicked()
{
    QSqlQuery sqlquery;
    //传递输入的数据
    int id = ui->lineEdit_Id->text().toInt();
    QString name = ui->lineEdit_Name->text();
    double price = ui->lineEdit_Price->text().toDouble();
    //条件查询：通过字符串拼接实现条件数不定查询
    QString condation = "";
    //flag==true 前一个条件非空 需要加"and"
    bool flag = false;
    //检索id
    if(!ui->lineEdit_Id->text().isEmpty()){
        condation.append(QString(" id = %1").arg(id));
        flag = true;
    }
    //检索name
    if(!ui->lineEdit_Name->text().isEmpty())
    {
        if(flag)
        {
            condation.append(QString(" and name='%1'").arg(name));
        }
        else
        {
            condation.append(QString(" name='%1'").arg(name));
            flag = true;
        }
    }

    //检索price
    if(!ui->lineEdit_Price->text().isEmpty())
    {
        if(flag)
        {
            condation.append(QString(" and price=%1").arg(price));
        }
        else
        {
            condation.append(QString(" price=%1").arg(price));
            flag = true;
        }
    }

    //flag == false 无条件 输出所有信息
    QString strsql;
    if(flag == true)
    {
        strsql = QString("select * from commodityInformation where%1").arg(condation);
    }
    else
    {
        strsql = QString("select * from commodityInformation");
    }

//    qDebug()<<flag;
//    qDebug()<<QString("select * from commodityInformation where%1").arg(condation);
    //执行查询操作
    if(sqlquery.exec(strsql) == false)
    {
        QMessageBox::critical(this, "错误", "未找到信息", QMessageBox::Ok);
    }
    else
    {
        QMessageBox::information(this, "成功", "查询成功", QMessageBox::Ok);
    }
    //调用函数查询数据
    queryTableFunc(strsql);
}



//---------------------------------------------------------------
//数据更新
//---------------------------------------------------------------
void CommodityLoginDlg::on_pushButton_Update_clicked()
{
    //传递id、name、price
    //id
    int id = ui->lineEdit_Id->text().toInt();
    if(id < 0)
    {
        QMessageBox::critical(this, "错误", "编号不能小于0", QMessageBox::Ok);
    }
    //name
    QString name = ui->lineEdit_Name->text();
    if(name == "")
    {
        QMessageBox::critical(this, "错误", "名称不能为空", QMessageBox::Ok);
    }
    //price
    double price = ui->lineEdit_Price->text().toDouble();
    if(price < 0)
    {
        QMessageBox::critical(this, "错误", "价格不能小于0", QMessageBox::Ok);
    }

    //更新语言
    QSqlQuery query;
    QString strsql = QString("update commodityInformation set name = '%1', price = %2 where id = %3").
                     arg(name).arg(price).arg(id);
    if(!query.exec(strsql))
    {
        QMessageBox::critical(this, "失败", "更新失败，请检查", QMessageBox::Ok);
        return;
    }
    else
    {
        QMessageBox::information(this, "成功", "数据更新成功", QMessageBox::Ok);
    }

}



//---------------------------------------------------------------
//数据删除
//---------------------------------------------------------------
void CommodityLoginDlg::on_pushButton_Delete_clicked()
{
    //传递数据
    //id
    int id = ui->lineEdit_Id->text().toInt();
    if(id < 0){
        QMessageBox::critical(this, "错误", "编号不能小于0", QMessageBox::Ok);
    }
    //name
    QString name = ui->lineEdit_Name->text();
    if(name == "")
    {
        QMessageBox::critical(this, "错误", "名称不能为空", QMessageBox::Ok);
    }
    //price
    double price = ui->lineEdit_Price->text().toDouble();
    if(price < 0)
    {
        QMessageBox::critical(this, "错误", "价格不能小于0", QMessageBox::Ok);
    }

    //筛选条件
    QString condation = "";
    //判断上一个条件是否为空
    bool flag = false;
    if(!ui->lineEdit_Id->text().isEmpty())
    {
        condation.append(QString(" id == %1").arg(id));
        flag = true;
    }

    if(!ui->lineEdit_Name->text().isEmpty())
    {
        //前一列属性为空
        if(flag)
        {
            condation.append(QString(" and name = '%1'").arg(name));
            flag = true;
        }
        else
        {
            condation.append(QString(" name = '%1'").arg(name));
        }
    }

    if(!ui->lineEdit_Name->text().isEmpty())
    {
        //前一列属性为空
        if(flag)
        {
            condation.append(QString(" and price = '%1'").arg(price));
            flag = true;
        }
        else
        {
            condation.append(QString(" price = '%1'").arg(price));
        }
    }
//    qDebug()<<QString("delete from commodityInformation where%1").arg(condation);
    QString strsql;
    //存在筛选条件
    if(flag)
    {
        strsql = QString("delete from commodityInformation where%1").arg(condation);
    }

    QSqlQuery query;
    if(!query.exec(strsql))
    {
        QMessageBox::critical(this, "失败", "删除失败，请检查", QMessageBox::Ok);
        return;
    }
    else
    {
        QMessageBox::information(this, "成功", "数据删除成功", QMessageBox::Ok);
    }
}



//---------------------------------------------------------------
//创建SQLite数据库
//---------------------------------------------------------------
void CommodityLoginDlg::createDatabaseFunc()
{
    //1.添加数据库驱动
    sqldb = QSqlDatabase::addDatabase("QSQLITE");

    //2.设置数据库名称
    sqldb.setDatabaseName("commoditysys");

    //3.数据库打开是否成功
    if(sqldb.open() == true){
        QMessageBox::information(this, "正确", "恭喜你，数据库打开成功", QMessageBox::Ok);
    }
    else{
        QMessageBox::critical(this, "错误", "数据库打开失败，请重试", QMessageBox::Ok);
        return;
    }
}


//---------------------------------------------------------------
//创建SQLite数据表
//---------------------------------------------------------------
void CommodityLoginDlg::createTableFunc()
{
    QSqlQuery createquery;
    //创建SQL语句
    QString strsql = QString("create table commodityInformation("
                             "id int primary key not null,"
                             "name text not null,"
                             "price real not null)");

    //检查数据表是否创建成功
    if(createquery.exec(strsql) == true){
        QMessageBox::information(this, "正确", "数据表创建成功", QMessageBox::Ok);
    }
    else{
        QMessageBox::critical(this, "错误", "数据表创建失败，请重试", QMessageBox::Ok);
        return;
    }
}



//---------------------------------------------------------------
//执行查询操作(用于将数据显示到界面上)
//---------------------------------------------------------------
void CommodityLoginDlg::queryTableFunc(QString strsql)
{
    //debug
    //qDebug()<<strsql;

    //数据表model
    sqlmodel = new QSqlQueryModel(ui->tableView);
    sqlmodel->setQuery(strsql);
    //设置属性名
    sqlmodel->setHeaderData(0, Qt::Horizontal, QObject::tr("编号"));
    sqlmodel->setHeaderData(1, Qt::Horizontal, QObject::tr("名称"));
    sqlmodel->setHeaderData(2, Qt::Horizontal, QObject::tr("价格"));
    //设置模型
    ui->tableView->setModel(sqlmodel);
}



//---------------------------------------------------------------
//获取表格信息
//---------------------------------------------------------------
void CommodityLoginDlg::on_tableView_clicked(const QModelIndex &index)
{
    QSqlRecord record = sqlmodel->record(index.row());
    //获取被点击行的id、name、price值
    int id = record.value("id").toInt();
    QString name = record.value("name").toString();
    double price = record.value("price").toDouble();
    ui->lineEdit_Id->setText(QString("%1").arg(id));
    ui->lineEdit_Name->setText(name);
    ui->lineEdit_Price->setText(QString("%1").arg(price));
}

