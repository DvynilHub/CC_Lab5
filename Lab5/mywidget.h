#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QtWidgets>

struct Task {
    int beginIndex;
    int endIndex;
    QList<qint64>* listPtr;
};

class mywidget : public QWidget
{
    Q_OBJECT
public:
    mywidget(QWidget *parent = 0);

signals:

public slots:
    void clickStart();
    void executionWithoutThread(int countElements);
    void executionWithThread(int countElements);
private slots:
    void progressValueChanged(int value);
    void finished();
    void clickClose();
    void clickStop();
private:
    QTextEdit *outputField;
    QFutureWatcher<qint64> *watcher;
    QElapsedTimer timer;
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QList<qint64> array;
    QList<Task> tasks;
    QFuture<qint64> future;
    QLineEdit *elementsEditField;
    QLineEdit *threadsEditField;
    QLabel *messageLbl;
};

#endif // MYWIDGET_H
