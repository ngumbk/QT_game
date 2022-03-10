#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "gameover.h"
#include <QString>
#include <QMessageBox>
#include <ctime>
#include <cstdlib>

GameWindow::GameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    ui->Logo->setStyleSheet("image:url(:/util_img/util_img/Logo_pink_grey.png);");
    score = 0;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./CityGuessDB.db");
    if (db.open()) {
        qDebug("opened");
    }
    else {
        qDebug("not opened");
    }
    query = new QSqlQuery(db);
    model = new QSqlTableModel(this, db);

    //Настройка пака
    srand(time(0));
    ShufflePack(pack_order, 10);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot()));
    timer->start(1000);
    LockAllButtons();
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::GetSettings(int timer, bool hintsEnabled, int pack_index) {
    QString hints;
    this->hintsEnabled = hintsEnabled;
    timer_time = timer + 4; //3 доп секунды на подготовку
    session_time = timer;
    if (!hintsEnabled) {
        ui->hint_5050->setEnabled(false);
        ui->hint_gym->setEnabled(false);
        ui->hint_friend->setEnabled(false);
    }
    if (pack_index == 0) {
        pack_prefix = "RU";
        source_path_template = "image:url(:/Packs/Packs/RussiaPack/";
    }
    else if (pack_index == 1) {
        pack_prefix = "WD";
        source_path_template = "image:url(:/Packs/Packs/WorldPack/";
    }
}

void GameWindow::ShufflePack(int m[], int size) {
    for (int i = 0; i < size; ++i)
        std::swap(m[i], m[std::rand() % size]);
}

void GameWindow::InsertAnswers() {
    switch (correct_answer_idx)
          {
             case 0:
                ui->option_1->setText(correct_answer);
                ui->option_2->setText(incorrect_answers.value(0));
                ui->option_3->setText(incorrect_answers.value(1));
                ui->option_4->setText(incorrect_answers.value(2));
                break;
             case 1:
                ui->option_1->setText(incorrect_answers.value(0));
                ui->option_2->setText(correct_answer);
                ui->option_3->setText(incorrect_answers.value(1));
                ui->option_4->setText(incorrect_answers.value(2));
                break;
             case 2:
                ui->option_1->setText(incorrect_answers.value(0));
                ui->option_2->setText(incorrect_answers.value(1));
                ui->option_3->setText(correct_answer);
                ui->option_4->setText(incorrect_answers.value(2));
                break;
             case 3:
                ui->option_1->setText(incorrect_answers.value(0));
                ui->option_2->setText(incorrect_answers.value(1));
                ui->option_3->setText(incorrect_answers.value(2));
                ui->option_4->setText(correct_answer);
                break;
          }
}

void GameWindow::SetupNextPicture() {
    query->exec("SELECT * FROM Images WHERE PicName LIKE '" + pack_prefix + QString::number(pack_order[pack_idx]) + "%'");
    query->next();
    ui->Picture->setStyleSheet(source_path_template + query->value(0).toString() + ")");
    correct_answer = query->value(1).toString();
    correct_answer_idx = std::rand() % 4;

    incorrect_answers_raw = query->value(2).toString();
    incorrect_answers = incorrect_answers_raw.split("/");
    InsertAnswers();
    pack_idx++;
    query->clear();
}

void GameWindow::LockAllButtons() {
    ui->option_1->setDisabled(true);
    ui->option_2->setDisabled(true);
    ui->option_3->setDisabled(true);
    ui->option_4->setDisabled(true);
    ui->hint_5050->setDisabled(true);
    ui->hint_friend->setDisabled(true);
    ui->hint_gym->setDisabled(true);
}

void GameWindow::RestoreButtons() {
    if (hintsEnabled) {
        if (!ui->hint_5050->isEnabled()) {
            ui->option_1->setEnabled(true);
            ui->option_2->setEnabled(true);
            ui->option_3->setEnabled(true);
            ui->option_4->setEnabled(true);
        }
        if (!ui->hint_friend->isEnabled()) {
            ui->option_1->setStyleSheet("");
            ui->option_2->setStyleSheet("");
            ui->option_3->setStyleSheet("");
            ui->option_4->setStyleSheet("");
        }
        if (!ui->hint_gym->isEnabled()) {
            ui->o1_percent->setStyleSheet("");
            ui->o2_percent->setStyleSheet("");
            ui->o3_percent->setStyleSheet("");
            ui->o4_percent->setStyleSheet("");
            ui->o1_percent->setText("");
            ui->o2_percent->setText("");
            ui->o3_percent->setText("");
            ui->o4_percent->setText("");
        }
    }
}

