// Ceyda Unal - 22100011055
// fruit ninja oyunu - her oyun bitiminde kullanıcı adına göre dosyaya skorunu yazar
// ekledigim ekstra ozellik kullanici adi isteyip skorlar dosyama isimle yazdirma
#include "dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    return a.exec();
}
