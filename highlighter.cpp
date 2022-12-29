#include "highlighter.h"

Highlighter::Highlighter(QTextDocument* parent) : QSyntaxHighlighter(parent) {

    HighlightingRule rule;

    keywordFormat.setForeground(QColor("#559bd4"));
    const QString keywordPatterns[] = {

        QStringLiteral("\\balignas\\b"), QStringLiteral("\\balignof\\b"), QStringLiteral("\\band\\b"),
        QStringLiteral("\\band_eq\\b"), QStringLiteral("\\basm\\b"), QStringLiteral("\\bauto\\b"),
        QStringLiteral("\\bbitand\b"), QStringLiteral("\\bbitor\\b"), QStringLiteral("\\bbool\\b"),
        QStringLiteral("\\bbreak\\b"), QStringLiteral("\\bcase\\b"), QStringLiteral("\\bcatch\\b"),
        QStringLiteral("\\bchar\\b"), QStringLiteral("\\bchar\\b"), QStringLiteral("\\bchar16_t\\b"),
        QStringLiteral("\\bchar32_t\\b"), QStringLiteral("\\bclass\\b"), QStringLiteral("\\bcompl\\b"),
        QStringLiteral("\\bconst\\b"), QStringLiteral("\\bconstexpr\\b"), QStringLiteral("\\bconst_cast\\b"),
        QStringLiteral("\\bcontinue\\b"), QStringLiteral("\\bdecltype\\b"), QStringLiteral("\\bdefault\\b"),
        QStringLiteral("\\bdelete\\b"), QStringLiteral("\\bdo\\b"), QStringLiteral("\\bdouble\\b"),
        QStringLiteral("\\bdynamic_cast\\b"), QStringLiteral("\\belse\\b"), QStringLiteral("\\benum\\b"),
        QStringLiteral("\\bexplicit\\b"), QStringLiteral("\\bexport\\b"), QStringLiteral("\\bextern\\b"),
        QStringLiteral("\\bfalse\\b"), QStringLiteral("\\bfloat\\b"), QStringLiteral("\\bfor\\b"),
        QStringLiteral("\\bfriend\\b"), QStringLiteral("\\bgoto\\b"), QStringLiteral("\\bif\\b"),
        QStringLiteral("\\binline\\b"), QStringLiteral("\\bint\\b"), QStringLiteral("\\blong\\b"),
        QStringLiteral("\\bmutable\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\bnew\\b"),
        QStringLiteral("\\bnoexcept\\b"), QStringLiteral("\\bnot\\b"), QStringLiteral("\\bnot_eq\\b"),
        QStringLiteral("\\bnullptr\\b"), QStringLiteral("\\boperator\\b"), QStringLiteral("\\bor\\b"),
        QStringLiteral("\\bor_eq\\b"), QStringLiteral("\\bprivate\\b"), QStringLiteral("\\bprotected\\b"),
        QStringLiteral("\\bpublic\\b"), QStringLiteral("\\bregister\\b"), QStringLiteral("\\breinterpret_cast\\b"),
        QStringLiteral("\\breturn\\b"), QStringLiteral("\\bshort\\b"), QStringLiteral("\\bsigned\\b"),
        QStringLiteral("\\bsizeof\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstatic_assert\\b"),
        QStringLiteral("\\bstatic_cast\\b"), QStringLiteral("\\bstruct\\b"), QStringLiteral("\\bswitch\\b"),
        QStringLiteral("\\btemplate\\b"), QStringLiteral("\\bthis\\b"), QStringLiteral("\\bthread_local\\b"),
        QStringLiteral("\\bthrow\\b"), QStringLiteral("\\btrue\\b"), QStringLiteral("\\btry\\b"),
        QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypeid\\b"), QStringLiteral("\\btypename\\b"),
        QStringLiteral("\\bunion\\b"), QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\busing\\b"),
        QStringLiteral("\\bvirtual\\b"), QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bvolatile\\b"),
        QStringLiteral("\\bwchar_t\\b"), QStringLiteral("\\bwhile\\b"), QStringLiteral("\\bxor\\b"),
        QStringLiteral("\\bxor_eq\\b")
    };

    for (const QString& pattern : keywordPatterns) {

        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);

    }


    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    // without space
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\{)"));
    rule.format = classFormat;
    highlightingRules.append(rule);
    // with space
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\ {)"));
    rule.format = classFormat;
    highlightingRules.append(rule);



    quotationFormat.setForeground(QColor("#679553"));
    // ""
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    // ''
    rule.pattern = QRegularExpression(QStringLiteral("\'.*\'"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    // < >
    rule.pattern = QRegularExpression(QStringLiteral("\<.*\>"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);


    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);


    singleLineCommentFormat.setForeground(QColor("#c4cace"));
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor("#c4cace"));
    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));

}

void Highlighter::highlightBlock(const QString &text) {

    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {

        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);

        while (matchIterator.hasNext()) {

            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);

        }

    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {

        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;

        if (endIndex == -1) {

            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;

        } else {

            commentLength = endIndex - startIndex + match.capturedLength();

        }

        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);

    }

}
