#ifndef HIGLIGHTER_H
#define HIGLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QRegularExpression>

class Higlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Higlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;



};

#endif // HIGLIGHTER_H
