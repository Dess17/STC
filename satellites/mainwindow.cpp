#include "mainwindow.h"
#include "tle.h"

#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QNetworkReply>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    urlEdit = new QLineEdit("http://celestrak.org/NORAD/elements/supplemental/sup-gp.php?FILE=iridium&FORMAT=tle");
    QPushButton *urlButton = new QPushButton("Загрузить по ссылке");
    QPushButton *fileButton = new QPushButton("Открыть файл");
    QPushButton *saveButton = new QPushButton("Сохранить");
    output = new QPlainTextEdit;
    output->setReadOnly(true);

    QHBoxLayout *top = new QHBoxLayout;
    top->addWidget(urlEdit);
    top->addWidget(urlButton);
    top->addWidget(fileButton);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(top);
    layout->addWidget(output);
    layout->addWidget(saveButton);

    connect(urlButton, &QPushButton::clicked, this, &MainWindow::loadUrl);
    connect(fileButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::save);
    resize(700, 500);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл: " + file.errorString());
        return;
    }
    showData(file.readAll());
}

void MainWindow::loadUrl()
{
    QUrl url(urlEdit->text().trimmed());
    if (!url.isValid() || url.scheme().isEmpty() || url.host().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Некорректная ссылка");
        return;
    }

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    QNetworkReply *reply = network.get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(this, "Ошибка", "Не удалось загрузить данные: " + reply->errorString());
            return;
        }
        showData(reply->readAll());
    });
}

void MainWindow::showData(const QByteArray &data)
{
    QString error;
    QVector<Satellite> satellites = parseTle(QString::fromUtf8(data), error);
    if (!error.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат данных, " + error);
        return;
    }
    output->setPlainText(statistics(satellites));
}

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, QString(), "statistics.txt");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл: " + file.errorString());
        return;
    }
    file.write(output->toPlainText().toUtf8());
}
