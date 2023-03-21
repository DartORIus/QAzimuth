#include "higlighter.h"

Higlighter::Higlighter(QTextDocument *parent)
                     : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    QTextCharFormat keywordPOHPR;
    keywordPOHPR.setForeground(QColor(180, 0, 180));
    rule.pattern = QRegExp("POHPR[^*]*");
    rule.format = keywordPOHPR;
    highlightingRules.append(rule);

    QTextCharFormat keywordGPGGA;
    keywordGPGGA.setForeground(QColor(0, 120, 120));
    rule.pattern = QRegExp("[^$]*GGA[^*]*");
    rule.format = keywordGPGGA;
    highlightingRules.append(rule);

    QTextCharFormat keywordGPZDA;
    keywordGPZDA.setForeground(QColor(120, 180, 0));
    rule.pattern = QRegExp("[^$]*ZDA[^*]*");
    rule.format = keywordGPZDA;
    highlightingRules.append(rule);

    QTextCharFormat keywordGPHDT;
    keywordGPHDT.setForeground(QColor(20, 0, 180));

    rule.pattern = QRegExp("GPHDT[^*]*");
    rule.format = keywordGPHDT;
    highlightingRules.append(rule);

    QTextCharFormat keywordPORZV;
    keywordPORZV.setForeground(QColor(180, 120, 0));
    rule.pattern = QRegExp("PORZV[^*]*");
    rule.format = keywordPORZV;
    highlightingRules.append(rule);

    QTextCharFormat keywordPOUGT;
    keywordPOUGT.setForeground(QColor(255, 20, 20));
    rule.pattern = QRegExp("POUGT[^*]*");
    rule.format = keywordPOUGT;
    highlightingRules.append(rule);
}

void Higlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
         QRegExp expression(rule.pattern);
         int index = expression.indexIn(text);
         while (index >= 0) {
             int length = expression.matchedLength();
             setFormat(index, length, rule.format);
             index = expression.indexIn(text, index + length);
         }
     }
}
