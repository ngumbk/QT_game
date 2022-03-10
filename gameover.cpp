#include "gameover.h"
#include "ui_gameover.h"

GameOver::GameOver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameOver)
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
    model = new QSqlTableModel(this, db);

    model->setTable("Scoreboard");
    model->select();
}

GameOver::~GameOver()
{
    delete ui;
}

void GameOver::get_score(int score) {
    this->score = score;
}

void GameOver::on_OkButton_clicked()
{
    qDebug() << model->rowCount();
    if (query->exec("SELECT Place, Name, Score FROM Scoreboard WHERE Score < " + QString::number(score) + " ORDER BY Place")) { //если найдутся такие элементы, то...
        if (query->next()) { //начало и центр таблицы
            qDebug() << "Новый игрок: " + ui->lineEdit->text() + " " + QString::number(score);
            qDebug() << "Место нового игрока: " + query->value(0).toString();
            new_place = query->value(0).toInt();
            //Сдвиг таблицы
            for (int i = model->rowCount(); i >= new_place; i--) { //от 6 до нового места
                query->exec("UPDATE Scoreboard SET Place = " + QString::number(i + 1) + " WHERE Place = " + QString::number(i));
                qDebug() << "Старый лидер переехал с " << QString::number(i) + " места на " + QString::number(i + 1);
            }
            //Вставка новой строки
            bool test = query->exec("INSERT INTO Scoreboard (Place, Name, Score) VALUES (" + QString::number(new_place) + ", \'" + ui->lineEdit->text() + "\', " + QString::number(score) + ")");
            qDebug() << "Новый лидер сел на место " + QString::number(new_place) + "? " + QString::number(test);
            qDebug() << "INSERT INTO Scoreboard (Place, Name, Score) VALUES (" + QString::number(new_place) + ", \'" + ui->lineEdit->text() + "\', " + QString::number(score) + ")";
        }
        else if(model->rowCount() < 6) { //просто инсертнуть в конец
            new_place = model->rowCount() + 1;
            qDebug() << "Для нового лидера нашлось место внизу списка. " + QString::number(new_place);
            query->exec("INSERT INTO Scoreboard (Place, Name, Score) VALUES "
                        "(" + QString::number(new_place) + ", \'" + ui->lineEdit->text() + "\', " + QString::number(score) + ")");
            qDebug() << "Новый лидер добавлен в незаполненную таблицу. Проверьте БД.";
        }
        else {
            qDebug() << "Чет не вышло :( Ищи ошибку.";
        }
    }
    //Обновляем модель для корректного отображения .rowCount()
    model->setTable("Scoreboard");
    model->select();
    qDebug() << model->rowCount();

    //Удаляем 7-ю строку, если она есть
    if (model->rowCount() == 7) {
        qDebug() << query->exec("DELETE FROM Scoreboard WHERE Place = 7");
    }
    qDebug() << "7 строка удалена.";
    close();
}

