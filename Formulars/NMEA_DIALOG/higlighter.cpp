#include "higlighter.h"

Higlighter::Higlighter(QTextDocument *parent)
                     : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    QTextCharFormat keywordPOHPR;
    keywordPOHPR.setForeground(QColor(180, 0, 180));
    rule.pattern = QRegularExpression("POHPR[^*]*");
    rule.format = keywordPOHPR;
    highlightingRules.append(rule);

    QTextCharFormat keywordGPGGA;
    keywordGPGGA.setForeground(QColor(0, 120, 120));
    rule.pattern = QRegularExpression("[^$]*GGA[^*]*");
    rule.format = keywordGPGGA;
    highlightingRules.append(rule);

    QTextCharFormat keywordGPZDA;
    keywordGPZDA.setForeground(QColor(120, 180, 0));
    rule.pattern = QRegularExpression("[^$]*ZDA[^*]*");
    rule.format = keywordGPZDA;
    highlightingRules.append(rule);

    QTextCharFormat keywordGPHDT;
    keywordGPHDT.setForeground(QColor(20, 0, 180));

    rule.pattern = QRegularExpression("GPHDT[^*]*");
    rule.format = keywordGPHDT;
    highlightingRules.append(rule);

    QTextCharFormat keywordPORZV;
    keywordPORZV.setForeground(QColor(180, 120, 0));
    rule.pattern = QRegularExpression("PORZV[^*]*");
    rule.format = keywordPORZV;
    highlightingRules.append(rule);

    QTextCharFormat keywordPOUGT;
    keywordPOUGT.setForeground(QColor(255, 20, 20));
    rule.pattern = QRegularExpression("POUGT[^*]*");
    rule.format = keywordPOUGT;
    highlightingRules.append(rule);
}

void Higlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpression expression(rule.pattern);
        QRegularExpressionMatchIterator i = expression.globalMatch(text);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            int startOffset = match.capturedStart();
            int lengthOffset = match.capturedLength();
            setFormat(startOffset, lengthOffset, rule.format);
        }
    }
}
