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

    // 增加元素
    void addElement(QString parentNode, QString nodeName, QList<QMap<QString, QString> > attribute, QString text="");
    void addRecursiveElement(QString parentNode, QString nodeName, QList<QMap<QString, QString> > attributes, QList<QMap<QString, QString> > children);

    // 删除元素
    void removeElements(QString elementName);
    void removeElementByAttribute(QString elementName, QMap<QString, QString> attributeAndValue);

    // 修改元素
    void alterElementText(QString elementName);
    void alterElementTextByAttribute(QString elementName, QMap<QString, QString>attribute, QString text);
    void alterElementAttribute(QString elementName, QMap<QString, QString>attribute);

    // 重排 id
    void resetId();

    // 获得数据
    QList<QMap<QString, QList<QMap<QString, QString> > > > getElementAttributeValueAndChildrenText(QString elementName, QMap<QString, QString> elementAttribute, QStringList elementNames);

private:
    // 获得 dom
    QDomDocument getDomFromXml();

    // 保存 dom
    void savaDom(QDomDocument dom);

    const QString xmlPath;
};

#endif // XMLPROCESS_H
