#include "stdafx.h"
#include "QtMyDataBase.h"
#include <QImageReader>
#include <QPixmap>
#include <QSqlQuery>
#include <QSqlError>


QtMyDataBase::QtMyDataBase(QObject *parent)
	: QObject(parent)
{
	dbName = "MySqliteDB.sqlite3";
	initTable();
}
QtMyDataBase::~QtMyDataBase()
{

}
void QtMyDataBase::initTable()
{
	myDatabase = QSqlDatabase::addDatabase("QSQLITE");
	myDatabase.setDatabaseName(dbName);
	qDebug() << QDir::currentPath();

	if (myDatabase.open())
	{
		qDebug() << "连接数据库成功";
	}
	else
	{
		qDebug() << "连接数据库失败";
	}
#if 0
	QString sql = QString("create table if not exists imagetable(filename TEXT,imagedata BLOB)");
	QSqlQuery query(myDatabase);//加载数据库
	query.prepare(sql);//执行sql 语句
	if (!query.exec())
	{
		QString sError = query.lastError().text();
		sError = "数据库表格插入数据失败:" + sError;
		qDebug() << "创建失败";
	}
	else
	{
		qDebug() << "创建成功";
	}
	//#else
	//如果不存在则创建my_table表,id自增,name唯一
	const QString cmdSql = R"(
                           CREATE TABLE IF NOT EXISTS image_table (
                           id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
                           imagename TEXT NOT NULL,
                           type CHAR(50) NOT NULL,
                           notes TEXT,
                           imagedata BLOB NOT NULL
                           );)";
	//QSqlQuery构造前，需要db已打开并连接;未指定db或者db无效时使用默认连接进行操作
	QSqlQuery myQuery(myDatabase);

	if(!myQuery.exec(cmdSql))
	{
		QString sError = myQuery.lastError().text();
		sError = "数据库表格插入数据失败:" + sError;
		QMessageBox::information(nullptr, "提示", sError);
		myDatabase.close();
	}
#endif // 0
}
// 获取图片并插入到数据库
void QtMyDataBase::testInsert()
{
	QString imagePath = "C:\\Users\\Administrator\\Pictures\\BnS\\CharacterCustomize\\外形_人_女00000.jpg";
	//QImage image(imagePath);
	//QByteArray ba;
	//QBuffer buffer(&ba);
	//buffer.open(QIODevice::WriteOnly);
	//image.save(&buffer, "PNG"); // writes image into ba in PNG format

	qDebug() << imagePath;
	QFile* file = new QFile(imagePath);
	bool isOpen = file->open(QIODevice::ReadOnly);
	QByteArray data = file->readAll();
	file->close();
	QSqlQuery query;
	QSqlQuery insertQuery(myDatabase);
	insertQuery.prepare("insert into test1 (name, value) values (:name, :value);");
	insertQuery.bindValue(":name", "外形_人_女00000");
	//QVariant var(ba);
	//insertQuery.addBindValue(var);
	insertQuery.bindValue(":value", data);
	
	//QString strsql = QString("insert into test1(filename,imagedata)values(\"%1\",:imagedata)").arg(file->fileName());
	//QSqlQuery query;
	//query.prepare(strsql);
	//query.addBindValue(data);
	if (insertQuery.exec())
	{
		qDebug() << "插入图片成功";
	}
	else
	{
		qDebug() << "插入图片失败";
	}

	//ba.clear();
}


void QtMyDataBase::Insertimg()
{
	QString imagePath = "C:\\Users\\Administrator\\Pictures\\BnS\\CharacterCustomize\\外形_人_女00000.jpg";
	QImage image(imagePath);

	QSqlQuery querySQL = QSqlQuery("", myDatabase);
	QByteArray ba;
	QBuffer imgBuffer(&ba);
	imgBuffer.open(QIODevice::WriteOnly);
	image.save(&imgBuffer, "jpg", 100);
	querySQL.prepare("insert into test1 (name, value) values (?, ?)");
	querySQL.addBindValue("jpg");
	QVariant var(ba);
	querySQL.addBindValue(var);
	querySQL.exec();
	if (!querySQL.isActive())
	{
		qDebug() << "OCR图片插入数据库失败：isactive状态没有激活";
		return;
	}
	ba.clear();
	imgBuffer.close();

	querySQL.finish();
#if 0



	//另一种方法
	QImage image;
	QByteArray ba;
	QBuffer buffer(&ba);
	buffer.open(QIODevice::WriteOnly);
	image.save(&buffer, "PNG"); // writes image into ba in PNG format

	//QByteArray转化为 QImage:
	QImage image;
	QByteArray ba;
	image.loadFromData(ba);
#endif // 0
}