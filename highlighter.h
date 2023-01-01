#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>

class Highlighter : public QSyntaxHighlighter {

    Q_OBJECT

public:

    Highlighter(QTextDocument* parent = nullptr);

protected:

    void highlightBlock(const QString &text) override;

private:

    struct HighlightingRule {

        QRegularExpression pattern;
        QTextCharFormat format;

    };

    QList<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat signsFormat;

};

#endif // HIGHLIGHTER_H
