#include <QtGui>

class map
{
public:
    map(const QString &_id, const QString &_value)
    {
        id = _id;
        value = _value;
    }

    QString id, value;

    bool isNull() const
    {
        return id.isNull();
    }

    static map null() { return map(QString::null, QString::null); }
};

//class

typedef QList<map> maps;

map readMap(QXmlStreamReader &reader);
maps readAll(QXmlStreamReader &reader);
int findMap(const maps &source, const QString &value);
int findMapID(const maps &source, const QString &id);
QString translate(const maps &chs, const maps &eng, const QString &id);
QString getID(const maps &source, const QString &value);

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QTextStream cout(stdout);

    QFile *ori_f = new QFile("/Users/hjkcai/Desktop/combinations.txt"); ori_f->open(QFile::ReadWrite | QFile::Text);
    QFile *english_f = new QFile("/Users/hjkcai/Desktop/strings.xml"); english_f->open(QFile::ReadOnly | QFile::Text);
    QFile *chs_f = new QFile("/Users/hjkcai/Desktop/strings_chs.xml"); chs_f->open(QFile::ReadOnly | QFile::Text);

    //QString ori = ori_f->readAll();
    QXmlStreamReader english_r(english_f), chs_r(chs_f);
    english_r.readNext(); chs_r.readNext();

    maps eng = readAll(english_r), chs = readAll(chs_r);

    QString new_f, format1 = "%0%1:%2=%3\n";

    while (!ori_f->atEnd())
    {
        QString s = ori_f->readLine(); s = s.trimmed();
        QString translation;
        QStringList ss = s.split('=');
        QStringList ss1 = ss[1].split('+');

        bool terminal = ss[0][ss[0].length() - 1] == '*';
        if (terminal) ss[0] = ss[0].mid(0, ss[0].length() - 1);

        ss[0] = getID(eng, ss[0]);
        translation = translate(chs, eng, ss[0]);

        QString append;
        for (int i = 0; i < ss1.length(); i++)
        {
            ss1[i] = getID(eng, ss1[i]);
            //cout << ss1[i] << endl;
            append += ss1[i] + "+";
        }
        append = append.mid(0, append.length() - 1);

        new_f.append(format1.arg(terminal ? "*" : "").arg(ss[0]).arg(translation).arg(append));
        //cout << append << endl;
        //cout << format1.arg(terminal ? "*" : "").arg(ss[0]).arg(translation).arg(append) << endl;
    }

    QFile *out = new QFile("/Users/hjkcai/Desktop/out.txt");
    out->open(QFile::WriteOnly | QFile::Text);

    QTextStream ts(out);
    QTextCodec *codec = QTextCodec::codecForName("Utf-8");
    ts.setCodec(codec);
    ts << new_f;

    out->close();
    //ori_f->readLine()

    app.exit();
}

map readMap(QXmlStreamReader &reader)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            if (reader.name() == "string")
            {
                QString id = reader.attributes().value("name").toString(), value = reader.readElementText();
                return map(id, value);
            }
        }
    }

    return map::null();
}

maps readAll(QXmlStreamReader &reader)
{
    maps ret;
    while (!reader.atEnd())
    {
        if (reader.isStartElement())
        {
            if (reader.name() == "resources")
            {
                map result = map::null();
                while (!(result = readMap(reader)).isNull())
                    ret << result;
            }
        }
        else reader.readNext();
    }

    return ret;
}

int findMap(const maps &source, const QString &value)
{
    for (int i = 0; i < source.length(); i++)
    {
        if (source[i].value.toLower() == value.toLower())
            return i;
    }

    return -1;
}

int findMapID(const maps &source, const QString &id)
{
    for (int i = 0; i < source.length(); i++)
    {
        if (source[i].id.toLower() == id.toLower())
            return i;
    }

    return -1;
}

QString translate(const maps &chs, const maps &eng, const QString &id)
{
    QString result;
//QTextStream cout(stdout);
    int index = findMapID(chs, id);
    //cout << index << endl;
    if (index == -1)
    {
        index = findMapID(eng, id);
        if (index == -1) result = id + QString("(!)");
        else result = eng[index].value;
    }
    else result = chs[index].value;

    return result;
}

QString getID(const maps &source, const QString &value)
{
    QString result;

    int index = findMap(source, value);
    if (index == -1)
        result = value + QString("(!)");
    else result = source[index].id;

    return result;
}
