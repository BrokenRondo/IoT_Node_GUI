#pragma once

#include <QWidget>
#include <QTableWidget>
namespace Ui { class NodeBlockchainDatabase; };

class NodeBlockchainDatabase : public QWidget
{
	Q_OBJECT

public:
	NodeBlockchainDatabase(QWidget *parent = Q_NULLPTR);
	NodeBlockchainDatabase(int index);
	~NodeBlockchainDatabase();
	void UpdateMsgDB(QString, QString, QString, QString);
	void UpdateLocalDB(QString, QString);
	void UpdateLog(QString);
	std::string GetData(time_t start, time_t end);
	void clearDataifCorrect();
	void addBlock(QString);
private :
	std::vector<char*> result;
	std::vector<int> index;
public:
	Ui::NodeBlockchainDatabase *ui;
private slots:
	
};
