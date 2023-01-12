#include <QtConcurrent>
#include <QtWidgets>
#include "mywidget.h"
#include <cstdlib>

//Вычисление на участке
qint64 perElementFunc(const Task task) {
	qint64 result;
	for (int i=task.beginIndex; i<=task.endIndex; i++) {
        if (task.listPtr->at(i) % 2 == 0)
        {
        result += task.listPtr->at(i);
        }
        QThread::msleep(1);
	}
	return result;
}

//объединение результатов
void reduce(qint64 & sum, const qint64 semiSum) {
    sum += semiSum;
}


int generateRandomNumber(int minVal, int maxVal, int seed) {
    qsrand(seed);
    return (qrand() % ((maxVal + 1) - minVal) + minVal);
}

mywidget::mywidget(QWidget *parent) : QWidget(parent) {

    outputField = new QTextEdit();
    elementsEditField = new QLineEdit();
    threadsEditField = new QLineEdit();
    startBtn = new QPushButton(tr("Старт"));
    stopBtn = new QPushButton(tr("Остановить"));
    QPushButton *closeBtn = new QPushButton(tr("Закрыть"));
    messageLbl = new QLabel();
	
    outputField->setReadOnly(true);
    outputField->append("Поиск cуммы четных чисел:\n"
        "Количестве элементов: от 100000 до 1000000;\n"
    "Количество потоков: от 2 до 100000.");
    messageLbl->setStyleSheet("color: red");
    elementsEditField->setValidator(new QIntValidator(100000, 1000000, this));
    threadsEditField->setValidator(new QIntValidator(2,100000,this));
    elementsEditField->setText("100000");
    threadsEditField->setText(QString::number(QThread::idealThreadCount()));
    stopBtn->setEnabled(false);
	

    QFormLayout *fieldsLayout = new QFormLayout;
    fieldsLayout->addRow(tr("Кол-во элементов:"), elementsEditField);
    fieldsLayout->addRow(tr("    Кол-во потоков:"), threadsEditField);
	
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(outputField);
    mainLayout->addWidget(messageLbl);
    mainLayout->addLayout(fieldsLayout);
    mainLayout->addWidget(startBtn);
    mainLayout->addWidget(stopBtn);
    mainLayout->addWidget(closeBtn);
	
    setLayout(mainLayout);
	
    connect(startBtn,SIGNAL(clicked()),this,SLOT(clickStart()));
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(clickClose()));
    connect(stopBtn,SIGNAL(clicked()),this,SLOT(clickStop()));
}

void mywidget::clickStart() {
	

    if (elementsEditField->hasAcceptableInput() && threadsEditField->hasAcceptableInput()){

		messageLbl->setText("");
		elementsEditField->setReadOnly(true);
		threadsEditField->setReadOnly(true);
		startBtn->setEnabled(false);
		int countElements = elementsEditField->text().toInt();
		
		for (int i=0; i<countElements; i++) {
			array.append(generateRandomNumber(100,100000,QDateTime::currentMSecsSinceEpoch()));
		}
		
		executionWithoutThread(countElements);
		
		executionWithThread(countElements);
		
		} else {
        messageLbl->setText("Входные данные выходят за границу диапазона.");
	}
}

void mywidget::executionWithoutThread(int countElements) {
	
    outputField->append("\nОднопоточный режим \nВычисление");
    timer.start();
	
    qint64 result = 1;
    for (int i=0; i<countElements; i++) {
        if (array[i] % 2 == 0)
        {
        result+=array[i];
        }
        QThread::msleep(1);
	}
    outputField->append(tr("Завершено.\nВремя выполнения: ")+
                        QString::number(timer.elapsed())+" мс");
}

void mywidget::executionWithThread(int countElements) {
	
    int countThreads = threadsEditField->text().toInt();
    int perThread = countElements / countThreads;
	
    timer.restart();
    stopBtn->setEnabled(true);
    outputField->append("\nМногопоточный режим \nПодготовка задач...");
    timer.start();
	
    for (int i=0; i<=countThreads-1; i++) {
        Task task;
        task.beginIndex = i*perThread;
        task.endIndex = (i+1)*perThread-1;
        task.listPtr = &array;
        tasks.append(task);
	}
	
    watcher = new QFutureWatcher<qint64>();
    connect(watcher,SIGNAL(progressValueChanged(int)),this,SLOT(progressValueChanged(int)));
    connect(watcher,SIGNAL(finished()),this,SLOT(finished()));
    future = QtConcurrent::mappedReduced(tasks, perElementFunc, reduce);
    watcher->setFuture(future);
}


void mywidget::progressValueChanged(int value) {
    outputField->append(tr("Прогресс: ")+QString::number(value));
}


void mywidget::finished() {
    outputField->append(tr("Завершено.\nВремя выполнения: ")+
                        QString::number(timer.elapsed())+" мс");
    elementsEditField->setReadOnly(false);
    threadsEditField->setReadOnly(false);
    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
}


void mywidget::clickClose() {
	watcher->cancel();
	watcher->waitForFinished();
	close();
}


void mywidget::clickStop() {
	if (watcher->isFinished()) { return; }
	if (watcher->isPaused()) {
		watcher->resume();
        outputField->append(tr("Возобновлено"));
		stopBtn->setText(tr("Остановить"));
		} else {
        watcher->pause();
        outputField->append(tr("Остановлено"));
        stopBtn->setText(tr("Возобновить"));
	}
	
}


