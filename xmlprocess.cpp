#include "xmlprocess.h"
#include <QFile>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QXmlStreamReader>
#include <QString>
#include <QMessageBox>

XMLProcess::XMLProcess(const QString xmlPath) :
    xmlPath(xmlPath)
{
}

// 初始化 XML 文件
void XMLProcess::initXmlFile()
{
    QFile file(xmlPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(0, "配置文件错误", "XML 文件创建失败，用户信息无法保存！", QMessageBox::Ok);
        return;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();                // <?xml version="1.0" encoding="UTF-8"?>
    stream.writeStartElement("UserData");       // <UserData>
    stream.writeEndElement();
    stream.writeEndDocument();
    file.close();
}

// 获取 dom
QDomDocument XMLProcess::getDomFromXml()
{
    QDomDocument dom;

    // open file
    QFile fileReader(xmlPath);
    if (!fileReader.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(0, "配置文件错误", "XML 文件读取失败，用户信息无法保存！", QMessageBox::Ok);
        return dom;
    }

    // read dom
    dom.setContent(&fileReader);
    fileReader.close();

    //return dom
    return dom;
}

// 保存 dom
void XMLProcess::savaDom(QDomDocument dom)
{
    QFile fileWriter(xmlPath);
    if (!fileWriter.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(0, "配置文件错误", "XML 文件保存失败，用户信息无法保存！", QMessageBox::Ok);
        return;
    }
    QTextStream stream(&fileWriter);
    dom.save(stream, 4);
    fileWriter.close();
}

// 增加元素
void XMLProcess::addElement(QString parentNode, QString nodeName, QList<QMap<QString, QString> > attribute, QString text)
{
    // get dom
    QDomDocument dom;
    dom = getDomFromXml();

    // new element
    QDomElement parentElement = dom.elementsByTagName(parentNode).at(0).toElement();
    QDomElement newElement = dom.createElement(nodeName);
    for (int i=0; i<attribute.length(); ++i)
    {
        newElement.setAttribute(attribute.at(i).firstKey(), attribute.at(i).first());
    }
    if (text != "")
    {
        QDomText newElementText = dom.createTextNode(text);
        newElement.appendChild(newElementText);
    }
    parentElement.appendChild(newElement);

    // save dom
    savaDom(dom);
}

// 增加递归节点
void XMLProcess::addRecursiveElement(QString parentNode, QString nodeName, QList<QMap<QString, QString> > attributes, QList<QMap<QString, QString> > children)
{
    // get dom
    QDomDocument dom;
    dom = getDomFromXml();

    // new element
    QDomElement parentElement = dom.elementsByTagName(parentNode).at(0).toElement();
    QDomElement newElement = dom.createElement(nodeName);
    for (int i=0; i<attributes.length(); ++i)
    {
        newElement.setAttribute(attributes.at(i).firstKey(), attributes.at(i).first());
    }

    // add children
    for (int i=0; i<children.length(); ++i)
    {
        QDomElement child = dom.createElement(children.at(i).firstKey());
        QDomText text = dom.createTextNode(children.at(i).first());
        child.appendChild(text);
        newElement.appendChild(child);
    }

    // add new element
    parentElement.appendChild(newElement);

    // save dom
    savaDom(dom);
}

// 删除节点
void XMLProcess::removeElements(QString elementName)
{
    // get dom
    QDomDocument dom;
    dom = getDomFromXml();

    // find nodes
    QDomNodeList targetList = dom.elementsByTagName(elementName);
    Q_ASSERT_X(targetList.length() > 0, "removeXmlElements", "element name is not exist!");

    // remove node
    for (int i=0; i<targetList.length(); ++i)
    {
        QDomNode parent = targetList.at(i).parentNode();
        parent.removeChild(targetList.at(i));
    }

    // save dom
    savaDom(dom);
}

void XMLProcess::removeElementByAttribute(QString elementName, QMap<QString, QString> attributeAndValue)
{
    // get dom
    QDomDocument dom;
    dom = getDomFromXml();

    // find nodes
    QDomNodeList targetList = dom.elementsByTagName(elementName);
    QDomNode target;
    for (int i=0; i<targetList.length(); ++i)
    {
        QDomElement element = targetList.at(i).toElement();
        if (element.attributes().contains(attributeAndValue.firstKey()))
        {
            if (element.attribute(attributeAndValue.firstKey()) == attributeAndValue.first())
            {
                target = targetList.at(i);
            }
        }
    }

    // remove node
    Q_ASSERT_X(!target.isNull(), "removeXmlElementByAttribute", "element name is not exist!");
    QDomNode parent = target.parentNode();
    parent.removeChild(target);

    // save dom
    savaDom(dom);
}

// 修改元素文本
void XMLProcess::alterElementText(QString elementName)
{
    // get dom
    QDomDocument dom;
    dom = getDomFromXml();

    // alter node


    // save dom
    savaDom(dom);
}

// 修改具有某属性的元素的文本
void XMLProcess::alterElementTextByAttribute(QString elementName, QMap<QString, QString> attribute, QString text)
{
    // get dom
    QDomDocument dom;
    dom = getDomFromXml();

    // alter node


    // save dom
    savaDom(dom);
}

// 修改元素属性值
void XMLProcess::alterElementAttribute(QString elementName, QMap<QString, QString> attribute)
{
    // get dom
    QDomDocument dom;
    dom = getDomFromXml();

    // alter node


    // save dom
    savaDom(dom);
}

// 重排 id 值
void XMLProcess::resetId()
{
    // get dom
    QDomDocument dom;
    dom = getDomFromXml();

    // reset id
    QDomNodeList two = dom.childNodes();
    QList<QDomNode> root;
    for (int i=0; i<two.length(); ++i)
    {
        root.append(two.at(i));
    }

    while (root.length() > 0)
    {
        QDomNode parent = root.at(0);
        root.pop_front();
        QDomNodeList children = parent.childNodes();
        QString base = parent.toElement().attribute("name");
        int num = 0;
        for (int i=0; i<children.length(); ++i)
        {
            if (children.at(i).attributes().contains("id"))
            {
                QDomElement element = children.at(i).toElement();
                element.setAttribute("id", QObject::tr("%1-%2").arg(base).arg(num));
                num ++;
            }
            if (children.at(i).hasChildNodes())
            {
                root.append(children.at(i));
            }
        }
    }

    // save dom
    savaDom(dom);
}

// 获得数据
QList<QMap<QString, QList<QMap<QString, QString> > > > XMLProcess::getElementAttributeValueAndChildrenText(QString elementName, QMap<QString, QString> elementAttribute, QStringList elementNames)
{
    QList<QMap<QString, QList<QMap<QString, QString> > > > result;

    QFile file(xmlPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(0, "配置文件错误", "XML 文件打开失败，无法获取用户数据！", QMessageBox::Ok);
        return result;
    }

    QMap<QString, QList<QMap<QString, QString> > > values;
    QString attributeValue;
    QList<QMap<QString, QString> > brotherElementsTextList;
    QString firstValue;
    QString secondValue;

    QXmlStreamReader reader;
    reader.setDevice(&file);

    while (!reader.atEnd())
    {
        QXmlStreamReader::TokenType type = reader.readNext();

        switch(type)
        {
        case QXmlStreamReader::StartElement:
            if (reader.name() == elementName)
            {
                attributeValue = reader.attributes().at(0).value().toString();
            }
            else if (reader.name() == "url")
            {
                firstValue = reader.readElementText();
            }
            else if (reader.name() == "name")
            {
                secondValue = reader.readElementText();

                QMap<QString, QString> brotherElementsText;
                brotherElementsText.insert(firstValue, secondValue);
                brotherElementsTextList.append(brotherElementsText);
            }
            break;
        case QXmlStreamReader::EndElement:
            if (!attributeValue.isNull() && reader.name()==elementName)
            {
                values.insert(attributeValue, brotherElementsTextList);
                result.append(values);
                brotherElementsTextList.clear();
            }
            break;
        default:
            break;
        }
    }
    file.close();
    return result;
}
