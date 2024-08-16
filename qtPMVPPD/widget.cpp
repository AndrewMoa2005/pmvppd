#include "widget.hpp"
#include "./ui_widget.h"
#include "engine_pmv_ppd.h"
#include <QFileDialog>
#include <QMessageBox>

Widget::Widget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::Widget)
{
	ui->setupUi(this);
	this->setMaximumSize(this->size());
}

Widget::~Widget()
{
	delete ui;
}

void Widget::on_bn_calc_clicked()
{
	double clo = this->ui->en_CLO->value();
	double met = this->ui->en_MET->value();
	double wme = this->ui->en_WME->value();
	double ta = this->ui->en_TA->value();
	double tr = this->ui->en_TR->value();
	double vel = this->ui->en_VEL->value();
	double rh = this->ui->en_RH->value();
	double pa = 0.0;
	double pmv = 0.0;
	double ppd = 0.0;
	double set = 0.0;
	int b = PMVboth(clo, met, wme, ta, tr, vel, rh, pa, &pmv, &ppd, &set);
	if (b < 1)
		return;
	this->ui->en_PMV->setValue(pmv);
	this->ui->en_PPD->setValue(ppd);
	this->ui->en_SET->setValue(set);
}

void Widget::on_bn_down_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save template file", QString(), "CSV(*.csv)");
	if (fileName.isEmpty())
		return;
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QString head = QString("\"%1\",\"%2\",\"%3\",\"%4\",\"%5\",\"%6\",\"%7\"\n")
		.arg(this->ui->label_CLO->text())
		.arg(this->ui->label_MET->text())
		.arg(this->ui->label_WME->text())
		.arg(this->ui->label_TA->text())
		.arg(this->ui->label_VEL->text())
		.arg(this->ui->label_TR->text())
		.arg(this->ui->label_RH->text());
	file.write(head.toUtf8());
	QString data = QString("%1,%2,%3,%4,%5,%6,%7\n")
		.arg(this->ui->en_CLO->text())
		.arg(this->ui->en_MET->text())
		.arg(this->ui->en_WME->text())
		.arg(this->ui->en_TA->text())
		.arg(this->ui->en_VEL->text())
		.arg(this->ui->en_TR->text())
		.arg(this->ui->en_RH->text());
	file.write(data.toUtf8());
	file.close();
	QMessageBox msgBox(this);
	msgBox.setText(QString("Template file has been saved : %1").arg(fileName));
	msgBox.exec();
}

void Widget::on_bn_handle_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Select table file", QString(), "CSV(*.csv)");
	if (fileName.isEmpty())
		return;
	QString bakFileName = fileName + ".bak";
	if (!QFile::rename(fileName, bakFileName)) {
		QMessageBox msgBox1(this);
		msgBox1.setText(QString("Cannot backup file : %1").arg(fileName));
		msgBox1.exec();
		return;
	}
	QFile inFile(bakFileName);
	QFile outFile(fileName);
	if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	int buffer_size = 0x400; //1KiB buffer
	char* buffer = new char[buffer_size];
	int len = 0;
	len = inFile.readLine(buffer, buffer_size);
	QStringList headList = QString(buffer).split(",");
	double clo, met, wme, ta, vel, tr, rh, pmv, ppd, set;
	int countIn = 0;
	QString head = QString("\"%1\",\"%2\",\"%3\",\"%4\",\"%5\",\"%6\",\"%7\",\"%8\",\"%9\",\"%10\"\n")
		.arg(this->ui->label_CLO->text())
		.arg(this->ui->label_MET->text())
		.arg(this->ui->label_WME->text())
		.arg(this->ui->label_TA->text())
		.arg(this->ui->label_VEL->text())
		.arg(this->ui->label_TR->text())
		.arg(this->ui->label_RH->text())
		.arg(this->ui->label_PMV->text())
		.arg(this->ui->label_PPD->text())
		.arg(this->ui->label_SET->text());
	outFile.write(head.toUtf8());
	bool errRead = true;
	while ((len = inFile.readLine(buffer, buffer_size)) > 0) {
		QStringList sList = QString(buffer).split(",");
		if (sList.size() != headList.size())
			continue;
		countIn = 0;
		for (int i = 0; i < headList.size(); i++) {
			if (headList[i].contains("CLO", Qt::CaseSensitive)) {
				clo = sList[i].toDouble(&errRead);
				countIn++;
			}
			else if (headList[i].contains("MET", Qt::CaseSensitive)) {
				met = sList[i].toDouble(&errRead);
				countIn++;
			}
			else if (headList[i].contains("WME", Qt::CaseSensitive)) {
				wme = sList[i].toDouble(&errRead);
				countIn++;
			}
			else if (headList[i].contains("TA", Qt::CaseSensitive)) {
				ta = sList[i].toDouble(&errRead);
				countIn++;
			}
			else if (headList[i].contains("VEL", Qt::CaseSensitive)) {
				vel = sList[i].toDouble(&errRead);
				countIn++;
			}
			else if (headList[i].contains("MRT", Qt::CaseSensitive)) {
				tr = sList[i].toDouble(&errRead);
				countIn++;
			}
			else if (headList[i].contains("RH", Qt::CaseSensitive)) {
				rh = sList[i].toDouble(&errRead);
				countIn++;
			}
		}
		if (countIn < 7) {
			inFile.close();
			outFile.remove();
			outFile.close();
			QFile::rename(bakFileName, fileName);
			QMessageBox msgBox2(this);
			msgBox2.setText(QString("Not enough parameter in : %1").arg(fileName));
			msgBox2.exec();
			return;
		}
		if (!errRead) {
			inFile.close();
			outFile.remove();
			outFile.close();
			QFile::rename(bakFileName, fileName);
			QMessageBox msgBox5(this);
			msgBox5.setText(QString("Parameter error in : %1").arg(fileName));
			msgBox5.exec();
			return;
		}
		if (PMVboth(clo, met, wme, ta, tr, vel, rh, 0, &pmv, &ppd, &set) < 1)
			set = -273.15;
		QString data;
		data.append(QString::number(clo, 'f', 2)).append(',')
			.append(QString::number(met, 'f', 2)).append(',')
			.append(QString::number(wme, 'f', 2)).append(',')
			.append(QString::number(ta, 'f', 2)).append(',')
			.append(QString::number(vel, 'f', 4)).append(',')
			.append(QString::number(tr, 'f', 2)).append(',')
			.append(QString::number(rh, 'f', 2)).append(',')
			.append(QString::number(pmv, 'f', 2)).append(',')
			.append(QString::number(ppd, 'f', 2)).append(',')
			.append(QString::number(set, 'f', 2)).append('\n');
		if (outFile.write(data.toUtf8()) < 0) {
			inFile.close();
			outFile.remove();
			outFile.close();
			QFile::rename(bakFileName, fileName);
			QMessageBox msgBox3(this);
			msgBox3.setText(QString("Cannot write file : %1").arg(fileName));
			msgBox3.exec();
			return;
		}
	}
	inFile.close();
	outFile.close();
	QMessageBox msgBox4(this);
	msgBox4.setText(QString("Table file has been saved : %1\nOriginal file has been moved : %2").arg(fileName).arg(bakFileName));
	msgBox4.exec();
	delete[] buffer;
}