void GameWindow::TimerSlot()
{
    timer_time--;
    ui->TimeLabel->setText(QString::number(timer_time));
    if(timer_time == session_time) { //игра началась
        ui->TimeLabel->setStyleSheet("font: 700 16pt \"Sitka Small\"");
        ui->Picture->setText("");
        ui->option_1->setEnabled(true);
        ui->option_2->setEnabled(true);
        ui->option_3->setEnabled(true);
        ui->option_4->setEnabled(true);
        if (!hintsEnabled) {
            ui->hint_5050->setEnabled(false);
            ui->hint_gym->setEnabled(false);
            ui->hint_friend->setEnabled(false);
        }
        else {
            ui->hint_5050->setEnabled(true);
            ui->hint_friend->setEnabled(true);
            ui->hint_gym->setEnabled(true);
        }
        model->setTable("Images");
        model->select();

        SetupNextPicture();
    }
    if (timer_time == 0) { //игра закончилась
        timer->stop();
        model->setTable("Scoreboard");
        model->select();
        query->exec("SELECT COUNT(*) FROM Scoreboard WHERE Score < " + QString::number(score));
        query->next();
        if ((model->rowCount() < 6) || (query->value(0).toInt() > 0)) { //Scoreboard еще не заполнен полностью
            GameOver gameover;
            gameover.get_score(score);
            gameover.setModal(true);
            gameover.exec();
        }
        else {
            qDebug() << "Рекорд не установлен.";
        }
        ui->option_1->setDisabled(true);
        ui->option_2->setDisabled(true);
        ui->option_3->setDisabled(true);
        ui->option_4->setDisabled(true);
        ui->hint_5050->setDisabled(true);
        ui->hint_friend->setDisabled(true);
        ui->hint_gym->setDisabled(true);
    }
}

void GameWindow::on_PauseButton_clicked()
{
    this->ui->stackedWidget->setCurrentWidget(this->ui->Pause);
    timer->stop();
}


void GameWindow::on_Continue_clicked()
{
    this->ui->stackedWidget->setCurrentWidget(this->ui->Game);
    if(timer_time > 0) {
        timer->start(1000);
    }
}


void GameWindow::on_backToMenu_clicked()
{
    this->close();
}


void GameWindow::on_Exit_clicked()
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
}

void GameWindow::on_option_1_clicked()
{
    RestoreButtons();
    if (correct_answer_idx == 0) {
        score++;
        ui->ScoreLabel->setText(QString::number(score));
    }
    if (pack_idx < 10) {
        SetupNextPicture();
    }
    else {
        ui->Picture->setStyleSheet("font: 700 italic 22pt \"Sitka Small\";"
                                   "background-color: rgb(195, 195, 195);"
                                   "color: rgb(0, 0, 0);");
        ui->Picture->setText("\tНабор закончился.\n\tСпасибо за игру :)");
        LockAllButtons();
    }
}


void GameWindow::on_option_2_clicked()
{
    RestoreButtons();
    if (correct_answer_idx == 1) {
        score++;
        ui->ScoreLabel->setText(QString::number(score));
    }
    if (pack_idx < 10) {
        SetupNextPicture();
    }
    else {
        ui->Picture->setStyleSheet("font: 700 italic 22pt \"Sitka Small\";"
                                   "background-color: rgb(195, 195, 195);"
                                   "color: rgb(0, 0, 0);");
        ui->Picture->setText("\tНабор закончился.\n\tСпасибо за игру :)");
        LockAllButtons();
    }
}


void GameWindow::on_option_3_clicked()
{
    RestoreButtons();
    if (correct_answer_idx == 2) {
        score++;
        ui->ScoreLabel->setText(QString::number(score));
    }

    if (pack_idx < 10) {
        SetupNextPicture();
    }
    else {
        ui->Picture->setStyleSheet("font: 700 italic 22pt \"Sitka Small\";"
                                   "background-color: rgb(195, 195, 195);"
                                   "color: rgb(0, 0, 0);");
        ui->Picture->setText("\tНабор закончился.\n\tСпасибо за игру :)");
        LockAllButtons();
    }
}


void GameWindow::on_option_4_clicked()
{
    RestoreButtons();
    if (correct_answer_idx == 3) {
        score++;
        ui->ScoreLabel->setText(QString::number(score));
    }

    if (pack_idx < 10) {
        SetupNextPicture();
    }
    else {
        ui->Picture->setStyleSheet("font: 700 italic 22pt \"Sitka Small\";"
                                   "background-color: rgb(195, 195, 195);"
                                   "color: rgb(0, 0, 0);");
        ui->Picture->setText("\tНабор закончился.\n\tСпасибо за игру :)");
        LockAllButtons();
    }
}


