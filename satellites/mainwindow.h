#pragma once

#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QPlainTextEdit>
#include <QWidget>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void openFile();
    void loadUrl();
    void showData(const QByteArray &data);
    void save();

    QLineEdit *urlEdit;
    QPlainTextEdit *output;
    QNetworkAccessManager network;
};
