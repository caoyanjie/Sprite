#ifndef XMLPROCESS_H
#define XMLPROCESS_H

class QString;
class QDomDocument;
#include <QMap>
#include <QList>
#include <QString>
#include <QStringList>
class XMLProcess
{
public:
//    struct{
//        QString name;
//        QString id;
//        QString parentName;
//        QMap<QString, QString> parentAttributes;
//        QStringList childrenNames;
//    };

    XMLProcess(const QString xmlPath);

    // 初始化 xml 文件
    void initXmlFile();

    // 检测元素是否存在
    bool isElementExist(QString elementName);

    // 增加元素
    void addElement(QString parentName, QString elementName, QString attributeKey="", QString attributeValue="", QString elementText="");
    void addElement(QString parentName, QString elementName, QList<QMap<QString, QString> > attribute, QString elementText="");
    void addElementWithText(QString parentName, QString elementName, QString text);
    void addElements(QString parentName, QList<QMap<QString, QMap<QString, QString> > > children);
    void addRecursiveElement(QString parentNode, QString nodeName, QList<QMap<QString, QString> > attributes, QList<QMap<QString, QString> > children);
    void addRecursiveElement(QString parentName, QString parentAttributeKey, QString parentAttributeValue, QList<QMap<QString, QMap<QString, QString> > > selfNameAttributeKeyAndValue, QStringList childrenName, QList<QMap<QString, QString> > children);

    // 删除元素
    void removeElements(QString elementName);
    void removeElementByAttribute(QString elementName, QMap<QString, QString> attributeAndValue);
    void removeAllChildrenByAttribute(QString elementName, QString attributeKey, QString attributeValue);

    // 修改元素
    void alterElementText(QString parentName, QString selfName, QString text);
    void alterElementTextByAttribute(QString elementName, QMap<QString, QString>attribute, QString text);
    void alterElementAttribute(QString elementName, QMap<QString, QString>attribute);

    // 获得数据
    QList<QMap<QString, QList<QMap<QString, QString> > > > getElementAttributeValueAndChildrenText(QString elementName, QStringList elementNames);
    QString getElementText(QString elementName);

    const QString FirstChildElement;
    const QString MusicListElement;
    const QString MusicListElementKey;
    const QString MusicElement;
    const QString SettingElement;
    const QString ThemeElement;
    const QString VolumnElement;
    const QString PlayModeElement;

private:
    // 获得 dom
    QDomDocument getDomFromXml();

    // 保存 dom
    void savaDom(QDomDocument dom);

    // 重排 id
    void resetId(QDomDocument &dom);

    const QString xmlPath;
};

#endif // XMLPROCESS_H
