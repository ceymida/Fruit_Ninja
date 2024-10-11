// Ceyda Unal - 22100011055
// fruit ninja oyunu - her oyun bitiminde kullanıcı adına göre dosyaya skorunu yazar
// ekledigim ekstra ozellik kullanici adi isteyip skorlar dosyama isimle yazdirma
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QPixmap>
#include <QVector>
#include <QLabel>
#include <QMouseEvent>
#include <QIcon>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void updateGame();
    void endGame();

private:
    Ui::Dialog *ui;
    QTimer *timer;
    QPixmap background;
    QPixmap watermelon;
    QPixmap slicedWatermelon;
    int gameTime;
    int slicedCount;
    int missedCount;
    bool gameEnded;
    QVector<QLabel*> watermelonLabels;

    void loadImages();
    void drawScores();
    void dosyadanOku(const QString &dosyaYolu);
    void addWatermelonLabel(int x, int y);
    void loadWatermelonPositions();
    void addRandomWatermelon();
};

#endif // DIALOG_H
