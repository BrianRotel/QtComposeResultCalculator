#pragma once

#include <QObject>
#include <QSqlDatabase>

class QtMyDataBase  : public QObject
{
	Q_OBJECT

public:
	QtMyDataBase(QObject *parent);
	~QtMyDataBase();


	void initTable();

	void testInsert();

	void Insertimg();

private:
private:
	QString dbName;
	QSqlDatabase myDatabase;
};
