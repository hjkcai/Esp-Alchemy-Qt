#include <QtGui>

QString readString(QXmlStreamReader &reader);

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QTextStream cout(stdout);

    QString prefix = "/Users/hjkcai/Desktop/drawable/%0.png";
    QString suffix[] = { "", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" };
    const int sflen = 37;
    int pf = 0, sf = 0;

    QFile *file = new QFile("/Users/hjkcai/Desktop/strings.xml"); file->open(QFile::ReadOnly | QFile::Text);
    QXmlStreamReader reader; reader.setDevice(file); reader.readNext();

    while (!reader.atEnd())
    {
        if (reader.isStartElement())
        {
            if (reader.name() == "resources")
            {
                QString result;
                while (!(result = readString(reader)).isNull())
                {
                    sf++;
                    if (sf >= sflen)
                    {
                        sf = 1;
                        pf++; if (pf == 1) pf = 2;
                    }

                    //cout << QString("j%0%1").arg(suffix[pf]).arg(suffix[sf]) << " -> " << result << endl;
                    QFile::rename(prefix.arg(QString("j%0%1").arg(suffix[pf]).arg(suffix[sf])), prefix.arg(result));
                }
            }
        }
        else reader.readNext();
    }

    file->close();
    app.exit();
}

QString readString(QXmlStreamReader &reader)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            if (reader.name() == "string")
                return reader.attributes().value("name").toString();
        }
    }

    return QString::null;
}
