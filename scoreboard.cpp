#include "scoreboard.h"
#include "ui_scoreboard.h"

Scoreboard::Scoreboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Scoreboard)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./CityGuessDB.db");
    if (db.open()) {
        qDebug("opened");
    }
    else {
        qDebug("not opened");
    }
    query = new QSqlQuery(db);
    query->exec("CREATE TABLE Scoreboard ( Place INTEGER NOT NULL, Name	TEXT NOT NULL, Score INTEGER NOT NULL, PRIMARY KEY(Place) );");

    model = new QSqlTableModel(this, db);
    model->setTable("Scoreboard");
    model->select();
    QString names[6] = {"...", "...", "...", "...", "...", "..."};
    QString scores[6] = {"-", "-", "-", "-", "-", "-"};
    for (int i = 1; i <= model->rowCount(); i++) {
        if (query->exec("SELECT * FROM Scoreboard WHERE Place = " + QString::number(i))) {
            while (query->next()) {
                names[i - 1] = query->value(1).toString();
                scores[i - 1] = query->value(2).toString();
            }
        }
    }
    ui->top_name_1->setText(names[0]);
    ui->top_score_1->setText(scores[0]);
    ui->top_name_2->setText(names[1]);
    ui->top_score_2->setText(scores[1]);
    ui->top_name_3->setText(names[2]);
    ui->top_score_3->setText(scores[2]);
    ui->top_name_4->setText(names[3]);
    ui->top_score_4->setText(scores[3]);
    ui->top_name_5->setText(names[4]);
    ui->top_score_5->setText(scores[4]);
    ui->top_name_6->setText(names[5]);
    ui->top_score_6->setText(scores[5]);
}

Scoreboard::~Scoreboard()
{
    delete ui;
}

void Scoreboard::on_backToMenu_clicked()
{
        this->close();
}


void Scoreboard::on_pushButton_clicked()
{
    /*
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение сброса", "Вы действительно хотите сбросить таблицу рекордов?",
                                                                                                QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        qDebug() << query->exec("DELETE FROM Scoreboard");
        close();
    }
    */
    QMessageBox reply(QMessageBox::Question,
                      "Подтверждение сброса",
                      "Вы действительно хотите сбросить таблицу рекордов?",
                      QMessageBox::Yes | QMessageBox::No);
    reply.setButtonText(QMessageBox::Yes, "Да");
    reply.setButtonText(QMessageBox::No, "Нет");
    if(reply.exec() == QMessageBox::Yes) {
        qDebug() << query->exec("DELETE FROM Scoreboard");
        close();
    }
}