void GameWindow::on_hint_5050_clicked()
{
    int t[3] = {0, 1, 2};
    ShufflePack(t, 3);

    if (incorrect_answers.value(t[0]) == ui->option_1->text()) {
        ui->option_1->setDisabled(true);
    }
    else if (incorrect_answers.value(t[0]) == ui->option_2->text()) {
        ui->option_2->setDisabled(true);
    }
    else if (incorrect_answers.value(t[0]) == ui->option_3->text()) {
        ui->option_3->setDisabled(true);
    }
    else if (incorrect_answers.value(t[0]) == ui->option_4->text()) {
        ui->option_3->setDisabled(true);
    }

    if (incorrect_answers.value(t[1]) == ui->option_1->text()) {
        ui->option_1->setDisabled(true);
    }
    else if (incorrect_answers.value(t[1]) == ui->option_2->text()) {
        ui->option_2->setDisabled(true);
    }
    else if (incorrect_answers.value(t[1]) == ui->option_3->text()) {
        ui->option_3->setDisabled(true);
    }
    else if (incorrect_answers.value(t[1]) == ui->option_4->text()) {
        ui->option_3->setDisabled(true);
    }
    ui->hint_5050->setDisabled(true);
}


void GameWindow::on_hint_friend_clicked()
{
    switch (correct_answer_idx) {
    case 0:
        ui->option_1->setStyleSheet("background-color: rgb(4, 255, 0);");
        break;
    case 1:
        ui->option_2->setStyleSheet("background-color: rgb(4, 255, 0);");
        break;
    case 2:
        ui->option_3->setStyleSheet("background-color: rgb(4, 255, 0);");
        break;
    case 3:
        ui->option_4->setStyleSheet("background-color: rgb(4, 255, 0);");
        break;
    }
    ui->hint_friend->setDisabled(true);
}


void GameWindow::on_hint_gym_clicked()
{
    int correct_percent = rand() % 100;
    if (correct_percent < 50) {
        correct_percent = 100 - correct_percent;
    }
    int sum_incorrect_percent = 100 - correct_percent;
    int percent_allocated = 0;
    switch (correct_answer_idx) {
    case 0:
        ui->o1_percent->setText(" " + QString::number(correct_percent) + "%"); //+
        percent_allocated = abs(sum_incorrect_percent - rand() % sum_incorrect_percent - 2);
        sum_incorrect_percent -= percent_allocated;
        ui->o2_percent->setText(" " + QString::number(percent_allocated) + "%");
        percent_allocated = abs(sum_incorrect_percent - rand() % sum_incorrect_percent - 2);
        sum_incorrect_percent -= percent_allocated;
        ui->o3_percent->setText(" " + QString::number(percent_allocated) + "%");
        ui->o4_percent->setText(" " + QString::number(sum_incorrect_percent) + "%");
        break;
    case 1:
        ui->o2_percent->setText(" " + QString::number(correct_percent) + "%");
        percent_allocated = abs(sum_incorrect_percent - rand() % sum_incorrect_percent - 2);
        sum_incorrect_percent -= percent_allocated;
        ui->o1_percent->setText(" " + QString::number(percent_allocated) + "%");
        percent_allocated = abs(sum_incorrect_percent - rand() % sum_incorrect_percent - 2);
        sum_incorrect_percent -= percent_allocated;
        ui->o3_percent->setText(" " + QString::number(percent_allocated) + "%");
        ui->o4_percent->setText(" " + QString::number(sum_incorrect_percent) + "%");
        break;
    case 2:
        ui->o3_percent->setText(" " + QString::number(correct_percent) + "%");
        percent_allocated = abs(sum_incorrect_percent - rand() % sum_incorrect_percent - 2);
        sum_incorrect_percent -= percent_allocated;
        ui->o2_percent->setText(" " + QString::number(percent_allocated) + "%");
        percent_allocated = abs(sum_incorrect_percent - rand() % sum_incorrect_percent - 2);
        sum_incorrect_percent -= percent_allocated;
        ui->o1_percent->setText(" " + QString::number(percent_allocated) + "%");
        ui->o4_percent->setText(" " + QString::number(sum_incorrect_percent) + "%");
        break;
    case 3:
        ui->o4_percent->setText(" " + QString::number(correct_percent) + "%");
        percent_allocated = abs(sum_incorrect_percent - rand() % sum_incorrect_percent - 2);
        sum_incorrect_percent -= percent_allocated;
        ui->o2_percent->setText(" " + QString::number(percent_allocated) + "%");
        percent_allocated = abs(sum_incorrect_percent - rand() % sum_incorrect_percent - 2);
        sum_incorrect_percent -= percent_allocated;
        ui->o3_percent->setText(" " + QString::number(percent_allocated) + "%");
        ui->o1_percent->setText(" " + QString::number(sum_incorrect_percent) + "%");
        break;
    }

    ui->o1_percent->setStyleSheet("background-color: rgb(255, 248, 30);");
    ui->o2_percent->setStyleSheet("background-color: rgb(255, 248, 30);");
    ui->o3_percent->setStyleSheet("background-color: rgb(255, 248, 30);");
    ui->o4_percent->setStyleSheet("background-color: rgb(255, 248, 30);");
    ui->hint_gym->setDisabled(true);
}

