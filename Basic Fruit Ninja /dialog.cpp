// Ceyda Unal - 22100011055
// fruit ninja oyunu - her oyun bitiminde kullanıcı adına göre dosyaya skorunu yazar
// ekledigim ekstra ozellik kullanici adi isteyip skorlar dosyama isimle yazdirma

#include "dialog.h"
#include "ui_dialog.h"
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QInputDialog>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    loadImages();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Dialog::updateGame);
    gameTime = 30;
    slicedCount = 0;
    missedCount = 0;
    gameEnded = false;
    timer->start(1000);
    //loadWatermelonPositions(); // Karpuzların konumlarını dosyadan yükle
    QString dosyaYolu = "/Users/ceydaunal/Desktop/konumlar.txt"; // random coordinates
    dosyadanOku(dosyaYolu);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::loadImages()
{
    background.load("/Users/ceydaunal/Desktop/back.jpg");  //background image
    watermelon.load("/Users/ceydaunal/Desktop/1.png");    //watermelon image
    slicedWatermelon.load("/Users/ceydaunal/Desktop/2.png");//sliced watermelon image
}

void Dialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0, background);
    drawScores();
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    if (gameEnded) return;

    int x = event->x();
    int y = event->y();

    // Karpuzların koordinatlarını kontrol et
    for (int i = 0; i < watermelonLabels.size(); ++i) {
        if (watermelonLabels[i]->geometry().contains(x, y)) {
            // Kesilmiş karpuz görseli oluştur
            QLabel *slicedLabel = new QLabel(this);
            slicedLabel->setPixmap(slicedWatermelon.scaledToWidth(50)); // Kesilmiş karpuz boyutunu küçült
            slicedLabel->setGeometry(watermelonLabels[i]->geometry());
            slicedLabel->show();

            // 2 saniye sonra kesilmiş karpuz görselini kaldır
            QTimer::singleShot(2000, slicedLabel, &QWidget::deleteLater);

            delete watermelonLabels[i];
            watermelonLabels.removeAt(i);
            ++slicedCount;
            update();
            return;
        }
    }
    ++missedCount;
}

void Dialog::updateGame()
{
    if (gameTime == 0) {
        endGame();
        return;
    }
    // Yeni karpuz ekle
    if (rand() % 100 < 80 ) { // Her 2 saniyede bir yeni karpuz ekleme

        addRandomWatermelon();
    }

    // Karpuzların konumlarını güncelle
    for (int i = 0; i < watermelonLabels.size(); ++i) {
        watermelonLabels[i]->move(watermelonLabels[i]->x(), watermelonLabels[i]->y() + 20); // Yavaşça aşağı inme
        // Karpuz ekranın dışına çıktıysa kaçırıldı olarak işaretle
        if (watermelonLabels[i]->y() > height()) {
            delete watermelonLabels[i];
            watermelonLabels.removeAt(i);
            ++missedCount;
            --i; // Çünkü bir önceki karpuzun indisini kontrol ediyoruz
        }
    }
    --gameTime;
    update();
}
void Dialog::addRandomWatermelon()
{
    QFile file("/Users/ceydaunal/Desktop/konumlar.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open positions.txt";
        return;
    }

    QList<QPoint> positions;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList coords = line.split(" ");
        int x = coords[0].toInt();
        int y = coords[1].toInt();
        positions.append(QPoint(x, y));
    }
    file.close();

    if (!positions.isEmpty()) {
        // Rastgele bir konum seç
        int index = rand() % positions.size();
        QPoint position = positions[index];

        // Karpuzu ekle
        addWatermelonLabel(position.x(), position.y());
    }
}

void Dialog::addWatermelonLabel(int x, int y)
{
    QLabel *watermelonLabel = new QLabel(this);
    QPixmap scaledPixmap = watermelon.scaledToWidth(50); // kesilmis karpuz boyutunu küçültme
    watermelonLabel->setPixmap(scaledPixmap);
    // watermelonLabel->setGeometry(rand()%800 , rand()%600,70,70);
    watermelonLabel->setGeometry(x, y, 70, 70); // Label boyutları ayarla
    watermelonLabel->show();
    watermelonLabels.append(watermelonLabel);


    /* QLabel * watermelonLabel =new QLabel(this);
    QIcon *icon = new QIcon();
    icon->addPixmap(QPixmap("/Users/ceydaunal/Desktop/1.png"));
    watermelonLabel ->setIcon(*icon);
    watermelonLabel ->setIconSize(QSize(70,70));
    watermelonLabel->setGeometry(x, y, 70, 70);
    //konumlar .txt den konumları geçmemiz lazım
    //karpuz->setGeometry(rand()%800 , rand()%600,70,70);
    watermelonLabel->show();
    watermelonLabels.append(watermelonLabel);*/

}

void Dialog::endGame()
{
    timer->stop();
    gameEnded = true;
    // Get user's name
    QString playerName = QInputDialog::getText(this, tr("Enter Your Name"), tr("Please enter your name:"));

    // Skoru kaydet
    QFile file("/Users/ceydaunal/Desktop/scores.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Cannot open scores.txt for writing";
        return;
    }
    QTextStream out(&file);
    out << "Player: " << playerName << ", Score: " << slicedCount << ", Missed: " << missedCount << Qt::endl;
    file.close();
    // Kontrol ekranı göster
    QMessageBox::information(this, "Game Over", "Game Over! Score: " + QString::number(slicedCount) + ", Missed: " + QString::number(missedCount));
}

void Dialog::drawScores()
{
    QPainter painter(this);
    painter.drawText(10, 20, "Time: " + QString::number(gameTime));
    //ui->LabelSure->setText("<font color='blue"+QString::number(gameTime)+"</font>");
    painter.drawText(10, 40, "Score: " + QString::number(slicedCount));
    painter.drawText(10, 60, "Missed: " + QString::number(missedCount));
}

void Dialog::dosyadanOku(const QString &dosyaYolu)
{

    QFile konumlar(dosyaYolu);
    if (!konumlar.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open positions.txt";
        return;
    }
    QTextStream okuyucu(&konumlar);
    while (!okuyucu.atEnd()) {
        QString line = okuyucu.readLine();
        QStringList coords = line.split(" ");
        if (coords.size() == 2) {
            // İlk parçayı x'e, ikinci parçayı y'ye ata
            int x = coords[0].toInt();
            int y = coords[1].toInt();
            // Burada x ve y değerlerini kullanabilirsiniz
            addWatermelonLabel(x, y);
        }
    }
    konumlar.close();
}


