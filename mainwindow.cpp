#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scoreboard.h"
#include "gamewindow.h"

#include <QStatusBar>
#include <QString>
#include <QMessageBox>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->HintsCheck->setChecked(true);
    ui->logo->setStyleSheet("image:url(:/util_img/util_img/Logo_pink_grey.png);");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_playButton_clicked()
{
    statusBar()->showMessage("Преднастройки игры", 8000);
    this->ui->stackedWidget->setCurrentWidget(this->ui->settingsLayer);
}


void MainWindow::on_scoreboardButton_clicked()
{
    statusBar()->showMessage("Таблица лидеров открыта");
    Scoreboard scoreboard_window;
    scoreboard_window.setModal(true);
    scoreboard_window.exec();
    statusBar()->clearMessage();
}


void MainWindow::on_exitButton_clicked()
{
    QMessageBox reply(QMessageBox::Question,
                      "Подтверждение выхода",
                      "Вы действительно хотите выйти из игры?",
                      QMessageBox::Yes | QMessageBox::No);
    reply.setButtonText(QMessageBox::Yes, "Да");
    reply.setButtonText(QMessageBox::No, "Нет");
    if(reply.exec() == QMessageBox::Yes) {
        QApplication::quit();
    }
    else {
        statusBar()->showMessage("Выход отменен", 8000);
    }
}




void MainWindow::on_BackToMenu_clicked()
{
    statusBar()->showMessage("Возврат в меню", 8000);
    this->ui->stackedWidget->setCurrentWidget(this->ui->menuLayer);
}


void MainWindow::on_TimerSlider_valueChanged(int value)
{
    value = this->ui->TimerSlider->value() * 30;
    this->ui->TimerValueL->setText(QString::number(value));
}


void MainWindow::on_PlayButton2_clicked()
{

    statusBar()->showMessage("Игра");
    GameWindow game_session;
    game_session.GetSettings(this->ui->TimerSlider->value() * 30, this->ui->HintsCheck->isChecked(), this->ui->PackChoiceBox->currentIndex());
    this->ui->stackedWidget->setCurrentWidget(this->ui->menuLayer);
    game_session.setModal(true);
    game_session.exec();
    statusBar()->clearMessage();

}

